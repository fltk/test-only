/
// "$Id$
/
// Value slider header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#ifndef Fl_Value_Slider_
#define Fl_Value_Slider_

#include "Fl_Slider.H

/** The Fl_Value_Slider widget is a Fl_Slider widget with a box displaying the curren
 * value.
 *   \image html value_slider.gi
 *
class FL_EXPORT Fl_Value_Slider : public Fl_Slider 
    uchar textfont_, textsize_
    unsigned textcolor_
public
    void draw()
    int handle(int)
      /** Creates a new Fl_Value_Slider widget using the given position, size, an
       * label string. The default boxtype is FL_DOWN_BOX . *
    Fl_Value_Slider(int x,int y,int w,int h, const char *l = 0)
      /** Gets the typeface of the text in the value box. *
    Fl_Font textfont() const {return (Fl_Font)textfont_;
      /** Sets the typeface of the text in the value box. *
    void textfont(uchar s) {textfont_ = s;
      /** Gets the size of the text in the value box. *
    uchar textsize() const {return textsize_;
      /** Sets the size of the text in the value box. *
    void textsize(uchar s) {textsize_ = s;
      /** Gets the color of the text in the value box. *
    Fl_Color textcolor() const {return (Fl_Color)textcolor_;
      /** Sets the color of the text in the value box. *
    void textcolor(unsigned s) {textcolor_ = s;
}

#endi

/
// End of "$Id$"
/
