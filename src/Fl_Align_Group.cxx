// Align layout manager for the Fast Light Tool Kit (FLTK).
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

// The Fl_Align_Group overrides all group's children's label alignments to
// it's own align() value, tiles and resize()s the children to fit in the space 
// not required by the (outsize) labels. 

#include <stdio.h>
#include <string.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Flags.h>
#include <fltk/Fl_Group.h>
#include <fltk/Fl_Align_Group.h>

void Fl_Align_Group::layout() {
  Fl_Widget::layout();
  int i;

  int n_lines = n_to_break() ? 
                (children() / n_to_break() + (children() % n_to_break()?1:0)):
		1;
  bool variable_is_y = (align() & (FL_ALIGN_TOP|FL_ALIGN_BOTTOM)) != 0;
  int nx = vertical() ? n_lines : n_to_break() ? n_to_break() : children(); 
  int ny = vertical() ? n_to_break() ? n_to_break() : children() : n_lines;
  int n_variable = variable_is_y ? ny : nx;
  uchar label_space[256];
  memset(label_space,0,n_variable*sizeof(uchar));

  int u=0, v=0;
  int total_label_space=0;
  int numchildren = children();
  if (align()) {
    for (i = 0; i < numchildren; i++) {
      Fl_Widget* o = child(i);
      fl_font(o->label_font(),o->label_size());
      int w = this->w()-o->w(),h = this->h()-o->h();
      fl_measure(o->label(),w,h,o->flags());
      if (variable_is_y) w = h;
      int which = (variable_is_y == vertical()) ? u : v; 
      if (label_space[which] < w) label_space[which] = w;
      if (++u == n_to_break()) {u = 0; v++;}
    }
    for (i = 0; i<n_variable; i++) total_label_space+=label_space[i];
  }

  int W = (w() - (variable_is_y ? 0:total_label_space) - (nx + 1)*dw()) / nx;
  int H = (h() - (variable_is_y ? total_label_space:0) - (ny + 1)*dh()) / ny; 
  int cx = dw(), cy = dh();
  int icx = cx, icy = cy;

  u = v = 0;
  for (i = 0; i < numchildren; i++) {
    Fl_Widget* o = child(i);
    int which = (variable_is_y == vertical()) ? u : v; 
    int X = cx, Y = cy;
    if (align() & FL_ALIGN_TOP) Y += label_space[which];
    else if (!variable_is_y && align() & FL_ALIGN_LEFT) X += label_space[which];

    o->resize(X,Y,W,H);
    o->align(align());
    o->layout();

    if (++u == n_to_break()) {
      u = 0; v++;
      if (vertical()) {
	cx += W+dw(); cy = icy;
        if (!variable_is_y) cx += label_space[which];
      } else {
	cy += H+dh(); cx = icx;
        if (variable_is_y) cy += label_space[which];
      }
    } else {
      if (vertical()) {
	cy += H+dh();
        if (variable_is_y) cy += label_space[which];
      } else {
	cx += W+dw();
        if (!variable_is_y) cx += label_space[which];
      }
    }
  }
  init_sizes();
}


