//
// "$Id: Fl_Gl_Window.H,v 1.7.2.5.2.3.2.3 2003/12/02 02:51:44 easysw Exp $"
//
// OpenGL header file for the Fast Light Tool Kit (FLTK).
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
#ifndef Fl_Gl_Window_H
#define Fl_Gl_Window_H

#include "Fl_Window.H"

#ifndef GLContext
typedef void* GLContext; // actually a GLXContext or HGLDC
#endif

class Fl_Gl_Choice; // structure to hold result of glXChooseVisual

/** The Fl_Gl_Window widget sets things up so OpenGL works, and also keeps
 * an OpenGL "context" for that window, so that changes to the lighting and
 * projection may be reused between redraws. Fl_Gl_Window also flushes the
 * OpenGL streams and swaps buffers after draw() returns.
 *
 * OpenGL hardware typically provides some overlay bit planes, which are
 * very useful for drawing UI controls atop your 3D graphics. If the overlay
 * hardware is not provided, FLTK tries to simulate the overlay, This works
 * pretty well if your graphics are double buffered, but not very well for
 * single-buffered. 
 *
 * Use Fl_Gl_Window::draw() to render OpenGL drawings. Fl_Gl_Window::draw()
 * is a pure virtual method. You must subclass Fl_Gl_Window and provide an
 * implementation for draw(). You may also provide an implementation of
 * draw_overlay() if you want to draw into the overlay planes. You can avoid
 * reinitializing the viewport and lights and other things by checking 
 * valid() at the start of draw() and only doing the initialization if it
 * is false.
 *
 * The draw() method can only use OpenGL calls. Do not attempt to call X,
 * any of the functions in <FL/fl_draw.H>, or glX directly. Do not call
 * gl_start() or gl_finish().
 *
 * If double-buffering is enabled in the window, the back and front buffers
 * are swapped after this function is completed.
 */
class FL_EXPORT Fl_Gl_Window : public Fl_Window {

  int mode_;
  const int *alist;
  Fl_Gl_Choice *g;
  GLContext context_;
  char valid_;
  char damage1_; // damage() of back buffer
    /** You must implement this virtual function if you want to draw into
     * the overlay. The overlay is cleared before this is called. You should
     * draw anything that is not clear using OpenGL. You must use
     * gl_color(i) to choose colors (it allocates them from the colormap
     * using system-specific calls), and remember that you are in an
     * indexed OpenGL mode and drawing anything other than flat-shaded will
     * probably not work.
     *
     * Both this function and Fl_Gl_Window::draw() should check
     * Fl_Gl_Window::valid() and set the same transformation. If you don't
     * your code may not work on other systems. Depending on the OS, and on
     * whether overlays are real or simulated, the OpenGL context may be the
     * same or different between the overlay and main window. */
  virtual void draw_overlay();
  void init();

  void *overlay;
  void make_overlay();
  friend class _Fl_Gl_Overlay;

  static int can_do(int, const int *);
  int mode(int, const int *);

public:

  void show();
  void show(int a, char **b) {Fl_Window::show(a,b);}
  void flush();
    /** Hides the window and destroys the OpenGL context. */
  void hide();
  void resize(int,int,int,int);

    /** Fl_Gl_Window::valid() is turned off when FLTK creates a new context
     * for this window or when the window resizes, and is turned on after
     * draw() is called. You can use this inside your draw() method to avoid
     * unneccessarily initializing the OpenGL context. Just do this:
     * \code
void mywindow::draw() {
  if (!valid()) {
    glViewport(0,0,w(),h());
    glFrustum(...);
    glLight(...);
    ...other initialization...
  }
  ... draw your geometry here ...
}
    * \endcode
    * You can turn valid() on by calling valid(1). You should only do this
    * after fixing the transformation inside a draw() or after make_current().
    * This is done automatically after draw() returns. */
  char valid() const {return valid_;}
  void valid(char v) {valid_ = v;}
    /** The invalidate() method turns off valid() and is equivalent to calling value(0). */
  void invalidate();

