//
// "$Id: Fl.h,v 1.13 2002/10/29 00:37:22 easysw Exp $"
//
// Main header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_H
#define Fl_H

#include "Enumerations.h"

#ifndef FLTK_2
// These header files are only needed for the obsolete functions:
#include "Fl_Color.h"
#include "Fl_Boxtype.h"
#endif

class FL_API Fl_Widget;
class FL_API Fl_Window;
struct Fl_Screen_Info {
  int x, y, w, h;
  int width;
  int height;
  int depth;
  int width_mm;
  int height_mm;
};

typedef void (*Fl_Timeout_Handler)(void*);

/**

These functions mostly are concerned with event handling and waiting for 
events. Fl:: should be considered a C++ "namespace". However it is 
implemented as a class with no instance variables and only static methods, 
in order to work with older C++ compilers that do not handle namespaces. 
 

FLTK also provides a number of global functions with fl_xyz names. The majority of 
these are concerned with drawing or graphics state. 
The others are a set of modal popup utility functions for asking 
questions. 

*/
class FL_API Fl {
  Fl() {}; // no constructor!

public: // should be private!

  static int damage_;
  static int e_type;
  static int e_x;
  static int e_y;
  static int e_dx;
  static int e_dy;
  static int e_x_root;
  static int e_y_root;
  static int e_state;
  static int e_clicks;
  static int e_is_click;
  static int e_keysym;
  static int e_length;
  static char* e_text;
  static int compose_state;
  static Fl_Widget* belowmouse_;
  static Fl_Widget* pushed_;
  static Fl_Widget* focus_;
  static Fl_Widget* modal_;
  static bool grab_;
  static bool exit_modal_;
  static void damage(int x) {damage_ = x;}
  static void (*idle)();

public:
  /**
   Returns the version number of fltk. This can be compared to the value of 
   the FL_VERSION macro to see if the shared library of fltk your program 
   linked with is up to date. 
   The FLTK version number is stored in a number of compile-time constants: 

   - FL_MAJOR_VERSION - The major release number, currently 2. 
   - FL_MINOR_VERSION - The minor release number, currently 0. 
   - FL_PATCH_VERSION - The patch release number, currently 1. 
   - FL_VERSION       - A combined floating-point version number of the 
                        form M.mmpp where M is the major number, mm is the 
                        minor number, and pp is the patch number, 
                        currently 2.0001. 
 
  */
  static float version();

  // argument parsers:

  /**
   FLTK provides an entirely optional command-line switch parser. You don't 
   have to call it if you don't like them! Everything it can do can be done 
   with other calls to FLTK. 
   To use the switch parser, call Fl::args(...) near the start of your 
   program. This does not open the display, instead switches that need the 
   display open are stashed into static variables. Then you must display your
   first window by calling window->show(argc,argv), which will do anything 
   stored in the static variables.

   callback lets you define your own switches. It is called with the same 
   argc and argv, and with i the index of each word. The callback should 
   return zero if the switch is unrecognized, and not change i. It should 
   return non-zero if the switch is recognized, and add at least 1 to i (it 
   can add more to consume words after the switch). This function is called 
   before any other tests, so you can override any FLTK switch (this is why 
   fltk can use very short switches instead of the long ones all other toolkits 
   force you to use).

   On return i is set to the index of the first non-switch. This is either:

   The first word that does not start with '-'. 
   The word '-' (used by many programs to name stdin as a file) 
   The first unrecognized switch (return value is 0). 
   argc 
   The return value is i unless an unrecognized switch is found, in which 
   case it is zero. If your program takes no arguments other than switches 
   you should produce an error if the return value is less than argc. 
   All switches may be abbreviated one letter and case is ignored: 

   - -display host:n.n  The X display to use (ignored under WIN32). 
   - -geometry WxH      Window is resized to this width & height 
   - -geometry +X+Y     Initial window position 
   - -geometry WxH+X+Y  Window is resized and positioned. 
   - -name string       Fl_Window::xclass(string) will be done to the window, 
                        possibly changing its icon. 
   - -title string      Fl_Window::label(string) will be done to the window, 
                        changing both its title and the icontitle. 
   - -iconic            Fl_Window::iconize() will be done to the window. 
   - -bg color          XParseColor is used to lookup the passed color and 
                        then fl_background() is done. Under WIN32 only color 
                        names of the form "#xxxxxx" are understood. 
   - -theme name        Calls Fl_Style::load_theme(name) and if found it calls 
                        Fl_Style::theme(x) with it. If the theme plugin is not 
                        found it exits with an error. The plugin is looked for 
                        in ~/.fltk/name.theme and /usr/local/lib/name.theme. 
   - -scheme name       Calls Fl_Style::scheme(name). Some theme plugins may 
                        do something with this argument, typically it is used 
                        to name a file the theme reads. 

  */
  static int args(int argc, char** argv, int& i, int (*callback)(int,char**,int&) = 0);
  /**
   The second form of Fl::args() is useful if your program does not 
   have command line switches of its own. It parses all the switches, 
   and if any are not recognized it calls Fl::fatal(Fl::help). 
  */
  static void args(int argc, char** argv);
  /**
   Consume a single switch from argv, starting at word i. Returns the number 
   of words eaten (1 or 2, or 0 if it is not recognized) and adds the same 
   value to i. You can use this function if you prefer to control the 
   incrementing through the arguments yourself. 
  */
  static int arg(int argc, char** argv, int& i);
  /**
   This is the usage string that is displayed if Fl::args() detects an 
   invalid argument on the command-line. 
   It's value is: 
   \verbatim
   " -d[isplay] host:n.n\n"
   " -g[eometry] WxH+X+Y\n"
   " -t[heme] theme\n"
   " -n[ame] windowname\n"
   " -i[conic]\n"
   " -bg color";
   \endverbatim
  */
  static const char* const help;

