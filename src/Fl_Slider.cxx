//
// "$Id: Fl_Slider.cxx,v 1.55 2002/02/25 09:00:22 spitzak Exp $"
//
// Slider widget for the Fast Light Tool Kit (FLTK).
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
#include <fltk/Fl_Slider.h>
#include <fltk/Fl_Group.h>
#include <fltk/fl_draw.h>
#include <math.h>
#include <config.h>

extern void fl_dotted_box(int,int,int,int);

// Return the location of the left/top edge of a box of slider_size() would
// be if the area the slider can move in is of width/height w.
int Fl_Slider::slider_position(double value, int w) {
  w -= slider_size_; if (w <= 0 || minimum() == maximum()) return 0;
  double val = (value-minimum())/(maximum()-minimum());
  if (!horizontal()) val = 1-val;
  if (val >= 1) return w;
  else if (val <= 0) return 0;
  else return int(val*w+.5);
}

// Return the value if the left/top edge of a box of slider_size() is placed
// at this location in an area of width/height w:
double Fl_Slider::position_value(int X, int w) {
  w -= slider_size_; if (w <= 0) return minimum();
  if (!horizontal()) X = w-X;
  return round(X*(maximum()-minimum())/w + minimum());
}

// Draw tick marks. These lines cross the passed rectangle perpendicular to
// the slider direction. In the direction parallel to the slider direction
// the box should have the same size as the area the slider moves in.
void Fl_Slider::draw_ticks(int x, int y, int w, int h)
{
  int x1, y1, x2, y2, dx, dy;
  if (horizontal()) {
    x1 = x2 = x+(slider_size_-1)/2; dx = 1;
    y1 = y; y2 = y + h; dy = 0;
  } else {
    x1 = x; x2 = x+w; dx = 0;
    y1 = y2 = y+(slider_size_-1)/2; dy = 1;
    w = h;
  }
  if (w <= 0 || minimum() == maximum()) return;
  int incr = pagesize(); if (incr < 1) incr = 1;
  Fl_Color color = text_color();
  if (!active_r()) color = fl_inactive(color);
  fl_color(color);
  for (double v = minimum();
       v >= minimum() && v <= maximum();
       v = increment(v,incr)) {
    int t = slider_position(v, w);
    fl_line(x1+dx*t, y1+dy*t, x2+dx*t, y2+dy*t);
  }
}

