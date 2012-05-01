//
// "$Id$"
//
// Adjuster widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <fltk3/run.h>
#include <fltk3/Adjuster.h>
#include <fltk3/Bitmap.h>
#include <fltk3/draw.h>
#include <fltk3/Wrapper.h>


// three bitmaps for the arrow images
#include "fastarrow.h"
static fltk3::Bitmap fastarrow(fastarrow_bits, fastarrow_width, fastarrow_height);
#include "mediumarrow.h"
static fltk3::Bitmap mediumarrow(mediumarrow_bits, mediumarrow_width, mediumarrow_height);
#include "slowarrow.h"
static fltk3::Bitmap slowarrow(slowarrow_bits, slowarrow_width, slowarrow_height);


// changing the value does not change the appearance:
void fltk3::Adjuster::value_damage() 
{
}


void fltk3::Adjuster::draw() 
{
  int dx, dy, W, H, hor = (w()>=h());
  if (hor) {
    dx = W = w()/3;
    dy = 0; H = h();
  } else {
    dx = 0; W = w();
    dy = H = h()/3;
  }
  fltk3::Boxtype up = box(), dn = fltk3::down(box());
  draw_box(Boxtype((drag==1?dn:up)|(hor?TIE_RIGHT:TIE_TOP)), 0, 2*dy, W, H, color());
  draw_box(Boxtype((drag==2?dn:up)|(hor?TIE_LEFT|TIE_RIGHT:TIE_TOP|TIE_BOTTOM)), dx, dy, W, H, color());
  draw_box(Boxtype((drag==3?dn:up)|(hor?TIE_LEFT:TIE_BOTTOM)), 2*dx, 0, W, H, color());
  if (active_r())
    fltk3::color(selection_color());
  else
    fltk3::color(fltk3::inactive(selection_color()));
  fastarrow.draw((W-fastarrow_width)/2, 2*dy+(H-fastarrow_height)/2, W, H);
  mediumarrow.draw(dx+(W-mediumarrow_width)/2, dy+(H-mediumarrow_height)/2, W, H);
  slowarrow.draw(2*dx+(W-slowarrow_width)/2, (H-slowarrow_width)/2, W, H);
  if (fltk3::focus() == this) draw_focus();
}


int fltk3::Adjuster::handle(int event) 
{
  double v;
  int delta;
  int mx = fltk3::event_x();
  // fltk3::WidgetTracker wp(this);
  switch (event) {
    case fltk3::PUSH:
      if (fltk3::visible_focus()) fltk3::focus(this);
      ix = mx;
      if (w()>=h())
	drag = (3*mx)/w() + 1;
      else
	drag = 3-3*(fltk3::event_y()-1)/h();
    { fltk3::WidgetTracker wp(this);
      handle_push();
      if (wp.deleted()) return 1;
    }
      redraw();
      return 1;
    case fltk3::DRAG:
      if (w() >= h()) {
	delta = (drag-1)*w()/3;	// left edge of button
	if (mx < delta)
	  delta = mx-delta;
	else if (mx > (delta+w()/3)) // right edge of button
	  delta = mx-delta-w()/3;
	else
	  delta = 0;
      } else {
	if (mx < 0)
	  delta = mx-x();
	else if (mx > +w())
	  delta = mx-w();
	else
	  delta = 0;
      }
      switch (drag) {
	case 3: v = increment(previous_value(), delta); break;
	case 2: v = increment(previous_value(), delta*10); break;
	default:v = increment(previous_value(), delta*100); break;
      }
      handle_drag(soft() ? softclamp(v) : clamp(v));
      return 1;
    case fltk3::RELEASE:
      if (fltk3::event_is_click()) { // detect click but no drag
	if (fltk3::event_state()&0xF0000) delta = -10;
	else delta = 10;
	switch (drag) {
	  case 3: v = increment(previous_value(), delta); break;
	  case 2: v = increment(previous_value(), delta*10); break;
	  default:v = increment(previous_value(), delta*100); break;
	}
	fltk3::WidgetTracker wp(this);
	handle_drag(soft() ? softclamp(v) : clamp(v));
	if (wp.deleted()) return 1;
      }
      drag = 0;
      redraw();
      handle_release();
      return 1;
    case fltk3::KEYBOARD :
      switch (fltk3::event_key()) {
	case fltk3::UpKey:
          if (w() > h()) return 0;
	  handle_drag(clamp(increment(value(),-1)));
	  return 1;
	case fltk3::DownKey:
          if (w() > h()) return 0;
	  handle_drag(clamp(increment(value(),1)));
	  return 1;
	case fltk3::LeftKey:
          if (w() < h()) return 0;
	  handle_drag(clamp(increment(value(),-1)));
	  return 1;
	case fltk3::RightKey:
          if (w() < h()) return 0;
	  handle_drag(clamp(increment(value(),1)));
	  return 1;
	default:
          return 0;
      }
      // break not required because of switch...
      
    case fltk3::FOCUS:
    case fltk3::UNFOCUS:
      if (fltk3::visible_focus()) {
        redraw();
        return 1;
      } else return 0;
      
    case fltk3::ENTER :
    case fltk3::LEAVE :
      return 1;
  }
  return 0;
}

fltk3::Adjuster::Adjuster(int X, int Y, int W, int H, const char* l)
: fltk3::Valuator(X, Y, W, H, l) 
{
  set_group_relative(); // FIXME: remove later
  box(fltk3::UP_BOX);
  step(1, 10000);
  selection_color(fltk3::SELECTION_COLOR);
  drag = 0;
  soft_ = 1;
}

//
// End of "$Id$".
//
