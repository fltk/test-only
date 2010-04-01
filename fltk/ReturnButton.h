// "$Id: ReturnButton.h 4886 2006-03-30 09:55:32Z fabien $"
//

#ifndef fltk2_ReturnButton_h
#define fltk2_ReturnButton_h

#include <fltk3/ReturnButton.h>
#include "Button.h"

namespace fltk {

class FL_API ReturnButton : public Button {
public:
  ReturnButton(int x,int y,int w,int h,const char *label=0) {
    _p = new fltk3::ReturnButton(x, y, w, h, label); 
    _p->wrapper(this);
  }
  // fltk123: static NamedStyle* default_style;
protected:
  // fltk123: void draw();
};

}

#endif

// End of "$Id: ReturnButton.h 4886 2006-03-30 09:55:32Z fabien $".
