//
// "$Id: Fl_Box.cxx,v 1.25 2002/10/04 07:48:13 spitzak Exp $"
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

#include <fltk/Fl_Widget.h>
#include <fltk/Fl_Box.h>

static void revert(Fl_Style* s) {
  s->color = FL_GRAY;
  s->box = FL_NO_BOX;
}
// this is unnamed as there is no need for themes to alter this:
static Fl_Named_Style style(0, revert, &Fl_Box::default_style);
Fl_Named_Style* Fl_Box::default_style = &::style;

Fl_Box::Fl_Box(int x, int y, int w, int h, const char *l)
  : Fl_Widget(x,y,w,h,l)
{
  style(default_style);
}

Fl_Box::Fl_Box(Fl_Boxtype b, int x, int y, int w, int h, const char *l)
  : Fl_Widget(x,y,w,h,l)
{
  style(default_style);
  box(b);
}

extern Fl_Widget* fl_did_clipping;

void Fl_Box::draw() {
  // check for completely blank widgets. We must not clip to their
  // area because it will break lots of programs that assumme these
  // can overlap any other widgets:
  if (box()==FL_NO_BOX && 
    (!label() && !image() ||
     align() != FL_ALIGN_CENTER && !(align()&FL_ALIGN_INSIDE))) {
    fl_did_clipping = this;
    return;
  }
  Fl_Widget::draw();
}

//
// End of "$Id: Fl_Box.cxx,v 1.25 2002/10/04 07:48:13 spitzak Exp $".
//
