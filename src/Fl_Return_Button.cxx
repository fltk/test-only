//
// "$Id: Fl_Return_Button.cxx,v 1.17 1999/11/21 06:23:26 carl Exp $"
//
// Return button widget for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Return_Button.H>

Fl_Return_Button::Fl_Return_Button(int x,int y,int w,int h,const char *l)
  : Fl_Button(x,y,w,h,l)
{
  style(default_style);
}

void Fl_Return_Button::draw() {
  Fl_Color lc = draw_button();
  int W = h();
  if (w()/3 < W) W = w()/3;
  Fl_Flags f = active_r() ? 0 : FL_INACTIVE;
  draw_glyph(FL_GLYPH_RETURN, x()+w()-W-4, y(), W, h(), f);
  draw_button_label(x(), y(), w()-W+4, h(), lc);
}

int Fl_Return_Button::handle(int event) {
  if (event == FL_SHORTCUT &&
      (Fl::event_key() == FL_Enter || Fl::event_key() == FL_KP_Enter)) {
    do_callback();
    return 1;
  } else
    return Fl_Button::handle(event);
}

static void revert(Fl_Style* s) {
  s->glyph_box = FL_NO_BOX;
  s->glyph = fl_glyph_return;
}

Fl_Style* Fl_Return_Button::default_style = new Fl_Named_Style("Return_Button", revert, &Fl_Return_Button::default_style);

//
// End of "$Id: Fl_Return_Button.cxx,v 1.17 1999/11/21 06:23:26 carl Exp $".
//
