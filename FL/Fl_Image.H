//
// "$Id: Fl_Image.H,v 1.6 1999/08/16 07:30:50 bill Exp $"
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

FL_EXPORT class Fl_Image {
protected:
  ulong id, mask;
  void _draw(int X, int Y, int W, int H, int cx, int cy);
public:
  int w, h;
  Fl_Image() : id(0), mask(0) {}
  virtual void measure(int W, int H);
  virtual void draw(int,int,int,int, int cx, int cy) = 0;
  virtual ~Fl_Image();

  void draw(int X, int Y, int W, int H, Fl_Flags);

  // back compatability:
  void Fl_Image::label(Fl_Widget* o);
  void Fl_Image::label(Fl_Menu_Item* o);
};

#endif

//
// End of "$Id: Fl_Image.H,v 1.6 1999/08/16 07:30:50 bill Exp $".
//
