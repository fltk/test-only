//
// "$Id$"
//
// Image header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper started
//  - image base wrapper class must be implemented including missing flags
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

#ifndef Fl_Image_H
#define Fl_Image_H

#include <fltk3/Image.h>
#include <fltk3/Wrapper.h>
#include "Enumerations.H"

class Fl_Widget;
struct Fl_Menu_Item;
struct Fl_Label;


namespace fltk3 {
  inline Fl_Image *_3to1_image(Image *w) { return (Fl_Image*)w->wrapper(); }
}


class FL_EXPORT Fl_Image : public fltk3::Wrapper {
  
  Fl_Image & operator=(const Fl_Image &);
  Fl_Image(const Fl_Image &);

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
  
  // FIXME: 123 - Fl_Label not wrapped!
  static void labeltype(const Fl_Label *lo, int lx, int ly, int lw, int lh, Fl_Align la) {
    fltk3::Image::labeltype((fltk3::Label*)lo, lx, ly, lw, lh, fltk3::_1to3_align(la));
  }
  
  static void measure(const Fl_Label *lo, int &lw, int &lh) {
    fltk3::Image::measure((fltk3::Label*)lo, lw, lh);
  }

public:

  Fl_Image() {}
  
  Fl_Image(fltk3::Image *img) {
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
    _p = new fltk3::Image(W, H, D);
    _p->wrapper(this);
  }
  
  // FIXME: 123 - virtual
  Fl_Image *copy(int W, int H) {
    return new Fl_Image( ((fltk3::Image*)_p)->copy(W, H) );
  }
  
  Fl_Image *copy() { 
    return new Fl_Image( ((fltk3::Image*)_p)->copy() );
  }
  
  // FIXME: 123 - virtual
  void color_average(Fl_Color c, float i) {
    ((fltk3::Image*)_p)->color_average(fltk3::_1to3_color(c), i);
  }
  
  void inactive() {
    ((fltk3::Image*)_p)->inactive();
  }

  // FIXME: 123 - virtual
  virtual void desaturate();
  // FIXME: 123 - virtual
  virtual void label(Fl_Widget*w);
  // FIXME: 123 - virtual
  virtual void label(Fl_Menu_Item*m);
  // FIXME: 123 - virtual
  virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0);
  // FIXME: 123 - virtual
  virtual void uncache();

  void draw(int X, int Y) {
    ((fltk3::Image*)_p)->draw(X, Y);
  }

};


class FL_EXPORT Fl_RGB_Image : public Fl_Image {

public:

  Fl_RGB_Image(const uchar *bits, int W, int H, int D=3, int LD=0) {
    _p = new fltk3::RGBImage(bits, W, H, D, LD);
    _p->wrapper(this);
  }
  
  Fl_RGB_Image(fltk3::Image *img) {
    _p = img;
    _p->wrapper(this);
  }
  
  Fl_Image *copy() { 
    return new Fl_RGB_Image( ((fltk3::RGBImage*)_p)->copy() );
  }
  
  void draw(int X, int Y) {
    ((fltk3::Image*)_p)->draw(X, Y);
  }
  
  // FIXME: 123 - virtual
  virtual Fl_Image *copy(int W, int H);
  // FIXME: 123 - virtual
  virtual void color_average(Fl_Color c, float i);
  // FIXME: 123 - virtual
  virtual void desaturate();
  // FIXME: 123 - virtual
  virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0);
  // FIXME: 123 - virtual
  virtual void label(Fl_Widget*w);
  // FIXME: 123 - virtual
  virtual void label(Fl_Menu_Item*m);
  // FIXME: 123 - virtual
  virtual void uncache();
    
};

#endif // !Fl_Image_H

//
// End of "$Id$".
//
