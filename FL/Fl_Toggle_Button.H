//
// "$Id$"
//

#ifndef Fltk1_Toggle_Button_H
#define Fltk1_Toggle_Button_H

#include <fltk3/ToggleButton.h>

#include "Fl_Button.H"

class Fl_Toggle_Button : public Fl_Button {
public:
  Fl_Toggle_Button(int X,int Y,int W,int H,const char *l=0) {
    _p = new fltk3::ToggleButton(X, Y, W, H, l);
    _p->wrapper(this);
  }
};

#endif

//
// End of "$Id$".
//
