/
// "$Id$
/
// OpenGL header file for the Fast Light Tool Kit (FLTK)
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
#ifndef Fl_Gl_Window_
#define Fl_Gl_Window_

#include "Fl_Window.H

#ifndef GLContex
typedef void* GLContext; // actually a GLXContext or HGLD
#endi

class Fl_Gl_Choice; // structure to hold result of glXChooseVisua

/** The Fl_Gl_Window widget sets things up so OpenGL works, and also keep
 * an OpenGL "context" for that window, so that changes to the lighting an
 * projection may be reused between redraws. Fl_Gl_Window also flushes th
 * OpenGL streams and swaps buffers after draw() returns
 
 * OpenGL hardware typically provides some overlay bit planes, which ar
 * very useful for drawing UI controls atop your 3D graphics. If the overla
 * hardware is not provided, FLTK tries to simulate the overlay, This work
 * pretty well if your graphics are double buffered, but not very well fo
 * single-buffered.
 
 * Use Fl_Gl_Window::draw() to render OpenGL drawings. Fl_Gl_Window::draw(
 * is a pure virtual method. You must subclass Fl_Gl_Window and provide a
 * implementation for draw(). You may also provide an implementation o
 * draw_overlay() if you want to draw into the overlay planes. You can avoi
 * reinitializing the viewport and lights and other things by checking
 * valid() at the start of draw() and only doing the initialization if i
 * is false
 
 * The draw() method can only use OpenGL calls. Do not attempt to call X
 * any of the functions in <FL/fl_draw.H>, or glX directly. Do not cal
 * gl_start() or gl_finish()
 
 * If double-buffering is enabled in the window, the back and front buffer
 * are swapped after this function is completed
 *
class FL_EXPORT Fl_Gl_Window : public Fl_Window 

  int mode_
  const int *alist
  Fl_Gl_Choice *g
  GLContext context_
  char valid_
  char damage1_; // damage() of back buffe
    /** You must implement this virtual function if you want to draw int
     * the overlay. The overlay is cleared before this is called. You shoul
     * draw anything that is not clear using OpenGL. You must us
     * gl_color(i) to choose colors (it allocates them from the colorma
     * using system-specific calls), and remember that you are in a
     * indexed OpenGL mode and drawing anything other than flat-shaded wil
     * probably not work
     
     * Both this function and Fl_Gl_Window::draw() should chec
     * Fl_Gl_Window::valid() and set the same transformation. If you don'
     * your code may not work on other systems. Depending on the OS, and o
     * whether overlays are real or simulated, the OpenGL context may be th
     * same or different between the overlay and main window. *
  virtual void draw_overlay()
  void init()

  void *overlay
  void make_overlay()
  friend class _Fl_Gl_Overlay

  static int can_do(int, const int *)
  int mode(int, const int *)

public

  void show()
  void show(int a, char **b) {Fl_Window::show(a,b);
  void flush()
    /** Hides the window and destroys the OpenGL context. *
  void hide()
  void resize(int,int,int,int)

    /** Fl_Gl_Window::valid() is turned off when FLTK creates a new contex
     * for this window or when the window resizes, and is turned on afte
     * draw() is called. You can use this inside your draw() method to avoi
     * unneccessarily initializing the OpenGL context. Just do this
     * \cod
void mywindow::draw() 
  if (!valid()) 
    glViewport(0,0,w(),h())
    glFrustum(...)
    glLight(...)
    ...other initialization..
  
  ... draw your geometry here ..

    * \endcod
    * You can turn valid() on by calling valid(1). You should only do thi
    * after fixing the transformation inside a draw() or after make_current()
    * This is done automatically after draw() returns. *
  char valid() const {return valid_;
  void valid(char v) {valid_ = v;
    /** The invalidate() method turns off valid() and is equivalent to calling value(0). *
  void invalidate()

    /** Returns non-zero if the hardware supports the given or current OpenGL mode. *
  static int can_do(int m) {return can_do(m,0);
  static int can_do(const int *m) {return can_do(0, m);
  int can_do() {return can_do(mode_,alist);
  Fl_Mode mode() const {return (Fl_Mode)mode_;
    /** Set or change the OpenGL capabilites of the window. The value ca
     * be any of the following OR'd together
     * \arg \c FL_RGB - RGB color (not indexed
     * \arg \c FL_RGB8 - RGB color with at least 8 bits of each colo
     * \arg \c FL_INDEX - Indexed mod
     * \arg \c FL_SINGLE - not double buffere
     * \arg \c FL_DOUBLE - double buffere
     * \arg \c FL_ACCUM - accumulation buffe
     * \arg \c FL_ALPHA - alpha channel in colo
     * \arg \c FL_DEPTH - depth buffe
     * \arg \c FL_STENCIL - stencil buffe
     * \arg \c FL_MULTISAMPLE - multisample antialiasin
     
     * \c FL_RGB and \c FL_SINGLE have a value of zero, so they are "on
     * unless you give \c FL_INDEX or \c FL_DOUBLE
     
     * If the desired combination cannot be done, FLTK will try turning of
     * \c FL_MULTISAMPLE. If this also fails the show() will cal
     * Fl::error() and not show the window
     *
     * You can change the mode while the window is displayed. This is mos
     * useful for turning double-buffering on and off. Under X this wil
     * cause the old X window to be destroyed and a new one to be created
     * If this is a top-level window this will unfortunately also cause th
     * window to blink, raise to the top, and be de-iconized, and th
     * xid() will change, possibly breaking other code. It is best to mak
     * the GL window a child of another window if you wish to do this! *
  int mode(int a) {return mode(a,0);
  int mode(const int *a) {return mode(0, a);

    /** Return a pointer to the GLContext that this window is using. *
  void* context() const {return context_;
    /** Set a pointer to the GLContext that this window is using. This is 
     * system-dependent structure, but it is portable to copy the contex
     * from one window to another. You can also set it to \c NULL, whic
     * will force FLTK to recreate the context the next time make_current(
     * is called, this is useful for getting around bugs in OpenGL implementations.
     
     * If destroy_flag is true the context will be destroyed by fltk whe
     * the window is destroyed, or when the mode() is changed, or the nex
     * time context(x) is called. *
  void context(void*, int destroy_flag = 0)
    /** The make_current() method selects the OpenGL context for the widget
     * It is called automatically prior to the draw() method being calle
     * and can also be used to implement feedback and/or selection withi
     * the handle() method. *
  void make_current()
    /** The swap_buffers() method swaps the back and front buffers. It i
     * called automatically after the draw() method is called. *
  void swap_buffers()
    /** Set the projection so 0,0 is in the lower left of the window an
     * each pixel is 1 unit wide/tall. If you are drawing 2D images, you
     * draw() method may want to call this ifvalid() is false. *
  void ortho()

    /** Returns true if the hardware overlay is possible. If this is false
     * FLTK will try to simulate the overlay, with significant loss o
     * update speed. Calling this will cause FLTK to open the display. *
  int can_do_overlay()
    /** This method causes draw_overlay to be called at a later time
     * Initially the overlay is clear, if you want the window to displa
     * something in the overlay when it first appears, you must call thi
     * immediately after you show() your window. *
  void redraw_overlay()
  void hide_overlay()
    /** The make_overlay_current() method selects the OpenGL context for th
     * widget's overlay. It is called automatically prior to the draw_overlay(
     * method being called and can also be used to implement feedback and/o
     * selection within the handle() method. *
  void make_overlay_current()

    /** The destructor removes the widget and destroys the OpenGL contex
     * associated with it. *
  ~Fl_Gl_Window()
    /** Creates a new Fl_Gl_Window widget using the given position, size,
     * and label string. The default boxtype is \c FL_NO_BOX. The defaul
     * mode is <tt>FL_RGB|FL_DOUBLE|FL_DEPTH</tt>. *
  Fl_Gl_Window(int W, int H, const char *l=0) : Fl_Window(W,H,l) {init();
  Fl_Gl_Window(int X, int Y, int W, int H, const char *l=0
    : Fl_Window(X,Y,W,H,l) {init();
}

#endi

/
// End of "$Id$"
/
