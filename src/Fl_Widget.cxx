//
// "$Id: Fl_Widget.cxx,v 1.12 1999/05/06 05:52:22 carl Exp $"
//
// Base widget class for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tooltip.H>

#define DEFAULT_STYLE ((Style*)default_style())

////////////////////////////////////////////////////////////////
// for compatability with Forms, all widgets without callbacks are
// inserted into a "queue" when they are activated, and the forms
// compatability interaction functions (fl_do_events, etc) will
// read one widget at a time from this queue and return it:

const int QUEUE_SIZE = 20;

static Fl_Widget *obj_queue[QUEUE_SIZE];
static int obj_head, obj_tail;

void Fl_Widget::default_callback(Fl_Widget *o, void * /*v*/) {
#if 0
  // This is necessary for strict forms compatability but is confusing.
  // Use the parent's callback if this widget does not have one.
  for (Fl_Widget *p = o->parent(); p; p = p->parent())
    if (p->callback() != default_callback) {
      p->do_callback(o,v);
      return;
    }
#endif
  obj_queue[obj_head++] = o;
  if (obj_head >= QUEUE_SIZE) obj_head = 0;
  if (obj_head == obj_tail) {
    obj_tail++;
    if (obj_tail >= QUEUE_SIZE) obj_tail = 0;
  }
}

Fl_Widget *Fl::readqueue() {
  if (obj_tail==obj_head) return 0;
  Fl_Widget *o = obj_queue[obj_tail++];
  if (obj_tail >= QUEUE_SIZE) obj_tail = 0;
  return o;
}
    
////////////////////////////////////////////////////////////////

int Fl_Widget::handle(int e) { return (e == FL_ENTER) ? 1 : 0; }

Fl_Widget::Style* Fl_Widget::_default_style;

Fl_Widget::Style::Style() {
  sbf = 0;
  not_dynamic = 0;

  widget(LABELCOLOR) = FL_BLACK;
  widget(LABELSIZE) = 12;
  widget(LABELTYPE) = FL_NORMAL_LABEL;
  widget(LABELFONT) = FL_HELVETICA;
  widget(COLOR) = FL_GRAY;
  widget(COLOR2) = FL_GRAY;
  widget(COLOR3) = FL_GRAY;
  widget(BOX) = FL_NO_BOX;
}

void Fl_Widget::loadstyle() const {
  if (!Fl::s_widget) {
    Fl::s_widget = 1;

    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "color2", COLOR2 },
      { "color3", COLOR3 },
      { "box", BOX },
      { 0 }
    };
    Fl::load_attributes("widget", DEFAULT_STYLE->widget_, widget_attributes);
  }
}

Fl_Widget::Fl_Widget(int X, int Y, int W, int H, const char* L) {
  _style = 0;

  x_ = X; y_ = Y; w_ = W; h_ = H;

  label_	= L;
  callback_	= default_callback;
  user_data_ 	= 0;
  type_		= 0;
  flags_	= 0;
  damage_	= 0;
  align_	= FL_ALIGN_CENTER;
  when_		= FL_WHEN_RELEASE;

  tooltip_ = 0;

  parent_ = 0;

  if (Fl_Group::current()) Fl_Group::current()->add(this);
}

void Fl_Widget::resize(int X, int Y, int W, int H) {
  x_ = X; y_ = Y; w_ = W; h_ = H;
}

// this is useful for parent widgets to call to resize children:
int Fl_Widget::damage_resize(int X, int Y, int W, int H) {
  if (x() == X && y() == Y && w() == W && h() == H) return 0;
  resize(X, Y, W, H);
  redraw();
  return 1;
}

int Fl_Widget::take_focus() {
  if (!takesevents()) return 0;
  if (!handle(FL_FOCUS)) return 0; // see if it wants it
  if (contains(Fl::focus())) return 1; // it called Fl::focus for us
  Fl::focus(this);
  return 1;
}

extern void fl_throw_focus(Fl_Widget*); // in Fl_x.C

// Destruction does not remove from any parent group!  And groups when
// destroyed destroy all their children.  This is convienent and fast.
// However, it is only legal to destroy a "root" such as an Fl_Window,
// and automatic destructors may be called.
Fl_Widget::~Fl_Widget() {
  if (_style && !_style->not_dynamic) delete _style;
  parent_ = 0; // kludge to prevent ~Fl_Group from destroying again
  Fl_Tooltip::exit(this);
  fl_throw_focus(this);
}

