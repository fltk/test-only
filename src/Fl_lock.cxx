/*! \defgroup multithreading Multithreaded FLTK Applications

  Fltk has no multithreaded support unless the main thread calls
  fltk::lock().

  You must have a "master" thread. Only this thread is allowed to wait
  for events by calling fltk::wait() or fltk::run() or any similar
  call.  From then on fltk will be locked except when the main thread
  is actually waiting for events from the user. Other threads must
  call fltk::lock() and fltk::unlock() to surround \e all calls
  to fltk (such as to change widgets or redraw them).

  FLTK provides the file <fltk/Thread.h> which defines some
  convenience portability wrappers around the native threads
  system. It provides a Thread type and the classes Mutex and
  SignalMutex. <i>This file is optional</i>. Fltk does not use it (it
  has an internal mutex implementation). The header file's only
  purpose is so we can write portable demo programs. It may be useful
  or an inspiration to people who want to try writing multithreaded
  programs themselves, however you can instead use pthreads or any
  other library that is compatable with your system.

  \code
  main() {
    fltk::lock(); // ALWAYS call before any fltk calls
    create_widgets();
    Thread t1; create_thread(t1, func, data1);
    Thread t2; create_thread(t2, func, data2);
    for (;;) {
      fltk::wait();
      check_what_threads_are_up_to();
    }
  }

  fltk::Mutex mutex;

  func1(void* data) {
    mutex.lock();
    do_something();
    mutex.unlock();
    fltk::lock();
    widget->value(foo);
    widget->redraw();
    fltk::awake(); // Without this it may not redraw until next event!
    fltk::unlock();
  }
  \endcode

  Warning: on Windows including <fltk/Thread.h> will cause the
  <windows.h> header file to be included. This nasty file defines
  no end of macros and other things designed to screw up your programs.

*/

/*! \fn void fltk::lock()

  The main thread must call this before the first call to wait() or
  run() to initialize the thread system.  The lock is locked all the
  time except when wait() is waiting for events.

  Child threads should call this method prior to making any fltk
  calls.  Blocks the current thread until it can safely access FLTK
  widgets and data.  Child threads must call fltk::unlock() when they
  are done accessing FLTK. They may want to call fltk::awake() first
  if the display needs to change.

  This is a "recursive lock". If you call fltk::lock() more than once,
  the subsequent calls return immediately. But you must call
  fltk::unlock() the same number of times as you called fltk::lock()
  before the lock is released.
*/

/*! \fn void fltk::unlock()
  Releases the lock that was set using the fltk::lock() method. Child
  threads should call this method as soon as they are finished
  accessing FLTK. If some other thread is waiting for fltk::lock() to
  return, it will get control.
*/

/*! \fn void fltk::awake(void* message)

  A child thread can call this to cause the main thread's call to
  wait() to return (with the lock locked) even if there are no events
  ready. The main purpose of this is to get the main thread to redraw
  the screen, but it will also cause fltk::wait() to return so the
  program's code can do something.

  You should call this immediately before fltk::unlock() for best
  performance.

  The message argument can be retrieved by the other thread using
  fltk::thread_message().
*/

/*! \fn void* fltk::thread_message()

  Returns an argument sent to an awake call, or returns null if none.
  Warning: the current implementation only has a one-entry queue and
  only returns the most recent value!
*/

#include <fltk/run.h>
#include <config.h>

////////////////////////////////////////////////////////////////
#if defined(_WIN32)

#include <windows.h>
#include <process.h>

// these pointers are in win32.cxx:
extern void (*fl_lock_function)();
extern void (*fl_unlock_function)();

static DWORD main_thread;

CRITICAL_SECTION cs;

static void unlock_function() {
  LeaveCriticalSection(&cs);
}

static void lock_function() {
  EnterCriticalSection(&cs);
}

void fltk::lock() {
  if (!main_thread)
    InitializeCriticalSection(&cs);
  lock_function();
  if (!main_thread) {
    fl_lock_function = lock_function;
    fl_unlock_function = unlock_function;
    main_thread = GetCurrentThreadId();
  }
}

void fltk::unlock() {
  unlock_function();
}

extern UINT fl_wake_msg;

//
// 'Fl::awake()' - Let the main thread know an update is pending.
//
// When called from a thread, it causes FLTK to awake from Fl::wait()...
//
void fltk::awake(void* msg) {
  PostThreadMessage( main_thread, fl_wake_msg, (WPARAM)msg, 0);
}

////////////////////////////////////////////////////////////////
#elif HAVE_PTHREAD
#include <unistd.h>
#include <fcntl.h>
#define __USE_GNU // makes the RECURSIVE stuff appear on Linux
#include <pthread.h>

#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
// Linux supports recursive locks, use them directly:

static pthread_mutex_t fltk_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

static void lock_function() {
  pthread_mutex_lock(&fltk_mutex);
}

void fltk::unlock() {
  pthread_mutex_unlock(&fltk_mutex);
}

//Possibly more portable to do this at startup:
//static pthread_mutexattr_t attrib = {PTHREAD_MUTEX_RECURSIVE_NP};
//pthread_mutex_init(&fltk_mutex, &attrib);

#else
// Make a recursive lock out of the pthread mutex:

static pthread_mutex_t fltk_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t fltk_owner;
static int fltk_counter;

static void lock_function() {
  pthread_t self = pthread_self();
  if (!fltk_counter || !pthread_equal(fltk_owner,self)) {
    pthread_mutex_lock(&fltk_mutex);
    //assert(!fltk_owner);
    fltk_owner = self;
    fltk_counter = 1;
  } else {
    //assert(pthread_equal(fltk_owner,self));
    ++fltk_counter;
  }
}

void fltk::unlock() {
  //pthread_t self = pthread_self();
  //assert(fltk_counter > 0 && pthread_equal(fltk_owner,self));
  if (!--fltk_counter) {
    //fltk_owner = 0;
    pthread_mutex_unlock(&fltk_mutex);
  }
}

#endif

static int thread_filedes[2];

// these pointers are in Fl_x.cxx:
extern void (*fl_lock_function)();
extern void (*fl_unlock_function)();

static void* thread_message_;
void* fltk::thread_message() {
  void* r = thread_message_;
  thread_message_ = 0;
  return r;
}

static void thread_awake_cb(int fd, void*) {
  while (read(fd, &thread_message_, sizeof(void*)) > 0);
}

void fltk::lock() {
  lock_function();
  if (!thread_filedes[1]) { // initialize the mt support
    // Init threads communication pipe to let threads awake FLTK from wait
    pipe(thread_filedes);
    fcntl(thread_filedes[0], F_SETFL, O_NONBLOCK);
    add_fd(thread_filedes[0], READ, thread_awake_cb);
    fl_lock_function = lock_function;
    fl_unlock_function = unlock;
  }
}

void fltk::awake(void* msg) {
  write(thread_filedes[1], &msg, sizeof(void*));
}

#endif
