//
// "$Id: Fl_Tiled_Image.h,v 1.2 2002/01/31 22:54:49 robertk Exp $"
//
// Tiled image header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2001 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef Fl_Tiled_Image_h
#define Fl_Tiled_Image_h

#include "Fl_Image.h"

// Tiled image class. 
class FL_API Fl_Tiled_Image : public Fl_Image {
protected:
  Fl_Image* image_;
public:
  Fl_Tiled_Image(Fl_Image *i) : Fl_Image(), image_(i) {}
  Fl_Image* image() const {return image_;}
  void image(Fl_Image* i) {image_ = i;}
  void measure(int& w, int& h);
  void draw(int x, int y, int w, int h, Fl_Flags = 0);
};

#endif // !Fl_Tiled_Image_H

//
// End of "$Id: Fl_Tiled_Image.h,v 1.2 2002/01/31 22:54:49 robertk Exp $"
//
