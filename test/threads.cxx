//
// "$Id: threads.cxx,v 1.13 2002/12/13 19:42:59 easysw Exp $"
//
// Threading example program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2001 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <config.h>

#if HAVE_PTHREAD || defined(WIN32)
#  include <fltk/Window.h>
#  include <fltk/Browser.h>
#  include <fltk/ValueInput.h>
#  include <fltk/Threads.h>
#  include <fltk/run.h>
#  include <stdio.h>
#  include <math.h>

fltk::Thread prime_thread;

fltk::Browser *browser1, *browser2;
fltk::ValueInput *value1, *value2;
int start2 = 3;

void* prime_func(void* p)
{
  fltk::Browser* browser = (fltk::Browser*) p;
  fltk::ValueInput *value;
  int n;
  int step;

  if (browser == browser2) {
    n      = start2;
    start2 += 2;
    step   = 12;
    value  = value2;
  } else {
    n     = 3;
    step  = 2;
    value = value1;
  }

  // very simple prime number calculator !
  for (; ; n+= step) {
    int p;
    int hn = (int)sqrt(n);
    for (p=3; p<=hn; p+=2) if ( n%p == 0 ) break;
    if (p >= hn) {
      char s[128];
      sprintf(s, "%d", n);
      fltk::lock();
      browser->add(s);
      browser->bottomline(browser->size());
      if (n > value->value()) value->value(n);
      fltk::unlock();
      fltk::awake((void*) (browser == browser1? p:0));	// Cause the browser to redraw ...
    }
  }
  return 0;
}

int main()
{
  fltk::Window* w = new fltk::Window(200, 200, "Single Thread");
  w->begin();
  browser1 = new fltk::Browser(0, 0, 200, 175);
  w->resizable(browser1);
  value1 = new fltk::ValueInput(100, 175, 200, 25, "Max Prime:");
  w->end();
  w->show();
  w = new fltk::Window(200, 200, "Six Threads");
  w->begin();
  browser2 = new fltk::Browser(0, 0, 200, 175);
  w->resizable(browser2);
  value2 = new fltk::ValueInput(100, 175, 200, 25, "Max Prime:");
  w->end();
  w->show();
  
  browser1->add("Prime numbers:");
  browser2->add("Prime numbers:");

  fltk::lock(); // you must do this before creating any threads!

  // One thread displaying in one browser
  fltk::create_thread(prime_thread, prime_func, browser1);
  // Several threads displaying in another browser
  fltk::create_thread(prime_thread, prime_func, browser2);
  fltk::create_thread(prime_thread, prime_func, browser2);
  fltk::create_thread(prime_thread, prime_func, browser2);
  fltk::create_thread(prime_thread, prime_func, browser2);
  fltk::create_thread(prime_thread, prime_func, browser2);
  fltk::create_thread(prime_thread, prime_func, browser2);

  //  fltk::run();
  while (w->visible()) {
    fltk::wait();
//    void* m = fltk::thread_message();
//    printf("Received message: %p\n", m);
  }

  return 0;
}
#else
#  include <fltk/ask.h>

int main() {
  fltk::alert("Sorry, threading not supported on this platform!");
}
#endif // HAVE_PTHREAD || WIN32


//
// End of "$Id: threads.cxx,v 1.13 2002/12/13 19:42:59 easysw Exp $".
//
