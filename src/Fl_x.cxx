//
// "$Id: Fl_x.cxx,v 1.132 2002/09/02 06:33:47 spitzak Exp $"
//
// X specific code for the Fast Light Tool Kit (FLTK).
// This file is #included by Fl.cxx
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

#define CONSOLIDATE_MOTION 0 // this was 1 in fltk 1.0

#include <config.h>
#include <fltk/Fl.h>
#include <fltk/x.h>
#include <fltk/Fl_Window.h>
#include <fltk/Fl_Style.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>

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

void Fl::add_fd(int n, int events, void (*cb)(int, void*), void *v) {
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

void Fl::add_fd(int fd, void (*cb)(int, void*), void* v) {
  Fl::add_fd(fd, POLLIN, cb, v);
}

void Fl::remove_fd(int n, int events) {
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
static Fl_Window* send_motion;
#endif
static void do_queued_events(int, void*) {
  while (!Fl::exit_modal_ && XEventsQueued(fl_display,QueuedAfterReading)) {
    XNextEvent(fl_display, &fl_xevent);
    fl_handle();
  }
  // we send FL_LEAVE only if the mouse did not enter some other window:
  if (!xmousewin) Fl::handle(FL_LEAVE, 0);
#if CONSOLIDATE_MOTION
  else if (send_motion == xmousewin) {
    send_motion = 0;
    Fl::handle(FL_MOVE, xmousewin);
  }
#endif
}

// these pointers are set by the Fl::lock() function:
static void nothing() {}
void (*fl_lock_function)() = nothing;
void (*fl_unlock_function)() = nothing;

// Wait up to the given time for any events or sockets to become ready,
// do the callbacks for the events and sockets:
static inline int fl_wait(double time_to_wait) {

  // OpenGL and other broken libraries call XEventsQueued()
  // and thus cause the file descriptor to not be ready,
  // so we must check for already-read events:
  if (fl_display && XQLength(fl_display)) {do_queued_events(0,0); return 1;}

#if !USE_POLL
  fd_set fdt[3];
  fdt[0] = fdsets[0];
  fdt[1] = fdsets[1];
  fdt[2] = fdsets[2];
#endif

  fl_unlock_function();
#if USE_POLL
  int n = ::poll(pollfds, nfds,
		 (time_to_wait<2147483.648) ? int(time_to_wait*1000+.5) : -1);
#else
  int n;
  if (time_to_wait < 2147483.648) {
    timeval t;
    t.tv_sec = int(time_to_wait);
    t.tv_usec = int(1000000 * (time_to_wait-t.tv_sec));
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

// fl_ready() is just like fl_wait(0.0) except no callbacks are done:
static inline int fl_ready() {
  if (XQLength(fl_display)) return 1;
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

Display *fl_display = 0;
Window fl_message_window;
int fl_screen;
XVisualInfo *fl_visual;
Colormap fl_colormap;

static Atom WM_DELETE_WINDOW;
static Atom WM_PROTOCOLS;
       Atom fl_MOTIF_WM_HINTS;
static Atom FLTKChangeScheme;
static Atom TARGETS;
static Atom CLIPBOARD;
Atom fl_XdndAware;
Atom fl_XdndSelection;
Atom fl_XdndEnter;
Atom fl_XdndTypeList;
Atom fl_XdndPosition;
Atom fl_XdndLeave;
Atom fl_XdndDrop;
Atom fl_XdndStatus;
Atom fl_XdndActionCopy;
Atom fl_XdndFinished;
Atom fl_textplain;
Atom fl_texturilist;
//Atom fl_XdndProxy;

extern "C" {
static int io_error_handler(Display*) {Fl::fatal("X I/O error"); return 0;}

static int xerror_handler(Display* d, XErrorEvent* e) {
  char buf1[128], buf2[128];
  sprintf(buf1, "XRequest.%d", e->request_code);
  XGetErrorDatabaseText(d,"",buf1,buf1,buf2,128);
  XGetErrorText(d, e->error_code, buf1, 128);
  Fl::warning("%s: %s 0x%lx", buf2, buf1, e->resourceid);
  return 0;
}
}

void fl_open_display() {
  if (fl_display) return;

  XSetIOErrorHandler(io_error_handler);
  XSetErrorHandler(xerror_handler);

  Display *d = XOpenDisplay(0);
  if (!d) Fl::fatal("Can't open display \"%s\"",XDisplayName(0));

  fl_open_display(d);
}

void fl_open_display(Display* d) {
  fl_display = d;
  Fl::add_fd(ConnectionNumber(d), POLLIN, do_queued_events);

  WM_DELETE_WINDOW      = XInternAtom(d, "WM_DELETE_WINDOW",	0);
  WM_PROTOCOLS          = XInternAtom(d, "WM_PROTOCOLS",	0);
  fl_MOTIF_WM_HINTS     = XInternAtom(d, "_MOTIF_WM_HINTS",	0);
  FLTKChangeScheme      = XInternAtom(d, "FLTKChangeScheme",	0);
  TARGETS               = XInternAtom(d, "TARGETS",		0);
  CLIPBOARD		= XInternAtom(d, "CLIPBOARD",		0);
  fl_XdndAware          = XInternAtom(d, "XdndAware",		0);
  fl_XdndSelection      = XInternAtom(d, "XdndSelection",	0);
  fl_XdndEnter          = XInternAtom(d, "XdndEnter",		0);
  fl_XdndTypeList       = XInternAtom(d, "XdndTypeList",	0);
  fl_XdndPosition       = XInternAtom(d, "XdndPosition",	0);
  fl_XdndLeave          = XInternAtom(d, "XdndLeave",		0);
  fl_XdndDrop           = XInternAtom(d, "XdndDrop",		0);
  fl_XdndStatus         = XInternAtom(d, "XdndStatus",		0);
  fl_XdndActionCopy     = XInternAtom(d, "XdndActionCopy",	0);
  fl_XdndFinished       = XInternAtom(d, "XdndFinished",	0);
  fl_textplain	      	= XInternAtom(d, "text/plain",		0);
  fl_texturilist      	= XInternAtom(d, "text/uri-list",	0);
  //fl_XdndProxy        = XInternAtom(d, "XdndProxy",		0);

  fl_screen = DefaultScreen(d);

  // Carl inserted something much like the KDE plugin does to register
  // a style client message.  I would prefer to either leave this up
  // to the plugin, or use the SAME atoms as KDE (to avoid even more
  // namespace pollution).  See the kde plugin for sample code.

  // I don't think an X window with the name "FLTK_STYLE_WINDOW" pollutes
  // the namespace very badly.  We could use the same atoms as KDE, but then
  // FLTK windows would redraw every time we changed the KDE scheme, and
  // KDE windows would redraw every time we changed the FLTK scheme which
  // to me would be unacceptable.  Moreover, this would make FLTK dependant
  // on a particular KDE version; different versions of KDE (currently just
  // v1 & v2) handle this in different ways.  We would need put logic in
  // FLTK to figure out which version of KDE is running and do it in that
  // way, and that would break again if the KDE folks changed methods again.
  // It doesn't hurt anything and saves code needed for KDE compatibility
  // to just do it our own way.  Finally, it just doesn't seem "right" for
  // FLTK applications to advertise themselves as KDE applications when in
  // fact they are not.  That could potentially break KDE as it waits for
  // a FLTK application to do some KDE thing that it doesn't know how to
  // respond to.  That's what the KDE theme is for!
  //
  // CET

  // WAS: ok.  I made it public so the window id can be used for grab, etc.
  // We may want the atom to be FLTK_MESSAGE_WINDOW, and change the message
  // to be FLTKChangeStyle so that the names match kde more closely.

  fl_message_window =
    XCreateSimpleWindow(d, RootWindow(d,fl_screen), 0,0,1,1,0, 0, 0);
  Atom style_atom = XInternAtom(d, "FLTK_STYLE_WINDOW", False);
  long data = 1;
  XChangeProperty(d, fl_message_window, style_atom, style_atom, 32,
                  PropModeReplace, (unsigned char *)&data, 1);

  // construct an XVisualInfo that matches the default Visual:
  XVisualInfo templt; int num;
  templt.visualid = XVisualIDFromVisual(DefaultVisual(d, fl_screen));
  fl_visual = XGetVisualInfo(d, VisualIDMask, &templt, &num);
  fl_colormap = DefaultColormap(d, fl_screen);

#if !USE_COLORMAP
  Fl::visual(FL_RGB);
#endif
}

void fl_close_display() {
  Fl::remove_fd(ConnectionNumber(fl_display));
  XCloseDisplay(fl_display);
}

static bool reload_info = true;

const Fl_Screen_Info& Fl::info() {
  static Fl_Screen_Info info;
  if (reload_info) {
    reload_info = false;
    fl_open_display();

    // WAS: this should do something on multihead systems to match Windoze!
    // I don't have multihead versions of either X or Windows to test...
    info.x = 0;
    info.y = 0;
    info.width = DisplayWidth(fl_display,fl_screen);
    info.height = DisplayHeight(fl_display,fl_screen);
    info.w = info.width;
    info.h = info.height;

    // do any screens really return 0 for MM?
    info.width_mm = DisplayWidthMM(fl_display, fl_screen);
    if (!info.width_mm) info.width_mm = int(info.width*25.4/75);
    //info.dpi_x = info.width*25.4/info.width_mm;
    info.height_mm = DisplayHeightMM(fl_display, fl_screen);
    if (!info.height_mm) info.height_mm = int(info.height*25.4/75);
    // info.dpi_y = info.height*25.4/info.height_mm;
  }
  return info;
}

void Fl::get_mouse(int &x, int &y) {
  fl_open_display();
  Window root = RootWindow(fl_display, fl_screen);
  Window c; int mx,my,cx,cy; unsigned int mask;
  XQueryPointer(fl_display,root,&root,&c,&mx,&my,&cx,&cy,&mask);
  x = mx;
  y = my;
}

////////////////////////////////////////////////////////////////

Window fl_dnd_source_window;
Atom *fl_dnd_source_types; // null-terminated list of data types being supplied
Atom *fl_incoming_dnd_source_types;
Atom fl_dnd_type;
Atom fl_dnd_source_action;
Atom fl_dnd_action;

void fl_sendClientMessage(Window window, Atom message,
                                 unsigned long d0,
                                 unsigned long d1=0,
                                 unsigned long d2=0,
                                 unsigned long d3=0,
                                 unsigned long d4=0)
{
  XEvent e;
  e.xany.type = ClientMessage;
  e.xany.window = window;
  e.xclient.message_type = message;
  e.xclient.format = 32;
  e.xclient.data.l[0] = (long)d0;
  e.xclient.data.l[1] = (long)d1;
  e.xclient.data.l[2] = (long)d2;
  e.xclient.data.l[3] = (long)d3;
  e.xclient.data.l[4] = (long)d4;
  XSendEvent(fl_display, window, 0, 0, &e);
}

////////////////////////////////////////////////////////////////
// Code used for cut & paste

static Fl_Widget *selection_requestor;
static char *selection_buffer[2];
static int selection_length[2];
static int selection_buffer_length[2];
bool fl_i_own_selection[2];

void Fl::copy(const char *stuff, int len, bool clipboard) {
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
  XSetSelectionOwner(fl_display, property, fl_message_window, fl_event_time);
}

// Call this when a "paste" operation happens:
void Fl::paste(Fl_Widget &receiver, bool clipboard) {
  if (fl_i_own_selection[clipboard]) {
    // We already have it, do it quickly without window server.
    // Notice that the text is clobbered if set_selection is
    // called in response to FL_PASTE!
    Fl::e_text = selection_buffer[clipboard];
    Fl::e_length = selection_length[clipboard];
    receiver.handle(FL_PASTE);
    return;
  }
  // otherwise get the window server to return it:
  selection_requestor = &receiver;
  Atom property = clipboard ? CLIPBOARD : XA_PRIMARY;
  XConvertSelection(fl_display, property, XA_STRING, property,
		    fl_xid(Fl::first_window()), fl_event_time);
}

////////////////////////////////////////////////////////////////

XEvent fl_xevent; // the current x event
ulong fl_event_time; // the last timestamp from an x event

char fl_key_vector[32]; // used by Fl::get_key()

// Record event mouse position and state from an XEvent:

static void set_event_xy(bool push) {
#if CONSOLIDATE_MOTION
  send_motion = 0;
#endif
  Fl::e_x_root = fl_xevent.xbutton.x_root;
  Fl::e_x = fl_xevent.xbutton.x;
  Fl::e_y_root = fl_xevent.xbutton.y_root;
  Fl::e_y = fl_xevent.xbutton.y;
  Fl::e_state = fl_xevent.xbutton.state << 16;
  fl_event_time = fl_xevent.xbutton.time;
#ifdef __sgi
  // get the Win key off PC keyboards:
  if (fl_key_vector[18]&0x18) Fl::e_state |= FL_WIN;
#endif
  // turn off is_click if enough time or mouse movement has passed:
  static int px, py;
  static ulong ptime;
  if (abs(Fl::e_x_root-px)+abs(Fl::e_y_root-py) > 3 
      || fl_event_time >= ptime+(push?1000:200))
    Fl::e_is_click = 0;
  if (push) {
    px = Fl::e_x_root;
    py = Fl::e_y_root;
    ptime = fl_event_time;
  }
}

////////////////////////////////////////////////////////////////

static Fl_Window* resize_from_system;

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

bool fl_handle()
{
  Fl_Window* window = fl_find(fl_xevent.xany.window);
  int event = 0;

  switch (fl_xevent.type) {

  case KeymapNotify:
    memcpy(fl_key_vector, fl_xevent.xkeymap.key_vector, 32);
    break;

  case MappingNotify:
    XRefreshKeyboardMapping((XMappingEvent*)&fl_xevent.xmapping);
    break;

  case ClientMessage: {
    Atom message = fl_xevent.xclient.message_type;
    const long* data = fl_xevent.xclient.data.l;

    if (window && (Atom)(data[0]) == WM_DELETE_WINDOW) {
      if (!Fl::grab() && !(Fl::modal() && window != Fl::modal()))
	window->do_callback();
      return true;

    } else if (message == FLTKChangeScheme) {
      Fl_Style::reload_theme();
      return true;

    } else if (message == fl_XdndEnter) {
      xmousewin = window;
      fl_dnd_source_window = data[0];
      // version number is data[1]>>24
      if (data[1]&1) {
	// get list of data types:
	Atom actual; int format; unsigned long count, remaining;
	unsigned char *buffer = 0;
	XGetWindowProperty(fl_display, fl_dnd_source_window, fl_XdndTypeList,
			   0, 0x8000000L, False, XA_ATOM, &actual, &format,
			   &count, &remaining, &buffer);
	if (actual != XA_ATOM || format != 32 || count<4 || !buffer)
	  goto FAILED;
	delete [] fl_incoming_dnd_source_types;
	fl_incoming_dnd_source_types = new Atom[count+1];
	fl_dnd_source_types = fl_incoming_dnd_source_types;
	for (unsigned i = 0; i < count; i++)
	  fl_dnd_source_types[i] = ((Atom*)buffer)[i];
	fl_dnd_source_types[count] = 0;
      } else {
      FAILED:
	// less than four data types, or if the above messes up:
	if (!fl_incoming_dnd_source_types)
	  fl_incoming_dnd_source_types = new Atom[4];
	fl_dnd_source_types = fl_incoming_dnd_source_types;
	fl_dnd_source_types[0] = data[2];
	fl_dnd_source_types[1] = data[3];
	fl_dnd_source_types[2] = data[4];
	fl_dnd_source_types[3] = 0;
      }
      // This should return one of the fl_dnd_source_types. Unfortunately
      // no way to just force it to cough up whatever data is "most text-like"
      // instead I have to select from a list of known types. We may need
      // to add to this list in the future, turn on the #if to print the
      // types if you get a drop that you think should work.
      fl_dnd_type = fl_textplain; // try this if no matches, it may work
      for (int i = 0; ; i++) {
	Atom type = fl_dnd_source_types[i]; if (!type) break;
#if 0 // print what types are being pasted:
	char* x = XGetAtomName(fl_display, type);
	printf("source type of %s\n",x);
	XFree(x);
#else
	if (type == fl_textplain) {fl_dnd_type = type; break;} // our favorite
	if (type == fl_texturilist) fl_dnd_type = type; // ok
#endif
      }
      event = FL_DND_ENTER;
      break;

    } else if (message == fl_XdndPosition) {
      xmousewin = window;
      fl_dnd_source_window = data[0];
      Fl::e_x_root = data[2]>>16;
      Fl::e_y_root = data[2]&0xFFFF;
      if (window) {
	Fl::e_x = Fl::e_x_root-window->x();
	Fl::e_y = Fl::e_y_root-window->y();
      }
      fl_event_time = data[3];
      fl_dnd_source_action = data[4];
      fl_dnd_action = fl_XdndActionCopy;
      int accept = Fl::handle(FL_DND_DRAG, window);
      fl_sendClientMessage(data[0], fl_XdndStatus,
                           fl_xevent.xclient.window,
                           accept ? 1 : 0,
                           0, // used for xy rectangle to not send position inside
                           0, // used for width+height of rectangle
                           accept ? fl_dnd_action : None);
      return true;

    } else if (message == fl_XdndLeave) {
      fl_dnd_source_window = 0; // don't send a finished message to it
      event = FL_DND_LEAVE;
      break;

    } else if (message == fl_XdndDrop) {
      xmousewin = window;
      fl_dnd_source_window = data[0];
      fl_event_time = data[2];
      Window to_window = fl_xevent.xclient.window;
      if (Fl::handle(FL_DND_RELEASE, window)) {
	selection_requestor = Fl::belowmouse();
	XConvertSelection(fl_display, fl_XdndSelection,
			  fl_dnd_type, XA_SECONDARY,
			  to_window, fl_event_time);
      } else {
	// Send the finished message if I refuse the drop.
	// It is not clear whether I can just send finished always,
	// or if I have to wait for the SelectionNotify event as the
	// code is currently doing.
	fl_sendClientMessage(fl_dnd_source_window, fl_XdndFinished, to_window);
	fl_dnd_source_window = 0;
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
    window = fl_find(fl_xevent.xmapping.window);
    if (!window) break;
    if (window->parent()) break; // ignore child windows

    // figure out where OS really put window
    XWindowAttributes actual;
    XGetWindowAttributes(fl_display, fl_xid(window), &actual);
    Window cr; int X, Y, W = actual.width, H = actual.height;
    XTranslateCoordinates(fl_display, fl_xid(window), actual.root,
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
    // tell Fl_Window about it and set flag to prevent echoing:
    if (window->resize(X, Y, W, H)) resize_from_system = window;
    break; // allow add_handler to do something too
  }

  case UnmapNotify:
    window = fl_find(fl_xevent.xmapping.window);
    if (!window) break;
    if (window->parent()) break; // ignore child windows
    // turning this flag makes iconic() return true:
    Fl_X::i(window)->wait_for_expose = true;
    break; // allow add_handler to do something too

  case Expose:
  case GraphicsExpose:
    if (!window) break;
    // If this window completely fills it's parent, parent will not get
    // an expose event and the wait flag will not turn off. So force this:
    if (Fl_X::i(window)->wait_for_expose) {
      for (Fl_Window* w = window;;) {
	Fl_X::i(w)->wait_for_expose = false;
	w = w->window();
	if (!w) break;
      }
    }
    Fl_X::i(window)->expose(fl_xevent.xexpose.x, fl_xevent.xexpose.y,
			    fl_xevent.xexpose.width, fl_xevent.xexpose.height);
    return true;

  case ButtonPress: {
    unsigned n = fl_xevent.xbutton.button;
    Fl::e_keysym = FL_Button(n);
    set_event_xy(true);
    // turn off is_click if enough time or mouse movement has passed:
    if (Fl::e_is_click == Fl::e_keysym) {
      Fl::e_clicks++;
    } else {
      Fl::e_clicks = 0;
      Fl::e_is_click = Fl::e_keysym;
    }
    if (n == wheel_up_button) {
      Fl::e_dy = +1;
      event = FL_MOUSEWHEEL;
    } else if (n == wheel_down_button) {
      Fl::e_dy = -1;
      event = FL_MOUSEWHEEL;
    } else {
      Fl::e_state |= (FL_BUTTON1 << (n-1));
      event = FL_PUSH;
    }}
    goto J1;

  case MotionNotify:
    set_event_xy(false);
#if CONSOLIDATE_MOTION
    send_motion = window;
    return false;
#else
    event = FL_MOVE;
    goto J1;
#endif

  case ButtonRelease: {
    unsigned n = fl_xevent.xbutton.button;
    Fl::e_keysym = FL_Button(n);
    set_event_xy(false);
    //if (n == wheel_up_button || n == wheel_down_button) break;
    Fl::e_state &= ~(FL_BUTTON1 << (n-1));
    event = FL_RELEASE;}
    goto J1;

  case EnterNotify:
    if (fl_xevent.xcrossing.detail == NotifyInferior) break;
    // XInstallColormap(fl_display, Fl_X::i(window)->colormap);
    set_event_xy(false);
    Fl::e_state = fl_xevent.xcrossing.state << 16;
    event = FL_ENTER;
  J1:
    xmousewin = window;
    // send a mouse event, with cruft so the grab around modal things works:
    if (Fl::grab_) {
      Fl::handle(event, window);
      if (Fl::grab_ && !Fl::exit_modal_)
	XAllowEvents(fl_display, SyncPointer, CurrentTime);
      return true;
    }
    break;

  case LeaveNotify:
    if (fl_xevent.xcrossing.detail == NotifyInferior) break;
    set_event_xy(false);
    Fl::e_state = fl_xevent.xcrossing.state << 16;
    xmousewin = 0;
    break;

  case FocusIn:
    xfocus = window;
    if (window) {fl_fix_focus(); return true;}
    break;

  case FocusOut:
    if (window && window == xfocus) {xfocus = 0; fl_fix_focus(); return true;}
    break;

  case KeyPress:
  case KeyRelease: {
  KEYPRESS:
    //if (Fl::grab_) XAllowEvents(fl_display, SyncKeyboard, CurrentTime);
    unsigned keycode = fl_xevent.xkey.keycode;
    static unsigned lastkeycode;
    if (fl_xevent.type == KeyPress) {
      event = FL_KEY;
      fl_key_vector[keycode/8] |= (1 << (keycode%8));
      // Make repeating keys increment the click counter:
      if (keycode == lastkeycode) {
	Fl::e_clicks++;
	Fl::e_is_click = 0;
      } else {
	Fl::e_clicks = 0;
	Fl::e_is_click = 1;
	lastkeycode = keycode;
      }
      static char buffer[21];
      KeySym keysym;
      int len = XLookupString(&(fl_xevent.xkey), buffer, 20, &keysym, 0);
      // Make ctrl+dash produce ^_ like it used to:
      if (fl_xevent.xbutton.state&4 && keysym == '-') buffer[0] = 0x1f;
      // Any keys producing foreign letters produces the bottom 8 bits:
      if (!len && keysym < 0xf00) {buffer[0]=(char)keysym; len = 1;}
      buffer[len] = 0;
      Fl::e_text = buffer;
      Fl::e_length = len;
    } else {
      // Stupid X sends fake key-up events when a repeating key is held
      // down, probably due to some back compatability problem. Fortunatley
      // we can detect this because the repeating KeyPress event is in
      // the queue, get it and execute it instead:
      XEvent temp;
      if (XCheckIfEvent(fl_display,&temp,fake_keyup_test,(char*)(&fl_xevent))){
	fl_xevent = temp;
	goto KEYPRESS;
      }
      event = FL_KEYUP;
      fl_key_vector[keycode/8] &= ~(1 << (keycode%8));
      // event_is_click is left on if they press & release the key quickly:
      Fl::e_is_click = (keycode == lastkeycode);
      // make next keypress not be a repeating one:
      lastkeycode = 0;
    }
    // Use the unshifted keysym! This matches the symbols that the Win32
    // version produces. However this will defeat older keyboard layouts
    // that use shifted values for function keys.
    KeySym keysym = XKeycodeToKeysym(fl_display, keycode, 0);
    if (keysym >= 0xff95 && keysym <= 0xff9f) { // XK_KP_*
      // Make all keypad keys act like NumLock is on all the time. This
      // is nicer (imho), but more importantly this gets rid of a range of
      // keysyms that the Win32 version cannot produce. This strange
      // table translates XK_KP_Home to '7', etc:
      keysym = FL_KP("7486293150."[keysym-0xff95]);
      Fl::e_text[0] = char(keysym) & 0x7F;
      Fl::e_text[1] = 0;
      Fl::e_length = 1;
    } else if (keysym == 0xffe7) { // XK_Meta_L
      // old versions of XFree86 used XK_Meta for the "windows" key
      keysym = FL_Win_L;
    } else if (keysym == 0xffe8) { // XK_Meta_R
      keysym = FL_Win_R;
    } else if (!keysym) { // X did not map this key
      keysym = keycode|0x8000;
#ifdef __sgi
      // You can plug a microsoft keyboard into an Irix box but these
      // keys are not translated.  Make them translate like XFree86 does:
      switch (keycode) {
      case 147: keysym = FL_Win_L; break;
      case 148: keysym = FL_Win_R; break;
      case 149: keysym = FL_Menu; break;
      }
#endif
    }
    Fl::e_keysym = int(keysym);
    set_event_xy(true);
    break;}

  case SelectionNotify: {
    if (!selection_requestor) return false;
    static unsigned char* buffer;
    if (buffer) {XFree(buffer); buffer = 0;}
    long read = 0;
    if (fl_xevent.xselection.property) for (;;) {
      // The Xdnd code pastes 64K chunks together, possibly to avoid
      // bugs in X servers, or maybe to avoid an extra round-trip to
      // get the property length.  I copy this here:
      Atom actual; int format; unsigned long count, remaining;
      unsigned char* portion;
      if (XGetWindowProperty(fl_display,
			     fl_xevent.xselection.requestor,
			     fl_xevent.xselection.property,
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
    Fl::e_text = (char*)buffer;
    Fl::e_length = read;
    selection_requestor->handle(FL_PASTE);
    // Detect if this paste is due to Xdnd by the property name (I use
    // XA_SECONDARY for that) and send an XdndFinished message. It is not
    // clear if this has to be delayed until now or if it can be done
    // immediatly after calling XConvertSelection.
    if (fl_xevent.xselection.property == XA_SECONDARY &&
	fl_dnd_source_window) {
      fl_sendClientMessage(fl_dnd_source_window, fl_XdndFinished,
                           fl_xevent.xselection.requestor);
      fl_dnd_source_window = 0; // don't send a second time
    }
    return true;}

  case SelectionClear: {
    bool clipboard = fl_xevent.xselectionclear.selection == CLIPBOARD;
    fl_i_own_selection[clipboard] = false;
    return true;}

  case SelectionRequest: {
    XSelectionEvent e;
    e.type = SelectionNotify;
    e.requestor = fl_xevent.xselectionrequest.requestor;
    e.selection = fl_xevent.xselectionrequest.selection;
    bool clipboard = e.selection == CLIPBOARD;
    e.target = fl_xevent.xselectionrequest.target;
    e.time = fl_xevent.xselectionrequest.time;
    e.property = fl_xevent.xselectionrequest.property;
    if (e.target == TARGETS) {
      Atom a = XA_STRING;
      XChangeProperty(fl_display, e.requestor, e.property,
		      XA_ATOM, sizeof(Atom)*8, 0, (unsigned char*)&a,
		      sizeof(Atom));
    } else if (/*e.target == XA_STRING &&*/ selection_length[clipboard]) {
      XChangeProperty(fl_display, e.requestor, e.property,
		      e.target, 8, 0,
		      (unsigned char *)selection_buffer[clipboard],
		      selection_length[clipboard]);
    } else {
//    char* x = XGetAtomName(fl_display,e.target);
//    fprintf(stderr,"selection request of %s\n",x);
//    XFree(x);
      e.property = 0;
    }
    XSendEvent(fl_display, e.requestor, 0, 0, (XEvent *)&e);}
    return true;

  }

  return Fl::handle(event, window);
}

////////////////////////////////////////////////////////////////

/**
 * Resizes the actual system window in response to a resize() call from
 * the program.
 */
void Fl_Window::layout() {
  if (this == resize_from_system) {
    // Ignore changes that came from the system
    resize_from_system = 0;
  } else if ((layout_damage()&FL_LAYOUT_XYWH) && i) { // only for shown windows
    // figure out where the window should be in it's parent:
    int x = this->x(); int y = this->y();
    for (Fl_Widget* p = parent(); p && !p->is_window(); p = p->parent()) {
      x += p->x(); y += p->y();
    }
    if (layout_damage() & FL_LAYOUT_WH) {
      // Some window managers refuse to allow resizes unless the resize
      // information allows it:
      if (minw == maxw && minh == maxh) size_range(w(), h(), w(), h());
      XMoveResizeWindow(fl_display, i->xid, x, y,
			w()>0 ? w() : 1, h()>0 ? h() : 1);
      // Wait for echo (relies on window having StaticGravity!!!)
      i->wait_for_expose = true;
    } else {
      XMoveWindow(fl_display, i->xid, x, y);
    }
  }
  Fl_Group::layout();
}

////////////////////////////////////////////////////////////////
// Innards of Fl_Window::create()

extern bool fl_show_iconic; // In Fl_Window.cxx, set by iconize() or -i switch

void Fl_Window::create() {
  Fl_X::create(this, fl_visual, fl_colormap, -1);
}

void Fl_X::create(Fl_Window* window,
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
    // any intermediate Fl_Group widgets:
    for (Fl_Widget *o = window->parent(); ; o = o->parent()) {
      if (o->is_window()) {root = ((Fl_Window*)o)->i->xid; break;}
      X += o->x();
      Y += o->y();
    }
    attr.event_mask = ExposureMask;
  } else {
    if (X == FL_USEDEFAULT) X = (DisplayWidth(fl_display,fl_screen)-W)/2;
    if (Y == FL_USEDEFAULT) Y = (DisplayHeight(fl_display,fl_screen)-H)/2;
    root = RootWindow(fl_display, fl_screen);
    attr.event_mask =
      ExposureMask | StructureNotifyMask
      | KeyPressMask | KeyReleaseMask | KeymapStateMask | FocusChangeMask
      | ButtonPressMask | ButtonReleaseMask
      | EnterWindowMask | LeaveWindowMask
      | PointerMotionMask;
    if (window->override()) {
      Fl::flush(); // make sure save_under does not undo any drawing!
      attr.override_redirect = 1;
      attr.save_under = 1;
      mask |= CWOverrideRedirect | CWSaveUnder;
    }
  }

  if (background >= 0) {
    attr.background_pixel = background;
    mask |= CWBackPixel;
  }

  Fl_X* x = new Fl_X;
  x->xid = XCreateWindow(fl_display,
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
  x->next = Fl_X::first;
  Fl_X::first = x;

  if (!window->parent() && !window->override()) { // send junk to X window manager:

    // Setting this allows the window manager to use the window's class
    // to look up things like border colors and icons in the xrdb database:
    XChangeProperty(fl_display, x->xid, XA_WM_CLASS, XA_STRING, 8, 0,
		    (unsigned char *)window->xclass(), strlen(window->xclass()));

    // Set the label:
    window->label(window->label(), window->iconlabel());
    // Makes the close button produce an event:
    XChangeProperty(fl_display, x->xid, WM_PROTOCOLS,
 		    XA_ATOM, 32, 0, (uchar*)&WM_DELETE_WINDOW, 1);

    // Make it receptive to DnD:
    int version = 4;
    XChangeProperty(fl_display, x->xid, fl_XdndAware,
		    XA_ATOM, sizeof(int)*8, 0, (unsigned char*)&version, 1);

    // Send child window information:
    if (window->child_of() && window->child_of()->shown())
      XSetTransientForHint(fl_display, x->xid, window->child_of()->i->xid);

    // Set up the icon and initial icon state:
    XWMHints hints;
    hints.input = True; // some window managers require this to be sent?
    hints.flags = InputHint;
    if (fl_show_iconic) {
      hints.flags |= StateHint;
      hints.initial_state = IconicState;
      fl_show_iconic = false;
    }
    if (window->icon()) {
      hints.icon_pixmap = (Pixmap)window->icon();
      hints.flags       |= IconPixmapHint;
    }
    XSetWMHints(fl_display, x->xid, &hints);

    // send size limits and border:
    x->sendxjunk();
  }
}

////////////////////////////////////////////////////////////////
// Send X window stuff that can be changed over time:

void Fl_X::sendxjunk() {
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
	hints.max_width = DisplayWidth(fl_display,fl_screen);
      if (hints.max_height < hints.min_height)
	hints.max_height = DisplayHeight(fl_display,fl_screen);
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

  if (window->x() != FL_USEDEFAULT || window->y() != FL_USEDEFAULT) {
    hints.flags |= USPosition;
    hints.x = window->x();
    hints.y = window->y();
  }

 if (!window->border()) {
   prop[0] |= 2; // MWM_HINTS_DECORATIONS
   prop[2] = 0; // no decorations
 }

  XSetWMNormalHints(fl_display, xid, &hints);
  XChangeProperty(fl_display, xid,
		  fl_MOTIF_WM_HINTS, fl_MOTIF_WM_HINTS,
		  32, 0, (unsigned char *)prop, 5);
}

void Fl_Window::size_range_() {
  size_range_set = 1;
  if (i) i->sendxjunk();
}

/**
 * Returns true if the window is shown but is iconized. On X this is
 * also true in the time between when show() is called and when the
 * window manager finally puts the window on the screen and causes
 * an expose event.
 */
bool Fl_Window::iconic() const {
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

void Fl_Window::label(const char *name,const char *iname) {
  Fl_Widget::label(name);
  iconlabel_ = iname;
  if (i && !parent()) {
    if (!name) name = "";
    XChangeProperty(fl_display, i->xid, XA_WM_NAME,
		    XA_STRING, 8, 0, (uchar*)name, strlen(name));
    if (!iname) iname = filename_name(name);
    XChangeProperty(fl_display, i->xid, XA_WM_ICON_NAME, 
		    XA_STRING, 8, 0, (uchar*)iname, strlen(iname));
  }
}

////////////////////////////////////////////////////////////////
// Drawing context

Fl_Drawable* fl_drawable;
Window fl_window;
const Fl_Window *Fl_Window::current_;
GC fl_gc;

void Fl_Window::make_current() const {
  current_ = this;
  i->make_current();
  static GC gc;	// the GC used by all X windows with fl_visual
  if (!gc) gc = XCreateGC(fl_display, i->xid, 0, 0);
  fl_gc = gc;
}

////////////////////////////////////////////////////////////////
// Get the KDE colors that it writes to the xrdb database:

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
  if (!resourcestring) resourcestring = XResourceManagerString(fl_display);
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
    } else if (match(r, Fl_Window::xclass()) && (*r == '.' || *r == '*')) {
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
  return XGetDefault(fl_display, Fl_Window::xclass(), a);
}

static inline const char* get_default(const char* a, const char* b) {
  return XGetDefault(fl_display, a, b);
}

#endif

static Fl_Color to_color(const char* p) {
  return p ? fl_rgb(p) : 0;
}

bool fl_get_system_colors() {
  fl_open_display();

  Fl_Color color;

  color = to_color(get_default("background"));
  if (color) fl_background(color);

  color = to_color(get_default("foreground"));
  if (color) {
    Fl_Widget::default_style->label_color = color;
    Fl_Widget::default_style->highlight_label_color = color;
  }

#if MY_GET_DEFAULT
  color = to_color(get_default("Text.background"));
  if (color) Fl_Widget::default_style->color = color;

  color = to_color(get_default("Text.foreground"));
  if (color) Fl_Widget::default_style->text_color = color;

  color = to_color(get_default("Text.selectBackground"));
  if (color) Fl_Widget::default_style->selection_color = color;

  color = to_color(get_default("Text.selectForeground"));
  if (color) Fl_Widget::default_style->selection_text_color = color;
#else
  color = to_color(get_default("Text","background"));
  if (color) Fl_Widget::default_style->color = color;

  color = to_color(get_default("Text","foreground"));
  if (color) Fl_Widget::default_style->text_color = color;

  color = to_color(get_default("Text","selectBackground"));
  if (color) Fl_Widget::default_style->selection_color = color;

  color = to_color(get_default("Text","selectForeground"));
  if (color) Fl_Widget::default_style->selection_text_color = color;
#endif

  // also Scrollbar,width
  // does not appear to be anything there for setting the tooltips...
  // Maybe I should just add Tooltip,back/foreground.

  // Mousewheel stuff is read from the XDefaults database but there
  // are no standards for these. We should change these to match any
  // standards that arise:

  const char* w = get_default("wheel_scroll_lines");
  if (w) Fl_Style::wheel_scroll_lines = atoi(w);

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

//
// End of "$Id: Fl_x.cxx,v 1.132 2002/09/02 06:33:47 spitzak Exp $".
//
