//
// "$Id: Fl_Single_Window.h,v 1.2 2002/10/29 00:37:23 easysw Exp $"
//
// Single-buffered window header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Single_Window_H
#define Fl_Single_Window_H

#include "Fl_Window.h"

/**

   This is the same as Fl_Window. However, it is possible that some 
   implementations will provide double-buffered windows by default. 
   This subclass can be used to force single-buffering. This may be 
   useful for modifying existing programs that use incremental update, 
   or for some types of image data, such as a movie flipbook. 

*/
class FL_API Fl_Single_Window : public Fl_Window {
public:
  /*@{*/
  /**
   Creates a new Fl_Single_Window widget using the given position,
   size, and label (title) string.
  */

  Fl_Single_Window(int W, int H, const char *l=0)
    : Fl_Window(W,H,l) {}
  Fl_Single_Window(int X, int Y, int W, int H, const char *l=0)
    : Fl_Window(X,Y,W,H,l) {}
  /*@}*/
};

#endif

//
// End of "$Id: Fl_Single_Window.h,v 1.2 2002/10/29 00:37:23 easysw Exp $".
//
