//
// "$Id: Scrollbar.h 7981 2010-12-08 23:53:04Z greg.ercolano $"
//
// Scroll bar header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

/* \file
 fltk3::Scrollbar widget . */

#ifndef Fltk3_Scrollbar_H
#define Fltk3_Scrollbar_H

#include "Slider.h"

class Fl_Scrollbar;

namespace fltk3 {
  
  /**
   The fltk3::Scrollbar widget displays a slider with arrow buttons at
   the ends of the scrollbar. Clicking on the arrows move up/left and
   down/right by linesize(). Scrollbars also accept fltk3::SHORTCUT events:
   the arrows move by linesize(), and vertical scrollbars take Page
   Up/Down (they move by the page size minus linesize()) and Home/End
   (they jump to the top or bottom).
   
   Scrollbars have step(1) preset (they always return integers). If
   desired you can set the step() to non-integer values. You will then
   have to use casts to get at the floating-point versions of value()
   from fltk3::Slider.
   
   \image html  scrollbar.png 
   \image latex scrollbar.png "fltk3::Scrollbar" width=4cm
   */
  class FLTK3_EXPORT Scrollbar : public fltk3::Slider {
    
    friend class ::Fl_Scrollbar;
    
    int linesize_;
    int pushed_;
    static void timeout_cb(void*);
    void increment_cb();
  protected:
    void draw();
    
  public:
    
    Scrollbar(int X,int Y,int W,int H, const char *L = 0);
    ~Scrollbar();
    int handle(int);
    
    /**
     Gets the integer value (position) of the slider in the scrollbar.
     You can get the floating point value with fltk3::Slider::value().
     
     \see fltk3::Scrollbar::value(int p)
     \see fltk3::Scrollbar::value(int pos, int size, int first, int total)
     */
    int value() const {return int(Slider::value());}
    
    /**
     Sets the value (position) of the slider in the scrollbar.
     
     \see fltk3::Scrollbar::value()
     \see fltk3::Scrollbar::value(int pos, int size, int first, int total)
     */
    int value(int p) {return int(Slider::value((double)p));}
    
    /**
     Sets the position, size and range of the slider in the scrollbar.
     \param[in] pos   position, first line displayed
     \param[in] size  window size, number of lines displayed
     \param[in] first number of first line
     \param[in] total total number of lines
     
     You should call this every time your window changes size, your data
     changes size, or your scroll position changes (even if in response
     to a callback from this scrollbar).
     All necessary calls to redraw() are done.
     
     Calls fltk3::Slider::scrollvalue(int pos, int size, int first, int total).
     */
    int value(int pos, int size, int first, int total) {
      return scrollvalue(pos, size, first, total);
    }
    
    /**
     Get the size of step, in lines, that the arror keys move.
     */
    int linesize() const {return linesize_;}
    
    /**
     This number controls how big the steps are that the arrow keys do.
     In addition page up/down move by the size last sent to value()
     minus one linesize().  The default is 16.
     */
    void linesize(int i) {linesize_ = i;}
    
  };
  
}

#endif

//
// End of "$Id: Scrollbar.h 7981 2010-12-08 23:53:04Z greg.ercolano $".
//
