//
// "$Id: Fl_x.cxx,v 1.186 2004/07/15 16:23:20 spitzak Exp $"
//
// X specific code for the Fast Light Tool Kit (FLTK).
// This file is #included by Fl.cxx
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

#define CONSOLIDATE_MOTION 1 // this was 1 in fltk 1.0

#define DEBUG_SELECTIONS 0

#include <config.h>
#include <fltk/x.h>
#include <fltk/Window.h>
#include <fltk/Style.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#include <locale.h>
#include <fltk/visual.h>
#include <fltk/Font.h>
#include <fltk/Browser.h>
#include <fltk/utf.h>

// Actually this file contains a keysym->utf8 function that is useful
// when !USE_XIM, we should rearrange this so it can be used in that case:
#if USE_XIM && !defined(X_HAVE_UTF8_STRING)
# include "xutf8.cxx"
#endif

using namespace fltk;

////////////////////////////////////////////////////////////////
// interface to poll/select call:

#if USE_POLL

#include <poll.h>
static pollfd *pollfds = 0;

#else

#if HAVE_SYS_SELECT_H
#  include <sys/select.h>
#endif /* HAVE_SYS_SELECT_H */

// The following #define is only needed for HP-UX 9.x and earlier:
//#define select(a,b,c,d,e) select((a),(int *)(b),(int *)(c),(int *)(d),(e))

static fd_set fdsets[3];
static int maxfd;
#define POLLIN 1
#define POLLOUT 4
#define POLLERR 8

#endif /* USE_POLL */

static int nfds = 0;
static int fd_array_size = 0;
static struct FD {
#if !USE_POLL
  int fd;
  short events;
#endif
  void (*cb)(int, void*);
  void* arg;
} *fd = 0;

////////////////////////////////////////////////////////////////
#if USE_XIM

static XIM fl_xim_im = 0;
static XIC fl_xim_ic = 0;
static XFontSet fl_xim_fs = NULL;
static char fl_is_over_the_spot = 0;
static XRectangle status_area;

static void fl_new_ic()
{
  XVaNestedList preedit_attr;
  XVaNestedList status_attr;
  char          **missing_list;
  int           missing_count;
  char          *def_string;
  int predit = 0;
  int sarea = 0;
  XIMStyles* xim_styles = NULL;

  if (!fl_xim_fs) {
    fl_xim_fs = XCreateFontSet(xdisplay,
			       "-misc-fixed-medium-r-normal--14-*",
			       &missing_list, &missing_count, &def_string);
  }

  XPoint	spot;
  spot.x = 0;
  spot.y = 0;
  preedit_attr = XVaCreateNestedList(0,
				     XNSpotLocation, &spot,
				     XNFontSet, fl_xim_fs, NULL);
  status_attr = XVaCreateNestedList(0,
				    XNAreaNeeded, &status_area,
				    XNFontSet, fl_xim_fs, NULL);

  if (!XGetIMValues (fl_xim_im, XNQueryInputStyle, 
		     &xim_styles, NULL, NULL)) {
    int i;
    XIMStyle *style;
    for (i = 0, style = xim_styles->supported_styles;
	 i < xim_styles->count_styles; i++, style++) {
      if (*style == (XIMPreeditPosition | XIMStatusArea)) {
	sarea = 1;
	predit = 1;
      } else if (*style == (XIMPreeditPosition | XIMStatusNothing)) {
	predit = 1;
      }
    }
  }
  XFree(xim_styles);

  if (fl_xim_ic)
    XDestroyIC(fl_xim_ic);
  fl_xim_ic = NULL;
  if (sarea)
    fl_xim_ic = XCreateIC(fl_xim_im,
			  XNInputStyle, (XIMPreeditPosition | XIMStatusArea),
			  XNPreeditAttributes, preedit_attr,
			  XNStatusAttributes, status_attr,
			  NULL);

  if (!fl_xim_ic && predit)
    fl_xim_ic = XCreateIC(fl_xim_im,
			  XNInputStyle,(XIMPreeditPosition | XIMStatusNothing),
			  XNPreeditAttributes, preedit_attr,
			  NULL);

  XFree(preedit_attr);
  XFree(status_attr);

  if (!fl_xim_ic) {
    fl_is_over_the_spot = 0;
    fl_xim_ic = XCreateIC(fl_xim_im,
			  XNInputStyle, (XIMPreeditNothing | XIMStatusNothing),
			  NULL);
  } else {
    fl_is_over_the_spot = 1;
    XVaNestedList status_attr;
    status_attr = XVaCreateNestedList(0, XNAreaNeeded, &status_area, NULL);
    if (status_area.height != 0)
      XGetICValues(fl_xim_ic, XNStatusAttributes, status_attr, NULL);
    XFree(status_attr);
  }
}

static void fl_init_xim()
{	
  XIMStyles *xim_styles;
  if (!xdisplay) return;
  if (fl_xim_im) return;

  XSetLocaleModifiers("");
  fl_xim_im = XOpenIM(xdisplay, NULL, NULL, NULL);
  xim_styles = NULL;
  fl_xim_ic = NULL;

  if (fl_xim_im) {
    XGetIMValues (fl_xim_im, XNQueryInputStyle,
		  &xim_styles, NULL, NULL);
  } else {
    warning("XOpenIM() failed\n");
    return;
  }

  if (xim_styles && xim_styles->count_styles) {
    fl_new_ic();
  } else {
    warning("No XIM style found\n");
    XCloseIM(fl_xim_im);
    fl_xim_im = NULL;
    return;
  }

  if (!fl_xim_ic) {
    warning("XCreateIC() failed\n");
    XCloseIM(fl_xim_im);
    XFree(xim_styles);
    fl_xim_im = NULL;
  }
}

void fl_set_spot(fltk::Font *f, Widget *w, int x, int y)
{
  int change = 0;
  XVaNestedList preedit_attr;
  char          **missing_list;
  int           missing_count;
  char          *def_string;
  const char *fnt = NULL;
  static XIC ic = NULL;
  static fltk::Font *spotf = NULL;
  static Widget *spotw = NULL;
  static XPoint	spot, spot_set;
  static Color background_orig, textcolor_orig;
  static Color background, textcolor;
  XFontSet fs = NULL;

  if (!fl_xim_ic || !fl_is_over_the_spot) return;
  if (w != spotw) {
    spotw = w;
    change = 1;
  }
  if (x != spot.x || y != spot.y) {
    spot.x = x;
    spot.y = y;
    change = 1;
  }
  if (f != spotf) {
    spotf = f;
    if (f) {
      fnt = f->system_name();
      if (fnt)
	fs = XCreateFontSet(xdisplay, fnt, &missing_list,
			    &missing_count, &def_string);
      else
	return;
      if (fs) {
	XFreeFontSet(xdisplay, fl_xim_fs);
	fl_xim_fs = fs;
	change = 1;
      }
    } else {
      change = 1;
    }
  }
  if (fl_xim_ic != ic) {
    ic = fl_xim_ic;
    change = 1;
  }

  w->style()->boxcolors(w->current_flags()|OUTPUT,background,textcolor);
  if (background_orig != background || textcolor_orig != textcolor) {
    background_orig = background;
    textcolor_orig = textcolor;
    change = 1;
  }

  if (!change) return;

  if (f) {
    XFontSetExtents *extents;
    int ascent;
    extents = XExtentsOfFontSet(fl_xim_fs);
    ascent=-extents->max_logical_extent.y;
    spot_set = spot;
    spot_set.y += ascent;
    preedit_attr =
      XVaCreateNestedList(0,
			  XNSpotLocation, &spot_set,
			  XNForeground, xpixel(textcolor),
			  XNBackground, xpixel(background),
			  XNFontSet, fl_xim_fs, NULL);
    if (preedit_attr) {
      XSetICValues(fl_xim_ic, XNPreeditAttributes, preedit_attr, NULL);
      XFree(preedit_attr);
    }
    XSetICFocus(fl_xim_ic);
  } else {
    XUnsetICFocus(fl_xim_ic);
  }
}

static XWindow xim_win = 0; // which window we set xim to last

#else // !USE_XIM
void fl_set_spot(fltk::Font *f, Widget *w, int x, int y) {}
#endif

/*!

  Add file descriptor fd to listen to. When the fd becomes ready for
  reading fltk::wait() will call the callback function and then
  return. The callback is passed the fd and the arbitrary void*
  argument.

  The second argument is a bitfield to indicate when the callback
  should be done. You can or these together to make the callback be
  called for multiple conditions:
  - fltk::READ - Call the callback when there is data to be read.
  - fltk::WRITE - Call the callback when data can be written without blocking.
  - fltk::EXCEPT - Call the callback if an exception occurs on the file.

  Under UNIX any file descriptor can be monitored (files, devices,
  pipes, sockets, etc.) Due to limitations in Microsoft Windows, WIN32
  applications can only monitor sockets (? and is the when value
  ignored?)
*/
void fltk::add_fd(int n, int events, FileHandler cb, void *v) {
  remove_fd(n,events);
  int i = nfds++;
  if (i >= fd_array_size) {
    fd_array_size = 2*fd_array_size+1;
    fd = (FD*)realloc(fd, fd_array_size*sizeof(FD));
#if USE_POLL
    pollfds = (pollfd*)realloc(pollfds, fd_array_size*sizeof(pollfd));
#endif
  }
  fd[i].cb = cb;
  fd[i].arg = v;
#if USE_POLL
  pollfds[i].fd = n;
  pollfds[i].events = events;
#else
  fd[i].fd = n;
  fd[i].events = events;
  if (events & POLLIN) FD_SET(n, &fdsets[0]);
  if (events & POLLOUT) FD_SET(n, &fdsets[1]);
  if (events & POLLERR) FD_SET(n, &fdsets[2]);
  if (n > maxfd) maxfd = n;
#endif
}

