//
// "$Id: Fl_Slider.cxx,v 1.71 2004/03/05 08:14:18 spitzak Exp $"
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

#include <fltk/Slider.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/math.h>
#include <config.h>
#include <stdio.h>

using namespace fltk;

/*! \class fltk::Slider

  This widget contains a sliding "knob" that controls a
  single floating-point value. Moving the box all the way to the left
  or bottom sets it to the minimum(), and to the top/right to the
  maximum() value. The minimum() may be greater than the maximum()
  in case you want the larger number at the opposite end.

  See Valuator for how to set or get the value or handle callbacks
  when the value changes.

  \image html slider.gif

  The appearance of the base class may be changed in several ways:

  Setting the box() to any value other than the default of FLAT_BOX,
  as shown in the example on the bottom-left, will remove the "slot"
  and draw a box around the slider and the tick marks. The color()
  (which defaults to GRAY75) is used to fill in the area behind the
  slider and tick marks.

  The following bits may be or'd together and given to type():
  - fltk::Slider::VERTICAL : Slider moves vertically (this is the default)
  - fltk::Slider::HORIZONTAL : Slider moves horizontally.
  - fltk::Slider::TICK_ABOVE : Put tick marks above the horizontal slider.
  - fltk::Slider::TICK_LEFT : Put tick marks to the left of a vertical slider, same value as TICK_ABOVE
  - fltk::Slider::TICK_BELOW : Put tick marks below the horizontal slider.
  - fltk::Slider::TICK_RIGHT : Put tick marks to the right of a vertical slider, same value as TICK_BELOW
  - fltk::Slider::TICK_BOTH : Put tick marks on both sides of the slider.
  - fltk::Slider::LOG : Use a logarithimic or power scale for the slider.

  The tick marks are placed the slider_size() or more apart (they are
  also no closer than the step() value). The color of the tick marks
  is controlled by textcolor(), and the font used to draw the numbers
  is textfont() and textsize() (which defaults to 8).

  You can change the glyph() function to change how the moving part is
  drawn. The number 16 is passed to draw the horizontal slider, the
  number 17 is passed to draw the vertical slider. Other numbers are
  passed by the ScrollBar subclass to draw the arrows at the end. The
  glyph() defaults to a function that draws the buttonbox() in the
  buttoncolor() and then draws a divider line across it.

  \image html logslider.gif
  "Log" sliders have a non-uniform scale. This diagram shows some
  examples. The scale is truly logarithmic if both the minimum() and
  the maximum() are non-zero and have the same sign. Otherwise the
  slider position is the square root of the value (or -sqrt(-value)
  for negative values).

*/

/*! \fn void Slider::slider_size(int)

  Set the dimensions of the moving piece of slider. This is measured
  in pixels in a direction parallel to the slider's movement. The
  default value is 12.

  Setting slider_size() to zero will make the slider into a "fill"
  slider that draws a solid bar from the left/bottom to the current
  value. This is useful for indicating progress or as a output
  indicator.
*/

/*! \fn void Slider::tick_size(int)
  The slider is shrunk this many pixels from the widget's width so
  that the tick marks are visible next to it. The default value is 4.
*/

/*! This is used by subclasses to draw the slider correctly.
  Return the location of the left/top edge of a box of slider_size() would
  be if the slider is set to \a value and can move in an area
  of \a w pixels. The range() and log() settings are taken into
  account.
*/
int Slider::slider_position(double value, int w) {
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

/*! This is used by subclasses to handle events correctly:
  Return the value if the left/top edge of a box of slider_size() is placed
  at \a X pixels from the edge of an area of size \a w pixels. The range()
  and log() settings are taken into account, and it also rounds the value
  to multiples of step(), using powers of 10 larger and multiples of 2 or
  5 to get the steps close to 1 pixel so the user is presented with nice
  numerical values.
*/
double Slider::position_value(int X, int w) {
  w -= slider_size_; if (w <= 0) return minimum();
  double A = minimum();
  double B = maximum();
  bool flip = B < A;
  if (flip) {A = B; B = minimum();}
  if (!horizontal()) flip = !flip;
  if (flip) X = w-X;
  double fraction = double(X)/w;
  if (fraction <= 0) return A;
  if (fraction >= 1) return B;
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
    value = floor(value*denom/num+.5)*num/denom;
  }
  if (flip) return -value;
  return value;
}

/*! Draw tick marks. These lines cross the passed rectangle perpendicular to
  the slider direction. In the direction parallel to the slider direction
  the box should have the same size as the area the slider moves in. */
