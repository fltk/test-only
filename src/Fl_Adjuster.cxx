//
// "$Id: Fl_Adjuster.cxx,v 1.7 1999/03/15 18:19:05 carl Exp $"
//
// Adjuster widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Adjuster.H>
#include <FL/Fl_Bitmap.H>
#include <FL/fl_draw.H>

#include "fastarrow.h"
static Fl_Bitmap fastarrow(fastarrow_bits, fastarrow_width, fastarrow_height);
#include "mediumarrow.h"
static Fl_Bitmap mediumarrow(mediumarrow_bits, mediumarrow_width, mediumarrow_height);
#include "slowarrow.h"
static Fl_Bitmap slowarrow(slowarrow_bits, slowarrow_width, slowarrow_height);

#define DEFAULT_STYLE ((Style*)default_style())

void Fl_Adjuster::loadstyle() {
  if (!Fl::s_adjuster) {
    Fl::s_adjuster = 1;

    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "arrow color", COLOR2 },
      { "box", BOX },
      { 0 }
    };
    Fl::load_attributes("adjuster", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute adjuster_attributes[] = {
      { "highlight color", FLY_COLOR },
      { "highlight box", FLY_BOX },
      { 0 }
    };
    Fl::load_attributes("adjuster", DEFAULT_STYLE->adjuster_, adjuster_attributes);
  }
}

// changing the value does not change the appearance:
void Fl_Adjuster::value_damage() {}

void Fl_Adjuster::draw() {
  loadstyle();
  int dx, dy, W, H;
  if (w()>=h()) {
    dx = W = w()/3;
    dy = 0; H = h();
  } else {
    dx = 0; W = w();
    dy = H = h()/3;
  }

  Fl_Color col;
  Fl_Boxtype bt;
  if (fly_box() && Fl::belowmouse() == this)
    { bt = fly_box(); col = fly_color(); }
  else
    { bt = box(); col = color(); }
  fl_draw_box(drag==1?down(bt):bt, x(), y()+2*dy, W, H, col);
  fl_draw_box(drag==2?down(bt):bt, x()+dx, y()+dy, W, H, col);
  fl_draw_box(drag==3?down(bt):bt, x()+2*dx, y(), W, H, col);

  if (active_r())
    fl_color(selection_color());
  else
    fl_color(inactive(selection_color()));
  fastarrow.draw(x()+(W-fastarrow_width)/2,
		 y()+2*dy+(H-fastarrow_height)/2, W, H);
  mediumarrow.draw(x()+dx+(W-mediumarrow_width)/2,
		   y()+dy+(H-mediumarrow_height)/2, W, H);
  slowarrow.draw(x()+2*dx+(W-slowarrow_width)/2,
		 y()+(H-slowarrow_width)/2, W, H);
}

int Fl_Adjuster::handle(int event) {
  double v;
  int delta;
  int mx = Fl::event_x();
  switch (event) {
  case FL_PUSH:
    ix = mx;
    if (w()>=h())
      drag = 3*(mx-x())/w() + 1;
    else
      drag = 3-3*(Fl::event_y()-y()-1)/h();
    handle_push();
    redraw();
    return 1;
  case FL_DRAG:
    if (w() >= h()) {
      delta = x()+(drag-1)*w()/3;	// left edge of button
      if (mx < delta)
	delta = mx-delta;
      else if (mx > (delta+w()/3)) // right edge of button
	delta = mx-delta-w()/3;
      else
	delta = 0;
    } else {
      if (mx < x())
	delta = mx-x();
      else if (mx > (x()+w()))
	delta = mx-x()-w();
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
  case FL_RELEASE:
    if (Fl::event_is_click()) { // detect click but no drag
      if (Fl::event_state()&0xF0000) delta = -10;
      else delta = 10;
      switch (drag) {
      case 3: v = increment(previous_value(), delta); break;
      case 2: v = increment(previous_value(), delta*10); break;
      default:v = increment(previous_value(), delta*100); break;
      }
      handle_drag(soft() ? softclamp(v) : clamp(v));
    }
    drag = 0;
    redraw();
    handle_release();
    return 1;
  case FL_ENTER:
  case FL_LEAVE:
    if (fly_box()) redraw();
    return 1;
  }
  return 0;
}

Fl_Adjuster::Style Fl_Adjuster::_default_style;

Fl_Adjuster::Style::Style() : Fl_Widget::Style() {
  sbf = 0;

  widget(BOX) = FL_MEDIUM_UP_BOX;
  widget(COLOR2) = FL_BLACK;

  adjuster(FLY_COLOR) = 51;
  adjuster(FLY_BOX) = FL_MEDIUM_UP_BOX;
}

Fl_Adjuster::Fl_Adjuster(int x,int y,int w,int h,const char *l) : Fl_Valuator(x,y,w,h,l) {
  step(1, 10000);
  drag = 0;
  soft_ = 1;
}

Fl_Color Fl_Adjuster::fly_color() const {
  if (!_style || !(ADJUSTER_STYLE->sbf & bf(FLY_COLOR)))
    return (Fl_Color)DEFAULT_STYLE->adjuster(FLY_COLOR);
  return (Fl_Color)ADJUSTER_STYLE->adjuster(FLY_COLOR);
}

Fl_Boxtype Fl_Adjuster::fly_box() const {
  if (_style && (WIDGET_STYLE->sbf & bf(BOX)) && !(ADJUSTER_STYLE->sbf & bf(FLY_BOX)))
    return (Fl_Boxtype)WIDGET_STYLE->widget(BOX);
  if (!_style || !(ADJUSTER_STYLE->sbf & bf(FLY_BOX)))
    return (Fl_Boxtype)DEFAULT_STYLE->adjuster(FLY_BOX);
  return (Fl_Boxtype)ADJUSTER_STYLE->adjuster(FLY_BOX);
}

//
// End of "$Id: Fl_Adjuster.cxx,v 1.7 1999/03/15 18:19:05 carl Exp $".
//