/*! Same as add_fd(fd, READ, cb, v); */
void fltk::add_fd(int fd, FileHandler cb, void* v) {
  add_fd(fd, POLLIN, cb, v);
}

/*!
  Remove all the callbacks (ie for all different when values) for the
  given file descriptor. It is harmless to call this if there are no
  callbacks for the file descriptor. If when is given then those bits
  are removed from each callback for the file descriptor, and the
  callback removed only if all of the bits turn off.
*/
void fltk::remove_fd(int n, int events) {
  int i,j;
  for (i=j=0; i<nfds; i++) {
#if USE_POLL
    if (pollfds[i].fd == n) {
      int e = pollfds[i].events & ~events;
      if (!e) continue; // if no events left, delete this fd
      pollfds[j].events = e;
    }
#else
    if (fd[i].fd == n) {
      int e = fd[i].events & ~events;
      if (!e) continue; // if no events left, delete this fd
      fd[i].events = e;
    }
#endif
    // move it down in the array if necessary:
    if (j<i) {
      fd[j] = fd[i];
#if USE_POLL
      pollfds[j] = pollfds[i];
#endif
    }
    j++;
  }
  nfds = j;
#if !USE_POLL
  if (events & POLLIN) FD_CLR(n, &fdsets[0]);
  if (events & POLLOUT) FD_CLR(n, &fdsets[1]);
  if (events & POLLERR) FD_CLR(n, &fdsets[2]);
  if (n == maxfd) maxfd--;
#endif
}

#if CONSOLIDATE_MOTION
static Window* send_motion;
#endif
static bool in_a_window; // true if in any of our windows, even destroyed ones
static void do_queued_events(int, void*) {
  in_a_window = true;
  while (!exit_modal_ && XEventsQueued(xdisplay,QueuedAfterReading)) {
    XNextEvent(xdisplay, &xevent);
    handle();
  }
  // we send LEAVE only if the mouse did not enter some other window:
  if (!in_a_window) handle(LEAVE, 0);
#if CONSOLIDATE_MOTION
  else if (send_motion == xmousewin) {
    send_motion = 0;
    handle(MOVE, xmousewin);
  }
#endif
}

// these pointers are set by the lock() function:
static void nothing() {}
void (*fl_lock_function)() = nothing;
void (*fl_unlock_function)() = nothing;

// Wait up to the given time for any events or sockets to become ready,
// do the callbacks for the events and sockets:
static inline int fl_wait(float time_to_wait) {

  // OpenGL and other broken libraries call XEventsQueued()
  // and thus cause the file descriptor to not be ready,
  // so we must check for already-read events:
  if (xdisplay && XQLength(xdisplay)) {do_queued_events(0,0); return 1;}

#if !USE_POLL
  fd_set fdt[3];
  fdt[0] = fdsets[0];
  fdt[1] = fdsets[1];
  fdt[2] = fdsets[2];
#endif

  fl_unlock_function();
#if USE_POLL
  int n = ::poll(pollfds, nfds,
		 (time_to_wait<2147483.648f) ? int(time_to_wait*1000+.5f) : -1);
#else
  int n;
  if (time_to_wait < 2147483.648f) {
    timeval t;
    t.tv_sec = int(time_to_wait);
    t.tv_usec = int(1000000 * (time_to_wait-float(t.tv_sec)));
    n = ::select(maxfd+1,&fdt[0],&fdt[1],&fdt[2],&t);
  } else {
    n = ::select(maxfd+1,&fdt[0],&fdt[1],&fdt[2],0);
  }
#endif
  fl_lock_function();

  if (n > 0) {
    for (int i=0; i<nfds; i++) {
#if USE_POLL
      if (pollfds[i].revents) fd[i].cb(pollfds[i].fd, fd[i].arg);
#else
      int f = fd[i].fd;
      short revents = 0;
      if (FD_ISSET(f,&fdt[0])) revents |= POLLIN;
      if (FD_ISSET(f,&fdt[1])) revents |= POLLOUT;
      if (FD_ISSET(f,&fdt[2])) revents |= POLLERR;
      if (fd[i].events & revents) fd[i].cb(f, fd[i].arg);
#endif
    }
  }
  return n;
}

// ready() is just like wait(0.0) except no callbacks are done:
static inline int fl_ready() {
  if (XQLength(xdisplay)) return 1;
#if USE_POLL
  return ::poll(pollfds, nfds, 0);
#else
  timeval t;
  t.tv_sec = 0;
  t.tv_usec = 0;
  fd_set fdt[3];
  fdt[0] = fdsets[0];
  fdt[1] = fdsets[1];
  fdt[2] = fdsets[2];
  return ::select(maxfd+1,&fdt[0],&fdt[1],&fdt[2],&t);
#endif
}

////////////////////////////////////////////////////////////////

Display *fltk::xdisplay = 0;
XWindow fltk::message_window;
int fltk::xscreen;
XVisualInfo *fltk::xvisual;
Colormap fltk::xcolormap;

static Atom WM_DELETE_WINDOW;
static Atom WM_PROTOCOLS;
       Atom MOTIF_WM_HINTS;
static Atom FLTKChangeScheme;
static Atom TARGETS;
static Atom CLIPBOARD;
Atom XdndAware;
Atom XdndSelection;
Atom XdndEnter;
Atom XdndTypeList;
Atom XdndPosition;
Atom XdndLeave;
Atom XdndDrop;
Atom XdndStatus;
Atom XdndActionCopy;
Atom XdndFinished;
Atom textplainutf;
Atom textplain;
static Atom XA_TEXT;
Atom texturilist;
//Atom XdndProxy;
static Atom _NET_WM_NAME;
static Atom _NET_WM_ICON_NAME;
static Atom _NET_WORKAREA;
static Atom _NET_CURRENT_DESKTOP;
Atom UTF8_STRING;

extern "C" {
static int io_error_handler(Display*) {fatal("X I/O error"); return 0;}

static int xerror_handler(Display* d, XErrorEvent* e) {
  char buf1[128], buf2[128];
  sprintf(buf1, "XRequest.%d", e->request_code);
  XGetErrorDatabaseText(d,"",buf1,buf1,buf2,128);
  XGetErrorText(d, e->error_code, buf1, 128);
  warning("%s: %s 0x%lx", buf2, buf1, e->resourceid);
  return 0;
}
}

void fltk::open_display() {
  if (xdisplay) return;

  setlocale(LC_ALL, "");
  XSetIOErrorHandler(io_error_handler);
  XSetErrorHandler(xerror_handler);

  Display *d = XOpenDisplay(0);
  if (!d) fatal("Can't open display \"%s\"",XDisplayName(0));

  open_display(d);
}

void fltk::open_display(Display* d) {
  xdisplay = d;
  add_fd(ConnectionNumber(d), POLLIN, do_queued_events);

#define MAX_ATOMS 30
  Atom* atom_ptr[MAX_ATOMS];
  char* names[MAX_ATOMS];
  int i = 0;
#define atom(a,b) atom_ptr[i] = &a; names[i] = b; i++
  atom(	WM_DELETE_WINDOW	, "WM_DELETE_WINDOW");
  atom(	WM_PROTOCOLS		, "WM_PROTOCOLS");
  atom(	MOTIF_WM_HINTS		, "_MOTIF_WM_HINTS");
  atom(	FLTKChangeScheme	, "FLTKChangeScheme");
  atom(	TARGETS			, "TARGETS");
  atom(	CLIPBOARD		, "CLIPBOARD");
  atom(	XdndAware		, "XdndAware");
  atom(	XdndSelection		, "XdndSelection");
  atom(	XdndEnter		, "XdndEnter");
  atom(	XdndTypeList		, "XdndTypeList");
  atom(	XdndPosition		, "XdndPosition");
  atom(	XdndLeave		, "XdndLeave");
  atom(	XdndDrop		, "XdndDrop");
  atom(	XdndStatus		, "XdndStatus");
  atom(	XdndActionCopy		, "XdndActionCopy");
  atom(	XdndFinished		, "XdndFinished");
  atom(	textplainutf		, "text/plain;charset=UTF-8");
  atom(	textplain		, "text/plain");
  atom(	XA_TEXT			, "TEXT");
  atom(	texturilist		, "text/uri-list");
  //atom(XdndProxy		, "XdndProxy");
  atom( _NET_WM_NAME		, "_NET_WM_NAME");
  atom( _NET_WM_ICON_NAME	, "_NET_WM_ICON_NAME");
  atom(	_NET_WORKAREA		, "_NET_WORKAREA");
  atom(	_NET_CURRENT_DESKTOP	, "_NET_CURRENT_DESKTOP");
  atom( UTF8_STRING		, "UTF8_STRING");
#undef atom
  Atom atoms[MAX_ATOMS];
  XInternAtoms(d, names, i, 0, atoms);
  for (; i--;) *atom_ptr[i] = atoms[i];

  xscreen = DefaultScreen(d);

  // This window is used for interfaces that need a window id even
  // though there is no real reason to have a window visible:
  message_window =
    XCreateSimpleWindow(d, RootWindow(d, xscreen), 0,0,1,1,0, 0, 0);

  // construct an XVisualInfo that matches the default Visual:
  XVisualInfo templt; int num;
  templt.visualid = XVisualIDFromVisual(DefaultVisual(d, xscreen));
  xvisual = XGetVisualInfo(d, VisualIDMask, &templt, &num);
  xcolormap = DefaultColormap(d, xscreen);
#if USE_XIM
  fl_init_xim();
#endif

#if !USE_COLORMAP
  visual(RGB);
#endif
}

