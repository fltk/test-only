/
// "$Id$
/
// Double-buffered window header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Double_Window_
#define Fl_Double_Window_

#include "Fl_Window.H

/** The Fl_Double_Window class provides a double-buffered window. I
 * possible this will use the X double buffering extension (Xdbe). If not
 * it will draw the window data into an off-screen pixmap, and then copy i
 * to the on-screen window
 
 * It is highly recommended that you put the following code before the firs
 * show() of anywindow in your program
 
 * <tt>Fl::visual(FL_DOUBLE|FL_INDEX)</tt
 
 * This makes sure you can use Xdbe on servers where double buffering doe
 * not exist for every visual
 *
class FL_EXPORT Fl_Double_Window : public Fl_Window 
protected
  void flush(int eraseoverlay)
  char force_doublebuffering_; // force db, even if the OS already buffers windows (overlays need that on MacOS and Windows2000
public
  void show()
  void show(int a, char **b) {Fl_Window::show(a,b);
  void flush()
  void resize(int,int,int,int)
  void hide()
  ~Fl_Double_Window()
  Fl_Double_Window(int W, int H, const char *l = 0)
    : Fl_Window(W,H,l), force_doublebuffering_(0) { type(FL_DOUBLE_WINDOW); 
    /** Creates a new Fl_Double_Window widget using the given position
     * size, and label (title) string. *
  Fl_Double_Window(int X, int Y, int W, int H, const char *l = 0
    : Fl_Window(X,Y,W,H,l), force_doublebuffering_(0) { type(FL_DOUBLE_WINDOW); 
}

#endi

/
// End of "$Id$"
/
