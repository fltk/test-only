// Inline classes to provide portable support for threads and mutexes.
//
// Fltk does not use this (it has an internal mutex implementation
// that is used if Fl::lock() is called). This header file's only
// purpose is so we can write portable demo programs. It may be useful
// or an inspiration to people who want to try writing multithreaded
// programs themselves.
//
// Fltk has no multithreaded support unless the main thread calls Fl::lock().
// This main thread is the only thread allowed to call Fl::run or Fl::wait.
// From then on fltk will be locked except when the main thread is actually
// waiting for events from the user. Other threads must call Fl::lock() and
// Fl::unlock() to surround calls to fltk (such as to change widgets or
// redraw them).

#ifndef fltk_Threads_h
#define fltk_Threads_h

#ifndef _WIN32
// pthreads:

#include <pthread.h>

namespace fltk {

typedef pthread_t Thread;

static int create_thread(Thread& t, void *(*f) (void *), void* p)
{
  return pthread_create((pthread_t*)&t, 0, f, p);
}

// Linux supports recursive locks, use them directly, with some cheating:
#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP

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

static int create_thread(Thread& t, void *(*f) (void *), void* p) {
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
    WaitForSingleObject(event, INFINITE);
    EnterCriticalSection(&cs);
    // cs.count = save_counter;
  }
};

}

#endif
#endif
