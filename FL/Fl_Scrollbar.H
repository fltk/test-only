/
// "$Id$
/
// Scroll bar header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Scrollbar_
#define Fl_Scrollbar_

#include "Fl_Slider.H

/** The Fl_Scrollbar widget displays a slider with arrow buttons at the end
 * of the scrollbar. Clicking on the arrows move up/left and down/right b
 * linesize(). Scrollbars also accept FL_SHORTCUT events: the arrows mov
 * by linesize(), and vertical scrollbars take Page Up/Down (they move b
 * the page size minus linesize()) and Home/End (they jump to the top o
 * bottom)
 *
 * Scrollbars have step(1) preset (they always return integers). If desire
 * you can set the step() to non-integer values. You will then have to us
 * casts to get at the floating-point versions of value() from Fl_Slider
 *   \image html scrollbar.gi
 *
class FL_EXPORT Fl_Scrollbar : public Fl_Slider 

  int linesize_
  int pushed_
  static void timeout_cb(void*)
  void increment_cb()
protected
  void draw()

public

    /** Creates a new Fl_Scrollbar widget using the given position, size,
     * and label string. You need to do <tt>type(FL_HORIZONTAL)</tt> if you
     * want a horizontal scrollbar. *
  Fl_Scrollbar(int x,int y,int w,int h, const char *l = 0)
  int handle(int)

  int value() {return int(Fl_Slider::value());
    /** The first form returns the integer value of the scrollbar. You ca
     * get the floating point value with Fl_Slider::value(). The second for
     * sets value(), range(), and slider_size() to make a variable-sized
     * scrollbar. You should call this every time your window changes size
     * your data changes size, or your scroll position changes (even if i
     * response to a callback from this scrollbar). All necessary calls t
     * redraw() are done. *
  int value(int p, int s, int top, int total) 
    return scrollvalue(p, s, top, total)
  
  int linesize() const {return linesize_;
    /** This number controls how big the steps are that the arrow keys do
     * In addition page up/down move by the size last sent to value() minu
     * one linesize(). The default is 16. *
  void linesize(int i) {linesize_ = i;

}

#endi

/
// End of "$Id$"
/
