//
// "$Id: Fl_Value_Slider.cxx,v 1.20 2000/01/10 06:31:25 bill Exp $"
//
// Value slider widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Value_Slider.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>

void Fl_Value_Slider::draw() {
  int sxx = x(), syy = y(), sww = w(), shh = h();
  int bxx = x(), byy = y(), bww = w(), bhh = h();
  if (horizontal()) {
    bww = 35; sxx += 35; sww -= 35;
  } else {
    syy += 25; bhh = 25; shh -= 25;
  }
  Fl_Flags f = active_r() ? FL_NO_FLAGS : FL_INACTIVE;
  if (damage()&(~FL_DAMAGE_HIGHLIGHT))
    box()->draw(sxx, syy, sww, shh, color(), f|FL_FRAME_ONLY);
  if (!active_r()) f = FL_INACTIVE;
  else if (Fl::belowmouse() == this && highlight_color()) f = FL_HIGHLIGHT;
  else f = 0;
  box()->inset(sxx, syy, sww, shh);
  Fl_Slider::draw(sxx, syy, sww, shh, f);
  if (damage()&(~FL_DAMAGE_HIGHLIGHT)) {
    Fl_Output::default_style->box->draw(bxx, byy, bww, bhh,
         fl_inactive(Fl_Output::default_style->color, f), f);
    char buf[128];
    format(buf);
    fl_font(textfont(), textsize());
    fl_color(fl_inactive(textcolor(), f));
    fl_draw(buf, bxx, byy, bww, bhh, FL_ALIGN_CLIP);
  }
}

int Fl_Value_Slider::handle(int event) {
  int sxx = x(), syy = y(), sww = w(), shh = h();
  if (horizontal()) {
    sxx += 35; sww -= 35;
  } else {
    syy += 25; shh -= 25;
  }
  box()->inset(sxx, syy, sww, shh);
  return Fl_Slider::handle(event, sxx, syy, sww, shh);
}

static void revert(Fl_Style* s) {
  s->box = FL_THIN_DOWN_BOX;
  s->color = FL_DARK2;
  s->text_size = 10;
}

static Fl_Named_Style* style = new Fl_Named_Style("Value_Slider", revert, &style);

Fl_Value_Slider::Fl_Value_Slider(int x, int y, int w, int h, const char*l)
: Fl_Slider(x, y, w, h, l) {
  style(::style);
  step(1, 100);
}

//
// End of "$Id: Fl_Value_Slider.cxx,v 1.20 2000/01/10 06:31:25 bill Exp $".
//
