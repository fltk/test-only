//
// "$Id: Fl_win32.cxx,v 1.202 2003/12/13 11:06:53 spitzak Exp $"
//
// _WIN32-specific code for the Fast Light Tool Kit (FLTK).
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

// This file contains win32-specific code for fltk which is always linked
// in.	Search other files for "_WIN32" or filenames ending in _win32.C
// for other system-specific code.

#include <config.h>
#include <fltk/events.h>
#include <fltk/Window.h>
#include <fltk/Style.h>
#include <fltk/win32.h>
#include <fltk/filename.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <time.h>
#include <winsock.h>
#include <commctrl.h>
#include <ctype.h>

using namespace fltk;

//
// USE_ASYNC_SELECT - define it if you have WSAAsyncSelect()...
//

#define USE_ASYNC_SELECT

//
// WM_SYNCPAINT is an "undocumented" message, which is finally defined in
// VC++ 6.0.
//
#ifndef WM_SYNCPAINT
#  define WM_SYNCPAINT 0x0088
#endif

#ifndef WM_MOUSELEAVE
#  define WM_MOUSELEAVE 0x02a3
#endif

#ifndef WM_MOUSEWHEEL
#  define WM_MOUSEWHEEL 0x020a
#endif

#ifndef WHEEL_DELTA
#  define WHEEL_DELTA 120	// according to MSDN.
#endif

#ifndef ENUM_CURRENT_SETTINGS
#  define ENUM_CURRENT_SETTINGS       ((DWORD)-1)
#endif

//
// WM_FLSELECT is the user-defined message that we get when one of
// the sockets has pending data, etc.
//

#define WM_FLSELECT	(WM_USER+0x0400)

// WM_MAKEWAITRETURN is the user-defined message that is used to try
// to make wait() return to the main loop so the windows version acts
// like GUI programs on more sensible operating systems
#define WM_MAKEWAITRETURN (WM_USER+0x401)

/**
 * returns pointer to the filename, or "" if name ends with '/' or ':'
 */
const char *filename_name(const char *name) {
  const char *p,*q;
  q = name;
  if (q[0] && q[1]==':') q += 2; // skip leading drive letter
  for (p = q; *p; p++) if (*p == '/' || *p == '\\') q = p+1;
  return q;
}

////////////////////////////////////////////////////////////////
// interface to poll/select call:

// fd's are only implemented for sockets.  Microsoft Windows does not
// have a unified IO system, so it doesn't support select() on files,
// devices, or pipes...
//
// Microsoft provides the Berkeley select() call and an asynchronous
// select function that sends a _WIN32 message when the select condition
// exists...

#ifndef USE_ASYNC_SELECT
static fd_set fdsets[3];
#endif // !USE_ASYNC_SELECT

#define POLLIN 1
#define POLLOUT 4
#define POLLERR 8

static int nfds = 0;
static int fd_array_size = 0;
static struct FD {
  int fd;
  short events;
  void (*cb)(int, void*);
  void* arg;
} *fd = 0;


void fltk::add_fd(int n, int events, FileHandler cb, void *v) {
  remove_fd(n,events);
  int i = nfds++;
  if (i >= fd_array_size) {
    fd_array_size = 2*fd_array_size+1;
    fd = (FD*)realloc(fd, fd_array_size*sizeof(FD));
  }
  fd[i].fd = n;
  fd[i].events = events;
  fd[i].cb = cb;
  fd[i].arg = v;

#ifdef USE_ASYNC_SELECT
  int mask = 0;
  if (events & POLLIN) mask |= FD_READ;
  if (events & POLLOUT) mask |= FD_WRITE;
  if (events & POLLERR) mask |= FD_CLOSE;
  WSAAsyncSelect(n, 0/*window*/, WM_FLSELECT, mask);
#else
  if (events & POLLIN) FD_SET(n, &fdsets[0]);
  if (events & POLLOUT) FD_SET(n, &fdsets[1]);
  if (events & POLLERR) FD_SET(n, &fdsets[2]);
#endif // USE_ASYNC_SELECT
}

void fltk::add_fd(int fd, FileHandler cb, void* v) {
  add_fd(fd, POLLIN, cb, v);
}

void fltk::remove_fd(int n, int events) {
  int i,j;
  for (i=j=0; i<nfds; i++) {
    if (fd[i].fd == n) {
      int e = fd[i].events & ~events;
      if (!e) continue; // if no events left, delete this fd
      fd[i].events = e;
    }
    // move it down in the array if necessary:
    if (j<i) {
      fd[j]=fd[i];
    }
    j++;
  }
  nfds = j;

#ifdef USE_ASYNC_SELECT
  WSAAsyncSelect(n, 0, 0, 0);
#else
  if (events & POLLIN) FD_CLR(unsigned(n), &fdsets[0]);
  if (events & POLLOUT) FD_CLR(unsigned(n), &fdsets[1]);
  if (events & POLLERR) FD_CLR(unsigned(n), &fdsets[2]);
#endif // USE_ASYNC_SELECT
}

// these pointers are set by the lock() function:
static void nothing() {}
void (*fl_lock_function)() = nothing;
void (*fl_unlock_function)() = nothing;

static void* thread_message_;
void* fltk::thread_message() {
  void* r = thread_message_;
  thread_message_ = 0;
  return r;
}

MSG fltk::msg;

UINT fl_wake_msg = 0;

// Wait up to the given time for any events or sockets to become ready,
// do the callbacks for the events and sockets.
// It *should* return negative on error, 0 if nothing happens before
// timeout, and >0 if any callbacks were done.	This version only
// returns zero if nothing happens during a 0.0 timeout, otherwise
// it returns 1.
static inline int fl_wait(double time_to_wait) {
  int have_message = 0;
  int timerid;

#ifndef USE_ASYNC_SELECT
  if (nfds) {
    // For _WIN32 we need to poll for socket input FIRST, since
    // the event queue is not something we can select() on...
    timeval t;
    t.tv_sec = 0;
    t.tv_usec = 0;

    fd_set fdt[3];
    fdt[0] = fdsets[0];
    fdt[1] = fdsets[1];
    fdt[2] = fdsets[2];

    if (::select(0,&fdt[0],&fdt[1],&fdt[2],&t)) {
      // We got something - do the callback!
      for (int i = 0; i < nfds; i ++) {
	int f = fd[i].fd;
	short revents = 0;
	if (FD_ISSET(f,&fdt[0])) revents |= POLLIN;
	if (FD_ISSET(f,&fdt[1])) revents |= POLLOUT;
	if (FD_ISSET(f,&fdt[2])) revents |= POLLERR;
	if (fd[i].events & revents) fd[i].cb(f, fd[i].arg);
      }
      time_to_wait = 0.0; // just peek for any messages
    } else {
      // we need to check them periodically, so set a short timeout:
      if (time_to_wait > .001) time_to_wait = .001;
    }
  }
#endif // USE_ASYNC_SELECT

  fl_unlock_function();
  if (time_to_wait < 2147483.648) {
    have_message = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    if (!have_message) {
      int t = (int)(time_to_wait * 1000.0 + .5);
      if (t <= 0) {fl_lock_function(); return 0;} // too short to measure
      timerid = SetTimer(NULL, 0, t, NULL);
      have_message = GetMessage(&msg, NULL, 0, 0);
      KillTimer(NULL, timerid);
    }
  } else {
    have_message = GetMessage(&msg, NULL, 0, 0);
  }
  fl_lock_function();

  // Execute the message we got, and all other pending messages:
  while (have_message) {
#ifdef USE_ASYNC_SELECT
    if (msg.message == WM_FLSELECT) {
      // Got notification for socket
      for (int i = 0; i < nfds; i ++)
	if (fd[i].fd == (int)msg.wParam) {
	  (fd[i].cb)(fd[i].fd, fd[i].arg);
	  break;
	}
      // looks like it is best to do the dispatch-message anyway:
    }
#endif
    if (msg.message == fl_wake_msg) {
      // This is used by awake() and by WndProc() in an attempt
      // to get wait() to return. That does not always work
      // unfortunately, as Windoze calls WndProc directly sometimes.
      // If that happens it gives up and calls flush() 
      if (msg.wParam) thread_message_ = (void*)msg.wParam;
    }
    // WM_MAKEWAITRETURN is used by WndProc to try to make wait()
    // return so the main loop recovers and can flush the display. We
    // purposely do not dispatch this message, as the desired result
    // has happened: this function will return.  For who knows what
    // reason, sometimes Windoze will call WndProc directly in
    // response to the Post, so it never gets here. This is detected
    // and at that point I give up and call flush().
    if (msg.message != WM_MAKEWAITRETURN) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    have_message = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
  }

  // This should return 0 if only timer events were handled:
  return 1;
}

