//
// "$Id: Fl_Value_Slider.H,v 1.5.2.3.2.3.2.3 2003/12/02 02:51:44 easysw Exp $"
//
// Value slider header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Value_Slider_H
#define Fl_Value_Slider_H

#include "Fl_Slider.H"

/** The Fl_Value_Slider widget is a Fl_Slider widget with a box displaying the current
 * value. 
 *   \image html value_slider.gif
 */
class FL_EXPORT Fl_Value_Slider : public Fl_Slider {
    uchar textfont_, textsize_;
    unsigned textcolor_;
public:
    void draw();
    int handle(int);
      /** Creates a new Fl_Value_Slider widget using the given position, size, and
       * label string. The default boxtype is FL_DOWN_BOX . */
    Fl_Value_Slider(int x,int y,int w,int h, const char *l = 0);
      /** Gets the typeface of the text in the value box. */
    Fl_Font textfont() const {return (Fl_Font)textfont_;}
      /** Sets the typeface of the text in the value box. */
    void textfont(uchar s) {textfont_ = s;}
      /** Gets the size of the text in the value box. */
    uchar textsize() const {return textsize_;}
      /** Sets the size of the text in the value box. */
    void textsize(uchar s) {textsize_ = s;}
      /** Gets the color of the text in the value box. */
    Fl_Color textcolor() const {return (Fl_Color)textcolor_;}
      /** Sets the color of the text in the value box. */
    void textcolor(unsigned s) {textcolor_ = s;}
};

#endif

//
// End of "$Id: Fl_Value_Slider.H,v 1.5.2.3.2.3.2.3 2003/12/02 02:51:44 easysw Exp $".
//
