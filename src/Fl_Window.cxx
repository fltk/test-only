//
// "$Id: Fl_Window.cxx,v 1.41 2000/05/30 07:42:17 bill Exp $"
//
// Window widget class for the Fast Light Tool Kit (FLTK).
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

// The Fl_Window is a window in the fltk library.
// This is the system-independent portions.  The huge amount of 
// crap you need to do to communicate with X is in Fl_x.C, the
// equivalent (but totally different) crap for MSWindows is in Fl_win32.C

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>
#include <config.h>

Fl_Window *Fl_Widget::window() const {
  for (Fl_Widget *o = parent(); o; o = o->parent())
    if (o->is_window()) return (Fl_Window*)o;
  return 0;
}

int Fl_Window::x_root() const {
  Fl_Window *p = window();
  if (p) return p->x_root() + x();
  return x();
}

int Fl_Window::y_root() const {
  Fl_Window *p = window();
  if (p) return p->y_root() + y();
  return y();
}

void Fl_Window::draw() {
  int savex = x(); x(0);
  int savey = y(); y(0);
  Fl_Group::draw();
  y(savey);
  x(savex);
}

void Fl_Window::draw_n_clip() {} // does nothing

void Fl_Window::label(const char *name) {label(name, iconlabel());}

void Fl_Window::iconlabel(const char *iname) {label(label(), iname);}

void Fl_Window::default_callback(Fl_Window* window, void* v) {
  window->hide();
  Fl_Widget::default_callback(window, v); // put on Fl::read_queue()
}

static void revert(Fl_Style* s) {
  s->box = FL_FLAT_BOX;
}

static Fl_Named_Style* style = new Fl_Named_Style("Window", revert, &style);

void Fl_Window::_Fl_Window() {
  style(::style);
  type(FL_WINDOW);
  i = 0;
  icon_ = 0;
  iconlabel_ = 0;
  //resizable(0); // new default for group
  size_range_set = 0;
  callback((Fl_Callback*)default_callback);
}

Fl_Window::Fl_Window(int X,int Y,int W, int H, const char *l)
: Fl_Group(X, Y, W, H, l) {
  _Fl_Window();
}

Fl_Window::Fl_Window(int W, int H, const char *l)
// fix common user error of a missing end() with current(0):
  : Fl_Group((Fl_Group::current(0),FL_USEDEFAULT), FL_USEDEFAULT, W, H, l) {
  _Fl_Window();
  shortcut(FL_Escape);
  clear_visible();
}

void Fl_Window::destroy() {

  Fl_X* x = i;
  if (!x) return;

  // remove from the list of windows:
  Fl_X** pp = &Fl_X::first;
  for (; *pp != x; pp = &(*pp)->next) if (!*pp) return;
  *pp = x->next;
  i = 0;

  // recursively remove any subwindows:
  for (Fl_X *w = Fl_X::first; w;) {
    Fl_Window* W = w->w;
    if (W->window() == this) {
      W->destroy();
      w = Fl_X::first;
    } else w = w->next;
  }

  if (this == Fl::modal_) { // we are closing the modal window, find next one:
    Fl_Window* w;
    for (w = Fl::first_window(); w; w = Fl::next_window(w))
      if (w->modal()) break;
    Fl::modal_ = w;
  }

  // Make sure no events are sent to this window:
  throw_focus();
  handle(FL_HIDE);

#ifdef WIN32
  // we need to delete the pen and brush objects in the dc
  HDC dc = x->private_dc ? x->private_dc : GetDC(x->xid);
  COLORREF rgb = RGB(0, 0, 0);
  HPEN newpen = CreatePen(PS_SOLID, 1, rgb);
  HBRUSH newbrush = CreateSolidBrush(rgb);
  HPEN oldpen = (HPEN)SelectObject(dc, newpen); // this returns the old pen
  if (oldpen) DeleteObject(oldpen);
  HBRUSH oldbrush = (HBRUSH)SelectObject(dc, newbrush); // this returns the old brush
  if (oldbrush) DeleteObject(oldbrush);
  ReleaseDC(x->xid, dc); // not useful?
  if (x->xid == fl_window) {
    fl_window = 0;
    fl_gc = 0;
  }
  DestroyWindow(x->xid);
  if (x->region) XDestroyRegion(x->region);
  DeleteObject(newpen);
  DeleteObject(newbrush);
#else
  if (x->region) XDestroyRegion(x->region);
  XDestroyWindow(fl_display, x->xid);
#endif
  delete x;
}

Fl_Window::~Fl_Window() {
  destroy();
}

// Fl_Window::show()/hide() are different than Fl_Widget::show()/hide(),
// even though they are not virtual functions.  This was necessary for
// compatability with fltk 1.0.

