//
// "$Id$"
//
// Pack header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Pack_H
#define Fl_Pack_H

#include <FL/Fl_Group.H>

/** This widget was designed to add the functionality of compressing and
 * aligning widgets.
 *
 * If type() is \c FL_HORIZONTAL all the children are resized to the height
 * of the Fl_Pack, and are moved next to each other horizontally. If type()
 * is not \c FL_HORIZONTAL then the children are resized to the width and
 * are stacked below each other. Then the Fl_Pack resizes itself to surround
 * the child widgets.
 *
 * This widget is needed for the Fl_Tabs. In addition you may want to put
 * the Fl_Packinside an Fl_Scroll. */
class FL_EXPORT Fl_Pack : public Fl_Group {
  int spacing_;
public:
  enum { // values for type(int)
    VERTICAL = 0,
    HORIZONTAL = 1
  };
  void draw();
    /** Creates a new Fl_Pack widget using the given position, size, and
     * label string. The default boxtype is \c FL_NO_BOX. */
  Fl_Pack(int x,int y,int w ,int h,const char *l = 0);
    /** Gets the number of extra pixels of blank space that are added
     * between the children. */
  int spacing() const {return spacing_;}
    /** Sets the number of extra pixels of blank space that are added
     *  between the children. */
  void spacing(int i) {spacing_ = i;}
  uchar horizontal() const {return type();}
};

#endif

//
// End of "$Id$".
//
