//
// "$Id: Fl_Slider.cxx,v 1.61 2002/07/01 15:28:19 spitzak Exp $"
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
#include <fltk/math.h>
#include <config.h>

// Return the location of the left/top edge of a box of slider_size() would
// be if the area the slider can move in is of width/height w.
int Fl_Slider::slider_position(double value, int w) {
  double A = minimum();
  double B = maximum();
  if (B == A) return 0;
  bool flip = B < A;
  if (flip) {A = B; B = minimum();}
  if (!horizontal()) flip = !flip;
  // if both are negative, make the range positive:
  if (B <= 0) {flip = !flip; double t = A; A = -B; B = -t; value = -value;}
  double fraction;
  if (!log()) {
    // linear slider
    fraction = (value-A)/(B-A);
  } else if (A > 0) {
    // logatithmic slider
    if (value <= A) fraction = 0;
    else fraction = (::log(value)-::log(A))/(::log(B)-::log(A));
  } else if (A == 0) {
    // squared slider
    if (value <= 0) fraction = 0;
    else fraction = sqrt(value/B);
  } else {
    // squared signed slider
    if (value < 0) fraction = (1-sqrt(value/A))*.5;
    else fraction = (1+sqrt(value/B))*.5;
  }
  if (flip) fraction = 1-fraction;
  w -= slider_size_; if (w <= 0) return 0;
  if (fraction >= 1) return w;
  else if (fraction <= 0) return 0;
  else return int(fraction*w+.5);
}

// Return the value if the left/top edge of a box of slider_size() is placed
// at this location in an area of width/height w:
double Fl_Slider::position_value(int X, int w) {
  w -= slider_size_; if (w <= 0) return minimum();
  double A = minimum();
  double B = maximum();
  bool flip = B < A;
  if (flip) {A = B; B = minimum();}
  if (!horizontal()) flip = !flip;
  if (flip) X = w-X;
  double fraction = double(X)/w;
  // if both are negative, make the range positive:
  flip = (B <= 0);
  if (flip) {double t = A; A = -B; B = -t; fraction = 1-fraction;}
  double value;
  double derivative;
  if (!log()) {
    // linear slider
    value = fraction*(B-A)+A;
    derivative = (B-A)/w;
  } else if (A > 0) {
    // log slider
    double d = (::log(B)-::log(A));
    value = exp(fraction*d+::log(A));
    derivative = value*d/w;
  } else if (A == 0) {
    // squared slider
    value = fraction*fraction*B;
    derivative = 2*fraction*B/w;
  } else {
    // squared signed slider
    fraction = 2*fraction - 1;
    if (fraction < 0) B = A;
    value = fraction*fraction*B;
    derivative = 4*fraction*B/w;
  }
  // find nicest multiple of 10,5, or 2 of step() that is close to 1 pixel:
  if (step() && derivative > step()) {
    double w = log10(derivative);
    double l = ceil(w);
    int num = 1;
    int i; for (i = 0; i < l; i++) num *= 10;
    int denom = 1;
    for (i = -1; i >= l; i--) denom *= 10;
    if (l-w > 0.69897) denom *= 5;
    else if (l-w > 0.30103) denom *= 2;
    return rint(value*denom/num)*num/denom;
  }
  return value;
}

// Draw tick marks. These lines cross the passed rectangle perpendicular to
// the slider direction. In the direction parallel to the slider direction
// the box should have the same size as the area the slider moves in.
void Fl_Slider::draw_ticks(int x, int y, int w, int h, int min_spacing)
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
  if (w <= 0) return;
  Fl_Color color = text_color();
  if (!active_r()) color = fl_inactive(color);
  fl_color(color);
  double A = minimum();
  double B = maximum();
  if (A > B) {A = B; B = minimum();}

  // Figure out approximate size of min_spacing at zero:
  double derivative;
  if (!log()) {
    derivative = (B-A)/w;
  } else if (A > 0) {
    // log slider
    derivative = A/w*20;
  } else {
    // squared slider, derivative at edge is zero, use value at 1 pixel
    derivative = B/(w*w)*30;
    if (A < 0) derivative *= 4;
  }
  derivative *= min_spacing;
  if (derivative < step()) derivative = step();

  // Find closest multiple of 10 larger than spacing:
  int num = 1;
  while (num < derivative) num *= 10;
  int denom = 10;
  while (num >= derivative*denom) denom *= 10;
  denom /= 10;

  for (int n = 0; ; n++) {
    // every ten they get further apart for log slider:
    if (log() && n > 10) {n = 2; num *= 10;}
    double v = double(num*n)/denom;
    if (v > fabs(A) && v > fabs(B)) break;
    if (v >= A && v <= B) {
      int t = slider_position(v, w);
      fl_line(x1+dx*t, y1+dy*t, x2+dx*t, y2+dy*t);
      if (v == 0 || v == 1) {
	fl_font(text_font(), text_size());
	fl_draw(v?"1":"0", x1+dx*t+1, y1+dy*t+fl_size()-fl_descent()+1);
      }
    }
    if (-v >= A && -v <= B) {
      int t = slider_position(-v, w);
      fl_line(x1+dx*t, y1+dy*t, x2+dx*t, y2+dy*t);
    }
  }
}

