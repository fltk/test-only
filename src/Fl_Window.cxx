//
// "$Id: Fl_Window.cxx,v 1.53 2000/08/11 00:53:47 clip Exp $"
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
  modal_for_ = 0;
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

extern void fl_startup();
extern void fl_fix_focus();

char fl_show_iconic; // set by iconize() or by -i Fl::arg switch

int Fl_Window::handle(int event) {
  switch (event) {
    case FL_SHOW: {
      if (!i) create();
      Fl_Group::handle(event); // make the child windows map first
#ifdef WIN32
      int showtype;
      if (parent())
        showtype = SW_RESTORE;
      // If we've captured the mouse, we dont want do activate any
      // other windows from the code, or we loose the capture.
      // Also, we don't want to activate the window for tooltips.
      else if (Fl::grab() || !border())
        showtype = SW_SHOWNOACTIVATE;
      else
        showtype = SW_SHOWNORMAL;
      ShowWindow(i->xid, showtype);
#else
      if (parent())
        XMapWindow(fl_display, i->xid);
      else
        XMapRaised(fl_display, i->xid);
#endif
      return 0; // why is this returning 0?
    }

    case FL_HIDE: {
      if (i) {
        // We have to destroy the window because at least on my X system
        // just unmapping it causes undesired events to be generated. For
        // example, moving the cursor over the area of an unmapped window
        // causes the window behind it to get a LeaveNotify event!?  Not
        // sure whether this is a bug in XFree86 4 or not...
        destroy();

        //if (modal()) destroy(); // needed so modal can change next time
        //else XUnmapWindow(fl_display, i->xid);
      }
      break;
    }
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
  if (event == FL_PUSH && !parent()) XMapRaised(fl_display, i->xid);
#endif
  return 0;
}

const void* Fl_Window::sys_modal_for() const {
  const Fl_Window* w = modal_for_;
  while (w && w->parent()) w = w->window();
  if (w && w->shown()) return (void*)w->i->xid;
  return 0;
}

void Fl_Window::show() {
  if (!parent()) {
    if (modal()) {Fl::modal_ = this; fl_fix_focus();}
    // if this is a modal or non modal window, recreate each time to make
    // sure that modal_for is handled properly.
    if (modal() || non_modal()) destroy();

    if (!shown()) {
      // Create a new top-level window

      // This should not be needed but it is because sometimes outer windows
      // are created with the visible flag on, which prevents the FL_SHOW
      // event from going through:
      clear_visible();

      // this is the secret place where the world is initialized:
      Fl_Group::current(0); // get rid of very common user bug: forgot end()
      static char started = 0;
      if (!started) {
	started = 1;
#ifndef WIN32
	fl_open_display();
#endif
	fl_startup(); // checks flconfig options and loads themes / schemes
      }

      layout();
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
    } else if (visible()) {
      // raise/deiconize windows
#ifdef WIN32
      if (IsIconic(i->xid)) OpenIcon(i->xid);
      if (!Fl::grab() && border()) BringWindowToTop(i->xid);
#else
      XMapRaised(fl_display, i->xid);
#endif
    }
  }
  Fl_Widget::show();
}

void Fl_Window::show(const Fl_Window* w) {
  modal_for(w);
  show();
}

int Fl_Window::exec(const Fl_Window* w) {
  clear_value();
  set_modal();
  show(w);
  while (visible()) Fl::wait();
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

void Fl_Window::flush() {
  make_current();
//if (damage() == FL_DAMAGE_EXPOSE && can_boxcheat(box())) fl_boxcheat = this;
  fl_clip_region(i->region); i->region = 0;
  if (damage()&FL_DAMAGE_LAYOUT) layout();
  draw();
}

void Fl_Window::destroy() {
  Fl_X* x = i;
  if (!x) return;
  i = 0;

  // remove from the list of windows:
  Fl_X** pp = &Fl_X::first;
  for (; *pp != x; pp = &(*pp)->next) if (!*pp) return;
  *pp = x->next;

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
  clear_visible();

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

//
// End of "$Id: Fl_Window.cxx,v 1.53 2000/08/11 00:53:47 clip Exp $".
//
