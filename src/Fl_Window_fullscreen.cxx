//
// "$Id: Fl_Window_fullscreen.cxx,v 1.23 2004/07/24 12:18:59 laza2000 Exp $"
//
// Fullscreen window support for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <config.h>
#include <fltk/Window.h>
#include <fltk/Monitor.h>
#include <fltk/events.h>
#include <fltk/x.h>
using namespace fltk;

#ifdef _WIN32_WCE
// Windows CE taskbar
# define TASKBAR_CLASS TEXT("HHTaskBar")
#elif _WIN32
// Desktop windows taskbar
# define TASKBAR_CLASS TEXT("Shell_TrayWnd")
#endif

#if 0
#ifdef _WIN32
#elif (defined(__APPLE__) && !USE_X11)
#else
// Neither the X or Win32 version will successfully hide the taskbar.
// I would like it, but maybe that should be a third state of the window.
// On both systems it looks like it is much harder to do it.

// Supposedly this tells the new X window managers to put this atop 
// the taskbar. In our experiments there is no indication that this
// produces any different behavior.

// Right now KDE is broken and always puts the taskbar atop the window.
// Gnome appears to recognize the attempt to make the window fullscreen
// and puts the taskbar below it (this is good behavior).

// Last word on Windows is that you must use DirectX (!) to do this.
// Gah!
static void fsonoff(XWindow xwindow, bool onoff) {
  static Atom _NET_WM_STATE;
  static Atom _NET_WM_STATE_REMOVE;
  static Atom _NET_WM_STATE_ADD;
  static Atom _NET_WM_STATE_FULLSCREEN;
  if (!_NET_WM_STATE) {
#define MAX_ATOMS 30
    Atom* atom_ptr[MAX_ATOMS];
    char* names[MAX_ATOMS];
    int i = 0;
#define atom(a,b) atom_ptr[i] = &a; names[i] = b; i++
    atom(_NET_WM_STATE		, "_NET_WM_STATE");
    atom(_NET_WM_STATE_REMOVE	, "_NET_WM_STATE_REMOVE");
    atom(_NET_WM_STATE_ADD	, "_NET_WM_STATE_ADD");
    atom(_NET_WM_STATE_FULLSCREEN, "_NET_WM_STATE_FULLSCREEN");
#undef atom
    Atom atoms[MAX_ATOMS];
    XInternAtoms(xdisplay, names, i, 0, atoms);
    for (; i--;) *atom_ptr[i] = atoms[i];
  }
  XEvent e;
  e.xany.type = ClientMessage;
  e.xany.window = xwindow;
  e.xclient.message_type = _NET_WM_STATE;
  e.xclient.format = 32;
  e.xclient.data.l[0] = onoff ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
  e.xclient.data.l[1] = (long)_NET_WM_STATE_FULLSCREEN;
  e.xclient.data.l[2] = (long)0;
  e.xclient.data.l[3] = (long)0;
  e.xclient.data.l[4] = (long)0;
  XSendEvent(xdisplay, RootWindow(xdisplay, xscreen), 0,
	     SubstructureNotifyMask|SubstructureRedirectMask, &e);
}
#endif
#endif

/*! Makes the window completely fill the Monitor, without any window
  manager border or taskbar or anything else visible.

  You must use fullscreen_off() to undo this. resize() may appear to
  work but it may not turn off the necessary flags!

  Many older X window managers will not work and insist the border be
  visible.  You may be able to avoid this bug by calling destroy()
  first, then fullscreen(), and then show(). You will probably also
  have to do the same thing when calling fullscreen_off().

  Older versions of KDE, Gnome, and Windows would not hide the
  taskbar. It appears newer versions recognize attempts to make
  full-screen windows and do hide the taskbar. (I tried the
  _NET_WM_STATE_FULLSCREEN that is supposed to hide the taskbar on
  older KDE and Gnome, but saw no sign that it worked. On older
  Windows it is apparently impossible to hide the taskbar without
  using DirectX, which would require all drawing code to change)
*/
void Window::fullscreen() {
  const Monitor& monitor = Monitor::find(x()+w()/2, y()+h()/2);
#ifdef _WIN32
  // Disable caption & borders
  LONG flags = GetWindowLong(xid(this), GWL_STYLE);
  flags &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);
  SetWindowLong(xid(this), GWL_STYLE, flags);

