//
// "$Id$"
//
// Fl_File_Icon definitions.
// FLTK 123 wrapper started
//  - no virtual members, the class is simply derived from FLTK3
//
// Copyright 1999-2011 by Michael Sweet.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifndef _Fl_Fl_File_Icon_H_
#define _Fl_Fl_File_Icon_H_

#include <fltk3/FileIcon.h>
#include "Fl.H"

#  define FL_ICON_COLOR (Fl_Color)0xffffffff	/**< icon color [background?]*/


class FL_EXPORT Fl_File_Icon : public fltk3::FileIcon {
  
public:

  Fl_File_Icon(const char *p, int t, int nd = 0, short *d = 0) 
  : fltk3::FileIcon(p, t, nd, d) {
  }

  ~Fl_File_Icon() { }

  short *add_color(Fl_Color c) {
    return fltk3::FileIcon::add_color(fltk3::_1to3_color(c));
  }

  void draw(int x, int y, int w, int h, Fl_Color ic, int active = 1) {
    fltk3::FileIcon::draw(x, y, w, h, fltk3::_1to3_color(ic), active);
  }

  void label(Fl_Widget *w) {
    fltk3::FileIcon::label(fltk3::_1to3_widget(w));
  }

  static void labeltype(const Fl_Label *o, int x, int y, int w, int h, Fl_Align a) {
    fltk3::FileIcon::labeltype((fltk3::Label*)o, x, y, w, h, fltk3::_1to3_align(a));
  }
  
  Fl_File_Icon	*next() { 
    return (Fl_File_Icon*)fltk3::FileIcon::next();
  }
  
  static Fl_File_Icon *find(const char *filename, int filetype = ANY) {
    return (Fl_File_Icon*)fltk3::FileIcon::find(filename, filetype);
  }

  static Fl_File_Icon *first() { 
    return (Fl_File_Icon*)fltk3::FileIcon::first();
  }
  
};

#endif // !_Fl_Fl_File_Icon_H_

//
// End of "$Id$".
//
