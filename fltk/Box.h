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

typedef Symbol Box;

class FL_API FrameBox : public Box {
protected:
  const char* data_;
  const FrameBox* down;
  int dx, dy, dw, dh;
public:
  const char* data() const {return data_;}
  void _draw(const Rectangle&, const Style*, Flags) const;
  void inset(Rectangle&) const;
  bool fills_rectangle() const;
  bool is_frame() const;
  FrameBox(const char* n, const char* c, const FrameBox* d=0);
};

class FL_API FlatBox : public Box {
public:
  void _draw(const Rectangle&, const Style*, Flags) const;
  bool fills_rectangle() const;
  bool is_frame() const;
  FlatBox(const char* n);
};

class FL_API HighlightBox : public FlatBox {
  const Box* down;
public:
  void _draw(const Rectangle&, const Style*, Flags) const;
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
