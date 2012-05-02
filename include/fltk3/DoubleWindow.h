//
// "$Id$"
//
// Double-buffered window header file for the Fast Light Tool Kit (FLTK).
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

/* \file
 fltk3::DoubleWindow widget . */

#ifndef Fltk3_Double_Window_H
#define Fltk3_Double_Window_H

#include "Window.h"


namespace fltk3 {
  
  /**
   The fltk3::DoubleWindow provides a double-buffered window.
   If possible this will use the X double buffering extension (Xdbe).  If
   not, it will draw the window data into an off-screen pixmap, and then
   copy it to the on-screen window.
   <P>It is highly recommended that you put the following code before the
   first show() of <I>any</I> window in your program: </P>
   \code
   fltk3::visual(fltk3::DOUBLE|fltk3::INDEX)
   \endcode
   This makes sure you can use Xdbe on servers where double buffering
   does not exist for every visual.
   */
  class FLTK3_EXPORT DoubleWindow : public fltk3::Window {
    
  protected:
    void flush(int eraseoverlay);
    /**
     Force double buffering, even if the OS already buffers windows
     (overlays need that on MacOS and Windows2000)
     */
    char force_doublebuffering_;
  public:
    void show();
    void show(int a, char **b) {Window::show(a,b);}
    void flush();
    void resize(int,int,int,int);
    void hide();
    ~DoubleWindow();
    
    /**
     Creates a new fltk3::DoubleWindow widget using the given
     position, size, and label (title) string.
     */
    DoubleWindow(int W, int H, const char *l = 0);
    
    /**
     See  fltk3::DoubleWindow::DoubleWindow(int w, int h, const char *label = 0)
     */
    DoubleWindow(int X, int Y, int W, int H, const char *l = 0);
    
  };
  
} // namespace

#endif

//
// End of "$Id$".
//
