//
// "$Id$"
//

#ifndef Fl_Check_Button_H
#define Fl_Check_Button_H

#include <fltk3/CheckButton.h>

#include "Fl_Light_Button.H"

class FL_EXPORT Fl_Check_Button : public Fl_Light_Button {
public:
  Fl_Check_Button(int X, int Y, int W, int H, const char *L = 0) {
    _p = new fltk3::CheckButton(X, Y, W, H, L);
    _p->wrapper(this);
  }
};

#endif

//
// End of "$Id$".
//