# if 1
  // Find taskbar and hide it
  HWND taskbar = FindWindow(TASKBAR_CLASS, NULL);
  if (taskbar) ShowWindow(taskbar, SW_HIDE);
# else
  // Make window topmost, so it goes top of taskbar
  // This will still keep modal and child_of windows top of this.
  SetWindowPos(xid(this), HWND_TOPMOST, 0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
# endif
  clear_border(); // Necessary to make CreatedWindow::borders return dx,dy,dw,dh 0
  resize(monitor.x(), monitor.y(), monitor.w(), monitor.h());

  // Still missing: we need to tell Windows that this window can go atop
  // the taskbar.
#elif (defined(__APPLE__) && !USE_X11)
  resize(monitor.x(), monitor.y(), monitor.w(), monitor.h());
#else
  // Most X window managers will not place the window where we want it unless
  // the border is turned off. And most (all except Irix 4DWM, as far as I
  // can tell) will ignore attempts to change the border unless the window
  // is unmapped. Telling the window manager about the border changing
  // is done by i->sendxjunk(). Order is somewhat peculiar, we need to
  // have the window mapped before calling resize or some window managers
  // (KDE) ignore the positioning information:
  clear_border();
  if (shown()) i->sendxjunk();
  resize(monitor.x(), monitor.y(), monitor.w(), monitor.h());
  //if (shown()) fsonoff(i->xid, true);
#endif
}

/*! Turns off any side effects of fullscreen() and does resize(x,y,w,h). */
void Window::fullscreen_off(int X,int Y,int W,int H) {
  // This function must exactly undo whatever fullscreen() did
#ifdef _WIN32
  // Restore window flags
  LONG flags = GetWindowLong(xid(this), GWL_STYLE);
  flags |= (WS_BORDER | WS_CAPTION | WS_THICKFRAME);
  SetWindowLong(xid(this), GWL_STYLE, flags);

# if 1
  // Find taskbar and show it again
  HWND taskbar = FindWindow(TASKBAR_CLASS, NULL);
  if (taskbar) ShowWindow(taskbar, SW_RESTORE);
# else
  // We are not topmost anymore
  SetWindowPos(xid(this), HWND_NOTOPMOST, 0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
# endif

  clear_flag(Window::NOBORDER); //Borders are back
  resize(X, Y, W, H);
#elif (defined(__APPLE__) && !USE_X11)
  resize(X, Y, W, H);
#else
  clear_flag(Window::NOBORDER);
  //if (shown()) fsonoff(i->xid, false);
  if (shown()) i->sendxjunk();
  resize(X, Y, W, H);
#endif
}

/*! void Window::clear_border()
  You may turn off the window manager border before calling show() on
  the window the first time. On most window managers this means the
  user cannot move, iconize, or resize the window (unless your program
   does it).
*/

/*! bool Window::border() const
  Returns false if clear_border() has been called. */

/*! bool Window::set_override()

  Windows with this property set will use the exact position and size
  set by the programmer (will not be handled by the window manager)
  and will not have an entry in the task list. This will also clear
  the window's border like clear_border() above. This is used by the
  fltk menus and tooltips.

  On X this causes "override redirect". This is only good for
  short-lived windows as it can confuse X window managers, however
  this is the only reliable and fast way to do it. This also turns on
  "save under" which on many X servers (like XFree86) can make the
  window disappear much faster by having the server rememeber what was
  behind it.
*/

/*! bool Window::override() const
  Returns true if set_override() has been called. */

//
// End of "$Id: Fl_Window_fullscreen.cxx,v 1.23 2004/07/24 12:18:59 laza2000 Exp $".
//
