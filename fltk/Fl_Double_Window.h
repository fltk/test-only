//
// "$Id: Fl_Double_Window.h,v 1.3 2002/10/29 00:37:23 easysw Exp $"
//
// Double-buffered window header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Double_Window_H
#define Fl_Double_Window_H

#include "Fl_Window.h"

/**
   The Fl_Double_Window class provides a double-buffered window. If possible 
   this will use the X double buffering extension (Xdbe). If not, it will 
   draw the window data into an off-screen pixmap, and then copy it to the 
   on-screen window. 

   It is highly recommended that you put the following code before the 
   first show() of any window in your program: 

   Fl::visual(FL_DOUBLE|FL_INDEX)

   This makes sure you can use Xdbe on servers where double buffering does
   not exist for every visual. 
*/
class FL_API Fl_Double_Window : public Fl_Window {

public:

  virtual void layout();
  /**
   The destructor also deletes all the children. This allows a whole 
   tree to be deleted at once, without having to keep a pointer to 
   all the children in the user code.
  */
  ~Fl_Double_Window();

  /*@{*/
  /**
   Creates a new Fl_Double_Window widget using the given position, size,
   and label (title) string.
  */
  Fl_Double_Window(int W, int H, const char *l = 0) : Fl_Window(W,H,l) {}
  Fl_Double_Window(int X, int Y, int W, int H, const char *l = 0)
    : Fl_Window(X,Y,W,H,l) {}
  /*@}*/

protected:

  virtual void create();
  virtual void flush();
  void flush(bool eraseoverlay);
  virtual void destroy();

private:

  void free_backbuffer();

};

#endif

//
// End of "$Id: Fl_Double_Window.h,v 1.3 2002/10/29 00:37:23 easysw Exp $".
//
