//
// "$Id: Fl_Symbol.H,v 1.1.2.1 2005/01/27 21:24:33 rokan Exp $"
//
// Portable drawing function header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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


#ifndef Fl_Symbol_H
#define Fl_Symbol_H

#include <FL/Enumerations.H>

/** This small class is used for drawing various glyphs - arrows, checks etc. The constructor usually
    takes a pointer to a drawing function. The basic color used is the current one but other ones 
    can be used by using functions like fl_darker() etc. within the function.
    Class is copyable and copy constructable and should be used directly by value - not by a pointer.
*/


class FL_EXPORT Fl_Symbol{
  uchar type_;
  char scale_;
  void * draw_;
public:

  
  /** Function draws the symbol using current color. The glyph is usually scalled winthin x,y,w,h area. */
  void draw(int x, int y, int w, int h, Fl_Color color);

  
  Fl_Symbol():draw_(0){};
  // Following draw functions, used within the constructors, must be scalable and draw the glyph
  // within  [-1, -1], [1,1] area which is scalled to x,y,w,h

  Fl_Symbol(void (* draw)(Fl_Color c), char  size=0, bool square=0, uchar rot=0);


  // draw functions for these constructors draw dirrectly within x,y,w,h area. If square is set,
  // drawing is centered within x,y,w,h area, symbol itself is squared so that w==h whichever is smaller.

  Fl_Symbol(void (* draw)(int x, int y, int w, int h, Fl_Color c), char size = 0, bool square = 0);

  // Following draw functions are not scalled. x, y coordinates describe center of the symbol and when symbol is drawn
  // it is placed in the centre of x,y,w,h drawing area.
  Fl_Symbol(void (* draw)(int x, int y, Fl_Color c));

  // Label symbol strings can be used too construct symbols. Use trailling @ as well, eg Fl_Symbol("@+92->")
  Fl_Symbol(const char * s);

  // Even box-types can be used as symbols!
  Fl_Symbol(Fl_Boxtype box, char size = 0, bool square = 0);

};


#endif
