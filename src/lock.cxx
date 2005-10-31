/*! \defgroup multithreading Multithreaded FLTK Applications

  Fltk has no multithreaded support unless the main thread calls
  fltk::lock(). It should call this very early, such as the first
  line in main(). After that parallel threads should call fltk::lock()
  and fltk::unlock() around any fltk calls.

  The "main" thread is the one that calls fltk::wait(). It should call
  fltk::lock() immediately and then never call fltk::unlock().  Though
  it appears that fltk is then locked all the time, it is in fact
  unlocked while fltk::wait() is waiting for events, which is really
  the majority of the time.

  If non-main threads cause any fltk::Widget::redraw() calls, they
  should call fltk::awake() just before fltk::unlock(). Otherwise the
  drawing may not be done until the next event.

  Non-main threads cannot call all fltk functions. In particular any
  functions that wait for events (including fltk::Window::exec() and
  fltk::ask()) do not work. On Windows fltk::Window::show() does not
  work either. It it likely we will be fixing these in the future.  To
  make these broken calls you will have to store the fact that you
  want them called in static memory locations, then call fltk::awake()
  to make the main thread return, and have it check the static
  locations and do the calls.

  FLTK provides the file <fltk/Threads.h> which defines some
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

  Warning: on Windows including <fltk/Threads.h> will cause the
  <windows.h> header file to be included. This file often has
  undesirable effects and should be avoided if possible.

*/

/*! \fn void fltk::lock()

  A multi-threaded fltk program must surround all calls to any fltk
  functions with lock() and unlock() pairs. This is a "recursive
  lock", a thread can call lock() n times, and it must call
  unlock() n times before it really is unlocked.

  If another thread calls lock() while it is locked, it will block
  (not return from lock()) until the first thread unlocks.

  The main thread must call lock() once before \e any call to fltk to
  initialize the thread system.

  The X11 version of fltk uses XInitThreads(), XLockDisplay(), and
  XUnlockDisplay(). This should allow an fltk program to cooperate
  with other packages updating the display using Xlib calls.
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

/*! \fn bool fltk::in_main_thread()
  Returns true if the current thread is the main thread, i.e. the one
  that called wait() first. Many fltk calls such as wait() will not
  work correctly if this is not true. Notice that this function must
  be surrounded by lock() and unlock() just like all other fltk
  functions.
*/

#include <fltk/run.h>
#include <config.h>

// Define the mutex-init value needed by fltk::RecursiveMutex:
#if HAVE_PTHREAD && !defined(_WIN32)
# ifndef __USE_GNU
#  define __USE_GNU // makes the RECURSIVE stuff appear on Linux
# endif
#include <fltk/Threads.h>

# if defined(PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP)
static pthread_mutexattr_t recursive_attrib={PTHREAD_MUTEX_RECURSIVE_NP};
fltk::RecursiveMutex::RecursiveMutex() : Mutex(&recursive_attrib) {}
# elif defined(PTHREAD_MUTEX_RECURSIVE)
static pthread_mutexattr_t* recursive_attrib() {
  static pthread_mutexattr_t a;
  static bool beenhere;
  if (!beenhere) {
    pthread_mutexattr_init(&a);
    pthread_mutexattr_settype(&a,PTHREAD_MUTEX_RECURSIVE);
    beenhere=true;
  }
  return &a;
}
fltk::RecursiveMutex::RecursiveMutex() : Mutex(recursive_attrib()) {}
# endif
#endif

#if USE_X11 && USE_X11_MULTITHREADING

// This is NOT normally done, instead the HAVE_PTHREAD case is done
# include "x11/lock.cxx"

#elif defined(_WIN32)

# include "win32/lock.cxx"

#elif HAVE_PTHREAD

// Use our RecursiveLock for lock/unlock, and a pipe for awake():

#include <unistd.h>
#include <fcntl.h>

extern void (*fl_lock_function)();
extern void (*fl_unlock_function)();
static void init_function();
static void (*init_or_lock_function)() = init_function;

static fltk::RecursiveMutex fltkmutex;

static void lock_function() {fltkmutex.lock();}
static void unlock_function() {fltkmutex.unlock();}

static void* thread_message_;
static void thread_awake_cb(int fd, void*) {
  while (read(fd, &thread_message_, sizeof(void*)) > 0);
}
static int thread_filedes[2];

static void init_function() {
  // Init threads communication pipe to let threads awake FLTK from wait
  pipe(thread_filedes);
  fcntl(thread_filedes[0], F_SETFL, O_NONBLOCK);
  fltk::add_fd(thread_filedes[0], fltk::READ, thread_awake_cb);
  fl_lock_function = init_or_lock_function = lock_function;
  fl_unlock_function = unlock_function;
  lock_function();
}

void fltk::lock() {init_or_lock_function();}

void fltk::unlock() {fl_unlock_function();}

void fltk::awake(void* msg) {
  if (!in_main_thread())
    write(thread_filedes[1], &msg, sizeof(void*));
}

void* fltk::thread_message() {
  void* r = thread_message_;
  thread_message_ = 0;
  return r;
}

#else

// If no lock is supported, the fltk::lock() and similar functions are
// missing.

#endif

// end of lock.cxx
