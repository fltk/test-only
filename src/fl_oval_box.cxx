//
// "$Id: fl_oval_box.cxx,v 1.14 2000/01/16 07:44:37 robertk Exp $"
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


void Fl_Oval_Flat_Box::draw(int x, int y, int w, int h,
			    Fl_Color c, Fl_Flags) const {
  fl_color(c);
  fl_pie(x, y, w, h, 0, 360);
}
void Fl_Oval_Flat_Box::inset(int&,int&,int&,int&) const {}
int Fl_Oval_Flat_Box::fills_rectangle() const {return false;}
const Fl_Oval_Flat_Box fl_oval_flat_box(0);

void Fl_Oval_Box::draw(int x, int y, int w, int h,
		       Fl_Color c, Fl_Flags f) const {
  fl_color(c);
  fl_pie(x, y, w-1, h-1, 0, 360);
  fl_color(fl_inactive(FL_BLACK, f));
  fl_arc(x, y, w, h, 0, 360);
}

void Fl_Oval_Box::inset(int& x,int& y,int& w,int& h) const {
  x++; y++; w-=2; h-=2;
}

int Fl_Oval_Box::fills_rectangle() const {return false;}

const Fl_Oval_Box fl_oval_box(0);

void Fl_Oval_Shadow_Box::draw(int x, int y, int w, int h,
			      Fl_Color c, Fl_Flags f) const {
  w-=3; h-=3;
  fl_oval_flat_box.draw(x+3, y+3, w, h, FL_DARK3, FL_NO_FLAGS);
  fl_oval_box.draw(x, y, w, h, c, f);
}
void Fl_Oval_Shadow_Box::inset(int& x,int& y,int& w,int& h) const {
  x++; y++; w-=5; h-=5;
}
int Fl_Oval_Shadow_Box::fills_rectangle() const {return false;}
const Fl_Oval_Shadow_Box fl_oval_shadow_box(0);

//
// End of "$Id: fl_oval_box.cxx,v 1.14 2000/01/16 07:44:37 robertk Exp $".
//
