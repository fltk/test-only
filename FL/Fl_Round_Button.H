//
// "$Id$"
//

#ifndef Fltk1_Round_Button_H
#define Fltk1_Round_Button_H

#include <fltk3/RoundButton.h>

#include "Fl_Light_Button.H"

class FL_EXPORT Fl_Round_Button : public Fl_Light_Button {
protected:
  Fl_Round_Button() {}
public:
  Fl_Round_Button(int x,int y,int w,int h,const char *l = 0) {
    _p = new fltk3::RoundButton(x, y, w, h, l);
    _p->wrapper(this);
  }
};

#endif

//
// End of "$Id$".
//