// redraw this, plus redraw opaque object if there is an outside label
static void redraw_label(Fl_Widget* w) {
  w->redraw();
  if (w->label() && (w->align()&15) && !(w->align() & FL_ALIGN_INSIDE)) {
    for (Fl_Widget *p = w->parent(); p; p = p->parent())
      if (p->box() || !p->parent()) {p->redraw(); break;}
  }
}

void Fl_Widget::activate() {
  if (!active()) {
    clear_flag(INACTIVE);
    if (active_r()) {
      redraw_label(this);
      handle(FL_ACTIVATE);
      if (inside(Fl::focus())) Fl::focus()->take_focus();
    }
  }
}

void Fl_Widget::deactivate() {
  if (active_r()) {
    set_flag(INACTIVE);
    redraw_label(this);
    handle(FL_DEACTIVATE);
    fl_throw_focus(this);
  } else {
    set_flag(INACTIVE);
  }
}

int Fl_Widget::active_r() const {
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->active()) return 0;
  return 1;
}

void Fl_Widget::show() {
  if (!visible()) {
    clear_flag(INVISIBLE);
    if (visible_r()) {
      redraw_label(this);
      handle(FL_SHOW);
      if (inside(Fl::focus())) Fl::focus()->take_focus();
    }
  }
}

void Fl_Widget::hide() {
  if (visible_r()) {
    set_flag(INVISIBLE);
    for (Fl_Widget *p = parent(); p; p = p->parent())
      if (p->box() || !p->parent()) {p->redraw(); break;}
    handle(FL_HIDE);
    fl_throw_focus(this);
  } else {
    set_flag(INVISIBLE);
  }
}

int Fl_Widget::visible_r() const {
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->visible()) return 0;
  return 1;
}

// return true if widget is inside (or equal to) this:
// Returns false for null widgets.
int Fl_Widget::contains(const Fl_Widget *o) const {
  for (; o; o = o->parent_) if (o == this) return 1;
  return 0;
}

void Fl_Widget::measure_label(int& x, int& y) {
  Fl_Label l1;
  l1.value = label();
  l1.type = labeltype();
  l1.size = labelsize();
  l1.font = labelfont();
  l1.color = labelcolor();
  l1.measure(x, y);
}

// This file contains the parts of Fl_Tooltip.H that are always linked
// in.  Calling Fl_Widget::tooltip(const char*) will change the function
// pointers below and link in the tooltip popup code.

// It is also possible to change the function pointers to your own code
// if you want to display tooltips in your own way such as in a field
// in your main window.

static void nada(Fl_Widget*) {} // dummy place-holder function

void (*fl_tooltip_enter)(Fl_Widget *) = nada;
void (*fl_tooltip_exit)(Fl_Widget *) = nada;

uchar Fl_Widget::attr(Attribute a) const {
  loadstyle();
  if (!_style || !(WIDGET_STYLE->sbf & bf(a)))
    return DEFAULT_STYLE->widget(a);
  return WIDGET_STYLE->widget(a);
}

Fl_Boxtype Fl_Widget::box() const { return (Fl_Boxtype)attr(BOX); }
Fl_Color Fl_Widget::color() const { return (Fl_Color)attr(COLOR); }
Fl_Color Fl_Widget::selection_color() const { return (Fl_Color)attr(COLOR2); }
Fl_Color Fl_Widget::color3() const { return (Fl_Color)attr(COLOR3); }
Fl_Labeltype Fl_Widget::labeltype() const { return (Fl_Labeltype)attr(LABELTYPE); }
Fl_Color Fl_Widget::labelcolor() const { return (Fl_Color)attr(LABELCOLOR); }
Fl_Font Fl_Widget::labelfont() const { return (Fl_Font)attr(LABELFONT); }
uchar Fl_Widget::labelsize() const { return attr(LABELSIZE); }
Fl_Color Fl_Widget::color2() const {return selection_color();}

//
// End of "$Id: Fl_Widget.cxx,v 1.12 1999/05/06 05:52:22 carl Exp $".
//