    /** Returns non-zero if the hardware supports the given or current OpenGL mode. */
  static int can_do(int m) {return can_do(m,0);}
  static int can_do(const int *m) {return can_do(0, m);}
  int can_do() {return can_do(mode_,alist);}
  Fl_Mode mode() const {return (Fl_Mode)mode_;}
    /** Set or change the OpenGL capabilites of the window. The value can
     * be any of the following OR'd together:
     * \arg \c FL_RGB - RGB color (not indexed)
     * \arg \c FL_RGB8 - RGB color with at least 8 bits of each color
     * \arg \c FL_INDEX - Indexed mode
     * \arg \c FL_SINGLE - not double buffered
     * \arg \c FL_DOUBLE - double buffered
     * \arg \c FL_ACCUM - accumulation buffer
     * \arg \c FL_ALPHA - alpha channel in color
     * \arg \c FL_DEPTH - depth buffer
     * \arg \c FL_STENCIL - stencil buffer
     * \arg \c FL_MULTISAMPLE - multisample antialiasing
     *
     * \c FL_RGB and \c FL_SINGLE have a value of zero, so they are "on"
     * unless you give \c FL_INDEX or \c FL_DOUBLE.
     *
     * If the desired combination cannot be done, FLTK will try turning off
     * \c FL_MULTISAMPLE. If this also fails the show() will call
     * Fl::error() and not show the window.
     * 
     * You can change the mode while the window is displayed. This is most
     * useful for turning double-buffering on and off. Under X this will
     * cause the old X window to be destroyed and a new one to be created.
     * If this is a top-level window this will unfortunately also cause the
     * window to blink, raise to the top, and be de-iconized, and the
     * xid() will change, possibly breaking other code. It is best to make
     * the GL window a child of another window if you wish to do this! */
  int mode(int a) {return mode(a,0);}
  int mode(const int *a) {return mode(0, a);}

    /** Return a pointer to the GLContext that this window is using. */
  void* context() const {return context_;}
    /** Set a pointer to the GLContext that this window is using. This is a
     * system-dependent structure, but it is portable to copy the context
     * from one window to another. You can also set it to \c NULL, which
     * will force FLTK to recreate the context the next time make_current()
     * is called, this is useful for getting around bugs in OpenGL implementations. 
     *
     * If destroy_flag is true the context will be destroyed by fltk when
     * the window is destroyed, or when the mode() is changed, or the next
     * time context(x) is called. */
  void context(void*, int destroy_flag = 0);
    /** The make_current() method selects the OpenGL context for the widget.
     * It is called automatically prior to the draw() method being called
     * and can also be used to implement feedback and/or selection within
     * the handle() method. */
  void make_current();
    /** The swap_buffers() method swaps the back and front buffers. It is
     * called automatically after the draw() method is called. */
  void swap_buffers();
    /** Set the projection so 0,0 is in the lower left of the window and
     * each pixel is 1 unit wide/tall. If you are drawing 2D images, your
     * draw() method may want to call this ifvalid() is false. */
  void ortho();

    /** Returns true if the hardware overlay is possible. If this is false,
     * FLTK will try to simulate the overlay, with significant loss of
     * update speed. Calling this will cause FLTK to open the display. */
  int can_do_overlay();
    /** This method causes draw_overlay to be called at a later time.
     * Initially the overlay is clear, if you want the window to display
     * something in the overlay when it first appears, you must call this
     * immediately after you show() your window. */
  void redraw_overlay();
  void hide_overlay();
    /** The make_overlay_current() method selects the OpenGL context for the
     * widget's overlay. It is called automatically prior to the draw_overlay()
     * method being called and can also be used to implement feedback and/or
     * selection within the handle() method. */
  void make_overlay_current();

    /** The destructor removes the widget and destroys the OpenGL context
     * associated with it. */
  ~Fl_Gl_Window();
    /** Creates a new Fl_Gl_Window widget using the given position, size, 
     * and label string. The default boxtype is \c FL_NO_BOX. The default
     * mode is <tt>FL_RGB|FL_DOUBLE|FL_DEPTH</tt>. */
  Fl_Gl_Window(int W, int H, const char *l=0) : Fl_Window(W,H,l) {init();}
  Fl_Gl_Window(int X, int Y, int W, int H, const char *l=0)
    : Fl_Window(X,Y,W,H,l) {init();}
};

#endif

//
// End of "$Id: Fl_Gl_Window.H,v 1.7.2.5.2.3.2.3 2003/12/02 02:51:44 easysw Exp $".
//
