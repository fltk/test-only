//
// "$Id: Fl_Plugins.H,v 1.5 1999/09/25 22:27:05 vincent Exp $"
//
// Plug-ins support for ELF and WIN32 for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@easysw.com".
// 

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
#define FLDLopen(a) dlopen(a, RTLD_NOW)
#define FLDLclose(a) dlclose(a)
#define FLDLsym(handle, sym) dlsym(handle, sym)
#define FLDLerror() dlerror()
#define FLDLext ".so"

#define FLDLE
#define FLDLI

#endif

#endif // FL_Plugins_H

//
// End of "$Id: Fl_Plugins.H,v 1.5 1999/09/25 22:27:05 vincent Exp $"
//
