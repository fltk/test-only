//
// "$Id: Fl_Counter.cxx,v 1.15 1999/08/16 07:31:15 bill Exp $"
//
// Counter widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Counter.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>

void Fl_Counter::draw() {

  Fl_Flags fl[5];
  for (int i = 1; i < 5; i++) {
    unsigned f = flags();	
    if (Fl::belowmouse() == this) f |= FL_HIGHLIGHT;
    if (mouseobj == i) f |= FL_VALUE;
    fl[i] = (Fl_Flags)f;
  }

  int xx[5], ww[5];
  if (type() == FL_NORMAL_COUNTER) {
    int W = w()*15/100;
    xx[1] = x();	 ww[1] = W;
    xx[2] = x()+1*W;     ww[2] = W;
    xx[0] = x()+2*W;     ww[0] = w()-4*W;
    xx[3] = x()+w()-2*W; ww[3] = W;
    xx[4] = x()+w()-1*W; ww[4] = W;
  } else {
    int W = w()*20/100;
    xx[2] = x();	 ww[2] = W;
    xx[0] = x()+W;	 ww[0] = w()-2*W;
    xx[3] = x()+w()-1*W; ww[3] = W;
  }

  Fl_Output::default_style.box->draw(xx[0], y(), ww[0], h(),
				     Fl_Output::default_style.color, 0);
  fl_font(textfont(), textsize());
  fl_color(active_r() ? textcolor() : fl_inactive(textcolor()));
  char str[128]; format(str);
  fl_draw(str, xx[0], y(), ww[0], h(), FL_ALIGN_CENTER);
  if (!(damage()&FL_DAMAGE_ALL)) return; // only need to redraw text

  Fl_Color c = active_r() ? off_color() : Fl_Color(FL_INACTIVE_COLOR);
  if (type() == FL_NORMAL_COUNTER) {
    draw_glyph(0, xx[1], y(), ww[1], h(), fl[1]);
    fl_draw_symbol("@-4<<", xx[1], y(), ww[1], h(), c);
  }
  draw_glyph(0, xx[2], y(), ww[2], h(), fl[2]);
  fl_draw_symbol("@-4<",  xx[2], y(), ww[2], h(), c);
  draw_glyph(0, xx[3], y(), ww[3], h(), fl[3]);
  fl_draw_symbol("@-4>",  xx[3], y(), ww[3], h(), c);
  if (type() == FL_NORMAL_COUNTER) {
    draw_glyph(0, xx[4], y(), ww[4], h(), fl[4]);
    fl_draw_symbol("@-4>>", xx[4], y(), ww[4], h(), c);
  }
}

void Fl_Counter::increment_cb() {
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

void Fl_Counter::repeat_callback(void* v) {
  Fl_Counter* b = (Fl_Counter*)v;
  if (b->mouseobj) {
    Fl::add_timeout(REPEAT, repeat_callback, b);
    b->increment_cb();
  }
}

int Fl_Counter::calc_mouseobj() {
  if (type() == FL_NORMAL_COUNTER) {
    int W = w()*15/100;
    if (Fl::event_inside(x(), y(), W, h())) return 1;
    if (Fl::event_inside(x()+W, y(), W, h())) return 2;
    if (Fl::event_inside(x()+w()-2*W, y(), W, h())) return 3;
    if (Fl::event_inside(x()+w()-W, y(), W, h())) return 4;
  } else {
    int W = w()*20/100;
    if (Fl::event_inside(x(), y(), W, h())) return 2;
    if (Fl::event_inside(x()+w()-W, y(), W, h())) return 3;
  }
  return -1;
}

int Fl_Counter::handle(int event) {
  int i;
  switch (event) {
  case FL_RELEASE:
    if (!Fl::pushed()) {
      if (mouseobj) {
        Fl::remove_timeout(repeat_callback, this);
        mouseobj = 0;
        redraw();
      }
    }
    handle_release();
    return 1;
  case FL_PUSH:
    handle_push();
  case FL_DRAG:
    i = calc_mouseobj();
    if (i != mouseobj) {
      Fl::remove_timeout(repeat_callback, this);
      mouseobj = i;
      if (i) Fl::add_timeout(INITIALREPEAT, repeat_callback, this);
      increment_cb();
      redraw();
    }
    return 1;
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && active_r()) redraw();
    return 1;
  default:
    return 0;
  }
}

Fl_Counter::~Fl_Counter() {
  Fl::remove_timeout(repeat_callback, this);
}

Fl_Counter::Fl_Counter(int x, int y, int w, int h, const char *l) : Fl_Valuator(x, y, w, h, l) {
  align(FL_ALIGN_BOTTOM);
  bounds(-1000000.0, 1000000.0);
  Fl_Valuator::step(1, 10);
  lstep_ = 1.0;
  mouseobj = 0;
}

//
// End of "$Id: Fl_Counter.cxx,v 1.15 1999/08/16 07:31:15 bill Exp $".
//
