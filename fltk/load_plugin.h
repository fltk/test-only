// This is a convienence function for portable loading of a plugin
// and possibly returing a symbol in that plugin.

#ifndef fl_load_plugin_h
#define fl_load_plugin_h

#include "FL_API.h"

/*! \addtogroup utilities */
FL_API void* load_plugin(const char* name, const char* symbol);

#endif