// ready() is just like wait(0.0) except no callbacks are done:
static inline int fl_ready() {
  if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) return 1;
#ifdef USE_ASYNC_SELECT
  return 0;
#else
  timeval t;
  t.tv_sec = 0;
  t.tv_usec = 0;
  fd_set fdt[3];
  fdt[0] = fdsets[0];
  fdt[1] = fdsets[1];
  fdt[2] = fdsets[2];
  return ::select(0,&fdt[0],&fdt[1],&fdt[2],&t);
#endif // USE_ASYNC_SELECT
}

////////////////////////////////////////////////////////////////

static bool reload_info = true;

/** Return a "monitor" that surrounds all the monitors.
    If you have a single monitor, this returns a monitor structure that
    defines it. If you have multiple monitors this returns a fake monitor
    that surrounds all of them.
*/
const Monitor& Monitor::all() {
  static Monitor monitor;
  if (reload_info) {
    reload_info = false;

#if USE_MULTIMONITOR
    monitor.x_ = GetSystemMetrics(SM_XVIRTUALSCREEN);
    monitor.y_ = GetSystemMetrics(SM_YVIRTUALSCREEN);
    monitor.w_ = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    monitor.h_ = GetSystemMetrics(SM_CYVIRTUALSCREEN);
#endif

    // This is wrong, we should get the work area from the union of
    // all the monitors in monitor list
    RECT r;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
    monitor.work.set(r.left, r.top, r.right - r.left, r.bottom - r.top);

    DEVMODE mode;
    EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &mode);
#if !USE_MULTIMONITOR
    monitor.x_ = 0;
    monitor.y_ = 0;
    monitor.w_ = mode.dmPelsWidth;
    monitor.h_ = mode.dmPelsHeight;
#endif
    monitor.depth_ = mode.dmBitsPerPel;
    HDC screen = GetDC(0);
    monitor.dpi_x_ = (float)GetDeviceCaps(screen, LOGPIXELSX);
    monitor.dpi_y_ = (float)GetDeviceCaps(screen, LOGPIXELSY);

  }
  return monitor;
}

static Monitor* monitors = 0;
static int num_monitors=0;

#if USE_MULTIMONITOR
static int monitor_index;
static BOOL CALLBACK monitor_cb(HMONITOR hMonitor,
				HDC hdcMonitor,
				LPRECT lprcMonitor,
				LPARAM dwData)
{
  Monitor& m = monitors[monitor_index];
  MONITORINFO mi;
  mi.cbSize = sizeof(mi);
  GetMonitorInfo(hMonitor, &mi);
  m.x_ = mi.rcMonitor.left;
  m.y_ = mi.rcMonitor.top;
  m.w_ = mi.rcMonitor.right - m.x_;
  m.h_ = mi.rcMonitor.bottom - m.y_;
  m.work.x_ = mi.rcWork.left;
  m.work.y_ = mi.rcWork.top;
  m.work.w_ = mi.rcWork.right - m.work.x_;
  m.work.h_ = mi.rcWork.bottom - m.work.y_;
  // put the primary monitor first in list:
  if (monitor_index && (mi.dwFlags&MONITORINFOF_PRIMARY)) {
    Monitor t = monitors[0];
    monitors[0] = m;
    m = t;
  }
  monitor_index++;
  return 0;
}
#endif

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
#if USE_MULTIMONITOR
    num_monitors = GetSystemMetrics(SM_CMONITORS);
    monitors = new Monitor[num_monitors];
    // put the dpy and other info into them all:
    const Monitor& a = all();
    for (int j = 0; j < num_monitors; j++) monitors[j] = a;
    // now do the callback to get the per-monitor info:
    monitor_index = 0;
    EnumDisplayMonitors(0,0,monitor_cb,0);
#else
    num_monitors = 1; // indicate that Xinerama failed
    monitors = (Monitor*)(&all());
    // Guess if there are 2 monitors and assume they are side-by-side:
    int w = GetSystemMetrics(SM_CXSCREEN);
//  printf("SM_CXSCREEN = %d\n", w);
    // Guess for Nvidea which reports only one big monitor:
    if (w > monitors->h()*2) w = monitors->w()/2;
    if (w < monitors->w()) {
      num_monitors = 2;
      monitors = new Monitor[2];
      monitors[0] = monitors[1] = all();
      monitors[0].w_ = monitors[1].x_ = w;
      monitors[1].w_ -= w;
      monitors[0].work.w_ = w-monitors[0].work.x_;
      monitors[1].work.w_ -= monitors[0].work.w_;
      monitors[1].work.x_ = w;
    }
#endif
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

void fltk::get_mouse(int &x, int &y) {
  POINT p;
  GetCursorPos(&p);
  x = p.x;
  y = p.y;
}

////////////////////////////////////////////////////////////////
// code used for selections:

static char *selection_buffer[2];
static int selection_length[2];
static int selection_buffer_length[2];
static bool i_own_selection;

// call this when you create a selection:
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
  if (clipboard) {
    // set up for "delayed rendering":
    if (OpenClipboard(xid(Window::first()))) {
      EmptyClipboard();
      SetClipboardData(CF_TEXT, NULL);
      CloseClipboard();
    }
    i_own_selection = true;
  }
}

// Call this when a "paste" operation happens:
void fltk::paste(Widget &receiver, bool clipboard) {
  if (!clipboard || i_own_selection) {
    // We already have it, do it quickly without window server.
    // Notice that the text is clobbered if set_selection is
    // called in response to PASTE!
    e_text = selection_buffer[clipboard];
    e_length = selection_length[clipboard];
    receiver.handle(PASTE);
  } else {
    if (!OpenClipboard(NULL)) return;
    HANDLE h = GetClipboardData(CF_TEXT);
    if (h) {
      e_text = (LPSTR)GlobalLock(h);
      LPSTR a,b;
      a = b = e_text;
      while (*a) { // strip the CRLF pairs ($%$#@^)
	if (*a == '\r' && a[1] == '\n') a++;
	else *b++ = *a++;
      }
      *b = 0;
      e_length = b - e_text;
      receiver.handle(PASTE);
      GlobalUnlock(h);
    }
    CloseClipboard();
  }
}

