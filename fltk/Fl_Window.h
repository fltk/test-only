//
// "$Id: Fl_Window.h,v 1.6 2002/10/29 00:37:23 easysw Exp $"
//
// Window header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Window_H
#define Fl_Window_H

#include "Fl_Group.h"

// value for x,y to indicate window system places window
#define  FL_USEDEFAULT ((int)0x80000000) // same as Win32 value

/**

   This widget produces an actual window. This can either be a main window, 
   with a border and title and all the window management controls, or a 
   "subwindow" inside a window. This is controlled by whether or not the 
   window has a parent(). 

   Once you create a window, you usually add children Fl_Widget's to it by
   using window->add(child) for each new widget. See Fl_Group for more 
   information on how to add and remove children. 

   There are several subclasses of Fl_Window that provide double-buffering,
   overlay, menu, and OpenGL support. 

   The window's callback is done if the user tries to close a window using
   the window manager and Fl::modal() is zero or equal to the window. 
   Fl_Window has a default callback that calls Fl_Window::hide() and calls
   exit(0) if this is the last top-level window. 

*/
class FL_API Fl_Window : public Fl_Group {
public:

   /**
   The first form of the constructor should be used for a "top-level" 
   window (that is, one that is not inside another window). 
   It correctly sets visible() to false and parent() to NULL. By not 
   specifying the position of the window, the window system will pick 
   a place to show the window (some older X window managers will allow 
   the user to place the window by hand). If you want to force a position
   you should call position(x,y) or hotspot() before calling show(). 

   Fl_Widget::box() is set to FL_FLAT_BOX. If you plan to completely fill
   the window with children widgets you should change this to FL_NO_BOX. 
   If you turn the window border off you may want to change this to FL_UP_BOX. 
  */
  Fl_Window(int,int, const char* = 0);

  /**
   The second form of the constructor is for creating child windows. 
   It leaves visible() set to true.
  */
  Fl_Window(int,int,int,int, const char* = 0);
  static Fl_Named_Style* default_style;
  /**
   Calls destroy(). The destructor also deletes all the children. 
   This allows a whole tree to be deleted at once, without having 
   to keep a pointer to all the children in the user code. A kludge
   has been done so the Fl_Window and all of it's children can be
   automatic (local) variables, but you must declare the Fl_Window 
   first so that it is destroyed last. 
  */
  virtual ~Fl_Window();

  /*@{*/
  /** Gets or sets the window title bar label. */
  const char* label() const	{return Fl_Widget::label();}
  void label(const char*);
  /*@}*/
  /*@{*/
  /** Gets or sets the icon label. */
  const char* iconlabel() const	{return iconlabel_;}
  void iconlabel(const char*);
  /*@}*/
  void label(const char* label, const char* iconlabel);
  void copy_label(const char* c) {Fl_Widget::copy_label(c); label(label());}
  const void* icon() const	{return icon_;}
  void icon(const void * ic)	{icon_ = ic;}
  static const char* xclass()	{return xclass_;}
  static void xclass(const char* v) {xclass_ = v;}

  /**
   You may turn off the window manager border before calling show()
   on the window the first time. On most window managers this means
   the user cannot move, iconize, or resize the window 
   (unless your program does it). 
  */
  void clear_border()	{set_flag(FL_NOBORDER);}
  /** Returns false if clear_border() has been called. */
  bool border() const	{return !(flags() & FL_NOBORDER);}
  /**
   Windows with this property set will use the exact position and size set
   by the programmer (will not be handled by the window manager) and will
   not have an entry in the task list. This will also clear the window's 
   border like clear_border() above. This is used by the fltk menus and 
   tooltips. 
   
   On X this causes "override redirect". This is only good for short-lived
   windows as it can confuse X window managers, however this is the only 
   reliable and fast way to do it. This also turns on "save under" which 
   on many X servers (like XFree86) can make the window disappear much 
   faster by having the server rememeber what was behind it. 
  */
  void set_override()	{set_flag(FL_NOBORDER|FL_OVERRIDE);}
  /**
   Returns true if set_override() has been called. 
  */
  bool override() const {return (flags()&FL_OVERRIDE)!=0; }
  /*@{*/
  /**
   Tell the system that this window will not have an icon, it will dissappear
   and reappear when the parent window is iconized or shown, and it is forced
   to always be above the parent window. On X this is called a 
   "Transient window", and Windows calls this a "overlapping child". This 
   value is different than the Fl_Widget::parent(), which must be zero). 

   On both X and win32 changing this value causes the window to be removed 
   from the screen. 

   Win32 and some X window managers have an annoying bug where calling 
   show() on this window will also raise the parent window to right below
   this, making many useful user interface designs impossible! On X some
   old window managers may ignore the "TransisentForHint" and these windows
   will come out the same as other windows. 

   If you want a dialog that blocks interaction with the other windows of 
   your application or with all other applications, you need to look at 
   exec() or possibly Fl::modal() 
  */
  const Fl_Window* child_of() const {return child_of_;}
  void child_of(const Fl_Window* w);
  /*@}*/
  void set_modal()	{set_flag(FL_MODAL);} // back compatability only!
  void set_non_modal()	{set_flag(FL_NON_MODAL);} // back compatability only!

