// fl_plugin.cxx

#include <FL/Fl.H>
#include <FL/fl_load_plugin.H>
#include <FL/conf.h>
#include <config.h>

#ifndef WIN32

#include <unistd.h>
#include <dlfcn.h>
typedef void* DLhandle;

#else

// simulate posix on windows:
#include <windows.h>
#include <winbase.h>
typedef HINSTANCE DLhandle;
#define dlopen(a,b) LoadLibrary(a)
#define RTLD_NOW 2
#define dlsym(handle, sym) GetProcAddress(handle, sym)

#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifndef PATH_MAX
#define PATH_MAX 128
#endif

// returns address of function() (can be used as handle)
void* fl_load_plugin(const char* n, const char* function) {
#ifndef WIN32
  // do not allow plugins if this executable is setuid
  if (getuid() != geteuid()) return 0;
#endif
  if (!n || !function) return 0;
  char name[PATH_MAX];
  if (!conf_is_path_rooted(n)) snprintf(name, sizeof(name), "./%s", n);
  else strncpy(name, n, sizeof(name));
  // open plugin, any errors will be printed
  DLhandle handle = dlopen(name, RTLD_NOW);
  if (!handle) {
    fprintf(stderr, "fl_load_plugin(): could not open file \"%s\".\n", name);
    return 0;
  }

  void* f = (void*)dlsym(handle, function);
  if (!f) {
    fprintf(stderr, "fl_load_plugin(): could not find function \"%s\"\n"
                    "fl_load_plugin(): in file \"%s\".\n", function, name);
    return 0;
  }
  return f;
}
