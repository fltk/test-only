//
// "$Id: RepeatButton.h 4886 2006-03-30 09:55:32Z fabien $"
//

#ifndef fltk2_RepeatButton_h
#define fltk2_RepeatButton_h

#include <fltk3/RepeatButton.h>

#include "Button.h"

namespace fltk {

class FL_API RepeatButton : public Button {
public:
  // fltk123: int handle(int);
  RepeatButton(int x,int y,int w,int h,const char *l=0) {
    _p = new fltk3::RepeatButton(x, y, w, h, l);
    _p->wrapper(this);
  }
};

}

#endif

//
// End of "$Id: RepeatButton.h 4886 2006-03-30 09:55:32Z fabien $".
//
