/* fl_plugin.cxx
 *
 * "$Id: fl_load_plugin.cxx,v 1.17 2002/02/10 22:57:49 spitzak Exp $"
 *
 * This is a wrapper to make it simple to load plugins on various
 * systems. fl_load_plugin(file, symbol) will load the file as a
 * plugin and then return a pointer to "symbol" in that file.
 *
 * If symbol is null it will return a non-zero value if the plugin
 * loads but you cannot use this value for anything.
 *
 * If there is any problem (file not found, does not load as a plugin,
 * the symbol is not found) it will return null if there is any problem
 * and print debugging info on stderr.
 *
 */

#include <fltk/fl_load_plugin.h>
#include <stdio.h>
#include <config.h>

#if defined(_WIN32)

# include <windows.h>
# include <winbase.h>

void* fl_load_plugin(const char* name, const char* symbol) {
  HINSTANCE handle = LoadLibrary(name);
  if (handle) {
    if (!symbol) return (void*)handle;
    void* f = (void*)GetProcAddress(handle, symbol);
    if (f) return f;
    fprintf(stderr, "%s: function %s missing\n", name, symbol);
    return 0;
  }
  // anybody know where more informative error information is stored?
  fprintf(stderr, "%s: error loading plugin\n", name);
  return 0;
}

#else
#if HAVE_DLOPEN

#include <errno.h>
# include <unistd.h>
# include <dlfcn.h>

void* fl_load_plugin(const char* name, const char* symbol) {
  // do not allow plugins if this executable is setuid
  if (getuid() != geteuid())  {
    fprintf(stderr, "%s: plugins disabled in setuid programs\n", name);
    return 0;
  }
  void* handle = dlopen(name, RTLD_NOW);
  if (handle) {
    if (!symbol) return handle;
    void* f = dlsym(handle, symbol);
    if (f) return f;
  }
  fprintf(stderr, "%s\n", dlerror());
  return 0;
}

#else

void* fl_load_plugin(const char* name, const char*) {
  fprintf(stderr, "%s: loading of plugins not supported\n", name);
  return 0;
}

#endif
#endif

//
// End of "$Id: fl_load_plugin.cxx,v 1.17 2002/02/10 22:57:49 spitzak Exp $"
//
