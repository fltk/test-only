/
// "$Id$
/
// Pack header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Pack_
#define Fl_Pack_

#include <FL/Fl_Group.H

/** This widget was designed to add the functionality of compressing an
 * aligning widgets
 
 * If type() is \c FL_HORIZONTAL all the children are resized to the heigh
 * of the Fl_Pack, and are moved next to each other horizontally. If type(
 * is not \c FL_HORIZONTAL then the children are resized to the width an
 * are stacked below each other. Then the Fl_Pack resizes itself to surroun
 * the child widgets
 
 * This widget is needed for the Fl_Tabs. In addition you may want to pu
 * the Fl_Packinside an Fl_Scroll. *
class FL_EXPORT Fl_Pack : public Fl_Group 
  int spacing_
public
  enum { // values for type(int
    VERTICAL = 0
    HORIZONTAL = 
  }
  void draw()
    /** Creates a new Fl_Pack widget using the given position, size, an
     * label string. The default boxtype is \c FL_NO_BOX. *
  Fl_Pack(int x,int y,int w ,int h,const char *l = 0)
    /** Gets the number of extra pixels of blank space that are adde
     * between the children. *
  int spacing() const {return spacing_;
    /** Sets the number of extra pixels of blank space that are adde
     *  between the children. *
  void spacing(int i) {spacing_ = i;
  uchar horizontal() const {return type();
}

#endi

/
// End of "$Id$"
/
