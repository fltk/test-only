//
// "$Id$"
//

#ifndef Fltk1_Value_Slider_H
#define Fltk1_Value_Slider_H

#include <fltk3/ValueSlider.h>

#include "Fl_Slider.H"

class FL_EXPORT Fl_Value_Slider : public Fl_Slider {
protected:
  Fl_Value_Slider() {}
  // fltk123: void draw();
public:
  // fltk123: int handle(int);
  Fl_Value_Slider(int x,int y,int w,int h, const char *l = 0) {
    _p = new fltk3::ValueSlider(x, y, w, h, l);
    _p->wrapper(this);
  }
  // fltk123: Fl_Font textfont() const {return textfont_;}
  // fltk123: void textfont(Fl_Font s) {textfont_ = s;}
  // fltk123: Fl_Fontsize textsize() const {return textsize_;}
  // fltk123: void textsize(Fl_Fontsize s) {textsize_ = s;}
  // fltk123: Fl_Color textcolor() const {return textcolor_;}
  // fltk123: void textcolor(Fl_Color s) {textcolor_ = s;}
};

#endif

//
// End of "$Id$".
//
