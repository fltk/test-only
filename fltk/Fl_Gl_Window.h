//
// "$Id: Fl_Gl_Window.h,v 1.2 2002/10/29 00:37:23 easysw Exp $"
//
// OpenGL header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2000 by Bill Spitzak and others.
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
#ifndef Fl_Gl_Window_H
#define Fl_Gl_Window_H

#include "Fl_Window.h"

#ifndef GLContext
typedef void* GLContext; // actually a GLXContext or HGLDC
#endif

class Fl_Gl_Choice; // structure to hold result of glXChooseVisual

enum {
  FL_NO_AUTO_SWAP = 1024,
  FL_NO_ERASE_OVERLAY = 2048
};

/**
   The Fl_Gl_Window widget sets things up so OpenGL works, and also keeps an 
   OpenGL "context" for that window, so that changes to the lighting and 
   projection may be reused between redraws. Fl_Gl_Window also flushes the 
   OpenGL streams and swaps buffers after draw() returns. 

   OpenGL hardware typically provides some overlay bit planes, which are very
   useful for drawing UI controls atop your 3D graphics. If the overlay
   hardware is not provided, FLTK tries to simulate the overlay, This works
   pretty well if your graphics are double buffered, but not very well for 
   single-buffered. 
*/
class FL_API Fl_Gl_Window : public Fl_Window {

public:

  void create();
  void flush();
  void destroy();
  void layout();

  /**
     Fl_Gl_Window::valid() is turned off when FLTK creates a new context for 
     this window or when the window resizes, and is turned on after draw() 
     is called. You can use this inside your draw() method to avoid 
     unneccessarily initializing the OpenGL context. Just do this:
     
     \code
     void mywindow::draw() {
       if (!valid()) {
         glViewport(0,0,w(),h());
         glFrustum(...);
         glLight(...);
         ...other initialization...
       }
       ... draw your geometry here ...
     }
     \endcode

     You can turn valid() on by calling valid(1). You should only do this 
     after fixing the transformation inside a draw() or after make_current(). 
     This is done automatically after draw() returns. 
  */
  char valid() const {return valid_;}
  void valid(char i) {valid_ = i;}
  /**
   The invalidate() method turns off valid() and is equivalent to 
   calling value(0). 
  */
  void invalidate();

  /*@}*/
  /**
   Set or change the OpenGL capabilites of the window. 
   The value can be any of the following OR'd together: 

   - FL_RGB - RGB color (not indexed) 
   - FL_RGB8 - RGB color with at least 8 bits of each color 
   - FL_INDEX - Indexed mode 
   - FL_SINGLE - not double buffered 
   - FL_DOUBLE - double buffered 
   - FL_ACCUM - accumulation buffer 
   - FL_ALPHA - alpha channel in color 
   - FL_DEPTH - depth buffer 
   - FL_STENCIL - stencil buffer 
   - FL_MULTISAMPLE - multisample antialiasing 
   - FL_STEREO - stereo (if hardware supports it) 
   - FL_NO_AUTO_SWAP - 
      disable the automatic call to swap_buffers() after draw() is called. 
   - FL_NO_ERASE_OVERLAY - 
      disable the automatic call to glClear before draw_overlay() is called. 

   FL_RGB and FL_SINGLE have a value of zero, so they are "on" unless you 
   give FL_INDEX or FL_DOUBLE. 
   If the desired combination cannot be done, FLTK will try turning off 
   FL_MULTISAMPLE. If this also fails the show() will call Fl::error() 
   and not show the window. 

   You can change the mode while the window is displayed. This is most 
   useful for turning double-buffering on and off. Under X this will cause
   the old X window to be destroyed and a new one to be created. If this is
   a top-level window this will unfortunately also cause the window to blink,
   raise to the top, and be de-iconized, and the xid() will change, possibly
   breaking other code. It is best to make the GL window a child of another 
   window if you wish to do this! 
  */
  int mode() const {return mode_;}
  bool mode(int a);
  /*@}*/
  /*@{*/
  /**
   Returns non-zero if the hardware supports the given or current OpenGL mode.
  */
  static bool can_do(int);
  bool can_do() {return can_do(mode_);}
  /*@}*/

