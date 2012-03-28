//
// "$Id$"
//
// Pixmap header file for the Fast Light Tool Kit (FLTK).
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

/* \file
 fltk3::Pixmap widget . */

#ifndef Fltk3_Pixmap_H
#define Fltk3_Pixmap_H
#  include "Image.h"
#if defined(WIN32)
#  include "x.h"
#endif

class Fl_Pixmap;


namespace fltk3 { 
  
  class QuartzGraphicsDriver;
  class GDIGraphicsDriver;
  class XlibGraphicsDriver;
  class Widget; 
  struct MenuItem;
  
  // Older C++ compilers don't support the explicit keyword... :(
#  if defined(__sgi) && !defined(_COMPILER_VERSION)
#    define explicit
#  endif // __sgi && !_COMPILER_VERSION
  
  /**
   The fltk3::Pixmap class supports caching and drawing of colormap
   (pixmap) images, including transparency.
   */
  class FLTK3_EXPORT Pixmap : public fltk3::Image {
    
    friend class ::Fl_Pixmap;
    
    friend class QuartzGraphicsDriver;
    friend class GDIGraphicsDriver;
    friend class GDIPrinterGraphicsDriver;
    friend class XlibGraphicsDriver;
    void copy_data();
    void delete_data();
    void set_data(const char * const *p);
    int prepare(int XP, int YP, int WP, int HP, int cx, int cy,
		int &X, int &Y, int &W, int &H);
    
  protected:
    
    Pixmap(const uchar *a, int b, int c, int d=3, int e=0) : Image(a, b, c, d, e) {}

    void measure();
    
  public:
    
    int alloc_data; // Non-zero if data was allocated
    
  private:
    
#if defined(WIN32)
    UINT pixmap_bg_color; // RGB color used for pixmap background
#endif // WIN32
#if defined(__APPLE__) || defined(WIN32)
    void *id_; // for internal use
    void *mask_; // for internal use (mask bitmap)
#else
    unsigned id_; // for internal use
    unsigned mask_; // for internal use (mask bitmap)
#endif // __APPLE__ || WIN32
    
  public:
    
    /**    The constructors create a new pixmap from the specified XPM data.  */
    explicit Pixmap(char * const * D) : fltk3::Image(-1,0,1), alloc_data(0), id_(0), mask_(0) {set_data((const char*const*)D); measure();}
    /**    The constructors create a new pixmap from the specified XPM data.  */
    explicit Pixmap(uchar* const * D) : fltk3::Image(-1,0,1), alloc_data(0), id_(0), mask_(0) {set_data((const char*const*)D); measure();}
    /**    The constructors create a new pixmap from the specified XPM data.  */
    explicit Pixmap(const char * const * D) : fltk3::Image(-1,0,1), alloc_data(0), id_(0), mask_(0) {set_data((const char*const*)D); measure();}
    /**    The constructors create a new pixmap from the specified XPM data.  */
    explicit Pixmap(const uchar* const * D) : fltk3::Image(-1,0,1), alloc_data(0), id_(0), mask_(0) {set_data((const char*const*)D); measure();}
    virtual ~Pixmap();
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
  
}

#endif

//
// End of "$Id$".
//
