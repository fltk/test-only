// Tile layout manager for the Fast Light Tool Kit (FLTK).
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

// The Fl_Tile_Layout splits the group's area in equally sized
// cells and resize()s the children into them.

#include <FL/Fl_Group.H>
#include <FL/Fl_Layout.H>
#include <FL/Fl_Window.H> //just to get the FL_WINDOW constant!

void Fl_Tile_Layout::perform() {
  int cx = dw(), cy = dh();
  int n_lines = (children() / n_to_break() + (children() % n_to_break()?1:0));
  int W = (w() - ((vertical() ? n_lines:n_to_break()) + 1)*dw()) / 
          (vertical() ? n_lines : n_to_break() ? n_to_break() : 1);
  int H = (h() - ((vertical() ? n_to_break():n_lines) + 1)*dh()) / 
          (vertical() ? n_to_break() ? n_to_break() : 1 : n_lines);
  if (type() < FL_WINDOW) {cx += x(); cy += y();}
  int icx = cx, icy = cy;

  Fl_Widget*const* a = array();
  int to_break = n_to_break();
  for (int i=children(); i--;) {
    Fl_Widget* o = *a++;
    o->resize(cx,cy,W,H);
    if (n_to_break() && !--to_break) {
      if (vertical()) {
	cx += W+dw(); cy = icy;
      } else {
	cy += H+dh(); cx = icx;
      }
      to_break = n_to_break();
    } else {
      if (vertical()) {
	cy += H+dh();
      } else {
	cx += W+dw();
      }
    }
    o->layout();
  }
  init_sizes();
}


