//
// "$Id: Fl_Bar.h,v 1.1 2002/01/20 08:12:28 spitzak Exp $"
//
// Bar header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#ifndef Fl_Bar_H
#define Fl_Bar_H

#include <fltk/Fl_Group.h>

class FL_API Fl_Bar : public Fl_Group {
  bool open_;
  bool highlighted;
  bool pushed;
  int glyph_size_;
  int saved_size;
  void glyph_box(int& x, int& y, int& w, int& h);
public:
  Fl_Bar(int x,int y,int w ,int h,const char *l = 0);
  static Fl_Named_Style* default_style;
  enum { // values for type(int)
    VERTICAL	= GROUP_TYPE,
    HORIZONTAL	= GROUP_TYPE+1
  };
  void layout();
  int handle(int);
  void draw();
  bool opened() const {return open_;}
  bool opened(bool);
  bool open() {return opened(true);}
  bool close() {return opened(false);}
  int glyph_size() const {return glyph_size_;}
  void glyph_size(int v) {glyph_size_ = v;}
};

#endif

//
// End of "$Id: Fl_Bar.h,v 1.1 2002/01/20 08:12:28 spitzak Exp $".
//
