/* fl_plugin.cxx
 *
 * "$Id: fl_load_plugin.cxx,v 1.15 2001/07/24 16:25:08 clip Exp $"
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

#if HAVE_DLOPEN || defined(WIN32)

#include <errno.h>

#ifndef WIN32

# include <unistd.h>
# include <dlfcn.h>
typedef void* DLhandle;

#else

// simulate posix on windows:
# include <windows.h>
# include <winbase.h>
typedef HINSTANCE DLhandle;
# define dlopen(a,b) LoadLibrary(a)
# define RTLD_NOW 2
# define dlsym(handle, sym) GetProcAddress(handle, sym)

#endif

void* fl_load_plugin(const char* name, const char* symbol) {
#ifndef WIN32
  // do not allow plugins if this executable is setuid
  if (getuid() != geteuid()) return 0;
#endif
  if (!name) return 0;

  DLhandle handle = dlopen(name, RTLD_NOW);
  if (!handle) {
#ifndef WIN32
    fprintf(stderr, "%s\n", dlerror());
#else
    fprintf(stderr, "%s: error loading plugin\n", name);
#endif
    return 0;
  }

  if (!symbol) return (void*)handle;

  void* f = (void*)dlsym(handle, symbol);
  if (!f) {
#ifndef WIN32
    fprintf(stderr, "%s\n", dlerror());
#else
    fprintf(stderr, "%s: function %s missing\n", name, symbol);
#endif
    return 0;
  }
  return f;
}

#else

void* fl_load_plugin(const char* name, const char*) {
  fprintf(stderr, "%s: loading of plugins not supported\n", name);
  return 0;
}

#endif

//
// End of "$Id: fl_load_plugin.cxx,v 1.15 2001/07/24 16:25:08 clip Exp $"
//
