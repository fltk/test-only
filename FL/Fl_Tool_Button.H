//
// "$Id$"
//
// Lighted button header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

#ifndef Fl_Tool_Button_H
#define Fl_Tool_Button_H

#include "Fl_Button.H"

// Tool button only implements new style, by default it has flat_box


class Fl_Tool_Button_Style;

/**
 * Not yet documented.
 */
class FL_EXPORT Fl_Tool_Button : public Fl_Button {
protected:
  void draw();
public:
  typedef Fl_Tool_Button_Style Style;
  static Style * default_style();
  void style(Style *s ){Fl_Button::style((Fl_Button::Style *)s);}
  Style * style(){return (Style *) Fl_Widget::style();}

  uchar symbol_align() const; 
  void symbol_align(uchar a);



    /** Creates a new Fl_Return_Button widget using the given \a position, 
     * \a size, and \a label string. The default boxtype is \c FL_UP_BOX . */
  Fl_Tool_Button(int X, int Y, int W, int H,const char *label=0)
    : Fl_Button(X,Y,W,H,label){style(default_style());}

};

#endif

//
// End of "$Id$".
//
