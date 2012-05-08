//
// "$Id$"
//
// Browser test program for the Fast Light Tool Kit (FLTK).
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

/*
This is a test of how the browser draws lines.
This is a second line.
This is a third.

That was a blank line above this.

@r@_Right justify
@c@_Center justify
@_Left justify

@bBold text
@iItalic text
@b@iBold Italic
@fFixed width
@f@bBold Fixed
@f@iItalic Fixed
@f@i@bBold Italic Fixed
@lLarge
@l@bLarge bold
@sSmall
@s@bSmall bold
@s@iSmall italic
@s@i@bSmall italic bold
@uunderscore
@C1RED
@C2Green
@C4Blue

	You should try different browser types:
	fltk3::Browser
	fltk3::Select_Browser
	fltk3::Hold_Browser
	fltk3::Multi_Browser
*/

#include <fltk3/run.h>
#include <fltk3/SelectBrowser.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Button.h>
#include <fltk3/IntInput.h>
#include <fltk3/ask.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

fltk3::SelectBrowser *browser;
fltk3::Button	*top,
		*bottom,
		*middle,
		*visible,
		*swap,
		*sort;
fltk3::IntInput	*field;

void b_cb(fltk3::Widget* o, void*) {
  printf("callback, selection = %d, event_clicks = %d\n",
	 ((fltk3::Browser*)o)->value(), fltk3::event_clicks());
}

void show_cb(fltk3::Widget *o, void *) {
  int line = atoi(field->value());

  if (!line) {
    fltk3::alert("Please enter a number in the text field\n"
             "before clicking on the buttons.");
    return;
  }

  if (o == top)
    browser->topline(line);
  else if (o == bottom)
    browser->bottomline(line);
  else if (o == middle)
    browser->middleline(line);
  else
    browser->make_visible(line);
}

void swap_cb(fltk3::Widget *, void *) {
  int a = -1, b = -1;
  for ( int t=0; t<browser->size(); t++ ) {	// find two selected items
    if ( browser->selected(t) ) {
      if ( a < 0 )
	{ a = t; }
      else 
	{ b = t; break; }
    }
  }
  browser->swap(a, b);				// swap them
}

void sort_cb(fltk3::Widget *, void *) {
  browser->sort(fltk3::SORT_ASCENDING);
}

int main(int argc, char **argv) {
  int i;
  if (!fltk3::args(argc,argv,i)) fltk3::fatal(fltk3::help);
  const char* fname = (i < argc) ? argv[i] : "browser.cxx";
  fltk3::DoubleWindow window(480,400,fname);
  browser = new fltk3::SelectBrowser(10,10,460,330,0);
  browser->type(fltk3::MULTI_BROWSER);
  //browser->type(fltk3::HOLD_BROWSER);
  //browser->color(42);
  browser->callback(b_cb);
  // browser->scrollbar_right();
  //browser->has_scrollbar(fltk3::Browser::BOTH_ALWAYS);
  if (!browser->load(fname)) {
    int done = 0;
#ifdef _MSC_VER
    // if 'browser' was started from the VisualC environment in Win32, 
    // the current directory is set to the environment itself, 
    // so we need to correct the browser file path
    if ( i == argc ) 
    {
      fname = "../test/browser.cxx";
      done = browser->load(fname);
    }
#elif defined(USING_XCODE)
    if ( i == argc ) 
    {
      char buf[2048];
      strcpy(buf, argv[0]);
      char *slash = strrchr(buf, '/');
      if (slash)
        strcpy(slash, "/../Resources/browser.cxx");
#ifdef MATT_IS_LAZY
      done = browser->load("/Users/matt/dev/fltk-1.3.0/test/browser.cxx");
#else
      done = browser->load(buf);
#endif
    }
#endif
    if ( !done )
    {
      fltk3::message("Can't load %s, %s\n", fname, strerror(errno));
      exit(1);
    }
  }
  browser->position(0);

  field = new fltk3::IntInput(50, 350, 430, 25, "Line #:");
  field->callback(show_cb);

  top = new fltk3::Button(0, 375, 80, 25, "Top");
  top->callback(show_cb);

  bottom = new fltk3::Button(80, 375, 80, 25, "Bottom");
  bottom->callback(show_cb);

  middle = new fltk3::Button(160, 375, 80, 25, "Middle");
  middle->callback(show_cb);

  visible = new fltk3::Button(240, 375, 80, 25, "Make Vis.");
  visible->callback(show_cb);

  swap = new fltk3::Button(320, 375, 80, 25, "Swap");
  swap->callback(swap_cb);
  swap->tooltip("Swaps two selected lines\n(Use CTRL-click to select two lines)");

  sort = new fltk3::Button(400, 375, 80, 25, "Sort");
  sort->callback(sort_cb);

  window.resizable(browser);
  window.show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//

