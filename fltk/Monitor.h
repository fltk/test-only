//
// "$Id: Monitor.h,v 1.1 2003/10/28 17:45:13 spitzak Exp $"
//
// The basic fltk runtime. Every program needs to call this somewhere.
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
//

#ifndef fltk_Monitor_h
#define fltk_Monitor_h

#include "FL_API.h"

namespace fltk {

class FL_API Rectangle {
 public:
  int x_, y_, w_, h_;
  Rectangle() {} // allows arrays to be constructed quickly
  Rectangle(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h) {}
  void set(int x, int y, int w, int h) {x_=x; y_=y; w_=w; h_=h;}
  int x() const {return x_;}
  int y() const {return y_;}
  int w() const {return w_;}
  int h() const {return h_;}
  int r() const {return x_+w_;}
  int b() const {return y_+h_;}
};

class FL_API Monitor : public Rectangle {
  int depth_;
  float dpi_x_;
  float dpi_y_;
 public:
  Rectangle work; // Allows you to do rect->work.x(), etc.
  int depth() const {return depth_;}
  float dpi_x() const {return dpi_x_;}
  float dpi_y() const {return dpi_y_;}
  float dpi() const {return dpi_y_;}
  static int list(const Monitor**);
  static const Monitor& all();
  static const Monitor& find(int x, int y);
};

}

#endif


