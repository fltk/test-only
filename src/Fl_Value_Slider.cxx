//
// "$Id: Fl_Value_Slider.cxx,v 1.32 2001/01/23 18:47:55 spitzak Exp $"
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
  int sx = 0, sy = 0, sw = w(), sh = h();
  int bx = 0, by = 0, bw = w(), bh = h();
  if (horizontal()) {
    bw = 35; sx += 35; sw -= 35;
  } else {
    sy += 25; bh = 25; sh -= 25;
  }
  if (damage()&FL_DAMAGE_ALL) {
    draw_text_frame(sx, sy, sw, sh);
    default_style->text_box->draw(this, bx, by, bw, bh, FL_FRAME_ONLY);
  }
  text_box()->inset(sx, sy, sw, sh);
  Fl_Flags f = 0;
  if (!active_r()) f = FL_INACTIVE;
  else {
    if (Fl::pushed() == this) f |= (FL_SELECTED|FL_VALUE);
    else if (belowmouse()) f |= FL_HIGHLIGHT;
  }
  Fl_Slider::draw(sx, sy, sw, sh, f);
  if (damage()&(~FL_DAMAGE_HIGHLIGHT)) {
    // copy the box & color from the default style:
    fl_color(default_style->text_background);
    default_style->text_box->inset(bx, by, bw, bh);
    fl_rectf(bx, by, bw, bh);
    // now draw the text:
    char buf[128];
    format(buf);
    fl_clip(bx, by, bw, bh);
    fl_font(text_font(), text_size());
    fl_color(get_glyph_color());
    fl_draw(buf, bx, by, bw, bh, 0);
    fl_pop_clip();
  }
}

int Fl_Value_Slider::handle(int event) {
  int sx = 0, sy = 0, sw = w(), sh = h();
  if (horizontal()) {
    sx += 35; sw -= 35;
  } else {
    sy += 25; sh -= 25;
  }
  text_box()->inset(sx, sy, sw, sh);
  if (event == FL_PUSH) take_focus();
  return Fl_Slider::handle(event, sx, sy, sw, sh);
}

static void revert(Fl_Style* s) {
  s->text_background = FL_DARK2;
//  s->text_size = 10;
}

static Fl_Named_Style* style = new Fl_Named_Style("Value Slider", revert, &style);

Fl_Value_Slider::Fl_Value_Slider(int x, int y, int w, int h, const char*l)
: Fl_Slider(x, y, w, h, l) {
  style(::style);
  step(.01);
}

//
// End of "$Id: Fl_Value_Slider.cxx,v 1.32 2001/01/23 18:47:55 spitzak Exp $".
//
