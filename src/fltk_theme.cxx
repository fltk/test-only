// fltk_theme.cxx

#include <fltk/Style.h>

/*! This is the default value of fltk::theme(). It calls
  fltk::system_theme().

  If fltk is statically linked with a program then you can "theme"
  the program by simply linking in an object file that defines a
  different fltk_theme() function.
*/

extern "C" bool fltk_theme() {
  fltk::reset_theme();
  return fltk::system_theme();
}
