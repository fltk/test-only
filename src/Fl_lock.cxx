/* Lock.cxx

   I would prefer that fltk contain the minimal amount of extra stuff
   for doing threads.  There are other portable thread wrapper libraries
   out there and fltk should not be providing another.  This file
   is an attempt to make minimal additions and make them self-contained
   in this source file.

   lock() - recursive lock.  Plus you must call this before the
   first call to wait()/run() to initialize the thread system.
   The lock is locked all the time except when wait() is waiting
   for events.

   unlock() - release the recursive lock.

   awake(void*) - Causes wait() to return (with the lock locked)
   even if there are no events ready.

   thread_message() - returns an argument sent to an awake call,
   or returns null if none.  Warning: the current implementation only
   has a one-entry queue and only returns the most recent value!

   See also the Threads.h header file, which provides convienence
   functions so you can create your own threads and mutexes.
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
