//
// "$Id: cursor.cxx 8500 2011-03-03 09:20:46Z bgbnbigben $"
//
// Cursor test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2006 by Bill Spitzak and others.
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
//    http://www.fltk.org/str.php
//

#if 0 // FIXME: 123-2

// This is a complete rewrite that replaces the old test program.
// Cursors are no longer identified by an integer.

#include <config.h> 
#endif
#include <fltk/run.h>
#if 0 // FIXME: 123-2
#include <fltk/events.h>
#endif
#include <fltk/Window.h>
#if 0 // FIXME: 123-2
#include <fltk/Cursor.h>
#ifdef USE_XCURSOR
#include <fltk/SharedImage.h>
#include <stdio.h>
#endif
#endif

using namespace fltk;

class CursorBox : public Widget {
#if 0 // FIXME: 123-2
  int handle(int);
#endif
public:
  Cursor* cursor;
  CursorBox(int x, int y, int w, int h, const char* name, Cursor* c)
    : Widget(x,y,w,h,name), cursor(c) {}
};

#if 0 // FIXME: 123-2
int CursorBox::handle(int event) {
  if (event == ENTER) Widget::cursor(cursor);
  if (event == PUSH) return true; // drag the cursor around
  return Widget::handle(event);
}

struct gcc4_bug_workaround {const char* name; Cursor* cursor;} table[] = {
  {"0", 0},
  {"CURSOR_ARROW",	CURSOR_ARROW},
  {"CURSOR_CROSS",	CURSOR_CROSS},
  {"CURSOR_WAIT",	CURSOR_WAIT},
  {"CURSOR_INSERT",	CURSOR_INSERT},
  {"CURSOR_HAND",	CURSOR_HAND},
  {"CURSOR_HELP",	CURSOR_HELP},
  {"CURSOR_MOVE",	CURSOR_MOVE},
  {"CURSOR_NS",		CURSOR_NS},
  {"CURSOR_WE",		CURSOR_WE},
  {"CURSOR_NWSE",	CURSOR_NWSE},
  {"CURSOR_NESW",	CURSOR_NESW},
  {"CURSOR_NO",		CURSOR_NO},
  {"CURSOR_NONE",	CURSOR_NONE},
#ifdef USE_XCURSOR
  {"CUSTOM_CURSOR",	0},
#endif
};
#define COUNT (sizeof(table)/sizeof(table[0]))
#else
#define COUNT 10
#endif
#define W 200
#define H 25
#define GAP 5

#if 0 // FIXME: 123-2
#ifdef USE_XCURSOR
int load_file(int argc, char** argv, int&i){
  Image *img = SharedImage::get(argv[i]);
  if( img ){
    table[COUNT-1].cursor = fltk::cursor(img, 0, 0);
    ++i;
    return 1;
  }
  return 0;
}
#endif
#endif

#if 0 // FIXME: 123-2
#ifdef USE_XCURSOR
int main(int argc, char **argv) {
  register_images();
  int i;
  if ( fltk::args(argc, argv, i, load_file) < argc -1) {
    fprintf(stderr,"args return %d (%d)\n",i,argc);
    return 1;
  }
  if(!table[COUNT-1].cursor)table[COUNT-1].cursor = fltk::cursor(SharedImage::get("porsche.xpm"),32,32);
#else
int main(int argc, char **argv) {
#endif
#else
int main(int argc, char **argv) {
#endif
  fltk::Window window(W+2*GAP, (H+GAP)*COUNT+GAP);
  window.begin();
#if 0 // FIXME: 123-2  
  for (unsigned i = 0; i < COUNT; i++)
    new CursorBox(GAP, GAP+i*(H+GAP), W, H, table[i].name, table[i].cursor);
#else
  for (unsigned i = 0; i < COUNT; i++)
    new CursorBox(GAP, GAP+i*(H+GAP), W, H, "TEST", 0);
#endif
  window.end();
  window.show(argc, argv);
  return run();
}
  
//
// End of "$Id: cursor.cxx 8500 2011-03-03 09:20:46Z bgbnbigben $".
//
