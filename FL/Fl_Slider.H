//
// "$Id$"
//

#ifndef Fltk1_Slider_H
#define Fltk1_Slider_H

#include <fltk3/Slider.h>

#include "Fl_Valuator.H"

#define FL_VERT_SLIDER		0
#define FL_HOR_SLIDER		1
#define FL_VERT_FILL_SLIDER	2
#define FL_HOR_FILL_SLIDER	3
#define FL_VERT_NICE_SLIDER	4
#define FL_HOR_NICE_SLIDER	5

class FL_EXPORT Fl_Slider : public Fl_Valuator {

protected:
  Fl_Slider() {}
#if 0 // fltk123:
  void draw(int, int, int, int);
  int handle(int, int, int, int, int);
  void draw();
#endif

public:
  Fl_Slider(int X,int Y,int W,int H, const char *L = 0) {
    _p = new fltk3::Slider(X, Y, W, H, L);
    _p->wrapper(this);
  }
  Fl_Slider(uchar t,int X,int Y,int W,int H, const char *L) {
    _p = new fltk3::Slider(t, X, Y, W, H, L);
    _p->wrapper(this);
  }
#if 0 // fltk123:
  int handle(int);
  int scrollvalue(int pos,int size,int first,int total);
  void bounds(double a, double b);
  float slider_size() const {return slider_size_;}
  void slider_size(double v);
  Fl_Boxtype slider() const {return (Fl_Boxtype)slider_;}
  void slider(Fl_Boxtype c) {slider_ = c;}
#endif
};

#endif

//
// End of "$Id$".
//
