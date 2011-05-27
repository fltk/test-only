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

#ifndef Fl_Pixmap_H
#define Fl_Pixmap_H

#include <fltk3/Pixmap.h>
#include "Fl_Image.H"

//class Fl_Widget;
//struct Fl_Menu_Item;


class FL_EXPORT Fl_Pixmap : public Fl_Image {
  
#if 0 // FIXME: 123
  friend class Fl_Quartz_Graphics_Driver;
  friend class Fl_GDI_Graphics_Driver;
  friend class Fl_Xlib_Graphics_Driver;
  void copy_data();
  void delete_data();
  void set_data(const char * const *p);

  protected:

  void measure();
#endif
  
public:
  
  Fl_Pixmap() {}

  explicit Fl_Pixmap(char * const * D) {
    _p = new fltk3::Pixmap(D);
    _p->wrapper(this);
  }
  
  explicit Fl_Pixmap(uchar * const * D) {
    _p = new fltk3::Pixmap(D);
    _p->wrapper(this);
  }
  
  explicit Fl_Pixmap(const char * const * D) {
    _p = new fltk3::Pixmap(D);
    _p->wrapper(this);
  }
  
  explicit Fl_Pixmap(const uchar * const * D) {
    _p = new fltk3::Pixmap(D);
    _p->wrapper(this);
  }
  
#if 0 // FIXME: 123
  int alloc_data; // Non-zero if data was allocated
  
private:

  
#if defined(__APPLE__) || defined(WIN32)
  void *id_; // for internal use
  void *mask_; // for internal use (mask bitmap)
#else
  unsigned id_; // for internal use
  unsigned mask_; // for internal use (mask bitmap)
#endif // __APPLE__ || WIN32
  
  public:

  virtual ~Fl_Pixmap();
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

#endif

//
// End of "$Id$".
//
