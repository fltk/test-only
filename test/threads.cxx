//
// "$Id$"
//
// Threading example program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <config.h>

#if HAVE_PTHREAD || defined(WIN32)
#  include <fltk3/run.h>
#  include <fltk3/DoubleWindow.h>
#  include <fltk3/Browser.h>
#  include <fltk3/ValueOutput.h>
#  include <fltk3/ask.h>
#  include "threads.h"
#  include <stdio.h>
#  include <math.h>

fltk3::Thread prime_thread;

fltk3::Browser *browser1, *browser2;
fltk3::ValueOutput *value1, *value2;
int start2 = 3;

void magic_number_cb(void *p)
{
  fltk3::ValueOutput *w = (fltk3::ValueOutput*)p;
  w->labelcolor(fltk3::RED);
  w->redraw_label();
}

void* prime_func(void* p)
{
  fltk3::Browser* browser = (fltk3::Browser*) p;
  fltk3::ValueOutput *value;
  int n;
  int step;
  char proud = 0;

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
  //
  // The return at the end of this function can never be reached and thus 
  // will generate a warning with some compilers, however we need to have 
  // a return statement or other compilers will complain there is no return 
  // statement. To avoid warnings on all compilers, we fool the smart ones 
  // into beleiving that there is a chance that we reach the end by testing 
  // n>=0, knowing that logically, n will never be negative in this context.
  if (n>=0) for (;;) {
    int pp;
    int hn = (int)sqrt((double)n);

    for (pp=3; pp<=hn; pp+=2) if ( n%pp == 0 ) break;
    if (pp >= hn) {
      char s[128];
      sprintf(s, "%d", n);

      // Obtain a lock before we access the browser widget...
      fltk3::lock();

      browser->add(s);
      browser->bottomline(browser->size());
      if (n > value->value()) value->value(n);
      n += step;

      // Release the lock...
      fltk3::unlock();

      // Send a message to the main thread, at which point it will
      // process any pending redraws for our browser widget.  The
      // message we pass here isn't used for anything, so we could also
      // just pass NULL.
      fltk3::awake(p);
      if (n>10000 && !proud) {
        proud = 1;
        fltk3::awake(magic_number_cb, value);
      }
    } else {
      // This should not be necessary since "n" and "step" are local variables,
      // however it appears that at least MacOS X has some threading issues
      // that cause semi-random corruption of the (stack) variables.
      fltk3::lock();
      n += step;
      fltk3::unlock();
    }
  }
  return 0L;
}

int main(int argc, char **argv)
{
  fltk3::DoubleWindow* w = new fltk3::DoubleWindow(200, 200, "Single Thread");
  browser1 = new fltk3::Browser(0, 0, 200, 175);
  w->resizable(browser1);
  value1 = new fltk3::ValueOutput(100, 175, 200, 25, "Max Prime:");
  w->end();
  w->show(argc, argv);
  w = new fltk3::DoubleWindow(200, 200, "Six Threads");
  browser2 = new fltk3::Browser(0, 0, 200, 175);
  w->resizable(browser2);
  value2 = new fltk3::ValueOutput(100, 175, 200, 25, "Max Prime:");
  w->end();
  w->show();
  
  browser1->add("Prime numbers:");
  browser2->add("Prime numbers:");

  // Enable multi-thread support by locking from the main
  // thread.  fltk3::wait() and fltk3::run() call fltk3::unlock() and
  // fltk3::lock() as needed to release control to the child threads
  // when it is safe to do so...
  fltk3::lock();

  // Start threads...

  // One thread displaying in one browser
  fltk3::create_thread(prime_thread, prime_func, browser1);

  // Several threads displaying in another browser
  fltk3::create_thread(prime_thread, prime_func, browser2);
  fltk3::create_thread(prime_thread, prime_func, browser2);
  fltk3::create_thread(prime_thread, prime_func, browser2);
  fltk3::create_thread(prime_thread, prime_func, browser2);
  fltk3::create_thread(prime_thread, prime_func, browser2);
  fltk3::create_thread(prime_thread, prime_func, browser2);

  fltk3::run();

  return 0;
}
#else
#  include <fltk3/ask.h>

int main() {
  fltk3::alert("Sorry, threading not supported on this platform!");
}
#endif // HAVE_PTHREAD || WIN32


//
// End of "$Id$".
//
