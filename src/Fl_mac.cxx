//
// "$Id: Fl_mac.cxx,v 1.2 2002/09/09 01:39:58 spitzak Exp $"
//
// MacOS specific code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

//// From the inner edge of a MetroWerks CodeWarrior CD:
// (without permission)
//
// Three Compiles for 68Ks under the sky,
// Seven Compiles for PPCs in their fragments of code,
// Nine Compiles for Mortal Carbon doomed to die,
// One Compile for Mach-O Cocoa on its Mach-O throne,
// in the Land of MacOS X where the Drop-Shadows lie.
// 
// One Compile to link them all, One Compile to merge them,
// One Compile to copy them all and in the bundle bind them,
// in the Land of MacOS X where the Drop-Shadows lie.

#include <config.h>
#include <fltk/Fl.h>
#include <fltk/x.h>
#include <fltk/Fl_Window.h>
#include <fltk/Fl_Style.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// #define DEBUG_SELECT		// UNCOMMENT FOR SELECT()/THREAD DEBUGGING
#ifdef DEBUG_SELECT
#include <stdio.h>	// testing
#define DEBUGMSG(msg)		fprintf(stderr, msg);
#define DEBUGPERRORMSG(msg)	perror(msg)
#else
#define DEBUGMSG(msg)
#define DEBUGPERRORMSG(msg)
#endif /*DEBUGSELECT*/

////////////////////////////////////////////////////////////////
// interface to select call:
//
// WAS: This is similar to the Fl_x.cxx version but with USE_POLL set to 0
// I modified this from Matt's version to more closely match the X
// version, with the addition of the locks for the parallel thread. It
// may be better to kill the parallel thread and launch it on the next
// Fl_wait call, however.
//
// Surely this can be done without a second thread, what we need
// is a file descriptor that becomes ready when an Apple UI event
// is pending.

static fd_set fdsets[3];
static int maxfd;
#define POLLIN  1
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
static int G_pipe[2] = { 0,0 };		// work around pthread_cancel() problem
static pthread_mutex_t select_mutex;	// lock for above data

void Fl::add_fd(int n, int events, void (*cb)(int, void*), void *v) {
  remove_fd(n, events);
  pthread_mutex_lock(&select_mutex);
  int i = nfds++;
  if (i >= fd_array_size) {
    fd_array_size = 2*fd_array_size+1;
    fd = (FD*)realloc(fd, fd_array_size*sizeof(FD));
  }
  fd[i].cb = cb;
  fd[i].arg = v;
  fd[i].fd = n;
  fd[i].events = events;
  if (events & POLLIN) FD_SET(n, &fdsets[0]);
  if (events & POLLOUT) FD_SET(n, &fdsets[1]);
  if (events & POLLERR) FD_SET(n, &fdsets[2]);
  if (n > maxfd) maxfd = n;
  pthread_mutex_unlock(&select_mutex);
}

void Fl::add_fd(int fd, void (*cb)(int, void*), void* v) {
  Fl::add_fd(fd, POLLIN, cb, v);
}

void Fl::remove_fd(int n, int events) {
  pthread_mutex_lock(&select_mutex);
  int i,j;
  for (i=j=0; i<nfds; i++) {
    if (fd[i].fd == n) {
      int e = fd[i].events & ~events;
      if (!e) continue; // if no events left, delete this fd
      fd[i].events = e;
    }
    // move it down in the array if necessary:
    if (j<i) {
      fd[j] = fd[i];
    }
    j++;
  }
  nfds = j;
  if (events & POLLIN) FD_CLR(n, &fdsets[0]);
  if (events & POLLOUT) FD_CLR(n, &fdsets[1]);
  if (events & POLLERR) FD_CLR(n, &fdsets[2]);
  if (n == maxfd) maxfd--;
  pthread_mutex_unlock(&select_mutex);
}

enum { kEventClassFLTK = 'fltk' };
enum { kEventFLTKBreakLoop = 1, kEventFLTKDataReady };

// DATA READY THREAD
//    Separate thread, watches for changes in user's file descriptors.
//    Sends a 'data ready event' to the main thread if any change.
static void *dataready_thread(void *userdata)
{
  EventRef drEvent;
  CreateEvent( 0, kEventClassFLTK, kEventFLTKDataReady,
               0, kEventAttributeUserEvent, &drEvent);
  EventQueueRef eventqueue = (EventQueueRef)userdata;

  // Thread safe local copy
  pthread_mutex_lock(&select_mutex);
  int maxfd = ::maxfd;
  fd_set r = fdsets[0];
  fd_set w = fdsets[1];
  fd_set x = fdsets[2];
  pthread_mutex_unlock(&select_mutex);

  // TACK ON FD'S FOR 'CANCEL PIPE'
  FD_SET(G_pipe[0], &r);
  if ( G_pipe[0] > maxfd ) maxfd = G_pipe[0];

  // FOREVER UNTIL THREAD CANCEL OR ERROR
  while ( 1 )
  {
    timeval t = { 1000, 0 };	// 1000 seconds;
    int ret = ::select(maxfd+1, &r, &w, &x, &t);
    pthread_testcancel();	// OSX 10.0.4 and under: need to do this
                          // so parent can cancel us :(
    switch ( ret )
    {
      case  0:	// NO DATA
        continue;
      case -1:	// ERROR
      {
        DEBUGPERRORMSG("CHILD THREAD: select() failed");
        return(NULL);		// error? exit thread
      }
      default:	// DATA READY
      {
        DEBUGMSG("DATA READY EVENT: SENDING\n");
        PostEventToQueue(eventqueue, drEvent, kEventPriorityStandard );
        return(NULL);		// done with thread
      }
    }
  }
}

// Main thread calls this when it gets the above data-ready message:
// Check to see what's ready, and invoke user's cb's
static void HandleDataReady()
{
  DEBUGMSG("DATA READY EVENT: RECEIVED\n");
  pthread_mutex_lock(&select_mutex);
  int maxfd = ::maxfd;
  fd_set r = fdsets[0];
  fd_set w = fdsets[1];
  fd_set x = fdsets[2];
  pthread_mutex_unlock(&select_mutex);
  timeval t = { 0, 1 };		// quick check
  switch (::select(maxfd+1, &r, &w, &x, &t)) {
  case 0:		// NO DATA
    break;
  case -1:	// ERROR
    break;
  default:	// DATA READY
    for (int i=0; i<nfds; i++) {
      // fprintf(stderr, "CHECKING FD %d OF %d (%d)\n", i, nfds, fd[i].fd);
      int f = fd[i].fd;
      short revents = 0;
      if (FD_ISSET(f, &r)) revents |= POLLIN;
      if (FD_ISSET(f, &w)) revents |= POLLOUT;
      if (FD_ISSET(f, &x)) revents |= POLLERR;
      if (fd[i].events & revents) {
	DEBUGMSG("DOING CALLBACK: ");
	fd[i].cb(f, fd[i].arg);
	DEBUGMSG("DONE\n");
      }
    }
  }
}

