//
// "$Id: Fl_Window.cxx,v 1.77 2001/12/16 22:32:03 spitzak Exp $"
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

#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include <fltk/x.h>
//#include <stdlib.h>
#include <config.h>

Fl_Window *Fl_Widget::window() const {
  for (Fl_Widget *o = parent(); o; o = o->parent())
    if (o->is_window()) return (Fl_Window*)o;
  return 0;
}

void Fl_Window::draw() {Fl_Group::draw();}

void Fl_Window::label(const char *name) {label(name, iconlabel());}

void Fl_Window::iconlabel(const char *iname) {label(label(), iname);}

void Fl_Window::default_callback(Fl_Window* window, void*) {
  window->hide();
  // if there are no visible windows we exit:
  // Not anymore, it has been restored to fltk 1.0 behavior. Instead
  // Fl::run() checks after each loop to see if all windows are closed.
  // if (!Fl::first_window()) exit(0);
}

static void revert(Fl_Style* s) {
  s->box = FL_FLAT_BOX;
}
static Fl_Named_Style style("Window", revert, &Fl_Window::default_style);
Fl_Named_Style* Fl_Window::default_style = &::style;

void Fl_Window::_Fl_Window() {
  style(default_style);
  type(FL_WINDOW_TYPE);
  i = 0;
  icon_ = 0;
  iconlabel_ = 0;
  //resizable(0); // new default for group
  size_range_set = 0;
  child_of_ = 0;
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

// FL_SHOW events will normally create and map the window, FL_HIDE will
// unmap.  On both X and Win32 creating a window requires a lot of ugly
// cruft, some of it is here and much of it is in the machine-specific
// code like Fl_x.cxx.  There are also static variables (!) used to
// modify how the window is created, such as to create it iconized or
// to create it with a parent.

// This is set by Fl::arg to argv[0], or the user can set it.
// It is used by X to look up stuff in the X resource database:
const char* Fl_Window::xclass_ = "fltk";

extern void fl_fix_focus();

bool fl_show_iconic; // set by iconize() or by -i Fl::arg switch

int Fl_Window::handle(int event) {
  switch (event) {
    case FL_SHOW: {
      // Emulate the undocumented back-compatability modal() stuff:
      if (flags()&(FL_MODAL|FL_NON_MODAL)) {
	child_of(Fl::first_window()); // this may unmap window if it changes
	if (flags()&FL_MODAL) Fl::modal(this, false);
      }
      if (!shown()) {
	Fl::startup();
	layout();
	// back-compatability automatic size_range() based on resizable():
	if (!parent() && !size_range_set) {
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
      }

      Fl_Group::handle(event); // make the child windows map first

#ifdef _WIN32
      int showtype;
      if (parent())
        showtype = SW_RESTORE;
      // If we've captured the mouse, we don't want do activate any
      // other windows from the code, or we lose the capture.
      // Also, we don't want to activate the window for tooltips.
      else if (Fl::grab() || override())
        showtype = SW_SHOWNOACTIVATE;
      else if (fl_show_iconic)
	showtype = SW_SHOWMINNOACTIVE,fl_show_iconic = false;
      else
        showtype = SW_SHOWNORMAL;

      ShowWindow(i->xid, showtype);
#else
      XMapWindow(fl_display, i->xid);
#endif
      break;
    }

    case FL_HIDE:
#ifdef _WIN32
      // Try to stop the annoying "raise another program" behavior
      // This does not appear to work unfortunately...
      //if (child_of_) ((Fl_Window*)child_of_)->show();
#endif
      if (i) {
	XUnmapWindow(fl_display, i->xid);
	// Emulation of undocumented modal() from fltk1:
	if (flags()&FL_MODAL) Fl::modal(0,false);
      }
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
#ifndef _WIN32
  if (event == FL_PUSH && !parent()) XMapRaised(fl_display, i->xid);
#endif
  return 0;
}

// Fl_Window::show() should not actually be different than Fl_Widget::show,
// as most of the work is done by Fl_Window::handle(FL_SHOW). However for
// back compatability some stuff is done here, primarily it allows show()
// to raise existing windows.
void Fl_Window::show() {
  // get rid of very common user bug: forgot end():
  Fl_Group::current(0);
  if (!shown()) {
    // If the window was created with the xywh constructor, the visible()
    // flag was left on, turn it off:
    clear_visible();
  } else if (!parent()) {
    // raise/deiconize windows already-visible windows
#ifdef _WIN32
    if (IsIconic(i->xid)) OpenIcon(i->xid);
    if (!Fl::grab() && !override()) BringWindowToTop(i->xid);
#else
    XMapRaised(fl_display, i->xid);
#endif
  }
  // Otherwise all the work is done by handle(FL_SHOW):
  Fl_Widget::show();
}

void Fl_Window::show(const Fl_Window* w) {
  child_of(w);
  show();
}

void Fl_Window::child_of(const Fl_Window* w) {
  // if (contains(w)) return;
  while (w && w->parent()) w = w->window();
  if (child_of_ != w) destroy();
  child_of_ = w;
}

bool Fl_Window::exec(const Fl_Window* w, bool grab) {
  clear_value();
  child_of(w ? w : Fl::first_window());
  Fl_Widget* saved_modal = Fl::modal(); bool saved_grab = Fl::grab();
  Fl::modal(this, grab);
  show();
  while (!Fl::exit_modal_flag()) Fl::wait();
  hide();
  Fl::modal(saved_modal, saved_grab);
  return value();
}

#ifdef _WIN32
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

////////////////////////////////////////////////////////////////

void Fl_Widget::redraw(int X, int Y, int W, int H) {
  // go up to the window, clipping to each widget's area, quit if empty:
  Fl_Widget* window = this;
  for (;;) {
    if (X < 0) {W += X; X = 0;}
    if (Y < 0) {H += Y; Y = 0;}
    if (W > window->w()-X) W = window->w()-X;
    if (H > window->h()-Y) H = window->h()-Y;
    if (W <= 0 || H <= 0) return;
    if (window->is_window()) break;
    X += window->x();
    Y += window->y();
    window = window->parent();
    if (!window) return;
  }
  Fl_X* i = Fl_X::i((Fl_Window*)window);
  if (!i) return; // window not mapped, so ignore it
  window->damage_ |= FL_DAMAGE_EXPOSE;
  i->expose(X, Y, W, H);
}

// Merge a rectangle into a window's expose region. If the entire
// window is damaged we switch to a FL_DAMAGE_ALL mode which will
// avoid drawing it twice:
void Fl_X::expose(int X, int Y, int W, int H) {
  // Ignore if window already marked as completely damaged:
  if (window->damage() & FL_DAMAGE_ALL) ;
  // Detect expose events that cover the entire window:
  else if (X<=0 && Y<=0 && W>=window->w() && H>=window->h()) {
    window->set_damage(FL_DAMAGE_ALL);
  } else if (!region) {
    // create a new region:
    region = XRectangleRegion(X,Y,W,H);
  } else {
    // merge with the region:
#ifndef _WIN32
    XRectangle R;
    R.x = X; R.y = Y; R.width = W; R.height = H;
    XUnionRectWithRegion(&R, region, region);
#else
    Region R = XRectangleRegion(X, Y, W, H);
    CombineRgn(region, region, R, RGN_OR);
    XDestroyRegion(R);
#endif
  }
  // make Fl::flush() search for this window:
  Fl::damage(FL_DAMAGE_EXPOSE);
}

void Fl_Window::flush() {
  make_current();
  if (damage() & ~FL_DAMAGE_EXPOSE) {
    set_damage(damage() & ~FL_DAMAGE_EXPOSE);
    draw();
  }
  if (i->region && !(damage() & FL_DAMAGE_ALL)) {
    fl_clip_region(i->region); i->region = 0;
    set_damage(FL_DAMAGE_EXPOSE); draw();
    fl_clip_region(0);
  }
}

////////////////////////////////////////////////////////////////

void Fl_Window::destroy() {
  Fl_X* x = i;
  if (!x) return;
  i = 0;

  // remove from the list of windows:
  Fl_X** pp = &Fl_X::first;
  for (; *pp != x; pp = &(*pp)->next) if (!*pp) return;
  *pp = x->next;

  // recursively remove any subwindows:
  for (Fl_X *x1 = Fl_X::first; x1;) {
    Fl_Window* subwindow = x1->window;
    if (subwindow->window() == this) {
      subwindow->destroy();
      x1 = Fl_X::first;
    } else x1 = x1->next;
  }

  // Make sure no events are sent to this window:
  throw_focus();
  clear_visible();

#ifdef _WIN32
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
  DeleteObject(newpen);
  DeleteObject(newbrush);
  if (x->xid == fl_window) {fl_window = 0; fl_gc = 0;} // not necessary?
  if (x->region) XDestroyRegion(x->region);
  DestroyWindow(x->xid);
#else
  if (x->region) XDestroyRegion(x->region);
  XDestroyWindow(fl_display, x->xid);
#endif
  delete x;
}

Fl_Window::~Fl_Window() {
  destroy();
}

//
// End of "$Id: Fl_Window.cxx,v 1.77 2001/12/16 22:32:03 spitzak Exp $".
//
