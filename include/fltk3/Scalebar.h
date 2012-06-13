//
// "$Id$"
//
// Scalebar header file for FLTK.
//
// Copyright 2002-2012 by Matthias Melcher.
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
 fltk3::Scalebar widget . */

#ifndef Fltk3_Scalebar_H
#define Fltk3_Scalebar_H

#include "Scrollbar.h"

namespace fltk3 {

/** 
 * A Scalebar is a Scrollbar that can also scale the content window.
 *
 * This slider has two additional boxes which can be dragged to change
 * the amount of data shown from within a document. This widget is
 * great for vector data like time lines and curve editors.
 */
class FLTK3_EXPORT Scalebar : public Scrollbar 
{
  short pushed_;
  short slider_size_i_;
  short min_sash_size_;
  
private:
  void drawSlider( int x, int y, int w, int h );
  void drawSliderBg( int x, int y, int w, int h );
  
protected:
  void draw();
  int handle(int);
  
public:  
  Scalebar(int x,int y,int w,int h, const char *l = 0);
  int value();
  int value(int position, int size, int top, int total);
  int scrollvalue(int position, int size, int top, int total);
  void slider_size_i( int v );
  int slider_size_i();
};

}

#endif

//
// End of "$Id$".
//
