//
// "$Id$"
//
// Bitmap header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper started
//  - virtual image functions missing
//  - "copy" needs to generate a wrapper
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

#ifndef Fl_Bitmap_H
#define Fl_Bitmap_H

#include <fltk3/Bitmap.h>
#include "Fl_Image.H"

class Fl_Widget;
struct Fl_Menu_Item;

class FL_EXPORT Fl_Bitmap : public Fl_Image {

public:
  
  const uchar *get_array() { 
    return ((fltk3::Bitmap*)_p)->array;
  }
  
  int get_alloca_arrya() {
    return ((fltk3::Bitmap*)_p)->alloc_array;
  }

  Fl_Bitmap() {}

  Fl_Bitmap(const uchar *bits, int W, int H) {
    _p = new fltk3::Bitmap(bits, W, H);
    _p->wrapper(this);
  }

  Fl_Bitmap(const char *bits, int W, int H) {
    _p = new fltk3::Bitmap(bits, W, H);
    _p->wrapper(this);
  }

  // FIXME: 123 - virtual funciton!
  virtual Fl_Image *copy(int W, int H) {
    return 0;
  }
  
  // FIXME: 123 - virtual function! Creates a new object!
  Fl_Image *copy() { return 0L; }
  
  // FIXME: 123 - virtual function!
  virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0) { }
  
  void draw(int X, int Y) {
    ((fltk3::Bitmap*)_p)->draw(X, Y);
  }
  
  // FIXME: 123 - virtual function!
  virtual void label(Fl_Widget*w) { }
  
  // FIXME: 123 - virtual function!
  virtual void label(Fl_Menu_Item*m) {}
  
  // FIXME: 123 - virtual function!
  virtual void uncache() { }
};

#endif

//
// End of "$Id$".
//
