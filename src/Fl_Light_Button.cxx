//
// "$Id: Fl_Light_Button.cxx,v 1.21 2000/01/10 06:31:22 bill Exp $"
//
// Lighted button widget for the Fast Light Tool Kit (FLTK).
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

// The Fl_Light_Button draws the indicator as a rectangle in the
// style of Flame.  This is done by replacing the glyph drawing function
// in the default style.

#include <FL/Fl.H>
#include <FL/Fl_Light_Button.H>

static void glyph(int/*t*/, int x,int y,int w,int h, Fl_Color, Fl_Color fc,
		  Fl_Flags f, Fl_Boxtype box)
{
  int ww = w/2+1;
  box->draw(x+(w-ww)/2,y,ww,h, fc, f & FL_INACTIVE);
}

static void revert(Fl_Style* s) {
  s->box = FL_NORMAL_BOX;
  s->glyph = glyph;
  s->glyph_box = FL_THIN_DOWN_BOX;
  s->selection_color = FL_YELLOW;
}

static Fl_Named_Style* style = new Fl_Named_Style("Light_Button", revert, &style);

Fl_Light_Button::Fl_Light_Button(int x, int y, int w, int h, const char *l)
  : Fl_Check_Button(x, y, w, h, l)
{
  style(::style);
}

//
// End of "$Id: Fl_Light_Button.cxx,v 1.21 2000/01/10 06:31:22 bill Exp $".
//
