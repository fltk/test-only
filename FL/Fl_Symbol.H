//
// "$Id$"
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



/** This small class is used for drawing various glyphs - arrows, checks etc. The constructor usually
    takes a pointer to a drawing function. The basic color used is the current one but other ones 
    can be used by using functions like fl_darker() etc. within the function.
    Class is copyable and copy constructable and should be used directly by value - not by a pointer.
*/


class FL_EXPORT Fl_Symbol{
  uchar type_;
  void * draw_;
  short dx_, dy_, dw_, dh_;
  Fl_Symbol * down_;

 
public:
  Fl_Box_Draw_F * function(){if(!(type_ & 0x03)) return (Fl_Box_Draw_F *) draw_; else return 0;}

  Fl_Symbol():type_(0),draw_(0), dx_(0), dy_(0), dw_(0), dh_(0), down_(0){};
  void draw(int x, int y, int w, int h, Fl_Color c);
  Fl_Symbol * down();
  Fl_Symbol & down(Fl_Symbol * s){down_ = s; return *this;}
  int dx() const {return dx_;}
  int dy() const {return dy_;}
  int dw() const {return dw_;}
  int dh() const {return dh_;}

  Fl_Symbol & square(bool s){ if(s) type_ |=0x80; else  type_ &= ~((uchar)0x80); return  * this;}
  bool square(){return type_ & 0x80;}
 

  
  /** Function draws the symbol using current color. The glyph is usually scalled winthin x,y,w,h area. */
  void draw(int x, int y, int w, int h, Fl_Color color) const;


  
  


  // draw functions for these constructors draw dirrectly within x,y,w,h area. If square is set,
  // drawing is centered within x,y,w,h area, symbol itself is squared so that w==h whichever is smaller.

  Fl_Symbol & set(void (* draw)(int x, int y, int w, int h, Fl_Color c), int dx =0, int dy =0, int dw =0, int dh =0){
    type_ = 0; draw_ = (void *)draw; dx_ = dx; dy_ = dy; dw_ = dw; dh_ = dh; down_ = 0; return * this;}

  // Following draw functions, used within the constructors, must be scalable and draw the glyph
  // within  [-1, -1], [1,1] area which is scalled to x,y,w,h
  Fl_Symbol & set(void (* draw)(Fl_Color), uchar rot=0);



  Fl_Symbol & set(void (* draw)(int x, int y, Fl_Color)){type_ = 1; draw_ = (void *)draw; dx_ = dy_ = dw_ = dh_ = 0; down_ = 0; return * this;} 

  // Label symbol strings can be used too construct symbols. Use trailling @ as well, eg Fl_Symbol("@+92->")
  Fl_Symbol & set(const char * s){type_ = 2; draw_ = (void *)s; dx_ = dy_ = dw_ = dh_ = 0; down_ = 0; return * this;}

  Fl_Symbol & set(Fl_Symbol * s){*this = * s; if(s == s->down()) down_ = 0; return *this;}
  Fl_Symbol & set(Fl_Symbol &s){*this = s; if(&s == s.down()) down_ = 0; return *this;}


  Fl_Symbol(void (* draw)(Fl_Color c), uchar rot=0){set(draw , rot);}
  Fl_Symbol(void (* draw)(int x, int y, int w, int h, Fl_Color c), int dx, int dy, int dw, int dh){set(draw, dx, dy, dw, dh);}
  Fl_Symbol(void (* draw)(int x, int y, int w, int h, Fl_Color c)){set(draw,0,0,0,0);}

};


#endif
