//
// "$Id: Fl_x.cxx,v 1.101 2001/02/18 07:17:09 robertk Exp $"
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
#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Style.H>
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

void Fl::remove_fd(int n) {
  remove_fd(n, -1);
}

#if CONSOLIDATE_MOTION
static Fl_Window* send_motion;
extern Fl_Window* fl_xmousewin;
#endif
static void do_queued_events() {
  while (XEventsQueued(fl_display,QueuedAfterReading)) {
    XEvent xevent;
    XNextEvent(fl_display, &xevent);
    fl_handle(xevent);
  }
#if CONSOLIDATE_MOTION
  if (send_motion && send_motion == fl_xmousewin) {
    send_motion = 0;
    Fl::handle(FL_MOVE, fl_xmousewin);
  }
#endif
}

// these pointers are set by the Fl::lock() function:
static void nothing() {}
void (*fl_lock_function)() = nothing;
void (*fl_unlock_function)() = nothing;

static int fl_wait(double time_to_wait) {

  // OpenGL and other broken libraries call XEventsQueued
  // unnecessarily and thus cause the file descriptor to not be ready,
  // so we must check for already-read events:
  if (fl_display && XQLength(fl_display)) {do_queued_events(); return 1;}

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
static int fl_ready() {
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

Display *fl_display;
Window fl_message_window;
int fl_screen;
XVisualInfo *fl_visual;
Colormap fl_colormap;

static Atom WM_DELETE_WINDOW;
static Atom WM_PROTOCOLS;
       Atom fl_MOTIF_WM_HINTS;
static Atom FLTKChangeScheme;
static Atom TARGETS;
static Atom XdndAware;
static Atom XdndSelection;
static Atom XdndEnter;
static Atom XdndTypeList;
static Atom XdndPosition;
static Atom XdndLeave;
static Atom XdndDrop;
static Atom XdndStatus;
static Atom XdndActionCopy;
static Atom XdndFinished;
//static Atom XdndProxy;

static void fd_callback(int,void *) {do_queued_events();}

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
  Fl::add_fd(ConnectionNumber(d), POLLIN, fd_callback);

  WM_DELETE_WINDOW	= XInternAtom(d, "WM_DELETE_WINDOW",	0);
  WM_PROTOCOLS		= XInternAtom(d, "WM_PROTOCOLS",	0);
  fl_MOTIF_WM_HINTS	= XInternAtom(d, "_MOTIF_WM_HINTS",	0);
  FLTKChangeScheme	= XInternAtom(d, "FLTKChangeScheme",	0);
  TARGETS		= XInternAtom(d, "TARGETS",		0);
  XdndAware		= XInternAtom(d, "XdndAware",		0);
  XdndSelection		= XInternAtom(d, "XdndSelection",	0);
  XdndEnter		= XInternAtom(d, "XdndEnter",		0);
  XdndTypeList		= XInternAtom(d, "XdndTypeList",	0);
  XdndPosition		= XInternAtom(d, "XdndPosition",	0);
  XdndLeave		= XInternAtom(d, "XdndLeave",		0);
  XdndDrop		= XInternAtom(d, "XdndDrop",		0);
  XdndStatus		= XInternAtom(d, "XdndStatus",		0);
  XdndActionCopy	= XInternAtom(d, "XdndActionCopy",	0);
  XdndFinished		= XInternAtom(d, "XdndFinished",	0);
  //XdndProxy		= XInternAtom(d, "XdndProxy",		0);

  fl_screen = DefaultScreen(fl_display);
  // construct an XVisualInfo that matches the default Visual:
  XVisualInfo templt; int num;
  templt.visualid = XVisualIDFromVisual(DefaultVisual(fl_display,fl_screen));
  fl_visual = XGetVisualInfo(fl_display, VisualIDMask, &templt, &num);
  fl_colormap = DefaultColormap(fl_display,fl_screen);

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
  Atom style_atom = XInternAtom(fl_display, "FLTK_STYLE_WINDOW", False);
  long data = 1;
  XChangeProperty(fl_display, fl_message_window, style_atom, style_atom, 32,
                  PropModeReplace, (unsigned char *)&data, 1);

#if !USE_COLORMAP
  Fl::visual(FL_RGB);
#endif
}

void fl_close_display() {
  Fl::remove_fd(ConnectionNumber(fl_display));
  XCloseDisplay(fl_display);
}

int Fl::x() {return 0;}

int Fl::y() {return 0;}

int Fl::w() {
  fl_open_display();
  return DisplayWidth(fl_display,fl_screen);
}

int Fl::h() {
  fl_open_display();
  return DisplayHeight(fl_display,fl_screen);
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
// Code used for paste and DnD into the program:

static Fl_Widget *fl_selection_requestor;
static char *selection_buffer;
static int selection_length;
static int selection_buffer_length;
static char i_own_selection;

// Call this when a "paste" operation happens:
void Fl::paste(Fl_Widget &receiver) {
  if (i_own_selection) {
    // We already have it, do it quickly without window server.
    // Notice that the text is clobbered if set_selection is
    // called in response to FL_PASTE!
    Fl::e_text = selection_buffer;
    Fl::e_length = selection_length;
    receiver.handle(FL_PASTE);
    return;
  }
  // otherwise get the window server to return it:
  fl_selection_requestor = &receiver;
  XConvertSelection(fl_display, XA_PRIMARY, XA_STRING, XA_PRIMARY,
		    fl_xid(Fl::first_window()), fl_event_time);
}

Window fl_dnd_source_window;
Atom *fl_dnd_source_types; // null-terminated list of data types being supplied
Atom fl_dnd_type;
Atom fl_dnd_source_action;
Atom fl_dnd_action;

static void sendClientMessage(Window window, Atom message,
                              unsigned long d0,
                              unsigned long d1=0,
                              unsigned long d2=0,
                              unsigned long d3=0,
                              unsigned long d4=0)
{
  XClientMessageEvent e;
  e.type = ClientMessage;
  e.serial = 0;
  e.send_event = 0;
  e.display = fl_display;
  e.window = window;
  e.message_type = message;
  e.format = 32;
  e.data.l[0] = (long)d0;
  e.data.l[1] = (long)d1;
  e.data.l[2] = (long)d2;
  e.data.l[3] = (long)d3;
  e.data.l[4] = (long)d4;
  XSendEvent(fl_display, window, 0, 0, (XEvent*)&e);
}

////////////////////////////////////////////////////////////////
// Code for copying to clipboard and DnD out of the program:

void Fl::copy(const char *stuff, int len) {
  if (!stuff || len<0) return;
  if (len+1 > selection_buffer_length) {
    delete[] selection_buffer;
    selection_buffer = new char[len+100];
    selection_buffer_length = len+100;
  }
  memcpy(selection_buffer, stuff, len);
  selection_buffer[len] = 0; // needed for direct paste
  selection_length = len;
  i_own_selection = 1;
  XSetSelectionOwner(fl_display, XA_PRIMARY, fl_message_window, fl_event_time);
}

static int grabfunc(int event, void*) {
  if (event == FL_RELEASE) Fl::pushed(0);
  return 0;
}

// return version # of Xdnd this window supports.  Also change the
// window the the proxy if it uses a proxy:
static int dnd_aware(Window& window) {
  Atom actual; int format; unsigned long count, remaining;
  unsigned char *data = 0;
  XGetWindowProperty(fl_display, window, XdndAware,
		     0, 4, False, XA_ATOM, 
		     &actual, &format, 
		     &count, &remaining, &data);
  if (actual == XA_ATOM && format==32 && count && data)
    return int(*(Atom*)data);
  return 0;
}

// send an event to an fltk window belonging to this program:
static int local_handle(int event, Fl_Window* window) {
  Fl::local_grab(0);
  Fl::e_x = Fl::e_x_root-window->x();
  Fl::e_y = Fl::e_y_root-window->y();
  int ret = Fl::handle(event,window);
  Fl::local_grab(grabfunc);
  return ret;
}

int Fl::dnd() {
  Fl::first_window()->cursor((Fl_Cursor)21);
  Window source_window = fl_xid(Fl::first_window());
  local_grab(grabfunc);
  Window target_window = 0;
  Fl_Window* local_window = 0;
  int version = 4; int dest_x, dest_y;
  XSetSelectionOwner(fl_display, XdndSelection, fl_message_window, fl_event_time);

  while (Fl::pushed()) {

    // figure out what window we are pointing at:
    Window new_window = 0; int new_version = 0;
    Fl_Window* new_local_window = 0;
    for (Window child = RootWindow(fl_display, fl_screen);;) {
      Window root; unsigned int junk3;
      XQueryPointer(fl_display, child, &root, &child,
		    &e_x_root, &e_y_root, &dest_x, &dest_y, &junk3);
      if (!child) {
	if (!new_window && (new_version = dnd_aware(root))) new_window = root;
	break;
      }
      new_window = child;
      if ((new_local_window = fl_find(child))) break;
      if ((new_version = dnd_aware(new_window))) break;
    }

    if (new_window != target_window) {
      if (local_window) {
	local_handle(FL_DND_LEAVE, local_window);
      } else if (version) {
	sendClientMessage(target_window, XdndLeave, source_window);
      }
      version = new_version;
      target_window = new_window;
      local_window = new_local_window;
      if (local_window) {
	local_handle(FL_DND_ENTER, local_window);
      } else if (version) {
	sendClientMessage(target_window, XdndEnter, source_window,
			  version<<24, XA_STRING, 0, 0);
      }
    }
    if (local_window) {
      local_handle(FL_DND_DRAG, local_window);
    } else if (version) {
      sendClientMessage(target_window, XdndPosition, source_window,
			0, (e_x_root<<16)|e_y_root, fl_event_time,
			XdndActionCopy);
    }
    Fl::wait();
  }

  if (local_window) {
    i_own_selection = 1;
    if (local_handle(FL_DND_RELEASE, local_window)) paste(*belowmouse());
  } else if (version) {
    sendClientMessage(target_window, XdndDrop, source_window,
		      0, fl_event_time);
  } else if (target_window) {
    // fake a drop by clicking the middle mouse button:
    XButtonEvent msg;
    msg.type = ButtonPress;
    msg.window = target_window;
    msg.root = RootWindow(fl_display, fl_screen);
    msg.subwindow = 0;
    msg.time = fl_event_time+1;
    msg.x = dest_x;
    msg.y = dest_y;
    msg.x_root = Fl::e_x_root;
    msg.y_root = Fl::e_y_root;
    msg.state = 0x0;
    msg.button = Button2;
    XSendEvent(fl_display, target_window, False, 0L, (XEvent*)&msg);
    msg.time++;
    msg.state = 0x200;
    msg.type = ButtonRelease;
    XSendEvent(fl_display, target_window, False, 0L, (XEvent*)&msg);
  }

  release();
  Fl::first_window()->cursor(FL_CURSOR_DEFAULT);
  return 1;
}

////////////////////////////////////////////////////////////////

const XEvent* fl_xevent; // the current x event
ulong fl_event_time; // the last timestamp from an x event

char fl_key_vector[32]; // used by Fl::get_key()

// Record event mouse position and state from an XEvent:

static int px, py;
static ulong ptime;

static void set_event_xy() {
#if CONSOLIDATE_MOTION
  send_motion = 0;
#endif
  Fl::e_x_root = fl_xevent->xbutton.x_root;
  Fl::e_x = fl_xevent->xbutton.x;
  Fl::e_y_root = fl_xevent->xbutton.y_root;
  Fl::e_y = fl_xevent->xbutton.y;
  Fl::e_state = fl_xevent->xbutton.state << 16;
  fl_event_time = fl_xevent->xbutton.time;
#ifdef __sgi
  // get the meta key off PC keyboards:
  if (fl_key_vector[18]&0x18) Fl::e_state |= FL_META;
#endif
  // turn off is_click if enough time or mouse movement has passed:
  if (abs(Fl::e_x_root-px)+abs(Fl::e_y_root-py) > 3 
      || fl_event_time >= ptime+(Fl::pushed()?200:1000))
    Fl::e_is_click = 0;
}

// if this is same event as last && is_click, increment click count:
static inline void checkdouble() {
  if (Fl::e_is_click == Fl::e_keysym)
    Fl::e_clicks++;
  else {
    Fl::e_clicks = 0;
    Fl::e_is_click = Fl::e_keysym;
  }
  px = Fl::e_x_root;
  py = Fl::e_y_root;
  ptime = fl_event_time;
}

////////////////////////////////////////////////////////////////

static Fl_Window* resize_from_system;

unsigned fl_mousewheel_up = 4, fl_mousewheel_down = 5;

int fl_handle(const XEvent& xevent)
{
  fl_xevent = &xevent;
  Fl_Window* window = fl_find(xevent.xany.window);
  int event = 0;

  switch (xevent.type) {

  case KeymapNotify:
    memcpy(fl_key_vector, xevent.xkeymap.key_vector, 32);
    break;

  case MappingNotify:
    XRefreshKeyboardMapping((XMappingEvent*)&xevent.xmapping);
    break;

  case ClientMessage: {
    Atom message = fl_xevent->xclient.message_type;
    const long* data = fl_xevent->xclient.data.l;

    if (window && (Atom)(data[0]) == WM_DELETE_WINDOW) {
      if (!Fl::grab() && !(Fl::modal() && window != Fl::modal()))
	window->do_callback();
      return 1;

    } else if (message == FLTKChangeScheme) {
      Fl::reload_scheme();
      return 1;

    } else if (message == XdndEnter) {
      fl_dnd_source_window = data[0];
      // version number is data[1]>>24
      if (data[1]&1) {
	// get list of data types:
	Atom actual; int format; unsigned long count, remaining;
	unsigned char *buffer = 0;
	XGetWindowProperty(fl_display, fl_dnd_source_window, XdndTypeList,
			   0, 0x8000000L, False, XA_ATOM, &actual, &format,
			   &count, &remaining, &buffer);
	if (actual != XA_ATOM || format != 32 || count<4 || !buffer)
	  goto FAILED;
	delete [] fl_dnd_source_types;
	fl_dnd_source_types = new Atom[count+1];
	for (unsigned i = 0; i < count; i++)
	  fl_dnd_source_types[i] = ((Atom*)buffer)[i];
	fl_dnd_source_types[count] = 0;
      } else {
      FAILED:
	// less than four data types, or if the above messes up:
	if (!fl_dnd_source_types) fl_dnd_source_types = new Atom[4];
	fl_dnd_source_types[0] = data[2];
	fl_dnd_source_types[1] = data[3];
	fl_dnd_source_types[2] = data[4];
	fl_dnd_source_types[3] = 0;
      }
      fl_dnd_type = fl_dnd_source_types[0]; // should pick text or url
      event = FL_DND_ENTER;
      break;

    } else if (message == XdndPosition) {
      fl_dnd_source_window = data[0];
      Fl::e_x_root = data[2]>>16;
      Fl::e_y_root = data[2]&0xFFFF;
      if (window) {
	Fl::e_x = Fl::e_x_root-window->x();
	Fl::e_y = Fl::e_y_root-window->y();
      }
      fl_event_time = data[3];
      fl_dnd_source_action = data[4];
      fl_dnd_action = XdndActionCopy;
      int accept = Fl::handle(FL_DND_DRAG, window);
      sendClientMessage(data[0], XdndStatus,
			fl_xevent->xclient.window,
			accept ? 1 : 0,
			0, // used for xy rectangle to not send position inside
			0, // used for width+height of rectangle
			accept ? fl_dnd_action : None);
      return 1;

    } else if (message == XdndLeave) {
      fl_dnd_source_window = 0; // don't send a finished message to it
      event = FL_DND_LEAVE;
      break;

    } else if (message == XdndDrop) {
      fl_dnd_source_window = data[0];
      fl_event_time = data[2];
      Window to_window = fl_xevent->xclient.window;
      if (Fl::handle(FL_DND_RELEASE, window)) {
	fl_selection_requestor = Fl::belowmouse();
	XConvertSelection(fl_display, XdndSelection, fl_dnd_type, XA_SECONDARY,
			  to_window, fl_event_time);
      } else {
	// Send the finished message if I refuse the drop.
	// It is not clear whether I can just send finished always,
	// or if I have to wait for the SelectionNotify event as the	
	// code is currently doing.
	sendClientMessage(fl_dnd_source_window, XdndFinished, to_window);
	fl_dnd_source_window = 0;
      }
      return 1;

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
    window = fl_find(xevent.xmapping.window);
    if (!window) break;
    if (window->parent()) break; // ignore child windows

    // figure out where OS really put window
    XWindowAttributes actual;
    XGetWindowAttributes(fl_display, fl_xid(window), &actual);
    Window cr; int X, Y, W = actual.width, H = actual.height;
    XTranslateCoordinates(fl_display, fl_xid(window), actual.root,
                          0, 0, &X, &Y, &cr);

    // tell Fl_Window about it and set flag to prevent echoing:
    if (window->resize(X, Y, W, H)) resize_from_system = window;
    return 1;
  }

  case UnmapNotify:
    window = fl_find(xevent.xmapping.window);
    if (window) {Fl_X::i(window)->wait_for_expose = 1; return 1;}
    break;

  case Expose:
  case GraphicsExpose:
    if (!window) break;
    Fl_X::i(window)->wait_for_expose = 0;
    window->damage(FL_DAMAGE_EXPOSE, xevent.xexpose.x, xevent.xexpose.y,
		   xevent.xexpose.width, xevent.xexpose.height);
    return 1;

  case ButtonPress:
    Fl::e_keysym = FL_Button + xevent.xbutton.button;
    set_event_xy(); checkdouble();
    if (xevent.xbutton.button == fl_mousewheel_up) {
      Fl::e_dy = -14*3;
      event = FL_VIEWCHANGE;
    } else if (xevent.xbutton.button == fl_mousewheel_down) {
      Fl::e_dy = +14*3;
      event = FL_VIEWCHANGE;
    } else {
      Fl::e_state |= (FL_BUTTON1 << (xevent.xbutton.button-1));
      event = FL_PUSH;
    }
    break;

  case MotionNotify:
    set_event_xy();
#if CONSOLIDATE_MOTION
    send_motion = fl_xmousewin = window;
    return 0;
#else
    event = FL_MOVE;
    break;
#endif

  case ButtonRelease:
    Fl::e_keysym = FL_Button + xevent.xbutton.button;
    set_event_xy();
    if (xevent.xbutton.button == fl_mousewheel_up ||
	xevent.xbutton.button == fl_mousewheel_down) break;
    Fl::e_state &= ~(FL_BUTTON1 << (xevent.xbutton.button-1));
    event = FL_RELEASE;
    break;

  case FocusIn:
    xfocus = window;
    if (window) {fl_fix_focus(); return 1;}
    break;

  case FocusOut:
    if (window && window == xfocus) {xfocus = 0; fl_fix_focus(); return 1;}
    break;

  case KeyPress:
  case KeyRelease: {
    int keycode = xevent.xkey.keycode;
    KeySym keysym;
    if (xevent.type == KeyPress) {
      event = FL_KEYBOARD;
      fl_key_vector[keycode/8] |= (1 << (keycode%8));
      static char buffer[21];
      int len = XLookupString((XKeyEvent*)&(xevent.xkey), buffer, 20, &keysym, 0);
      if (keysym && keysym < 0x400) { // a character in latin-1,2,3,4 sets
	// force it to type a character (not sure if this ever is needed):
	if (!len) {buffer[0] = char(keysym); len = 1;}
	// ignore all effects of shift on the keysyms, which makes it a lot
	// easier to program shortcuts and is Windows-compatable:
	keysym = XKeycodeToKeysym(fl_display, keycode, 0);
      }
      if (fl_xevent->xbutton.state&4 && keysym == '-') buffer[0] = 0x1f; // ^_
      buffer[len] = 0;
      Fl::e_text = buffer;
      Fl::e_length = len;
    } else {
      event = FL_KEYUP;
      fl_key_vector[keycode/8] &= ~(1 << (keycode%8));
      // keyup events just get the unshifted keysym:
      keysym = XKeycodeToKeysym(fl_display, keycode, 0);
    }
#if 0
    // Attempt to fix keyboards that send "delete" for the key in the
    // upper-right corner of the main keyboard.  But it appears that
    // very few of these remain?  A better test would be to look at the
    // keymap and see if any key turns into FL_BackSpace.
    static int got_backspace;
    if (!got_backspace) {
      if (keysym == FL_Delete) keysym = FL_BackSpace;
      else if (keysym == FL_BackSpace) got_backspace = 1;
    }
#endif
#ifdef __sgi
    // You can plug a microsoft keyboard into an sgi but the extra shift
    // keys are not translated.  Make them translate like XFree86 does:
    if (!keysym) switch(keycode) {
    case 147: keysym = FL_Meta_L; break;
    case 148: keysym = FL_Meta_R; break;
    case 149: keysym = FL_Menu; break;
    }
#endif
    // We have to get rid of the XK_KP_function keys, because they are
    // not produced on Windows and thus case statements tend not to check
    // for them:
    if (keysym >= 0xff91 && keysym <= 0xff9f) {
      // Try to make them turn into FL_KP+'c' so that NumLock is
      // irrelevant, by looking at the shifted code:
      unsigned long keysym1 = XKeycodeToKeysym(fl_display, keycode, 1);
      if (keysym1 <= 0x7f || keysym1 > 0xff9f && keysym1 <= FL_KP_Last) {
	keysym = keysym1 | FL_KP;
	if (xevent.type == KeyPress) {
	  Fl::e_text[0] = char(keysym1) & 0x7F;
	  Fl::e_text[1] = 0;
	  Fl::e_length = 1;
	}
      } else {
	// If that failed to work, translate assumming PC keyboard layout:
	static const unsigned short table[15] = {
	  FL_F+1, FL_F+2, FL_F+3, FL_F+4,
	  FL_Home, FL_Left, FL_Up, FL_Right,
	  FL_Down, FL_Page_Up, FL_Page_Down, FL_End,
	  FL_Clear, FL_Insert, FL_Delete};
	keysym = table[keysym-0xff91];
      }
    }
    // We also need to get rid of Left_Tab, again to match Win32 and to
    // make each key on the keyboard send a single keysym:
    else if (keysym == 0xfe20) {
      keysym = FL_Tab;
      Fl::e_state |= FL_SHIFT;
    }
    Fl::e_keysym = int(keysym);
    set_event_xy(); checkdouble();
    break;}

  case EnterNotify:
    if (xevent.xcrossing.detail == NotifyInferior) break;
    // XInstallColormap(fl_display, Fl_X::i(window)->colormap);
    set_event_xy();
    Fl::e_state = xevent.xcrossing.state << 16;
    xmousewin = window;
    event = FL_ENTER;
    break;

  case LeaveNotify:
    if (xevent.xcrossing.detail == NotifyInferior) break;
    set_event_xy();
    Fl::e_state = xevent.xcrossing.state << 16;
    if (window != xmousewin) xmousewin = 0;
    event = FL_LEAVE;
    break;

  case SelectionNotify: {
    if (!fl_selection_requestor) return 0;
    static unsigned char* buffer;
    if (buffer) XFree(buffer);
    long read = 0;
    if (fl_xevent->xselection.property) for (;;) {
      // The Xdnd code pastes 64K chunks together, possibly to avoid
      // bugs in X servers, or maybe to avoid an extra round-trip to
      // get the property length.  I copy this here:
      Atom actual; int format; unsigned long count, remaining;
      unsigned char* portion;
      if (XGetWindowProperty(fl_display,
			     fl_xevent->xselection.requestor,
			     fl_xevent->xselection.property,
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
    fl_selection_requestor->handle(FL_PASTE);
    // Detect if this paste is due to Xdnd by the property name (I use
    // XA_SECONDARY for that) and send an XdndFinished message. It is not
    // clear if this has to be delayed until now or if it can be done
    // immediatly after calling XConvertSelection.
    if (fl_xevent->xselection.property == XA_SECONDARY &&
	fl_dnd_source_window) {
      sendClientMessage(fl_dnd_source_window, XdndFinished,
			fl_xevent->xselection.requestor);
      fl_dnd_source_window = 0; // don't send a second time
    }
    return 1;}

  case SelectionClear:
    i_own_selection = 0;
    return 1;

  case SelectionRequest: {
    XSelectionEvent e;
    e.type = SelectionNotify;
    e.display = fl_display;
    e.requestor = fl_xevent->xselectionrequest.requestor;
    e.selection = fl_xevent->xselectionrequest.selection;
    e.target = fl_xevent->xselectionrequest.target;
    e.time = fl_xevent->xselectionrequest.time;
    e.property = fl_xevent->xselectionrequest.property;
    if (e.target == TARGETS) {
      Atom a = XA_STRING;
      XChangeProperty(fl_display, e.requestor, e.property,
		      XA_ATOM, sizeof(Atom)*8, 0, (unsigned char*)&a,
		      sizeof(Atom));
    } else if (/*e.target == XA_STRING &&*/ selection_length) {
      XChangeProperty(fl_display, e.requestor, e.property,
		      e.target, 8, 0, (unsigned char *)selection_buffer,
		      selection_length);
    } else {
//    char* x = XGetAtomName(fl_display,e.target);
//    fprintf(stderr,"selection request of %s\n",x);
//    XFree(x);
      e.property = 0;
    }
    XSendEvent(fl_display, e.requestor, 0, 0, (XEvent *)&e);}
    return 1;

  }

  return Fl::handle(event, window);
}

////////////////////////////////////////////////////////////////

void Fl_Window::layout() {
  int x = this->x(); int y = this->y();
  for (Fl_Widget* p = parent(); p && !p->is_window(); p = p->parent()) {
    x += p->x(); y += p->y();
  }
  if (ow() == w() && oh() == h()) {
    if (this == resize_from_system) resize_from_system = 0;
//    else if (i && (ox() != this->x() || oy() != this->y()))
    else if (i && (ox() != x || oy() != y))
      XMoveWindow(fl_display, i->xid, x, y);
    for (int n = children(); n--;) {
      Fl_Widget* o = child(n);
      if (o->damage() & FL_DAMAGE_LAYOUT) o->layout();
    }
    Fl_Widget::layout();
    set_old_size();
  } else {
    if (this == resize_from_system) resize_from_system = 0;
    else if (i) {
      //if (!resizable()) size_range(w(), h(), w(), h());
      XMoveResizeWindow(fl_display, i->xid, x, y,
			w()>0 ? w() : 1, h()>0 ? h() : 1);
      i->wait_for_expose = 1; // (breaks menus somehow...)
      // redraw(); // 
    }
    Fl_Group::layout();
  }
}

////////////////////////////////////////////////////////////////
// Innards of Fl_Window::create()

extern char fl_show_iconic; // set by iconize() or Fl_arg -i switch

void Fl_Window::create() {
  Fl_X::create(this, fl_visual, fl_colormap, -1);
}

void Fl_X::create(Fl_Window* w,
		  XVisualInfo *visual, Colormap colormap,
		  int background)
{
  ulong root;
  XSetWindowAttributes attr;
  int mask = CWBorderPixel|CWColormap|CWEventMask|CWBitGravity;

  if (w->parent()) {
    root = w->window()->i->xid;
    attr.event_mask = ExposureMask;
  } else {
    root = RootWindow(fl_display, fl_screen);
    attr.event_mask =
      ExposureMask | StructureNotifyMask
      | KeyPressMask | KeyReleaseMask | KeymapStateMask | FocusChangeMask
      | ButtonPressMask | ButtonReleaseMask
      | EnterWindowMask | LeaveWindowMask
      | PointerMotionMask;
    if (!w->border()) {
      attr.override_redirect = 1;
      attr.save_under = 1;
      mask |= CWOverrideRedirect | CWSaveUnder;
    }
  }
  attr.border_pixel = 0;
  attr.colormap = colormap;
  attr.bit_gravity = 0; // StaticGravity;

  if (background >= 0) {
    attr.background_pixel = background;
    mask |= CWBackPixel;
  }

  int W = w->w();
  if (W <= 0) W = 1; // X don't like zero...
  int H = w->h();
  if (H <= 0) H = 1; // X don't like zero...
  int X = w->x(); if (X == FL_USEDEFAULT) X = (Fl::w()-W)/2;
  int Y = w->y(); if (Y == FL_USEDEFAULT) Y = (Fl::h()-H)/2;

  Fl_X* x = new Fl_X;
  x->xid = XCreateWindow(fl_display,
			 root,
			 X, Y, W, H,
			 0, // borderwidth
			 visual->depth,
			 InputOutput,
			 visual->visual,
			 mask, &attr);

  x->other_xid = 0;
  x->w = w; w->i = x;
  x->region = 0;
  x->wait_for_expose = 1;
  x->next = Fl_X::first;
  Fl_X::first = x;

  if (!w->parent() && w->border()) { // send junk to X window manager:

    // Setting this allows the window manager to use the window's class
    // to look up things like border colors and icons in the xrdb database:
    XChangeProperty(fl_display, x->xid, XA_WM_CLASS, XA_STRING, 8, 0,
		    (unsigned char *)w->xclass(), strlen(w->xclass()));

    // Set the label:
    w->label(w->label(), w->iconlabel());

    // Makes the close button produce an event:
    XChangeProperty(fl_display, x->xid, WM_PROTOCOLS,
 		    XA_ATOM, 32, 0, (uchar*)&WM_DELETE_WINDOW, 1);

    // Make it receptive to DnD:
    int version = 4;
    XChangeProperty(fl_display, x->xid, XdndAware,
		    XA_ATOM, sizeof(int)*8, 0, (unsigned char*)&version, 1);

    // Send child window information:
    if (w->modal_for()) {
      const Fl_Window* modal_for = w->modal_for();
      while (modal_for && modal_for->parent()) modal_for = modal_for->window();
      if (modal_for && modal_for->shown())
	XSetTransientForHint(fl_display, x->xid, modal_for->i->xid);
    }

    // Set up the icon and initial icon state:
    XWMHints hints;
    hints.input = True; // some window managers require this to be sent?
    hints.flags = InputHint;
    if (fl_show_iconic) {
      hints.flags |= StateHint;
      hints.initial_state = IconicState;
    }
    if (w->icon()) {
      hints.icon_pixmap = (Pixmap)w->icon();
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
  if (w->parent() || !w->border()) return; // it's not a window manager window!

  XSizeHints hints;
  // memset(&hints, 0, sizeof(hints)); jreiser suggestion to fix purify?
  hints.min_width = w->minw;
  hints.min_height = w->minh;
  hints.max_width = w->maxw;
  hints.max_height = w->maxh;
  hints.width_inc = w->dw;
  hints.height_inc = w->dh;
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
      if (hints.max_width < hints.min_width) hints.max_width = Fl::w();
      if (hints.max_height < hints.min_height) hints.max_height = Fl::h();
    }
    if (hints.width_inc && hints.height_inc) hints.flags |= PResizeInc;
//     if (w->aspect) {
//       hints.min_aspect.x = hints.max_aspect.x = hints.min_width;
//       hints.min_aspect.y = hints.max_aspect.y = hints.min_height;
//       hints.flags |= PAspect;
//     }
  } else { // not resizable:
    hints.flags = PMinSize|PMaxSize;
    prop[0] = 1; // MWM_HINTS_FUNCTIONS
    prop[1] = 1|2|16; // MWM_FUNC_ALL | MWM_FUNC_RESIZE | MWM_FUNC_MAXIMIZE
  }

  if (w->x() != FL_USEDEFAULT || w->y() != FL_USEDEFAULT) {
    hints.flags |= USPosition;
    hints.x = w->x();
    hints.y = w->y();
  }

//   if (!w->border()) {
//     prop[0] |= 2; // MWM_HINTS_DECORATIONS
//     prop[2] = 0; // no decorations
//   }

  XSetWMNormalHints(fl_display, xid, &hints);
  XChangeProperty(fl_display, xid,
		  fl_MOTIF_WM_HINTS, fl_MOTIF_WM_HINTS,
		  32, 0, (unsigned char *)prop, 5);
}

void Fl_Window::size_range_() {
  size_range_set = 1;
  if (i) i->sendxjunk();
}

int Fl_Window::iconic() const {
  return (i && visible() && i->wait_for_expose);
}

////////////////////////////////////////////////////////////////

// returns pointer to the filename, or null if name ends with '/'
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

Window fl_window;
const Fl_Window *Fl_Window::current_;
GC fl_gc;
// the current drawing origin
int fl_x_, fl_y_;

void Fl_Window::make_current() const {
  static GC gc;	// the GC used by all X windows with fl_visual
  if (!gc) gc = XCreateGC(fl_display, i->xid, 0, 0);
  fl_window = i->xid;
  fl_gc = gc;
  current_ = this;
  fl_clip_region(0);
  fl_x_ = fl_y_ = 0;
}

////////////////////////////////////////////////////////////////
// Get the KDE colors that it writes to the xrdb database:

static const char* get_default(const char* a, const char* b) {
  return XGetDefault(fl_display, a, b);
}

static const char* get_default(const char* a) {
  return XGetDefault(fl_display, Fl_Window::xclass(), a);
}

static Fl_Color to_color(const char* p) {
  return p ? fl_rgb(p) : 0;
}

void fl_get_system_colors() {
  Fl_Color color;

  color = to_color(get_default("background"));
  if (color) fl_background(color);

  color = to_color(get_default("foreground"));
  if (color) {
    Fl_Widget::default_style->label_color = color;
    Fl_Widget::default_style->highlight_label_color = color;
  }

  color = to_color(get_default("Text","background"));
  if (color) Fl_Widget::default_style->text_background = color;

  color = to_color(get_default("Text","foreground"));
  if (color) Fl_Widget::default_style->text_color = color;

  color = to_color(get_default("Text","selectBackground"));
  if (color) Fl_Widget::default_style->selection_color = color;

  color = to_color(get_default("Text","selectForeground"));
  if (color) Fl_Widget::default_style->selection_text_color = color;

  // also Scrollbar,width
  // does not appear to be anything there for setting the tooltips...
  // Maybe I should just add Tooltip,back/foreground.

}

//
// End of "$Id: Fl_x.cxx,v 1.101 2001/02/18 07:17:09 robertk Exp $".
//