// these pointers are set by the Fl::lock() function:
static void nothing() {}
void (*fl_lock_function)() = nothing;
void (*fl_unlock_function)() = nothing;

static int got_events = 0;

////////////////////////////////////////////////////////////////

// public variables
//Handle fl_system_menu;
//Fl_Sys_Menu_Bar *fl_sys_menu_bar = 0;

static WindowRef fl_capture = 0;
static WindowRef fl_os_capture = 0;

static Fl_Window* resize_from_system;
static CursPtr default_cursor_ptr;
static Cursor default_cursor;
CursHandle fl_default_cursor;

/**
 * handle Apple Menu items (can be created using the Fl_Sys_Menu_Bar
 * returns eventNotHandledErr if the menu item could not be handled
 */
OSStatus HandleMenu( HICommand *cmd )
{
  OSStatus ret = eventNotHandledErr;
  // attributes, commandIDm menu.menuRef, menu.menuItemIndex
#if 0 // NYI
  UInt32 ref;
  OSErr rrc = GetMenuItemRefCon( cmd->menu.menuRef, cmd->menu.menuItemIndex, &ref );
  //printf( "%d, %08x, %08x, %d, %d, %8x\n", rrc, cmd->attributes, cmd->commandID, cmd->menu.menuRef, cmd->menu.menuItemIndex, rrc );
  if ( rrc==noErr && ref )
  {
    Fl_Menu_Item *m = (Fl_Menu_Item*)ref;
    //printf( "Menu: %s\n", m->label() );
    fl_sys_menu_bar->picked( m );
    if ( m->flags & FL_MENU_TOGGLE ) // update the menu toggle symbol
      SetItemMark( cmd->menu.menuRef, cmd->menu.menuItemIndex, (m->flags & FL_MENU_VALUE ) ? 0x12 : 0 );
    if ( m->flags & FL_MENU_RADIO ) // update all radio buttons in this menu
    {
      Fl_Menu_Item *j = m;
      int i = cmd->menu.menuItemIndex;
      for (;;)
      {
        if ( j->flags & FL_MENU_DIVIDER )
          break;
        j++; i++;
        if ( !j->text || !j->radio() )
          break;
        SetItemMark( cmd->menu.menuRef, i, ( j->flags & FL_MENU_VALUE ) ? 0x13 : 0 );
      }
      j = m-1; i = cmd->menu.menuItemIndex-1;
      for ( ; i>0; j--, i-- )
      {
        if ( !j->text || j->flags&FL_MENU_DIVIDER || !j->radio() )
          break;
        SetItemMark( cmd->menu.menuRef, i, ( j->flags & FL_MENU_VALUE ) ? 0x13 : 0 );
      }
      SetItemMark( cmd->menu.menuRef, cmd->menu.menuItemIndex, ( m->flags & FL_MENU_VALUE ) ? 0x13 : 0 );
    }
    ret = noErr; // done handling this event
  }
#endif
  HiliteMenu(0);
  return ret;
}


/**
 * We can make every event pass through this function
 * - mouse events need to be manipulated to use a mouse focus window
 * - keyboard, mouse and some window events need to quit the Apple Event Loop
 *   so FLTK can continue its own management
 */
static pascal OSStatus carbonDispatchHandler( EventHandlerCallRef nextHandler, EventRef event, void *userData )
{
  OSStatus ret = eventNotHandledErr;
  HICommand cmd;

  fl_lock_function();

  got_events = 1;

  switch ( GetEventClass( event ) )
  {
  case kEventClassMouse:
    switch ( GetEventKind( event ) )
    {
    case kEventMouseUp:
    case kEventMouseMoved:
    case kEventMouseDragged:
      if ( fl_capture )
        ret = SendEventToEventTarget( event, GetWindowEventTarget( fl_capture ) );
      else if ( fl_os_capture )
        ret = SendEventToEventTarget( event, GetWindowEventTarget( fl_os_capture ) );
      break;
    }
    break;
  case kEventClassCommand:
    switch (GetEventKind( event ) )
    {
      case kEventCommandProcess:
        GetEventParameter( event, kEventParamDirectObject, typeHICommand, NULL, sizeof(HICommand), NULL, &cmd );
        ret = HandleMenu( &cmd );
        break;
    }
    break;
  case kEventClassFLTK:
    switch ( GetEventKind( event ) )
    {
    case kEventFLTKBreakLoop:
      ret = noErr;
      break;
    case kEventFLTKDataReady:
      HandleDataReady();
      ret = noErr;
      break;
    }
  }
  if ( ret == eventNotHandledErr )
    ret = CallNextEventHandler( nextHandler, event ); // let the OS handle the activation, but continue to get a click-through effect
  QuitApplicationEventLoop();

  fl_unlock_function();

  return ret;
}

/**
 * This callback quits the loop immediately so that Fl::wait() will
 * return after the pending time happens.
 */
static pascal void timerProcCB( EventLoopTimerRef, void* )
{
  fl_lock_function();
  QuitApplicationEventLoop();
  fl_unlock_function();
}

/**
 * Break the current event loop so that Fl::wait() returns, hopefully
 * after any events that already exist.
 */
static void breakMacEventLoop()
{
  EventRef breakEvent;
  // fl_lock_function(); // WAS: I don't think the lock is needed
  CreateEvent( 0, kEventClassFLTK, kEventFLTKBreakLoop, 0,
	       kEventAttributeUserEvent, &breakEvent );
  PostEventToQueue( GetCurrentEventQueue(), breakEvent,
		    kEventPriorityLow /*kEventPriorityStandard*/ );
  ReleaseEvent( breakEvent );
  //fl_unlock_function();
}

/**
 * Wait up to the given time for any events or sockets to become ready,
 * do the callbacks for the events and sockets. Returns non-zero if
 * anything happened during the time period.
 */
