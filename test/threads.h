/
// "$Id$
/
// Simple threading API for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2003 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

// Inline classes to provide portable support for threads and mutexes
/
// FLTK does not use this (it has an internal mutex implementatio
// that is used if Fl::lock() is called). This header file's onl
// purpose is so we can write portable demo programs. It may be usefu
// or an inspiration to people who want to try writing multithreade
// programs themselves
/
// FLTK has no multithreaded support unless the main thread calls Fl::lock()
// This main thread is the only thread allowed to call Fl::run() or Fl::wait()
// From then on FLTK will be locked except when the main thread is actuall
// waiting for events from the user. Other threads must call Fl::lock() an
// Fl::unlock() to surround calls to FLTK (such as to change widgets o
// redraw them)

#ifndef Threads_
#  define Threads_

#  if HAVE_PTHREAD_
// Use POSIX threading..

#    include <pthread.h

typedef pthread_t Fl_Thread

static int fl_create_thread(Fl_Thread& t, void *(*f) (void *), void* p

  return pthread_create((pthread_t*)&t, 0, f, p)


#  elif defined(WIN32) && !defined(__WATCOMC__) // Use Windows threading..

#    include <windows.h
#    include <process.h

typedef unsigned long Fl_Thread

static int fl_create_thread(Fl_Thread& t, void *(*f) (void *), void* p) 
  return t = (Fl_Thread)_beginthread((void( __cdecl * )( void * ))f, 0, p)



#  elif defined(__WATCOMC__
#    include <process.h

typedef unsigned long Fl_Thread

static int fl_create_thread(Fl_Thread& t, void *(*f) (void *), void* p) 
  return t = (Fl_Thread)_beginthread((void(* )( void * ))f, 32000, p)


#  endif // !HAVE_PTHREAD_
#endif // !Threads_

/
// End of "$Id$"
/
