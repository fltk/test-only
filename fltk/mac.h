//
// "$Id$"
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

namespace fltk {

////////////////////////////////////////////////////////////////
// constant information about the display:

//extern FL_API void    *qdisplay;
extern FL_API void	open_display();
extern FL_API void	close_display();
extern FL_API WindowPtr  quartz_window;
extern FL_API CGContextRef quartz_gc;
extern FL_API Handle    system_menu;
extern FL_API class Fl_Sys_Menu_Bar *sys_menu_bar;

////////////////////////////////////////////////////////////////
// event handling:

// we want some way to access last message from system

////////////////////////////////////////////////////////////////
// drawing functions:

extern FL_API void	clip_region(RgnHandle);
extern FL_API RgnHandle	clip_region();
extern FL_API void	draw_into(CGImageRef xid);
extern FL_API void	stop_drawing(CGImageRef xid);
extern FL_API ATSFontRef xfont();

extern FL_API void clear_quartz_clipping();
extern FL_API void begin_quartz_image(CGRect&, const Rectangle&);
extern FL_API void end_quartz_image();

////////////////////////////////////////////////////////////////
#ifdef fltk_Window_h // only include this if <fltk/Window.h> was included

// When fltk tells X about a window, one of these objects is created.
// Warning: this object is highly subject to change!  It's definition
// is only here so that fltk::xid(Window) can be declared inline:

class FL_API CreatedWindow {
public:
  WindowPtr xid; // used by main windows
  Window* window;
  RgnHandle region; // damage region
  void expose(const Rectangle&);
  CreatedWindow* next;
  RgnHandle subRegion; // region which clips out children
  CreatedWindow *children, *brother;
  bool wait_for_expose;
  bool need_new_subRegion;
  bool overlay;
  static CreatedWindow* first;
  static CreatedWindow* find(const Window* window) {return window->i;}
  static int borders(const Window* w, int& dx, int& dy, int& dw, int& dh);
  // Quartz additions:
  CGContextRef gc;
};

extern FL_API void fill_quartz_context();
extern FL_API void release_quartz_context(CreatedWindow *x=0);

// convert xid <-> Window:
//inline WindowPtr xid(const Window*w) {return CreatedWindow::find(w)->xid;}
WindowPtr xid(const Window*);
Window* find(WindowPtr xid);

extern CursHandle default_cursor;
extern CursHandle current_cursor;
extern const Widget* cursor_for;

#endif //Fl_Window_H
////////////////////////////////////////////////////////////////

}

#endif

//
// End of "$Id$".
//

