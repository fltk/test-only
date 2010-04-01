//
// "$Id$"
//

#ifndef Fltk1_Valuator_H
#define Fltk1_Valuator_H

#include <fltk3/Valuator.h>

#include "Fl_Widget.H"

#define FL_VERTICAL		0
#define FL_HORIZONTAL		1

class FL_EXPORT Fl_Valuator : public Fl_Widget {

protected:
  Fl_Valuator() {}
  Fl_Valuator(int X, int Y, int W, int H, const char* L);
#if 0 // fltk123:
  int horizontal() const {return type()& FL_HORIZONTAL;}
  double previous_value() const {return previous_value_;}
  void handle_push() {previous_value_ = value_;}
  double softclamp(double);
  void handle_drag(double newvalue);
  void handle_release();
  virtual void value_damage();
  void set_value(double v) {value_ = v;}
#endif
  
public:
  void bounds(double a, double b) { ((fltk3::Valuator*)_p)->bounds(a, b); }
#if 0 // fltk123:
    double minimum() const {return min;}
    void minimum(double a) {min = a;}
    double maximum() const {return max;}
    void maximum(double a) {max = a;}
  void range(double a, double b) {min = a; max = b;}
#endif
  void step(int a) { ((fltk3::Valuator*)_p)->step(a); }
  void step(double a, int b) { ((fltk3::Valuator*)_p)->step(a, b); }
  void step(double s) { ((fltk3::Valuator*)_p)->step(s); }
  double step() const { return ((fltk3::Valuator*)_p)->step(); }
#if 0 // fltk123:
  void precision(int);
#endif
  double value() const { return ((fltk3::Valuator*)_p)->value(); }
  int value(double v) { return ((fltk3::Valuator*)_p)->value(v); }
#if 0 // fltk123:
  virtual int format(char*);
  double round(double); // round to nearest multiple of step
  double clamp(double); // keep in range
  double increment(double, int); // add n*step to value
#endif
};

#endif

//
// End of "$Id$".
//