  /*@{*/
  /**
   position() the window so that the mouse is pointing at the given position,
   or at the center of the given widget, which may be the window itself. 
   If offscreen is true then the window is allowed to extend off the
   screen (some X window managers do not allow this). 
  */
  void hotspot(int x, int y, bool offscreen = false);
  void hotspot(const Fl_Widget*, bool offscreen = false);
  void hotspot(const Fl_Widget& p, bool offscrn = false) {hotspot(&p,offscrn);}
  /*@}*/
  /**
   Set the allowable range the user can resize this window to. 
   This only works for top-level windows. 

   - minw and minh are the smallest the window can be. 

   - maxw and maxh are the largest the window can be. If either is equal to 
   the minimum then you cannot resize in that direction. If either is zero
   then FLTK picks a maximum size in that direction such that the window 
   will fill the screen. 
   
   - dw and dh are size increments. The window will be constrained to widths
   of minw + N * dw, where N is any non-negative integer. If these are less
   or equal to 1 they are ignored. (this is ignored on WIN32) 
   
   - aspect is a flag that indicates that the window should preserve it's
   aspect ratio. This is ignored by WIN32 and by most X window managers
   (in addition X does not describe what to do if the minimum and maximum
   sizes have different aspect ratios...) 
   
   It is undefined what happens if the current size does not fit in the
   constraints passed to size_range(). 
   
   If this function is not called, FLTK tries to figure out the range
   from the setting of resizeable(): 

   - If resizeable() is NULL (this is the default) then the window cannot
   be resized. 
   
   - If either dimension of resizeable() is less than 100, then that is
   considered the minimum size. Otherwise the resizeable() has a 
   minimum size of 100. 
   
   - If either dimension of resizeable() is zero, then that is also the
   maximum size (so the window cannot resize in that direction). 
  */
  void size_range(int a, int b, int c=0, int d=0, int e=0, int f=0)
    { minw=a; minh=b; maxw=c; maxh=d; dw=e; dh=f; size_range_(); }

  /**
   Returns non-zero if show() has been called, 
   but destroy() has not been called.
  */
  bool shown() const {return i != 0;}
  /**
   Put the window on the screen. Usually this has the side effect of 
   opening the display. 
   
   It is harmless to call show() multiple times. If the window is 
   already shown then it is deiconized and raised to the top. 
  */
  void show();
  /**
   This must be called after Fl::args(argc,argv) to show 
   the "main" window, this indicates which window should 
   be affected by any -geometry switch. In addition if Fl::args() 
   has not been called yet this does so, this is a useful shortcut 
   for the main window in a small program.
  */
  void show(int, char**);
  /** Same as child_of(parent),show(). */
  void show(const Fl_Window* parent);
  /**
   Simple description: the window is popped up and this function waits until
   the user closes it, this then returns a true if the user hit ok, false if
   the user cancelled or closed the window in some other way. During this 
   time events to other windows in this application are either thrown away 
   or redirected to this window. 

   This does child_of(parent) (using Fl::first_window() if parent is null.
   It then does show() to make the window visible and raise it. It then 
   uses Fl::modal(this,grab) to make all events go to this window, and waits
   until Fl::exit_modal() is called (typically by the window being hidden 
   or destroyed). 

   The return value is value() of the window, which is true only if some 
   callback does window->set(). To use this, make an OK button with a 
   callback that does this. 
   
   If parent is null the window that last received an event is used as 
   the parent. This is convenient for popups that appear in response 
   to a mouse or key click. 

   See Fl::modal() for what grab does. This is useful for popup menus.
  */
  bool exec(const Fl_Window* parent = 0, bool grab = false);
  /**
   Make the window with a normal system border and behavior, but place it
   inside the frame as though that was the desktop. This is what Windows
   calls "MDI". Typically the other window (which must already be shown)
   is a child window so that space can remain around it for a menu/tool bar. 
   
   Notice that parent() of the window must be zero and it will remain 
   zero after this is called. Fltk uses a zero parent to indicate that 
   the system is managing the window. 

   On systems that don't support nested desktops (i.e. X) this does
   show(frame), which produces a normal overlapping window that is 
   tied to remain atop and iconize with the frame window (IMHO this
   is a great improvement over MDI!). 
  */
  void show_inside(const Fl_Window* parent);
  /**
   Iconifies the window. If you call this when shown() is false 
   it will show() it as an icon. If the window is already 
   iconified this does nothing. 
   Call show() to restore the window. 

   Currently there are only X and Win32 system-specific ways to 
   control what is drawn in the icon. You should not rely on window
   managers displaying the icons.
  */
  void iconize();
  /**
   Returns true if the window is currently displayed as an icon. 
   Returns false if the window is not shown() or hide() has been 
   called. On X this will return true for a short time after 
   show() is called the first time.
  */
  bool iconic() const;
  /**
   Hides the window and also deletes all window system information 
   about the window, and thus returns if back to the state it was 
   in before the first show(). It is harmless to call this if the 
   window is already destroyed. 

   Subclasses can override this, if you do this you must also 
   override the destructor and make it call destroy(). 
  */
  virtual void destroy();