static inline int fl_wait(double time) 
{
  static bool been_here = 0;
  static RgnHandle rgn;
  
  // initialize events and a region that enables mouse move events
  if (!been_here) {
    rgn = NewRgn();
    Point mp;
    GetMouse(&mp);
    SetRectRgn(rgn, mp.h, mp.v, mp.h, mp.v);
    SetEventMask(everyEvent);
    been_here = 1;
  }
  OSStatus ret;
  static EventTargetRef target = 0;
  static EventLoopTimerRef timer = 0;
  if ( !target ) {
    target = GetEventDispatcherTarget();
    EventHandlerUPP dispatchHandler =
      NewEventHandlerUPP( carbonDispatchHandler );
    static EventTypeSpec dispatchEvents[] = {
        { kEventClassWindow, kEventWindowShown },
        { kEventClassWindow, kEventWindowHidden },
        { kEventClassWindow, kEventWindowActivated },
        { kEventClassWindow, kEventWindowDeactivated },
        { kEventClassWindow, kEventWindowClose },
        { kEventClassKeyboard, kEventRawKeyDown },
        { kEventClassKeyboard, kEventRawKeyRepeat },
        { kEventClassKeyboard, kEventRawKeyUp },
        { kEventClassKeyboard, kEventRawKeyModifiersChanged },
        { kEventClassMouse, kEventMouseDown },
        { kEventClassMouse, kEventMouseUp },
        { kEventClassMouse, kEventMouseMoved },
        { kEventClassMouse, kEventMouseWheelMoved },
        { kEventClassMouse, kEventMouseDragged },
        { kEventClassFLTK, kEventFLTKBreakLoop },
        { kEventClassFLTK, kEventFLTKDataReady } };
    ret = InstallEventHandler( target, dispatchHandler,
			       GetEventTypeCount(dispatchEvents),
			       dispatchEvents, 0, 0L );
    static EventTypeSpec appEvents[] = {
        { kEventClassCommand, kEventCommandProcess } };
    ret = InstallApplicationEventHandler( dispatchHandler,
					  GetEventTypeCount(appEvents),
					  appEvents, 0, 0L );
    ret = InstallEventLoopTimer( GetMainEventLoop(), 0, 0,
				 NewEventLoopTimerUPP( timerProcCB ), 0,
				 &timer );
  }

  got_events = 0;
  // START A THREAD TO WATCH FOR DATA READY
  static pthread_t dataready_tid = 0;
  if ( nfds ) {
    void *userdata = (void*)GetCurrentEventQueue();

    // PREPARE INTER-THREAD DATA
    pthread_mutex_init(&select_mutex, NULL);

    if ( G_pipe[0] ) { close(G_pipe[0]); G_pipe[0] = 0; }
    if ( G_pipe[1] ) { close(G_pipe[1]); G_pipe[1] = 0; }
    pipe(G_pipe);

    DEBUGMSG("*** START THREAD\n");
    pthread_create(&dataready_tid, NULL, dataready_thread, userdata);
  }

  fl_unlock_function();

  if ( time > 0.0 ) {
    SetEventLoopTimerNextFireTime( timer, time );
    RunApplicationEventLoop(); // will return after the previously set time
    if ( dataready_tid != 0 ) {
      DEBUGMSG("*** CANCEL THREAD: ");
      pthread_cancel(dataready_tid);		// cancel first
      write(G_pipe[1], "x", 1);		// then wakeup thread from select
      pthread_join(dataready_tid, NULL);	// wait for thread to finish
      if ( G_pipe[0] ) { close(G_pipe[0]); G_pipe[0] = 0; }
      if ( G_pipe[1] ) { close(G_pipe[1]); G_pipe[1] = 0; }
      dataready_tid = 0;
      DEBUGMSG("OK\n");
    }
  } else {
    breakMacEventLoop();
    RunApplicationEventLoop();
    if ( dataready_tid != 0 ) {
      DEBUGMSG("*** CANCEL THREAD: ");
      pthread_cancel(dataready_tid);		// cancel first
      write(G_pipe[1], "x", 1);		// then wakeup thread from select
      pthread_join(dataready_tid, NULL);	// wait for thread to finish
      if ( G_pipe[0] ) { close(G_pipe[0]); G_pipe[0] = 0; }
      if ( G_pipe[1] ) { close(G_pipe[1]); G_pipe[1] = 0; }
      dataready_tid = 0;
      DEBUGMSG("OK\n");
    }
  }

  fl_lock_function();
  // we send FL_LEAVE only if the mouse did not enter some other window:
  // I'm not sure if this is needed or if it works...
  //if (!xmousewin) Fl::handle(FL_LEAVE, 0);
  return got_events;
}

/**
 * fl_ready() is just like fl_wait(0.0) except no callbacks are done.
 * \todo nyi, check if there is actually a message pending!
 */
static inline int fl_ready() {
  return 1;
}

////////////////////////////////////////////////////////////////

/**
 * event handler for Apple-Q key combination
 * This attempts to close all the windows. If that succeeds hopefully
 * Fl::run() will return and then the application will exit.
 */
static OSErr QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon )
{
  fl_lock_function();
  while (!Fl::modal() && Fl_X::first ) {
    Fl_X *x = Fl_X::first;
    if (!x->window->parent()) x->window->do_callback();
    // quit if callback did not close the window:
    if ( Fl_X::first == x ) break;
  }
  fl_unlock_function();
  return noErr;
}

// forward definition of functions in this file
static void handleUpdateEvent( WindowPtr xid );

/**
 * Carbon Window handler
 * This needs to be linked into all new window event handlers
 */
static pascal OSStatus carbonWindowHandler( EventHandlerCallRef nextHandler, EventRef event, void *userData )
{
  UInt32 kind = GetEventKind( event );
  OSStatus ret = eventNotHandledErr;
  Fl_Window *window = (Fl_Window*)userData;

  Rect currentBounds, originalBounds;
  WindowClass winClass;
  static Fl_Window *activeWindow = 0;
  
  fl_lock_function();
  
  switch ( kind )
  {
  case kEventWindowDrawContent:
    handleUpdateEvent( fl_xid( window ) );
    ret = noErr;
    break;
  case kEventWindowBoundsChanged: {
    GetEventParameter( event, kEventParamCurrentBounds, typeQDRectangle, NULL, sizeof(Rect), NULL, &currentBounds );
    GetEventParameter( event, kEventParamOriginalBounds, typeQDRectangle, NULL, sizeof(Rect), NULL, &originalBounds );
    int X = currentBounds.left, W = currentBounds.right-X;
    int Y = currentBounds.top, H = currentBounds.bottom-Y;
    resize_from_system = window;
    window->resize( X, Y, W, H );
    if ( ( originalBounds.right - originalBounds.left != W ) 
      || ( originalBounds.bottom - originalBounds.top != H ) )
    {
      if ( window->shown() ) 
        handleUpdateEvent( fl_xid( window ) );
    } 
    break; }
  case kEventWindowShown:
    if ( !window->parent() )
    {
      GetWindowClass( fl_xid( window ), &winClass );
      if ( winClass != kHelpWindowClass ) {	// help windows can't get the focus!
        Fl::handle( FL_FOCUS, window);
        activeWindow = window;
      }
      Fl::handle( FL_SHOW, window);
    }
    break;
  case kEventWindowHidden:
    if ( !window->parent() ) Fl::handle( FL_HIDE, window);
    break;
  case kEventWindowActivated:
    if ( window!=activeWindow ) 
    {
      GetWindowClass( fl_xid( window ), &winClass );
      if ( winClass != kHelpWindowClass ) {	// help windows can't get the focus!
        Fl::handle( FL_FOCUS, window);
        activeWindow = window;
      }
    }
    break;
  case kEventWindowDeactivated:
    if ( window==activeWindow ) 
    {
      Fl::handle( FL_UNFOCUS, window);
      activeWindow = 0;
    }
    break;
  case kEventWindowClose:
    window->do_callback(); // this might or might not close the window
    ret = noErr; // returning noErr tells Carbon to stop following up on this event
    break;
  }

  fl_unlock_function();

  return ret;
}


