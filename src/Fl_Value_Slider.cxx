//
// "$Id: Fl_Value_Slider.cxx,v 1.37 2002/01/23 08:46:01 spitzak Exp $"
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

#include <fltk/Fl.h>
#include <fltk/Fl_Value_Slider.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Output.h>
#include <config.h>

#define vertical() (!(type()&1))

void Fl_Value_Slider::draw() {
  int sx = 0, sy = 0, sw = w(), sh = h();
  int bx = 0, by = 0, bw = w(), bh = h();
  if (vertical()) {
    sy += 25; bh = 25; sh -= 25;
  } else {
    bw = 35; sx += 35; sw -= 35;
  }
  Fl_Flags flags = this->flags();
  if (!active_r()) flags |= FL_INACTIVE;

  if (damage()&(~FL_DAMAGE_HIGHLIGHT)) {
    // copy the box & color from the default style:
    Fl_Color bg = Fl_Output::default_style->color;
    if (!bg) bg = Fl_Widget::default_style->color;
    Fl_Boxtype valuebox = Fl_Output::default_style->box;
    if (!valuebox) valuebox = Fl_Widget::default_style->box;
    // draw the edge of the box around text:
    if (damage()&FL_DAMAGE_ALL)
      valuebox->draw(bx, by, bw, bh, bg, flags|FL_INVISIBLE);
    // erase the interior of the box:
    valuebox->inset(bx, by, bw, bh);
    fl_color(bg);
    fl_rectf(bx, by, bw, bh);
    // now draw the text:
    char buf[128];
    format(buf);
    fl_push_clip(bx, by, bw, bh);
    fl_font(text_font(), text_size());
    fl_color(fl_inactive(text_color(),flags));
    fl_draw(buf, bx, by, bw, bh, 0);
    fl_pop_clip();
  }

  // draw the edge of the slider:
  if (damage()&FL_DAMAGE_ALL)
    box()->draw(sx, sy, sw, sh, color(), flags|FL_INVISIBLE);
  // draw the slider itself:
  if (!(flags & FL_INACTIVE)) {
    if (Fl::pushed() == this) flags |= FL_VALUE;
    else if (belowmouse()) flags |= FL_HIGHLIGHT;
  }
  box()->inset(sx, sy, sw, sh);
  Fl_Slider::draw(sx, sy, sw, sh, flags);
}

int Fl_Value_Slider::handle(int event) {
  int sx = 0, sy = 0, sw = w(), sh = h();
  if (vertical()) {
    sy += 25; sh -= 25;
  } else {
    sx += 35; sw -= 35;
  }
  box()->inset(sx, sy, sw, sh);
#if CLICK_MOVES_FOCUS
  if (event == FL_PUSH) take_focus();
#endif
  return Fl_Slider::handle(event, sx, sy, sw, sh);
}

static void revert(Fl_Style* s) {
  s->color = FL_DARK2;
//s->text_size = 10;
}

static Fl_Named_Style* style = new Fl_Named_Style("Value Slider", revert, &style);

Fl_Value_Slider::Fl_Value_Slider(int x, int y, int w, int h, const char*l)
: Fl_Slider(x, y, w, h, l) {
  style(::style);
  step(.01);
}

//
// End of "$Id: Fl_Value_Slider.cxx,v 1.37 2002/01/23 08:46:01 spitzak Exp $".
//
