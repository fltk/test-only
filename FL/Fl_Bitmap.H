/
// "$Id$
/
// Bitmap header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
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
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#ifndef Fl_Bitmap_
#define Fl_Bitmap_
#  include "Fl_Image.H

class Fl_Widget
struct Fl_Menu_Item

/// Fl_Bitmap draws images are drawn using the current color.
/** It supports caching and drawing of mono-color (bitmap) images. *

class FL_EXPORT Fl_Bitmap : public Fl_Image 
  public

  const uchar *array
  int alloc_array; // Non-zero if data was allocate
 
    /** The constructors create a new bitmap from the specified bitmap data. *
  Fl_Bitmap(const uchar *bits, int W, int H) 
    Fl_Image(W,H,0), array(bits), alloc_array(0) {data((const char **)&array, 1);
  Fl_Bitmap(const char *bits, int W, int H) 
    Fl_Image(W,H,0), array((const uchar *)bits), alloc_array(0){data((const char **)&array, 1);
    /** The destructor frees all memory and server resources that are used by the bitmap. *
  virtual ~Fl_Bitmap()
  virtual Fl_Image *copy(int W, int H)
  Fl_Image *copy() { return copy(w(), h()); 
  virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0)
  void draw(int X, int Y) {draw(X, Y, w(), h(), 0, 0);
  virtual void label(Fl_Widget*w)
  virtual void label(Fl_Menu_Item*m)
  // virtual void uncache(); // RK: common uncache now..
}

#endi

/
// End of "$Id$"
/
