//
// "$Id: browser.cxx,v 1.9 2000/04/24 08:31:29 bill Exp $"
//
// Browser test program for the Fast Light Tool Kit (FLTK).
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

*/

#include <FL/Fl.H>
#include <FL/Fl_NewBrowser.H>
#include <FL/Fl_Window.H>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <FL/Fl_Item.H>
#include <FL/Fl_Item_Group.H>

void b_cb(Fl_Widget* o, void*) {
  printf("callback, selection = %d, event_clicks = %d\n",
	 ((Fl_Browser*)o)->value(), Fl::event_clicks());
}

extern int fl_mousewheel_up;
extern int fl_mousewheel_down;

int main(int argc, char **argv) {
  int i;
  if (!Fl::args(argc,argv,i)) Fl::fatal(Fl::help);
  const char* fname = (i < argc) ? argv[i] : "browser.cxx";
  Fl_Window window(400,400,fname);
  window.box(FL_NO_BOX); // because it is filled with browser
  Fl_Browser browser(0,0,400,400,0);
  //browser.type(FL_MULTI_BROWSER);
  //browser.color(42);
  browser.callback(b_cb);
  // browser.scrollbar_right();
  //browser.has_scrollbar(Fl_Browser::BOTH_ALWAYS);

  browser.begin();
  new Fl_Item("Alpha");
  new Fl_Item("Beta");
  new Fl_Item("Ceta");
  new Fl_Item("Delta");
  (new Fl_Item("Epsilon and this item is a bit long"))->align(FL_ALIGN_LEFT);
  Fl_Item_Group* g = new Fl_Item_Group("Closed group");
  g->begin();
  for (int j = 0; j < 10; j++) {
    char buf[100];
    sprintf(buf, "Child %d\n", j);
    new Fl_Item(strdup(buf));
  }
  g->end();
  g = new Fl_Item_Group("Open group");
  g->set_flag(FL_OPEN);
  g->begin();
  for (int j = 0; j < 10; j++) {
    char buf[100];
    sprintf(buf, "Child %d\n", j);
    new Fl_Item(strdup(buf));
  }
  Fl_Group* k = new Fl_Item_Group("Open group");
  k->set_flag(FL_OPEN);
  k->begin();
  for (int j = 0; j < 10; j++) {
    char buf[100];
    sprintf(buf, "Child %d\n", j);
    new Fl_Item(strdup(buf));
  }
  k->end();
  g->end();
  for (int j = 0; j < 100; j++) {
    char buf[100];
    sprintf(buf, "Item %d\n", j);
    new Fl_Item(strdup(buf));
  }
  new Fl_Item("The last item");
  browser.end();

  //browser.position(0);
  window.resizable(&browser);
  window.show(argc,argv);
//  fl_mousewheel_up = 2;
//  fl_mousewheel_down = 3;
  return Fl::run();
}

//
// End of "$Id: browser.cxx,v 1.9 2000/04/24 08:31:29 bill Exp $".
//