void fltk::close_display() {
  remove_fd(ConnectionNumber(xdisplay));
  XCloseDisplay(xdisplay);
}

////////////////////////////////////////////////////////////////

/*! \class fltk::Rectangle
  Class proposed for replacing all the x,y,w,h sets being passed
  to most fltk functions. This may make a lot of calls easier to
  read.

  Negative w() or h() is supposed to mean an empty and thus
  invisible rectangle, but some code will treat the rectangle as
  reflected about x or y. Set the size to zero to make sure you
  have an empty one.
*/

/*! \fn Rectangle::Rectangle()
  The default constructor does not put anything into the fields!
  You can either call set() or just modify the x_, y_, w_, and h_
  variables directly.
*/

/*! \fn Rectangle::Rectangle(int x, int y, int w, int h)
  Constructor that initializes all the fields. You can also use
  the copy constructor to make a new rectangle equal to an old one.
*/

/*! \fn void Rectangle::set(int x, int y, int w, int h)
  Set all 4 fields at once.
*/

/*! \fn int Rectangle::x() const
  Return the left edge of the rectangle. */
/*! \fn int Rectangle::y() const
  Return the top edge of the rectangle. */
/*! \fn int Rectangle::w() const
  Return the width of the rectangle. */
/*! \fn int Rectangle::h() const
  Return the height of the rectangle. */
/*! \fn int Rectangle::r() const
  Return x()+w(), the right edge of the rectangle. */
/*! \fn int Rectangle::b() const
  Return y()+h(), the bottom edge of the rectangle. */

////////////////////////////////////////////////////////////////

static bool reload_info = true;

/*! \class fltk::Monitor
    Structure describing a monitor (screen).
    Structure describing one of the monitors (screens) connected to
    the system. You can ask for one by position with find(), ask for
    all of them with list(), and ask for a fake one that surrounds all
    of them with all(). You then look in the structure to get information
    like the size and bit depth.
*/

/*! \var Monitor::work
  The rectangle of the monitor not covered by tool or menu bars. This
  is not a method because it looks clearer to write "monitor.work.x()"
  than "monitor.work().x()".
*/

/*! Return a "monitor" that surrounds all the monitors.
    If you have a single monitor, this returns a monitor structure that
    defines it. If you have multiple monitors this returns a fake monitor
    that surrounds all of them.
*/
const Monitor& Monitor::all() {
  static Monitor monitor;
  if (reload_info) {
    reload_info = false;
    open_display();
    monitor.set(0, 0,
		DisplayWidth(xdisplay, xscreen),
		DisplayHeight(xdisplay, xscreen));

    // initially assume work area is entire monitor:
    monitor.work = monitor;

    // Try to get the work area from the X Desktop standard:
    // First find out what desktop we are on, as it allows the work area
    // to be different (however fltk will return whatever the first answer
    // is even if the user changes the desktop, so this is not exactly
    // right):
    XWindow root = RootWindow(xdisplay, xscreen);
    Atom actual=0; int format; unsigned long count, remaining;
    unsigned char* buffer = 0;
    XGetWindowProperty(xdisplay, root, _NET_CURRENT_DESKTOP,
		       0, 1, false, XA_CARDINAL,
		       &actual, &format, &count, &remaining, &buffer);
    int desktop = 0;
    if (buffer && actual == XA_CARDINAL && format == 32 && count > 0)
      desktop = int(*(long*)buffer);
    if (buffer) {XFree(buffer); buffer = 0;}
    // Now get the workarea, which is an array of workareas for each
    // desktop. The 4*desktop argument makes it index the correct
    // distance into the workarea:
    actual = 0;
    XGetWindowProperty(xdisplay, root, _NET_WORKAREA,
		       4*desktop, 4, false, XA_CARDINAL,
		       &actual, &format, &count, &remaining, &buffer);
    if (buffer && actual == XA_CARDINAL && format == 32 && count > 3) {
      long* p = (long*)buffer;
      monitor.work.set(int(p[0]),int(p[1]),int(p[2]),int(p[3]));
    }
    if (buffer) {XFree(buffer); buffer = 0;}

    monitor.depth_ = xvisual->depth;

    // do any screens really return 0 for MM?
    int mm = DisplayWidthMM(xdisplay, xscreen);
    monitor.dpi_x_ = mm ? monitor.w()*25.4f/mm : 100.0f;
    mm = DisplayHeightMM(xdisplay, xscreen);
    monitor.dpi_y_ = mm ? monitor.h()*25.4f/mm : monitor.dpi_x_;

  }
  return monitor;
}

#if USE_XINERAMA
extern "C" {
#include <X11/extensions/Xinerama.h>
// Version 1 is what is on my RedHat 7.2 machine, with "XineramaQueryScreens"
// Version 2 is what is in the Xinerama documentation, with "XineramaGetData"
#define XINERAMA_VERSION 1
}
#endif

static Monitor* monitors = 0;
static int num_monitors=0;

/*! Return an array of all Monitors.
    p is set to point to a static array of Monitor structures describing
    all monitors connected to the system. If there is a "primary" monitor,
    it will be first in the list.

    Subsequent calls will usually
    return the same array, but if a signal comes in indicating a change
    it will probably delete the old array and return a new one.
*/
int Monitor::list(const Monitor** p) {
  if (!num_monitors) {
    open_display();
#if USE_XINERAMA
# if XINERAMA_VERSION > 1
    XRectangle* rects = 0; int count = 0;
    int a=0; int b=0;
    if (XineramaQueryExtension(xdisplay,&a,&b) &&
	XineramaIsActive(xdisplay, message_window))
      XineramaGetData(xdisplay, message_window, &rects, &count);
# else
    XineramaScreenInfo* rects = 0; int count = 0;
    int a=0; int b=0;
    if (XineramaQueryExtension(xdisplay,&a,&b))
      rects = XineramaQueryScreens(xdisplay, &count);
# endif
    if (count > 1 && count < 100) {
      const Monitor& a = all();
      num_monitors = count;
      monitors = new Monitor[count];
      for (int i = 0; i < count; i++) {
	Monitor& m = monitors[i];
	m = a;
# if XINERAMA_VERSION > 1
	int x = rects[i].x;
	int y = rects[i].y;
# else
	int x = rects[i].x_org;
	int y = rects[i].y_org;
# endif
	int r = x+rects[i].width;
	int b = y+rects[i].height;
	m.set(x, y, r-x, b-y);
	if (a.work.x() > x) x = a.work.x();
	if (a.work.y() > y) y = a.work.y();
	if (a.work.r() < r) r = a.work.r();
	if (a.work.b() < b) b = a.work.b();
	m.work.set(x, y, r-x, b-y);
      }
      XFree(rects);
      goto DONE;
    }
    XFree(rects);
#endif
    num_monitors = 1; // indicate that Xinerama failed
    monitors = (Monitor*)(&all());
#if !USE_XINERAMA
    // Guess for dual monitors placed next to each other:
    if (monitors->w() > 2*monitors->h()) {
      num_monitors = 2;
      monitors = new Monitor[2];
      monitors[0] = monitors[1] = all();
      monitors[0].w_ = monitors[1].x_ = monitors[0].w_/2;
      monitors[1].w_ -= monitors[1].x_;
      monitors[0].work.w_ = monitors[0].w_-monitors[0].work.x_;
      monitors[1].work.w_ -= monitors[0].work.w_;
      monitors[1].work.x_ = monitors[1].x_;
    }
#else
  DONE:;
#endif
#if 0
    printf("Got %d monitors:\n", num_monitors);
    for (int i=0; i < num_monitors; i++) {
      const Monitor& m = monitors[i];
      printf(" %d %d %d %d, work %d %d %d %d\n",
  	     m.x(), m.y(), m.w(), m.h(),
  	     m.work.x(), m.work.y(), m.work.w(), m.work.h());
    }
#endif
  }
  *p = monitors;
  return num_monitors;
}

/*! Return a pointer to a Monitor structure describing the monitor
    that contains or is closest to the given x,y, position.
*/
const Monitor& Monitor::find(int x, int y) {
  const Monitor* monitors;
  int count = list(&monitors);
  const Monitor* ret = monitors+0;
  if (count > 1) {
    int r = 0;
    for (int i = 0; i < count; i++) {
      const Monitor& m = monitors[i];
      // find distances to nearest edges:
      int rx;
      if (x <= m.x()) rx = m.x()-x;
      else if (x >= m.r()) rx = x-m.r();
      else rx = 0;
      int ry;
      if (y <= m.y()) ry = m.y()-y;
      else if (y >= m.b()) ry = y-m.b();
      else ry = 0;
      // return this screen if inside:
      if (rx <= 0 && ry <= 0) return m;
      // use larger of horizontal and vertical distances:
      if (rx < ry) rx = ry;
      // remember if this is the closest screen:
      if (!i || rx < r) {
	ret = monitors+i;
	r = rx;
      }
    }
  }
  return *ret;
}