/**
 * Carbon Mousewheel handler
 * This needs to be linked into all new window event handlers
 */
static pascal OSStatus carbonMousewheelHandler( EventHandlerCallRef nextHandler, EventRef event, void *userData )
{
  Fl_Window *window = (Fl_Window*)userData;
  EventMouseWheelAxis axis;

  fl_lock_function();
  
  GetEventParameter( event, kEventParamMouseWheelAxis, typeMouseWheelAxis, NULL, sizeof(EventMouseWheelAxis), NULL, &axis );
  long delta;
  GetEventParameter( event, kEventParamMouseWheelDelta, typeLongInteger, NULL, sizeof(long), NULL, &delta );
  if ( axis == kEventMouseWheelAxisX )
  {
    Fl::e_dx = delta;
    if ( Fl::e_dx) Fl::handle( FL_MOUSEWHEEL, window );
  }
  else if ( axis == kEventMouseWheelAxisY )
  {
    Fl::e_dy = -delta;
    if ( Fl::e_dy) Fl::handle( FL_MOUSEWHEEL, window );
  }
  else {
    fl_unlock_function();

    return eventNotHandledErr;
  }

  fl_unlock_function();
  
  return noErr;
}


/**
 * convert the current mouse chord into the FLTK modifier state
 */
static void chord_to_e_state( UInt32 chord )
{
  static ulong state[] = 
  { 
    0, FL_BUTTON1, FL_BUTTON3, FL_BUTTON1|FL_BUTTON3, FL_BUTTON2,
    FL_BUTTON2|FL_BUTTON1, FL_BUTTON2|FL_BUTTON3, FL_BUTTON2|FL_BUTTON1|FL_BUTTON3
  };
  Fl::e_state = ( Fl::e_state & 0xff0000 ) | state[ chord & 0x07 ];
}

EventRef fl_os_event;		// last (mouse) event

/**
 * Carbon Mouse Button Handler
 */
static pascal OSStatus carbonMouseHandler( EventHandlerCallRef nextHandler, EventRef event, void *userData )
{
  static int keysym[] = { 0, FL_Button(1), FL_Button(3), FL_Button(2) };
  static int px, py;

  fl_lock_function();
  
  fl_os_event = event;
  Fl_Window *window = (Fl_Window*)userData;
  Point pos;
  GetEventParameter( event, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(Point), NULL, &pos );
  EventMouseButton btn;
  GetEventParameter( event, kEventParamMouseButton, typeMouseButton, NULL, sizeof(EventMouseButton), NULL, &btn );
  UInt32 clickCount;
  GetEventParameter( event, kEventParamClickCount, typeUInt32, NULL, sizeof(UInt32), NULL, &clickCount );
  UInt32 chord;
  GetEventParameter( event, kEventParamMouseChord, typeUInt32, NULL, sizeof(UInt32), NULL, &chord );
  WindowRef xid = fl_xid(window), tempXid;
  int sendEvent = 0, part;
  switch ( GetEventKind( event ) )
  {
  case kEventMouseDown:
    part = FindWindow( pos, &tempXid );
    if ( part != inContent ) {
      fl_unlock_function();
      return CallNextEventHandler( nextHandler, event ); // let the OS handle this for us
    }
    if ( !IsWindowActive( xid ) )
      CallNextEventHandler( nextHandler, event ); // let the OS handle the activation, but continue to get a click-through effect
    // normal handling of mouse-down follows
    fl_os_capture = xid;
    sendEvent = FL_PUSH;
    Fl::e_is_click = 1; px = pos.h; py = pos.v;
    Fl::e_clicks = clickCount-1;
    // fall through
  case kEventMouseUp:
    if ( !window ) break;
    if ( !sendEvent ) {
      sendEvent = FL_RELEASE; 
    }
    Fl::e_keysym = keysym[ btn ];
    // fall through
  case kEventMouseMoved:
    if ( !sendEvent ) { 
      sendEvent = FL_MOVE; chord = 0; 
    }
    // fall through
  case kEventMouseDragged:
    if ( !sendEvent ) {
      sendEvent = FL_MOVE; // Fl::handle will convert into FL_DRAG
      if (abs(pos.h-px)>5 || abs(pos.v-py)>5) 
        Fl::e_is_click = 0;
    }
    chord_to_e_state( chord );
    GrafPtr oldPort;
    GetPort( &oldPort );
    SetPort( GetWindowPort(xid) ); // \todo replace this! There must be some GlobalToLocal call that has a port as an argument
    SetOrigin(0, 0);
    Fl::e_x_root = pos.h;
    Fl::e_y_root = pos.v;
    GlobalToLocal( &pos );
    Fl::e_x = pos.h;
    Fl::e_y = pos.v;
    SetPort( oldPort );
    Fl::handle( sendEvent, window );
    break;
  }

  fl_unlock_function();
  
  return noErr;
}


/**
 * convert the current mouse chord into the FLTK modifier state
 */
static void mods_to_e_state( UInt32 mods )
{
  long state = 0;
  if ( mods & kEventKeyModifierNumLockMask ) state |= FL_NUM_LOCK;
  if ( mods & cmdKey ) state |= FL_CTRL;
  if ( mods & (optionKey|rightOptionKey) ) state |= FL_ALT;
  if ( mods & (controlKey|rightControlKey) ) state |= FL_META;
  if ( mods & (shiftKey|rightShiftKey) ) state |= FL_SHIFT;
  if ( mods & alphaLock ) state |= FL_CAPS_LOCK;
  Fl::e_state = ( Fl::e_state & 0xff000000 ) | state;
  //printf( "State 0x%08x (%04x)\n", Fl::e_state, mods );
}


/**
 * convert the current mouse chord into the FLTK keysym
 */
