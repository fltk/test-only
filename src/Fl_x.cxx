//
// "$Id: Fl_x.cxx,v 1.159 2004/01/19 21:38:41 spitzak Exp $"
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

#define CONSOLIDATE_MOTION 0 // this was 1 in fltk 1.0

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
Atom textplain;
Atom textplain2;
Atom texturilist;
//Atom XdndProxy;
static Atom _NET_WORKAREA;
static Atom _NET_CURRENT_DESKTOP;

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
  atom(	textplain		, "text/plain");
  atom(	textplain2		, "TEXT");
  atom(	texturilist		, "text/uri-list");
  //atom(XdndProxy		, "XdndProxy");
  atom(	_NET_WORKAREA		, "_NET_WORKAREA");
  atom(	_NET_CURRENT_DESKTOP	, "_NET_CURRENT_DESKTOP");
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

#if !USE_COLORMAP
  visual(RGB);
#endif
}

void fltk::close_display() {
  remove_fd(ConnectionNumber(xdisplay));
  XCloseDisplay(xdisplay);
}

////////////////////////////////////////////////////////////////

static bool reload_info = true;

/** \class fltk::Monitor
    Structure describing a monitor (screen).
    Structure describing one of the monitors (screens) connected to
    the system. You can ask for one by position with find(), ask for
    all of them with list(), and ask for a fake one that surrounds all
    of them with all(). You then look in the structure to get information
    like the size and bit depth.
*/