  // things called by initialization:
  /**
   Sets the X display to use for all windows. Actually this just sets 
   the environment variable $DISPLAY to the passed string, so this only
   works before you show() the first window or otherwise open the display,
   and does nothing useful under WIN32. 
  */
  static void display(const char*);
  /**
   This call may be useful on multi-visual X servers to change from the 
   default to a more useful color mode. You must call this before you show()
   any windows. The integer argument is an 'or' of the following: 
   - FL_INDEX  indicates that a colormapped visual is ok. This call will 
               normally fail if a TrueColor visual cannot be found. 
   - FL_RGB    this value is zero and may be passed to indicate 
               that FL_INDEX is not wanted. 
   - FL_RGB8   indicates that the TrueColor visual must have at least 
               8 bits of red, green, and blue (Windows calls this 
               "millions of colors"). 
   - FL_DOUBLE indicates that hardware accelerated double buffering is 
               wanted. This will make Fl_Double_Window work better. 

   This returns true if the system has the capabilities by default or FLTK 
   suceeded in turing them on. Your program will still work even if this 
   returns false (it just won't look as good). On non-X systems this just 
   returns true or false indicating if the system supports the passed values. 
  */
  static int visual(int);
  /**
   Makes FLTK use its own colormap. This may make FLTK display better and 
   will reduce conflicts with other programs that want lots of colors. 
   However the colors may flash as you move the cursor between windows. 
   This does nothing if the current visual is not colormapped.
  */
  static void own_colormap();

  // execution:
  /**
   Same as Fl::wait(infinity). Call this repeatedly to "run" your program.
   You can also check what happened each time after this returns, which is
   quite useful for managing program state. 
  */
  static int wait();
  /**
   Waits until "something happens", or the given time interval passes. 
   It can return much sooner than the time if something happens. 
   What this really does is call all idle callbacks, all elapsed 
   timeouts, call Fl::flush() to get the screen to update, and then 
   wait some time (zero if there are idle callbacks, the shortest of 
   all pending timeouts, or the given time), for any events from the 
   user or any Fl::add_fd() callbacks. It then handles the events and 
   calls the callbacks and then returns. 

   The return value is non-zero if there are any visible windows (this 
   may change in future versions of fltk). 

   The return value is whatever the select() system call returned. 
   This will be negative if there was an error (this will happen on
   Unix if a signal happens), zero if the timeout occurred, and positive 
   if any events or fd's came in. 

   On Win32 the return value is zero if nothing happened and time is 0.0.
   Otherwise 1 is returned. 
  */
  static int wait(float time);
  /**
   Same as Fl::wait(0). Calling this during a big calculation will keep 
   the screen up to date and the interface responsive: 

   \code

   while (!calculation_done()) {
     calculate();
     Fl::check();
     if (user_hit_abort_button()) break;
   }

   \endcode
  */
  static int check();
  /**
   This is similar to Fl::check() except this does not call Fl::flush() and 
   thus does not draw anything, and does not read any events or call any 
   callbacks. This returns true if Fl::check() would do anything (it will 
   continue to return true until you call Fl::check() or Fl::wait()). 

   This is useful if your program is in a state where such callbacks are 
   illegal, or because the expense of redrawing the screen is much greater 
   than the expense of your calculation. 
   
   \code
   while (!calculation_done()) {
     calculate();
     if (Fl::ready()) {
       do_expensive_cleanup();
       Fl::check();
       if (user_hit_abort_button()) break;
     }
   }
   \endcode
  */
  static int ready();
  /**
   Calls Fl::wait() as long as any windows are not closed. When all 
   the windows are hidden by Fl_Window::hide() (which is normally 
   called by the user closing them with the close box) this will 
   return with zero. A program can also exit by having a callback 
   call exit() or abort(). 
   
   Most fltk programs will end main() with return Fl::run();. 
  */
  static int run();
  /*@{*/
  /**
   First thing: much of the time Fl_Window::exec() will do what you want, 
   so try using that. 

   This function sets the passed widget as the "modal widget". All user 
   events are directed to it or a child of it, preventing the user from 
   messing with other widgets. The modal widget does not have to be visible
   or even a child of an Fl_Window for this to work (but if it not visible, 
   Fl::event_x() and Fl::event_y() are meaningless, use Fl::event_x_root() 
   and Fl::event_y_root()). 

   Fl::exit_modal() sets the Fl::exit_modal_flag(). This may be used by 
   user callbacks to cancel modal state. The flag is also set by the 
   destruction or hiding of the modal widget, and on Windows by other 
   applications taking the focus when grab is on. 

   The calling code then loops calling Fl::wait() until Fl::exit_modal_flag() 
   is set or you otherwise decide to get out of the modal state. It is the 
   calling code's responsibility to monitor this flag and restore the modal 
   widget to it's previous value when it turns on. 

   Fl::modal() returns the current modal widget, or null if there isn't one, 
   and Fl::grab() returns the current value of grab (this is always false if 
   the modal widget is null). It is useful to test these in timeouts and file 
   descriptor callbacks in order to block actions that should not happen while 
   the modal window is up. You also need these in order to save and restore 
   the modal state. 

   grab indicates that the modal widget should get events from anywhere on 
   the screen. This is done by messing with the window system. If Fl::exit_modal() 
   is called in response to an FL_PUSH event (rather than waiting for the drag 
   or release event) fltk will "repost" the event so that it is handled after 
   modal state is exited. This may also be done for keystrokes in the future.
   On both X and WIN32 grab will not work unless you have some visible window 
   because the system interface needs a visible window id. On X be careful that 
   your program does not enter an infinite loop while grab() is on, it will 
   lock up your screen! 
  */
  static void modal(Fl_Widget*, bool grab = false);
  static Fl_Widget* modal() {return modal_;}
  static bool grab() {return grab_;}
  static void exit_modal() {exit_modal_ = true;}
  static bool exit_modal_flag() {return exit_modal_;}
  /*@}*/