static void mods_to_e_keysym( UInt32 mods )
{
  if ( mods & cmdKey ) Fl::e_keysym = FL_Control_L;
  else if ( mods & kEventKeyModifierNumLockMask ) Fl::e_keysym = FL_Num_Lock;
  else if ( mods & optionKey ) Fl::e_keysym = FL_Alt_L;
  else if ( mods & rightOptionKey ) Fl::e_keysym = FL_Alt_R;
  else if ( mods & controlKey ) Fl::e_keysym = FL_Meta_L;
  else if ( mods & rightControlKey ) Fl::e_keysym = FL_Meta_R;
  else if ( mods & shiftKey ) Fl::e_keysym = FL_Shift_L;
  else if ( mods & rightShiftKey ) Fl::e_keysym = FL_Shift_R;
  else if ( mods & alphaLock ) Fl::e_keysym = FL_Caps_Lock;
  else Fl::e_keysym = 0;
}


static unsigned short macKeyLookUp[128] =
{
    'a', 's', 'd', 'f', 'h', 'g', 'z', 'x',
    'c', 'v', 0/*ISO extra ('#' on German keyboard)*/, 'b', 'q', 'w', 'e', 'r',

    'y', 't', '1', '2', '3', '4', '6', '5',
    '=', '9', '7', '-', '8', '0', ']', 'o',

    'u', '[', 'i', 'p', FL_Enter, 'l', 'j', '\'',
    'k', ';', '\\', ',', '/', 'n', 'm', '.',

    FL_Tab, ' ', '`', FL_BackSpace, 0, FL_Escape, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,

    0, FL_KP('.'), FL_Right, FL_KP('*'), 0, FL_KP('+'), FL_Left, FL_Num_Lock,
    FL_Down, 0, 0, FL_KP('/'), FL_KP_Enter, FL_Up, FL_KP('-'), 0,

    0, FL_KP('='), FL_KP('0'), FL_KP('1'), FL_KP('2'), FL_KP('3'), FL_KP('4'), FL_KP('5'),
    FL_KP('6'), FL_KP('7'), 0, FL_KP('8'), FL_KP('9'), 0, 0, 0,

    FL_F(5), FL_F(6), FL_F(7), FL_F(3), FL_F(8), FL_F(9), 0, FL_F(11),
    0, 0, FL_Print, FL_Scroll_Lock, 0, FL_F(10), 0, FL_F(12),

    0, FL_Pause, FL_Help, FL_Home, FL_Page_Up, FL_Delete, FL_F(4), FL_End,
    FL_F(2), FL_Page_Down, FL_F(1), FL_Left, FL_Right, FL_Down, FL_Up, 0,
};

/**
 * handle carbon keyboard events
 */
pascal OSStatus carbonKeyboardHandler( EventHandlerCallRef nextHandler, EventRef event, void *userData )
{
  static char buffer[5];
  int sendEvent = 0;
  Fl_Window *window = (Fl_Window*)userData;
  UInt32 mods;
  static UInt32 prevMods = 0xffffffff;

  fl_lock_function();
  
  GetEventParameter( event, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &mods );
  if ( prevMods == 0xffffffff ) prevMods = mods;
  UInt32 keyCode;
  GetEventParameter( event, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &keyCode );
  unsigned char key;
  GetEventParameter( event, kEventParamKeyMacCharCodes, typeChar, NULL, sizeof(char), NULL, &key );
  unsigned short sym;

  switch ( GetEventKind( event ) )
  {
  case kEventRawKeyDown:
  case kEventRawKeyRepeat:
    sendEvent = FL_KEYBOARD;
    // fall through
  case kEventRawKeyUp:
    if ( !sendEvent ) sendEvent = FL_KEYUP;
    sym = macKeyLookUp[ keyCode & 0x7f ];
    Fl::e_keysym = sym;
    if ( keyCode==0x4c ) key=0x0d;
    if ( ( (sym>=FL_KP(0)) && (sym<=FL_KP_Last) ) || ((sym&0xff00)==0) || (sym==FL_Tab) ) {
      buffer[0] = key;
      Fl::e_length = 1;
    } else {
      buffer[0] = 0;
      Fl::e_length = 0;
    }
    Fl::e_text = buffer;
    // insert UnicodeHandling here!
    break;
  case kEventRawKeyModifiersChanged: {
    UInt32 tMods = prevMods ^ mods;
    if ( tMods )
    {
      mods_to_e_keysym( tMods );
      if ( Fl::e_keysym ) 
        sendEvent = ( prevMods<mods ) ? FL_KEYBOARD : FL_KEYUP;
      Fl::e_length = 0;
      buffer[0] = 0;
      prevMods = mods;
    }
    mods_to_e_state( mods );
    break; }
  }
  while (window->parent()) window = window->window();
  if (sendEvent && Fl::handle(sendEvent,window)) {
    fl_unlock_function();
  
    return noErr; // return noErr if FLTK handled the event
  } else {
    fl_unlock_function();
  
    return CallNextEventHandler( nextHandler, event );
  }
}


/**
 * initialize the Mac toolboxes and set the default menubar
 */
void fl_open_display() {
  static char beenHereDoneThat = 0;
  if ( !beenHereDoneThat )  {
    beenHereDoneThat = 1;
    
    FlushEvents(everyEvent,0);

    MoreMasters(); // \todo Carbon suggests MoreMasterPointers()
    AEInstallEventHandler( kCoreEventClass, kAEQuitApplication,
	NewAEEventHandlerUPP((AEEventHandlerProcPtr)QuitAppleEventHandler),
			   0, false );

    // create the Mac Handle for the default cursor (a pointer to a pointer)
    GetQDGlobalsArrow(&default_cursor);
    default_cursor_ptr = &default_cursor;
    fl_default_cursor  = &default_cursor_ptr;
    
    ClearMenuBar();
    AppendResMenu( GetMenuHandle( 1 ), 'DRVR' );
    DrawMenuBar();

    // Probably should do the initialization that is in fl_wait() here...
  }
}

static bool reload_info = true;

const Fl_Screen_Info& Fl::info() {
  static Fl_Screen_Info info;
  if (reload_info) {
    reload_info = false;
    BitMap r;
    GetQDGlobalsScreenBits(&r);
    info.x = r.bounds.left;
    info.y = r.bounds.top + 20; // \todo 20 pixel menu bar?
    info.width = r.bounds.right;
    info.w = info.width-info.x;
    info.height = r.bounds.bottom;
    info.h = info.height-info.y;
    // I don't know if this scale info is available...
    info.width_mm = int(info.width*25.4/75);
    info.height_mm = int(info.height*25.4/75);
  }
  return info;
}

/**
 * get the current mouse pointer world coordinates
 */
