//
// "$Id: Fl_Counter.cxx,v 1.37 2000/08/10 09:24:31 spitzak Exp $"
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
#include <FL/Fl_Output.H>
#include <FL/fl_draw.H>

enum {
  FL_GLYPH_LEFTARROW,
  FL_GLYPH_RIGHTARROW,
  FL_GLYPH_2LEFTARROW,
  FL_GLYPH_2RIGHTARROW
};

static void glyph(const Fl_Widget* widget, int t,
		  int x,int y,int w,int h, Fl_Flags f)
{
  widget->draw_box(x,y,w,h,f);
  Fl_Color fc = widget->glyph_color(f);
  switch (t) {
    case FL_GLYPH_LEFTARROW: fl_draw_symbol("@-4<",  x, y, w, h, fc); break;
    case FL_GLYPH_2LEFTARROW: fl_draw_symbol("@-4<<", x, y, w, h, fc); break;
    case FL_GLYPH_RIGHTARROW: fl_draw_symbol("@-4>",  x, y, w, h, fc); break;
    case FL_GLYPH_2RIGHTARROW: fl_draw_symbol("@-4>>", x, y, w, h, fc); break;
  }
}

void Fl_Counter::draw() {
  Fl_Flags fl[5];
  for (int i = 1; i < 5; i++) {
    fl[i] = flags();
    if (!active_r()) fl[i] |= FL_INACTIVE;
    else if (mouseobj == i) fl[i] |= FL_VALUE;
    else if (highlight == i) fl[i] |= FL_HIGHLIGHT;
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

  draw_text_box(xx[0], y(), ww[0], h());

  char str[128]; format(str);
  fl_font(text_font(), text_size());
  if (focused()) {
    fl_color(selection_color());
    int h = text_size()+leading();
    int w = fl_width(str);
    fl_rectf(xx[0]+(ww[0]-w)/2,y()+(this->h()-h)/2,w,h);
    fl_color(selection_text_color());
  } else {
    fl_color(glyph_color(flags()));
  }
  fl_draw(str, xx[0], y(), ww[0], h(), FL_ALIGN_CENTER);

  if (type() == FL_NORMAL_COUNTER &&
      (damage()&FL_DAMAGE_ALL || last == 1 || highlight == 1))
    draw_glyph(FL_GLYPH_2LEFTARROW, xx[1], y(), ww[1], h(), fl[1]);
  if (damage()&FL_DAMAGE_ALL || last == 2 || highlight == 2)
    draw_glyph(FL_GLYPH_LEFTARROW, xx[2], y(), ww[2], h(), fl[2]);
  if (damage()&FL_DAMAGE_ALL || last == 3 || highlight == 3)
    draw_glyph(FL_GLYPH_RIGHTARROW, xx[3], y(), ww[3], h(), fl[3]);
  if (type() == FL_NORMAL_COUNTER &&
      (damage()&FL_DAMAGE_ALL || last == 4 || highlight == 4))
    draw_glyph(FL_GLYPH_2RIGHTARROW, xx[4], y(), ww[4], h(), fl[4]);
  last = highlight;
}

void Fl_Counter::increment_cb() {
  if (!mouseobj) return;
  int delta = 0;
  switch (mouseobj) {
  case 1: delta = -linesize(); break;
  case 2: delta = -1; break;
  case 3: delta =  1; break;
  case 4: delta = linesize(); break;
  }
  handle_drag(clamp(increment(value(), delta)));
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
  int mx = Fl::event_x();
  if (type() == FL_NORMAL_COUNTER) {
    int W = w()*15/100;
    if (mx < x()+W) return 1;
    if (mx < x()+2*W) return 2;
    if (mx >= x()+w()-W) return 4;
    if (mx >= x()+w()-2*W) return 3;
  } else {
    int W = w()*20/100;
    if (mx < x()+W) return 2;
    if (mx >= x()+w()-W) return 3;
  }
  return 0;
}

int Fl_Counter::handle(int event) {
  switch (event) {

  case FL_RELEASE:
    if (Fl::pushed()) return 1;
    if (mouseobj) {
      Fl::remove_timeout(repeat_callback, this);
      mouseobj = 0;
      damage(FL_DAMAGE_EXPOSE);
    }
    handle_release();
    return 1;

  case FL_PUSH:
    take_focus();
    handle_push();
  case FL_DRAG:
    highlight = calc_mouseobj();
    if (highlight != mouseobj) {
      Fl::remove_timeout(repeat_callback, this);
      mouseobj = highlight;
      if (highlight) Fl::add_timeout(INITIALREPEAT, repeat_callback, this);
      increment_cb();
    }
    return 1;

  case FL_LEAVE:
    highlight = 0; goto J1;
  case FL_ENTER:
  case FL_MOVE:
    highlight = calc_mouseobj();
  J1:
    if (last != highlight) redraw();
    return 1;

  default:
    return Fl_Valuator::handle(event);
  }
}

Fl_Counter::~Fl_Counter() {
  Fl::remove_timeout(repeat_callback, this);
}

static void revert(Fl_Style* s) {
  s->selection_color = FL_GRAY;
  s->selection_text_color = FL_BLACK;
  s->glyph = glyph;
}

static Fl_Named_Style* style = new Fl_Named_Style("counter", revert, &style);

Fl_Counter::Fl_Counter(int x, int y, int w, int h, const char *l) : Fl_Valuator(x, y, w, h, l) {
  style(::style);
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_BOTTOM);
  range(-1000000.0, 1000000.0);
  Fl_Valuator::step(.1);
  linesize(10);
  mouseobj = highlight = last = 0;
}

//
// End of "$Id: Fl_Counter.cxx,v 1.37 2000/08/10 09:24:31 spitzak Exp $".
//
