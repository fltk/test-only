//
// "$Id: Fl_x.cxx,v 1.78 2000/06/18 07:57:32 bill Exp $"
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

// This function must subtract running time from any pending timeouts.
// If reset_clock is true then we have not called this recently, in
// that case just remember the time and subtract nothing.
static void elapse_timeouts() {
  static struct timeval prevclock;
  struct timeval newclock;
  gettimeofday(&newclock, NULL);
  if (reset_clock) {
    prevclock.tv_sec = newclock.tv_sec;
    prevclock.tv_usec = newclock.tv_usec;
    reset_clock = 0;
    return;
  }
  double elapsed = newclock.tv_sec - prevclock.tv_sec +
    (newclock.tv_usec - prevclock.tv_usec)/1000000.0;
  prevclock.tv_sec = newclock.tv_sec;
  prevclock.tv_usec = newclock.tv_usec;
  if (elapsed <= 0) return;

  // expire any timeouts:
  for (int i=0; i<numtimeouts; i++) timeout[i].time -= elapsed;
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
int fl_screen;
XVisualInfo *fl_visual;
Colormap fl_colormap;

static Atom wm_delete_window;
static Atom wm_protocols;
       Atom fl_motif_wm_hints;

static void fd_callback(int,void *) {do_queued_events();}

static int io_error_handler(Display*) {Fl::fatal("X I/O error"); return 0;}

static int xerror_handler(Display* d, XErrorEvent* e) {
  char buf1[128], buf2[128];
  sprintf(buf1, "XRequest.%d", e->request_code);
  XGetErrorDatabaseText(d,"",buf1,buf1,buf2,128);
  XGetErrorText(d, e->error_code, buf1, 128);
  Fl::warning("%s: %s 0x%lx", buf2, buf1, e->resourceid);
  return 0;
}

// this function handles FLTK style change messages
static int style_event_handler(int) {
  if (fl_xevent->type != ClientMessage) return 0; // not a Client message
  Atom Scheme = XInternAtom(fl_display, "FLTKChangeScheme", False);
  XClientMessageEvent* cm = (XClientMessageEvent*)fl_xevent;
  if (cm->message_type != Scheme || !strcasecmp(Fl::scheme(), "none")) return 0;
  char scheme[80];
  if (!Fl::getconf("scheme", scheme, sizeof(scheme))) {
    Fl::scheme(scheme);
  }
  return 1;
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

  wm_delete_window = XInternAtom(d,"WM_DELETE_WINDOW",0);
  wm_protocols = XInternAtom(d,"WM_PROTOCOLS",0);
  fl_motif_wm_hints = XInternAtom(d,"_MOTIF_WM_HINTS",0);
  Fl::add_fd(ConnectionNumber(d), POLLIN, fd_callback);

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

  Atom style_atom = XInternAtom(fl_display, "FLTK_STYLE_WINDOW", False);
  Window root = RootWindow(fl_display, fl_screen);
  Window style_win = XCreateSimpleWindow(fl_display, root, 0,0,1,1,0, 0, 0);
  long data = 1;
  XChangeProperty(fl_display, style_win, style_atom, style_atom, 32,
                  PropModeReplace, (unsigned char *)&data, 1);

  // add handler to process style change X events
  Fl::add_handler(style_event_handler);

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
  Fl::e_keysym = 0;
  fl_xevent = &xevent;
  Window xid = xevent.xany.window;

  switch (xevent.type) {

  // events where we don't care about window:

  case KeymapNotify:
    memcpy(fl_key_vector, xevent.xkeymap.key_vector, 32);
    return 0;

  case MappingNotify:
    XRefreshKeyboardMapping((XMappingEvent*)&xevent.xmapping);
    return 0;

  // events where interesting window id is in a different place:
  case CirculateNotify:
  case CirculateRequest:
  case ConfigureNotify:
  case ConfigureRequest:
  case CreateNotify:
  case DestroyNotify:
  case GravityNotify:
  case MapNotify:
  case MapRequest:
  case ReparentNotify:
  case UnmapNotify:
    xid = xevent.xmaprequest.window;
    break;
  }

  int event = 0;
  Fl_Window* window = fl_find(xid);

  if (window) switch (xevent.type) {

  case ClientMessage:
    if ((Atom)(xevent.xclient.data.l[0]) != wm_delete_window) break;
    if (Fl::grab() || Fl::modal() && window != Fl::modal()) return 0;
    window->do_callback();
    return 1;

  case UnmapNotify:
    Fl_X::i(window)->wait_for_expose = 1;
    return 1;

  case MapNotify:
    // supposedly an Expose event will come in and deiconize it
    return 1;

  case Expose:
    Fl_X::i(window)->wait_for_expose = 0;
#if 0
    // try to keep windows on top even if WM_TRANSIENT_FOR does not work:
    // opaque move/resize window managers do not like this, so I disabled it.
    if (Fl::first_window()->non_modal() && window != Fl::first_window())
      Fl::first_window()->show();
#endif

  case GraphicsExpose:
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
    fl_fix_focus();
    break;

  case FocusOut:
    if (window == xfocus) {xfocus = 0; fl_fix_focus();}
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
      if (Fl::event_state(FL_CTRL) && keysym == '-') buffer[0] = 0x1f; // ^_
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
	  0xff0b/*XK_Clear*/, FL_Insert, FL_Delete};
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

  case ConfigureNotify: {
    // We cannot rely on the x,y position in the configure notify event.
    // I now think this is an unavoidable problem with X: it is impossible
    // for a window manager to prevent the "real" notify event from being
    // sent when it resizes the contents, even though it can send an
    // artificial event with the correct position afterwards (and some
    // window managers do not send this fake event anyway)
    // So anyway, do a round trip to find the correct x,y:
    Window r, c; int X, Y, wX, wY; unsigned int m;
    XQueryPointer(fl_display, fl_xid(window), &r, &c, &X, &Y, &wX, &wY, &m);
    if (window->resize(X-wX, Y-wY,
		       xevent.xconfigure.width, xevent.xconfigure.height))
      resize_from_system = window;
    return 1;}
  }

  return Fl::handle(event, window);
}