////////////////////////////////////////////////////////////////

/*!
  Return where the mouse is on the screen by doing a round-trip query
  to the server. You should use fltk::event_x_root() and
  fltk::event_y_root() if possible, but this is necessary if you are
  not sure if a mouse event has been processed recently (such as to
  position your first window). If the display is not open, this will
  open it.
*/
void fltk::get_mouse(int &x, int &y) {
  open_display();
  XWindow root = RootWindow(xdisplay, xscreen);
  XWindow c; int mx,my,cx,cy; unsigned int mask;
  XQueryPointer(xdisplay,root,&root,&c,&mx,&my,&cx,&cy,&mask);
  x = mx;
  y = my;
}

////////////////////////////////////////////////////////////////

XWindow fltk::dnd_source_window;
Atom *fltk::dnd_source_types; // null-terminated list of data types being supplied
Atom *fl_incoming_dnd_source_types;
Atom fltk::dnd_type;
Atom fltk::dnd_source_action;
Atom fltk::dnd_action;

void fl_sendClientMessage(XWindow xwindow, Atom message,
			  unsigned long d0,
			  unsigned long d1=0,
			  unsigned long d2=0,
			  unsigned long d3=0,
			  unsigned long d4=0)
{
  XEvent e;
  e.xany.type = ClientMessage;
  e.xany.window = xwindow;
  e.xclient.message_type = message;
  e.xclient.format = 32;
  e.xclient.data.l[0] = (long)d0;
  e.xclient.data.l[1] = (long)d1;
  e.xclient.data.l[2] = (long)d2;
  e.xclient.data.l[3] = (long)d3;
  e.xclient.data.l[4] = (long)d4;
  XSendEvent(xdisplay, xwindow, 0, 0, &e);
}

////////////////////////////////////////////////////////////////
// Code used for cut & paste

static Widget *selection_requestor;
static char *selection_buffer[2];
static int selection_length[2];
static int selection_buffer_length[2];
bool fl_i_own_selection[2];

/*!
  Change the current selection. The block of text is copied to an
  internal buffer by FLTK (be careful if doing this in response to an
  fltk::PASTE as this may be the same buffer returned by
  event_text()).

  The block of text may be retrieved (from this program or whatever
  program last set it) with fltk::paste().

  There are actually two buffers. If \a clipboard is true then the text
  goes into the user-visible selection that is moved around with
  cut/copy/paste commands (on X this is the CLIPBOARD selection). If
  \a clipboard is false then the text goes into a less-visible buffer
  used for temporarily selecting text with the mouse and for drag &
  drop (on X this is the XA_PRIMARY selection).
*/
void fltk::copy(const char *stuff, int len, bool clipboard) {
  if (!stuff || len<0) return;
  if (len+1 > selection_buffer_length[clipboard]) {
    delete[] selection_buffer[clipboard];
    selection_buffer[clipboard] = new char[len+100];
    selection_buffer_length[clipboard] = len+100;
  }
  memcpy(selection_buffer[clipboard], stuff, len);
  selection_buffer[clipboard][len] = 0; // needed for direct paste
  selection_length[clipboard] = len;
  fl_i_own_selection[clipboard] = true;
  Atom property = clipboard ? CLIPBOARD : XA_PRIMARY;
  XSetSelectionOwner(xdisplay, property, message_window, event_time);
}

/*!
  This is what a widget does when a "paste" command (like Ctrl+V or
  the middle mouse click) is done to it. Cause an fltk::PASTE event to
  be sent to the receiver with the contents of the current selection
  in the fltk::event_text(). The selection can be set by fltk::copy().

  There are actually two buffers. If \a clipboard is true then the text
  is from the user-visible selection that is moved around with
  cut/copy/paste commands (on X this is the CLIPBOARD selection). If
  \a clipboard is false then the text is from a less-visible buffer
  used for temporarily selecting text with the mouse and for drag &
  drop (on X this is the XA_PRIMARY selection).

  The reciever should be prepared to be called \e directly by this, or
  for it to happen later, or possibly not at all. This allows the
  window system to take as long as necessary to retrieve the paste
  buffer (or even to screw up completely) without complex and
  error-prone synchronization code most toolkits require.
*/
void fltk::paste(Widget &receiver, bool clipboard) {
  if (fl_i_own_selection[clipboard]) {
    // We already have it, do it quickly without window server.
    // Notice that the text is clobbered if set_selection is
    // called in response to PASTE!
    e_text = selection_buffer[clipboard];
    e_length = selection_length[clipboard];
    receiver.handle(PASTE);
    return;
  }
  // otherwise get the window server to return it:
  selection_requestor = &receiver;
  Atom property = clipboard ? CLIPBOARD : XA_PRIMARY;
  XConvertSelection(xdisplay, property, TARGETS, property,
		    xid(Window::first()), event_time);
}

////////////////////////////////////////////////////////////////

XEvent fltk::xevent; // the current x event
ulong fltk::event_time; // the last timestamp from an x event

char fl_key_vector[32]; // used by get_key()

// Records shift keys that X does not handle:
static int extra_state;

// Record event mouse position and state from an XEvent.
static void set_event_xy(bool push) {
#if CONSOLIDATE_MOTION
  send_motion = 0;
#endif
  e_x_root = xevent.xbutton.x_root;
  e_x = xevent.xbutton.x;
  e_y_root = xevent.xbutton.y_root;
  e_y = xevent.xbutton.y;
  e_state = (xevent.xbutton.state << 16) | extra_state;
  event_time = xevent.xbutton.time;
  // turn off is_click if enough time or mouse movement has passed:
  static int px, py;
  static ulong ptime;
  if (abs(e_x_root-px)+abs(e_y_root-py) > 3 
      || event_time >= ptime+(push?1000:200))
    e_is_click = 0;
  if (push) {
    px = e_x_root;
    py = e_y_root;
    ptime = event_time;
  }
}

////////////////////////////////////////////////////////////////

static Window* resize_from_system;

static unsigned wheel_up_button = 4;
static unsigned wheel_down_button = 5;

int fl_actual_keysym;

extern "C" {
  static Bool fake_keyup_test(Display*, XEvent* event, char* previous) {
     return
      event->type == KeyPress &&
      event->xkey.keycode == ((XKeyEvent*)previous)->keycode &&
      event->xkey.time == ((XKeyEvent*)previous)->time;
  }
}

