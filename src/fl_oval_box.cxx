//
// "$Id: fl_oval_box.cxx,v 1.6 1999/10/07 07:04:59 bill Exp $"
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

static void oval_flat_draw(Fl_Boxtype, int x, int y, int w, int h,
			   Fl_Color c, Fl_Flags)
{
  fl_color(c);
  fl_pie(x, y, w, h, 0, 360);
}
const Fl_Boxtype_ fl_oval_flat_box = {
  oval_flat_draw, 0, 0, 0,0,0,0,
};

static void oval_draw(Fl_Boxtype, int x, int y, int w, int h,
		      Fl_Color c, Fl_Flags f)
{
  fl_color(c);
  fl_pie(x, y, w-1, h-1, 0, 360);
  fl_color((f&FL_INACTIVE) ? FL_INACTIVE_COLOR : FL_NO_COLOR);
  fl_arc(x, y, w, h, 0, 360);
}
const Fl_Boxtype_ fl_oval_box = {
  oval_draw, 0, 0, 1,1,2,2,
};

static void oval_shadow_draw(Fl_Boxtype b, int x, int y, int w, int h,
			     Fl_Color c, Fl_Flags f)
{
  oval_flat_draw(b, x+3, y+3, w, h, FL_DARK3, FL_NO_FLAGS);
  oval_draw(b, x, y, w, h, c, f);
}
const Fl_Boxtype_ fl_oval_shadow_box = {
  oval_shadow_draw, 0, 0, 1,1,2,2,
};

//
// End of "$Id: fl_oval_box.cxx,v 1.6 1999/10/07 07:04:59 bill Exp $".
//
