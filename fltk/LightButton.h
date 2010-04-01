//
// "$Id: LightButton.h 4886 2006-03-30 09:55:32Z fabien $"
//

#ifndef fltk2_Light_Button_h
#define fltk2_Light_Button_h

#include <fltk3/CheckButton.h>

#include "CheckButton.h"

namespace fltk {

class FL_API LightButton : public CheckButton {
protected:
  LightButton() {}
public:
  LightButton(int x,int y,int w,int h,const char *l = 0) {
    _p = new fltk3::LightButton(x, y, w, h, l);
    _p->wrapper(this);
  }
  // fltk123: static NamedStyle* default_style;
  // fltk123: static void default_glyph(const Widget*, int, int,int,int,int, Flags);
};

}
#endif

//
// End of "$Id: LightButton.h 4886 2006-03-30 09:55:32Z fabien $".
//