// Communicating data to Win32 requires it to be in "global memory", this
// copies the given selection to such a block and returns it. It appears
// this block is usually handed to Windows and Windows deletes it.
HANDLE fl_global_selection(int clipboard) {
  HANDLE h = GlobalAlloc(GHND, selection_length[clipboard]+1);
  if (h) {
    LPSTR p = (LPSTR)GlobalLock(h);
    memcpy(p, selection_buffer[clipboard], selection_length[clipboard]);
    p[selection_length[clipboard]] = 0;
    GlobalUnlock(h);
  }
  return h;
}

////////////////////////////////////////////////////////////////

// Drag-n-drop requires GCC 3.x or a non-GNU compiler...
#if !defined(__GNUC__) || __GNUC__ >= 3
// Drag-n-drop is totally disabled until Wonko can figure out what is wrong
# define USE_DRAGDROP 1
#else
# define USE_DRAGDROP 0
#endif

#if USE_DRAGDROP
// I believe this was written by Matthias Melcher, correct?

#include <ole2.h>
#include <ShellApi.h>

static Window *dnd_target_window = 0;

/*
 * subclass the IDropTarget to receive data from DnD operations
 */
class FlDropTarget : public IDropTarget
{
  DWORD m_cRefCount;
  DWORD lastEffect;
  int px, py;
public:
  FlDropTarget() : m_cRefCount(0) { } // initialize
  HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, LPVOID *ppvObject ) {
    if (IID_IUnknown==riid || IID_IDropTarget==riid)
    {
      *ppvObject=this;
      ((LPUNKNOWN)*ppvObject)->AddRef();
      return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
  }
  ULONG STDMETHODCALLTYPE AddRef() { return ++m_cRefCount; }
  ULONG STDMETHODCALLTYPE Release() {
    long nTemp;
    nTemp = --m_cRefCount;
    //if (nTemp==0) delete this; // we don't really destroy it
    return nTemp;
  }
  HRESULT STDMETHODCALLTYPE DragEnter( IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) {
    if( !pDataObj ) return E_INVALIDARG;
    // set e_modifiers here from grfKeyState, set e_x and e_root_x
    // check if FLTK handles this drag and return if it can't (i.e. BMP drag without filename)
    POINT ppt; 
    e_x_root = ppt.x = pt.x; 
    e_y_root = ppt.y = pt.y;
    HWND hWnd = WindowFromPoint( ppt );
    Window *target = find( hWnd );
    if (target) {
      e_x = e_x_root-target->x();
      e_y = e_y_root-target->y();
    }
    dnd_target_window = target;
    px = pt.x; py = pt.y;
      // FLTK has no mechanism yet for the different drop effects, so we allow move and copy
    if ( target && handle( DND_ENTER, target ) )
      *pdwEffect = DROPEFFECT_MOVE|DROPEFFECT_COPY; //|DROPEFFECT_LINK;
    else
      *pdwEffect = DROPEFFECT_NONE;
    lastEffect = *pdwEffect;
    flush(); // get the display to update for local drags
    return S_OK;
  }
  HRESULT STDMETHODCALLTYPE DragOver( DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) {
    if ( px==pt.x && py==pt.y ) 
    {
      *pdwEffect = lastEffect;
      return S_OK;
    }
    if ( !dnd_target_window )
    {
      *pdwEffect = lastEffect = DROPEFFECT_NONE;
      return S_OK;
    }
    // set e_modifiers here from grfKeyState, set e_x and e_root_x
    e_x_root = pt.x; 
    e_y_root = pt.y;
    if (dnd_target_window) {
      e_x = e_x_root-dnd_target_window->x();
      e_y = e_y_root-dnd_target_window->y();
    }
    // Group will change DND_DRAG into DND_ENTER and DND_LEAVE if needed
    if ( handle( DND_DRAG, dnd_target_window ) )
      *pdwEffect = DROPEFFECT_MOVE|DROPEFFECT_COPY; //|DROPEFFECT_LINK;
    else 
      *pdwEffect = DROPEFFECT_NONE;
    px = pt.x; py = pt.y;
    lastEffect = *pdwEffect;
    flush(); // get the display to update for local drags
    return S_OK;
  }
  HRESULT STDMETHODCALLTYPE DragLeave() {
    if ( dnd_target_window )
    {
      handle( DND_LEAVE, dnd_target_window );
      flush(); // get the display to update for local drags
      dnd_target_window = 0;
    }
    return S_OK;
  }
  HRESULT STDMETHODCALLTYPE Drop( IDataObject *data, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) {
    if ( !dnd_target_window )
      return S_OK;
    Window *target_window = dnd_target_window;
    dnd_target_window = 0;
    e_x_root = pt.x; 
    e_y_root = pt.y;
    if (target_window) {
      e_x = e_x_root-target_window->x();
      e_y = e_y_root-target_window->y();
    }
    // tell FLTK that the user released an object on this widget
    if ( !handle( DND_RELEASE, target_window ) )
      return S_OK;

    Widget* target = belowmouse();
    if (!target) return S_OK;
    Widget *w = target;
    while (w->parent()) w = w->window();
    HWND hwnd = xid( (Window*)w );

    FORMATETC fmt = { 0 };
    STGMEDIUM medium = { 0 };
    fmt.tymed = TYMED_HGLOBAL;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.cfFormat = CF_TEXT;
    // if it is ASCII text, send an PASTE with that text
    if ( data->GetData( &fmt, &medium )==S_OK )
    {
      void *stuff = GlobalLock( medium.hGlobal );
      //long len = GlobalSize( medium.hGlobal );
      e_length = strlen( (char*)stuff ); // min(strlen, len)
      e_text = (char*)stuff;
      target->handle(PASTE); // e_text will be invalid after this call
      GlobalUnlock( medium.hGlobal );
      ReleaseStgMedium( &medium );
      SetForegroundWindow( hwnd );
      return S_OK;
    }
    fmt.tymed = TYMED_HGLOBAL;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.cfFormat = CF_HDROP;
    // if it is a pathname list, send an PASTE with a \n seperated list of filepaths
    if ( data->GetData( &fmt, &medium )==S_OK )
    {
      HDROP hdrop = (HDROP)medium.hGlobal;
      int i, n, nn = 0, nf = DragQueryFile( hdrop, (UINT)-1, 0, 0 );
      for ( i=0; i<nf; i++ ) nn += DragQueryFile( hdrop, i, 0, 0 );
      nn += nf;
      e_length = nn-1;
      char *dst = e_text = (char*)malloc(nn+1);
      for ( i=0; i<nf; i++ ) {
	n = DragQueryFile( hdrop, i, dst, nn );
	dst += n;
	if ( i<nf-1 ) *dst++ = '\n';
      }
      *dst = 0;
      target->handle(PASTE);
      free( e_text );
      ReleaseStgMedium( &medium );
      SetForegroundWindow( hwnd );
      return S_OK;
    }
    return S_OK;
  }
} flDropTarget;

