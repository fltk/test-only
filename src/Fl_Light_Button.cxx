//
// "$Id: Fl_Light_Button.cxx,v 1.19 1999/11/22 09:00:17 bill Exp $"
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

// Subclass of Fl_Button that draws FL_GLYPH_CHECK or FL_GLYPH_RADIO
// Usually the outer box is FL_FLAT_BOX so that, to the user, it looks
// like there is a just a checkmark and text drawn on a background.

// The Fl_Light_Button draws the indicator as a rectangle in the
// style of Flame.

#include <FL/Fl.H>
#include <FL/Fl_Light_Button.H>

static void glyph(int/*t*/, int x,int y,int w,int h, Fl_Color, Fl_Color fc,
		  Fl_Flags f, Fl_Boxtype box)
{
  box->draw(x,y,w,h, fc, f); // notice it passes foreground color!
}

void Fl_Light_Button::draw() {

  // Draw the outer box as though it was a button:
  Fl_Flags f = flags();
  Fl_Color c = color();
  Fl_Color lc = label_color();
  if (!active_r()) {
    f |= FL_INACTIVE;
  } else if (Fl::belowmouse() == this && highlight_color()) {
    c = highlight_color();
    if (highlight_label_color()) lc = highlight_label_color();
    f |= FL_HIGHLIGHT;
  }
  box()->draw(x(), y(), w(), h(), c,
	      Fl::pushed()==this ? (f|FL_VALUE) : (f&~FL_VALUE));

  // Draw the check box:
  int d = h()/6;
  int W = w()<h() ? w() : h();
  int hh = h()-2*d;
  int ww = hh/2+1;
  int xx = d*2;
  if (w()<ww+2*xx) xx = (w()-ww)/2;
  glyph()(0, x()+xx, y()+d, ww, hh,
          off_color(), (f&FL_VALUE) ? on_color() : off_color(),
          active_r() ? 0 : FL_INACTIVE, glyph_box());

  draw_button_label(x()+W-d, y(), w()-W+d, h(), lc);
}

int Fl_Light_Button::handle(int event) {
  if (event == FL_RELEASE && !Fl::pushed()) redraw();
  return Fl_Button::handle(event);
}

static void revert(Fl_Style* s) {
  s->box = FL_NORMAL_BOX;
  s->glyph = glyph;
  s->glyph_box = FL_THIN_DOWN_BOX;
  s->selection_color = FL_YELLOW;
}

Fl_Style* Fl_Light_Button::default_style = new Fl_Named_Style("Light_Button", revert, &Fl_Light_Button::default_style);

Fl_Light_Button::Fl_Light_Button(int x, int y, int w, int h, const char *l)
  : Fl_Button(x, y, w, h, l)
{
  type(FL_TOGGLE_BUTTON);
  set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  style(default_style);
}

//
// End of "$Id: Fl_Light_Button.cxx,v 1.19 1999/11/22 09:00:17 bill Exp $".
//
