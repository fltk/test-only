//
// "$Id: Fl_Pack.h,v 1.4 2002/10/29 00:37:23 easysw Exp $"
//
// Pack header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Pack_H
#define Fl_Pack_H

#include <fltk/Fl_Group.h>

/**

   This grouping widget arranges and resizes all of it's children so they are 
   all packed close together and completely fill the widget. This is similar 
   to layout widgets used by TK and some other toolkits. 

   \image Fl_Pack.gif

   In the above image the widgets 1,4,7, and 8 have the FL_PACK_VERTICAL 
   bit on in their flags(). The widgets are numbered with the order they
   were added to the pack. 

   Each child widget is stretched (ie one of it's dimensions is changed
   and the other left alone) to completely fill the remaining space on 
   one edge of the pack and placed against all the previous widgets on 
   that edge. 

   Any widget with the FL_PACK_VERTICAL bit (8) set in the flags() are 
   placed against the left or right edge, otherwise against the top or 
   bottom edge. (If type() of the pack is set to Fl_Pack::HORIZONTAL then
   it will "pack horzontally" which means that all widgets are packed 
   against the left or right edges, whether or not they have 
   FL_PACK_VERTICAL turned on) 

   All widgets before the resizable() widget are placed against the
   top and left edge, all widgets after the resizable() widget are 
   placed against the bottom and right edge in backwards order. If 
   the resizable() is not set, all widgets are placed against the 
   top and left edge, and the pack itself resizes to surround them.
   Such a pack must be imbedded in another pack or be the only child
   of a window so that this resizing can be used to layout the parent widget. 

   If any child changes size the pack automatically re-layouts and 
   possibly changes it's own size to accomodate it. If you add, move,
   remove, hide, or show a child widget, or change the FL_PACK_VERTICAL
   flag, you should call relayout() on the pack yourself. 

   The default box is FL_FLAT_BOX. If you set it to something else 
   the border of the box is drawn and the widgets are packed against
   the inside of this border. 

*/
class FL_API Fl_Pack : public Fl_Group {
  int spacing_;
public:
  enum { // values for type(int)
    NORMAL	= GROUP_TYPE,
    VERTICAL	= NORMAL, // for back compatability
    HORIZONTAL	= GROUP_TYPE+1
  };
  void layout();
  /** Creates a new empty Fl_Pack. */
  Fl_Pack(int x,int y,int w ,int h,const char *l = 0);
  /*@{*/
  /**
   Gets or sets the number of extra pixels of blank space 
   that are added between each child widget. 
  */
  int spacing() const {return spacing_;}
  void spacing(int i) {spacing_ = i;}
  /*@}*/
};

#endif

//
// End of "$Id: Fl_Pack.h,v 1.4 2002/10/29 00:37:23 easysw Exp $".
//