#endif

////////////////////////////////////////////////////////////////

//
// USE_TRACK_MOUSE - define it if you have TrackMouseEvent()...
//
// Apparently, at least some versions of Cygwin/MingW don't provide
// the TrackMouseEvent() function.  You can define this by hand
// if you have it - this is only needed to support subwindow
// enter/leave notification under Windows.
//

#if !defined(__GNUC__)
#  define USE_TRACK_MOUSE
#endif // !__GNUC__

static bool mouse_event(Window *window, int what, int button,
			WPARAM wParam, LPARAM lParam)
{
  xmousewin = window;
  if (!window) return false;
#ifdef USE_TRACK_MOUSE
  TRACKMOUSEEVENT tme;
  tme.cbSize    = sizeof(TRACKMOUSEEVENT);
  tme.dwFlags   = TME_LEAVE;
  tme.hwndTrack = xid(window);
  _TrackMouseEvent(&tme);
#endif
  static int px, py, pmx, pmy;
  POINT pt;
  e_x = pt.x = (signed short)LOWORD(lParam);
  e_y = pt.y = (signed short)HIWORD(lParam);
  ClientToScreen(xid(window), &pt);
  e_x_root = pt.x;
  e_y_root = pt.y;
  while (window->parent()) {
    e_x += window->x();
    e_y += window->y();
    window = window->window();
  }

  unsigned long state = e_state & 0xff0000; // keep shift key states
#if 0
  // mouse event reports some shift flags, perhaps save them?
  if (wParam & MK_SHIFT) state |= SHIFT;
  if (wParam & MK_CONTROL) state |= CTRL;
#endif
  if (wParam & MK_LBUTTON) state |= BUTTON1;
  if (wParam & MK_MBUTTON) state |= BUTTON2;
  if (wParam & MK_RBUTTON) state |= BUTTON3;
  e_state = state;

  switch (what) {
  case 1: // double-click
    // This is not detecting triple-clicks, does anybody know how to fix?
    if (e_is_click) {e_clicks++; goto J1;}
  case 0: // single-click
    e_clicks = 0;
  J1:
    if (!grab_) SetCapture(xid(window));
    e_keysym = button;
    e_is_click = 1;
    px = pmx = e_x_root; py = pmy = e_y_root;
    if (handle(PUSH, window)) return true;
    // If modal is on and 0 is returned, we should turn off modal and
    // pass the event on to other widgets. The pass-on part is nyi!
    if (grab_) {exit_modal(); /* NYI */ }
    return false;

  case 2: // release:
    // WAS: this should turn off e_is_click if more than .2 second passed
    // since the push event!
    if (!grab_) ReleaseCapture();
    e_keysym = button;
    return handle(RELEASE,window);

  case 3: // move:
  default: // avoid compiler warning
    // MSWindows produces extra events even if mouse does not move, ignore em:
    if (e_x_root == pmx && e_y_root == pmy) return true;
    pmx = e_x_root; pmy = e_y_root;
    if (abs(e_x_root-px)>5 || abs(e_y_root-py)>5) e_is_click = 0;
    return handle(MOVE,window);
  }
}

// convert a MSWindows VK_x to an Fltk (X) Keysym:
// See also the inverse converter in get_key_win32.C
// This table is in numeric order by VK:
static const struct {unsigned short vk, fltk, extended;} vktab[] = {
  {VK_BACK,	BackSpaceKey},
  {VK_TAB,	TabKey},
  {VK_CLEAR,	Keypad5,	ClearKey},
  {VK_RETURN,	ReturnKey,	KeypadEnter},
  {VK_SHIFT,	LeftShiftKey,	RightShiftKey},
  {VK_CONTROL,	LeftCtrlKey,	RightCtrlKey},
  {VK_MENU,	LeftAltKey,	RightAltKey},
  {VK_PAUSE,	PauseKey},
  {VK_CAPITAL,	CapsLockKey},
  {VK_ESCAPE,	EscapeKey},
  {VK_SPACE,	SpaceKey},
  {VK_PRIOR,	Keypad9,	PageUpKey},
  {VK_NEXT,	Keypad3,	PageDownKey},
  {VK_END,	Keypad1,	EndKey},
  {VK_HOME,	Keypad7,	HomeKey},
  {VK_LEFT,	Keypad4,	LeftKey},
  {VK_UP,	Keypad8,	UpKey},
  {VK_RIGHT,	Keypad6,	RightKey},
  {VK_DOWN,	Keypad2,	DownKey},
  {VK_SNAPSHOT, PrintKey},	// does not work on NT
  {VK_INSERT,	Keypad0,	InsertKey},
  {VK_DELETE,	DecimalKey,	DeleteKey},
  {VK_LWIN,	LeftMetaKey},
  {VK_RWIN,	RightMetaKey},
  {VK_APPS,	MenuKey},
  {VK_MULTIPLY, MultiplyKey},
  {VK_ADD,	AddKey},
  {VK_SUBTRACT, SubtractKey},
  {VK_DECIMAL,	DecimalKey},
  {VK_DIVIDE,	DivideKey},
  {VK_NUMLOCK,	NumLockKey},
  {VK_SCROLL,	ScrollLockKey},
  {0xba,	';'},
  {0xbb,	'='},
  {0xbc,	','},
  {0xbd,	'-'},
  {0xbe,	'.'},
  {0xbf,	'/'},
  {0xc0,	'`'},
  {0xdb,	'['},
  {0xdc,	'\\'},
  {0xdd,	']'},
  {0xde,	'\''}
};
static int ms2fltk(int vk, int extended) {
  static unsigned short vklut[256];
  static unsigned short extendedlut[256];
  if (!vklut[1]) { // init the table
    unsigned int i;
    for (i = 0; i < 256; i++) vklut[i] = tolower(i);
    for (i=VK_F1; i<=VK_F16; i++) vklut[i] = i+(F1Key-VK_F1);
    for (i=VK_NUMPAD0; i<=VK_NUMPAD9; i++) vklut[i] = i+(Keypad0-VK_NUMPAD0);
    for (i = 0; i < sizeof(vktab)/sizeof(*vktab); i++) {
      vklut[vktab[i].vk] = vktab[i].fltk;
      extendedlut[vktab[i].vk] = vktab[i].extended;
    }
    for (i = 0; i < 256; i++) if (!extendedlut[i]) extendedlut[i] = vklut[i];
  }
  return extended ? extendedlut[vk] : vklut[vk];
}

#if USE_COLORMAP
extern HPALETTE fl_select_palette(void); // in color_win32.C
#endif

static Window* resize_from_system;
//  static Window* in_wm_paint;
//  static PAINTSTRUCT paint;

