/
// "$Id$
/
// JPEG image header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_JPEG_Image_
#define Fl_JPEG_Image_
#  include "Fl_Image.H

/** The Fl_JPEG_Image class supports loading, caching, and drawing of Join
 * Photographic Experts Group (JPEG) File Interchange Format (JFIF) images
 * The class supports grayscale and color (RGB) JPEG image files. *
class FL_EXPORT Fl_JPEG_Image : public Fl_RGB_Image 

  public
    /** The constructor loads the named JPEG image. *
  Fl_JPEG_Image(const char* filename)
}

#endi

/
// End of "$Id$"
/
