// "$Id: Rectangle.h,v 1.1 2005/01/24 08:07:06 spitzak Exp $"
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#ifndef fltk_Rectangle_h
#define fltk_Rectangle_h

#include "FL_API.h"

namespace fltk {

class FL_API Rectangle {
  int x_, y_, w_, h_;

 public:

  /*! Left edge */
  int x() const {return x_;}
  /*! Top edge */
  int y() const {return y_;}
  /*! Distance between left and right edges */
  int w() const {return w_;}
  /*! Distance between top and bottom edges */
  int h() const {return h_;}
  /*! Return x()+w(), the right edge of the rectangle. */
  int r() const {return x_+w_;}
  /*! Return y()+h(), the bottom edge of the rectangle. */
  int b() const {return y_+h_;}
  /*! Move the rectangle so the left edge is at \a v. */
  void x(int v) {x_ = v;}
  /*! Move the rectangle so the top edge is at \a v. */
  void y(int v) {y_ = v;}
  /*! Change w() by moving the right edge. x() does not change. */
  void w(int v) {w_ = v;}
  /*! Change h() by moving the bottom edge. y() does not change. */
  void h(int v) {h_ = v;}
  /*! Change x() without changing r(), by changing the width. */
  void set_x(int v) {w_ -= v-x_; x_ = v;}
  /*! Change y() without changing b(), by changing the height. */
  void set_y(int v) {h_ -= v-y_; y_ = v;}
  /*! Change r() without changing x(), by changine the width. */
  void set_r(int v) {w_ = v-x_;}
  /*! Change b() without changing y(), by changine the height. */
  void set_b(int v) {h_ = v-y_;}
  /*! Set x(), y(), w(), and h() all at once. */
  void set(int x, int y, int w, int h) {x_=x; y_=y; w_=w; h_=h;}
  /*! Add \a d to x() without changing r() (it reduces w() by \a d). */
  void move_x(int d) {x_ += d; w_ -= d;}
  /*! Add \a d to y() without changing b() (it reduces h() by \a d). */
  void move_y(int d) {y_ += d; h_ -= d;}
  /*! Add \a d to r() and w(). */
  void move_r(int d) {w_ += d;}
  /*! Add \a d to b() and h(). */
  void move_b(int d) {h_ += d;}
  /*! Move all edges in by \a d. See also Symbol::inset() */
  void inset(int d) {x_ += d; y_ += d; w_ -= 2*d; h_ -= 2*d;}
  /*! Move entire rectangle by given distance in x and y. */
  void move(int dx, int dy) {x_ += dx; y_ += dy;}
  /*! True if w() or h() are less or equal to zero. */
  bool empty() const {return w_ <= 0 || h_ <= 0;}
  /*! Same as !empty(), true if w() and h() are both greater than zero. */
  bool notempty() const {return w_ > 0 && h_ > 0;}
  /*! Integer center position. Rounded to the left if w() is odd. */
  int center_x() const {return x_+(w_>>1);}
  /*! Integer center position. Rounded to lower y if h() is odd. */
  int center_y() const {return y_+(h_>>1);}
  /*! Where to put baseline to center current font nicely */
  int baseline_y() const;

  Rectangle() {}

  /*! Constructor that sets x(), y(), w(), and h(). */
  Rectangle(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h) {}

  /*! Constructor that sets x() and y() to zero, and sets w() and h(). */
  Rectangle(int w, int h) : x_(0), y_(0), w_(w), h_(h) {}

  /*! Copy constructor. */
  Rectangle(const Rectangle& r) : x_(r.x_),y_(r.y_),w_(r.w_),h_(r.h_) {}

  Rectangle(const Rectangle& r, int w, int h, int flags = 0);

  /*! True if rectangle contains the pixel who's upper-left corner is at x,y */
  bool contains(int x, int y) const {return x>=x_ && y>=y_ && x<x_+w_ && y<y_+h_;}
};

}

#endif


