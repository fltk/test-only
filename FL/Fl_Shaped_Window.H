/
// "$Id$
/
// Image file header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-1999 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@easysw.com"
/

#ifndef Fl_Shaped_Window_
#define Fl_Shaped_Window_

#include <FL/Fl_Window.H
#include <FL/Fl_Bitmap.H

class FL_EXPORT Fl_Shaped_Window : public Fl_Window 
  public
    Fl_Shaped_Window(int W, int H, const char *l = 0
      : Fl_Window(W,H,l) { set_flag(8); shape_ = 0; lw = lh = 0; changed = 0; 
    Fl_Shaped_Window(int X, int Y, int W, int H, const char *l = 0
      : Fl_Window(X,Y,W,H,l) { set_flag(8); shape_ = 0; lw = lh = 0; 
    void shape(Fl_Bitmap* b) { shape_ = b; changed = 1; 
    void shape(Fl_Bitmap& b) { shape(&b); 

  protected
    virtual void draw()
    Fl_Bitmap* shape_
    int lw, lh
    int changed
}


#endi

/
// End of "$Id$
/
