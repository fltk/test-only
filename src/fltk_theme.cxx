/* fltk_theme.cxx

   This function is in a file by itself so that a program that statically
   links with fltk can customize it's theme by linking one of the theme
   plugins. Because the theme plugins define a function called fltk_theme()
   they will override this and cause fltk to call the plugin instead.
*/

#include <fltk/Style.h>

extern "C" bool fltk_theme() {
  fltk::get_system_colors();
  return true;
}