void Fl::get_mouse(int &x, int &y) 
{
  fl_open_display();
  Point loc; 
  GetMouse( &loc );
  LocalToGlobal( &loc );
  x = loc.h;
  y = loc.v;
}

// Damage all the child windows as well as this one...
static void recursive_expose(Fl_X* i) {
  i->wait_for_expose = false;
  i->expose(0, 0, i->window->w(), i->window->h());
  for (Fl_X* c = i->children; c; c = c->brother) recursive_expose(c);
}

/**
 * Handle a request from the system to update the window.
 * Like the windows version this can't be done in any easy way because
 * it interferes with the damage bits so that every widget would need
 * two draw() functions, one for expose events, and one for other
 * drawing. So like Windows I try to fake X behavior by retrieving
 * the damage region (assummed to be the whole window now) and store
 * that region as though an expose() event came in, then get Fl::wait()
 * to return so Fl::flush() will get called.
 */
void handleUpdateEvent( WindowPtr xid ) 
{
  Fl_Window *window = fl_find( xid );
  Fl_X *i = Fl_X::i( window );
  recursive_expose(i);
  breakMacEventLoop();
}

////////////////////////////////////////////////////////////////

/**
 * convert a Mac FSSpec structure into a Unix filename 
 */
static int FSSpec2UnixPath( FSSpec *fs, char *dst )
{
  FSRef fsRef;
  FSpMakeFSRef( fs, &fsRef );
  FSRefMakePath( &fsRef, (UInt8*)dst, 1024 );
  return strlen(dst);
/* keep the code below. The above function is only implemented in OS X, so we might need the other code for OS 9 and friends
  short offset = 0;
  if ( fs->parID != fsRtParID )
  {
    FSSpec parent;
    OSErr ret = FSMakeFSSpec( fs->vRefNum, fs->parID, 0, &parent );
    if ( ret != noErr ) return 0;
    offset = FSSpec2UnixPath( &parent, dst );
  }

  if ( fs->parID == fsRtParID && fs->vRefNum == -100 ) //+ bad hack: we assume that volume -100 is mounted as root
  {
    memcpy( dst, "/", 2 );
    return 1; // don't add anything to the filename - we are fine already
  }

  short len = fs->name[0];
  if ( fs->parID == fsRtParID ) { // assume tat all other volumes are in this directory (international name WILL vary!)
    memcpy( dst, "/Volumes", 8 );
    offset = 8;
  }
  
  if ( offset!=1 ) dst[ offset++ ] = '/'; // avoid double '/'
  memcpy( dst+offset, fs->name+1, len );
  dst[ len+offset ] = 0;
  return len+offset;
*/
}
 
Fl_Window *fl_dnd_target_window = 0;
#include <fltk/fl_draw.h>
/**
 * Drag'n'drop tracking handler
 */
static pascal OSErr dndTrackingHandler( DragTrackingMessage msg, WindowPtr w, void *userData, DragReference dragRef )
{
  Fl_Window *target = (Fl_Window*)userData;
  Point mp;
  static int px, py;
  
  switch ( msg )
  {
  case kDragTrackingEnterWindow:
    // check if 'TEXT' is available
    GetDragMouse( dragRef, &mp, 0 );
    Fl::e_x_root = px = mp.h;
    Fl::e_y_root = py = mp.v;
    Fl::e_x = px - target->x();
    Fl::e_y = py - target->y();
    fl_dnd_target_window = target;
    if ( Fl::handle( FL_DND_ENTER, target ) )
      fl_cursor( FL_CURSOR_HAND ); //ShowDragHilite( ); // modify the mouse cursor?!
    else
      fl_cursor( FL_CURSOR_DEFAULT ); //HideDragHilite( dragRef );
    breakMacEventLoop();
    return noErr;
  case kDragTrackingInWindow:
    GetDragMouse( dragRef, &mp, 0 );
    if ( mp.h==px && mp.v==py )
      break;	//+ return previous condition for dnd hiliting
    Fl::e_x_root = px = mp.h;
    Fl::e_y_root = py = mp.v;
    Fl::e_x = px - target->x();
    Fl::e_y = py - target->y();
    fl_dnd_target_window = target;
    if ( Fl::handle( FL_DND_DRAG, target ) )
      fl_cursor( FL_CURSOR_HAND ); //ShowDragHilite( ); // modify the mouse cursor?!
    else
      fl_cursor( FL_CURSOR_DEFAULT ); //HideDragHilite( dragRef );
    breakMacEventLoop();
    return noErr;
    break;
  case kDragTrackingLeaveWindow:
    // HideDragHilite()
    fl_cursor( FL_CURSOR_DEFAULT ); //HideDragHilite( dragRef );
    if ( fl_dnd_target_window )
    {
      Fl::handle( FL_DND_LEAVE, fl_dnd_target_window );
      fl_dnd_target_window = 0;
    }
    breakMacEventLoop();
    return noErr;
  }
  return noErr;
}


/**
 * Drag'n'drop receive handler
 */
static pascal OSErr dndReceiveHandler( WindowPtr w, void *userData, DragReference dragRef )
{
  Point mp;
  OSErr ret;
  
  Fl_Window *target = fl_dnd_target_window = (Fl_Window*)userData;
  GetDragMouse( dragRef, &mp, 0 );
  Fl::e_x_root = mp.h;
  Fl::e_y_root = mp.v;
  Fl::e_x = Fl::e_x_root - target->x();
  Fl::e_y = Fl::e_y_root - target->y();
  if ( !Fl::handle( FL_DND_RELEASE, target ) )
    return userCanceledErr;
    
  // get the ASCII text
  UInt16 i, nItem;
  ItemReference itemRef;
  FlavorFlags flags;
  Size itemSize, size = 0;
  CountDragItems( dragRef, &nItem );
  for ( i = 1; i <= nItem; i++ )
  {
    GetDragItemReferenceNumber( dragRef, i, &itemRef );
    ret = GetFlavorFlags( dragRef, itemRef, 'TEXT', &flags );
    if ( ret == noErr )
    {
      GetFlavorDataSize( dragRef, itemRef, 'TEXT', &itemSize );
      size += itemSize;
    }
    ret = GetFlavorFlags( dragRef, itemRef, 'hfs ', &flags );
    if ( ret == noErr )
    {
      size += 1024; //++ ouch! We should create the full pathname and figure out its length
    }
  }
  
  if ( !size )
    return userCanceledErr;
  
  Fl::e_length = size + nItem - 1;
  char *dst = Fl::e_text = (char*)malloc( size+nItem );;
  
  for ( i = 1; i <= nItem; i++ )
  {
    GetDragItemReferenceNumber( dragRef, i, &itemRef );
    ret = GetFlavorFlags( dragRef, itemRef, 'TEXT', &flags );
    if ( ret == noErr )
    {
      GetFlavorDataSize( dragRef, itemRef, 'TEXT', &itemSize );
      GetFlavorData( dragRef, itemRef, 'TEXT', dst, &itemSize, 0L );
      dst += itemSize;
      *dst++ = '\n'; // add our element seperator
    }
    ret = GetFlavorFlags( dragRef, itemRef, 'hfs ', &flags );
    if ( ret == noErr )
    {
      HFSFlavor hfs; itemSize = sizeof( hfs );
      GetFlavorData( dragRef, itemRef, 'hfs ', &hfs, &itemSize, 0L );
      itemSize = FSSpec2UnixPath( &hfs.fileSpec, dst );
      dst += itemSize;
      if ( itemSize>1 && ( hfs.fileType=='fold' || hfs.fileType=='disk' ) ) 
        *dst++ = '/';
      *dst++ = '\n'; // add our element seperator
    }
  }
  
  dst[-1] = 0;
//  if ( Fl::e_text[Fl::e_length-1]==0 ) Fl::e_length--; // modify, if trailing 0 is part of string
  Fl::e_length = dst - Fl::e_text - 1;
  target->handle(FL_PASTE);
  free( Fl::e_text );
  
  fl_dnd_target_window = 0L;
  breakMacEventLoop();
  return noErr;
}

