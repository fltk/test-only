// "$Id: Valuator.h 6944 2009-11-27 12:18:37Z cwarrens $"
//

#ifndef fltk2_Valuator_h
#define fltk2_Valuator_h

#include <fltk3/Valuator.h>

#include "Widget.h"

namespace fltk {

class FL_API Valuator : public Widget {
protected:
  Valuator() {}
  
public:
  Valuator(int X, int Y, int W, int H, const char* L) {
    _p = new fltk3::Valuator(X, Y, W, H, L);
    _p->wrapper(this);
  }
  double value() const { return ((fltk3::Valuator*)_p)->value(); }
  bool value(double v) { return ((fltk3::Valuator*)_p)->value(v); }

#if 0 // fltk123:
  double minimum() const {return minimum_;}
  void minimum(double a) {minimum_ = a;}

  double maximum() const {return maximum_;}
  void maximum(double a) {maximum_ = a;}
#endif
  
  void range(double a, double b) { ((fltk3::Valuator*)_p)->range(a, b); }

  double step() const { return ((fltk3::Valuator*)_p)->step(); }
  void step(double a) { ((fltk3::Valuator*)_p)->step(a); }

#if 0 // fltk123:
  double linesize() const;
  void linesize(double a) {linesize_ = a;}
  double linesize_setting() const {return linesize_;}

  virtual int format(char*);

  int handle(int);

#ifdef FLTK_1_SLIDER
  void step(double a, int b) {step(a/b);}
  void bounds(double a, double b) {minimum_=a; maximum_=b;}
  void precision(int p) {
    int B = 1;
    for (int i=0; i<p; i++) B *= 10;
    step_ = 1.0f/B;
  }
#endif

//protected:

  double previous_value() const {return previous_value_;}
  void handle_push() {previous_value_ = value_;}
  void handle_drag(double newvalue);
  void handle_release();

  virtual void value_damage(); // callback whenever value changes
  void set_value(double v) {value_ = v;} // change w/o doing value_damage
#endif
};

}

#endif

// End of "$Id: Valuator.h 6944 2009-11-27 12:18:37Z cwarrens $".
