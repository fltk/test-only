//
// "$Id: HorizontalFillSlider.h,v 1.4 2004/01/06 06:43:02 spitzak Exp $"
//
// Horizontal slider with a solid bar, like a progress report bar
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
#ifndef fltk_HorizontalFillSlider_h
#define fltk_HorizontalFillSlider_h

#include "FillSlider.h"

namespace fltk {

/*! Slider that is filled from the end (like a progress bar) */
class FL_API HorizontalFillSlider : public FillSlider {
public:
    HorizontalFillSlider(int x,int y,int w,int h,const char *l=0)
      : FillSlider(x,y,w,h,l) {type(HORIZONTAL);}
};

}

#endif

//
// End of "$Id: HorizontalFillSlider.h,v 1.4 2004/01/06 06:43:02 spitzak Exp $".
//