#define MakeWaitReturn() PostMessage(hWnd, WM_MAKEWAITRETURN, 0, 0)

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  //printf("Window %x msg %x\n", hWnd, uMsg);
  // Copy the message to msg so add_handler code can see it, it is
  // already there if this is called by DispatchMessage, but not if
  // Windows calls this directly.
  msg.hwnd = hWnd;
  msg.message = uMsg;
  msg.wParam = wParam;
  msg.lParam = lParam;
  //msg.time = ???
  //msg.pt = ???
  //msg.lPrivate = ???

  Window *window = find(hWnd);

  switch (uMsg) {

  case WM_SYNCPAINT :
  case WM_NCPAINT :
  case WM_ERASEBKGND :
    // Andreas Weitl - WM_SYNCPAINT needs to be passed to DefWindowProc
    // so that Windows can generate the proper paint messages...
    // Similarly, WM_NCPAINT and WM_ERASEBKGND need this, too...
    break;

  case WM_CAPTURECHANGED:
    if (grab_) {
      exit_modal();
      // Make wait() return so that the function that called modal()
      // can continue executing:
      MakeWaitReturn();
    }
    break;

#if 0
  case WM_NCACTIVATE:
    // This prevents the title bar highlighting from turning off while a
    // modal window is running. This is certainly necessary for menus
    // but they do the grab which seems to make this not happen.
    if (modal_) wParam = 1;
    break;
#endif

  case WM_QUIT: // this should not happen?
    fatal("WM_QUIT message");

  case WM_CLOSE: // user clicked close box
    if (!window) break;
    if (!modal_ || window == modal_) window->do_callback();
    return 1;

  case WM_PAINT: {
    if (!window) break;
    CreatedWindow *i = CreatedWindow::find(window);
    i->wait_for_expose = false;
#if 1
    // Merge the region into whatever is accumulated by fltk. I do this
    // by invalidating the fltk region and reading the resulting region
    // back:
    if (i->region) InvalidateRgn(hWnd, i->region, FALSE);
    else i->region = CreateRectRgn(0,0,0,0);
    GetUpdateRgn(hWnd, i->region, 0);
    // This convinces MSWindows we have painted whatever they wanted
    // us to paint, and stops it from sending WM_PAINT messages:
    ValidateRgn(hWnd, i->region);
    fltk::damage(1); // make flush() do something
    // Originally it called flush() directly here, but it appears to
    // be better to get wait() to return by posting a message. This
    // merges the damage together and waits for idle. Windows appears to
    // sometimes send WM_PAINT while we are painting the previous contents
    // and this minimizes the chances that will mess it up:
    MakeWaitReturn();
    //flush();
#else
    // This version was an attempt to fool fltk into doing what Windows
    // wants, which is to draw immediately in response to the WM_PAINT
    // event. This did not work as well as the above simpler version,
    // and also appeared to be no faster.
    // Since we can't merge or otherwise change the clip region, we
    // must first get rid of any other damage before doing the drawing:
    if (window->damage() || i->region) {
      window->flush();
      window->set_damage(0);
      if (i->region) {XDestroyRegion(i->region); i->region = 0;}
    }
    // Now get the damage region, so fltk has some idea what area it
    // needs to draw:
    i->region = CreateRectRgn(0,0,0,0);
    GetUpdateRgn(hWnd, i->region, 0);
    // Now draw it using Windows' gc and clip region:
    BeginPaint(i->xid, &paint);
    in_wm_paint = window; // makes it use the hdc from the paint struct
    window->flush();
    window->set_damage(0);
    if (i->region) {XDestroyRegion(i->region); i->region = 0;}
    EndPaint(i->xid, &paint);
    in_wm_paint = 0;
#endif
    } break;

  case WM_LBUTTONDOWN:	mouse_event(window, 0, 1, wParam, lParam); return 0;
  case WM_LBUTTONDBLCLK:mouse_event(window, 1, 1, wParam, lParam); return 0;
  case WM_LBUTTONUP:	mouse_event(window, 2, 1, wParam, lParam); return 0;
  case WM_MBUTTONDOWN:	mouse_event(window, 0, 2, wParam, lParam); return 0;
  case WM_MBUTTONDBLCLK:mouse_event(window, 1, 2, wParam, lParam); return 0;
  case WM_MBUTTONUP:	mouse_event(window, 2, 2, wParam, lParam); return 0;
  case WM_RBUTTONDOWN:	mouse_event(window, 0, 3, wParam, lParam); return 0;
  case WM_RBUTTONDBLCLK:mouse_event(window, 1, 3, wParam, lParam); return 0;
  case WM_RBUTTONUP:	mouse_event(window, 2, 3, wParam, lParam); return 0;

  case WM_MOUSEMOVE:    mouse_event(window, 3, 0, wParam, lParam); return 0;

  case WM_MOUSELEAVE:
    // In fltk2 we should only call handle(LEAVE) if the mouse is
    // not pointing at a window belonging to the application. This seems
    // to work, probably because the enter event has already been done
    // and has changed xmousewin to some other window:
    if (window == xmousewin) {xmousewin = 0; handle(LEAVE, window);}
    break;

  case WM_KILLFOCUS:
    window = 0;
  case WM_SETFOCUS:
    if (xfocus != window) {
      xfocus = window;
      fix_focus();
      MakeWaitReturn();
    }
    break;

  case WM_KEYDOWN:
  case WM_SYSKEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYUP:
    // save the keysym until we figure out the characters:
    e_keysym = ms2fltk(wParam,lParam&(1<<24));
    // See if TranslateMessage turned it into a WM_*CHAR message:
    if (PeekMessage(&msg, hWnd, WM_CHAR, WM_SYSDEADCHAR, PM_REMOVE)) {
      uMsg = msg.message;
      wParam = msg.wParam;
      lParam = msg.lParam;
    }
    // fall through to the character case:
  case WM_DEADCHAR:
  case WM_SYSDEADCHAR:
  case WM_CHAR:
  case WM_SYSCHAR: {
    static int lastkeysym;
    unsigned long state = e_state & 0xff000000; // keep the mouse button state
    // if GetKeyState is expensive we might want to comment some of these out:
    if (GetKeyState(VK_SHIFT)&~1) state |= SHIFT;
    if (GetKeyState(VK_CAPITAL)) state |= CAPSLOCK;
    if (GetKeyState(VK_CONTROL)&~1) state |= CTRL;
    // Alt gets reported for the Alt-GR switch on foreign keyboards.
    // so we need to check the event as well to get it right:
    if ((lParam&(1<<29)) //same as GetKeyState(VK_MENU)
	&& uMsg != WM_CHAR) state |= ALT;
    if (GetKeyState(VK_NUMLOCK)) state |= NUMLOCK;
    if (GetKeyState(VK_LWIN)&~1 || GetKeyState(VK_RWIN)&~1) {
      // _WIN32 bug?  GetKeyState returns garbage if the user hit the
      // Windows key to pop up start menu.  Sigh.
      if ((GetAsyncKeyState(VK_LWIN)|GetAsyncKeyState(VK_RWIN))&~1)
	state |= META;
    }
    if (GetKeyState(VK_SCROLL)) state |= SCROLLLOCK;
    e_state = state;
    if (lParam & (1<<31)) { // key up events.
      e_is_click = (e_keysym == lastkeysym);
      lastkeysym = 0;
      if (handle(KEYUP, window)) return 0;
      break;
    }
    // if same as last key, increment repeat count:
    if (lParam & (1<<30)) {
      e_clicks++;
    } else {
      e_clicks = 0;
    }
    lastkeysym = e_keysym;
    // translate to text:
    static char buffer[2];
    if (uMsg == WM_CHAR || uMsg == WM_SYSCHAR) {
      buffer[0] = char(wParam);
      if (e_keysym==ReturnKey || e_keysym==KeypadEnter) buffer[0] = '\r';
      e_length = 1;
    } else if (e_keysym >= Keypad && e_keysym <= KeypadLast) {
      buffer[0] = e_keysym-Keypad;
      e_length = 1;
    } else {
      buffer[0] = 0;
      e_length = 0;
    }
    e_text = buffer;
    // for (int i = lParam&0xff; i--;)
    if (window) while (window->parent()) window = window->window();
    if (handle(KEY,window)) return 0;
    break;}

  case WM_MOUSEWHEEL: {
    static int delta = 0; // running total of all motion
    delta += (SHORT)(HIWORD(wParam));
    if ((e_dy = delta / WHEEL_DELTA)) {
      delta -= e_dy * WHEEL_DELTA;
      handle(MOUSEWHEEL, window);
    }
    return 0;
  }

  case WM_GETMINMAXINFO:
    if (window) CreatedWindow::find(window)->set_minmax((LPMINMAXINFO)lParam);
    break;

  case WM_SHOWWINDOW:
    if (!window) break;
    if (wParam) { // Map event
      if (window->parent()) break; // ignore child windows

      // figure out where OS really put window
      RECT wr; GetClientRect(xid(window), &wr);
      POINT wul = { 0, 0 }; ClientToScreen(xid(window), &wul);

      // tell Window about it
      if (window->resize(wul.x, wul.y, wr.right, wr.bottom))
	resize_from_system = window;

      MakeWaitReturn();
    } else { // Unmap event
      CreatedWindow::find(window)->wait_for_expose = true;
    }
    break;

  case WM_SIZE:
    if (window && !window->parent()) {
      if (wParam == SIZE_MINIMIZED || wParam == SIZE_MAXHIDE) { // iconize
	CreatedWindow::find(window)->wait_for_expose = true;
      } else { // resize, deiconize
	// supposedly a Paint event will come in turn off iconize indicator
	if (window->resize(window->x(), window->y(),
			   LOWORD(lParam), HIWORD(lParam)))
	  resize_from_system = window;
      }
    }
    MakeWaitReturn();
    break;

  case WM_MAKEWAITRETURN:
    // This will be called if MakeWaitReturn fails because Stoopid Windows
    // called the WndProc directly. Instead do the best we can, which is
    // to flush the display.
    flush();
    break;

  case WM_MOVE:
    if (!window || window->parent()) break; // ignore child windows
#if 1
    if (window->resize((signed short)LOWORD(lParam),
		       (signed short)HIWORD(lParam),
		       window->w(), window->h()))
      resize_from_system = window;
    MakeWaitReturn();
#else
    // Don't bother having dragging windows call resize or layout, just
    // set the coordinates directly. This should be faster but I can't
    // see too much difference. This should also be copied to X version.
    window->x((signed short)LOWORD(lParam));
    window->y((signed short)HIWORD(lParam));
#endif
    break;

  case WM_SETCURSOR:
    if (window && LOWORD(lParam) == HTCLIENT) {
      while (window->parent()) window = window->window();
      CreatedWindow* i = CreatedWindow::find(window);
      if (i && i->cursor != default_cursor &&
	  !i->cursor_for->contains(belowmouse_))
	i->cursor = default_cursor;
      SetCursor(i->cursor);
      return 0;
    }
    break;

#if USE_COLORMAP
  case WM_QUERYNEWPALETTE :
    window->make_current();
    if (fl_select_palette()) InvalidateRect(hWnd, NULL, FALSE);
    break;

  case WM_PALETTECHANGED:
    if ((HWND)wParam != hWnd) {
      window->make_current();
      if (fl_select_palette()) UpdateColors(gc);
    }
    break;

#if 0
    // This seems to be called directly by CreateWindowEx so I can't
    // have stored the windowid yet and thus cannot find the Window!
  case WM_CREATE :
    window->make_current();
    fl_select_palette();
    break;
#endif
#endif

  case WM_DISPLAYCHANGE:
  case WM_SETTINGCHANGE:
    reload_info = true;
#if USE_MULTIMONITOR
    delete[] monitors;
#else
    if (num_monitors > 1) delete[] monitors;
#endif
    monitors = 0;
    num_monitors = 0;
  case WM_SYSCOLORCHANGE:
    reload_theme();
    break;

  case WM_DESTROYCLIPBOARD:
    i_own_selection = false;
    return 1;

  case WM_RENDERALLFORMATS:
    i_own_selection = false;
    // Windoze seems unhappy unless I do these two steps. Documentation
    // seems to vary on whether opening the clipboard is necessary or
    // is in fact wrong:
    CloseClipboard();
    OpenClipboard(NULL);
    // fall through...
  case WM_RENDERFORMAT:
    SetClipboardData(CF_TEXT, fl_global_selection(1));
    // Windoze also seems unhappy if I don't do this. Documentation very
    // unclear on what is correct:
    if (msg.message == WM_RENDERALLFORMATS) CloseClipboard();
    return 1;

  default:
    if (handle(0,0)) return 0;
    break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////////

/**
 * Sets the 4 return values to the thickness of the window border,
 * and returns the value to put into the window style. This is used
 * so that window positions are specified for the contents of the
 * window, rather than for the border, to match how X works.
 */
int CreatedWindow::borders(const Window* window,
			   int& dx,int& dy,int& dw,int& dh)
{
  if (!window->border() || window->override() || window->parent()) {
    dx = dy = dw = dh = 0;
    return WS_POPUP;
  } else if (window->maxw != window->minw || window->maxh != window->minh) { // resizable
    dx = GetSystemMetrics(SM_CXSIZEFRAME);
    dw = 2*dx;
    int bt = GetSystemMetrics(SM_CYCAPTION);
    int by = GetSystemMetrics(SM_CYSIZEFRAME);
    dy = bt+by;
    dh = bt+2*by;
    return WS_THICKFRAME | WS_MAXIMIZEBOX | WS_CAPTION;
  } else {
    dx = GetSystemMetrics(SM_CXFIXEDFRAME);
    dw = 2*dx;
    int bt = GetSystemMetrics(SM_CYCAPTION);
    int by = GetSystemMetrics(SM_CYFIXEDFRAME);
    dy = bt+by;
    dh = bt+2*by;
    return WS_DLGFRAME | WS_CAPTION;
  }
}

////////////////////////////////////////////////////////////////

void Window::layout() {
  UINT flags;
  if (layout_damage() & LAYOUT_WH)
    flags = SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_NOACTIVATE;
  else if (layout_damage() & LAYOUT_XY)
    flags = SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE;
  else
    flags = 0;
  Group::layout();
  if (this == resize_from_system) {
    resize_from_system = 0;
  } else if (i && flags) {
     int real_x = this->x(); int real_y = this->y();
     // this should not treat x,y differently from any other widget
     for (Widget* p = parent(); p && !p->is_window(); p = p->parent()) {
       real_x += p->x(); real_y += p->y();
     }
     int dx, dy, dw, dh; CreatedWindow::borders(this, dx, dy, dw, dh);
     SetWindowPos(i->xid, 0, real_x-dx, real_y-dy, w()+dw, h()+dh, flags);
     if (!(flags & SWP_NOSIZE)) {redraw(); /*i->wait_for_expose = true;*/}
  }
}

////////////////////////////////////////////////////////////////
// Innards of Window::create():

void Window::create() {
  CreatedWindow::create(this);
}

const Window* fl_mdi_window = 0; // set by show_inside()
HCURSOR fltk::default_cursor;

void CreatedWindow::create(Window* window) {

  static bool registered = false;
  if (!registered) {
    registered = true;
    static WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hInstance = xdisplay;
    HICON icon = (HICON)window->icon();
    if (!icon) {
      icon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(101),
			      IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR|LR_SHARED);
      if (!icon) icon = LoadIcon(NULL, IDI_APPLICATION);
    }
    wc.hIcon = wc.hIconSm = icon;
    if (!default_cursor) default_cursor = LoadCursor(NULL, IDC_ARROW);
    wc.hCursor = default_cursor;
    //uchar r,g,b; get_color(GRAY,r,g,b);
    //wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(r,g,b));
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "fltk";
    wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&wc);
    fl_wake_msg = RegisterWindowMessage("fltk::ThreadWakeup");
    // This is needed or multiple DLL's get confused (?):
    // No, doing this makes none of the windows appear:
    //UnregisterClass(wc.lpszClassName, xdisplay);
#if USE_DRAGDROP
    OleInitialize(0L);
#endif
  }

  HWND parent;
  DWORD style;
  DWORD styleEx;

  int xp = window->x();
  int yp = window->y();

  int dx, dy, dw, dh;

  if (window->parent()) {
    for (Widget* p=window->parent(); p && !p->is_window(); p=p->parent()) {
      xp += p->x(); yp += p->y();
    }
    style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD;
    styleEx = WS_EX_LEFT | WS_EX_WINDOWEDGE | WS_EX_CONTROLPARENT;
    parent = fltk::xid(window->window());
    dx=dy=dw=dh=0;
  } else {
    style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | borders(window, dx,dy,dw,dh);
    styleEx = WS_EX_LEFT | WS_EX_WINDOWEDGE | WS_EX_CONTROLPARENT;
    // we don't want an entry in the task list for menuwindows or tooltips!
    // This seems to have no effect on NT, maybe for Win2K?
    if (window->override()) {
      styleEx |= WS_EX_TOOLWINDOW;
    } else if (!window->contains(modal())) {
      style |= WS_SYSMENU | WS_MINIMIZEBOX;
    }

    xp = window->x(); if (xp != USEDEFAULT) xp -= dx;
    yp = window->y(); if (yp != USEDEFAULT) yp -= dy;

    if (window->child_of() && window->child_of()->shown()) {
      parent = window->child_of()->i->xid;
    } else if (fl_mdi_window) {
      parent = fl_mdi_window->i->xid;
    } else {
      parent = 0;
    }
  }

  CreatedWindow* x = new CreatedWindow;
  x->backbuffer.xid = 0;
  x->backbuffer.dc = 0;
  x->window = window; window->i = x;
  x->region = 0;
  x->cursor = default_cursor;
  x->cursor_for = 0;
  x->xid = CreateWindowEx(
    styleEx,
    "fltk", window->label(), style,
    xp, yp, window->w()+dw, window->h()+dh,
    parent,
    NULL, // menu
    xdisplay,
    NULL // creation parameters
    );
  x->dc = GetDC(x->xid);
  SetTextAlign(x->dc, TA_BASELINE|TA_LEFT);
  SetBkMode(x->dc, TRANSPARENT);

