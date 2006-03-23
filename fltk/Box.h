//
// "$Id$"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef fltk_Box_h
#define fltk_Box_h

#include "Color.h"
#include "Flags.h"
#include "Symbol.h"

namespace fltk {

///////////////////////////////////////////////////////////////////
class FL_API Box : public Symbol {
protected:
  int dx_, dy_, dw_, dh_;
public:
  Box(const char* name=0) : Symbol(name), dx_(0), dy_(0), dw_(0), dh_(0) {};
  Box(const char* name, int dx, int dy, int dw, int dh) : Symbol(name), dx_(dx), dy_(dy), dw_(dw), dh_(dh) {};
    
// fabien:  IMPORTANT IMPLEMENTATION CHANGE NOTE:
// add a way to access these important border FRAME dimensions int the Box class
// As all default boxes are not FrameBox but are Box* we NEED to determine 
// these from the Box class level and NOT frome the FrameBox level
// WAS: My intent was that Symbol::inset() does this. However it may make
// sense to move these variables from FrameBox to the Symbol base class,
// so the common case where the insets are constant works.
  int dx() const {return dx_;}
  int dy() const {return dy_;}
  int dw() const {return dw_;}
  int dh() const {return dh_;}
};

// fabien: let's add those facilities as in fltk1.1x but done the fltk2 way ...
// WAS: null is not allowed for boxes
inline int box_dx(const Box *b) {return b->dx();}
inline int box_dy(const Box *b) {return b->dy();}
inline int box_dw(const Box *b) {return b->dw();}
inline int box_dh(const Box *b) {return b->dh();}

class FL_API FrameBox : public Box {
protected:
  const char* data_;
  const FrameBox* down_;
public:
  const char* data() const {return data_;}
  void _draw(const Rectangle&) const;
  void inset(Rectangle&) const;
  bool fills_rectangle() const;
  bool is_frame() const;
  FrameBox(const char* name, int dx,int dy,int dw,int dh, const char* pattern)
    : Box(name,dx,dy,dw,dh), data_(pattern) {
      down_ = this; // fabien: cannot be null
  }
  FrameBox(const char* name, int dx,int dy,int dw,int dh, const char* pattern, const FrameBox* down)
      : Box(name,dx,dy,dw,dh), data_(pattern) {
      down_ = down ? down : this; // fabien: cannot be null
  }
};

class FL_API FlatBox : public Box {
public:
  void _draw(const Rectangle&) const;
  bool fills_rectangle() const;
  bool is_frame() const;
  FlatBox(const char* n);
};

class FL_API HighlightBox : public FlatBox {
  const Box* down_;
public:
  void _draw(const Rectangle&) const;
  void inset(Rectangle&) const;
  bool fills_rectangle() const;
  bool is_frame() const;
  HighlightBox(const char* n, const Box* d);
};

}

#endif

//
// End of "$Id$".
//
