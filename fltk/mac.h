//
// "$Id: mac.h,v 1.10 2004/05/04 07:30:42 spitzak Exp $"
//
// Mac header file for the Fast Light Tool Kit (FLTK).
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

// Do not directly include this file, instead use <FL/x.h>.  It will
// include this file if "__APPLE__" is defined and "USE_X11" is UNdefined.
// This is to encourage portability of even the system-specific code...

#ifndef fltk_mac_h
#define fltk_mac_h

// Standard MacOS Carbon API includes...
#define Style XStyle
#include <Carbon/Carbon.h>
#undef Style

// Now make some fixes to the headers...
#undef check			// Dunno where this comes from...

////////////////////////////////////////////////////////////////
// Emulate X somewhat:

typedef WindowPtr XWindow;
struct XPoint { int x, y; };
struct XRectangle {int x, y, width, height;};
typedef RgnHandle Region;
typedef GWorldPtr Pixmap;

inline Region XRectangleRegion(int x, int y, int w, int h) {
  Region R = NewRgn();
  SetRectRgn(R, x, y, x+w, y+h);
  return R;
}
inline void XDestroyRegion(Region r) {DisposeRgn(r);}
#define XDestroyWindow(a,b) DisposeWindow(b)
#define XMapWindow(a,b) ShowWindow(b)
#define XUnmapWindow(a,b) HideWindow(b)

namespace fltk {

////////////////////////////////////////////////////////////////
// constant information about the display:

extern FL_API void	open_display();
extern FL_API void	close_display();
//extern FL_API void	xdisplay;
extern FL_API Handle	system_menu;
extern FL_API class Fl_Sys_Menu_Bar *sys_menu_bar;

////////////////////////////////////////////////////////////////
// event handling:

// we want some way to access last message from system

////////////////////////////////////////////////////////////////
// drawing functions:

extern FL_API void	clip_region(Region);
extern FL_API Region	clip_region();
extern FL_API void	draw_into(GWorldPtr xid);
extern FL_API void	stop_drawing(GWorldPtr xid);

////////////////////////////////////////////////////////////////
#ifdef fltk_Window_h // only include this if <fltk/Window.h> was included

// When fltk tells X about a window, one of these objects is created.
// Warning: this object is highly subject to change!  It's definition
// is only here so that fltk::xid(Window) can be declared inline:

class FL_API CreatedWindow {
public:
  WindowPtr xid; // used by main windows
  Window* window;
  Region region; // damage region
  void expose(int x, int y, int w, int h);
  CreatedWindow* next;
  Region subRegion; // region which clips out children
  CreatedWindow *children, *brother;
  bool wait_for_expose;
  bool need_new_subRegion;
  bool overlay;
  static CreatedWindow* first;
  static CreatedWindow* find(const Window* window) {return window->i;}
  static int borders(const Window* w, int& dx, int& dy, int& dw, int& dh);
  void free_gc() {}
};

// convert xid <-> Window:
inline XWindow xid(const Window*w) {return CreatedWindow::find(w)->xid;}
Window* find(XWindow xid);

extern CursHandle default_cursor;
extern CursHandle current_cursor;
extern const Widget* cursor_for;

#endif //Fl_Window_H
////////////////////////////////////////////////////////////////

}

#endif

//
// End of "$Id: mac.h,v 1.10 2004/05/04 07:30:42 spitzak Exp $".
//