#if 0 // WAS: Doing this completely breaks NT, the title bar loses highlight:
  if (window->override()) {
    UINT posflags = SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING;
    if(style & WS_POPUP)
      posflags |= SWP_NOACTIVATE;
    SetWindowPos(x->xid, HWND_TOPMOST, 0, 0, 0, 0, posflags);
  }
#endif

  x->wait_for_expose = true;
  x->next = CreatedWindow::first;
  CreatedWindow::first = x;


#if USE_DRAGDROP
  RegisterDragDrop(x->xid, &flDropTarget);
#endif
}

////////////////////////////////////////////////////////////////

HINSTANCE fltk::xdisplay = GetModuleHandle(NULL);

void fltk::open_display() {}
void fltk::close_display() {}

void Window::size_range_() {
  size_range_set = 1;
}

void CreatedWindow::set_minmax(LPMINMAXINFO minmax)
{
  int dx, dy, dw, dh; borders(window, dx, dy, dw, dh);

  minmax->ptMinTrackSize.x = window->minw + dw;
  minmax->ptMinTrackSize.y = window->minh + dh;
  if (window->maxw) {
    minmax->ptMaxTrackSize.x = window->maxw + dw;
    minmax->ptMaxSize.x = window->maxw + dw;
  }
  if (window->maxh) {
    minmax->ptMaxTrackSize.y = window->maxh + dw;
    minmax->ptMaxSize.y = window->maxh + dw;
  }
}

