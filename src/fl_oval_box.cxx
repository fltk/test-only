//
// "$Id: fl_oval_box.cxx,v 1.17 2001/02/20 06:59:50 spitzak Exp $"
//
// Oval box drawing code for the Fast Light Tool Kit (FLTK).
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


// Less-used box types are in seperate files so they are not linked
// in if not used.

#include <FL/Fl_Boxtype.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.H>

void Fl_Oval_Flat_Box::draw(const Fl_Widget* widget,
			    int x, int y, int w, int h, Fl_Flags f) const
{
  fl_ellipse(x, y, w, h);
  fl_color(widget->get_box_color(f));
  fl_fill();
}
Fl_Oval_Flat_Box::Fl_Oval_Flat_Box(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = dw_ = dh_ = 0;
  fills_rectangle_ = 0;
}
const Fl_Oval_Flat_Box fl_oval_flat_box(0);

void Fl_Oval_Box::draw(const Fl_Widget* widget,
		       int x, int y, int w, int h, Fl_Flags f) const
{
  fl_ellipse(x, y, w-1, h-1);
  fl_color(widget->get_box_color(f));
  fl_fill_stroke(widget->get_glyph_color(f));
}
Fl_Oval_Box::Fl_Oval_Box(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = 1; dw_ = dh_ = 2;
  fills_rectangle_ = 0;
}
const Fl_Oval_Box fl_oval_box(0);

void Fl_Oval_Shadow_Box::draw(const Fl_Widget* widget,
			      int x, int y, int w, int h, Fl_Flags f) const
{
  w-=3; h-=3;
  fl_ellipse(x+3, y+3, w, h);
  fl_color(FL_DARK3);
  fl_oval_box.draw(widget, x, y, w, h, f);
}
Fl_Oval_Shadow_Box::Fl_Oval_Shadow_Box(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = 1; dw_ = dh_ = 5;
  fills_rectangle_ = 0;
}
const Fl_Oval_Shadow_Box fl_oval_shadow_box(0);

//
// End of "$Id: fl_oval_box.cxx,v 1.17 2001/02/20 06:59:50 spitzak Exp $".
//
