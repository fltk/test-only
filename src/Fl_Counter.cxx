//
// "$Id: Fl_Counter.cxx,v 1.13 1999/05/04 22:54:21 carl Exp $"
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

#define DEFAULT_STYLE ((Style*)default_style())

void Fl_Counter::draw() {
  loadstyle();
  int i; Fl_Boxtype boxtype[5];
  Fl_Color selcolor;

  Fl_Color col;
  Fl_Boxtype bt;
  if (fly_box() && Fl::belowmouse() == this)
    { bt = fly_box(); col = fly_color(); }
  else
    { bt = box(); col = color(); }

  boxtype[0] = down_box();

  for (i=1; i<5; i++)
    if (mouseobj == i)
      boxtype[i] = down(box());
    else
      boxtype[i] = bt;

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

  draw_box(boxtype[0], xx[0], y(), ww[0], h(), color3());
  fl_font(textfont(), textsize());
  fl_color(active_r() ? textcolor() : inactive(textcolor()));
  char str[128]; format(str);
  fl_draw(str, xx[0], y(), ww[0], h(), FL_ALIGN_CENTER);
  if (!(damage()&FL_DAMAGE_ALL)) return; // only need to redraw text

  if (active_r())
    selcolor = selection_color();
  else
    selcolor = inactive(selection_color());
  if (type() == FL_NORMAL_COUNTER) {
    draw_box(boxtype[1], xx[1], y(), ww[1], h(), col);
    fl_draw_symbol("@-4<<", xx[1], y(), ww[1], h(), selcolor);
  }
  draw_box(boxtype[2], xx[2], y(), ww[2], h(), col);
  fl_draw_symbol("@-4<",  xx[2], y(), ww[2], h(), selcolor);
  draw_box(boxtype[3], xx[3], y(), ww[3], h(), col);
  fl_draw_symbol("@-4>",  xx[3], y(), ww[3], h(), selcolor);
  if (type() == FL_NORMAL_COUNTER) {
    draw_box(boxtype[4], xx[4], y(), ww[4], h(), col);
    fl_draw_symbol("@-4>>", xx[4], y(), ww[4], h(), selcolor);
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
    if (takesevents() && fly_box()) redraw();
    return 1;
  default:
    return 0;
  }
}

Fl_Counter::~Fl_Counter() {
  Fl::remove_timeout(repeat_callback, this);
}

Fl_Widget::Style* Fl_Counter::_default_style = 0;

Fl_Counter::Style::Style() : Fl_Widget::Style() {
  sbf = 0;

  widget(COLOR2) = FL_BLACK;
  widget(COLOR3) = FL_GRAY;
  widget(BOX) = FL_MEDIUM_UP_BOX;

  counter(DOWN_BOX) = FL_ENGRAVED_BOX;
  counter(TEXTFONT) = FL_HELVETICA;
  counter(TEXTSIZE) = 10;
  counter(TEXTCOLOR) = FL_BLACK;
  counter(FLY_COLOR) = 51;
  counter(FLY_BOX) = FL_MEDIUM_UP_BOX;
}

void Fl_Counter::loadstyle() const {
  if (!Fl::s_counter) {
    Fl::s_counter = 1;
    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "arrow color", COLOR2 },
      { "text area color", COLOR3 },
      { "box", BOX },
      { 0 }
    };
    Fl::load_attributes("counter", DEFAULT_STYLE->widget_, widget_attributes);
    static Fl::Attribute counter_attributes[] = {
      { "highlight color", FLY_COLOR },
      { "highlight box", FLY_BOX },
      { "box2", DOWN_BOX },
      { "text font", TEXTFONT },
      { "text size", TEXTSIZE },
      { "text color", TEXTCOLOR },
      { 0 }
    };
    Fl::load_attributes("counter", DEFAULT_STYLE->counter_, counter_attributes);
  }
}

Fl_Counter::Fl_Counter(int x, int y, int w, int h, const char *l) : Fl_Valuator(x, y, w, h, l) {
  align(FL_ALIGN_BOTTOM);
  bounds(-1000000.0, 1000000.0);
  Fl_Valuator::step(1, 10);
  lstep_ = 1.0;
  mouseobj = 0;
}

Fl_Font Fl_Counter::textfont() const {
  if (!_style || !(COUNTER_STYLE->sbf & bf(TEXTFONT)))
    return (Fl_Font)DEFAULT_STYLE->counter(TEXTFONT);
  return (Fl_Font)COUNTER_STYLE->counter(TEXTFONT);
}

uchar Fl_Counter::textsize() const {
  if (!_style || !(COUNTER_STYLE->sbf & bf(TEXTSIZE)))
    return DEFAULT_STYLE->counter(TEXTSIZE);
  return COUNTER_STYLE->counter(TEXTSIZE);
}

Fl_Color Fl_Counter::textcolor() const {
  if (!_style || !(COUNTER_STYLE->sbf & bf(TEXTCOLOR)))
    return (Fl_Color)DEFAULT_STYLE->counter(TEXTCOLOR);
  return (Fl_Color)COUNTER_STYLE->counter(TEXTCOLOR);
}

Fl_Boxtype Fl_Counter::down_box() const {
  if (!_style || !(COUNTER_STYLE->sbf & bf(DOWN_BOX)))
    return (Fl_Boxtype)DEFAULT_STYLE->counter(DOWN_BOX);
  return (Fl_Boxtype)COUNTER_STYLE->counter(DOWN_BOX);
}

Fl_Boxtype Fl_Counter::fly_box() const {
  if (_style && (WIDGET_STYLE->sbf & bf(BOX)) && !(COUNTER_STYLE->sbf & bf(FLY_BOX)))
    return (Fl_Boxtype)WIDGET_STYLE->widget(BOX);
  if (!_style || !(COUNTER_STYLE->sbf & bf(FLY_BOX)))
    return (Fl_Boxtype)DEFAULT_STYLE->counter(FLY_BOX);
  return (Fl_Boxtype)COUNTER_STYLE->counter(FLY_BOX);
}

Fl_Color Fl_Counter::fly_color() const {
  if (!_style || !(COUNTER_STYLE->sbf & bf(FLY_COLOR)))
    return (Fl_Color)DEFAULT_STYLE->counter(FLY_COLOR);
  return (Fl_Color)COUNTER_STYLE->counter(FLY_COLOR);
}

//
// End of "$Id: Fl_Counter.cxx,v 1.13 1999/05/04 22:54:21 carl Exp $".
//