/**
 * Returns true if the window is shown but is iconized.
 */
bool Window::iconic() const {
  return i && IsIconic(i->xid);
}

////////////////////////////////////////////////////////////////

void Window::label(const char *name,const char *iname) {
  Widget::label(name);
  iconlabel_ = iname;
  if (i && !parent()) {
    if (!name) name = "";
    SetWindowText(i->xid, name);
    // if (!iname) iname = filename_name(name);
    // should do something with iname here, it should label the taskbar icon
  }
}

////////////////////////////////////////////////////////////////
// Drawing context

const Window *Window::current_;
HDC fltk::gc;

static HDC screen_gc; // result from last getDC()

/** Return an arbitrary HDC which you can use for Win32 functions that
    need one as an argument. The returned value is short-lived and may
    be destroyed by the next call to this or to the next call to destroy
    a window.
*/
HDC fltk::getDC() {
  if (screen_gc) {
    ReleaseDC(0, screen_gc);
    screen_gc = 0;
  }
  if (gc) return gc;
  screen_gc = GetDC(0);
  return screen_gc;
}

// make X drawing go into this window (called by subclass flush() impl.)
void Window::make_current() const {
//    if (this == in_wm_paint)
//      gc = paint.hdc;
//    else
    i->make_current();
#if USE_COLORMAP
  // Windows maintains a hardware and software color palette; the
  // SelectPalette() call updates the current soft->hard mapping
  // for all drawing calls, so we must select it here before any
  // code does any drawing...
  fl_select_palette();
#endif // USE_COLORMAP
  current_ = this;
}

// Code used to switch output to an off-screen window.  See macros in
// win32.h which save the old state in local variables.

HDC fltk::makeDC(HBITMAP bitmap) {
  HDC new_gc = CreateCompatibleDC(gc);
  SetTextAlign(new_gc, TA_BASELINE|TA_LEFT);
  SetBkMode(new_gc, TRANSPARENT);
#if USE_COLORMAP
  if (xpalette) SelectPalette(new_gc, xpalette, FALSE);
#endif
  SelectObject(new_gc, bitmap);
  return new_gc;
}

void fltk::copy_offscreen(int x,int y,int w,int h,
			  HBITMAP bitmap,int srcx,int srcy) {
  HDC new_gc = CreateCompatibleDC(gc);
  SelectObject(new_gc, bitmap);
  BitBlt(gc, x, y, w, h, new_gc, srcx, srcy, SRCCOPY);
  DeleteDC(new_gc);
}