  /**
   Makes the window completely fill the screen, without any window manager 
   border or taskbar or anything else visible (at least that is the 
   intention, it is currenty not well implemented, only older X window
   managers work. The Windows and Gnome and KDE taskbars remain visible.
   Anybody who knows how to get this to work reliably is welcome to
   contribute code to do so). 
   
   You must use fullscreen_off() to undo this. This may not work with all 
   X window managers (and currently it is not successful at hiding
   the Windows taskbar). 
  */
  void fullscreen();
  /**
   Turns off any side effects of fullscreen() and does resize(x,y,w,h). 
  */
  void fullscreen_off(int x,int y,int w,int h);

  /** Returns the last window that was made current. */
  static const Fl_Window *current() {return current_;}
  /**
   make_current() sets things up so that the drawing functions 
   in <fltk/fl_draw.h> will go into this window. This is useful 
   for incremental update of windows, such as in an idle callback,
   which will make your program behave much better if it draws a 
   slow graphic. This call does not work for Fl_Double_Window!. 
  */
  void make_current() const;

  /**

   Change the cursor for this window. This always calls the system,
   if you are changing the cursor a lot you may want to keep track 
   of how you set it in a static varaible and call this only if the
   new cursor is different. The colors only work on X, they are not
   implemented on WIN32.

   The following constants define the mouse cursors that 
   are available:

   - FL_CURSOR_DEFAULT - the default cursor, usually an arrow (0) 
   - FL_CURSOR_ARROW - an arrow pointer 
   - FL_CURSOR_CROSS - crosshair 
   - FL_CURSOR_WAIT - watch or hourglass 
   - FL_CURSOR_INSERT - I-beam 
   - FL_CURSOR_HAND - hand (uparrow on MSWindows) 
   - FL_CURSOR_HELP - question mark 
   - FL_CURSOR_MOVE - 4-pointed arrow 
   - FL_CURSOR_NS - up/down arrow 
   - FL_CURSOR_WE - left/right arrow 
   - FL_CURSOR_NWSE - diagonal arrow 
   - FL_CURSOR_NESW - diagonal arrow 
   - FL_CURSOR_NONE - invisible 

   Under X you can get any XC_cursor value
   by passing Fl_Cursor((XC_foo/2)+1)). 

  */
  void cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE);
  static void default_callback(Fl_Window*, void* v);

  virtual int handle(int);
  virtual void layout();
  virtual void flush();
  virtual void draw();

protected:
  virtual void create();
  static const Fl_Window *current_;

private:

  friend class Fl_X; Fl_X *i; // points at the system-specific stuff
  const Fl_Window* child_of_;
  const char* iconlabel_;
  const void* icon_;
  // size_range stuff:
  short minw, minh, maxw, maxh;
  unsigned char dw, dh, size_range_set;
  void size_range_();
  // values for flags():
  enum {
    FL_MODAL		= 0x80000000,
    FL_NOBORDER 	= 0x40000000,
    FL_OVERRIDE         = 0x20000000,
    FL_NON_MODAL	= 0x10000000
  };
  static const char* xclass_;
  void _Fl_Window(); // constructor innards
};

#endif

//
// End of "$Id: Fl_Window.h,v 1.6 2002/10/29 00:37:23 easysw Exp $".
//
