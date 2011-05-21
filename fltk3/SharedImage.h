//
// "$Id: SharedImage.h 8306 2011-01-24 17:04:22Z matt $"
//
// Shared image header file for the Fast Light Tool Kit (FLTK).
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

/** \file
 fltk3::SharedImage class. */

#ifndef Fltk3_Shared_Image_H
#  define Fltk3_Shared_Image_H

#  include "Image.h"

namespace fltk3 {

  class JPEGImage;
  class PNGImage;
  
  // Test function for adding new formats
  typedef fltk3::Image *(*SharedHandler)(const char *name, uchar *header,
                                             int headerlen);
  
  // Shared images class. 
  /**
   This class supports caching, loading,
   and drawing of image files. Most applications will also want to
   link against the fltk_images library and call the
   fltk3::register_images()
   function to support standard image formats such as BMP, GIF, JPEG, and PNG.
   */
  class FLTK3_EXPORT SharedImage : public fltk3::Image {
    
    friend class JPEGImage;
    friend class PNGImage;
    
  protected:
    
    static fltk3::SharedImage **images_;	// Shared images
    static int	num_images_;		// Number of shared images
    static int	alloc_images_;		// Allocated shared images
    static fltk3::SharedHandler *handlers_;	// Additional format handlers
    static int	num_handlers_;		// Number of format handlers
    static int	alloc_handlers_;	// Allocated format handlers
    
    const char	*name_;			// Name of image file
    int		original_;		// Original image?
    int		refcount_;		// Number of times this image has been used
    fltk3::Image	*image_;		// The image that is shared
    int		alloc_image_;		// Was the image allocated?
    
    static int	compare(fltk3::SharedImage **i0, fltk3::SharedImage **i1);
    
    // Use get() and release() to load/delete images in memory...
    SharedImage();
    SharedImage(const char *n, fltk3::Image *img = 0);
    virtual ~SharedImage();
    void add();
    void update();
    
  public:
    /** Returns the filename of the shared image */
    const char	*name() { return name_; }
    /** Returns the number of references of this shared image. When reference is below 1, the image is deleted. */
    int		refcount() { return refcount_; }
    void		release();
    void		reload();
    
    virtual fltk3::Image *copy(int W, int H);
    fltk3::Image *copy() { return copy(w(), h()); }
    virtual void color_average(fltk3::Color c, float i);
    virtual void desaturate();
    virtual void draw(int X, int Y, int W, int H, int cx, int cy);
    void draw(int X, int Y) { draw(X, Y, w(), h(), 0, 0); }
    virtual void uncache();
    
    static fltk3::SharedImage *find(const char *n, int W = 0, int H = 0);
    static fltk3::SharedImage *get(const char *n, int W = 0, int H = 0);
    static fltk3::SharedImage **images();
    static int		num_images();
    static void		add_handler(fltk3::SharedHandler f);
    static void		remove_handler(fltk3::SharedHandler f);
  };
  
  //
  // The following function is provided in the fltk_images library and
  // registers all of the "extra" image file formats that are not part
  // of the core FLTK library...
  //
  
  
  FLTK3_EXPORT extern void register_images();
  
}

#endif // !Fl_Shared_Image_H

//
// End of "$Id: SharedImage.h 8306 2011-01-24 17:04:22Z matt $"
//
