// fltk_theme.cxx

#include <config.h>

/*! This is the default value of fltk::theme().

  This is declared as a "C" function to allow it to be replaced with
  a function from a loaded plugin. If you are statically linking fltk
  you can also override this by declaring a fltk_theme() function in
  your own code (some Unix systems, but not Linux, will do this even
  if fltk is a shared library).

  <b>Unix/Linux/X11:</b>

  This attempts to read information from the KDE files
  "~/.kde/share/config/kdeglobals" and "/usr/share/config/kdeglobals".
  If these exist it also installs an event handler to listen for KDE
  style-change events and call fltk::reload_theme() on them. Fltk does
  not even bother trying to look at the xrdb databases, nobody seems
  to use that any more.

  <b>Windows:</b>

  On Windows it will read colors and fonts using GetSysColor() and
  SystemParametersInfo(). Unlike X, Windows FLTK always calls
  fltk::reload_theme() when a WM_SYSCOLORCHANGE, WM_DISPLAYCHANGE, or
  WM_SETTINGCHANGE event happens, even if you replace fltk_theme(), so
  your replacement should be prepared for this.

*/
extern "C" bool fltk_theme();

#ifdef USE_X11

// Maybe _WIN32 should use the Windows version anyway? It would work!
# include "fltk_theme_x.cxx"

#elif defined(_WIN32)

# include "fltk_theme_win32.cxx"

//#elif defined(__APPLE__)

#else

extern "C" bool fltk_theme() {}

#endif
