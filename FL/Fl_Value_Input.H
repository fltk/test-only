//
// "$Id: Fl_Value_Input.H,v 1.5.2.3.2.4.2.2 2003/11/22 01:04:39 matthiaswm Exp $"
//
// Value input header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Value_Input_H
#define Fl_Value_Input_H

#include "Fl_Valuator.H"
#include "Fl_Input.H"

/** The Fl_Value_Input widget displays a numeric value. The user can click in the text
 * field and edit it - there is in fact a hidden Fl_Input widget with 
 * <tt>type(FL_FLOAT_INPUT)</tt> or <tt>type(FL_INT_INPUT)</tt> in there - and when
 * they hit return or tab the value updates to what they typed and the callback is done.
 * 
 * If step() is non-zero, the user can also drag the mouse across the object and thus
 * slide the value. The left button moves one step() per pixel, the middle by 
 * <tt>10 * step()</tt>, and the right button by <tt>100 * step()</tt>. It is
 * therefore impossible to select text by dragging across it, although clicking can
 * still move the insertion cursor.
 *
 *  If step() is non-zero and integral, then the range of numbers are limited to
 * integers instead of floating point values.
 *   \image html Fl_Value_Input.gif
 */
class FL_EXPORT Fl_Value_Input : public Fl_Valuator {
public:
  Fl_Input input;
private:
  char soft_;
  static void input_cb(Fl_Widget*,void*);
  virtual void value_damage(); // cause damage() due to value() changing
public:
  int handle(int);
  void draw();
  void resize(int,int,int,int);
    /** Creates a new Fl_Value_Input widget using the given position, size, and label
     * string. The default boxtype is \c FL_DOWN_BOX. */
  Fl_Value_Input(int x,int y,int w,int h,const char *l=0);

  void soft(char s) {soft_ = s;}
    /** If "soft" is turned on, the user is allowed to drag the value outside the 
     * range. If they drag the value to one of the ends, let go, then grab again and
     * continue to drag, they can get to any value. The default is true. */
  char soft() const {return soft_;}

    /** Gets the typeface of the text in the value box. */
  Fl_Font textfont() const {return input.textfont();}
    /** Sets the typeface of the text in the value box. */
  void textfont(uchar s) {input.textfont(s);}
    /** Gets the size of the text in the value box. */
  uchar textsize() const {return input.textsize();}
    /** ets the size of the text in the value box. */
  void textsize(uchar s) {input.textsize(s);}
    /** Gets the color of the text in the value box. */
  Fl_Color textcolor() const {return input.textcolor();}
    /** Sets the color of the text in the value box. */
  void textcolor(unsigned n) {input.textcolor(n);}
    /** Get the color of the text cursor. */
  Fl_Color cursor_color() const {return input.cursor_color();}
    /** Set the color of the text cursor. The text cursor is black by default. */
  void cursor_color(unsigned n) {input.cursor_color(n);}

};

#endif

//
// End of "$Id: Fl_Value_Input.H,v 1.5.2.3.2.4.2.2 2003/11/22 01:04:39 matthiaswm Exp $".
//
