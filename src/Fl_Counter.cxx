//
// "$Id$"
//
// Counter widget for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/Counter.h>
#include <fltk3/draw.h>

void fltk3::Counter::draw() {
  FLTK3_OBJECT_VCALLS_WRAPPER(draw(), Draw)
  int i; fltk3::Boxtype boxtype[5];
  fltk3::Color selcolor;

  boxtype[0] = box();
  if (boxtype[0] == fltk3::UP_BOX) boxtype[0] = fltk3::DOWN_BOX;
  if (boxtype[0] == fltk3::THIN_UP_BOX) boxtype[0] = fltk3::THIN_DOWN_BOX;
  for (i=1; i<5; i++)
    if (mouseobj == i)
      boxtype[i] = fltk3::down(box());
    else
      boxtype[i] = box();

  int xx[5], ww[5];
  if (type() == fltk3::NORMAL_COUNTER) {
    int W = w()*15/100;
    xx[1] = x();	 ww[1] = W;
    xx[2] = x()+1*W;     ww[2] = W;
    xx[0] = x()+2*W;     ww[0] = w()-4*W;
    xx[3] = x()+w()-2*W; ww[3] = W;
    xx[4] = x()+w()-1*W; ww[4] = W;
  } else {
    int W = w()*20/100;
    xx[1] = 0;	         ww[1] = 0;
    xx[2] = x();	 ww[2] = W;
    xx[0] = x()+W;	 ww[0] = w()-2*W;
    xx[3] = x()+w()-1*W; ww[3] = W;
    xx[4] = 0;	         ww[4] = 0;
  }

  draw_box(boxtype[0], xx[0], y(), ww[0], h(), fltk3::BACKGROUND2_COLOR);
  fltk3::font(textfont(), textsize());
  fltk3::color(active_r() ? textcolor() : fltk3::inactive(textcolor()));
  char str[128]; format(str);
  fltk3::draw(str, xx[0], y(), ww[0], h(), fltk3::ALIGN_CENTER);
  if (fltk3::focus() == this) draw_focus(boxtype[0], xx[0], y(), ww[0], h());
  if (!(damage()&fltk3::DAMAGE_ALL)) return; // only need to redraw text

  if (active_r())
    selcolor = labelcolor();
  else
    selcolor = fltk3::inactive(labelcolor());

  if (type() == fltk3::NORMAL_COUNTER) {
    draw_box(boxtype[1], xx[1], y(), ww[1], h(), color());
    fltk3::draw_symbol("@-4<<", xx[1], y(), ww[1], h(), selcolor);
  }
  draw_box(boxtype[2], xx[2], y(), ww[2], h(), color());
  fltk3::draw_symbol("@-4<",  xx[2], y(), ww[2], h(), selcolor);
  draw_box(boxtype[3], xx[3], y(), ww[3], h(), color());
  fltk3::draw_symbol("@-4>",  xx[3], y(), ww[3], h(), selcolor);
  if (type() == fltk3::NORMAL_COUNTER) {
    draw_box(boxtype[4], xx[4], y(), ww[4], h(), color());
    fltk3::draw_symbol("@-4>>", xx[4], y(), ww[4], h(), selcolor);
  }
}

void fltk3::Counter::increment_cb() {
  if (!mouseobj) return;
  double v = value();
  switch (mouseobj) {
  case 1: v -= lstep_; break;
  case 2: v = increment(v, -1); break;
  case 3: v = increment(v, 1); break;
  case 4: v += lstep_; break;
  }
  handle_drag(clamp(round(v)));
}

#define INITIALREPEAT .5
#define REPEAT .1

void fltk3::Counter::repeat_callback(void* v) {
  fltk3::Counter* b = (fltk3::Counter*)v;
  if (b->mouseobj) {
    fltk3::add_timeout(REPEAT, repeat_callback, b);
    b->increment_cb();
  }
}

int fltk3::Counter::calc_mouseobj() {
  if (type() == fltk3::NORMAL_COUNTER) {
    int W = w()*15/100;
    if (fltk3::event_inside(x(), y(), W, h())) return 1;
    if (fltk3::event_inside(x()+W, y(), W, h())) return 2;
    if (fltk3::event_inside(x()+w()-2*W, y(), W, h())) return 3;
    if (fltk3::event_inside(x()+w()-W, y(), W, h())) return 4;
  } else {
    int W = w()*20/100;
    if (fltk3::event_inside(x(), y(), W, h())) return 2;
    if (fltk3::event_inside(x()+w()-W, y(), W, h())) return 3;
  }
  return -1;
}

int fltk3::Counter::handle(int event) {
  FLTK3_OBJECT_VCALLS_WRAPPER_RET(int, handle(event), Handle)
  int i;
  switch (event) {
  case fltk3::RELEASE:
    if (mouseobj) {
      fltk3::remove_timeout(repeat_callback, this);
      mouseobj = 0;
      redraw();
    }
    handle_release();
    return 1;
  case fltk3::PUSH:
    if (fltk3::visible_focus()) fltk3::focus(this);
    { fltk3::WidgetTracker wp(this);
      handle_push();
      if (wp.deleted()) return 1;
    }
  case fltk3::DRAG:
    i = calc_mouseobj();
    if (i != mouseobj) {
      fltk3::remove_timeout(repeat_callback, this);
      mouseobj = (uchar)i;
      if (i) fltk3::add_timeout(INITIALREPEAT, repeat_callback, this);
      fltk3::WidgetTracker wp(this);
      increment_cb();
      if (wp.deleted()) return 1;
      redraw();
    }
    return 1;
  case fltk3::KEYBOARD :
    switch (fltk3::event_key()) {
      case fltk3::LeftKey:
	handle_drag(clamp(increment(value(),-1)));
	return 1;
      case fltk3::RightKey:
	handle_drag(clamp(increment(value(),1)));
	return 1;
      default:
        return 0;
    }
    // break not required because of switch...
  case fltk3::FOCUS : /* FALLTHROUGH */
  case fltk3::UNFOCUS :
    if (fltk3::visible_focus()) {
      redraw();
      return 1;
    } else return 0;
  case fltk3::ENTER : /* FALLTHROUGH */
  case fltk3::LEAVE :
    return 1;
  default:
    return 0;
  }
}

/**
  Destroys the valuator.
 */
fltk3::Counter::~Counter() {
  fltk3::remove_timeout(repeat_callback, this);
}

/**
  Creates a new fltk3::Counter widget using the given position, size, and label
  string. The default type is fltk3::NORMAL_COUNTER.
  \param[in] X, Y, W, H position and size of the widget
  \param[in] L widget label, default is no label
 */
fltk3::Counter::Counter(int X, int Y, int W, int H, const char* L)
  : fltk3::Valuator(X, Y, W, H, L) {
  box(fltk3::UP_BOX);
  selection_color(fltk3::INACTIVE_COLOR); // was fltk3::BLUE
  align(fltk3::ALIGN_BOTTOM);
  bounds(-1000000.0, 1000000.0);
  Valuator::step(1, 10);
  lstep_ = 1.0;
  mouseobj = 0;
  textfont_ = fltk3::HELVETICA;
  textsize_ = fltk3::NORMAL_SIZE;
  textcolor_ = fltk3::FOREGROUND_COLOR;
}

//
// End of "$Id$".
//
