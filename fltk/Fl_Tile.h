//
// "$Id: Fl_Tile.h,v 1.2 2002/10/29 00:37:23 easysw Exp $"
//
// Tile header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Tile_H
#define Fl_Tile_H

#include "Fl_Group.h"

/**

   The Fl_Tile class lets you resize the children by dragging the border
   between them: 

   \image Fl_Tile.gif

   Fl_Tile allows objects to be resized to zero dimensions. To prevent
   this you can use the resizable() to limit where corners can be dragged to. 

   Even though objects can be resized to zero sizes, they must initially
   have non-zero sizes so the Fl_Tile can figure out their layout. If desired,
   call position() after creating the children but before displaying the window
   to set the borders where you want. 

   The "borders" are part of the children, an Fl_Tile does not draw any 
   graphics of it's own. In the above example all the final children have 
   FL_DOWN_BOX types, and the "ridges" you see are two adjacent FL_DOWN_BOX's
   drawn next to each other. 

*/
class FL_API Fl_Tile : public Fl_Group {
protected:
  void layout();
public:
  int handle(int);
  /**
   Creates a new Fl_Tile widget using the given position, size, and label string.
   The default boxtype is FL_NO_BOX.
  */
  Fl_Tile(int X,int Y,int W,int H,const char*l=0) : Fl_Group(X,Y,W,H,l) {}
  /**
   Drag the intersection at from_x,from_y to to_x,to_y. 
   This redraws all the necessary children.
  */
  void position(int, int, int, int);
};

#endif

//
// End of "$Id: Fl_Tile.h,v 1.2 2002/10/29 00:37:23 easysw Exp $".
//
