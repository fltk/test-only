//
// "$Id: Fl_Value_Slider.cxx,v 1.39 2002/02/25 09:00:22 spitzak Exp $"
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
#include <fltk/Fl_Group.h>
#include <config.h>

extern void fl_dotted_box(int,int,int,int);

void Fl_Value_Slider::draw() {
  // figure out region of box, leaving room for tick marks:
  int bx = 0, by = 0, bw = w(), bh = h();
  int tick_size = 0;
  if (horizontal()) {
    switch (type()&TICK_BOTH) {
    case TICK_BOTH: by = tick_size = h()/4; bh -= 2*tick_size; break;
    case TICK_ABOVE: by = tick_size = h()/3; bh -= tick_size; break;
    case TICK_BELOW: tick_size = h()/3; bh -= tick_size; break;
    }
  } else {
    switch (type()&TICK_BOTH) {
    case TICK_BOTH: bx = tick_size = w()/4; bw -= 2*tick_size; break;
    case TICK_ABOVE: bx = tick_size = w()/3; bw -= tick_size; break;
    case TICK_BELOW: tick_size = w()/3; bw -= tick_size; break;
    }
  }

  // figure out the inner size of the slider and text areas:
  Fl_Boxtype box = this->box();
  int ix = bx, iy = by, iw = bw, ih = bh;
  box->inset(ix,iy,iw,ih);
  int tx = ix, ty = iy, tw = iw, th = ih;
  if (horizontal()) {
    tw = 35; ix += tw; iw -= tw;
  } else {
    th = text_size(); ih -= th; ty += ih;
  }

  Fl_Flags flags = 0;
  if (!active_r()) {
    flags |= FL_INACTIVE;
  } else {
    //if (Fl::pushed() == this) f |= FL_VALUE;
    if (belowmouse()) flags |= FL_HIGHLIGHT;
  }

  // minimal-update the slider, if it indicates the background needs
  // to be drawn, draw that:
  if (Fl_Slider::draw(ix, iy, iw, ih, flags, iy==by)) {
    // draw the box or the visible parts of the window
    if (!box->fills_rectangle()) parent()->draw_group_box();
    box->draw(bx, by, bw, bh, color(), flags);

    // draw the focus indicator inside the box:
    if (focused()) {
      fl_color(text_color());
      fl_dotted_box(ix+1, iy+1, iw-2, ih-2);
    }

    if (tick_size && (damage() & FL_DAMAGE_ALL)) {
      // first clip to the background area and erase it with draw_group_box:
      fl_clip_out(bx, by, bw, bh);
      if (box->fills_rectangle()) parent()->draw_group_box();
      // now draw the ticks into the clipped area:
      if (horizontal()) draw_ticks(ix, 0, iw, h());
      else draw_ticks(0, iy, w(), ih);
    }

    fl_pop_clip();
  }

  // draw the text:
  if (damage() & (FL_DAMAGE_ALL|FL_DAMAGE_VALUE)) {
    fl_push_clip(tx, ty, tw, th);
    // erase the background if not already done:
    if (!(damage()&FL_DAMAGE_ALL)) {
      if (!box->fills_rectangle()) parent()->draw_group_box();
      box->draw(bx, by, bw, bh, color(), flags);
    }
    // now draw the text:
    char buf[128];
    format(buf);
    fl_font(text_font(), text_size());
    fl_color(fl_inactive(text_color(),flags));
    fl_draw(buf, tx, ty, tw, th, 0);
    fl_pop_clip();
  }

}

int Fl_Value_Slider::handle(int event) {
  // figure out the inner size of the slider and text areas:
  Fl_Boxtype box = this->box();
  int ix = 0, iy = 0, iw = w(), ih = h();
  box->inset(ix,iy,iw,ih);
  if (horizontal()) {
    int tw = 35; ix += tw; iw -= tw;
  } else {
    int th = text_size(); ih -= th;
  }
#if CLICK_MOVES_FOCUS
  if (event == FL_PUSH) take_focus();
#endif
  return Fl_Slider::handle(event, ix, iy, iw, ih);
}

Fl_Value_Slider::Fl_Value_Slider(int x, int y, int w, int h, const char*l)
: Fl_Slider(x, y, w, h, l) {
  step(.01);
}

//
// End of "$Id: Fl_Value_Slider.cxx,v 1.39 2002/02/25 09:00:22 spitzak Exp $".
//