  /**
   Add a one-shot timeout callback. The function will be called by 
   Fl::wait() at t seconds after this function is called. The 
   optional void* argument is passed to the callback. 
  */
  static void add_timeout(float t,Fl_Timeout_Handler,void* v = 0);
  /**
   Inside a timeout callback you can call this to add another timeout. 
   Rather than the time being measured from "now", it is measured from 
   when the system call elapsed that caused this timeout to be called. 
   This will result in far more accurate spacing of the timeout callbacks,
   it also has slightly less system call overhead. (It will also use all 
   your machine time if your timeout code and fltk's overhead take more
   than t seconds, as the real timeout will be reduced to zero). 

   It is undefined what this does if called from outside a timeout callback. 

   This code will print "TICK" each second on stdout, with a
   fair degree of accuracy: 
   
   \code
   void callback(void*) {
     printf("TICK\n");
     Fl::repeat_timeout(1.0,callback);
   }

   main() {
     Fl::add_timeout(1.0,callback);
     for (;;) Fl::wait();
   }
   \endcode
  */
  static void repeat_timeout(float t, Fl_Timeout_Handler,void* = 0);
  /**
   Returns true if the timeout exists and has not been called yet. 
  */
  static bool has_timeout(Fl_Timeout_Handler, void* = 0);
  /**
   Removes a timeout callback. It is harmless to remove a timeout 
   callback that no longer exists. 
  */
  static void remove_timeout(Fl_Timeout_Handler, void* = 0);
  /**
   Fltk will call this callback just before it flushes the display and 
   waits for events. This is different than an idle callback because it 
   is only called once, then fltk calls the system and tells it not to 
   return until an event happens. 

   This can be used by code that wants to monitor the application's state,
   such as to keep a display up to date. The advantage of using a check 
   callback is that it is called only when no events are pending. If events
   are coming in quickly, whole blocks of them will be processed before 
   this is called once. This can save significant time and avoid the 
   application falling behind the events. 

   Sample code: 
   \code
   bool state_changed; // anything that changes the display turns this on

   void callback(void*) {
     if (!state_changed) return;
     state_changed = false;
     do_expensive_calculation();
     widget->redraw();
   }

   main() {
     Fl::add_check(1.0,callback);
     return Fl::run();
   }
   \endcode
  */
  static void add_check(Fl_Timeout_Handler cb, void* v= 0);
  /**
   Returns true if the check exists and has not been called yet.
  */
  static bool has_check(Fl_Timeout_Handler, void* = 0);
  /**
   Removes a check callback. It is harmless to remove a check 
   callback that does not exist. 
  */
  static void remove_check(Fl_Timeout_Handler, void* = 0);
  /*@{*/
  /**
   Add file descriptor fd to listen to. When the fd becomes ready for reading 
   Fl::wait() will call the callback and then return. The callback is passed 
   the fd and the arbitrary void* argument. 

   The second version takes a when bitfield to indicate when the callback 
   should be done. You can or these together to make the callback be called 
   for multiple conditions: 

   - FL_READ   - Call the callback when there is data to be read. 
   - FL_WRITE  - Call the callback when data can be written without blocking. 
   - FL_EXCEPT - Call the callback if an exception occurs on the file. 

   Under UNIX any file descriptor can be monitored (files, devices, pipes, 
   sockets, etc.) Due to limitations in Microsoft Windows, WIN32 applications
   can only monitor sockets (? and is the when value ignored?) 
  */
  static void add_fd(int fd, int when, void (*cb)(int,void*),void* =0);
  static void add_fd(int fd, void (*cb)(int, void*), void* = 0);
  /*@}*/
  /**
   Remove all the callbacks (ie for all different when values) for the given 
   file descriptor. It is harmless to call this if there are no callbacks for
   the file descriptor. If when is given then those bits are removed from 
   each callback for the file descriptor, and the callback removed only if 
   all of the bits turn off. 
  */
  static void remove_fd(int, int when = -1);
  static void set_idle(void (*cb)()) {idle = cb;}
  /**
   Adds a callback function that is called every time by Fl::wait() and also 
   makes it act as though the timeout is zero (this makes Fl::wait() return 
   immediately, so if it is in a loop it is called repeatedly, and thus the 
   idle fucntion is called repeatedly). The idle function can be used to get 
   background processing done. 

   You can have multiple idle callbacks. To remove an idle callback 
   use Fl::remove_idle(). 

   Fl::wait() and Fl::check() call idle callbacks, but Fl::ready() does not. 

   The idle callback can call any FLTK functions, including Fl::wait(), 
   Fl::check(), and Fl::ready(). Fltk will not recursively call the 
   idle callback. 
  */
  static void add_idle(void (*cb)(void*), void* = 0);
  /**
   Returns true if the specified idle callback is currently installed. 
  */
  static bool has_idle(void (*cb)(void*), void* = 0);
  /** Removes the specified idle callback, if it is installed. */
  static void remove_idle(void (*cb)(void*), void* = 0);
  /**
   True if any Fl_Widget::redraw() calls have been done 
   since the last flush(). 
  */
  static int damage() {return damage_;}
  /**
   Redraws all widgets. This is a good idea if you have made global 
   changes to the styles. 
  */
  static void redraw();
  /**
   Causes all the windows that need it to be redrawn and graphics 
   forced out through the pipes. This is what wait() does before 
   looking for events. 
  */
  static void flush();
  /*@{*/
  /**
   FLTK will call these to print messages when unexpected conditions occur.
   
   Fl::warning means that there was a recoverable problem, the display may 
   be messed up but the user can probably keep working. (all X protocol 
   errors call this). 

   Fl::error means there is a recoverable error, but the display is so
   messed up it is unlikely the user can continue (very little calls this now). 

   Fl::fatal must not return, as FLTK is in an unusable state, however your 
   version may be able to use longjmp or an exception to continue, as long as 
   it does not call FLTK again.

   The default versions on Unix print messages to stderr, 
   while on Windows they use MessageBox(). 
   Fl::error and Fl::fatal call abort(). You can override the behavior by 
   setting the function pointers to your own routines. 
  */
  static void (*warning)(const char*, ...);
  static void (*error)(const char*, ...);
  static void (*fatal)(const char*, ...);
  /*@}*/