void Slider::draw_ticks(int x, int y, int w, int h, int min_spacing)
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
  if (min_spacing < 1) min_spacing = 10; // fix for fill sliders
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
    int small = n%5 ? 2 : 0;
    if (v >= A && v <= B) {
      int t = slider_position(v, w);
      drawline(x1+dx*t+dy*small, y1+dy*t+dx*small, x2+dx*t, y2+dy*t);
      if (n%10 == 0) {
	char buffer[20]; sprintf(buffer,"%g",v);
	char* p = buffer;
	while (p[0]=='0' && p[1]) p++;
	setfont(textfont(), textsize());
	drawtext(p, x1+dx*t+1, y1+dy*t+getsize()-getdescent());
      }
    }
    if (v && -v >= A && -v <= B) {
      int t = slider_position(-v, w);
      drawline(x1+dx*t+dy*small, y1+dy*t+dx*small, x2+dx*t, y2+dy*t);
      if (n%10 == 0) {
	char buffer[20]; sprintf(buffer+1,"%g",v);
	char* p = buffer+1;
	while (p[0]=='0' && p[1]) p++;
	p--; p[0] = '-';
	setfont(textfont(), textsize());
	drawtext(p, x1+dx*t+1, y1+dy*t+getsize()-getdescent());
      }
    }
  }
}

void Slider::draw()
{
  // figure out the inner size of the box:
  Box* box = this->box();
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

  Flags f2 = current_flags_highlight();
  Flags flags = f2 & ~HIGHLIGHT;
  if (pushed()) f2 |= VALUE;

  // minimal-update the slider, if it indicates the background needs
  // to be drawn, draw that. We draw the slot if the current box type
  // has no border:
  if (draw(sx, sy, sw, sh, f2, iy==0)) {

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
	case TICK_ABOVE: ih = sy+sh/2-iy; break;
	case TICK_BELOW: ih += iy; iy = sy+sh/2+(iy?0:3); ih -= iy; break;
	}
      } else {
	switch (type()&TICK_BOTH) {
	case TICK_ABOVE: iw = sx+sw/2-ix; break;
	case TICK_BELOW: iw += ix; ix = sx+sw/2+(iy?0:3); iw -= ix; break;
	}
      }
      Color color = textcolor();
      if (flags&INACTIVE) color = inactive(color);
      setcolor(color);
      draw_ticks(ix, iy, iw, ih, (slider_size_+1)/2);
    }

    pop_clip();
  }
}

/*! This call is provied so subclasses can draw the moving part inside
  an arbitrary rectangle and then draw arbitrary backgrounds behind
  the moving part.

  Do minimal-update redraw of the moving part of the slider and also
  draw the "slot" if \a slot is true. If this returns true then it has
  done an fltk::push_clip() and you must fill in the remaining area
  with the background that goes behind the slider. The clipped area
  will either be the entire widget or the area the slider used to be
  in, with the area of the new slider and the slot removed from it.

  Typical usage in a Slider subclass:

  \code
void MySlider::draw() {
  // figure out inset box to hold moving part of slider:
  int ix = 10;
  int iy = 10;
  int iw = w()-20;
  int ih = h()-20;
  // draw the moving part:
  if (draw(ix, iy, iw, ih, 0, false)) {
    // we must draw the background:
    draw_spiffy_background(0,0,w(),h());
    // draw the tick marks across whole widget:
    draw_ticks(ix, 0, iw, h());
    fltk::pop_clip();
  }
}
  \endcode
*/

