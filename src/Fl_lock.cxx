/* Fl_Lock.cxx

   I would prefer that fltk contain the minimal amount of extra stuff
   for doing threads.  There are other portable thread wrapper libraries
   out there and fltk should not be providing another.  This file
   is an attempt to make minimal additions and make them self-contained
   in this source file.

   Fl::lock() - recursive lock.  Plus you must call this before the
   first call to Fl::wait()/run() to initialize the thread system.
   The lock is locked all the time except when Fl::wait() is waiting
   for events.

   Fl::unlock() - release the recursive lock.

   Fl::awake() - Causes Fl::wait() to return (with the lock locked) even
   if there are no events ready.

*/

#include <FL/Fl.H>
#include "config.h"

#ifdef HAVE_PTHREAD

#include <pthread.h>

#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
// Linux supports recursive locks, use them directly:

static pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

static void _lock() {
  pthread_mutex_lock(&mutex);
}

void Fl::unlock() {
  pthread_mutex_unlock(&mutex);
}

#else
// Make a recursive lock out of the pthread mutex:

static pthread_mutex_t = PTHREAD_MUTEX_INITIALIZER;
static pthread_t owner;
static int counter = 0;

static void lock() {
  if (!counter || owner != pthread_self()) {
    pthread_mutex_lock(&mutex); owner = pthread_self();
  }
  counter++;
}

void Fl::unlock() {
  if (!--counter) pthread_mutex_unlock(&mutex);
}

#endif

static int thread_filedes[2];

// these pointers are in Fl_x.cxx:
extern void (*fl_lock_function)();
extern void (*fl_unlock_function)();

static void thread_awake_cb(int fd, void*) {
  void* junk[1];
  read(fd, junk, sizeof(void*));
}

void Fl::lock() {
  if (!thread_filedes[1]) { // initialize the mt support
    // Init threads communication pipe to let threads awake FLTK from wait
    pipe(thread_filedes);
    Fl::add_fd(thread_filedes[0], FL_READ, thread_awake_cb);
    fl_lock_function = _lock;
    fl_unlock_function = unlock;
  }
  _lock();
}

void Fl::awake()
{
  void* junk[1];
  junk[0] = 0;
  write(thread_filedes[1], junk, sizeof(void*));
}

////////////////////////////////////////////////////////////////
#elif defined(WIN32)

#include <windows.h>
#include <process.h>

static CRITICAL_SECTION cs;

// these pointers are in Fl_win32.cxx:
extern void (*fl_lock_function)();
extern void (*fl_unlock_function)();

void Fl::lock() {
  fl_lock_function = lock;
  fl_unlock_function = unlock;
  EnterCriticalSection(&cs);
}

void Fl::unlock() {
  LeaveCriticalSection(&cs);
}

void Fl::awake() {
  // sndMessage?  I dunno...
}

#endif

// If threads are not supported the functions are undefined, thus
// linking of programs that call them is impossible.


