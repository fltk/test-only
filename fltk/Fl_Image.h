//
// "$Id: Fl_Image.h,v 1.2 2001/08/05 21:12:15 spitzak Exp $"
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

#include "Fl_Flags.h"
#include "x.h"

class FL_API Fl_Widget;

class FL_API Fl_Image {
protected:
  int w, h;
  Pixmap id, mask;
  void _draw(int x, int y, Fl_Flags);
public:
  Fl_Image() : id(0), mask(0) {}
  virtual void measure(int& W, int& H);
  virtual void draw(int x, int y, Fl_Flags) = 0;
  virtual ~Fl_Image();

  void draw_tiled(int X, int Y, int W, int H, int cx=0, int cy=0);

  // back compatability:
  void label(Fl_Widget* o);
};

#endif

//
// End of "$Id: Fl_Image.h,v 1.2 2001/08/05 21:12:15 spitzak Exp $".
//
