// Simple threads support functions.  These should not really be part of
// fltk, but without them we cannot write portable demo programs.  These
// should be useful for other programs.

// You must not include this file on systems that don't support either
// pthreads or WIN32 threads.  This is because all functions are defined
// inline here for maximum speed.

// You MUST call Fl::lock() before calling create_thread!  This is
// totally necessary, as calling Fl::unlock() without Fl::lock() having
// been called first will crash!  Fl::lock/unlock uses it's own internal
// implementation of mutex.

#ifndef Fl_Threads_H
#define Fl_Threads_H

#ifndef WIN32
// pthreads:

#include <pthread.h>

typedef pthread_t Fl_Thread;

static int fl_create_thread(Fl_Thread& t, void *(*f) (void *), void* p)
{
  return pthread_create((pthread_t*)&t, 0, f, p);
}

// Linux supports recursive locks, use them directly, with some cheating:
#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP

extern pthread_mutexattr_t Fl_Mutex_attrib;

class Fl_Mutex {
  friend class Fl_SignalMutex;
  pthread_mutex_t mutex;
  Fl_Mutex(const Fl_Mutex&);
  Fl_Mutex& operator=(const Fl_Mutex&);
public:
  Fl_Mutex() {pthread_mutex_init(&mutex, &Fl_Mutex_attrib);}
  void lock() {pthread_mutex_lock(&mutex);}
  void unlock() {pthread_mutex_unlock(&mutex);}
  ~Fl_Mutex() {pthread_mutex_destroy(&mutex);}
};

class Fl_SignalMutex : public Fl_Mutex {
  pthread_cond_t cond;
public:
  Fl_SignalMutex() : Fl_Mutex() {pthread_cond_init(&cond, 0);}
  void signal() {pthread_cond_broadcast(&cond);}
  void wait() {
    int save_counter = mutex.m_count; mutex.m_count = 1;
    pthread_cond_wait(&cond, &mutex);
    mutex.m_count = save_counter;
  }
};

#else // standard pthread mutexes need a bit of work to be recursive:

class Fl_Mutex {
  friend class Fl_SignalMutex;
  pthread_mutex_t mutex;
  pthread_t owner;
  int counter;
  Fl_Mutex(const Fl_Mutex&);
  Fl_Mutex& operator=(const Fl_Mutex&);
public:
  Fl_Mutex() : counter(0) {pthread_mutex_init(&mutex, 0);}
  void lock() {
    if (!counter || owner != pthread_self()) {
      pthread_mutex_lock(&mutex); owner = pthread_self();
    }
    counter++;
  }
  void unlock() {if (!--counter) pthread_mutex_unlock(&mutex);}
  ~Fl_Mutex() {pthread_mutex_destroy(&mutex);}
};

class Fl_SignalMutex : public Fl_Mutex {
  pthread_cond_t cond;
public:
  Fl_SignalMutex() : Fl_Mutex() {pthread_cond_init(&cond, 0);}
  void signal() {pthread_cond_broadcast(&cond);}
  void wait() {
    int save_counter = counter; counter = 0;
    pthread_cond_wait(&cond, &mutex);
    counter = save_counter;
    owner = pthread_self();
  }
};

#endif

#else
// WIN32:

#include <windows.h>
#include <process.h>

typedef unsigned long Fl_Thread;

static int fl_create_thread(Fl_Thread& t, void *(*f) (void *), void* p) {
  return t = (Fl_Thread)_beginthread((void( __cdecl * )( void * ))f, 0, p);
}

class Fl_Mutex {
  friend class Fl_SignalMutex;
  CRITICAL_SECTION cs;
  Fl_Mutex(const Fl_Mutex&);
  Fl_Mutex& operator=(const Fl_Mutex&);
public:
  Fl_Mutex() {InitializeCriticalSection(&cs);}
  void lock() {EnterCriticalSection(&cs);}
  void unlock() {LeaveCriticalSection(&cs);}
  ~Fl_Mutex() {DeleteCriticalSection(&cs);}
};

class Fl_SignalMutex : public Fl_Mutex {
  HANDLE event;
public:
  Fl_SignalMutex() : Fl_Mutex() {event = CreateEvent(0, FALSE, FALSE, 0);}
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

#endif
#endif
