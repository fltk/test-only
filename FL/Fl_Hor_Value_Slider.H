//
// "$Id$"
//

#ifndef Fltk1_Hor_Value_Slider_H
#define Fltk1_Hor_Value_Slider_H

#include <fltk3/HorValueSlider.h>

#include "Fl_Value_Slider.H"

class Fl_Hor_Value_Slider : public Fl_Value_Slider {
public:
  Fl_Hor_Value_Slider(int X,int Y,int W,int H,const char *l=0) {
    _p = new fltk3::HorValueSlider(X, Y, W, H, l);
    _p->wrapper(this);
  }
};

#endif

//
// End of "$Id$".
//
