// Plug-ins support for ELF and WIN32

#ifndef FL_Plugins_H
#define FL_Plugins_H

#ifdef _WIN32

#include <windows.h>
#include <winbase.h>

typedef HINSTANCE FLDLhandle;
#define FLDLopen(a) LoadLibrary(a)
#define FLDLclose(a) FreeLibrary(a)
#define FLDLsym(handle, sym) GetProcAddress(handle, sym)
#define FLDLerror() GetLastError()
#define FLDLext ".dll"

#ifdef FL_INPLUGIN
# define FLDLE __declspec( dllimport )
#elif defined(OUTPLUGIN)
# define FLDLE __declspec( dllexport )
#else
# define FLDLE
#endif

#ifdef FL_OUTPLUGIN
# define FLDLI __declspec( dllimport )
#elif defined(INPLUGIN)
# define FLDLI __declspec( dllexport )
#else
# define FLDLI
#endif

#else

#include <dlfcn.h>
typedef void* FLDLhandle;
#define FLDLopen(a) dlopen(a, RTLD_LAZY)
#define FLDLclose(a) dlclose(a)
#define FLDLsym(handle, sym) dlsym(handle, sym)
#define FLDLerror() dlerror()
#define FLDLext ".so"

#define FLDLE
#define FLDLI

#endif

#endif // FL_Plugins_H
