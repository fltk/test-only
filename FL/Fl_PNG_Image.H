//
// "$Id: Fl_PNG_Image.H,v 1.1.2.3.2.2 2003/11/19 02:46:11 matthiaswm Exp $"
//
// PNG image header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#ifndef Fl_PNG_Image_H
#define Fl_PNG_Image_H
#  include "Fl_Image.H"

/** The Fl_PNG_Image class supports loading, caching, and drawing of 
 * Portable Network Graphics (PNG) image files. The class loads colormapped
 * and full-color images and handles color- and alpha-based transparency. 
 */
class FL_EXPORT Fl_PNG_Image : public Fl_RGB_Image {

  public:
    /** The constructor loads the named PNG image. */
  Fl_PNG_Image(const char* filename);
};

#endif

//
// End of "$Id: Fl_PNG_Image.H,v 1.1.2.3.2.2 2003/11/19 02:46:11 matthiaswm Exp $".
//