  // list of top-level shown() windows:

  /**
   Returns the id of some visible() window. If there is more than one, 
   the last one to receive an event is returned. This is useful for 
   setting the Fl_Window::child_of() for dialog boxes that are used 
   in multiple places in your program, and is used by Fl_Window::exec() 
   if no other parent is specified. 
  */
  static Fl_Window* first_window();
  /**
   If this window is visible, this removes it from wherever it is in 
   the list and inserts it at the top, as though it received an event. 
   This can be used to change the parent of dialog boxes run by 
   Fl_Window::exec() or fl_ask(). 
  */
  static void first_window(Fl_Window*);
  /**
   Returns the next visible() top-level window, returns NULL after the last one.
   You can use this and first_window() to iterate through all the visible windows. 
  */
  static Fl_Window* next_window(const Fl_Window*);

  // event information:
  /**
   Returns the most recent event handled, such as FL_PUSH or FL_KEY. 
   This is useful so callbacks can find out why they were called. 
  */
  static int event()		{return e_type;}
  /*@{*/
  /**
   Returns the mouse position of the event relative to the 
   Fl_Window it was passed to. 
  */
  static int event_x()		{return e_x;}
  static int event_y()		{return e_y;}
  /*@}*/
  /*@{*/
  /**
   For FL_MOUSEWHEEL events this is how many clicks the user moved in the x and y 
   directions (currently dx is always zero). 
  */
  static int event_dx()		{return e_dx;}
  static int event_dy()		{return e_dy;}
  /*@}*/
  /*@{*/
  /**
   Returns the mouse position on the screen of the event. 
   To find the absolute position of an Fl_Window on the screen, 
   use the difference between event_x_root(),event_y_root() and 
   event_x(),event_y(). 
  */
  static int event_x_root()	{return e_x_root;}
  static int event_y_root()	{return e_y_root;}
  /*@}*/
  /*@{*/
  /**
   Returns the number of times the last mouse button or keyboard key was 
   pushed while Fl::event_is_click() was true. 

   For a normal FL_PUSH this is zero, if the user "double-clicks" this 
   is one, and it is N-1 for each subsequent click. This is also used to 
   see if the keyboard is repeating, if the most recent FL_KEY was caused 
   by a repeating key this is non-zero, and is N-1 for N repeats of the key. 

   Setting this value with Fl::event_clicks(n) can be used to make callbacks 
   think things were (or were not) double-clicked. 
  */
  static int event_clicks()	{return e_clicks;}
  static void event_clicks(int i) {e_clicks = i;}
  /*@}*/
  /*@{*/
  /**
   This is true if the time and mouse movement since the last FL_PUSH or FL_KEY is 
   short enough that the user is intending to "click". After enough time or after 
   enough mouse movement this turns off. 

   The main use of this is to decide whether to increment Fl::event_clicks() when 
   the user clicks the mouse. But you can also test this on a FL_RELEASE or FL_KEYUP 
   event to decide if the user clicked quickly, versus holding the mouse or key down. 

   You can set this to zero with Fl::event_is_click(0), this can be used to prevent 
   the next mouse click from being considered a double click. It is not possible to 
   set this true because the saved time and position are inaccessible. 
  */
  static bool event_is_click()	{return e_is_click != 0;}
  static void event_is_click(bool) {e_is_click = 0;} // only false works!
  /*@}*/
  /**
   Returns which mouse button was last pushed or released. You can use numerical 
   constants or these values: 
   - FL_LEFT_MOUSE 1 
   - FL_MIDDLE_MOUSE 2 
   - FL_RIGHT_MOUSE 3 
  */
  static int event_button()	{return e_keysym-FL_Button(0);}
  /*@{*/
  /**
   This is a bitfield of what shift states were on and what mouse buttons 
   were held down during the most recent event. The second version returns
   true if any of the passed bits are turned on. The legal bits are:
   
   - FL_SHIFT       - One of the shift keys is down. 
   - FL_CAPS_LOCK   - The caps lock is on. 
   - FL_CTRL        - One of the ctrl keys is down. 
   - FL_ALT         - One of the alt keys is down. 
   - FL_NUM_LOCK    - The num lock is on. 
   - FL_WIN         - One of the Windows keys is down. 
   - FL_SCROLL_LOCK - The scroll lock is on. 
   - FL_BUTTON1     - Mouse button 1 is pushed. 
   - FL_BUTTON2     - Mouse button 2 is pushed. 
   - FL_BUTTON3     - Mouse button 3 is pushed. 
   - FL_BUTTONS     - Any mouse button is pushed. 
   - FL_BUTTON(n)   - Mouse button n is pushed. 

   X servers do not agree on shift states, so FL_NUM_LOCK, FL_WIN, 
   and FL_SCROLL_LOCK may not work. The values were selected to match 
   the XFree86 server on Linux. In addition there is a bug in the way X 
   works so that the shift state is not correctly reported until the first
   event after the shift key is pressed or released. 
  */
  static int event_state()	{return e_state;}
  static bool event_state(int i) {return (e_state&i) != 0;}
  /*@}*/
  /**
   Returns which key on the keyboard was last pushed. FL_FOCUS events 
   produced by the system, and mouse button events, set this to values 
   that cannot be confused with keys, to allow a widget to determine if 
   the focus is being changed due to a keystroke and what keystroke that was. 
   Keys are identified by the unshifted values. FLTK defines a set of 
   symbols that should work on most modern machines for every key on 
   the keyboard:

   - 'a' through 'z', '0' through '9', '`', '-', '=', '[', ']', '\\',
      ';', '\'', ',', '.', '/' - All keys on the main keyboard producing a 
      printable ASCII character use the value of that ASCII character (as 
      though shift, ctrl, and caps lock were not on). 
   - Fl_Space     - The space key, same as ' ' or 32. 
   - FL_BackSpace - The backspace key. 
   - FL_Tab       - The tab key. 
   - FL_Enter     - The enter key. 
   - FL_Pause     - The pause key. 
   - FL_Scroll_Lock - The scroll lock key. 
   - FL_Escape    - The escape key. 
   - FL_Home      - The home key. 
   - FL_Left      - The left arrow key. 
   - FL_Up        - The up arrow key. 
   - FL_Right     - The right arrow key. 
   - FL_Down      - The down arrow key. 
   - FL_Page_Up   - The page-up key. 
   - FL_Page_Down - The page-down key. 
   - FL_End       - The end key. 
   - FL_Print     - The print (or print-screen) key. 
   - FL_Insert    - The insert key. 
   - FL_Menu      - The menu key. 
   - FL_Num_Lock  - The num lock key. 
   - FL_KP(c)     - One of the keypad numbers, 
                    for instance FL_KP('5') is the 5 key 
   - FL_KP_Enter  - Same as FL_KP('\r'), the enter key on the keypad. 
   - FL_KP_Last   - All keypad keys are in the range FL_KP(0),FL_KP_Last 
   - FL_F(n)      - A numbered function key, 
                    for instance FL_F(2) is the F2 key. 
   - FL_F_Last    - All function keys are in the range FL_F(0),FL_F_Last 
   - FL_Shift_L   - The lefthand shift key. 
   - FL_Shift_R   - The righthand shift key. 
   - FL_Control_L - The lefthand control key. 
   - FL_Control_R - The righthand control key. 
   - FL_Caps_Lock - The caps lock key. 
   - FL_Alt_L     - The left alt key. 
   - FL_Alt_R     - The right alt key. 
   - FL_Win_L     - The left Windows key. 
   - FL_Win_R     - The right Windows key. 
   - FL_Delete    - The delete key. 
   On X systems any unrecognized keys are reported as their X keysym value. 
  */
  static int event_key()	{return e_keysym;}
  /**
   Returns true if the given key was held down (or pressed) during the 
   last event. This is constant until the next event is read from the server.
   The possible values for the key are listed above. 

   On Win32 Fl::event_key_state(FL_KP_Enter) does not work. 
  */
  static bool event_key_state(int);
  /**
   Returns the ASCII text (in the future this may be UTF-8) produced by the 
   last FL_KEY or FL_PASTE or possibly other event. A zero-length string is 
   returned for any keyboard function keys that do not produce text. This 
   pointer points at a static buffer and is only valid until the next event 
   is processed. 

   Under X this is the result of calling XLookupString().
  */
  static const char* event_text() {return e_text;}
  /**
   Returns the length of the text in Fl::event_text(). There will always be a
   nul at this position in the text. However there may be a nul before that 
   if the keystroke translates to a nul character or you paste a nul character. 
  */
  static int event_length() {return e_length;}