bool fltk::handle()
{
  Window* window = find(xevent.xany.window);
  int event = 0;

 KEYPRESS:
  if (XFilterEvent((XEvent *)&xevent, 0)) return 1;

  switch (xevent.type) {

  case KeymapNotify:
    memcpy(fl_key_vector, xevent.xkeymap.key_vector, 32);
    break;

  case MappingNotify:
    XRefreshKeyboardMapping((XMappingEvent*)&xevent.xmapping);
    extra_state = 0;
    break;

  case ClientMessage: {
    Atom message = xevent.xclient.message_type;
    const long* data = xevent.xclient.data.l;

    if (window && (Atom)(data[0]) == WM_DELETE_WINDOW) {
      if (!grab() && !(modal() && window != modal()))
	window->do_callback();
      return true;

    } else if (message == FLTKChangeScheme) {
      reload_theme();
      return true;

    } else if (message == XdndEnter) {
      xmousewin = window;
      in_a_window = true;
      dnd_source_window = data[0];
      // version number is data[1]>>24
      if (data[1]&1) {
	// get list of data types:
	Atom actual; int format; unsigned long count, remaining;
	unsigned char *buffer = 0;
	XGetWindowProperty(xdisplay, dnd_source_window, XdndTypeList,
			   0, 0x8000000L, False, XA_ATOM, &actual, &format,
			   &count, &remaining, &buffer);
	if (actual != XA_ATOM || format != 32 || count<4 || !buffer)
	  goto FAILED;
	delete [] fl_incoming_dnd_source_types;
	fl_incoming_dnd_source_types = new Atom[count+1];
	dnd_source_types = fl_incoming_dnd_source_types;
	for (unsigned i = 0; i < count; i++)
	  dnd_source_types[i] = ((Atom*)buffer)[i];
	dnd_source_types[count] = 0;
      } else {
      FAILED:
	// less than four data types, or if the above messes up:
	if (!fl_incoming_dnd_source_types)
	  fl_incoming_dnd_source_types = new Atom[4];
	dnd_source_types = fl_incoming_dnd_source_types;
	dnd_source_types[0] = data[2];
	dnd_source_types[1] = data[3];
	dnd_source_types[2] = data[4];
	dnd_source_types[3] = 0;
      }
      // This should return one of the dnd_source_types. Unfortunately
      // no way to just force it to cough up whatever data is "most text-like"
      // instead I have to select from a list of known types. We may need
      // to add to this list in the future, turn on the #if to print the
      // types if you get a drop that you think should work.
      dnd_type = textplain; // try this if no matches, it may work
#if DEBUG_SELECTIONS
      printf("dnd source types:\n");
#endif
      for (int i = 0; ; i++) {
 	Atom type = dnd_source_types[i]; if (!type) break;
#if DEBUG_SELECTIONS
 	char* x = XGetAtomName(xdisplay, type);
 	printf(" %s\n",x);
 	XFree(x);
#endif
	if (type == textplainutf ||
	    type == textplain ||
	    type == UTF8_STRING) {dnd_type = type; break;} // ok
      }
      event = DND_ENTER;
      break;

    } else if (message == XdndPosition) {
      xmousewin = window;
      in_a_window = true;
      dnd_source_window = data[0];
      e_x_root = data[2]>>16;
      e_y_root = data[2]&0xFFFF;
      if (window) {
	e_x = e_x_root-window->x();
	e_y = e_y_root-window->y();
      }
      event_time = data[3];
      dnd_source_action = data[4];
      dnd_action = XdndActionCopy;
      int accept = handle(DND_DRAG, window);
      fl_sendClientMessage(data[0], XdndStatus,
			   xevent.xclient.window,
			   accept ? 1 : 0,
			   0, // used for xy rectangle to not send position inside
			   0, // used for width+height of rectangle
			   accept ? dnd_action : None);
      return true;

    } else if (message == XdndLeave) {
      dnd_source_window = 0; // don't send a finished message to it
      event = DND_LEAVE;
      break;

    } else if (message == XdndDrop) {
      xmousewin = window;
      in_a_window = true;
      dnd_source_window = data[0];
      event_time = data[2];
      XWindow to_window = xevent.xclient.window;
      if (handle(DND_RELEASE, window)) {
	selection_requestor = belowmouse();
	XConvertSelection(xdisplay, XdndSelection,
			  dnd_type, XA_SECONDARY,
			  to_window, event_time);
      } else {
	// Send the finished message if I refuse the drop.
	// It is not clear whether I can just send finished always,
	// or if I have to wait for the SelectionNotify event as the
	// code is currently doing.
	fl_sendClientMessage(dnd_source_window, XdndFinished, to_window);
	dnd_source_window = 0;
      }
      return true;

    }
    break;}

  // We cannot rely on the x,y position in the configure notify event.
  // I now think this is an unavoidable problem with X: it is impossible
  // for a window manager to prevent the "real" notify event from being
  // sent when it resizes the contents, even though it can send an
  // artificial event with the correct position afterwards (and some
  // window managers do not send this fake event anyway)
  // So anyway, do a round trip to find the correct x,y:
  // WAS: Actually, TWO round trips! Is X stoopid or what?
  case ConfigureNotify:
  case MapNotify: {
    window = find(xevent.xmapping.window);
    if (!window) break;
    if (window->parent()) break; // ignore child windows

    // figure out where OS really put window
    XWindowAttributes actual;
    XGetWindowAttributes(xdisplay, xid(window), &actual);
    XWindow cr; int X, Y, W = actual.width, H = actual.height;
    XTranslateCoordinates(xdisplay, xid(window), actual.root,
			  0, 0, &X, &Y, &cr);
#if 0
    // Faster version that does not bother with calling resize as the
    // user drags the window around. This was what most Win32 versions
    // of fltk did. This breaks programs that want to track the current
    // position to figure out what corner is being resized when layout
    // is called.
    if (W == window->w() && H == window->h()) {
      window->x(X);
      window->y(Y);
      break;
    }
#endif
    // tell Window about it and set flag to prevent echoing:
    if (window->resize(X, Y, W, H)) resize_from_system = window;
    break; // allow add_handler to do something too
  }

  case UnmapNotify:
    window = find(xevent.xmapping.window);
    if (!window) break;
    if (window->parent()) break; // ignore child windows
    // turning this flag makes iconic() return true:
    CreatedWindow::find(window)->wait_for_expose = true;
    break; // allow add_handler to do something too

  case Expose:
  case GraphicsExpose:
    if (!window) break;
    // If this window completely fills it's parent, parent will not get
    // an expose event and the wait flag will not turn off. So force this:
    if (CreatedWindow::find(window)->wait_for_expose) {
      for (Window* w = window;;) {
	CreatedWindow::find(w)->wait_for_expose = false;
	w = w->window();
	if (!w) break;
      }
    }
    CreatedWindow::find(window)->expose(xevent.xexpose.x,
					xevent.xexpose.y,
					xevent.xexpose.width,
					xevent.xexpose.height);
    return true;

  case ButtonPress: {
    unsigned n = xevent.xbutton.button;
    e_keysym = n;
    set_event_xy(true);
    // turn off is_click if enough time or mouse movement has passed:
    if (e_is_click == e_keysym) {
      e_clicks++;
    } else {
      e_clicks = 0;
      e_is_click = e_keysym;
    }
    if (n == wheel_up_button) {
      e_dy = +1;
      event = MOUSEWHEEL;
    } else if (n == wheel_down_button) {
      e_dy = -1;
      event = MOUSEWHEEL;
    } else {
      e_state |= BUTTON(n);
      event = PUSH;
    }}
    goto J1;

  case MotionNotify:
    set_event_xy(false);
#if CONSOLIDATE_MOTION
    send_motion = xmousewin = window;
    in_a_window = true;
    return false;
#else
    event = MOVE;
    goto J1;
#endif

  case ButtonRelease: {
    unsigned n = xevent.xbutton.button;
    e_keysym = n;
    set_event_xy(false);
    //if (n == wheel_up_button || n == wheel_down_button) break;
    e_state &= ~BUTTON(n);
    event = RELEASE;}
    goto J1;

  case EnterNotify:
    set_event_xy(false);
    e_state = xevent.xcrossing.state << 16;
    if (xevent.xcrossing.detail == NotifyInferior) break;
//      printf("EnterNotify window %s, xmousewin %s\n",
//  	   window ? window->label() : "NULL",
//  	   xmousewin ? xmousewin->label() : "NULL");
    // XInstallColormap(xdisplay, CreatedWindow::find(window)->colormap);
    event = ENTER;
  J1:
    xmousewin = window;
    in_a_window = true;
    // send a mouse event, with cruft so the grab around modal things works:
    if (grab_) {
      handle(event, window);
      if (grab_ && !exit_modal_)
	XAllowEvents(xdisplay, SyncPointer, CurrentTime);
      return true;
    }
    break;

  case LeaveNotify:
    set_event_xy(false);
    e_state = xevent.xcrossing.state << 16;
    if (xevent.xcrossing.detail == NotifyInferior) break;
//      printf("LeaveNotify window %s, xmousewin %s\n",
//  	   window ? window->label() : "NULL",
//  	   xmousewin ? xmousewin->label() : "NULL");
    xmousewin = 0;
    in_a_window = false; // make do_queued_events produce LEAVE event
    return false;

  case FocusIn:
#if USE_XIM
    if (xim_win != xevent.xclient.window) {
      xim_win = xevent.xclient.window;
      if (fl_xim_ic)
	XDestroyIC(fl_xim_ic);
      fl_xim_ic = NULL;
      fl_new_ic();
      if (fl_xim_ic)
	XSetICValues(fl_xim_ic,
		     XNClientWindow, xim_win,
		     NULL);
    }
    if (fl_xim_ic) XSetICFocus(fl_xim_ic);
#endif
    xfocus = window;
    if (window) {fix_focus(); return true;}
    break;

  case FocusOut:
#if USE_XIM
    if (fl_xim_ic) XUnsetICFocus(fl_xim_ic);
#endif
    if (window && window == xfocus) {xfocus = 0; fix_focus(); return true;}
    break;

  case KeyPress: {
    set_event_xy(true);
    //if (grab_) XAllowEvents(xdisplay, SyncKeyboard, CurrentTime);
    unsigned keycode = xevent.xkey.keycode;
    // Make repeating keys increment the click counter:
    if (fl_key_vector[keycode/8]&(1<<(keycode%8))) {
      e_clicks++;
      e_is_click = 0;
    } else {
      e_clicks = 0;
      e_is_click = keycode+100;
    }
    fl_key_vector[keycode/8] |= (1 << (keycode%8));
    static char* buffer = 0;
    static int buffer_len;
    if (!buffer) buffer = (char*) malloc(buffer_len = 20);
    int len;
    KeySym keysym;
#if USE_XIM
    if (fl_xim_ic) {
      Status status;
    RETRY:
      buffer[0] = 0;
      keysym = 0;
#ifdef __sgi
#define Xutf8LookupString XmbLookupString
#endif
      len = Xutf8LookupString(fl_xim_ic, (XKeyPressedEvent *)&xevent.xkey,
			      buffer, buffer_len-1, &keysym, &status);
      switch (status) {
      case XBufferOverflow:
	buffer_len = len+1;
	buffer = (char*)realloc(buffer, buffer_len);
	goto RETRY;
      case XLookupChars:
      case XLookupKeySym:
      case XLookupBoth:
	break;
      default:
	goto NO_XIM;
      }
    } else {
    NO_XIM:
#endif
      len = XLookupString(&(xevent.xkey), buffer, buffer_len-1, &keysym, 0);
      // Make ctrl+dash produce ^_ like it used to:
      if (xevent.xbutton.state&4 && keysym == '-') buffer[0] = 0x1f;
#if USE_XIM
    }
#endif
    buffer[len] = 0;
    e_text = buffer;
    e_length = len;
    event = KEY;
    fl_actual_keysym = int(keysym);
    goto GET_KEYSYM;}

  case KeyRelease: {
    // Stupid X sends fake key-up events when a repeating key is held
    // down, probably due to some back compatability problem. Fortunatley
    // we can detect this because the repeating KeyPress event is in
    // the queue, get it and execute it instead:
    XEvent temp;
    if (XCheckIfEvent(xdisplay,&temp,fake_keyup_test,(char*)(&xevent))){
      xevent = temp;
      goto KEYPRESS;
    }
    set_event_xy(false);
    unsigned keycode = xevent.xkey.keycode;
    fl_key_vector[keycode/8] &= ~(1 << (keycode%8));
    // Leave event_is_click() on only if this is the last key pressed:
    if (e_is_click != int(keycode+100)) e_is_click = 0;
    event = KEYUP;
    goto GET_KEYSYM;}

  GET_KEYSYM: { // code for both KeyPress and KeyRelease:
    // For compatability with Windows, and to allow alt+x to be matched
    // for keyboard shortcuts even if the keyboard has been reprogrammed,
    // we always return the unshifted keysym, except for the keypad where
    // if numlock is off we return the function keys to match Windows:
    unsigned keycode = xevent.xkey.keycode;
    KeySym keysym = XKeycodeToKeysym(xdisplay, keycode, 0);
    if (!keysym) {
      // X did not map this key, return keycode with 0x8000:
      keysym = keycode|0x8000;
#ifdef __sgi
      // You can plug a Microsoft keyboard into an Irix box but these
      // keys are not translated.  Make them translate like XFree86 does:
      switch (keycode) {
      case 147: keysym = LeftMetaKey; break;
      case 148: keysym = RightMetaKey; break;
      case 149: keysym = MenuKey; break;
      }
      if (fl_key_vector[18]&0x18) extra_state |= META;
      else extra_state &= ~META;
#endif
    } else if (keysym >= 0xff95 && keysym <= 0xff9f) { // XK_KP_*
      if (fl_actual_keysym != int(keysym) && (e_state&NUMLOCK)) {
	// numlock has changed it into a Keypad+n key
	keysym = fl_actual_keysym;
      } else {
#if 0 // turn them always into numeric keys (my preference):
	keysym = Keypad+"7486293150."[keysym-0xff95];
	e_text[0] = char(keysym) & 0x7F;
	e_text[1] = 0;
	e_length = 1;
#else // turn them into function keys indistinguisable from the normal
      // function keys. This matches Windows and was voted on as the
      // preferred behavior:
	static const unsigned short table[] = {
	  HomeKey, LeftKey, UpKey, RightKey,
	  DownKey, PageUpKey, PageDownKey, EndKey,
	  ClearKey, InsertKey, DeleteKey};
	keysym = table[keysym-0xff95];
	e_text[0] = 0;
	e_length = 0;
	// This is what Windows does so if the user uses shift+keypad
	// to get an arrow, the text editor does not think it is a
	// shift+arrow:
	if (e_state&NUMLOCK) e_state &= ~SHIFT;
#endif
      }
    } else {
      // WHY, OH WHY, do they keep changing the Alt + Meta keys!
      // Detect all the assignments I have seen and try to map them
      // back. And now some of them are not setting the shift flags,
      // so I have to kludge that as well...
      switch (keysym) {
      case F0Key+13: // F13, wtf? Use this only if F14 does not exist
	if (XKeysymToKeycode(xdisplay, F0Key+14)) break;
	// otherwise fall through
      case 0xffeb: // XK_Super_L
	keysym = LeftMetaKey;
	break;
      case 0xffec: // XK_Super_R
	keysym = RightMetaKey;
	break;
      case 0xff20: // XK_Multi_key
	keysym = RightMetaKey;
	if (event==KEY) extra_state |= META;
	else extra_state &= ~META;
	break;
      case 0xff7e: // XK_Mode_switch
	keysym = RightAltKey;
	if (event==KEY) extra_state |= ALT;
	else extra_state &= ~ALT;
	break;
      }
    }
    e_keysym = int(keysym);
    break;}

  case SelectionNotify: {
    if (!selection_requestor) return false;
    static unsigned char* buffer;
    if (buffer) {XFree(buffer); buffer = 0;}
    long read = 0;
    if (xevent.xselection.property) for (;;) {
      // The Xdnd code pastes 64K chunks together, possibly to avoid
      // bugs in X servers, or maybe to avoid an extra round-trip to
      // get the property length.  I copy this here:
      Atom actual; int format; unsigned long count, remaining;
      unsigned char* portion;
      if (XGetWindowProperty(xdisplay,
			     xevent.xselection.requestor,
			     xevent.xselection.property,
			     read/4, 65536, 1, 0,
			     &actual, &format, &count, &remaining,
			     &portion)) break; // quit on error
      if (actual == TARGETS || actual == XA_ATOM) {
	Atom type = XA_STRING;
	// see if it offers a better type:
#if DEBUG_SELECTIONS
	printf("selection source types:\n");
	for (unsigned i = 0; i<count; i++) {
 	  Atom t = ((Atom*)portion)[i];
   	  char* x = XGetAtomName(xdisplay, t);
   	  printf(" %s\n",x);
   	  XFree(x);
	}
#endif
	for (unsigned i = 0; i<count; i++) {
 	  Atom t = ((Atom*)portion)[i];
	  if (t == textplainutf ||
	      t == textplain ||
	      t == UTF8_STRING) {type = t; break;}
	  // rest are only used if no utf-8 available:
	  if (t == XA_TEXT || t == texturilist) type = t;
	}
	XFree(portion);
	Atom property = xevent.xselection.property;
	XConvertSelection(xdisplay, property, type, property,
			  xid(Window::first()),
			  event_time);
	return true;
      }
#if DEBUG_SELECTIONS
      char* x = XGetAtomName(xdisplay, actual);
      printf("selection notify of type %s\n",x);
      XFree(x);
#endif
      if (read) { // append to the accumulated buffer
	buffer = (unsigned char*)realloc(buffer, read+count*format/8+remaining);
	memcpy(buffer+read, portion, count*format/8);
	XFree(portion);
      } else {	// Use the first section without moving the memory:
	buffer = portion;
      }
      read += count*format/8;
      if (!remaining) break;
    }
    e_text = (char*)buffer;
    e_length = read;
    selection_requestor->handle(PASTE);
    // Detect if this paste is due to Xdnd by the property name (I use
    // XA_SECONDARY for that) and send an XdndFinished message. It is not
    // clear if this has to be delayed until now or if it can be done
    // immediatly after calling XConvertSelection.
    if (xevent.xselection.property == XA_SECONDARY &&
	dnd_source_window) {
      fl_sendClientMessage(dnd_source_window, XdndFinished,
			   xevent.xselection.requestor);
      dnd_source_window = 0; // don't send a second time
    }
    return true;}

  case SelectionClear: {
    bool clipboard = xevent.xselectionclear.selection == CLIPBOARD;
    fl_i_own_selection[clipboard] = false;
    return true;}

  case SelectionRequest: {
    XSelectionEvent e;
    e.type = SelectionNotify;
    e.requestor = xevent.xselectionrequest.requestor;
    e.selection = xevent.xselectionrequest.selection;
    bool clipboard = e.selection == CLIPBOARD;
    e.target = xevent.xselectionrequest.target;
    e.time = xevent.xselectionrequest.time;
    e.property = xevent.xselectionrequest.property;
#if DEBUG_SELECTIONS
    char* x = XGetAtomName(xdisplay,e.target);
    fprintf(stderr,"selection request for %s\n",x);
    XFree(x);
#endif
    if (!selection_length[clipboard]) {
      e.property = 0;
    } else if (e.target == TARGETS) {
      Atom a[3] = {UTF8_STRING, XA_STRING, XA_TEXT};
      XChangeProperty(xdisplay, e.requestor, e.property,
		      XA_ATOM, sizeof(Atom)*8, 0, (unsigned char*)a,
		      sizeof(a)/sizeof(Atom));
    } else if (e.target == UTF8_STRING ||
	       e.target == XA_STRING ||
	       e.target == XA_TEXT ||
	       e.target == textplain ||
	       e.target == textplainutf) {
      /*if (e.target == XA_TEXT)*/ e.target = UTF8_STRING;
      XChangeProperty(xdisplay, e.requestor, e.property,
  		      e.target, 8, 0,
  		      (unsigned char *)selection_buffer[clipboard],
  		      selection_length[clipboard]);
    } else {
      e.property = 0;
    }
    XSendEvent(xdisplay, e.requestor, 0, 0, (XEvent *)&e);}
    return true;

  }

  return handle(event, window);
}

