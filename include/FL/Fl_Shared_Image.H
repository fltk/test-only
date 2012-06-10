//
// "$Id$"
//
// Shared image header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Shared_Image_H
#define Fl_Shared_Image_H

#include <fltk3/SharedImage.h>
#include "Fl_Image.H"


typedef fltk3::SharedHandler Fl_Shared_Handler;

FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Shared_Image, SharedImage)
FLTK3_WRAPPER_INTERFACE_IMAGE(Fl_Shared_Image, SharedImage)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Shared_Image : public Fl_Image {
  
  friend class Fl_JPEG_Image;
  friend class Fl_PNG_Image;

protected:

#if 0
  // FIXME: 123-1
  static Fl_Shared_Image **images_;	// Shared images
  static int	num_images_;		// Number of shared images
  static int	alloc_images_;		// Allocated shared images
  static Fl_Shared_Handler *handlers_;	// Additional format handlers
  static int	num_handlers_;		// Number of format handlers
  static int	alloc_handlers_;	// Allocated format handlers

  const char	*name_;			// Name of image file
  int		original_;		// Original image?
  int		refcount_;		// Number of times this image has been used
  Fl_Image	*image_;		// The image that is shared
  int		alloc_image_;		// Was the image allocated?

  static int	compare(Fl_Shared_Image **i0, Fl_Shared_Image **i1);

  Fl_Shared_Image();
  Fl_Shared_Image(const char *n, Fl_Image *img = 0);

  void add();
  void update();
#endif

public:

  FLTK3_IMAGE_VCALLS(Fl_Shared_Image, SharedImage)

#if 0
  // FIXME: 123-1
  /** Returns the filename of the shared image */
  const char	*name() { return name_; }
  /** Returns the number of references of this shared image. When reference is below 1, the image is deleted. */
  int		refcount() { return refcount_; }
#endif

  void release() {
    ((fltk3::SharedImage*)_p)->release();
  }

#if 0
  // FIXME: 123-1
  void		reload();

  Fl_Image *copy() { return copy(w(), h()); }
  
  void draw(int X, int Y) { draw(X, Y, w(), h(), 0, 0); }
  
#endif

  static Fl_Shared_Image *find(const char *n, int W = 0, int H = 0) {
    fltk3::SharedImage *img3 = fltk3::SharedImage::find(n, W, H);
    if (!img3) 
      return 0L;
    Fl_Shared_Image *img1 = (Fl_Shared_Image*)img3->wrapper();
    if (!img1) {
      img1 = new Fl_Shared_Image();
      img1->_p = img3;
      img3->wrapper(img1);
    }
    return img1;
  }

  static Fl_Shared_Image *get(const char *n, int W = 0, int H = 0) {
    fltk3::SharedImage *img3 = fltk3::SharedImage::get(n, W, H);
    if (!img3) 
      return 0L;
    Fl_Shared_Image *img1 = (Fl_Shared_Image*)img3->wrapper();
    if (!img1) {
      img1 = new Fl_Shared_Image();
      img1->_p = img3;
      img3->wrapper(img1);
    }
    return img1;
  }

#if 0
  // FIXME: 123-1
  static Fl_Shared_Image **images();
  static int		num_images();
#endif

#if 0 // TODO: 123 - can we port this?
  // FIXME: 123-1
  static void add_handler(Fl_Shared_Handler f) {
    fltk3::SharedImage::add_handler((fltk3::SharedHandler)f);
  }

  static void remove_handler(Fl_Shared_Handler f) {
    fltk3::SharedImage::add_handler((fltk3::SharedHandler)f);
  }
#endif

};

inline extern void fl_register_images() {
  fltk3::register_images();
}

#endif // !Fl_Shared_Image_H

//
// End of "$Id$"
//
