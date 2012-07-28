//
// "$Id$"
//
// Menu window code for the Fast Light Tool Kit (FLTK).
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

// This is the window type used by Fl_Menu to make the pop-ups.
// It draws in the overlay planes if possible.

// Also here is the implementation of the mouse & keyboard grab,
// which are used so that clicks outside the program's windows
// can be used to dismiss the menus.

#include <config.h>
#include <fltk3/run.h>
#include <fltk3/x.h>
#include <fltk3/draw.h>
#include <fltk3/MenuWindow.h>

// WIN32 note: HAVE_OVERLAY is false
#if HAVE_OVERLAY
extern XVisualInfo *fl_find_overlay_visual();
extern XVisualInfo *fl_overlay_visual;
extern Colormap fl_overlay_colormap;
extern unsigned long fl_transparent_pixel;
static GC gc;	// the GC used by all X windows
extern uchar fl_overlay; // changes how fltk3::color(x) works
#endif

#include <stdio.h>

void fltk3::MenuWindow::show() {
#if HAVE_OVERLAY
  if (!shown() && overlay() && fl_find_overlay_visual()) {
    XInstallColormap(fl_display, fl_overlay_colormap);
    fl_background_pixel = int(fl_transparent_pixel);
    Fl_X::make_xid(this, fl_overlay_visual, fl_overlay_colormap);
    fl_background_pixel = -1;
  } else
#endif
    SingleWindow::show();
}

void fltk3::MenuWindow::flush() {
#if HAVE_OVERLAY
  if (!fl_overlay_visual || !overlay()) {fltk3::SingleWindow::flush(); return;}
  Fl_X *myi = Fl_X::i(this);
  fl_window = myi->xid;
  if (!gc) {
	  gc = XCreateGC(fl_display, myi->xid, 0, 0);
# if defined(FLTK_USE_CAIRO)
	  if(fltk3::autolink_context()) fltk3::cairo_make_current(gc); // capture gc changes automatically to update the cairo context adequately
# endif
  }
  fl_gc = gc;
  fl_overlay = 1;
  fltk3::clip_region(myi->region); myi->region = 0; current_ = this;
  draw();
  fl_overlay = 0;
#else
  SingleWindow::flush();
#endif
}

/** Erases the window, does nothing if HAVE_OVERLAY is not defined config.h */
void fltk3::MenuWindow::erase() {
#if HAVE_OVERLAY
  if (!gc || !shown()) return;
//XSetForeground(fl_display, gc, 0);
//XFillRectangle(fl_display, fl_xid(this), gc, 0, 0, w(), h());
  XClearWindow(fl_display, fl_xid(this));
#endif
}

// Fix the colormap flashing on Maximum Impact Graphics by erasing the
// menu before unmapping it:
void fltk3::MenuWindow::hide() {
  erase();
  SingleWindow::hide();
}

/**  Destroys the window and all of its children.*/
fltk3::MenuWindow::~MenuWindow() {
  hide();
}

fltk3::MenuWindow::MenuWindow(int W, int H, const char *l)
: fltk3::SingleWindow(W,H,l) 
{ 
  image(0); 
}


fltk3::MenuWindow::MenuWindow(int X, int Y, int W, int H, const char *l)
: fltk3::SingleWindow(X,Y,W,H,l) 
{ 
  image(0); 
}


//------------------------------------------------------------------------------


fltk3::PopupWindow::PopupWindow(int x, int y, int w, int h, const char *title)
: fltk3::MenuWindow(x, y, w, h, title),
  pUserPosition(true)
{
  set_modal();
  clear_border();
  set_menu_window();
}


fltk3::PopupWindow::PopupWindow(int w, int h, const char *title)
: fltk3::MenuWindow(w, h, title),
  pUserPosition(false)
{
  set_modal();
  clear_border();
  set_menu_window();
}


fltk3::PopupWindow::~PopupWindow()
{
  hide();
}


void fltk3::PopupWindow::hide_i(fltk3::Widget* w, void *d)
{
  fltk3::PopupWindow* p = ((fltk3::PopupWindow*)d);
  p->pTrigger = w;
  p->hide();
}


int fltk3::PopupWindow::handle(int e) {
  fltk3::Widget* wi;
  switch (e) {
    case fltk3::KEYBOARD:
      switch (fltk3::event_key()) {
        case fltk3::EscapeKey:
          pTrigger = 0L;
          hide();
          return 1;
      }
      for (wi = fltk3::focus(); wi; wi = wi->parent()) {
        if (wi==this) break;
        if (wi->send(fltk3::KEYBOARD)) return 1;
      }
      return 0;
    case fltk3::SHORTCUT: 
      switch (fltk3::event_key()) {
        case fltk3::EscapeKey:
          pTrigger = 0L;
          hide();
          return 1;
      }
      break;
    case fltk3::PUSH: {
      int mx = fltk3::event_x_root();
      int my = fltk3::event_y_root();
      if (!Rectangle::contains(mx, my)) {
        pTrigger = 0L;
        hide();
        return 1;
      }
      break;
    }
  }
  return MenuWindow::handle(e);
}


fltk3::Widget* fltk3::PopupWindow::popup()
{
  pTrigger = 0L;
  if (!pUserPosition)
    position(fltk3::event_x_root(), fltk3::event_y_root());
  show();
  fltk3::grab(this);
  for (;;) {
    fltk3::wait();
    if (!visible_r()) 
      break;
  }
  hide();
  fltk3::grab(0);
  return pTrigger;
}


//
// End of "$Id$".
//