/** Return a "monitor" that surrounds all the monitors.
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

/** Return an array of all Monitors.
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
#if XINERAMA_VERSION > 1
    XRectangle* rects = 0; int count = 0;
    int a=0; int b=0;
    if (XineramaQueryExtension(xdisplay,&a,&b) &&
	XineramaIsActive(xdisplay, message_window))
      XineramaGetData(xdisplay, message_window, &rects, &count);
#else
    XineramaScreenInfo* rects = 0; int count = 0;
    int a=0; int b=0;
    if (XineramaQueryExtension(xdisplay,&a,&b))
      rects = XineramaQueryScreens(xdisplay, &count);
#endif
    if (count > 1 && count < 100) {
      const Monitor& a = all();
      num_monitors = count;
      monitors = new Monitor[count];
      for (int i = 0; i < count; i++) {
	Monitor& m = monitors[i];
	m = a;
#if XINERAMA_VERSION > 1
	int x = rects[i].x;
	int y = rects[i].y;
#else
	int x = rects[i].x_org;
	int y = rects[i].y_org;
#endif
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
#endif
  DONE:;
//      printf("Got %d monitors:\n", num_monitors);
//      for (int i=0; i < num_monitors; i++) {
//        const Monitor& m = monitors[i];
//        printf(" %d %d %d %d, work %d %d %d %d\n",
//  	     m.x(), m.y(), m.w(), m.h(),
//  	     m.work.x(), m.work.y(), m.work.w(), m.work.h());
//      }
  }
  *p = monitors;
  return num_monitors;
}

/** Return a pointer to a Monitor structure describing the monitor
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
  XConvertSelection(xdisplay, property, XA_STRING, property,
		    xid(Window::first()), event_time);
}

////////////////////////////////////////////////////////////////

XEvent fltk::xevent; // the current x event
ulong fltk::event_time; // the last timestamp from an x event

char fl_key_vector[32]; // used by get_key()

// Record event mouse position and state from an XEvent.

static void set_event_xy(bool push) {
#if CONSOLIDATE_MOTION
  send_motion = 0;
#endif
  e_x_root = xevent.xbutton.x_root;
  e_x = xevent.xbutton.x;
  e_y_root = xevent.xbutton.y_root;
  e_y = xevent.xbutton.y;
  e_state = xevent.xbutton.state << 16;
  event_time = xevent.xbutton.time;
#ifdef __sgi
  // get the Win key off PC keyboards:
  if (fl_key_vector[18]&0x18) e_state |= META;
#endif
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

  switch (xevent.type) {

  case KeymapNotify:
    memcpy(fl_key_vector, xevent.xkeymap.key_vector, 32);
    break;

  case MappingNotify:
    XRefreshKeyboardMapping((XMappingEvent*)&xevent.xmapping);
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
      for (int i = 0; ; i++) {
	Atom type = dnd_source_types[i]; if (!type) break;
#if 0 // print what types are being pasted:
	char* x = XGetAtomName(xdisplay, type);
	printf("source type of %s\n",x);
	XFree(x);
#else
	if (type == textplain) {dnd_type = type; break;} // our favorite
	if (type == textplain2) {dnd_type = type; break;} // ok
	if (type == texturilist) dnd_type = type; // ok
#endif
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
    send_motion = window;
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
    xfocus = window;
    if (window) {fix_focus(); return true;}
    break;

  case FocusOut:
    if (window && window == xfocus) {xfocus = 0; fix_focus(); return true;}
    break;

  case KeyPress:
  KEYPRESS: {
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
    static char buffer[21];
    KeySym keysym;
    int len = XLookupString(&(xevent.xkey), buffer, 20, &keysym, 0);
    // Make ctrl+dash produce ^_ like it used to:
    if (xevent.xbutton.state&4 && keysym == '-') buffer[0] = 0x1f;
    // Any keys producing foreign letters produces the bottom 8 bits:
    if (!len && keysym < 0xf00) {buffer[0]=(char)keysym; len = 1;}
    buffer[len] = 0;
    e_text = buffer;
    e_length = len;
    event = KEY;
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
    unsigned keycode = xevent.xkey.keycode;
    // Use the unshifted keysym! This matches the symbols that the Win32
    // version produces. However this will defeat older keyboard layouts
    // that use shifted values for function keys.
    KeySym keysym = XKeycodeToKeysym(xdisplay, keycode, 0);
    if (!keysym) {
      // X did not map this key, return keycode with 0x8000:
      keysym = keycode|0x8000;
#ifdef __sgi
      // You can plug a microsoft keyboard into an Irix box but these
      // keys are not translated.  Make them translate like XFree86 does:
      switch (keycode) {
      case 147: keysym = LeftMetaKey; break;
      case 148: keysym = RightMetaKey; break;
      case 149: keysym = MenuKey; break;
      }
#endif
    } else if (keysym >= 0xff95 && keysym <= 0xff9f) { // XK_KP_*
      // Make all keypad keys act like NumLock is on all the time. This
      // is nicer (imho), but more importantly this gets rid of a range of
      // keysyms that the Win32 version cannot produce. This strange
      // table translates XK_KP_Home to '7', etc:
      keysym = Keypad+"7486293150."[keysym-0xff95];
      e_text[0] = char(keysym) & 0x7F;
      e_text[1] = 0;
      e_length = 1;
    } else if (keysym == 0xffeb) {
      // Newer X servers make the Windoze key return XK_Super_L
      keysym = LeftMetaKey;
    } else if (keysym == 0xffec) {
      // Newer X servers make the Windows key return XK_Super_R
      keysym = RightMetaKey;
//  } else if (keysym == 0xff20) {
//    // Some older ones made it return XK_Multi_key
//    keysym = RightMetaKey;
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
    if (e.target == TARGETS) {
      Atom a = XA_STRING;
      XChangeProperty(xdisplay, e.requestor, e.property,
		      XA_ATOM, sizeof(Atom)*8, 0, (unsigned char*)&a,
		      sizeof(Atom));
    } else if (/*e.target == XA_STRING &&*/ selection_length[clipboard]) {
      XChangeProperty(xdisplay, e.requestor, e.property,
		      e.target, 8, 0,
		      (unsigned char *)selection_buffer[clipboard],
		      selection_length[clipboard]);
    } else {
//    char* x = XGetAtomName(xdisplay,e.target);
//    fprintf(stderr,"selection request of %s\n",x);
//    XFree(x);
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
  Group::layout();
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

  x->backbuffer.xid = 0;
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
  x->backbuffer.xid = 0;
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

/**
 * returns pointer to the filename, or "" if name ends with '/'
 */
const char *filename_name(const char *name) {
  const char *p,*q;
  for (p=q=name; *p;) if (*p++ == '/') q = p;
  return q;
}

void Window::label(const char *name, const char *iname) {
  Widget::label(name);
  iconlabel_ = iname;
  if (i && !parent()) {
    if (!name) name = "";
    XChangeProperty(xdisplay, i->xid, XA_WM_NAME,
		    XA_STRING, 8, 0, (uchar*)name, strlen(name));
    if (!iname) iname = filename_name(name);
    XChangeProperty(xdisplay, i->xid, XA_WM_ICON_NAME, 
		    XA_STRING, 8, 0, (uchar*)iname, strlen(iname));
  }
}

////////////////////////////////////////////////////////////////
// Drawing context

fltk::Drawable* fltk::drawable;
XWindow fltk::xwindow;
const Window *Window::current_;
GC fltk::gc;

/*! Sets things up so that the drawing functions in <fltk/draw.h> will
  go into this window. This is useful for incremental update of
  windows, such as in an idle callback, which will make your program
  behave much better if it draws a slow graphic. <i>This call does not
  work for DoubleBufferWindow!</i> */
void Window::make_current() const {
  current_ = this;
  i->make_current();
  static GC normalgc;	// the GC used by all X windows with the default visual
  if (!normalgc) normalgc = XCreateGC(xdisplay, i->xid, 0, 0);
  fltk::gc = normalgc;
}

/*! \fn static Window* Window::current()
  Returns the last window make_current() was called on. */

////////////////////////////////////////////////////////////////

/*! \fn fltk::system_theme()
  Sets all the NamedStyle structures to correct values for information
  read from the operating system.

  On X this used to read the xrdb database for colors like "background".
  As it appears nobody is using this, this is disabled and the current
  version does nothing.

  On Windows it uses GetSysColor and SystemParametersInfo to set many
  colors and fonts.
*/

// I commented out the entire XRDB stuff, use this to get it back:
#if USE_XRDB

// Set this to 1 to get my attempt to improve XGetDefault:
#define MY_GET_DEFAULT 0

#if MY_GET_DEFAULT

// Simplified resource search that understands periods in the names.
// Matches class.a:, class*a:, *.a:, *a:, and a:
// Ignores case on everything.
// Strips whitespace from both sides of the value
// An empty value returns null.
// Also understands = instead of :
// Comments start with ! or ;
// No quoting of any kind on the values!

// case independent compare, r advanced to point after match:
static int match(char* & rr, const char* a) {
  for (char* r = rr; ; a++, r++) {
    if (!*a) {rr = r; return 1;}
    if (tolower(*r) != tolower(*a)) return 0;
  }
}

// return true if this character is the end of line:
static int iseol(char c) {return !c || c=='\n' || c=='!' ||c==';';}

// like strtok, this mangles the string temporarily using these:
static char* resourcestring;
static char* clobbered;
static char clobbered_value;

static const char* get_default(const char* a) {
  if (clobbered) {*clobbered = clobbered_value; clobbered = 0;}
  if (!resourcestring) resourcestring = XResourceManagerString(xdisplay);
  char* r = resourcestring;
  char* found = 0;
  for (;;) { // for each line in r
    // skip leading whitespace (and also all blank lines):
    while (isspace(*r)) r++;
    if (!*r) break;
    // exact is true if the program name is matched:
    int exact = 0;
    if (*r == '*') {
      // checked for * or *.:
      r++;
      if (*r == '.') r++;
    } else if (match(r, Window::xclass()) && (*r == '.' || *r == '*')) {
      // matched the program name:
      r++;
      exact = 1;
    } // otherwise try just 'a':
    if (match(r, a)) {
      while (*r != '\n' && isspace(*r)) r++;
      if (*r == ':' || *r == '=') {
	r++;
	while (*r != '\n' && isspace(*r)) r++;
	if (iseol(*r)) { // blank attribute
	  if (exact) return 0;
	  goto SKIP;
	}
	// find the end of the word (point to first whitespace at eol or \n):
	char* e = r; while (!iseol(*e)) e++;
	while (e > r && isspace(*(e-1))) e--;
	// remove anything for previous find
	if (clobbered) *clobbered = clobbered_value;
	// replace the end with a null, which we will put back later:
	clobbered = e;
	clobbered_value = *e;
	*e = 0;
	if (exact || !clobbered_value) return r;
	found = r; r = e+1;
      } else {
	// check for blank attribute name with no : or = sign:
	if (exact && iseol(*r)) return 0;
      }
    }
  SKIP:
    while (*r && *r++ != '\n'); // go to next line
  }
  return found;
}

#else

static inline const char* get_default(const char* a) {
  return XGetDefault(xdisplay, Window::xclass(), a);
}

static inline const char* get_default(const char* a, const char* b) {
  return XGetDefault(xdisplay, a, b);
}

#endif

static Color to_color(const char* p) {
  return p ? color(p) : 0;
}

bool fltk::system_theme() {
  open_display();

  Color color;

  color = to_color(get_default("background"));
  if (color) set_background(color);

  color = to_color(get_default("foreground"));
  if (color) {
    Widget::default_style->labelcolor(color);
    Widget::default_style->highlight_textcolor(color);
  }

#if MY_GET_DEFAULT
  color = to_color(get_default("Text.background"));
  if (color) Widget::default_style->color(color);

  color = to_color(get_default("Text.foreground"));
  if (color) Widget::default_style->textcolor(color);

  color = to_color(get_default("Text.selectBackground"));
  if (color) Widget::default_style->selection_color(color);

  color = to_color(get_default("Text.selectForeground"));
  if (color) Widget::default_style->selection_textcolor(color);
#else
  color = to_color(get_default("Text","background"));
  if (color) Widget::default_style->color(color);

  color = to_color(get_default("Text","foreground"));
  if (color) Widget::default_style->textcolor(color);

  color = to_color(get_default("Text","selectBackground"));
  if (color) Widget::default_style->selection_color(color);

  color = to_color(get_default("Text","selectForeground"));
  if (color) Widget::default_style->selection_textcolor(color);
#endif

  // also Scrollbar,width
  // does not appear to be anything there for setting the tooltips...
  // Maybe I should just add Tooltip,back/foreground.

  // Mousewheel stuff is read from the XDefaults database but there
  // are no standards for these. We should change these to match any
  // standards that arise:

  const char* w = get_default("wheel_scroll_lines");
  if (w) Widget::default_style->wheel_scroll_lines(atoi(w));

  w = get_default("wheel_up_button");
  if (w) wheel_up_button = atoi(w);

  w = get_default("wheel_down_button");
  if (w) wheel_down_button = atoi(w);

#if MY_GET_DEFAULT
  // undo my mangling of the x resource string:
  if (clobbered) {*clobbered = clobbered_value; clobbered = 0;}
#endif

  return true;
}
#else

// non-xrdb version of system_theme():
bool fltk::system_theme() {return true;}

#endif

//
// End of "$Id: Fl_x.cxx,v 1.159 2004/01/19 21:38:41 spitzak Exp $".
//
