#ifndef Fl_Style_Set_h
#define Fl_Style_Set_h

#include <fltk/Fl_Style.h>

class FL_API Fl_Style_Set {
  Fl_Named_Style* first_style;
  Fl_Theme theme;
  const char* scheme;
  Fl_Color background;
  void* reserved; // pointer to extra saved stuff
public:
  Fl_Style_Set();
  void make_current();
  ~Fl_Style_Set();
};

#endif