  /**
   Returns non-zero if the current Fl::event_x() and Fl::event_y() put 
   it inside the passed box. You should always call this rather than 
   doing your own comparison so you are consistent about edge effects.
  */
  static bool event_inside(int x,int y,int w,int h);
  /**
   Test the current event, which must be an FL_KEY or FL_SHORTCUT, 
   against a shortcut value (described in Fl_Button). Returns non-zero
   if there is a match. Not to be confused with Fl_Widget::test_shortcut(). 
  */
  static bool test_shortcut(int shortcut);
  /**
   Unparse a key name (as returned by Fl::event_key()) or a shortcut 
   value (as used by Fl_Button or Fl_Menu_Item) into a human-readable
   string like "Alt+N". If the shortcut is zero an empty string is returned.
   The return value points at a static buffer that is overwritten with 
   each call. 
  */
  static const char* key_name(int key);
  /**
   Use of this function is very simple. Any text editing widget should call 
   this for each FL_KEY event. 

   If true is returned, then it has modified the Fl::event_text() and 
   Fl::event_length() to a set of bytes to insert (it may be of zero length!).
   It will also set the del parameter to the number of bytes to the left of 
   the cursor to delete, this is used to delete the results of the previous 
   call to Fl::compose(). 

   If false is returned, the keys should be treated as function keys. 
   You could insert the text anyways, if you don't know what else to do, 
   del is set to zero and the Fl::event_text() and Fl::event_length() are 
   left unchanged, length is zero for any function keys. 

   Though the current implementation returns immediately, future versions 
   may take quite awhile, as they may pop up a window or do other 
   user-interface things to allow characters to be selected. 
  */
  static bool compose(int &del);
  /**
   If the user moves the cursor, be sure to call Fl::compose_reset(). 
   The next call to Fl::compose() will start out in an initial state. 
   In particular it will not set "del" to non-zero. This call is very 
   fast so it is ok to call it many times and in many places.
  */
  static void compose_reset() {compose_state = 0;}

