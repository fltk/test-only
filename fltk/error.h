/* Functions to report errors and possibly kill the program */

#ifndef fltk_error_h
#define fltk_error_h

#include "FL_API.h"

namespace fltk {

extern FL_API void (*warning)(const char*, ...);
extern FL_API void (*error)(const char*, ...);
extern FL_API void (*fatal)(const char*, ...);

}
#endif
