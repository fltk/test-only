//
// "$Id$"
//
// PNM image header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#ifndef Fl_PNM_Image_H
#define Fl_PNM_Image_H

#include <fltk3images/PNMImage.h>
#include "Fl_Image.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_PNM_Image, PNMImage)
FLTK3_WRAPPER_INTERFACE_IMAGE(Fl_PNM_Image, PNMImage)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_PNM_Image : public Fl_RGB_Image {

public:

  FLTK3_IMAGE_VCALLS(Fl_PNM_Image, PNMImage)

  Fl_PNM_Image(const char* filename) {
    // FIXME: 123 PNMImage_I
    _p = new fltk3::PNMImage(filename);
    _p->wrapper(this);
  }

};


#endif

//
// End of "$Id$".
//
