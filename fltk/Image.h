// "$Id$"
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

#ifndef fltk_Image_h
#define fltk_Image_h

#include "Symbol.h"

namespace fltk {

class FL_API Widget;
class Picture;

class FL_API Image : public Symbol {

  friend class fltk::Picture; // system-specific data for drawn image
  fltk::Picture* picture;
protected:
  int w_, h_;
  void set_alpha_bitmap(const uchar* bitmap, int w, int h);

public:

  Image(const char* name=0) : Symbol(name), picture(0), w_(-1), h_(-1) {}
  Image(int w, int h, const char* name=0) : Symbol(name), picture(0), w_(w), h_(h) {}

  int w() const {return w_;}
  int width() const {return w_;}
  int h() const {return h_;}
  int height() const {return h_;}

  bool drawn() const;
  void redraw();
  void destroy();
  void setsize(int w, int h);
  void make_current();
  void over(int x, int y) const;

  // implementation as Symbol subclass:
  void _draw(const Rectangle&) const;
  void _measure(int& W, int& H) const;
  bool fills_rectangle() const;

  void draw(int x, int y) const;
  void draw(const Rectangle& r) const {_draw(r);} // un-hide the base class

  // for back compatability with fltk1 only:
  void label(Widget* o);

};

}

#endif

//
// End of "$Id$".
//
