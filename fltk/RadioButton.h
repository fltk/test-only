//
// "$Id: RadioButton.h 4886 2006-03-30 09:55:32Z fabien $"
//

#ifndef fltk2_RadioButton_h
#define fltk2_RadioButton_h

#include <fltk3/RadioButton.h>

#include "CheckButton.h"

namespace fltk {

class FL_API RadioButton : public CheckButton {
public:
  RadioButton(int x, int y, int w, int h, const char *l=0) {
    _p = new fltk3::RadioButton(x, y, w, h, l);
    _p->wrapper(this);
  }
  // fltk123: static NamedStyle* default_style;
};

}

#endif

//
// End of "$Id: RadioButton.h 4886 2006-03-30 09:55:32Z fabien $".
//
