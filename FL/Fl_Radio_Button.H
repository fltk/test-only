//
// "$Id$"
//

/* \file
   Fl_Radio_Button widget . */

#ifndef Fltk1_Radio_Button_H
#define Fltk1_Radio_Button_H

#include <fltk3/RadioButton.h>

#include "Fl_Button.H"

class Fl_Radio_Button : public Fl_Button {
protected:
  Fl_Radio_Button() {}
public:
  Fl_Radio_Button(int x,int y,int w,int h,const char *l=0) {
    _p = new fltk3::RadioButton(x, y, w, h, l);
    _p->wraper(this);
  }
};

#endif

//
// End of "$Id$".
//