  /*@{*/
  /**
   Return or set a pointer to the GLContext that this window is using. 
   This is a system-dependent structure, but it is portable to copy the 
   context from one window to another. You can also set it to NULL, which 
   will force FLTK to recreate the context the next time make_current() is 
   called, this is useful for getting around bugs in OpenGL implementations. 

   If destroy_flag is true the context will be destroyed by fltk when the 
   window is destroyed, or when the mode() is changed, or the next time 
   context(x) is called. 
  */
  void* context() const {return context_;}
  void context(void*, bool destroy_flag = false);
  /*@}*/

  /**
   The make_current() method selects the OpenGL context for the widget. 
   It is called automatically prior to the draw() method being called and
   can also be used to implement feedback and/or selection within the 
   handle() method. 
  */
  void make_current();
  /**
   The swap_buffers() method swaps the back and front buffers. It is called 
   automatically after the draw() method is called. 
  */
  void swap_buffers();

  /**
   Set the projection so 0,0 is in the lower left of the window and each 
   pixel is 1 unit wide/tall. If you are drawing 2D images, your draw()
   method may want to call this if valid() is false. 
  */
  void ortho();

  /**
   Returns true if the hardware overlay is possible. If this is false,
   FLTK will try to simulate the overlay, with significant loss of update 
   speed. Calling this will cause FLTK to open the display. 
  */
  bool can_do_overlay();
  /**
   This method causes draw_overlay to be called at a later time. 
   Initially the overlay is clear, if you want the window to display 
   something in the overlay when it first appears, you must call this 
   immediately after you show() your window. 
  */
  void redraw_overlay();
  void hide_overlay();
  /**
   The make_overlay_current() method selects the OpenGL context for the 
   widget's overlay. It is called automatically prior to the draw_overlay()
   method being called and can also be used to implement feedback and/or 
   selection within the handle() method. 
  */
  void make_overlay_current();

  /**
   The destructor removes the widget and destroys the OpenGL context
   associated with it. 
  */
  ~Fl_Gl_Window();
  /*@{*/
  /**
   Creates a new Fl_Gl_Window widget using the given position, size, and label
   string. The default boxtype is FL_NO_BOX. 
   The default mode is FL_RGB|FL_DOUBLE|FL_DEPTH. 
  */
  Fl_Gl_Window(int W, int H, const char *l=0) : Fl_Window(W,H,l) {init();}
  Fl_Gl_Window(int X, int Y, int W, int H, const char *l=0)
    : Fl_Window(X,Y,W,H,l) {init();}
  /*@}*/
private:

  int mode_;
  Fl_Gl_Choice *gl_choice;
  GLContext context_;
  char valid_;
  char damage1_; // damage() of back buffer

  /**
   You must implement this virtual function if you want to draw into the
   overlay. The overlay is cleared before this is called. You should draw 
   anything that is not clear using OpenGL. You must use gl_color(i) to 
   choose colors (it allocates them from the colormap using 
   system-specific calls), and remember that you are in an indexed OpenGL
   mode and drawing anything other than flat-shaded will probably not work. 
   
   Both this function and Fl_Gl_Window::draw() should check Fl_Gl_Window::valid()
   and set the same transformation. If you don't your code may not work on other
   systems. Depending on the OS, and on whether overlays are real or simulated, 
   the OpenGL context may be the same or different between the overlay and main
   window. 
  */
  virtual void draw_overlay();
  void init();

  void *overlay;
  void make_overlay();
  friend class _Fl_Gl_Overlay;

  void draw_swap();
};

#endif

//
// End of "$Id: Fl_Gl_Window.h,v 1.2 2002/10/29 00:37:23 easysw Exp $".
//
