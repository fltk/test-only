/
// "$Id$
/
// Counter header file for the Fast Light Tool Kit (FLTK)
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

// A numerical value with up/down step buttons.  From Forms

#ifndef Fl_Counter_
#define Fl_Counter_

#ifndef Fl_Valuator_
#include "Fl_Valuator.H
#endi

// values for type()
#define FL_NORMAL_COUNTER	
#define FL_SIMPLE_COUNTER	

/** The Fl_Counter widget is provided for forms compatibility. It control
 * a single floating point value.
 *   \image html counter.gi
 * Fl_Counter supports the following type()'s
 * \arg \c FL_NORMAL_COUNTER - Displays a counter with 4 arrow buttons
 * \arg \c FL_SIMPLE_COUNTER - Displays a counter with only 2 arrow buttons
 *
class FL_EXPORT Fl_Counter : public Fl_Valuator 

  uchar textfont_, textsize_
  unsigned textcolor_
  double lstep_
  uchar mouseobj
  static void repeat_callback(void *)
  int calc_mouseobj()
  void increment_cb()

protected

  void draw()

public

  int handle(int)
    /** Creates a new Fl_Counter widget using the given position, size,
     * and label string. The default type is \c FL_NORMAL_COUNTER. *
  Fl_Counter(int x,int y,int width,int height,const char *label = 0)
    /** Destroys the valuator. *
  ~Fl_Counter()
    /** Set the increment for the double-arrow buttons. The default value is 1.0. *
  void lstep(double a) {lstep_ = a;
  void step(double a,double b) {Fl_Valuator::step(a); lstep_ = b;
  void step(double a) {Fl_Valuator::step(a);
  Fl_Font textfont() const {return (Fl_Font)textfont_;
  void textfont(uchar s) {textfont_ = s;
  uchar textsize() const {return textsize_;
  void textsize(uchar s) {textsize_ = s;
  Fl_Color textcolor() const {return (Fl_Color)textcolor_;
  void textcolor(unsigned s) {textcolor_ = s;

}

#endi

/
// End of "$Id$"
/
