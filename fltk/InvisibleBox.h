//
// "$Id: InvisibleBox.h,v 1.1 2002/12/15 10:42:49 spitzak Exp $"
//
// This is a box that is invisible due to not having a box. The
// label still prints so it can be used to position labels. Also
// this is useful as a resizable() widget.

#ifndef fltk_InvisibleBox_h
#define fltk_InvisibleBox_h

#include <fltk/Widget.h>

namespace fltk {

class FL_API InvisibleBox : public fltk::Widget {
public:
  InvisibleBox(int x, int y, int w, int h, const char *l=0);
  InvisibleBox(fltk::Box* b, int x, int y, int w, int h, const char *l);
  static fltk::NamedStyle* default_style;
  void draw();
};

}

#endif

//
// End of "$Id: InvisibleBox.h,v 1.1 2002/12/15 10:42:49 spitzak Exp $".
//
