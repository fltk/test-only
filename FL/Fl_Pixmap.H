//
// "$Id: Fl_Pixmap.H,v 1.10 1999/08/30 09:36:06 vincent Exp $"
//
// Pixmap header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Pixmap_H
#define Fl_Pixmap_H

#include "Fl_Image.H"

FL_EXPORT class Fl_Pixmap : public Fl_Image {
public:
  const char * const * data;
  Fl_Pixmap(const char * const * d) : data(d) {w = -1;}
  Fl_Pixmap(const uchar* const * d) : data((char**)d) {w = -1;}
  void measure(int&,int&);

  void draw(int, int, int, int, int cx, int cy);
  
  void draw(int X, int Y) {draw(X,Y,1000,1000,0,0);}

  void draw(int X, int Y, int W, int H, Fl_Flags f=0) {
    Fl_Image::draw(X,Y,W,H,f);
  }
};

#endif

//
// End of "$Id: Fl_Pixmap.H,v 1.10 1999/08/30 09:36:06 vincent Exp $".
//
