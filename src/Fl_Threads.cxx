//
// "$Id: Fl_Threads.cxx,v 1.5 1999/10/26 15:43:11 mike Exp $"
//
// Threads support Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <FL/Fl.H>
#include "config.h"


int Fl::thread_filedes[2];
void* Fl::thread_message;
Fl_Mutex Fl::main_lock;

//
// POSIX pthread
//
#ifdef HAVE_PTHREAD

#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Initialise FLTK thread support (mainly, initialise Fl::main_mutex)
// Called automatically in Fl::create_thread too ...

#if !defined(PRI_OTHER_MAX)
#if HAVE_SCHED_GET_PRIORITY_MAX
#include <sched.h>

int PRI_OTHER_MIN;
int PRI_OTHER_MAX;
#else
#define PRI_OTHER_MIN 1
#define PRI_OTHER_MAX 32
#endif
#endif

int Fl::init_threads_support()
{
  if (Fl::main_lock == 0) {
    Fl::mutex_init(Fl::main_lock);
    Fl::lock(Fl::main_lock);
#ifndef PRI_OTHER_MAX
    PRI_OTHER_MIN = sched_get_priority_min(SCHED_OTHER);
    PRI_OTHER_MAX = sched_get_priority_max(SCHED_OTHER);
#endif
  }
  return 0;
}

// create a new thread 
// 'a': where to put the handler of the new thread
// 'b': starting execution address
// 'c': one parameter to the function
int Fl::create_thread(Fl_Thread& t, void *(*f) (void *), void* p)
{
  init_threads_support();
  return pthread_create((pthread_t*)&t, 0, f, p);
}

// set the priority of a thread. default value 0.5 for normal priority, 
// 1 for highest, 0 for lowest.

int Fl::set_thread_priority(Fl_Thread t, float pri)
{
#if HAVE_PTHREAD_GETSCHEDPARAM
  struct sched_param sp;
  int policy;
  if (pthread_getschedparam(t, &policy, &sp)) return -1;
  sp.sched_priority = int(PRI_OTHER_MIN + 
			  (PRI_OTHER_MAX - PRI_OTHER_MIN) * pri);
  return pthread_setschedparam(t, policy, &sp);
#else
  return -1;
#endif // HAVE_PTHREAD_GETSCHEDPARAM
}

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
// sleep for 'a' seconds
void Fl::sleep(double time)
{
#if HAVE_USLEEP
  usleep(int(time * 1000000));
#else
  timeval timeout;

  timeout.tv_sec = int(time);
  timeout.tv_usec = int(1000000 * (time-timeout.tv_sec));

  select(0,0, 0, 0, &timeout);
#endif // HAVE_USLEEP
}

// create a mutex
int Fl::mutex_init(Fl_Mutex& a)
{
  pthread_mutex_t* m = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  a = (void*)m;
  return pthread_mutex_init(m, 0);
}

// destroy a mutex
int Fl::mutex_destroy(Fl_Mutex a)
{
  int r = pthread_mutex_destroy((pthread_mutex_t*)a);
  free((void*)a);
  return r;
}

// lock/unlock a mutex
int Fl::lock(Fl_Mutex a)
{
  return pthread_mutex_lock((pthread_mutex_t*)a);
}
int Fl::unlock(Fl_Mutex a)
{
  return pthread_mutex_unlock((pthread_mutex_t*)a);
}

// when called from a thread, it causes FLTK to awake from Fl::wait()
int Fl::awake(void* msg)
{
  return write(Fl::thread_filedes[1], &msg, sizeof(void*));
}


//
// WIN32 threads
//
#elif defined(WIN32)

#include "windows.h"
#include "winbase.h"

#ifndef _MT
#define _MT
#endif
#ifdef _POSIX_
#undef _POSIX_
#endif
#include <process.h>

static DWORD main_thread;

// Initialise FLTK thread support (mainly, initialise Fl::main_mutex)
// Called automatically in Fl::create_thread too ...
int Fl::init_threads_support()
{
  if (Fl::mutex == 0) {
    Fl::mutex_init(Fl::mutex);
    Fl::lock(Fl::mutex);
    main_thread = GetCurrentThreadId();
  }
  return 0;
}

// create a new thread 
// 'a': where to put the handler of the new thread
// 'b': starting execution address
// 'c': one parameter to the function
int Fl::create_thread(Fl_Thread& t, void *(*f) (void *), void* p)
{
  init_threads_support();
  return t = (Fl_Thread)_beginthread((void( __cdecl * )( void * ))f, 0, p);
}

// set the priority of a thread. default value 0.5 for normal priority, 
// 1 for highest, 0 for lowest.
int Fl::set_thread_priority(Fl_Thread t, float pri)
{
  return SetThreadPriority(t, int(THREAD_PRIORITY_LOWEST + 
	(THREAD_PRIORITY_HIGHEST - THREAD_PRIORITY_LOWEST) * pri));
}

// sleep for 'a' seconds
void Fl::sleep(double a)
{
  Sleep(int(a*1000));
}

// create a mutex
int Fl::mutex_init(Fl_Mutex& mutex)
{
   return (mutex = (Fl_Mutex)CreateMutex(NULL, FALSE, NULL)) != 0;
}

// destroy a mutex
int Fl::mutex_destroy(Fl_Mutex a)
{
  return CloseHandle((HANDLE)a);
}

// lock/unlock a mutex
int Fl::lock(Fl_Mutex a)
{
  return WaitForSingleObject((HANDLE)a, INFINITE);
}
int Fl::unlock(Fl_Mutex a)
{
  return ReleaseMutex((HANDLE)a);
}

// when called from a thread, it causes FLTK to awake from Fl::wait()
int Fl::awake(void* msg)
{
  return PostThreadMessage( main_thread, WM_USER, (WPARAM)msg, 0)? 0 : -1;
}

//
// no thread support ...
//
#else


// Initialise FLTK thread support (mainly, initialise Fl::main_mutex)
// Called automatically in Fl::create_thread too ...
int Fl::init_threads_support()
{
  return -1;
}

// create a new thread 
// 'a': where to put the handler of the new thread
// 'b': starting execution address
// 'c': one parameter to the function
int Fl::create_thread(Fl_Thread& t, void *(*f) (void *), void* p)
{
  return -1;
}

// set the priority of a thread. default value 0.5 for normal priority, 
// 1 for highest, 0 for lowest.
int Fl::set_thread_priority(Fl_Thread, float )
{
  return -1;
}

// sleep for 'a' seconds
void Fl::sleep(double a)
{
}

// create a mutex
int Fl::mutex_init(Fl_Mutex&)
{
  return -1;
}

// destroy a mutex
int Fl::mutex_destroy(Fl_Mutex)
{
  return -1;
}

// lock/unlock a mutex
int Fl::lock(Fl_Mutex)
{
  return -1;
}
int Fl::unlock(Fl_Mutex)
{
  return -1;
}

// when called from a thread, it causes FLTK to awake from Fl::wait()
int Fl::awake(void* msg)
{
   return -1;
}

#endif

//
// End of "$Id: Fl_Threads.cxx,v 1.5 1999/10/26 15:43:11 mike Exp $".
//
