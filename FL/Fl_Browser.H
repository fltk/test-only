#ifndef Fl_Browser_H
#define Fl_Browser_H

#include "Fl_Menu_.H"
#include <fltk/Browser.h>
typedef fltk::Browser Fl_Browser;

enum {
  FL_NORMAL_BROWSER	= fltk::Browser::NORMAL,
  FL_SELECT_BROWSER	= fltk::Browser::NORMAL,
  FL_HOLD_BROWSER	= fltk::Browser::NORMAL,
  FL_MULTI_BROWSER	= fltk::Browser::MULTI
};

#endif
