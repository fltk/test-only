//
// "$Id$"
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

#if 0 // FIXME: 123
  
protected:

  void w(int W) {w_ = W;}
  void h(int H) {h_ = H;}
  void d(int D) {d_ = D;}
  void ld(int LD) {ld_ = LD;}
  void data(const char * const *p, int c) {data_ = p; count_ = c;}
  void draw_empty(int X, int Y);
  static void labeltype(const Fl_Label *lo, int lx, int ly, int lw, int lh, Fl_Align la);
  static void measure(const Fl_Label *lo, int &lw, int &lh);

#endif

public:

  Fl_Image() {}
  
  Fl_Image(fltk3::Image *img) {
    _p = img;
    _p->wrapper(this);
  }
  
#if 0 // FIXME: 123

  int w() const {return w_;}
  int h() const {return h_;}
  int d() const {return d_;}
  int ld() const {return ld_;}
  int count() const {return count_;}
  const char * const *data() const {return data_;}
  Fl_Image(int W, int H, int D) {w_ = W; h_ = H; d_ = D; ld_ = 0; count_ = 0; data_ = 0;}
  virtual ~Fl_Image();
  virtual Fl_Image *copy(int W, int H);
  
#endif
  
  Fl_Image *copy() { 
    return new Fl_Image( ((fltk3::Image*)_p)->copy() );
  }
  
#if 0 // FIXME: 123

  virtual void color_average(Fl_Color c, float i);
  
#endif
  
  void inactive() {
    ((fltk3::Image*)_p)->inactive();
  }

#if 0 // FIXME: 123

  virtual void desaturate();
  virtual void label(Fl_Widget*w);
  virtual void label(Fl_Menu_Item*m);
  virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0); // platform dependent
  void draw(int X, int Y) {draw(X, Y, w(), h(), 0, 0);} // platform dependent
  virtual void uncache();

#endif

};


class FL_EXPORT Fl_RGB_Image : public Fl_Image {

#if 0 // FIXME: 123

  friend class Fl_Quartz_Graphics_Driver;
  friend class Fl_GDI_Graphics_Driver;
  friend class Fl_Xlib_Graphics_Driver;
  
public:

  Fl_RGB_Image(const uchar *bits, int W, int H, int D=3, int LD=0);
  virtual ~Fl_RGB_Image();
  virtual Fl_Image *copy(int W, int H);
  Fl_Image *copy() { return copy(w(), h()); }
  virtual void color_average(Fl_Color c, float i);
  virtual void desaturate();
  virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0);
  void draw(int X, int Y) {draw(X, Y, w(), h(), 0, 0);}
  virtual void label(Fl_Widget*w);
  virtual void label(Fl_Menu_Item*m);
  virtual void uncache();
  
#endif
  
};

#endif // !Fl_Image_H

//
// End of "$Id$".
//
