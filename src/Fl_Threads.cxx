//
// "$Id: Fl_Threads.cxx,v 1.1 1999/10/20 23:22:21 vincent Exp $"
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

int 	 Fl::thread_filedes[2];
void* 	 Fl::thread_message;
Fl_Mutex Fl::mutex;

//
// POSIX pthread
//
#ifdef HAVE_PTHREAD

#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

// create a new thread 
// 'a': where to put the handler of the new thread
// 'b': starting execution address
// 'c': one parameter to the function
int Fl::create_thread(Fl_Thread& t, void *(*f) (void *), void* p)
{
  if (Fl::mutex == 0) {
    Fl::mutex_init(Fl::mutex);
    Fl::lock(Fl::mutex);
  }
  return pthread_create((pthread_t*)&t, 0, f, p);
}

// set the priority of a thread. default value 0.5 for normal priority, 
// 1 for highest, 0 for lowest.
int Fl::set_thread_priority(Fl_Thread, float)
{
  // not implemented yet ...
  return -1;
}

// sleep for 'a' milliseconds
void Fl::sleep(int a)
{
  ::sleep(a==0 || a>500? int((a+500)/1000.) : 1);	// To be improved !!
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
  pthread_mutex_lock((pthread_mutex_t*)a);
}
int Fl::unlock(Fl_Mutex a)
{
  pthread_mutex_unlock((pthread_mutex_t*)a);
}

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

// create a new thread 
// 'a': where to put the handler of the new thread
// 'b': starting execution address
// 'c': one parameter to the function
int Fl::create_thread(Fl_Thread& t, void *(*f) (void *), void* p)
{
  DWORD dummy;
  return (t = (Fl_Thread)CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)f, p, 0, &dummy)) != 0;
}

// set the priority of a thread. default value 0.5 for normal priority, 
// 1 for highest, 0 for lowest.
int Fl::set_thread_priority(Fl_Thread, float)
{
/*   return SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS) &&
          SetThreadPriority(thread, THREAD_PRIORITY_TIME_CRITICAL);*/
  return -1;
}

// sleep for 'a' milliseconds
void Fl::sleep(int a)
{
  Sleep(a);
}

// create a mutex
int Fl::mutex_init(Fl_Mutex&)
{
   return (ptmutex = (Fl_Mutex)CreateMutex(NULL, FALSE, NULL)) != 0;
}

// destroy a mutex
int Fl::mutex_destroy(Fl_Mutex a)
{
  return CloseHandle((HANDLE)a);
}

// lock/unlock a mutex
int Fl::lock(Fl_Mutex)
{
  return WaitForSingleObject((HANDLE)a, INFINITE)
}
int Fl::unlock(Fl_Mutex)
{
  return ReleaseMutex((HANDLE)a);
}


//
// no thread support ...
//
#else


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

// sleep for 'a' milliseconds
void Fl::sleep(int a)
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

#endif

//
// End of "$Id: Fl_Threads.cxx,v 1.1 1999/10/20 23:22:21 vincent Exp $".
//
