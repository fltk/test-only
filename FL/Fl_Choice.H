//
// "$Id$"
//

#ifndef Fltk1_Choice_H
#define Fltk1_Choice_H

#include <fltk3/Choice.h>

#include "Fl_Menu_.H"

class FL_EXPORT Fl_Choice : public Fl_Menu_ {
protected:
  Fl_Choice() {}
  // fltk123: void draw();
public:
  Fl_Choice(int X, int Y, int W, int H, const char *L = 0) {
    _p = new fltk3::Choice(X, Y, W, H, L);
    _p->wrapper(this);
  }
  // fltk123: int handle(int);
  int value() const { return ((fltk3::Choice*)_p)->value(); }
  int value(int v) { return ((fltk3::Choice*)_p)->value(v); }
  int value(const Fl_Menu_Item* v) { return ((fltk3::Choice*)_p)->value((fltk3::MenuItem*)v); }
};

#endif

//
// End of "$Id$".
//
