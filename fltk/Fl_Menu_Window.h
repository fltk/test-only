//
// "$Id: Fl_Menu_Window.h,v 1.3 2002/10/29 00:37:23 easysw Exp $"
//
// Menu window header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu_Window_H
#define Fl_Menu_Window_H

#include "Fl_Single_Window.h"

/**

   The Fl_Menu_Window widget is a window type used for menus and tooltips.
   In earlier versions of fltk it used hardware overlay planes if possible,
   but this is currently disabled because it does not work with 
   Xft anti-aliased fonts. It also turns on the override() bit to
   remove window borders. 

*/
class FL_API Fl_Menu_Window : public Fl_Single_Window {
  enum {NO_OVERLAY = 0x08000000};

public:

  virtual void create();
  virtual void flush();
  virtual void destroy();

  int overlay() {return !(flags()&NO_OVERLAY);}
  /**
   Tells FLTK to use hardware overlay planes if they are available. 
  */
  void set_overlay() {clear_flag(NO_OVERLAY);}
  /**
   Tells FLTK to use normal drawing planes instead of overlay planes.
   This is usually necessary if your menu contains multi-color pixmaps. 
  */
  void clear_overlay() {set_flag(NO_OVERLAY);}

  /** Destroys the window and all of its children. */
  ~Fl_Menu_Window();

  /*@{*/
  /**
   Creates a new Fl_Menu_Window widget using the given
   position, size, and label string. 
  */
  Fl_Menu_Window(int W, int H, const char *l = 0)
    : Fl_Single_Window(W,H,l) {}
  Fl_Menu_Window(int X, int Y, int W, int H, const char *l = 0)
    : Fl_Single_Window(X,Y,W,H,l) {}
  /*@}*/

};

#endif

//
// End of "$Id: Fl_Menu_Window.h,v 1.3 2002/10/29 00:37:23 easysw Exp $".
//