  /**
   Returns true if the given key is held down now. Under X this requires a
   round-trip to the server and is much slower than Fl::event_key_state(int).

   On Win32 Fl::get_key_state(FL_KP_Enter) does not work. 
  */
  static bool get_key_state(int);
  /**
   Return where the mouse is on the screen by doing a round-trip query to 
   the server. You should use Fl::event_x_root() and Fl::event_y_root() if
   possible, but this is necessary if you are not sure if a mouse event has
   been processed recently (such as to position your first window). If the 
   display is not open, this will open it. 
  */
  static void get_mouse(int &,int &);

  // event destinations:
  static bool handle(int, Fl_Window*);
  /**
   Install a function to parse unrecognized events. If FLTK cannot figure 
   out what to do with an event, it calls each of these functions 
   (most recent first) until one of them returns non-zero. If none of them 
   returns non zero then the event is ignored.
   
   Currently there are only two uses for this: 

   1. If there is a keystroke that no widgets are interested in, 
      this is called with FL_SHORTCUT. You can use this to implement global
      shortcut keys. 

   2. On X, if FLTK does not recognize an X event or the window id the 
      event is sent to, this is called with zero. You can then use system 
      specific code to access the event data and figure out what to do. 
      (this is not done on Win32 due to the enormous number of bogus events
      sent, it was much to slow to search the handler list for every one). 

   Ignore any other values this is called with. We have not figured out what
   the rules for these are yet. 
  */
  static void add_handler(int (*h)(int));
  /**
   Get the widget that is below the mouse. This is the last widget to 
   respond to an FL_ENTER event as long as the mouse is still pointing at it.
   This is for highlighting buttons and bringing up tooltips. It is not used 
   to send FL_PUSH or FL_MOVE directly, for several obscure reasons, but those
   events typically go to this widget. 
  */
  static Fl_Widget* belowmouse() {return belowmouse_;}
  /*@{*/
  /**
  Change the Fl::belowmouse() widget, the previous one and all 
  parents (that don't contain the new widget) are sent FL_LEAVE events.
  Changing this does not send FL_ENTER to this or any widget, because 
  sending FL_ENTER is supposed to test if the widget wants the mouse (by it 
  returning non-zero from handle()).
  */
  static void belowmouse(Fl_Widget*);
  static void belowmouse(Fl_Widget& w) {belowmouse(&w);}
  /*@}*/
  /**
   Get the widget that is being pushed. FL_DRAG or FL_RELEASE (and any 
   more FL_PUSH) events will be sent to this widget. This is null if no 
   mouse button is being held down, or if no widget responded to the 
   FL_PUSH event. 
  */
  static Fl_Widget* pushed()	{return pushed_;}
  /*@{*/
  /**
   Change the Fl::pushed() widget, the previous one and all parents 
   (that don't contain the new widget) are sent FL_RELEASE events. 
   Changing this does not send FL_PUSH to this or any widget, 
   because sending FL_PUSH is supposed to test if the widget wants 
   the mouse (by it returning non-zero from handle()).
  */
  static void pushed(Fl_Widget*);
  static void pushed(Fl_Widget& w) {pushed(&w);}
  /*@}*/
  /**
   Returns the widgets that will receive FL_KEY events. 
   This is NULL if the application does not have focus now, 
   or if no widgets accepted focus.
  */
  static Fl_Widget* focus()	{return focus_;}
  /*@{*/
  /**
   Change Fl::focus() to the given widget, the previous widget and all 
   parents (that don't contain the new widget) are sent FL_UNFOCUS 
   events, the new widget and all parents that don't contain the old 
   widget are sent FL_FOCUS events. Fl::focus() is set whether or not 
   the applicaton has the focus or if the widgets accept the focus. You
   may want to use Fl_Widget::take_focus() instead, it will test first. 
  */
  static void focus(Fl_Widget*);
  static void focus(Fl_Widget& w) {focus(&w);}
  /*@}*/

