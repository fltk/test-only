/
// "$Id$
/
// Shared image header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Shared_Image_
#  define Fl_Shared_Image_

#  include "Fl_Image.H


// Test function for adding new format
typedef Fl_Image *(*Fl_Shared_Handler)(const char *name, uchar *header
                                       int headerlen)

/** The Fl_Shared_Image class supports caching, loading, and drawing of
 * image files. Most applications will also want to link against the
 * fltk_images library and call thefl_register_images() function to suppor
 * standard image formats such as BMP, GIF, JPEG, and PNG. */
class FL_EXPORT Fl_Shared_Image : public Fl_Image 
  protected

  static Fl_Shared_Image **images_;	// Shared image
  static int	num_images_;		// Number of shared image
  static int	alloc_images_;		// Allocated shared image
  static Fl_Shared_Handler *handlers_;	// Additional format handler
  static int	num_handlers_;		// Number of format handler
  static int	alloc_handlers_;	// Allocated format handler

  const char	*name_;			// Name of image fil
  int		original_;		// Original image
  int		refcount_;		// Number of times this image has been use
  Fl_Image	*image_;		// The image that is share
  int		alloc_image_;		// Was the image allocated

  static int	compare(Fl_Shared_Image **i0, Fl_Shared_Image **i1)

  // Use get() and release() to load/delete images in memory..
  Fl_Shared_Image()
    /** The constructors create a new shared image record in the image cache
     
     * The constructors are protected and cannot be used directly from 
     * program. Use theget() method instead. *
  Fl_Shared_Image(const char *n, Fl_Image *img = 0)
    /** The destructor free all memory and server resources that are used b
     * the image. The destructor is protected and cannot be used directl
     * from a program. Use the release() method instead. *
  virtual ~Fl_Shared_Image()
  void add()
  void update()

  public

  const char	*name() { return name_; 
  int		refcount() { return refcount_; 
  void		release()
  void		reload()

  virtual Fl_Image *copy(int W, int H)
  Fl_Image *copy() { return copy(w(), h()); 
  virtual void color_average(Fl_Color c, float i)
  virtual void desaturate()
  virtual void draw(int X, int Y, int W, int H, int cx, int cy)
  void draw(int X, int Y) { draw(X, Y, w(), h(), 0, 0); 
  virtual void uncache()

  static Fl_Shared_Image *find(const char *n, int W = 0, int H = 0)
  static Fl_Shared_Image *get(const char *n, int W = 0, int H = 0)
  static Fl_Shared_Image **images()
  static int		num_images()
  static void		add_handler(Fl_Shared_Handler f)
  static void		remove_handler(Fl_Shared_Handler f)
}


/** This function is provided in the fltk_images library an
 * registers all of the "extra" image file formats that are not par
 * of the core FLTK library..
 *
FL_EXPORT extern void fl_register_images()

#endif // !Fl_Shared_Image_

/
// End of "$Id$
/
