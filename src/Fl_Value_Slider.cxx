//
// "$Id: Fl_Value_Slider.cxx,v 1.54 2004/06/11 08:07:19 spitzak Exp $"
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <config.h>
#include <fltk/ValueSlider.h>
#include <fltk/damage.h>
#include <fltk/draw.h>
#include <fltk/Box.h>
using namespace fltk;

/*! \class fltk::ValueSlider

  The fltk::ValueSlider widget is a fltk::Slider widget with an area
  to the left or bottom displaying the current value.

  \image html value_slider.gif
*/

void ValueSlider::draw() {

  // figure out the inner size of the box:
  Box* box = this->box();
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
    th = int(textsize()); sh -= th; ty += sh;
    if (ix) {tx = ix; tw = iw;} // if box has border, center text
  }

  Flags f2 = current_flags_highlight();
  Flags flags = f2 & ~HIGHLIGHT;
  if (pushed()) f2 |= VALUE;

  // minimal-update the slider, if it indicates the background needs
  // to be drawn, draw that. We draw the slot if the current box type
  // has no border:
#if !NO_CLIP_OUT
  if (Slider::draw(sx, sy, sw, sh, f2, iy==0)) {
#endif

    // draw the box or the visible parts of the window
    if (!box->fills_rectangle()) draw_background();
    box->draw(0, 0, w(), h(), style(), flags|OUTPUT);

    // draw the focus indicator inside the box:
    if (focused()) {
      focusbox()->draw(ix+1, iy+1, iw-2, ih-2, style(), INVISIBLE);
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
      Color color = textcolor();
      if (flags&INACTIVE) color = inactive(color);
      setcolor(color);
      draw_ticks(sx, sy, sw, sh, (slider_size()+1)/2);
    }

#if NO_CLIP_OUT
    Slider::draw(sx, sy, sw, sh, f2, iy==0);
#else
    pop_clip();
  }
#endif

  // draw the text:
  if (damage() & (DAMAGE_ALL|DAMAGE_VALUE)) {
    push_clip(tx, ty, tw, th);
    // erase the background if not already done:
    if (!(damage()&DAMAGE_ALL)) {
      if (!box->fills_rectangle()) draw_background();
      box->draw(0, 0, w(), h(), style(), flags|OUTPUT);
      if (focused()) {
	focusbox()->draw(ix+1, iy+1, iw-2, ih-2, style(), INVISIBLE);
      }
    }
    // now draw the text:
    char buf[128];
    format(buf);
    setfont(textfont(), textsize());
    setcolor(inactive(textcolor(),flags));
    drawtext(buf, tx, ty, tw, th, 0);
    pop_clip();
  }

}

int ValueSlider::handle(int event) {
  // figure out the inner size of the slider and text areas:
  Box* box = this->box();
  int ix = 0, iy = 0, iw = w(), ih = h();
  box->inset(ix,iy,iw,ih);
  if (horizontal()) {
    int tw = 35; ix += tw; iw -= tw;
  } else {
    int th = int(textsize()); ih -= th;
  }
  return Slider::handle(event, ix, iy, iw, ih);
}

static void revert(Style *s) {
  s->color_ = GRAY75;
  s->box_ = FLAT_BOX;
  //s->glyph_ = ::glyph;
}
static NamedStyle style("ValueSlider", revert, &ValueSlider::default_style);
NamedStyle* ValueSlider::default_style = &::style;

ValueSlider::ValueSlider(int x, int y, int w, int h, const char*l)
: Slider(x, y, w, h, l) {
  if (!default_style->glyph_) default_style->glyph_ = style()->glyph_;
  style(default_style);
  step(.01);
  //set_click_to_focus();
}

//
// End of "$Id: Fl_Value_Slider.cxx,v 1.54 2004/06/11 08:07:19 spitzak Exp $".
//