void Fl_Slider::draw()
{
  // figure out the inner size of the box:
  Fl_Boxtype box = this->box();
  int ix = 0, iy = 0, iw = w(), ih = h();
  box->inset(ix,iy,iw,ih);

  // figure out where to draw the slider, leaving room for tick marks:
  int sx = ix, sy = iy, sw = iw, sh = ih;
  if (tick_size_ && (type()&TICK_BOTH)) {
    if (horizontal()) {
      sh -= tick_size_;
      switch (type()&TICK_BOTH) {
      case TICK_BOTH: sy += tick_size_/2; break;
      case TICK_ABOVE: sy += tick_size_; break;
      }
    } else {
      sw -= tick_size_;
      switch (type()&TICK_BOTH) {
      case TICK_BOTH: sx += tick_size_/2; break;
      case TICK_ABOVE: sx += tick_size_; break;
      }
    }
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
  if (draw(sx, sy, sw, sh, flags, iy==0)) {

    // draw the box or the visible parts of the window
    if (!box->fills_rectangle()) parent()->draw_group_box();
    box->draw(0, 0, w(), h(), color(), flags);

    // draw the focus indicator inside the box:
    if (focused()) {
      focus_box()->draw(ix+1, iy+1, iw-2, ih-2, label_color(), FL_INVISIBLE);
    }

    if (type() & TICK_BOTH) {
      if (horizontal()) {
	switch (type()&TICK_BOTH) {
	case TICK_ABOVE: ih = sy+sh/2-iy; break;
	case TICK_BELOW: ih += iy; iy = sy+sh/2+(iy?0:3); ih -= iy; break;
	}
      } else {
	switch (type()&TICK_BOTH) {
	case TICK_ABOVE: iw = sx+sw/2-ix; break;
	case TICK_BELOW: iw += ix; ix = sx+sw/2+(iy?0:3); iw -= ix; break;
	}
      }
      draw_ticks(ix, iy, iw, ih, slider_size_);
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
  // for back compatability, use type flag to set slider size:
  if (type()&FILL) slider_size(0);

  // if user directly set selected_color we use it:
  if (style()->selection_color) flags |= FL_SELECTED;

  // figure out where the slider should be:
  int sx = ix, sy = iy, sw = iw, sh = ih;
  int sp;
  if (horizontal()) {
    sx = sp = ix+slider_position(value(),iw);
    sw = slider_size_;
    if (!sw) {sw = sx-ix; sx = ix;} // fill slider
  } else {
    sy = sp = iy+slider_position(value(),ih);
    sh = slider_size_;
    if (!sh) sh = iy+ih-sy; // fill slider
  }

  if (damage()&FL_DAMAGE_ALL) {

    fl_push_clip(0, 0, w(), h());
    draw_glyph(0, sx, sy, sw, sh, flags); // draw the slider
    fl_clip_out(sx, sy, sw, sh); // clip out the area of the slider

  } else if (sp != old_position) {

    // update a moving slider:
    draw_glyph(0, sx, sy, sw, sh, flags); // draw slider in new position
    // clip to the region the old slider was in:
    if (horizontal()) {
      if (slider_size_) fl_push_clip(old_position, sy, sw, sh);
      else fl_push_clip(ix, iy, old_position, ih);
    } else {
      if (slider_size_) fl_push_clip(sx, old_position, sw, sh);
      else fl_push_clip(ix, old_position, iw, iy+ih-old_position);
    }
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
    int dx = (slider_size_-slot_size_)/2; if (dx < 0) dx = 0;
    if (horizontal()) {
      slx = dx;
      slw = iw-2*dx;
      slx += ix;
      sly = iy+(ih-slot_size_+1)/2;
      slh = slot_size_;
    } else {
      sly = dx;
      slh = ih-2*dx;
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
      // we are done if they clicked on the slider:
      if (offcenter >= (slider_size() ? 0 : -8) && offcenter <= slider_size_)
	return 1;
      if (Fl::event_button() > 1) {
	// Move the near end of the slider to the cursor. This is good
	// for scrollbars.
	offcenter = (offcenter < 0) ? 0 : slider_size_;
      } else {
	// Center the slider under the cursor, what most toolkits do
	offcenter = slider_size_/2;
      }
    }
    double v;
  RETRY:
    X = mx-offcenter;
    if (X < 0) {
      X = 0;
      offcenter = mx; if (offcenter < 0) offcenter = 0;
    } else if (X > (w-slider_size_)) {
      X = w-slider_size_;
      offcenter = mx-X; if (offcenter > slider_size_) offcenter = slider_size_;
    }
    v = position_value(X, w);
    handle_drag(v);
    // make sure a click outside the sliderbar moves it:
    if (event == FL_PUSH && value() == previous_value()) {
      offcenter = slider_size_/2;
      event = FL_DRAG;
      goto RETRY;
    }
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
    if (!((Fl_Slider*)widget)->slider_size()) return; // ignore FILL widgets
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
  s->text_color = FL_DARK3;
  s->text_size = 8;
  s->box = FL_FLAT_BOX;
  s->glyph = ::glyph;
}
static Fl_Named_Style style("Slider", revert, &Fl_Slider::default_style);
Fl_Named_Style* Fl_Slider::default_style = &::style;

Fl_Slider::Fl_Slider(int x, int y, int w, int h, const char* l)
: Fl_Valuator(x, y, w, h, l) {
  style(default_style);
  tick_size_ = 4;
  slider_size_ = 12;
}

//
// End of "$Id: Fl_Slider.cxx,v 1.61 2002/07/01 15:28:19 spitzak Exp $".
//
