//
// "$Id: InvisibleBox.h 5698 2007-02-19 05:40:36Z spitzak $"
//
// This is a box that is invisible due to not having a box. The
// label still prints so it can be used to position labels. Also
// this is useful as a resizable() widget.

#ifndef fltk2_InvisibleBox_h
#define fltk2_InvisibleBox_h

#include <fltk3/InvisibleBox.h>

#include "Widget.h"

namespace fltk {

class FL_API InvisibleBox : public Widget {
public:
  InvisibleBox(int x, int y, int w, int h, const char *l=0) {
    _p = new fltk3::Widget(x, y, w, h, l);
    _p->box(fltk3::NO_BOX);
  }
  
  InvisibleBox(Box* b, int x, int y, int w, int h, const char *l) {
    _p = new fltk3::Widget(x, y, w, h, l);
    box(b);
  }
  
  /*
  static NamedStyle* default_style;
  int handle(int);
  void draw();
   */
};

}

#endif

//
// End of "$Id: InvisibleBox.h 5698 2007-02-19 05:40:36Z spitzak $".
//
