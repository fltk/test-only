//
// "$Id: Fl_Tile.H,v 1.6.2.3.2.2.2.4 2004/03/18 08:00:35 matthiaswm Exp $"
//
// Tile header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#ifndef Fl_Tile_H
#define Fl_Tile_H

#include "Fl_Group.H"

/** The Fl_Tile class lets you resize the children by dragging the border
 * between them: 
 *   \image html Fl_Tile.gif
 * Fl_Tile allows objects to be resized to zero dimensions. To prevent this
 * you can use the resizable() to limit where corners can be dragged to.
 *
 * Even though objects can be resized to zero sizes, they must initially 
 * have non-zero sizes so the Fl_Tile can figure out their layout. If
 * desired, call position() after creating the children but before
 * displaying the window to set the borders where you want.
 *
 * The "borders" are part of the children - Fl_Tile does not draw any
 *  graphics of its own. In the example above, all of the children have
 * \c FL_DOWN_BOX types, and the "ridges" you see are actually two adjacent
 * FL_DOWN_BOX's drawn next to each other. All neighboring widgets share
 * the same edge - the widget's thick borders make it appear as though the
 * widgets aren't actually touching, but they are. If the edges of adjacent
 * widgets do not touch, then it will be impossible to drag the
 * corresponding edges.
 *
 * The "resizable" child widget (which should be invisible) limits where 
 * the border can be dragged to. If you don't set it, it will be possible
 * to drag the borders right to the edge, and thus resize objects on the
 * edge to zero width or height. The resizable() widget is not resized by
 * dragging any borders.
 */
class FL_EXPORT Fl_Tile : public Fl_Group {
public:
  int handle(int);
    /** Creates a new Fl_Tile widget using the given position, size, and
     * label string. The default boxtype is \c FL_NO_BOX. */
  Fl_Tile(int X,int Y,int W,int H,const char*l=0) : Fl_Group(X,Y,W,H,l) {}
  void resize(int x, int y, int width, int height);
    /** Drag the intersection at from_x,from_y to to_x,to_y. This redraws
     * all the necessary children. */
  void position(int from_x, int from_y, int to_x, int to_y);
};

#endif

//
// End of "$Id: Fl_Tile.H,v 1.6.2.3.2.2.2.4 2004/03/18 08:00:35 matthiaswm Exp $".
//
