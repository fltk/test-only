//
// "$Id: cursor.cxx,v 1.10 2002/12/09 04:52:31 spitzak Exp $"
//
// Cursor test program for the Fast Light Tool Kit (FLTK).
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

// This is a complete rewrite that replaces the old test program.
// Cursors are no longer identified by an integer.

#include <fltk/run.h>
#include <fltk/events.h>
#include <fltk/Window.h>
#include <fltk/Cursor.h>

using namespace fltk;

class CursorBox : public Widget {
  Cursor* cursor;
  int handle(int);
public:
  CursorBox(int x, int y, int w, int h, const char* name, Cursor* c)
    : Widget(x,y,w,h,name), cursor(c) {}
};

int CursorBox::handle(int event) {
  if (event == ENTER) Widget::cursor(cursor);
  if (event == PUSH) return true; // drag the cursor around
  return Widget::handle(event);
}

struct {const char* name; Cursor* cursor;} table[] = {
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
};
#define COUNT (sizeof(table)/sizeof(*table))
#define W 200
#define H 25
#define GAP 5

int main(int argc, char **argv) {
  Window window(W+2*GAP, (H+GAP)*COUNT+GAP);
  window.begin();
  for (unsigned i = 0; i < COUNT; i++)
    new CursorBox(GAP, GAP+i*(H+GAP), W, H, table[i].name, table[i].cursor);
  window.end();
  window.show(argc, argv);
  return run();
}

//
// End of "$Id: cursor.cxx,v 1.10 2002/12/09 04:52:31 spitzak Exp $".
//