  // cut/paste:
  /**
   Change the current selection. The block of text is copied to an internal 
   buffer by FLTK (be careful if doing this in response to an FL_PASTE as 
   this may be the same buffer returned by event_text()). 

   The block of text may be retrieved (from this program or whatever program 
   last set it) with Fl::paste(). 

   There are actually two buffers. If clipboard is true then the 
   text goes into the user-visible selection that is moved around 
   with cut/copy/paste commands (on X this is the CLIPBOARD selection). 
   If clipboard is false then the text goes into a less-visible buffer used 
   for temporarily selecting text with the mouse and for drag & drop (on X 
   this is the XA_PRIMARY selection). 

   Copying the buffer every time the selection is changed is obviously 
   wasteful, especially for large selections. An interface will probably be
   added in a future version to allow the selection to be made by a callback
   function. The current interface will be emulated on top of this. 
  */
  static void copy(const char* stuff, int len, bool clipboard = false);
  /**
   This is what a widget does when a "paste" command (like Ctrl+V or the 
   middle mouse click) is done to it. Cause an FL_PASTE event to be sent 
   to the receiver with the contents of the current selection in the 
   Fl::event_text(). The selection can be set by Fl::copy(). 

   There are actually two buffers. If clipboard is true then the text is 
   from the user-visible selection that is moved around with cut/copy/paste
   commands (on X this is the CLIPBOARD selection). If clipboard is false 
   then the text goes into a less-visible buffer used for temporarily
   selecting text with the mouse and for drag & drop (on X this is the 
   XA_PRIMARY selection). 

   The reciever should be prepared to be called directly by this, or 
   for it to happen later, or possibly not at all. This allows the window
   system to take as long as necessary to retrieve the paste buffer (or 
   even to screw up completely) without complex and error-prone 
   synchronization code most toolkits require. 
  */
  static void paste(Fl_Widget &receiver, bool clipboard = false);
  /**
   Drag and drop the data set by the most recent Fl::copy() (with the 
   clipboard argument false). Returns true if the data was dropped on 
   something that accepted it. 
   
   By default only blocks of text are dragged. You can use 
   system-specific variables to change the type of data. 
  */
  static bool dnd();

  // screen size (any any other static information from system):
  /**
   Return a structure of information describing the current state of the 
   screen. Currently this structure contains the following (this may be 
   added to in the future): 
   - int x         left edge of work area 
   - int y         top edge of work area 
   - int w         width of work area 
   - int h         height of work area 
   - int width     full screen width in pixels 
   - int height    full screen height in pixels 
   - int depth     bits per pixel 
   - int width_mm  screen width in millimeters 
   - int height_mm screen height in millimeters 
  */
  static const Fl_Screen_Info& info();

