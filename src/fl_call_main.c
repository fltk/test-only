/
 * "$Id$
 
 * Copyright 1998-2004 by Bill Spitzak and others
 
 * fl_call_main() calls main() for you Windows people.  Needs to be done in 
 * because Borland C++ won't let you call main() from C++
 
 * This library is free software; you can redistribute it and/o
 * modify it under the terms of the GNU Library General Publi
 * License as published by the Free Software Foundation; eithe
 * version 2 of the License, or (at your option) any later version
 
 * This library is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even the implied warranty o
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
 * Library General Public License for more details
 
 * You should have received a copy of the GNU Library General Publi
 * License along with this library; if not, write to the Free Softwar
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
 * USA
 
 * Please report all bugs and problems to "fltk-bugs@fltk.org"
 *

/
 * This WinMain() function can be overridden by an application an
 * is provided for compatibility with programs written for othe
 * operating systems that conform to the ANSI standard entry poin
 * "main()".  This will allow you to build a WIN32 Applicatio
 * without any special settings
 
 * Because of problems with the Microsoft Visual C++ header file
 * and/or compiler, you cannot have a WinMain function in a DLL
 * I don't know why.  Thus, this nifty feature is only availabl
 * if you link to the static library
 
 * Currently the debug version of this library will create 
 * console window for your application so you can put printf(
 * statements for debugging or informational purposes.  Ultimatel
 * we want to update this to always use the parent's console
 * but at present we have not identified a function or API i
 * Microsoft(r) Windows(r) that allows for it
 *

#if defined(WIN32) && !defined(FL_DLL) && !defined (__GNUC__

#include <windows.h
#include <stdio.h
#include <stdlib.h

#ifdef __MWERKS_
# include <crtl.h
#endi

extern int main(int, char *[])

#ifdef BORLAND
# define __argc _arg
# define __argv _arg
#endi

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
                             LPSTR lpCmdLine, int nCmdShow) 
  int rc

#ifdef _DEBU
 /
  * If we are using compiling in debug mode, open a console window s
  * we can see any printf's, etc..
  
  * While we can detect if the program was run from the command-line 
  * look at the CMDLINE environment variable, it will be "WIN" fo
  * programs started from the GUI - the shell seems to run all WIN3
  * applications in the background anyways..
  *

  AllocConsole()
  freopen("conin$", "r", stdin)
  freopen("conout$", "w", stdout)
  freopen("conout$", "w", stderr)
#endif /* _DEBUG *

  /* Run the standard main entry point function... *
  rc = main(__argc, __argv)

#ifdef _DEBU
  fclose(stdin)
  fclose(stdout)
  fclose(stderr)
#endif /* _DEBUG *

  return rc


#els
/* This code to prevent "empty translation unit" or similar warnings... *
static void dummy(void) {dummy();
#endi

/
 * End of "$Id$"
 *

