// fl_plugin.cxx

#include <FL/Fl.H>
#include <FL/fl_load_plugin.H>
#include "config.h"

#ifndef _WIN32

#include <dlfcn.h>
typedef void* DLhandle;

#include <unistd.h>

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
// returns -3 if couldn't find plugin_main()
// returns -4 if uid != euid (setuid program?)
// otherwise, returns result of func() (should be 0)
int fl_load_plugin(const char* name, const char* func) {
#ifndef WIN32
  if (getuid() != geteuid()) {
    fprintf(stderr, "fl_load_plugin(): Plugin loading disabled for setuid programs.\n");
    return -4;
  }
#endif
  // open plugin, any errors will be printed
  DLhandle handle = dlopen(name, RTLD_NOW);
  if (handle) {
    Function f = (Function)dlsym(handle, func);
    if (f) {
      const char* argv[2] = { name, 0 };
      return f(1, argv);
    }
  }
  fprintf(stderr, "fl_load_plugin(): %s\n", dlerror());
  if (handle) { dlclose(handle); return -3; }
  return -2;
}
