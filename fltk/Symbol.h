//
// "$Id: Symbol.h,v 1.1 2003/07/21 19:43:57 spitzak Exp $"
//
// The fltk drawing library
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

#ifndef fltk_Symbol_h
#define fltk_Symbol_h

#include "Flags.h" // for alignment values
#include "Color.h"

namespace fltk {

class FL_API Symbol {
  const char* name_;
 public:
  const char* name() const {return name_;}
  Symbol(const char* name);
  virtual void draw(const char* start, const char* end,
		    float x, float y) const = 0;
  virtual void measure(const char* start, const char* end,
		       float& w, float& h) const = 0;
  static const Symbol* find(const char* start, const char* end);
};

// Back-compatability constructor:
FL_API void add_symbol(const char* name, void (*drawit)(Color), int scalable);

}

#endif
