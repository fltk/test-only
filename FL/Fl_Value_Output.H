//
// "$Id$"
//
// Value output header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Value_Output_H
#define Fl_Value_Output_H

#ifndef Fl_Valuator_H
#include "Fl_Valuator.H"
#endif

/** The Fl_Value_Output widget displays a floating point value. If step() is not zero,
 * the user can adjust the value by dragging the mouse left and right. The left button
 * moves one step() per pixel, the middle by 10 * step(), and the right button by 
 * 100 * step().
 *
 * This is much lighter-weight than Fl_Value_Input because it contains no text editing
 * code or character buffer. 
 *   \image html Fl_Value_Output.gif
 */
class FL_EXPORT Fl_Value_Output : public Fl_Valuator {
  uchar textfont_, textsize_, soft_;
  unsigned textcolor_;
public:
  int handle(int);
  void draw();
    /** Creates a new Fl_Value_Output widget using the given position, size, and label
     * string. The default boxtype is \c FL_NO_BOX . */
  Fl_Value_Output(int x,int y,int w,int h,const char *l=0);

    /** If "soft" is turned on, the user is allowed to drag the value outside the
     * range. If they drag the value to one of the ends, let go, then grab again and
     * continue to drag, they can get to any value. Default is one. */
  void soft(uchar s) {soft_ = s;}
  uchar soft() const {return soft_;}

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
// End of "$Id$".
//
