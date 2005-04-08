//
// "$Id$"
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
  Flags flags = update_flags();
  Box* box = this->box();
  Rectangle r(w(),h()); box->inset(r, style(), flags);
  Rectangle sr(r);

  // figure out where to draw the slider, leaving room for tick marks:
  if (tick_size() && (type()&TICK_BOTH)) {
    if (horizontal()) {
      sr.move_b(-tick_size());
      switch (type()&TICK_BOTH) {
      case TICK_BOTH: sr.y(sr.y()+tick_size()/2); break;
      case TICK_ABOVE: sr.y(sr.y()+tick_size()); break;
      }
    } else {
      sr.move_r(-tick_size());
      switch (type()&TICK_BOTH) {
      case TICK_BOTH: sr.x(sr.x()+tick_size()/2); break;
      case TICK_ABOVE: sr.x(sr.x()+tick_size()); break;
      }
    }
  }

  Flags f2 = flags & ~FOCUSED;
  if (pushed()) f2 |= VALUE|PUSHED;
  flags &= ~HIGHLIGHT;

  // figure out where to draw the text:
  Rectangle tr(sr);
  if (horizontal()) {
    tr.w(35); sr.move_x(35);
    if (r.y()) {tr.y(r.y()); tr.h(r.h());} // if box has border, center text
  } else {
    tr.h(int(textsize())); sr.move_b(-tr.h()); tr.y(sr.b());
    if (r.x()) {tr.x(r.x()); tr.w(r.w());} // if box has border, center text
  }

  // minimal-update the slider, if it indicates the background needs
  // to be drawn, draw that. We draw the slot if the current box type
  // has no border:
  bool drawslot = r.y() == 0;
#if USE_CLIPOUT
  if (Slider::draw(sr, f2, drawslot)) {
#endif

    // draw the box or the visible parts of the window
    if (!box->fills_rectangle()) draw_background();
    box->draw(Rectangle(w(), h()), style(), flags|OUTPUT);

    // draw the focus indicator inside the box:
    focusbox()->draw(r, style(), flags|OUTPUT);

    if (type() & TICK_BOTH) {
      Rectangle tr(sr);
      if (horizontal()) {
	switch (type()&TICK_BOTH) {
	case TICK_ABOVE: tr.y(r.y()); tr.set_b(sr.center_y()); break;
	case TICK_BELOW: tr.y(sr.center_y()+(drawslot?3:0)); tr.set_b(r.b()-1); break;
	}
      } else {
	switch (type()&TICK_BOTH) {
	case TICK_ABOVE: tr.x(r.x()); tr.set_r(sr.center_x()); break;
	case TICK_BELOW: tr.x(sr.center_x()+(drawslot?3:0)); tr.set_r(r.r()-1); break;
	}
      }
      Color color = textcolor();
      if (flags&INACTIVE) color = inactive(color);
      setcolor(color);
      draw_ticks(tr, (slider_size()+1)/2);
    }

#if !USE_CLIPOUT
    Slider::draw(sr, f2, drawslot);
#else
    pop_clip();
  }
#endif

  // draw the text:
  if (damage() & (DAMAGE_ALL|DAMAGE_VALUE|DAMAGE_HIGHLIGHT)) {
    push_clip(tr);
    // erase the background if not already done:
    if (!(damage()&DAMAGE_ALL)) {
      if (!box->fills_rectangle()) draw_background();
      box->draw(Rectangle(w(), h()), style(), flags|OUTPUT);
      focusbox()->draw(r, style(), flags|OUTPUT);
    }
    // now draw the text:
    char buf[128];
    format(buf);
    setfont(textfont(), textsize());
    setcolor(inactive(textcolor(),flags));
    drawtext(buf, tr, 0);
    pop_clip();
  }
}

int ValueSlider::handle(int event) {
  // figure out the inner size of the slider and text areas:
  Box* box = this->box();
  Rectangle r(w(),h()); box->inset(r,style(),flags());
  if (horizontal()) {
    int tw = 35; r.move_x(tw);
  } else {
    int th = int(textsize()); r.move_b(-th);
  }
  return Slider::handle(event, r);
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
// End of "$Id$".
//