////////////////////////////////////////////////////////////////

/**
 * Resizes the actual system window in response to a resize() call from
 * the program.
 */
void Window::layout() {
  if (layout_damage() & LAYOUT_WH)
    free_backbuffer();
  if (this == resize_from_system) {
    // Ignore changes that came from the system
    resize_from_system = 0;
  } else if ((layout_damage()&LAYOUT_XYWH) && i) { // only for shown windows
    // figure out where the window should be in it's parent:
    int x = this->x(); int y = this->y();
    for (Widget* p = parent(); p && !p->is_window(); p = p->parent()) {
      x += p->x(); y += p->y();
    }
    if (layout_damage() & LAYOUT_WH) {
      // Some window managers refuse to allow resizes unless the resize
      // information allows it:
      if (minw == maxw && minh == maxh) size_range(w(), h(), w(), h());
      XMoveResizeWindow(xdisplay, i->xid, x, y,
			w()>0 ? w() : 1, h()>0 ? h() : 1);
      // Wait for echo (relies on window having StaticGravity!!!)
      i->wait_for_expose = true;
    } else {
      XMoveWindow(xdisplay, i->xid, x, y);
    }
  }
  if (layout_damage() & ~LAYOUT_XY) Group::layout();
  else layout_damage(0);
}

////////////////////////////////////////////////////////////////
// Innards of Window::create()

extern bool fl_show_iconic; // In Window.cxx, set by iconize() or -i switch

