// fl_plugin.cxx

#include <FL/fl_load_plugin.H>
#include "config.h"

#ifndef _WIN32

#include <dlfcn.h>
typedef void* DLhandle;

#include <unistd.h>

#else

// simulate posix on windoze:
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

typedef int (*Main)(int, const char * const *);

bool fl_load_plugin(const char* name, const char* type) {
  char s[1024];

  // see if an absolute name was given:
  if (name[0] == '/' || name[0] == '.'
#ifdef WIN32
      || name[0] == '\\' || name[1]==':'
#endif
      ) goto TRYIT;

  // try a hidden file in the home directory:
#ifndef WIN32
  if (getuid() == geteuid()) {
#endif
    const char* h = getenv("HOME");
#ifdef WIN32
    if (!h) h = getenv("WINDIR");
#endif
    if (h) {
      snprintf(s, 1024, "%s/.%s/%s", h, type, name);
      if (access(s, F_OK) == 0) {name = s; goto TRYIT;}
    }
#ifndef WIN32
  }
#endif

  // try the normal location:
  snprintf(s, 1024, "/usr/local/lib/%s/%s", type, name);
  if (access(s, F_OK) == 0) {name = s; goto TRYIT;}

  // also try this:
  snprintf(s, 1024, "/lib/%s/%s", type, name);
  if (access(s, F_OK) == 0) {name = s; goto TRYIT;}

  return false;

 TRYIT:
  // now open it, any further errors will be printed:
  DLhandle handle = dlopen(name, RTLD_NOW);
  if (handle) {
    Main f = (Main)dlsym(handle, "main");
    if (f) {
      const char* argv[2]; argv[0] = name; argv[1] = 0;
      return !f(1, argv);
    }
  }
  fprintf(stderr, "%s\n", dlerror());
  if (handle) dlclose(handle);
  return false;

}

