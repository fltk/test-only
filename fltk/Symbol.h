//
// "$Id$"
//
// The fltk drawing library
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

#include "FL_API.h"
#include "Color.h"
#include "Rectangle.h"
#include "PixelType.h"

namespace fltk {

class Style;

class FL_API Symbol {
  const char* name_;
  static const char* text_;

  // Forbid use of copy contructor and assign operator
  Symbol & operator=(const Symbol &);
  Symbol(const Symbol &);

 public:

  Symbol(const char* name=0);
  const char* name() const {return name_;}
  void name(const char*);

  virtual void _measure(int& w, int& h) const;
  void measure(int& w, int& h) const {_measure(w,h);}

  virtual void _draw(const Rectangle&) const = 0;
  void draw(const Rectangle& r) const {_draw(r);}
  void draw(const Rectangle& r, const Style*, Flags) const;

  uchar* readimage(uchar*, const Rectangle&) const;
  uchar* readimage(uchar*, const Rectangle&, int delta) const;
  uchar* readimage(uchar*, const Rectangle&, int delta, int linedelta) const;

  // Hints for widgets:
  virtual void inset(Rectangle& r) const;
  virtual bool fills_rectangle() const;
  virtual bool is_frame() const;

  int dx() const;
  int dy() const;
  int dw() const;
  int dh() const;

  // hash table lookup:
  static const Symbol* find(const char* name);
  static const Symbol* find(const char* start, const char* end);
  static const Symbol* iterate(int& index);
  static void text(const char* s) {text_=s;}
  static const char* text() {return text_;}
  
  // virtual ~Symbol() {}  // not done as it slows program exit
};

// Back-compatability constructor:
FL_API void add_symbol(const char* name, void (*drawit)(Color), int scalable);

}

#endif
