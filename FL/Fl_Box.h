//
// "$Id: Fl_Box.h,v 1.1 2002/12/09 04:49:44 spitzak Exp $"
//

// Back-compatability widget.
//
// In fltk2 the only difference between Fl_Box and Fl_Widget is that
// the box type defaults to FL_NO_BOX and there is a constructor that
// sets the box type. Also the drawing function does a special hack
// to do nothing if the widget is invisible, to avoid breaking old
// programs (they should hide() the widget instead!)

#ifndef Fl_Box_H
#define Fl_Box_H

#include <fltk/Widget.h>

class FL_API Fl_Box : public fltk::Widget {
public:
  Fl_Box(int x, int y, int w, int h, const char *l=0);
  Fl_Box(fltk::Box* b, int x, int y, int w, int h, const char *l);
  static fltk::NamedStyle* default_style;
  void draw();
};

#endif

//
// End of "$Id: Fl_Box.h,v 1.1 2002/12/09 04:49:44 spitzak Exp $".
//
