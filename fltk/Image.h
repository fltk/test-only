// "$Id$"
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

#ifndef fltk_Image_h
#define fltk_Image_h

#include "Symbol.h"
# undef OPAQUE // fix for windows.h

namespace fltk {

class FL_API Widget;
class FL_API xbmImage;
class DrawImageHelper;

class FL_API Image : public Symbol {

  void* picture;	// system-specific data for the drawn image
  int flags;		// flags indicating the state of picture
  friend class xbmImage;
  friend class DrawImageHelper;
  enum {DRAWN=1, OPAQUE=2, MASK=4, USES_FG=8, USES_BG=16};

protected:
  int w_, h_;

public:
  Image(const char* name=0) : Symbol(name), picture(0), flags(0), w_(-1), h_(-1) {}
  Image(int w, int h, const char* name=0) : Symbol(name), picture(0), flags(0), w_(w), h_(h) {}

  int w() const {return w_;}
  int width() const {return w_;}
  int h() const {return h_;}
  int height() const {return h_;}

  virtual void update() = 0;
  bool drawn() const;
  void redraw() {flags &= ~DRAWN;}
  void destroy();
  void setsize(int w, int h);
  void make_current();
  void over(const Rectangle& from, const Rectangle& to) const;

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
