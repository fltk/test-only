// Only Win32 and Linux are currently supported

#ifndef FL_DLL_H
#define FL_DLL_H

#ifdef WIN32

#include <windows.h>
#include <winbase.h>

typedef HINSTANCE FLDLhandle;
#define FLDLopen(a) LoadLibrary(a)
#define FLDLclose(a) FreeLibrary(a)
#define FLDLsym(handle, sym) GetProcAddress(handle, sym)
#define FLDLerror() GetLastError()
#define FLDLext ".dll"

#ifdef FL_INDLL
#define FLDLE __declspec( dllimport )
#elif defined(OUTDLL)
#define FLDLE __declspec( dllexport )
#else
#define FLDLE
#endif


#ifdef FL_OUTDLL
#define FLDLI __declspec( dllimport )
#elif defined(INDLL)
#define FLDLI __declspec( dllexport )
#else
#define FLDLI
#endif


#elif defined(Linux)

#include <dlfcn.h>
typedef void* FLDLhandle;
#define FLDLopen(a) dlopen(a, RTLD_LAZY)
#define FLDLclose(a) dlclose(a)
#define FLDLsym(handle, sym) dlsym(handle, sym)
#define FLDLerror() dlerror()
#define FLDLext ".so"

#define FLDLE
#define FLDLI



#else
#define FL_NO_DLL
#endif

#endif FL_DLL_H
