//
// "$Id: Symbol.h,v 1.5 2003/11/04 08:10:56 spitzak Exp $"
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

#include "FL_API.h"
#include "Flags.h"
#include "Color.h"

namespace fltk {

class FL_API Style;

struct BoxInfo {
  int dx,dy,dw,dh;
  int fills_rectangle;
};

class FL_API Symbol {
  const char* name_;
  static const char* text_;

  // Forbid use of copy contructor and assign operator
  Symbol & operator=(const Symbol &);
  Symbol(const Symbol &);

 public:

  Symbol(const char* name);
  const char* name() const {return name_;}
  void name(const char*);

  virtual void _measure(float& w, float& h) const;
  void measure(float& w, float& h) const {_measure(w,h);}
  void measure(int& w, int& h) const;

  virtual void _draw(float x,float y,float w,float h,const Style*,Flags) const;
  void draw(float x,float y,float w,float h,const Style* s,Flags f = 0) const {
    _draw(x,y,w,h,s,f);}

  virtual void _draw(int x, int y, int w, int h, const Style*,Flags) const;
  void draw(int x,int y,int w,int h,const Style* s,Flags f = 0) const {
    _draw(x,y,w,h,s,f);}
  //void draw(float x,float y,float w,float h,Flags f = 0) const;

  virtual const BoxInfo* boxinfo() const;
  int fills_rectangle() const {return boxinfo()->fills_rectangle;}
  int dx() const {return boxinfo()->dx;}
  int dy() const {return boxinfo()->dy;}
  int dw() const {return boxinfo()->dw;}
  int dh() const {return boxinfo()->dh;}
  void inset(int&X,int&Y,int&W,int&H) const {
    const BoxInfo* b = boxinfo(); X+=b->dx; Y+=b->dy; W-=b->dw; H-=b->dh;}

  static const Symbol* find(const char* name);

  static const Symbol* find(const char* start, const char* end);
  static void text(const char* s) {text_=s;}
  static const char* text() {return text_;}
};

// Back-compatability constructor:
FL_API void add_symbol(const char* name, void (*drawit)(Color), int scalable);

}

#endif
