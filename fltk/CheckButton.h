//
// "$Id: CheckButton.h 4910 2006-04-06 19:26:22Z fabien $"
//

#ifndef fltk_CheckButton_h
#define fltk_CheckButton_h

#include <fltk3/CheckButton.h>

#include "Button.h"

namespace fltk {

class FL_API CheckButton : public Button {
protected:
  CheckButton() {}
public:
  CheckButton(int x,int y,int w,int h,const char *l = 0) {
    _p = new fltk3::CheckButton(x, y, w, h, l);
    _p->wrapper(this);
  }
  // fltk123: static NamedStyle* default_style;
  // fltk123: virtual void draw();
};

}

#endif

//
// End of "$Id: CheckButton.h 4910 2006-04-06 19:26:22Z fabien $".
//
