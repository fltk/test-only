//
// "$Id$"
//

#ifndef Fltk1_Input_H
#define Fltk1_Input_H

#include <fltk3/Input.h>

#include "Fl_Input_.H"

class FL_EXPORT Fl_Input : public Fl_Input_ {
protected:
  Fl_Input() {}
  // fltk123: void draw();
public:
  // fltk123: int handle(int);
  Fl_Input(int x, int y, int w, int h, const char *l = 0) {
    _p = new fltk3::Input(x, y, w, h, l);
    _p->wrapper(this);
  }
};

#endif 

//
// End of "$Id$".
//