bool Slider::draw(int ix, int iy, int iw, int ih, Flags flags, bool slot)
{
  // for back compatability, use type flag to set slider size:
  if (type()&16/*FILL*/) slider_size(0);

  // if user directly set selected_color we use it:
  if (style()->selection_color_) flags |= SELECTED;

  // figure out where the slider should be:
  int sx = ix, sy = iy, sw = iw, sh = ih;
  int sp;
  int sglyph = 0;
  if (horizontal()) {
    sx = sp = ix+slider_position(value(),iw);
    sw = slider_size_;
    if (!sw) {sw = sx-ix; sx = ix;} // fill slider
    else sglyph=17;
  } else {
    sy = sp = iy+slider_position(value(),ih);
    sh = slider_size_;
    if (!sh) {sh = iy+ih-sy;} // fill slider
    else sglyph=16;
  }

  if (damage()&DAMAGE_ALL) {

    push_clip(0, 0, w(), h());
    draw_glyph(sglyph, sx, sy, sw, sh, flags); // draw the slider
    clip_out(sx, sy, sw, sh); // clip out the area of the slider

  } else if (sp != old_position) {

    // update a moving slider:
    draw_glyph(sglyph, sx, sy, sw, sh, flags); // draw slider in new position
    // clip to the region the old slider was in:
    if (horizontal()) {
      if (slider_size_) push_clip(old_position, sy, sw, sh);
      else push_clip(ix, iy, old_position, ih);
    } else {
      if (slider_size_) push_clip(sx, old_position, sw, sh);
      else push_clip(ix, old_position, iw, iy+ih-old_position);
    }
    clip_out(sx, sy, sw, sh); // don't erase new slider
    
  } else {

    // update for the highlight turning on/off
    if (damage() & DAMAGE_HIGHLIGHT) draw_glyph(sglyph, sx, sy, sw, sh, flags);
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
    THIN_DOWN_BOX->draw(slx, sly, slw, slh, style(), flags&INACTIVE|INVISIBLE);
    setcolor(BLACK);
    fillrect(slx+1,sly+1,slw-2,slh-2);
    clip_out(slx, sly, slw, slh);
  }
  return true;
}

/*! This call is provied so subclasses can draw the moving part inside
  an arbitrary rectangle. */
int Slider::handle(int event, int x, int y, int w, int h) {

  switch (event) {
  case FOCUS:
  case UNFOCUS:
    redraw(DAMAGE_ALL);
    return 1;
  case PUSH:
    redraw(DAMAGE_HIGHLIGHT);
    handle_push();
  case DRAG: {
    // figure out the space the slider moves in and where the event is:
    int mx;
    if (horizontal()) {
      w = w-box()->dw();
      mx = event_x()-x-box()->dx();
    } else {
      w = h-box()->dh();
      mx = event_y()-y-box()->dy();
    }
    if (w <= slider_size_) return 1;
    static int offcenter;
    int X = slider_position(value(), w);
    if (event == PUSH) {
      offcenter = mx-X;
      // we are done if they clicked on the slider:
      if (offcenter >= (slider_size() ? 0 : -8) && offcenter <= slider_size_)
	return 1;
      if (event_button() > 1) {
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
    if (event == PUSH && value() == previous_value()) {
      offcenter = slider_size_/2;
      event = DRAG;
      goto RETRY;
    }
    return 1;}
  case RELEASE:
    handle_release();
    redraw(DAMAGE_HIGHLIGHT);
    return 1;
  case KEY:
    // Only arrows in the correct direction are used.  This allows the
    // opposite arrows to be used to navigate between a set of parellel
    // sliders.
    switch (event_key()) {
    case UpKey:
    case DownKey:
      if (horizontal()) return 0;
      break;
    case LeftKey:
    case RightKey:
      if (!horizontal()) return 0;
    }
  default:
    return Valuator::handle(event);
  }
}

int Slider::handle(int event) {return handle(event,0,0,w(),h());}

static void glyph(int glyph, int x,int y,int w,int h, const Style* style, Flags flags)
{
  if (glyph<100) flags &= ~VALUE;
  Widget::default_glyph(glyph, x, y, w, h, style, flags);
  // draw the divider line into slider:
  if (w < 4 || h < 4) return;
  if (glyph==17) { // horizontal
    x = x+(w+1)/2;
    setcolor(GRAY33);
    drawline(x-1, y+1, x-1, y+h-2);
    setcolor(WHITE);
    drawline(x, y+1, x, y+h-2);
  } else if (glyph==16) { // vertical
    y = y+(h+1)/2;
    setcolor(GRAY33);
    drawline(x+1, y-1, x+w-2, y-1);
    setcolor(WHITE);
    drawline(x+1, y, x+w-2, y);
  }
}

static void revert(Style *s) {
  s->color_ = GRAY75;
  s->textcolor_ = GRAY33;
  s->textsize_ = 8;
  s->box_ = FLAT_BOX;
  s->glyph_ = ::glyph;
}
static NamedStyle style("Slider", revert, &Slider::default_style);
NamedStyle* Slider::default_style = &::style;

Slider::Slider(int x, int y, int w, int h, const char* l)
: Valuator(x, y, w, h, l) {
  style(default_style);
  tick_size_ = 4;
  slider_size_ = 12;
}

//
// End of "$Id: Fl_Slider.cxx,v 1.71 2004/03/05 08:14:18 spitzak Exp $".
//
