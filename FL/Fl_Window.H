/
// "$Id$
/
// Window header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#ifndef Fl_Window_
#define Fl_Window_

#include "Fl_Group.H

#define FL_WINDOW 0xF0	// all subclasses have type() >= thi
#define FL_DOUBLE_WINDOW 0xF

class Fl_X

/*
 * This widget produces an actual window. This can either be a main window, with a border
 * and title and all the window management controls, or a "subwindow" inside a window.
 * This is controlled by whether or not the window has a parent()
 *
 * Once you create a window, you usually add children Fl_Widget's to it by using
 * window->add(child) for each new widget. See Fl_Group for more information on how to
 * add and remove children
 *
 * There are several subclasses of Fl_Window that provide double-buffering, overlay, menu,
 * and OpenGL support
 
 * The window's callback is done if the user tries to close a window using the window manager
 * and Fl::modal() is zero or equal to the window. Fl_Window has a default callback that
 * calls Fl_Window::hide().
 *
class FL_EXPORT Fl_Window : public Fl_Group 

  friend class Fl_X
  Fl_X *i; // points at the system-specific stuf

  const char* iconlabel_
  const char* xclass_
  const void* icon_
  // size_range stuff
  int minw, minh, maxw, maxh
  uchar dw, dh, aspect, size_range_set
  // cursor stuf
  Fl_Cursor cursor_default
  Fl_Color cursor_fg, cursor_bg
  void size_range_()
  // values for flags()
  enum 
    FL_MODAL = 64
    FL_NOBORDER = 8
    FL_FORCE_POSITION = 16
    FL_NON_MODAL = 32
    FL_OVERRIDE = 25
  }
  void _Fl_Window(); // constructor innard

protected

  static Fl_Window *current_
  virtual void draw()
  virtual void flush()

public

    /** This form of the constructor is for creating child windows. It leaves
     * visible() set to \c true. *
  Fl_Window(int x,int y,int width,int height, const char *label = 0)
    /** This form of the constructor should be used for a "top-level" window (that is, one
     * that is not inside another window). It correctly setsvisible() to \c false and
     * parent() to \c NULL. By not specifying the position of the window, the window system
     * will pick a place to show the window or allow the user to pick a location. If you
     * want to force a position you should call position(x,y) or hotspot() before
     * calling show()
     
     * Fl_Widget::box() is set to \c FL_FLAT_BOX. If you plan to completely fill the window
     * with children widgets you should change this to \c FL_NO_BOX. If you turn the windo
     * border off you may want to change this to \c FL_UP_BOX. *
  Fl_Window(int width,int height, const char *label = 0)
    /** The destructor <i>also deletes all the children</i>. This allows a whole tree to be
     * deleted at once, without having to keep a pointer to all the children in the user
     * code. A kludge has been done so the Fl_Window and all of it's children can be
     * automatic (local) variables, but you must declare the Fl_Window first so that
     * it is destroyed last. *
  virtual ~Fl_Window()

  virtual int handle(int)

    /** Change the size and position of the window. If shown() is true, these changes are
     * communicated to the window server (which may refuse that size and cause a
     * further resize). If shown() is false, the size and position are used when show()
     * is called. See Fl_Group for the effect of resizing on the child widgets
     
     * You can also call the Fl_Widget methods size(x,y) and position(w,h), which are
     * inline wrappers for this virtual function. *
  virtual void resize(int x,int y,int width,int height)
    /** Sets whether or not the window manager border is around the window. The
     * default value is true. border(n) can be used to turn the border on and off, and
     * returns non-zero if the value has been changed. <i>Under most X window managers this
     * does not work after show() has been called, although SGI's 4DWM does work.</i> *
  void border(int b)
    /** clear_border() is a fast inline function to turn the border off. It only works
     * before show() is called. *
  void clear_border()	{set_flag(FL_NOBORDER);
    /** Gets whether or not the window manager border is around the window. *
  int border() const	{return !(flags() & FL_NOBORDER);
  void set_override()	{set_flag(FL_NOBORDER|FL_OVERRIDE);
  int override() const  { return flags()&FL_OVERRIDE; 
    /** A "modal" window, when shown(), will prevent any events from being delivered t
     * other windows in the same program, and will also remain on top of the other window
     * (if the X window manager supports the "transient for" property). Several modal
     * windows may be shown at once, in which case only the last one shown gets events.
     * You can see which window (if any) is modal by calling Fl::modal(). *
  void set_modal()	{set_flag(FL_MODAL);
    /** Returns true if this window is modal. *
  int modal() const	{return flags() & FL_MODAL;
    /** A "non-modal" window (terminology borrowed from Microsoft Windows) acts like a
     * modal() one in that it remains on top, but it has no effect on event delivery.
     * There are three states for a window: modal, non-modal, and normal. *
  void set_non_modal()	{set_flag(FL_NON_MODAL);
    /** Returns true if this window is modal or non-modal. *
  int non_modal() const {return flags() & (FL_NON_MODAL|FL_MODAL);

    /** position the window so that the mouse is pointing at the given position, or at the
     * center of the given widget, which may be the window itself. If the optional offscree
     * parameter is non-zero, then the window is allowed to extend off the screen (this doe
     * not work with some window managers). *
  void hotspot(int x, int y, int offscreen = 0)
  void hotspot(const Fl_Widget*, int offscreen = 0)
  void hotspot(const Fl_Widget& p, int offscreen = 0) {hotspot(&p,offscreen);
    /** Undoes the effect of a previous resize() or show() so that the next time show()
     * is called the window manager is free to position the window. *
  void free_position()	{clear_flag(FL_FORCE_POSITION);
    /** Set the allowable range the user can resize this window to. This only works for
     * top-level windows
     * \arg \c minw and \c minh are the smallest the window can be
     * \arg \c maxw and \c maxh are the largest the window can be. If either is equal to
     *      the minimum then you cannot resize in that direction. If either is zero then
     *      FLTK picks a maximum size in that direction such that the window will fill
     *      the screen
     * \arg \c dw and \c dh are size increments. The window will be constrained to widths
     *      of <i>minw + N * dw</i>, where \a N is any non-negative integer. If these are
     *      less or equal to 1 they are ignored. (this is ignored on WIN32
     * \arg \c aspect is a flag that indicates that the window should preserve it's aspect
     *      ratio. This only works if both the maximum and minimum have the same aspect ratio.
     *      (ignored on WIN32 and by many X window managers
     *
     * If this function is not called, FLTK tries to figure out the range from the setting
     * of resizable()
     * \arg If resizable() is \c NULL (this is the default) then the window cannot be
     *      resized and the resize border and max-size control will not be displayed for
     *      the window
     * \arg If either dimension of resizable() is less than 100, then that is considere
     *      the minimum size. Otherwise the resizable() has a minimum size of 100
     * \arg If either dimension of resizable() is zero, then that is also the maximu
     *      size (so the window cannot resize in that direction)
     *
     * It is undefined what happens if the current size does not fit in the constraints
     * passed to size_range(). *
  void size_range(int minw, int minh, int maxw=0, int maxh=0, int dw=0, int dh=0, int aspect=0) 
    this->minw=minw; this->minh=minh; this->maxw=maxw; this->maxh=maxh;
    this->dw=dw; this->dh=dh; this->aspect=aspect; size_range_();

    /** Gets the window title bar label. *
  const char* label() const	{return Fl_Widget::label();
    /** Gets the icon label. *
  const char* iconlabel() const	{return iconlabel_;
    /** Sets the window title bar label. *
  void label(const char *new_text)
    /** Sets the icon label. *
  void iconlabel(const char *icon)
  void label(const char* label, const char* iconlabel)
  const char* xclass() const	{return xclass_;
    /** A string used to tell the system what type of window this is. Mostly this identifies
     * the picture to draw in the icon. Under X, this is turned into a \c XA_WM_CLASS pair b
     * truncating at the first non-alphanumeric character and capitalizing the first character
     * and the second one if the first is 'x'. Thus "foo" turns into "foo, Foo", and "xprog.1
     * turns into "xprog, XProg". This only works if called before calling show()
     *
     * Under Microsoft Windows this string is used as the name of the \c WNDCLASS structure,
     * though it is not clear if this can have any visible effect. *
  void xclass(const char* c)	{xclass_ = c;
    /** Returns the window icon. *
  const void* icon() const	{return icon_;
    /** Sets the window icon
     * \arg \c icon is an operating systems specific pointer to an appropriate structure *
  void icon(const void * ic)	{icon_ = ic;

    /** Returns non-zero if show() has been called (but not hide() ). You can tell if a
     * window is iconified with (w->shown() &!w->visible()). *
  int shown() {return i != 0;
    /** Put the window on the screen. *
  virtual void show()
    /** Remove the window from the screen. If the window is already hidden or has not been
     * shown then this does nothing and is harmless. *
  virtual void hide()
    /** Put the window on the screen. Usually this has the side effect of opening the
     * display (X11). This form is used for top-level windows and allow standard arguments
     * to be parsed from the command-line
     
     * If the window is already shown then it is restored and raised to the top. This is
     * really convenient because your program can call show() at any time, even if the
     * window is already up. It also means that show() serves the purpose of raise() in
     * other toolkits *
  void show(int argc, char **argv)
    /** Makes the window completely fill the screen, without any window manager border
     * visible. You must use fullscreen_off() to undo this. This may not work with all
     * window managers. *
  void fullscreen()
    /** Turns off any side effects of fullscreen() and does resize(x,y,w,h). *
  void fullscreen_off(int x,int y,int width,int height)
 
    /** Iconifies the window. If you call this when shown() is false it will show() it as
     * an icon. If the window is already iconified this does nothing
     *
     * Call show() to restore the window
     *
     * When a window is iconified/restored (either by these calls or by the user) the
     * handle() method is called with \c FL_HIDE and \c FL_SHOW events and visible() is
     * turned on and off
     *
     * Set the window icon using the icon() call. You should not rely on window managers
     * displaying the icons. *
  void iconize()

  int x_root() const 
  int y_root() const 

    /** Returns the last window that was made current. *
  static Fl_Window *current()
    /** make_current() sets things up so that the drawing functions in <FL/fl_draw.H> will g
     * into this window. This is useful for incremental update of windows, such as in an idl
     * callback, which will make your program behave much better if it draws a slow graphic
     * <b>Danger: incremental update is very hard to debug and maintain!</b
     
     * This method only works for the \c Fl_Window and \c Fl_Gl_Window classes.  *
  void make_current()

    /** Change the cursor for this window. This always calls the system, if you are changing
     * the cursor a lot you may want to keep track of how you set it in a static varaible
     * and call this only if the new cursor is different
     
     * The type Fl_Cursor is an enumeration defined in <Enumerations.H>. (Under X you can ge
     * any \c XC_cursor value by passing Fl_Cursor((XC_foo/2)+1)). The colors only work on X
     * they are not implemented on WIN32 or Mac OS. *
  void cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE)
  void default_cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE)
  static void default_callback(Fl_Window*, void* v)

}

#endi

/
// End of "$Id$"
/
