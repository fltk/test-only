//
// "$Id$"
//

#ifndef Fltk1_Light_Button_H
#define Fltk1_Light_Button_H

#include <fltk3/LightButton.h>

#include "Fl_Button.H"

class FL_EXPORT Fl_Light_Button : public Fl_Button {
protected:
  Fl_Light_Button() {}
  // fltk123: virtual void draw();
public:
  // fltk123: virtual int handle(int);
  Fl_Light_Button(int x,int y,int w,int h,const char *l = 0) {
    _p = new fltk3::LightButton(x, y, w, h, l);
    _p->wrapper(this);
  }
};

#endif

//
// End of "$Id$".
//
