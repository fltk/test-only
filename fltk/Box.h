//
// "$Id: Box.h,v 1.3 2003/11/04 08:10:56 spitzak Exp $"
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

// draw a pattern of lines around a box, pattern is from the data string
// in which 'A'=GRAY00, 'X'=GRAY99, other characters are gray levels:
class FL_API FrameBox : public Box {
protected:
  const char* data_;
  const FrameBox* down;
  BoxInfo boxinfo_;
public:
  const char* data() const {return data_;}
  void _draw(int, int, int, int, const Style*, Flags) const;
  const BoxInfo* boxinfo() const;
  FrameBox(const char* n, const char* c, const FrameBox* d=0);
};

// no border, tiling pattern is in absolute coordinates:
class FL_API FlatBox : public Box {
public:
  void _draw(int, int, int, int, const Style*, Flags) const;
  const BoxInfo* boxinfo() const;
  FlatBox(const char* n);
};

// Combination of flat & any other boxtype when highlighted or pressed:
class FL_API HighlightBox : public FlatBox {
  const Box* down;
public:
  void _draw(int, int, int, int, const Style*, Flags) const;
  const BoxInfo* boxinfo() const;
  HighlightBox(const char* n, const Box* d);
};

}

#endif

//
// End of "$Id: Box.h,v 1.3 2003/11/04 08:10:56 spitzak Exp $".
//
