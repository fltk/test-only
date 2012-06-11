//
// "$Id$"
//
// Image header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Image_H
#define Fl_Image_H

#include <fltk3/Image.h>
#include <fltk3/Wrapper.h>
#include "Enumerations.H"

class Fl_Widget;
class Fl_Image;
struct Fl_Menu_Item;
struct Fl_Label;


namespace fltk3 {
  inline Fl_Image *_3to1_image(Image *w) { return w?(Fl_Image*)w->wrapper():0; }
}


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Image, Image)
FLTK3_WRAPPER_INTERFACE_IMAGE(Fl_Image, Image)
FLTK3_WRAPPER_INTERFACE_END()

FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_RGB_Image, RGBImage)
FLTK3_WRAPPER_INTERFACE_IMAGE(Fl_RGB_Image, RGBImage)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Image : public fltk3::Wrapper {
  
protected:

  void w(int W) {
    ((fltk3::Image*)_p)->w(W);
  }
  
  void h(int H) {
    ((fltk3::Image*)_p)->h(H);
  }
  
  void d(int D) {
    ((fltk3::Image*)_p)->d(D);
  }
  
  void ld(int LD) {
    ((fltk3::Image*)_p)->ld(LD);
  }
  
  void data(const char * const *p, int c) {
    ((fltk3::Image*)_p)->data(p, c);
  }
  
  void draw_empty(int X, int Y) {
    ((fltk3::Image*)_p)->draw_empty(X, Y);
  }
  
  // FIXME: 123-1 - Fl_Label not wrapped!
  static void labeltype(const Fl_Label *lo, int lx, int ly, int lw, int lh, Fl_Align la) {
    fltk3::Image::labeltype((fltk3::Label*)lo, lx, ly, lw, lh, fltk3::_1to3_align(la));
  }
  
  static void measure(const Fl_Label *lo, int &lw, int &lh) {
    fltk3::Image::measure((fltk3::Label*)lo, lw, lh);
  }

public:

  virtual Fl_Image* copy(int w, int h) {
    // FIXME: 123-1
    fltk3::Image* ret = 0;
    if ( pVCalls & pVCallImageCopyWH ) {
      ret = ((fltk3::Image_I*)_p)->copy(w, h);
    } else {
      pVCalls |= pVCallImageCopyWH;
      ret = ((fltk3::Image_I*)_p)->copy(w, h);
      pVCalls &= ~pVCallImageCopyWH;
    }
    return new Fl_Image( ret );
  }
  
  FLTK3_IMAGE_VCALLS(Fl_Image, Image)

  Fl_Image() { }
  
  Fl_Image(fltk3::Image *img) {
    // FIXME: 123-1
    _p = img;
    _p->wrapper(this);
  }
  
  int w() const {
    return ((fltk3::Image*)_p)->w();
  }
  
  int h() const {
    return ((fltk3::Image*)_p)->h();
  }
  
  int d() const {
    return ((fltk3::Image*)_p)->d();
  }
  
  int ld() const {
    return ((fltk3::Image*)_p)->ld();
  }
  
  int count() const {
    return ((fltk3::Image*)_p)->count();
  }
  
  const char * const *data() const {
    return ((fltk3::Image*)_p)->data();
  }
  
  Fl_Image(int W, int H, int D) {    
    // FIXME: 123-1
    _p = new fltk3::Image(W, H, D);
    _p->wrapper(this);
  }
  
  Fl_Image *copy() { 
    // FIXME: 123-1
   // return new Fl_Image( ((fltk3::Image*)_p)->copy() );
    return copy(w(), h());
  }
  
  void inactive() {
    ((fltk3::Image*)_p)->inactive();
  }

  void draw(int X, int Y) {
    ((fltk3::Image*)_p)->draw(X, Y);
  }

};


class FL_EXPORT Fl_RGB_Image : public Fl_Image {

public:

  FLTK3_IMAGE_VCALLS(Fl_RGB_Image, RGBImage)
  
  Fl_RGB_Image() { /* empty */ }

  Fl_RGB_Image(const uchar *bits, int W, int H, int D=3, int LD=0) {
    _p = new fltk3::RGBImage_I(bits, W, H, D, LD);
    _p->wrapper(this);
  }
  
  Fl_RGB_Image(fltk3::Image *img) {
    _p = img;
    _p->wrapper(this);
  }
    
  void draw(int X, int Y) {
    ((fltk3::Image*)_p)->draw(X, Y);
  }
  
};


namespace fltk3 {
  inline Image *_1to3_image(Fl_Image *w) { return w?((fltk3::Image*)w->_p):0; }
}


#endif // !Fl_Image_H

//
// End of "$Id$".
//
