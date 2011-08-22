//
// "$Id: PackedGroup.h 7903 2010-11-28 21:06:39Z matt $"
//
// Pack header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
 fltk3::PackedGroup widget . */

#ifndef Fltk3_Pack_H
#define Fltk3_Pack_H

#include <fltk3/Group.h>


class Fl_Pack;


namespace fltk3 {
  
  /**
   This widget was designed to add the functionality of compressing and
   aligning widgets.
   <P>If type() is fltk3::PackedGroup::HORIZONTAL all the children are
   resized to the height of the fltk3::PackedGroup, and are moved next to
   each other horizontally. If type() is not fltk3::PackedGroup::HORIZONTAL
   then the children are resized to the width and are stacked below each
   other.  Then the fltk3::PackedGroup resizes itself to surround the child
   widgets.
   <P>This widget is needed for the fltk3::TabGroup.
   In addition you may want to put the fltk3::PackedGroup inside an 
   fltk3::ScrollGroup.
   
   <P>The resizable for fltk3::PackedGroup is set to NULL by default.</p>
   <P>See also: fltk3::Group::resizable()
   */
  class FLTK3_EXPORT PackedGroup : public fltk3::Group {

    friend class ::Fl_Pack;

    int spacing_;
    
  public:
    enum { // values for type(int)
      VERTICAL = 0,
      HORIZONTAL = 1
    };
    
  protected:
    void draw();
    
  public:
    PackedGroup(int x,int y,int w ,int h,const char *l = 0);
    /**
     Gets the number of extra pixels of blank space that are added
     between the children.
     */
    int spacing() const {return spacing_;}
    /**
     Sets the number of extra pixels of blank space that are added
     between the children.
     */
    void spacing(int i) {spacing_ = i;}
    /** Same as fltk3::Group::type() */
    uchar horizontal() const {return type();}
    /** Recalculate the size of this widget and reposition all children. */
    void layout();
  };
  
}

#endif

//
// End of "$Id: PackedGroup.h 7903 2010-11-28 21:06:39Z matt $".
//