////////////////////////////////////////////////////////////////

/**
 * Resizes the actual system window in response to a resize() call from
 * the program.
 */
void Fl_Window::layout() {
  if (parent()) {
    // child windows are done entirely by us
  } else if (this == resize_from_system) {
    // Ignore changes that came from the system
    resize_from_system = 0;
  } else if ((layout_damage()&FL_LAYOUT_XYWH) && i) { // only for shown windows
    // figure out where the window should be in it's parent:
    int x = this->x(); int y = this->y();
    for (Fl_Widget* p = parent(); p && !p->is_window(); p = p->parent()) {
      x += p->x(); y += p->y();
    }
    MoveWindow(i->xid, x, y, 0);
    if (layout_damage() & FL_LAYOUT_WH) {
      SizeWindow(i->xid, w()>0 ? w() : 1, h()>0 ? h() : 1, 0);
      Rect all; all.top=-32000; all.bottom=32000; all.left=-32000; all.right=32000;
      InvalWindowRect( i->xid, &all );    
    }
  }
  Fl_Group::layout();
}

////////////////////////////////////////////////////////////////

/**
 * go ahead, create that (sub)window
 * \todo we should make menu windows slightly transparent for the new Mac look
 */
void Fl_Window::create()
{
  // Create structure to hold the rectangle, initialize the parts that
  // are the same for outer and child windows:
  Fl_X* x = new Fl_X;
  x->backbuffer.xid = 0;
  x->window = this; i = x;
  x->region = 0;
  x->subRegion = 0;
  x->children = x->brother = 0;
  x->cursor = fl_default_cursor;

  if (parent()) {
    // Apparently Mac does not have child windows, this shows that there
    // really isn't any need for the system to support them as they are
    // easily emulated at the application level. We find the parent
    // window and add ourselves to a list of children. This is then used
    // to create a clipping region that is applied when drawing the
    // parent. We may want to clip brothers, too...
    Fl_Window* root;
    for (Fl_Widget *o = parent(); ; o = o->parent()) {
      if (o->is_window()) {root = (Fl_Window*)o; break;}
    }
    Fl_X *rootx = Fl_X::i(root);
    x->xid = rootx->xid; // Not sure if this is a good idea
    // we need to add it to very end, so overlapping brothers are easy
    // to find:
    Fl_X** p = &(rootx->children);
    while (*p) p = &((*p)->brother);
    *p = x;
    x->wait_for_expose = false;
    x->next = Fl_X::first;
    Fl_X::first = x;
  } else {
    // create a desktop window
    int dx, dy, dw, dh; // border thickness
    int winclass, winattr, where;
    if (!border() || override()) {
      // used by menus and tooltips
      dx = dy = dw = dh = 0;
      winclass = kHelpWindowClass;
      winattr = 0;
      where = kWindowCascadeOnParentWindowScreen;
    } else {
      // a normal window with a border
      dx = dw = 0; dy = dh = 22;
      if (contains(Fl::modal())) {
	winclass = kMovableModalWindowClass;
	winattr = kWindowStandardHandlerAttribute |
	  kWindowCloseBoxAttribute;
	where = kWindowAlertPositionOnParentWindowScreen;
#if 0
      } else if (child_of()) {
	// Major kludge: this is to have the regular look, but stay
	// above the document windows
	SetWindowClass(x->xid, kFloatingWindowClass );
	where = kWindowCenterOnParentWindowScreen;
#endif
      } else {
	winclass = kDocumentWindowClass;
	winattr = kWindowStandardHandlerAttribute |
	  kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute;
	where = kWindowCascadeOnParentWindowScreen;
      }
      if (minw < maxw || minh < maxh)
	winattr |= kWindowFullZoomAttribute |
	  kWindowResizableAttribute | kWindowLiveResizeAttribute;
    }

    bool autoplace = (this->x() == FL_USEDEFAULT || this->y()== FL_USEDEFAULT);
    Rect wRect;
    wRect.left   = this->x();
    wRect.top    = this->y(); 
    if (autoplace) {
      // this is a kludge in case system autoplace does not work
      static int xyPos = 50;
      wRect.top = wRect.left = xyPos;
      xyPos += 25;
      if (xyPos >= 200) xyPos -= 170;
    }
    wRect.right  = wRect.left+w();
    wRect.bottom = wRect.top+h();

    winattr &= GetAvailableWindowAttributes(winclass);	// make sure that the window will open
    CreateNewWindow(winclass, winattr, &wRect, &(x->xid));
    MoveWindow(x->xid, wRect.left, wRect.top, 1); // avoid Carbon Bug on old OS
    label(label(), iconlabel());

    if (autoplace) {
      WindowRef pw =
	(child_of() && child_of()->shown()) ? child_of()->i->xid : 0;
      RepositionWindow(x->xid, pw, where);
    }

    x->wait_for_expose = true;
    x->next = Fl_X::first;
    Fl_X::first = x;

    // if (resizable()) DrawGrowIcon(x->xid); this should be in flush()?

    { // Install Carbon Event handlers 
      OSStatus ret;
      EventHandlerUPP mousewheelHandler = NewEventHandlerUPP( carbonMousewheelHandler ); // will not be disposed by Carbon...
      static EventTypeSpec mousewheelEvents[] = {
        { kEventClassMouse, kEventMouseWheelMoved } };
      ret = InstallWindowEventHandler( x->xid, mousewheelHandler, 1, mousewheelEvents, this, 0L );
      EventHandlerUPP mouseHandler = NewEventHandlerUPP( carbonMouseHandler ); // will not be disposed by Carbon...
      static EventTypeSpec mouseEvents[] = {
        { kEventClassMouse, kEventMouseDown },
        { kEventClassMouse, kEventMouseUp },
        { kEventClassMouse, kEventMouseMoved },
        { kEventClassMouse, kEventMouseDragged } };
      ret = InstallWindowEventHandler( x->xid, mouseHandler, 4, mouseEvents, this, 0L );
      EventHandlerUPP keyboardHandler = NewEventHandlerUPP( carbonKeyboardHandler ); // will not be disposed by Carbon...
      static EventTypeSpec keyboardEvents[] = {
        { kEventClassKeyboard, kEventRawKeyDown },
        { kEventClassKeyboard, kEventRawKeyRepeat },
        { kEventClassKeyboard, kEventRawKeyUp },
        { kEventClassKeyboard, kEventRawKeyModifiersChanged } };
      ret = InstallWindowEventHandler( x->xid, keyboardHandler, 4, keyboardEvents, this, 0L );
      EventHandlerUPP windowHandler = NewEventHandlerUPP( carbonWindowHandler ); // will not be disposed by Carbon...
      static EventTypeSpec windowEvents[] = {
        { kEventClassWindow, kEventWindowDrawContent },
        { kEventClassWindow, kEventWindowShown },
        { kEventClassWindow, kEventWindowHidden },
        { kEventClassWindow, kEventWindowActivated },
        { kEventClassWindow, kEventWindowDeactivated },
        { kEventClassWindow, kEventWindowClose },
        { kEventClassWindow, kEventWindowBoundsChanged } };
      ret = InstallWindowEventHandler( x->xid, windowHandler, 7, windowEvents, this, 0L );
      ret = InstallTrackingHandler( dndTrackingHandler, x->xid, this);
      ret = InstallReceiveHandler( dndReceiveHandler, x->xid, this);
    }
  }
}

