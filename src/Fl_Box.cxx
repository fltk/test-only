//
// "$Id: Fl_Box.cxx,v 1.19 2000/01/10 06:31:17 bill Exp $"
//
// Box widget for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>

void Fl_Box::draw() {
  draw_box();
  draw_label();
}

// Since FL_NO_BOX boxes are often used as invisible resizeboxes, I
// check for this and avoid unnecessary clipping.
void Fl_Box::draw_n_clip() {
  if (box() != FL_NO_BOX || label() || image())
    Fl_Widget::draw_n_clip();
}

static void revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
}

// this is unnamed as there is no need for themes to alter this:
static Fl_Named_Style *style = new Fl_Named_Style(0, revert, &style);

Fl_Box::Fl_Box(int x, int y, int w, int h, const char *l)
  : Fl_Widget(x,y,w,h,l)
{
  type(FL_BOX);
  style(::style);
}

//
// End of "$Id: Fl_Box.cxx,v 1.19 2000/01/10 06:31:17 bill Exp $".
//
