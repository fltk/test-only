//
// "$Id: Slider.h 5600 2007-01-13 00:04:55Z spitzak $"
//

#ifndef fltk2_Slider_h
#define fltk2_Slider_h

#include <fltk3/Slider.h>

#include "Valuator.h"

namespace fltk {

class FL_API Slider : public Valuator {
protected:
  Slider() {}
  
public:
  Slider(int x,int y,int w,int h, const char *l = 0) {
    _p = new fltk3::Slider(x, y, w, h, l);
    _p->wrapper(this);
  }

#if 0 // fltk123:
  enum { // bit flags for type():
    LINEAR		= 0,
    TICK_ABOVE		= 2,
    TICK_LEFT		= TICK_ABOVE,
    TICK_BELOW		= 4,
    TICK_RIGHT		= TICK_BELOW,
    TICK_BOTH		= TICK_ABOVE|TICK_BELOW,
    LOG			= 8
#ifdef FLTK_1_SLIDER
    // for back-compatability only
    ,VERTICAL		= 0,
    HORIZONTAL		= 1,
    FILL		= 16
#endif
  };
  bool horizontal() const {return !flag(LAYOUT_VERTICAL) || (type()&1);}
  bool log() const {return (type()&LOG)!=0;}

  void draw();
  int handle(int);

  static NamedStyle* default_style;

  unsigned short slider_size() const {return slider_size_;}
  void slider_size(int n) {slider_size_ = (unsigned short)n;}
  unsigned short tick_size() const {return tick_size_;}
  void tick_size(int n) {tick_size_ = (unsigned short)n;}
  
#ifdef FLTK_1_SLIDER
  // back comptability:
  Box* slider() const {return buttonbox();}
  void slider(Box* b) {buttonbox(b);}
  void slider_size(double v) {slider_size(int(v*w()));}
#endif

  //protected:

  int slider_position(double value, int w);
  double position_value(int x, int w);
  int handle(int event, const Rectangle&);
  void draw_ticks(const Rectangle&, int min_spacing);
  bool draw(const Rectangle&, Flags flags, bool slot);
#endif
};

}

#endif

//
// End of "$Id: Slider.h 5600 2007-01-13 00:04:55Z spitzak $".
//