extern void fl_font_rid();

// clean up after Windows
static struct Cleanup { ~Cleanup(); } cleanup;

Cleanup::~Cleanup() {
  // nasty but works (I think) - deallocates GDI resources in windows
  while (CreatedWindow* x = CreatedWindow::first) x->window->destroy();

  // get rid of allocated font resources
  fl_font_rid();
}

////////////////////////////////////////////////////////////////

#ifndef SPI_GETWHEELSCROLLLINES
#define SPI_GETWHEELSCROLLLINES   104
#endif

#ifndef WHEEL_PAGESCROLL
#define WHEEL_PAGESCROLL	(UINT_MAX) /* Scroll one page */
#endif

static Color win_color(int wincol) {
  int R = wincol&0xff;
  int G = (wincol >> 8)&0xff;
  int B = (wincol >> 16)&0xff;
  Color col = color(R, G, B);
  if (col) return col;
  return BLACK;
}

static int win_fontsize(int winsize) {
  if (winsize < 0) return -winsize; // -charsize: which is what FLTK uses
  if (winsize == 0) return 12; // pick any good size.  12 is good!
  return winsize*3/4; // cellsize: convert to charsize
}

bool fltk::system_theme() {

  Color background = win_color(GetSysColor(COLOR_BTNFACE));
  Color foreground = win_color(GetSysColor(COLOR_BTNTEXT));
  Color select_background = win_color(GetSysColor(COLOR_HIGHLIGHT));
  Color select_foreground = win_color(GetSysColor(COLOR_HIGHLIGHTTEXT));
  Color text_background = win_color(GetSysColor(COLOR_WINDOW));
  Color text_foreground = win_color(GetSysColor(COLOR_WINDOWTEXT));
  Color menuitem_background = win_color(GetSysColor(COLOR_MENU));
  Color menuitem_foreground = win_color(GetSysColor(COLOR_MENUTEXT));
  Color tooltip_background = win_color(GetSysColor(COLOR_INFOBK));
  Color tooltip_foreground = win_color(GetSysColor(COLOR_INFOTEXT));
// Windows doesn't seem to honor this one
// Color slider_background = win_color(GetSysColor(COLOR_SCROLLBAR));

  fltk::set_background(background);
  Widget::default_style->labelcolor_ = foreground;
  Widget::default_style->highlight_textcolor_ = foreground;
  Widget::default_style->color_ = text_background;
  Widget::default_style->textcolor_ = text_foreground;
  Widget::default_style->selection_color_ = select_background;
  Widget::default_style->selection_textcolor_ = select_foreground;

  Style* style;

  if ((style = Style::find("ScrollBar"))) {
//    style->color = lerp(slider_background, text_background, .5);
    style->color_ = lerp(background, text_background, .5);
  }

  if (menuitem_background != background || menuitem_foreground != foreground) {
    if ((style = Style::find("MenuBar"))) {
      style->color_ = menuitem_background;
      style->textcolor_ = menuitem_foreground;
//    style->selection_color_ = select_background;
//    style->selection_textcolor_ = select_foreground;
    }
    if ((style = Style::find("PopupMenu"))) {
      style->color_ = menuitem_background;
      style->textcolor_ = menuitem_foreground;
//    style->selection_color_ = select_background;
//    style->selection_textcolor_ = select_foreground;
    }
  }

/* This is the same as the defaults:
  if ((style = Style::find("menu title"))) {
    style->highlight_color_ = GRAY75;
    style->highlight_textcolor_ = foreground;
    style->selection_color_ = GRAY75;
    style->selection_textcolor_ = foreground;
  }
*/

  if ((style = Style::find("MenuBar"))) {
    style->highlight_color_ = GRAY75; // enable title highlightig
  }

  if ((style = Style::find("Tooltip"))) {
    style->color_ = tooltip_background;
    style->textcolor_ = tooltip_foreground;
  }

  /*
     Windows font stuff

     It looks Windows has just three separate fonts that it actually
     uses for stuff replaced by FLTK.  But the "Display Properties"
     dialog has a lot more fonts that you can set?  Wrong, look again.
     Some of the fonts are duplicates and another doesn't do anything!
     It has fonts for the titlebar and icons which we don't have to worry
     about, a menu font which is used for menubars and menu items, a
     status font which is for status bars and tooltips, and a message
     box font which is used for everything else.  Except that it's not
     used by everything else;  almost all non-menu widgets in every
     application I tested did not respond to font changes.  The fonts
     are apparently hard coded by the applications which seems to me to
     bad programming considering that Windows has an adequate system for
     allowing the user to specify font preferences.  This is especially
     true of Microsoft applications and Windows itself!  We will allow
     FLTK applications to automatically use the fonts specified by the
     user.

     CET
  */

  NONCLIENTMETRICS ncm;
  int sncm = sizeof(ncm);
  ncm.cbSize = sncm;
  SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sncm, &ncm, SPIF_SENDCHANGE);

  Font* font; float size;

  // get font info for regular widgets from LOGFONT structure
  font = fltk::font((const char*)ncm.lfMessageFont.lfFaceName,
		    (ncm.lfMessageFont.lfWeight >= 600 ? BOLD : 0) +
		    (ncm.lfMessageFont.lfItalic ? ITALIC : 0));
  size = float(win_fontsize(ncm.lfMessageFont.lfHeight));

  Widget::default_style->labelfont_ = font;
  Widget::default_style->textfont_ = font;
  Widget::default_style->labelsize_ = size;
  Widget::default_style->textsize_ = size;

  // get font info for menu items from LOGFONT structure
  font = fltk::font((const char*)ncm.lfMenuFont.lfFaceName,
		    (ncm.lfMenuFont.lfWeight >= 600 ? BOLD : 0) +
		    (ncm.lfMenuFont.lfItalic ? ITALIC : 0));
  size = float(win_fontsize(ncm.lfMenuFont.lfHeight));
  if ((style = Style::find("MenuBar"))) {
    style->textfont_ = font;
    style->textsize_ = size;
  }
  if ((style = Style::find("PopupMenu"))) {
    style->textfont_ = font;
    style->textsize_ = size;
  }

  if ((style = Style::find("Tooltip"))) {
    // get font info for tooltips from LOGFONT structure
    font = fltk::font((const char*)ncm.lfStatusFont.lfFaceName,
		      (ncm.lfStatusFont.lfWeight >= 600 ? BOLD : 0) +
		      (ncm.lfStatusFont.lfItalic ? ITALIC : 0));
    size = float(win_fontsize(ncm.lfStatusFont.lfHeight));

    style->textfont_ = font;
    style->textsize_ = size;
  }

  // grab mousewheel stuff from Windows
  UINT delta;
  SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, (PVOID)&delta, 0);
  if (delta == WHEEL_PAGESCROLL) Style::wheel_scroll_lines_ = 10000;
  else Style::wheel_scroll_lines_ = (int)delta;

  // CET - FIXME - do encoding stuff
  return true;
}

//
// End of "$Id: Fl_win32.cxx,v 1.202 2003/12/13 11:06:53 spitzak Exp $".
//
