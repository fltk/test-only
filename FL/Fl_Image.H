//
// "$Id: Fl_Image.H,v 1.9 1999/10/17 20:40:05 vincent Exp $"
//
// Image header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Image_H
#define Fl_Image_H

#include "Fl_Flags.H"

class Fl_Widget;
struct Fl_Menu_Item;

class FL_API Fl_Image {
protected:
  ulong id, mask;
  void _draw(int X, int Y, int W, int H, int cx, int cy);
public:
  int w, h; // public for back compatibility only

  Fl_Image() : id(0), mask(0) {}
  virtual void measure(int& W, int& H); // this is the safe way to get the size of an image
  virtual void draw(int,int,int,int, int cx, int cy) = 0;
  virtual ~Fl_Image();

  // the following draw methods will use the first defined virtual draw method
  // and do not need to be redefined in subclasses of Fl_Image
  void draw(int X, int Y, int W, int H, Fl_Flags f=0);
  void draw(int X, int Y) { int w, h; measure(w, h); draw(X, Y, w, h, 0, 0); }
  void draw_tiled(int X, int Y, int W, int H, int cx=0, int cy=0);

  // back compatability:
  void label(Fl_Widget* o);
  void label(Fl_Menu_Item* o);
};

#endif

//
// End of "$Id: Fl_Image.H,v 1.9 1999/10/17 20:40:05 vincent Exp $".
//