/**
 * Turn on flag to indicate that user set the min and max size.
 */
void Fl_Window::size_range_() {
  size_range_set = 1;
}

/**
 * Returns true if the window is shown but is iconized.
 */
bool Fl_Window::iconic() const {
  return i && IsWindowCollapsed(i->xid);
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

/**
 * set the window title bar
 * \todo make the dock icon work!
 */
void Fl_Window::label(const char *name, const char * iname) {
  Fl_Widget::label(name);
  iconlabel_ = iname;
  if (i && !parent()) {
    if (!name) name = "";
    Str255 pTitle;
    pTitle[0] = strlen(name); memcpy(pTitle+1, name, pTitle[0]);
    SetWTitle(fl_xid(this), pTitle);
    // if (!iname) iname = filename_name(name);
    // should do something with iname here, it should label the dock icon
  }
}

////////////////////////////////////////////////////////////////
// Drawing context

const Fl_Window *Fl_Window::current_;

/**
 * make all drawing go into this window (called by subclass flush() impl.)
 */
void Fl_Window::make_current() const
{
  current_ = this;
  SetPort(GetWindowPort(i->xid));
  // move origin to match where we are in the window:
  int X = 0;
  int Y = 0;
  for (const Fl_Widget* o = this; o->parent(); o = o->parent()) {
    X += o->x();
    Y += o->y();
  }
  SetOrigin(X, Y);
  // We force a clip region to handle child windows. To speed things up
  // we only recalculate the clip region when children are shown, hidden,
  // or moved or resized.
  if (!i->subRegion) {
    i->subRegion = NewRgn();
    SetRectRgn(i->subRegion, 0, 0, w(), h());
    for (Fl_X* cx = i->children; cx; cx = cx->brother) {
      Region r = NewRgn();
      Fl_Window* cw = cx->window;
      int x = cw->x();
      int y = cw->y();
      SetRectRgn(r, x, y, x+cw->w(), y+cw->h());
      DiffRgn(i->subRegion, r, i->subRegion);
      DisposeRgn(r);
    }
    for (Fl_X* cx = i->brother; cx; cx = cx->brother) {
      Region r = NewRgn();
      Fl_Window* cw = cx->window;
      int x = cw->x()-this->x();
      int y = cw->y()-this->y();
      SetRectRgn(r, x, y, x+cw->w(), y+cw->h());
      DiffRgn(i->subRegion, r, i->subRegion);
      DisposeRgn(r);
    }
  }
  SetPortClipRegion( GetWindowPort(i->xid), i->subRegion );
}

////////////////////////////////////////////////////////////////
// Cut & paste.

static char *selection_buffer[2];
static int selection_length[2];
static int selection_buffer_length[2];
static ScrapRef myScrap = 0;

/**
 * create a selection
 * owner: widget that created the selection
 * stuff: pointer to selected data
 * size of selected data
 */
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
  if (clipboard) {
    ClearCurrentScrap();
    if ( GetCurrentScrap( &myScrap ) == noErr ) {
      // Previous version changed \n to \r before sending the text, but I would
      // prefer to leave the local buffer alone, so a copied buffer may be
      // needed. Check to see if this is necessary on OS/X.
      PutScrapFlavor( myScrap, kScrapFlavorTypeText, 0,
		      len, selection_buffer[1] );
    } else {
      myScrap = 0;
    }
  }
}

// Call this when a "paste" operation happens:
void Fl::paste(Fl_Widget &receiver, bool clipboard) {
  if (clipboard) {
    // see if we own the selection, if not go get it:
    ScrapRef scrap = 0;
    Size len = 0;
    if (GetCurrentScrap(&scrap) == noErr && scrap != myScrap &&
	GetScrapFlavorSize(scrap, kScrapFlavorTypeText, &len) == noErr) {
      if ( len > selection_buffer_length[1] ) {
	selection_buffer_length[1] = len + 32;
	delete[] selection_buffer[1];
	selection_buffer[1] = new char[len];
      }
      GetScrapFlavorData( scrap, kScrapFlavorTypeText, &len,
			  selection_buffer[1] );
      selection_length[1] = len;
      // turn all \r characters into \n:
      for (int x = 0; x < len; x++) {
	if (selection_buffer[1][x] == '\r')
	  selection_buffer[1][x] = '\n';
      }
    }
  }
  Fl::e_text = selection_buffer[clipboard];
  Fl::e_length = selection_length[clipboard];
  receiver.handle(FL_PASTE);
  return;
}

//
// End of "$Id: Fl_mac.cxx,v 1.2 2002/09/09 01:39:58 spitzak Exp $".
//

