//
// "$Id: Image.h 8338 2011-01-30 09:24:40Z manolo $"
//
// Image header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

/* \file
 fltk3::Image, fltk3::RGBImage classes . */

#ifndef Fltk3_Image_H
#define Fltk3_Image_H

#include "Object.h"
#include "enumerations.h"


namespace fltk3 { 
  
  class QuartzGraphicsDriver;
  class GDIGraphicsDriver;
  class XlibGraphicsDriver;
  class Widget; 
  struct Label;
  struct MenuItem;
  
  /**
   fltk3::Image is the base class used for caching and
   drawing all kinds of images in FLTK. This class keeps track of
   common image data such as the pixels, colormap, width, height,
   and depth. Virtual methods are used to provide type-specific
   image handling.</P>
   
   <P>Since the fltk3::Image class does not support image
   drawing by itself, calling the draw() method results in
   a box with an X in it being drawn instead.
   */
  class FLTK3_EXPORT Image : public Object {
    int w_, h_, d_, ld_, count_;
    const char * const *data_;
    
    // Forbid use of copy contructor and assign operator
    Image & operator=(const fltk3::Image &);
    Image(const fltk3::Image &);
    
  protected:
    
    /**
     Sets the current image width in pixels.
     */
    void w(int W) {w_ = W;}
    /**
     Sets the current image height in pixels.
     */
    void h(int H) {h_ = H;}
    /**
     Sets the current image depth.
     */
    void d(int D) {d_ = D;}
    /**
     Sets the current line data size in bytes.
     */
    void ld(int LD) {ld_ = LD;}
    /** 
     Sets the current array pointer and count of pointers in the array.
     */
    void data(const char * const *p, int c) {data_ = p; count_ = c;}
    void draw_empty(int X, int Y);
    
    static void labeltype(const fltk3::Label *lo, int lx, int ly, int lw, int lh, fltk3::Align la);
    static void measure(const fltk3::Label *lo, int &lw, int &lh);
    
  public:
    
    /** 
     Returns the current image width in pixels.
     */
    int w() const {return w_;}
    /**     Returns the current image height in pixels.
     */
    int h() const {return h_;}
    /**
     Returns the current image depth. 
     The return value will be 0 for bitmaps, 1 for
     pixmaps, and 1 to 4 for color images.</P>
     */
    int d() const {return d_;}
    /**
     Returns the current line data size in bytes. 
     Line data is extra data that is included
     after each line of color image data and is normally not present.
     */
    int ld() const {return ld_;}
    /**
     The count() method returns the number of data values
     associated with the image. The value will be 0 for images with
     no associated data, 1 for bitmap and color images, and greater
     than 2 for pixmap images.
     */
    int count() const {return count_;}
    /**
     Returns a pointer to the current image data array. 
     Use the count() method to find the size of the data array.
     */
    const char * const *data() const {return data_;}
    
    /**
     The constructor creates an empty image with the specified
     width, height, and depth. The width and height are in pixels.
     The depth is 0 for bitmaps, 1 for pixmap (colormap) images, and
     1 to 4 for color images.
     */
    Image(int W, int H, int D) {w_ = W; h_ = H; d_ = D; ld_ = 0; count_ = 0; data_ = 0;}
    virtual ~Image();
    virtual fltk3::Image *copy(int W, int H);
    /**
     The copy() method creates a copy of the specified
     image. If the width and height are provided, the image is
     resized to the specified size. The image should be deleted (or in
     the case of fltk3::SharedImage, released) when you are done
     with it.
     */
    fltk3::Image *copy() { return copy(w(), h()); }
    virtual void color_average(fltk3::Color c, float i);
    /**
     The inactive() method calls
     color_average(fltk3::BACKGROUND_COLOR, 0.33f) to produce 
     an image that appears grayed out. <I>This method does not 
     alter the original image data.</I>
     */
    void inactive() { color_average(fltk3::GRAY, .33f); }
    virtual void desaturate();
    virtual void label(fltk3::Widget*w);
    virtual void label(fltk3::MenuItem*m);
    /**
     Draws the image with a bounding box. 
     This form specifies
     a bounding box for the image, with the origin
     (upper-lefthand corner) of the image offset by the cx
     and cy arguments.
     */
    virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0); // platform dependent
    /**
     Draws the image. 
     This form specifies the upper-lefthand corner of the image.
     */
    void draw(int X, int Y) {draw(X, Y, w(), h(), 0, 0);} // platform dependent
    virtual void uncache();
  };
  
  /**
   The fltk3::RGBImage class supports caching and drawing
   of full-color images with 1 to 4 channels of color information.
   Images with an even number of channels are assumed to contain
   alpha information, which is used to blend the image with the
   contents of the screen.</P>
   
   <P>fltk3::RGBImage is defined in
   &lt;fltk3/Image.h&gt;, however for compatibility reasons
   &lt;fltk3/RGBImage.h&gt; should be included.
   */
  class FLTK3_EXPORT RGBImage : public Image {
    friend class QuartzGraphicsDriver;
    friend class GDIGraphicsDriver;
    friend class XlibGraphicsDriver;
  public:
    
    const uchar *array;
    int alloc_array; // Non-zero if array was allocated
    
  private:
    
#if defined(__APPLE__) || defined(WIN32)
    void *id_; // for internal use
    void *mask_; // for internal use (mask bitmap)
#else
    unsigned id_; // for internal use
    unsigned mask_; // for internal use (mask bitmap)
#endif // __APPLE__ || WIN32
    
  public:
    
    /**  The constructor creates a new image from the specified data. */
    RGBImage(const uchar *bits, int W, int H, int D=3, int LD=0) :
    fltk3::Image(W,H,D), array(bits), alloc_array(0), id_(0), mask_(0) {data((const char **)&array, 1); ld(LD);}
    virtual ~RGBImage();
    virtual fltk3::Image *copy(int W, int H);
    fltk3::Image *copy() { return copy(w(), h()); }
    virtual void color_average(fltk3::Color c, float i);
    virtual void desaturate();
    virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0);
    void draw(int X, int Y) {draw(X, Y, w(), h(), 0, 0);}
    virtual void label(fltk3::Widget*w);
    virtual void label(fltk3::MenuItem*m);
    virtual void uncache();
  };
  
} // namespace

#endif // !Fltk3_Image_H

//
// End of "$Id: Image.h 8338 2011-01-30 09:24:40Z manolo $".
//
