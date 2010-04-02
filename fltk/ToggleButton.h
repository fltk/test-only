//
// "$Id: ToggleButton.h 4886 2006-03-30 09:55:32Z fabien $"
//

#ifndef fltk2_ToggleButton_h
#define fltk2_ToggleButton_h

#include <fltk3/ToggleButton.h>

#include "Button.h"

namespace fltk {

class ToggleButton : public Button {
protected:
  ToggleButton() {}
public:
  ToggleButton(int x,int y,int w,int h,const char *l=0) {
    _p = new fltk3::ToggleButton(x, y, w, h, l);
    _p->wrapper(this);
  }
};

}
#endif

//
// End of "$Id: ToggleButton.h 4886 2006-03-30 09:55:32Z fabien $".
//