  // Multithreading support:
  /**
   Blocks the current thread until it can safely access FLTK widgets and data.
   Child threads should call this method prior to updating any widgets or 
   accessing data. The main thread must call Fl::lock() to initialize the 
   threading support in FLTK before calling Fl::wait() or Fl::run(). 

   Child threads must call Fl::unlock() when they are done accessing FLTK. 
   They may want to call Fl::awake() first if the display needs to change. 

   This is a "recursive lock". If you call Fl::lock() more than once, the 
   subsequent calls return immediately. But you must call Fl::unlock() the
   same number of times as you called Fl::lock() before the lock is released. 

   When the wait() method is waiting for input or timeouts, child threads are
   given access to FLTK. Similarly, when the main thread receives events and 
   needs to do processing, it will wait until all child threads have called 
   unlock() before processing the events and doing callbacks. 

   See the file <fltk/Fl_Threads.h> for a simple portable recursive lock 
   object you can use in your own code for locking other objects. However 
   there is no requirement that you use this, you can use pthreads or any 
   other library that is compatable with your system. 
   */
  static void lock();
  /**
   Releases the lock that was set using the Fl::lock() method. 
   Child threads should call this method as soon as they are 
   finished accessing FLTK. If some other thread is waiting for 
   Fl::lock() to return, it will get control. 
  */
  static void unlock();
  /**
   Make the main thread (the one that is calling Fl::wait()) wake up. 
   The main purpose of this is to get the main thread to redraw the 
   screen, but it will also cause Fl::wait() to return so the program's
   code can do something. 

   Due to obscure race problems you must call this after you call Fl::unlock().
   If you call this while things are locked both the X and Win32 
   implementations can deadlock. 

   The message argument can be retrieved by the other thread 
   using Fl::thread_message(). 
  */
  static void awake(void* message = 0);
  /**
   Returns an argument sent to an Fl::awake() call, or returns null if none.
   The current implementation only has a one-entry queue and only returns 
   the most recent value! 
  */
  static void* thread_message();

#ifndef FLTK_2  // back-compatability section:
  // commented-out functions could not be emulated in fltk 2.0
  static int x() {return info().x;}
  static int y() {return info().y;}
  static int w() {return info().width;}
  static int h() {return info().height;}

  static void set_color(Fl_Color c,uchar r,uchar g,uchar b) {fl_set_color(c,fl_rgb(r,g,b));}
  static void set_color(Fl_Color c, Fl_Color v) {fl_set_color(c, v);}
  static unsigned get_color(Fl_Color c) {return fl_get_color(c);}
  static void get_color(Fl_Color c,uchar& r,uchar& g,uchar& b) {fl_get_color(c,r,g,b);}
  static void free_color(Fl_Color c, int = 0) {fl_free_color(c);}
  static void background(uchar r, uchar g, uchar b) {fl_background(fl_rgb(r,g,b));}
  //static void foreground(uchar r, uchar g, uchar b)

  //static const char* get_font(Fl_Font);
  //static const char* get_font_name(Fl_Font, int* attributes = 0);
  //static int get_font_sizes(Fl_Font, int*& sizep);
  //static void set_font(Fl_Font, const char*);
  //static void set_font(Fl_Font, Fl_Font);
  //static Fl_Font set_fonts(const char* = 0);
  //static void set_encoding(const char *e, const char *loc);

  //static void set_labeltype(Fl_Labeltype,y,z);
  //static void set_labeltype(Fl_Labeltype, Fl_Labeltype from);
  static void enable_symbols();

  //static void set_boxtype(Fl_Boxtype,Fl_Box_Draw_F*,uchar,uchar,uchar,uchar);
  //static void set_boxtype(Fl_Boxtype, Fl_Boxtype from);

  static int box_dx(Fl_Boxtype b) {return b->dx();}
  static int box_dy(Fl_Boxtype b) {return b->dy();}
  static int box_dw(Fl_Boxtype b) {return b->dw();}
  static int box_dh(Fl_Boxtype b) {return b->dh();}

  static void set_abort(void (*f)(const char*,...)) {fatal = f;}
  static bool event_shift() {return (e_state&FL_SHIFT)!=0;}
  static bool event_ctrl() {return (e_state&FL_CTRL)!=0;}
  static bool event_alt() {return (e_state&FL_ALT)!=0;}
  static bool event_buttons() {return (e_state&FL_BUTTONS)!=0;}
  static bool event_button1() {return (e_state&FL_BUTTON1)!=0;}
  static bool event_button2() {return (e_state&FL_BUTTON2)!=0;}
  static bool event_button3() {return (e_state&FL_BUTTON3)!=0;}

#endif

};

#endif

//
// End of "$Id: Fl.h,v 1.13 2002/10/29 00:37:22 easysw Exp $".
//
