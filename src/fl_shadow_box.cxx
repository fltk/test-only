//
// "$Id: fl_shadow_box.cxx,v 1.9 1999/11/19 10:06:55 bill Exp $"
//
// Shadow box drawing routines for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl_Boxtype.H>
#include <FL/fl_draw.H>

#define BW 3

void Fl_Shadow_Box::draw(int x, int y, int w, int h,
			 Fl_Color c, Fl_Flags f) const {
  w-=BW; h-=BW;
  if (!(f & FL_FRAME_ONLY)) {
    fl_color(c);
    fl_rectf(x+1,y+1,w-2,h-2);
  }
  fl_color(FL_DARK3);
  fl_rectf(x+BW, y+h,  w, BW);
  fl_rectf(x+w,  y+BW, BW,  h);
  fl_color((f&FL_INACTIVE) ? FL_INACTIVE_COLOR : FL_NO_COLOR);
  fl_rect(x,y,w,h);
}
void Fl_Shadow_Box::inset(int& x,int& y,int& w,int& h) const {
  x++; y++; w-=2+BW; h-=2+BW;
}
int Fl_Shadow_Box::fills_rectangle() const {return false;} // unfortunate...
const Fl_Shadow_Box fl_shadow_box(0);

//
// End of "$Id: fl_shadow_box.cxx,v 1.9 1999/11/19 10:06:55 bill Exp $".
//