// Fl_Window::show()/hide() actually create and destroy the X window.
// Fl_Window::show() also has the effect of raising an outer window if
// it is already shown.  The method Fl_Window::shown() is true between
// Fl_Window::show() and hide().

// Fl_Widget::show()/hide() are ignored for outer windows, it is assummed
// these are called only in response to the user iconizing/deiconizing
// the window and thus the window is already in the right state.  For
// child windows these do unmap/map the X window, this was so that
// child windows inside Fl_Tabs work.

// The purpose is to save server resources, and to avoid the need to
// have a fourth state for the window:
//
// visible:		window->shown();
// iconized:		window->shown() && !window->visible();
// not on screen:	!window->shown();
// not on screen but known by system: does not happen in fltk

// This is set by Fl::arg to argv[0], or the user can set it.
// It is used by X to look up stuff in the X resource database.
const char* Fl_Window::xclass_ = "fltk";

extern void fl_startup();
extern const Fl_Window* fl_modal_for;
extern void fl_fix_focus();

void Fl_Window::show() {
  if (parent()) {
    set_visible();
    handle(FL_SHOW);
  } else if (!i) {
    Fl_Group::current(0); // get rid of very common user bug: forgot end()

    // this is the secret place where the world is initialized:
#ifndef WIN32
    fl_open_display();
#endif
    // one-time startup stuff for schemes & config
    fl_startup();
// CET - FIXME    Fl::loadtheme();

    layout();

    // back compatability with older modal() and non_modal() flags:
    if (non_modal() && !fl_modal_for) {
      fl_modal_for = Fl::first_window();
      while (fl_modal_for && fl_modal_for->parent())
	fl_modal_for = fl_modal_for->window();
    }
    // back-compatability automatic size_range() based on resizable():
    if (!size_range_set) {
      if (resizable()) {
	Fl_Widget *o = resizable();
	int minw = o->w(); if (minw > 100) minw = 100;
	int minh = o->h(); if (minh > 100) minh = 100;
	size_range(w() - o->w() + minw, h() - o->h() + minh, 0, 0);
      } else {
	size_range(w(), h(), w(), h());
      }
    }
    create();
    fl_modal_for = 0;
    if (modal()) {Fl::modal_ = this; fl_fix_focus();}
  } else {
#ifdef WIN32
    if (IsIconic(i->xid)) OpenIcon(i->xid);
    if (!Fl::grab()) // we would lose the capture if we activated the window
      BringWindowToTop(i->xid);
#else
    XMapRaised(fl_display, i->xid);
#endif
  }
}

void Fl_Window::show(const Fl_Window* modal_for) {
  // find the outermost window and make sure it has been shown():
  while (modal_for && modal_for->parent()) modal_for = modal_for->window();
  if (modal_for && modal_for->shown()) fl_modal_for = modal_for;
  show();
  fl_modal_for = 0;
}

int Fl_Window::exec(const Fl_Window* modal_for) {
  clear(); // clear the value()
  set_modal();
  show(modal_for);
  while (shown()) Fl::wait();
  return value();
}

#ifdef WIN32
extern const Fl_Window* fl_mdi_window;
void Fl_Window::show_inside(const Fl_Window* w) {
  fl_mdi_window = w;
  show();
  fl_mdi_window = 0;
}
#else
void Fl_Window::show_inside(const Fl_Window* w) {
  show(w);
}
#endif

void Fl_Window::hide() {
  destroy();
  clear_visible();
}

// Fl_Widget::show()/hide() call this:
int Fl_Window::handle(int event) {
  if (parent()) switch (event) {
  case FL_SHOW:
    if (!i) create();
    else XMapWindow(fl_display, i->xid);
    break;
  case FL_HIDE:
    if (i && !visible()) XUnmapWindow(fl_display, i->xid);
    break;
  }
  if (Fl_Group::handle(event)) return 1;
  // Make the Escape key close windows:
  if (event == FL_SHORTCUT && !parent() && test_shortcut()) {
    do_callback();
    return 1;
  }
  // Raise windows that are clicked on, but don't raise when the
  // user hits buttons.  Unfortunately stupid ol' Win32 does this
  // all the time so there is not much I can do here...
#ifndef WIN32
  if (event == FL_PUSH && !parent()) show();
#endif
  return 0;
}

void Fl_Window::flush() {
  make_current();
//if (damage() == FL_DAMAGE_EXPOSE && can_boxcheat(box())) fl_boxcheat = this;
  fl_clip_region(i->region); i->region = 0;
  draw();
}

//
// End of "$Id: Fl_Window.cxx,v 1.41 2000/05/30 07:42:17 bill Exp $".
//