void Window::create() {
  CreatedWindow::create(this, xvisual, xcolormap, -1);
}

void CreatedWindow::create(Window* window,
			   XVisualInfo *visual, Colormap colormap,
			   int background)
{
  XSetWindowAttributes attr;
  attr.border_pixel = 0;
  attr.colormap = colormap;
  attr.bit_gravity = 0; // StaticGravity;
  int mask = CWBorderPixel|CWColormap|CWEventMask|CWBitGravity;

  int W = window->w();
  if (W <= 0) W = 1; // X don't like zero...
  int H = window->h();
  if (H <= 0) H = 1; // X don't like zero...
  int X = window->x();
  int Y = window->y();

  ulong root;

  if (window->parent()) {
    // Find the position in the surrounding window, taking into account
    // any intermediate Group widgets:
    for (Widget *o = window->parent(); ; o = o->parent()) {
      if (o->is_window()) {root = ((Window*)o)->i->xid; break;}
      X += o->x();
      Y += o->y();
    }
    attr.event_mask = ExposureMask;
  } else {
    if (X == USEDEFAULT || Y == USEDEFAULT) {
      // Select a window position. Some window managers will ignore this
      // because we do not set the "USPosition" or "PSPosition" bits in
      // the hints, indicating that automatic placement should be done.
      // But it appears that newer window managers use the position,
      // especially for child windows, so we better set it:
      const Window* parent = window->child_of();
      int sw = DisplayWidth(xdisplay, xscreen);
      int sh = DisplayHeight(xdisplay, xscreen);
      if (parent) {
	X = parent->x()+((parent->w()-W)>>1);
	if (X <= parent->x()) X = parent->x()+1;
	Y = parent->y()+((parent->h()-H)>>1);
	if (Y < parent->y()+20) Y = parent->y()+20;
      } else {
	X = (sw-W)>>1;
	Y = (sh-H)>>1;
      }
      if (!modal()) {
	static const Window* pp = 0;
	static int delta = 0;
	if (parent != pp) {pp = parent; delta = 0;}
	X += delta;
	Y += delta;
	delta += 16;
      }
      if (X+W >= sw) X = sw-W-1;
      if (X < 1) X = 1;
      if (Y+H >= sh) Y = sh-H-1;
      if (Y < 20) Y = 20;
      // we do not compare to sw/sh here because I think it may mess up
      // some virtual desktop implementations
    }
    root = RootWindow(xdisplay, xscreen);
    attr.event_mask =
      ExposureMask | StructureNotifyMask
      | KeyPressMask | KeyReleaseMask | KeymapStateMask | FocusChangeMask
      | ButtonPressMask | ButtonReleaseMask
      | EnterWindowMask | LeaveWindowMask
      | PointerMotionMask;
    if (window->override()) {
      flush(); // make sure save_under does not undo any drawing!
      attr.override_redirect = 1;
      attr.save_under = 1;
      mask |= CWOverrideRedirect | CWSaveUnder;
    }
  }

  if (background >= 0) {
    attr.background_pixel = background;
    mask |= CWBackPixel;
  }

  CreatedWindow* x = new CreatedWindow;
  x->xid = XCreateWindow(xdisplay,
			 root,
			 X, Y, W, H,
			 0, // borderwidth
			 visual->depth,
			 InputOutput,
			 visual->visual,
			 mask, &attr);

  x->backbuffer = 0;
  x->overlay = false;
  x->window = window; window->i = x;
  x->region = 0;
  x->wait_for_expose = true;
  x->cursor = None;
  x->cursor_for = 0;
  x->next = CreatedWindow::first;
  CreatedWindow::first = x;

  if (!window->parent() && !window->override()) {
    // send all kinds 'o junk to X Window Manager:

    // Set the label:
    window->label(window->label(), window->iconlabel());

    // Makes the close button produce an event:
    XChangeProperty(xdisplay, x->xid, WM_PROTOCOLS,
 		    XA_ATOM, 32, 0, (uchar*)&WM_DELETE_WINDOW, 1);

    // send size limits and border:
    x->sendxjunk();

    // Setting this allows the window manager to use the window's class
    // to look up things like border colors and icons in the xrdb database:
    if (window->xclass()) {
      char buffer[1024];
      char *p; const char *q;
      // truncate on any punctuation, because they break XResource lookup:
      for (p = buffer, q = window->xclass(); isalnum(*q)||(*q&128);) *p++ = *q++;
      *p++ = 0;
      // create the capitalized version:
      q = buffer;
      *p = toupper(*q++); if (*p++ == 'X') *p++ = toupper(*q++);
      while ((*p++ = *q++));
      XChangeProperty(xdisplay, x->xid, XA_WM_CLASS, XA_STRING, 8, 0,
		      (unsigned char *)buffer, p-buffer-1);
    }

    // Send child window information:
#if 0
    // This worked for KDE but Gnome puts the window on the desktop
    if (modal() == window)
      XSetTransientForHint(xdisplay, x->xid, None);
    else
#endif
      if (window->child_of() && window->child_of()->shown())
      XSetTransientForHint(xdisplay, x->xid, window->child_of()->i->xid);

    // Make it receptive to DnD:
    int version = 4;
    XChangeProperty(xdisplay, x->xid, XdndAware,
		    XA_ATOM, sizeof(int)*8, 0, (unsigned char*)&version, 1);

    // Set up the icon and initial icon state:
    XWMHints *hints = XAllocWMHints();
    hints->flags = InputHint;
    hints->input = True; // some window managers require this to be sent?
#if 0
    // not clear if this does anything useful. I have not seen any effect
    // on KDE or Gnome:
    for (const Window* w = window; w; w = w->child_of())
      if (w->shown()) hints->window_group = w->i->xid;
    hints->flags |= WindowGroupHint;
#endif
    if (!fltk::modal() && fl_show_iconic) {
      hints->flags |= StateHint;
      hints->initial_state = IconicState;
      fl_show_iconic = false;
    }
    if (window->icon()) {
      hints->icon_pixmap = (Pixmap)window->icon();
      hints->flags       |= IconPixmapHint;
    }
    XSetWMHints(xdisplay, x->xid, hints);
    XFree(hints);
  }
}

/*! Make an fltk::Window draw into an existing X Window */
CreatedWindow* CreatedWindow::set_xid(Window* window, XWindow winxid) {
  CreatedWindow* x = new CreatedWindow;
  x->xid = winxid;
  x->backbuffer = 0;
  x->window = window; window->i = x;
  x->region = 0;
  x->wait_for_expose = true;
  x->cursor = None;
  x->cursor_for = 0;
  x->next = CreatedWindow::first;
  CreatedWindow::first = x;
  return x;
}

////////////////////////////////////////////////////////////////
// Send X window stuff that can be changed over time:

void CreatedWindow::sendxjunk() {
  if (window->parent() || window->override()) return; // it's not a window manager window!

  XSizeHints hints;
  // memset(&hints, 0, sizeof(hints)); jreiser suggestion to fix purify?
  hints.min_width = window->minw;
  hints.min_height = window->minh;
  hints.max_width = window->maxw;
  hints.max_height = window->maxh;
  hints.width_inc = window->dw;
  hints.height_inc = window->dh;
  hints.win_gravity = StaticGravity;

  // see the file /usr/include/X11/Xm/MwmUtil.h:
  // fill all fields to avoid bugs in kwm and perhaps other window managers:
  // 0, MWM_FUNC_ALL, MWM_DECOR_ALL
  long prop[5] = {0, 1, 1, 0, 0};

  if (hints.min_width != hints.max_width ||
      hints.min_height != hints.max_height) { // resizable
    hints.flags = PMinSize|PWinGravity;
    if (hints.max_width >= hints.min_width ||
	hints.max_height >= hints.min_height) {
      hints.flags = PMinSize|PMaxSize|PWinGravity;
      // unfortunately we can't set just one maximum size.  Guess a
      // value for the other one.  Some window managers will make the
      // window fit on screen when maximized, others will put it off screen:
      if (hints.max_width < hints.min_width)
	hints.max_width = DisplayWidth(xdisplay, xscreen);
      if (hints.max_height < hints.min_height)
	hints.max_height = DisplayHeight(xdisplay, xscreen);
    }
    if (hints.width_inc && hints.height_inc) hints.flags |= PResizeInc;
//     if (window->aspect) {
//       hints.min_aspect.x = hints.max_aspect.x = hints.min_width;
//       hints.min_aspect.y = hints.max_aspect.y = hints.min_height;
//       hints.flags |= PAspect;
//     }
  } else { // not resizable:
    hints.flags = PMinSize|PMaxSize|PWinGravity;
    prop[0] = 1; // MWM_HINTS_FUNCTIONS
    prop[1] = 1|2|16; // MWM_FUNC_ALL | MWM_FUNC_RESIZE | MWM_FUNC_MAXIMIZE
  }

  if (window->x() != USEDEFAULT && window->y() != USEDEFAULT) {
    hints.flags |= USPosition;
    hints.x = window->x();
    hints.y = window->y();
  }

  if (!window->border()) {
    prop[0] |= 2; // MWM_HINTS_DECORATIONS
    prop[2] = 0; // no decorations
  }

  XSetWMNormalHints(xdisplay, xid, &hints);
  XChangeProperty(xdisplay, xid,
		  MOTIF_WM_HINTS, MOTIF_WM_HINTS,
		  32, 0, (unsigned char *)prop, 5);
}

