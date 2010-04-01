//
// "$Id: Fl_Box.H 7365 2010-03-30 15:18:29Z matt $"
//
// Box header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/* \file
   InvisibleBox widget . */

#ifndef Fl_Box_H
#define Fl_Box_H

#ifndef Fl_Widget_H
#include "Widget.h"
#endif

namespace fltk3 {

/**
 This is a box that is invisible due to not having a box. The
 label still prints so it can be used to position labels. Also
 this is useful as a resizable() widget.
 */
class FL_EXPORT InvisibleBox : public Widget {
public:
  InvisibleBox(int X, int Y, int W, int H, const char *l=0)
  : Widget(X,Y,W,H,l) { box(NO_BOX); }
  
  InvisibleBox(Boxtype b, int X, int Y, int W, int H, const char *l)
  : Widget(X,Y,W,H,l) {box(b);}
  
};

}

#endif

//
// End of "$Id: Fl_Box.H 7365 2010-03-30 15:18:29Z matt $".
//
