/
// "$Id$
/
// Single-buffered window header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Single_Window_
#define Fl_Single_Window_

#include "Fl_Window.H

/** This is the same as Fl_Window. However, it is possible that som
 * implementations will provide double-buffered windows by default. Thi
 * subclass can be used to force single-buffering. This may be useful fo
 * modifying existing programs that use incremental update, or for som
 * types of image data, such as a movie flipbook
 
 * On Apple's OSX and Microsoft's Windows XP, all windows are double buffer
 * by the system and cannot be forced to be single buffered. *
class FL_EXPORT Fl_Single_Window : public Fl_Window 
public
  void show()
  void show(int a, char **b) {Fl_Window::show(a,b);
  void flush()
  Fl_Single_Window(int W, int H, const char *l=0
    : Fl_Window(W,H,l) {
    /** Creates a new Fl_Single_Window widget using the given position, size
     * and label (title) string. *
  Fl_Single_Window(int X, int Y, int W, int H, const char *l=0
    : Fl_Window(X,Y,W,H,l) {
  int make_current()
}

#endi

/
// End of "$Id$"
/