void Fl_Slider::draw()
{
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

  // figure out the inner size of the box:
  Fl_Boxtype box = this->box();
  int ix = bx, iy = by, iw = bw, ih = bh;
  box->inset(ix,iy,iw,ih);

  Fl_Flags flags = 0;
  if (!active_r()) {
    flags |= FL_INACTIVE;
  } else {
    //if (Fl::pushed() == this) f |= FL_VALUE;
    if (belowmouse()) flags |= FL_HIGHLIGHT;
  }

  // minimal-update the slider, if it indicates the background needs
  // to be drawn, draw that:
  if (draw(ix, iy, iw, ih, flags,
	   iy==by // draw slot if it seems box has no border
	   )) {
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
}

// Do minimal-update redraw of the moving part of the slider. If this returnes
// true then it has done an fl_push_clip() and you must fill in the remaining
// area with the background that goes behind the slider. The clipped area
// will either be the entire widget or the area the slider used to be in,
// with the area of the new slider and the slot removed from it.

bool Fl_Slider::draw(int ix, int iy, int iw, int ih, Fl_Flags flags, bool slot)
{
  // if user directly set selected_color we use it:
  if (style()->selection_color) flags |= FL_SELECTED;

  // figure out where the slider should be:
  int sx = ix, sy = iy, sw = iw, sh = ih;
  int sp;
  if (horizontal()) {
    sx = sp = ix+slider_position(value(),iw);
    sw = slider_size_;
  } else {
    sy = sp = iy+slider_position(value(),ih);
    sh = slider_size_;
  }

  if (damage()&FL_DAMAGE_ALL) {

    fl_push_clip(0, 0, w(), h());
    draw_glyph(0, sx, sy, sw, sh, flags); // draw the slider
    fl_clip_out(sx, sy, sw, sh); // clip out the area of the slider

  } else if (sp != old_position) {

    // update a moving slider:
    draw_glyph(0, sx, sy, sw, sh, flags); // draw slider in new position
    // clip to the region the old slider was in:
    if (horizontal()) fl_push_clip(old_position, sy, sw, sh);
    else fl_push_clip(sx, old_position, sw, sh);
    fl_clip_out(sx, sy, sw, sh); // don't erase new slider
    
  } else {

    // update for the highlight turning on/off
    if (damage() & FL_DAMAGE_HIGHLIGHT) draw_glyph(0, sx, sy, sw, sh, flags);
    // otherwise no changes
    return false;

  }
  old_position = sp;

  // we draw a slot if it seems the box has no border:
  if (slot) {
    const int slot_size_ = 6;
    int slx, sly, slw, slh;
    if (horizontal()) {
      slx = (slider_size_-slot_size_)/2;
      slw = iw-2*slx;
      slx += ix;
      sly = iy+(ih-slot_size_+1)/2;
      slh = slot_size_;
    } else {
      sly = (slider_size_-slot_size_)/2;
      slh = ih-2*sly;
      sly += iy;
      slx = ix+(iw-slot_size_+1)/2;
      slw = slot_size_;
    }
    button_box()->draw(slx, sly, slw, slh, FL_BLACK,
		       flags&FL_INACTIVE|FL_VALUE);
    fl_clip_out(slx, sly, slw, slh);
  }
  return true;
}

int Fl_Slider::handle(int event, int x, int y, int w, int h) {

  switch (event) {
  case FL_FOCUS:
  case FL_UNFOCUS:
    redraw(FL_DAMAGE_ALL);
    return 1;
  case FL_PUSH:
    handle_push();
  case FL_DRAG: {
    // figure out the space the slider moves in and where the event is:
    int mx;
    if (horizontal()) {
      w = w-box()->dw();
      mx = Fl::event_x()-x-box()->dx();
    } else {
      w = h-box()->dh();
      mx = Fl::event_y()-y-box()->dy();
    }
    if (w <= slider_size_) return 1;
    static int offcenter;
    int X = slider_position(value(), w);
    if (event == FL_PUSH) {
      offcenter = mx-X;
      if (offcenter < 0) offcenter = 0;
      else if (offcenter > slider_size_) offcenter = slider_size_;
      else return 1;
    }
    X = mx-offcenter;
    double v;
  RETRY:
    if (X < 0) {
      X = 0;
      offcenter = mx; if (offcenter < 0) offcenter = 0;
    } else if (X > (w-slider_size_)) {
      X = w-slider_size_;
      offcenter = mx-X; if (offcenter > slider_size_) offcenter = slider_size_;
    }
    v = position_value(X, w);
    // make sure a click outside the sliderbar moves it:
    if (event == FL_PUSH && v == value()) {
      offcenter = slider_size_/2;
      event = FL_DRAG;
      goto RETRY;
    }
    handle_drag(clamp(v));
    return 1;}
  case FL_RELEASE:
    handle_release();
    return 1;
  case FL_KEY:
    // Only arrows in the correct direction are used.  This allows the
    // opposite arrows to be used to navigate between a set of parellel
    // sliders.
    switch (Fl::event_key()) {
    case FL_Up:
    case FL_Down:
      if (horizontal()) return 0;
      break;
    case FL_Left:
    case FL_Right:
      if (!horizontal()) return 0;
    }
  default:
    return Fl_Valuator::handle(event);
  }
}

int Fl_Slider::handle(int event) {return handle(event,0,0,w(),h());}

static void glyph(const Fl_Widget* widget, int glyph,
		  int x,int y,int w,int h, Fl_Flags flags)
{
  Fl_Widget::default_glyph(widget, glyph, x, y, w, h, flags);
  // draw the divider line into slider:
  if (!glyph) {
    if (w < 4 || h < 4) return;
    if (widget->type()&1) { // horizontal
      x = x+(w+1)/2;
      fl_color(FL_DARK3);
      fl_line(x-1, y+1, x-1, y+h-2);
      fl_color(FL_LIGHT3);
      fl_line(x, y+1, x, y+h-2);
    } else {
      y = y+(h+1)/2;
      fl_color(FL_DARK3);
      fl_line(x+1, y-1, x+w-2, y-1);
      fl_color(FL_LIGHT3);
      fl_line(x+1, y, x+w-2, y);
    }
  }
}

static void revert(Fl_Style *s) {
  s->color = FL_GRAY;
  s->box = FL_FLAT_BOX;
  s->glyph = ::glyph;
}
static Fl_Named_Style style("Slider", revert, &Fl_Slider::default_style);
Fl_Named_Style* Fl_Slider::default_style = &::style;

Fl_Slider::Fl_Slider(int x, int y, int w, int h, const char* l)
: Fl_Valuator(x, y, w, h, l) {
  style(default_style);
  slider_size_ = 12;
}

//
// End of "$Id: Fl_Slider.cxx,v 1.55 2002/02/25 09:00:22 spitzak Exp $".
//
