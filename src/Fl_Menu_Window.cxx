//
// "$Id: Fl_Menu_Window.cxx,v 1.22 2004/07/04 17:18:22 laza2000 Exp $"
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <config.h>
#include <fltk/damage.h>
#include <fltk/MenuWindow.h>
#include <fltk/x.h>
#include <fltk/draw.h>

using namespace fltk;

/*! \class fltk::MenuWindow

  This is the window type used by Menu to make the pop-ups, and for
  tooltip popups. It will send special information to the window server
  to indicate that the windows are temporary, won't move, and should
  not have any decorations.

  On X this turns on override_redirect and save-under.

  On older X servers this actually tried to draw into overlay hardware.
  This behavior only remains when FLTK is compiled on Irix (where
  overlays are still faster and save-under does not work):
*/

/*! \fn void MenuWindow::clear_overlay()
  Tells FLTK to use normal drawing planes instead of overlay
  planes. This is usually necessary if your menu contains multi-color
  images. Does nothing except on Irix.
*/

/*! \fn void MenuWindow::set_overlay()
  Undoes clear_overlay().
*/

#if USE_OVERLAY && !defined(__sgi)
#undef USE_OVERLAY
#define USE_OVERLAY 0
#endif

#ifdef _WIN32
# include <windows.h>
#endif

// _WIN32 note: USE_OVERLAY is false
#if USE_OVERLAY
extern XVisualInfo *fl_find_overlay_visual();
extern XVisualInfo *fl_overlay_visual;
extern Colormap fl_overlay_colormap;
extern unsigned long fl_transparent_pixel;
static GC menugc; // the GC used by all X windows
extern bool fl_overlay; // changes how color(x) works
#endif

void MenuWindow::create() {
  set_override();
#if USE_OVERLAY
  if (overlay() && fl_find_overlay_visual()) {
    XInstallColormap(xdisplay, fl_overlay_colormap);
    CreatedWindow::create(this, fl_overlay_visual, fl_overlay_colormap,
			  int(fl_transparent_pixel));
  } else
#endif
    Window::create();
#ifdef _WIN32
  ::SetWindowPos(xid(this), HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
#endif
}

void MenuWindow::flush() {
#if USE_OVERLAY
  if (!fl_overlay_visual || !overlay()) {Window::flush(); return;}
  CreatedWindow *i = CreatedWindow::find(this);
  xwindow = i->xid;
  if (!menugc) menugc = XCreateGC(xdisplay, i->xid, 0, 0);
  fltk::gc = menugc;
  fl_overlay = true;
  current_ = this;
  bool expose =
    (damage() & DAMAGE_EXPOSE) && !(damage() & DAMAGE_ALL);
  if (expose) set_damage(damage() & ~DAMAGE_EXPOSE);
  if (damage()) draw();
  if (expose) {
    clip_region(i->region); i->region = 0;
    set_damage(DAMAGE_EXPOSE); draw();
    clip_region(0);
  }
  fl_overlay = false;
#else
  Window::flush();
#endif
}

void MenuWindow::destroy() {
#if USE_OVERLAY
  // Fix the colormap flashing on Maximum Impact Graphics by erasing the
  // menu before unmapping it:
  if (gc && shown()) XClearWindow(xdisplay, xid(this));
#endif
  Window::destroy();
}

MenuWindow::~MenuWindow() {
  destroy();
}

//
// End of "$Id: Fl_Menu_Window.cxx,v 1.22 2004/07/04 17:18:22 laza2000 Exp $".
//
