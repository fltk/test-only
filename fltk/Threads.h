// Inline classes to provide portable support for threads and mutexes.
//

#ifndef fltk_Threads_h
#define fltk_Threads_h

#ifndef _WIN32
// pthreads:

#include <pthread.h>

namespace fltk {
/*! \addtogroup multithreading
  \{ */

/*! Hides whatever the system uses to identify a thread. Used so
  the "toy" interface is portable. */
typedef pthread_t Thread;

/*! Fork a new thread and make it run \a f(p). Returns negative number
  on error, otherwise \a t is set to the new thread. */
int create_thread(Thread& t, void *(*f) (void *), void* p)
{
  return pthread_create((pthread_t*)&t, 0, f, p);
}

// Linux supports recursive locks, use them directly, with some cheating:
#if defined(PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP) && !defined(DOXYGEN)

extern pthread_mutexattr_t Mutex_attrib;

class Mutex {
  friend class SignalMutex;
  pthread_mutex_t mutex;
  Mutex(const Mutex&);
  Mutex& operator=(const Mutex&);
public:
  Mutex() {pthread_mutex_init(&mutex, &Mutex_attrib);}
  void lock() {pthread_mutex_lock(&mutex);}
  void unlock() {pthread_mutex_unlock(&mutex);}
  ~Mutex() {pthread_mutex_destroy(&mutex);}
};

class SignalMutex : public Mutex {
  pthread_cond_t cond;
public:
  SignalMutex() : Mutex() {pthread_cond_init(&cond, 0);}
  void signal() {pthread_cond_broadcast(&cond);}
  void wait() {pthread_cond_wait(&cond, &mutex);}
};

#else // standard pthread mutexes need a bit of work to be recursive:

/*!
  Simple wrapper for the locking provided by the operating system
  so you can write portable toy or demo programs. This is a "recursive
  lock". Calling lock() will wait until nobody else has the lock and then
  will take it. Calling lock() N times means you must call unlock()
  N times before another thread can get it.
*/
class Mutex {
  friend class SignalMutex;
  pthread_mutex_t mutex;
  pthread_t owner;
  int counter;
  Mutex(const Mutex&);
  Mutex& operator=(const Mutex&);
public:
  Mutex() : counter(0) {pthread_mutex_init(&mutex, 0);}
  void lock() {
    if (!counter || owner != pthread_self()) {
      pthread_mutex_lock(&mutex); owner = pthread_self();
    }
    counter++;
  }
  void unlock() {if (!--counter) pthread_mutex_unlock(&mutex);}
  ~Mutex() {pthread_mutex_destroy(&mutex);}
};

/*!
  Proper semaphore with good behavior, useful for portable toy or
  demo programs. Besides a lock, this has a method of pausing until
  another thread completes an action. You \e must lock() this before
  you call wait(). wait() will unlock it, wait for a signal() from
  another thread, and then lock() it again. You must unlock() it
  some time after wait() returns. You can call signal() at any time,
  though usually you will have called lock() and then unlock() this
  afterwards.

  Warning: wait() may return even if nobody else did signal()! This is
  unavoidable. In particular, the WIN32 api is severely broken and
  cannot correctly implement this.  It can easily lock up and never
  return, so this always puts a timeout of .5 second to get around
  this.
*/
class SignalMutex : public Mutex {
  pthread_cond_t cond;
public:
  SignalMutex() : Mutex() {pthread_cond_init(&cond, 0);}
  void signal() {pthread_cond_broadcast(&cond);}
  void wait() {
    int save_counter = counter; counter = 0;
    pthread_cond_wait(&cond, &mutex);
    counter = save_counter;
    owner = pthread_self();
  }
};

#endif

}

#else // _WIN32:

#include <windows.h>
#include <process.h>

namespace fltk {

typedef unsigned long Thread;

int create_thread(Thread& t, void *(*f) (void *), void* p) {
  return t = (Thread)_beginthread((void( __cdecl * )( void * ))f, 0, p);
}

class FL_API Mutex {
  friend class SignalMutex;
  CRITICAL_SECTION cs;
  Mutex(const Mutex&);
  Mutex& operator=(const Mutex&);
public:
  Mutex() {InitializeCriticalSection(&cs);}
  void lock() {EnterCriticalSection(&cs);}
  void unlock() {LeaveCriticalSection(&cs);}
  ~Mutex() {DeleteCriticalSection(&cs);}
};

class FL_API SignalMutex : public Mutex {
  HANDLE event;
public:
  SignalMutex() : Mutex() {event = CreateEvent(0, FALSE, FALSE, 0);}
  void signal() {SetEvent(event);}
  void wait() {
    // int save_counter = cs.count; cs.count = 1;
    // the following three calls should be atomic, sigh...
    LeaveCriticalSection(&cs);
    WaitForSingleObject(event, 500 /*INFINITE*/);
    EnterCriticalSection(&cs);
    // cs.count = save_counter;
  }
};

/*! \} */

}

#endif
#endif
