//
// "$Id: Fl_Value_Slider.cxx,v 1.44 2002/05/31 16:35:55 robertk Exp $"
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

void Fl_Value_Slider::draw() {

  // figure out the inner size of the box:
  Fl_Boxtype box = this->box();
  int ix = 0, iy = 0, iw = w(), ih = h();
  box->inset(ix,iy,iw,ih);

  // figure out where to draw the slider, leaving room for tick marks:
  int sx = ix, sy = iy, sw = iw, sh = ih;
  if (tick_size() && (type()&TICK_BOTH)) {
    if (horizontal()) {
      sh -= tick_size();
      switch (type()&TICK_BOTH) {
      case TICK_BOTH: sy += tick_size()/2; break;
      case TICK_ABOVE: sy += tick_size(); break;
      }
    } else {
      sw -= tick_size();
      switch (type()&TICK_BOTH) {
      case TICK_BOTH: sx += tick_size()/2; break;
      case TICK_ABOVE: sx += tick_size(); break;
      }
    }
  }

  // figure out where to draw the text:
  int tx = sx, ty = sy, tw = sw, th = sh;
  if (horizontal()) {
    tw = 35; sx += tw; sw -= tw;
    if (iy) {ty = iy; th = ih;} // if box has border, center text
  } else {
    th = text_size(); sh -= th; ty += sh;
    if (ix) {tx = ix; tw = iw;} // if box has border, center text
  }

  Fl_Flags flags = 0;
  if (!active_r()) {
    flags |= FL_INACTIVE;
  } else {
    //if (Fl::pushed() == this) f |= FL_VALUE;
    if (belowmouse()) flags |= FL_HIGHLIGHT;
  }

  // minimal-update the slider, if it indicates the background needs
  // to be drawn, draw that. We draw the slot if the current box type
  // has no border:
  if (Fl_Slider::draw(sx, sy, sw, sh, flags, iy==0)) {

    // draw the box or the visible parts of the window
    if (!box->fills_rectangle()) parent()->draw_group_box();
    box->draw(0, 0, w(), h(), color(), flags);

    // draw the focus indicator inside the box:
    if (focused()) {
      focus_box()->draw(ix+1, iy+1, iw-2, ih-2, text_color(), FL_INVISIBLE);
    }

    if (type() & TICK_BOTH) {
      if (horizontal()) {
	switch (type()&TICK_BOTH) {
	case TICK_ABOVE: sh = sy+sh/2-iy; sy = iy; break;
	case TICK_BELOW: sy = sy+sh/2+(iy?0:3); sh = ih-sy; break;
	case TICK_BOTH: sy = iy; sh = ih; break;
	}
      } else {
	switch (type()&TICK_BOTH) {
	case TICK_ABOVE: sw = sx+sw/2-ix; sx = ix; break;
	case TICK_BELOW: sx = sx+sw/2+(iy?0:3); sw = iw-sx; break;
	case TICK_BOTH: sx = ix; sw = iw; break;
	}
      }
      draw_ticks(sx, sy, sw, sh);
    }

    fl_pop_clip();
  }

  // draw the text:
  if (damage() & (FL_DAMAGE_ALL|FL_DAMAGE_VALUE)) {
    fl_push_clip(tx, ty, tw, th);
    // erase the background if not already done:
    if (!(damage()&FL_DAMAGE_ALL)) {
      if (!box->fills_rectangle()) parent()->draw_group_box();
      box->draw(0, 0, w(), h(), color(), flags);
      if (focused()) {
	focus_box()->draw(ix+1, iy+1, iw-2, ih-2, text_color(), FL_INVISIBLE);
      }
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
  if (event == FL_PUSH && click_to_focus()) take_focus();
  return Fl_Slider::handle(event, ix, iy, iw, ih);
}

static void revert(Fl_Style *s) {
  s->color = FL_GRAY;
  s->box = FL_FLAT_BOX;
  //s->glyph = ::glyph;
}
static Fl_Named_Style style("Value_Slider", revert, &Fl_Value_Slider::default_style);
Fl_Named_Style* Fl_Value_Slider::default_style = &::style;

Fl_Value_Slider::Fl_Value_Slider(int x, int y, int w, int h, const char*l)
: Fl_Slider(x, y, w, h, l) {
  if (!default_style->glyph) default_style->glyph = style()->glyph;
  style(default_style);
  step(.01);
#if CLICK_MOVES_FOCUS
  set_click_to_focus();
#else
  clear_click_to_focus();
#endif
}

//
// End of "$Id: Fl_Value_Slider.cxx,v 1.44 2002/05/31 16:35:55 robertk Exp $".
//
