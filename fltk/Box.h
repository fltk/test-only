//
// "$Id: Box.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $"
//
// Define your own values for box() on a widget by making one of these.
//
// Copyright 2002 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef fltk_Box_h
#define fltk_Box_h

#include "Color.h"
#include "Flags.h"

namespace fltk {

class FL_API Box {
protected:
  int dx_, dy_, dw_, dh_;
  int fills_rectangle_;
public:
  virtual void draw(int,int,int,int, Color, Flags=0) const = 0;
  int fills_rectangle() const {return fills_rectangle_;}
  int dx() const {return dx_;}
  int dy() const {return dy_;}
  int dw() const {return dw_;}
  int dh() const {return dh_;}
  void inset(int&X,int&Y,int&W,int&H) const {X+=dx_; Y+=dy_; W-=dw_; H-=dh_;}
  const char* name;
  Box* next;
  static Box* first;
  Box() : name(0) {}
  Box(const char* n) : name(n), next(first) {first = this;}
  static Box* find(const char* name);
};

// draw a pattern of lines around a box, pattern is from the data string
// in which 'A'=GRAY00, 'X'=GRAY99, other characters are gray levels:
class FL_API FrameBox : public Box {
protected:
  const char* data_;
  const FrameBox* down;
public:
  const char* data() const {return data_;}
  void draw(int,int,int,int, Color, Flags=0) const;
  FrameBox(const char* n, const char* c, const FrameBox* d=0);
};

// no border, tiling pattern is in absolute coordinates:
class FL_API FlatBox : public Box {
public:
  void draw(int,int,int,int, Color, Flags=0) const;
  FlatBox(const char* n);
};

// Combination of flat & any other boxtype when highlighted or pressed:
class FL_API HighlightBox : public FlatBox {
  const Box* down;
public:
  void draw(int,int,int,int, Color, Flags=0) const;
  HighlightBox(const char* n, const Box* d);
};

}

#endif

//
// End of "$Id: Box.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $".
//