void Window::size_range_() {
  size_range_set = 1;
  if (i) i->sendxjunk();
}

/*! Returns true if the window is currently displayed as an
  icon. Returns false if the window is not shown() or hide() has been
  called.

  <i>On X this will return true in the time between when show() is
  called and when the window manager finally puts the window on the
  screen and causes an expose event.</i>
*/
bool Window::iconic() const {
  return (i && visible() && i->wait_for_expose);
}

////////////////////////////////////////////////////////////////

/*!
  Returns a pointer to after the last slash. If there is no slash
  this returns a pointer to the start of the name.
*/
const char *filename_name(const char *name) {
  const char *p,*q;
  for (p=q=name; *p;) if (*p++ == '/') q = p;
  return q;
}

/*! Sets both the label() and the iconlabel() */
void Window::label(const char *name, const char *iname) {
  Widget::label(name);
  iconlabel_ = iname;
  if (i && !parent()) {
    if (!name) name = "";
    int l = strlen(name);
    //if (is_utf8(name,name+l)>=0)
      XChangeProperty(xdisplay, i->xid, _NET_WM_NAME,
		      UTF8_STRING, 8, 0, (uchar*)name, l);
    XChangeProperty(xdisplay, i->xid, XA_WM_NAME,
		    XA_STRING, 8, 0, (uchar*)name, l);
    if (!iname) iname = filename_name(name);
    l = strlen(iname);
    //if (is_utf8(iname,iname+l)>=0)
      XChangeProperty(xdisplay, i->xid, _NET_WM_ICON_NAME,
		      UTF8_STRING, 8, 0, (uchar*)iname, l);
    XChangeProperty(xdisplay, i->xid, XA_WM_ICON_NAME, 
		    XA_STRING, 8, 0, (uchar*)iname, l);
  }
}

////////////////////////////////////////////////////////////////
// Drawing context

const Window *Window::current_;

/*! Sets things up so that the drawing functions in <fltk/draw.h> will
  go into this window. This is useful for incremental update of
  windows, such as in an idle callback, which will make your program
  behave much better if it draws a slow graphic. <i>This call does not
  work for DoubleBufferWindow!</i> */
void Window::make_current() const {
  current_ = this;
  draw_into(i->xid);
}

/*! \fn static Window* Window::current()
  Returns the last window make_current() was called on. */

XWindow fltk::xwindow;
GC fltk::gc;
#if USE_CAIRO
cairo_t* fltk::cc;
#endif


/*! Fltk can draw into any X window or pixmap that uses the fltk::xvisual.
  Calling this will make all drawing functions go there. Before you destroy
  the window or pixmap you must call fltk::stop_drawing() so that it can
  destroy any temporary structures pointing to it.
  This call also resets the transformation matrix to the identity, even
  if this is the same drawable as before.
*/
void fltk::draw_into(XWindow window) {
  xwindow = window;
  // in X the gc structure can be reused for every window:
  if (!fltk::gc) fltk::gc = XCreateGC(xdisplay, window, 0, 0);
  // The XFT context is not changed here, to avoid wasting time
  // if no text is drawn into this window. It is created when the
  // xft text is drawn the first time.
  load_identity();
#if USE_CAIRO
  // Cairo context created as well:
  if (cc) {
    if (cairo_status(cc)) {
      warning("Cairo: %s", cairo_status_string(cc));
      // reset it somehow?
    }
  } else {
    cc = cairo_create();
    cairo_set_line_width(cc,1);
  }
  cairo_set_target_drawable (cc, xdisplay, window);
#endif
}

/*! \fn void fltk::stop_drawing(XWindow window);
  If you are destroying a window or pixmap that you (may have)
  previous called draw_into() on, you should call this before
  destroying it. This will destroy any temporary stuctures ("gc"s)
  that fltk had to create to draw on it. This will do nothing if
  you have not called draw_into() on this.
*/
#if !USE_XFT
void fltk::stop_drawing(XWindow window) {
  if (xwindow == window) xwindow = 0;
}
#endif

////////////////////////////////////////////////////////////////
// Window update, double buffering, and overlay:

#if USE_XDBE
#define Window XWindow
#include <X11/extensions/Xdbe.h>
#undef Window

static bool use_xdbe; // true if we are using the XDBE extension

static bool can_xdbe() { // figure out use_xdbe.
  static bool tried;
  if (!tried) {
    tried = true;
    int event_base, error_base;
    if (!XdbeQueryExtension(xdisplay, &event_base, &error_base)) return false;
    XWindow root = RootWindow(xdisplay, xscreen);
    int numscreens = 1;
    XdbeScreenVisualInfo *a = XdbeGetVisualInfo(xdisplay, &root, &numscreens);
    if (!a) return false;
    for (int j = 0; j < a->count; j++)
      if (a->visinfo[j].visual == xvisual->visualid
	  /*&& a->visinfo[j].perflevel > 0*/) {use_xdbe = true; break;}
    XdbeFreeVisualInfo(a);
  }
  return use_xdbe;
}

#if 0
// Useless attempt to stop it blinking on resizing. In fact the X design
// just sucks, there is no way to do this when the window manager can
// resize my windo at any time, like right in the middle of when I am drawing!
extern "C" {
  static Bool resize_test(Display*, XEvent* event, char* data) {
    return
      event->type == ConfigureNotify &&
      event->xmapping.window == (XWindow)(data);
  }
}
#endif

#endif

void Window::flush() {
  unsigned char damage = this->damage();

  if (this->double_buffer() || i->overlay) {
    // double-buffer drawing

    bool eraseoverlay = i->overlay || (damage&DAMAGE_OVERLAY);
    if (eraseoverlay) damage &= ~DAMAGE_OVERLAY;

    if (!i->backbuffer) { // we need to create back buffer
#if USE_XDBE
      if (can_xdbe())
	i->backbuffer =
	  XdbeAllocateBackBufferName(xdisplay, i->xid, XdbeUndefined);
      else
#endif
	i->backbuffer = XCreatePixmap(xdisplay,xwindow,w(),h(),xvisual->depth);
      i->backbuffer_bad = true;
    }

    current_ = this;

    // draw the back buffer if it needs anything:
    if (damage
	|| i->backbuffer_bad
#if USE_XDBE
	//	|| (use_xdbe && i->region)
#endif
	) {
      // set the graphics context to draw into back buffer:
      draw_into(i->backbuffer);
      if ((damage & DAMAGE_ALL) || i->backbuffer_bad) {
	set_damage(DAMAGE_ALL);
	draw();
      } else {
      // draw all the changed widgets:
	if (damage & ~DAMAGE_EXPOSE) {
	  set_damage(damage & ~DAMAGE_EXPOSE);
	  draw();
	}
	// draw for any expose events (if Xdbe is not being used this will
	// only happen for redraw(x,y,w,h) calls):
	if (i->region) {
	  clip_region(i->region); i->region = 0;
	  set_damage(DAMAGE_EXPOSE); draw();
	  // hack to only copy the damage region from back to front:
	  if (!(damage & ~DAMAGE_EXPOSE) && !i->overlay) {
	    draw_into(i->xid);
	    goto ALREADY_CLIPPED;
	  }
	  clip_region(0);
	}
      }
#if USE_XDBE
# if 0 // useless attempt to make it not blink on resize
      XSync(xdisplay, false);
      XEvent temp;
      if (XCheckIfEvent(xdisplay, &temp, resize_test, (char*)(i->xid)))
	return;
# endif
      // use the faster Xdbe swap command for all normal redraw():
      if (use_xdbe && !eraseoverlay && (damage&~DAMAGE_EXPOSE)) {
	i->backbuffer_bad = true;
	XdbeSwapInfo s;
	s.swap_window = i->xid;
	s.swap_action = XdbeUndefined;
	XdbeSwapBuffers(xdisplay, &s, 1);
	return;
      }
#endif
    }

    draw_into(i->xid);

    // Clip the copying of the pixmap to the damage area,
    // this makes it faster, especially if the damage area is small:
    if (!eraseoverlay) {
      clip_region(i->region); i->region = 0;
    }

  ALREADY_CLIPPED:
    // Must be an implementation problem in the server, but on Irix (at least)
    // it is much faster if I clip the rectangle requested down:
    int X,Y,W,H; clip_box(0,0,w(),h(),X,Y,W,H);
    // Copy the backbuffer to the window:
    XCopyArea(xdisplay, i->backbuffer, xwindow, gc, X, Y, W, H, X, Y);
    if (i->overlay) draw_overlay();
    clip_region(0);

  }  else {

    // Single buffer drawing
    make_current();
    if (damage & ~DAMAGE_EXPOSE) {
      set_damage(damage & ~DAMAGE_EXPOSE);
      draw();
      i->backbuffer_bad = true;
    }
    if (i->region && !(damage & DAMAGE_ALL)) {
      clip_region(i->region); i->region = 0;
      set_damage(DAMAGE_EXPOSE); draw();
      clip_region(0);
    }
  }  
}

/*! Get rid of extra storage created by drawing when double_buffer() was
  turned on. */
void Window::free_backbuffer() {
  if (!i || !i->backbuffer) return;
  stop_drawing(i->backbuffer);
#if USE_XDBE
  if (use_xdbe) return;
#endif
  XFreePixmap(xdisplay, i->backbuffer);
  i->backbuffer = 0;
}

//
// End of "$Id: Fl_x.cxx,v 1.186 2004/07/15 16:23:20 spitzak Exp $".
//
