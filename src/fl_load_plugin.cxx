// fl_plugin.cxx

#include <FL/Fl.H>
#include <FL/fl_load_plugin.H>
#include <config.h>

#ifndef _WIN32

#include <dlfcn.h>
typedef void* DLhandle;

#else

// simulate posix on windows:
#include <windows.h>
#include <winbase.h>
typedef HINSTANCE DLhandle;
#define dlopen(a,b) LoadLibrary(a)
#define RTLD_LAZY 1
#define RTLD_NOW 2
#define dlclose(a) FreeLibrary(a)
#define dlsym(handle, sym) GetProcAddress(handle, sym)
#define dlerror() GetLastError()

#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef int (*Function)(int, const char * const *);

// returns -1 if plugin file not found
// returns -2 if plugin file couldn't be opened
// returns -3 if couldn't find func()
// otherwise, returns result of func() (should be 0)
int fl_load_plugin(const char* name, const char* function,
		   int argc, const char * const * argv) {
  // open plugin, any errors will be printed
  DLhandle handle = dlopen(name, RTLD_NOW);
  if (handle) {
    if (!function) return 0;
    Function f = (Function)dlsym(handle, function);
    if (f) return f(argc, argv);
  }
  fprintf(stderr, "fl_load_plugin(): %s\n", dlerror());
  if (handle) { dlclose(handle); return -3; }
  return -2;
}