////////////////////////////////////////////////////////////////

void Fl_Window::layout() {
  if (ow() == w() && oh() == h()) {
    if (this == resize_from_system) resize_from_system = 0;
    else if (i && (ox() != x() || oy() != y()))
      XMoveWindow(fl_display, i->xid, x(), y());
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
      XMoveResizeWindow(fl_display, i->xid, x(), y(),
			w()>0 ? w() : 1, h()>0 ? h() : 1);
      redraw(); // i->wait_for_expose = 1; (breaks menus somehow...)
    }
    Fl_Group::layout();
  }
}

////////////////////////////////////////////////////////////////
// Innards of Fl_Window::create()

void Fl_Window::create() {
  Fl_X::create(this, fl_visual, fl_colormap, -1);
}

extern char fl_show_iconic; // set by iconize() or Fl_arg -i switch
extern const Fl_Window* fl_modal_for;	// set by show(parent) or exec()

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
    attr.background_pixel=background;
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

  // figure out where OS really put window
  XWindowAttributes actual;
  XGetWindowAttributes(fl_display, x->xid, &actual);
  // tell Fl_Window about it
  w->resize(actual.x, actual.y, actual.width, actual.height);

  x->other_xid = 0;
  x->w = w; w->i = x;
  x->region = 0;
  x->wait_for_expose = 1;
  x->next = Fl_X::first;
  Fl_X::first = x;

  if (!w->parent() && w->border()) {
    // Communicate all kinds 'o junk to the X Window Manager:

    w->label(w->label(), w->iconlabel());

    XChangeProperty(fl_display, x->xid, wm_protocols,
 		    XA_ATOM, 32, 0, (uchar*)&wm_delete_window, 1);

    // send size limits and border:
    x->sendxjunk();

    // Setting this allows the window manager to use the window's class
    // to look up things like border colors and icons in the xrdb database:
    XChangeProperty(fl_display, x->xid, XA_WM_CLASS, XA_STRING, 8, 0,
		    (unsigned char *)w->xclass(), strlen(w->xclass()));

    if (fl_modal_for)
      XSetTransientForHint(fl_display, x->xid, fl_modal_for->i->xid);

    XWMHints hints;
    hints.input = True;
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
  }
}

////////////////////////////////////////////////////////////////
// Send X window stuff that can be changed over time:

void Fl_X::sendxjunk() {
  if (w->parent()) return; // it's not a window manager window!

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
		  fl_motif_wm_hints, fl_motif_wm_hints,
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
Fl_Window *Fl_Window::current_;
GC fl_gc;

void Fl_Window::make_current() {
  static GC gc;	// the GC used by all X windows with fl_visual
  if (!gc) gc = XCreateGC(fl_display, i->xid, 0, 0);
  fl_window = i->xid;
  fl_gc = gc;
  current_ = this;
  fl_clip_region(0);
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
// End of "$Id: Fl_x.cxx,v 1.78 2000/06/18 07:57:32 bill Exp $".
//
