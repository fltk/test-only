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
#include <FL/Fl_Threads.H>
#include "config.h"

#if HAVE_PTHREAD || defined(WIN32)
static Fl_Mutex main_lock(true);

void* Fl::thread_message;

void Fl::lock() {
  main_lock.lock();
}

void Fl::unlock() {
  main_lock.unlock();
}
#endif

#if HAVE_PTHREAD
#include <unistd.h>
#include <pthread.h>

#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
// Linux supports recursive locks, use them directly:

#define INITIALISER PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP

void Fl_Mutex::lock() {
  pthread_mutex_lock((pthread_mutex_t*)m);
}

void Fl_Mutex::unlock() {
  pthread_mutex_unlock((pthread_mutex_t*)m);
}

#else
// Make a recursive lock out of the pthread mutex:

#define INITIALISER PTHREAD_MUTEX_INITIALIZER

void Fl_Mutex::lock() {
  if (!counter || (pthread_t)owner != (pthread_t)pthread_self()) {
    pthread_mutex_lock((pthread_mutex_t*)m); owner = pthread_self();
  }
  counter++;
}

void Fl_Mutex::unlock() {
  if (!--counter) pthread_mutex_unlock((pthread_mutex_t*)m);
}

#endif

static int thread_filedes[2];

// these pointers are in Fl_x.cxx:
extern void (*fl_lock_function)();
extern void (*fl_unlock_function)();

static void thread_awake_cb(int fd, void*) {
  read(fd, &Fl::thread_message, sizeof(void*));
}

Fl_Mutex::Fl_Mutex(bool inistate) {
  if (!thread_filedes[1]) { // initialize the mt support
    // Init threads communication pipe to let threads awake FLTK from wait
    pipe(thread_filedes);
    Fl::add_fd(thread_filedes[0], FL_READ, thread_awake_cb);
    fl_lock_function = Fl::lock;
    fl_unlock_function = Fl::unlock;
  }
#ifndef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
  counter = 0;
#endif
  pthread_mutex_t tmp = INITIALISER;
  m = (pthread_mutex_t*) new pthread_mutex_t(tmp);
  if (inistate) lock();
}

Fl_Mutex::~Fl_Mutex()
{
  delete (pthread_mutex_t*) m;
}

void Fl::awake(void* msg)
{
  write(thread_filedes[1], &msg, sizeof(void*));
}

int fl_create_thread(Fl_Thread& t, void *(*f) (void *), void* p)
{
  return pthread_create((pthread_t*)&t, 0, f, p);
}

////////////////////////////////////////////////////////////////
#elif defined(WIN32)

#include <windows.h>
#include <process.h>

// these pointers are in Fl_win32.cxx:
extern void (*fl_lock_function)();
extern void (*fl_unlock_function)();

Fl_Mutex::Fl_Mutex(bool inistate)
{
  if (!fl_lock_function) {
    fl_lock_function = lock;
    fl_unlock_function = unlock;
  }
  m = (void*) new CRITICAL_SECTION;
  if (inistate) lock();
}

Fl_Mutex::~Fl_Mutex()
{
  delete (CRITICAL_SECTION*) m;
}

void Fl_Mutex::lock() {
  EnterCriticalSection((CRITICAL_SECTION*)m);
}

void Fl_Mutex::unlock() {
  LeaveCriticalSection((CRITICAL_SECTION*)m);
}

/*void Fl::awake() {
  // sndMessage?  I dunno...
}*/
// when called from a thread, it causes FLTK to awake from Fl::wait()
int Fl::awake(void* msg)
{
  return PostThreadMessage( main_thread, WM_USER, (WPARAM)msg, 0)? 0 : -1;
}

int fl_create_thread(Fl_Thread& t, void *(*f) (void *), void* p)
{
  return t = (Fl_Thread)_beginthread((void( __cdecl * )( void * ))f, 0, p);
}

#endif


// If threads are not supported the functions are undefined, thus
// linking of programs that call them is impossible.
