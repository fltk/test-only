//
// "$Id: Fl_Bitmap.cxx,v 1.27 2005/01/24 08:07:14 spitzak Exp $"
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

#include <config.h>
#include <fltk/events.h>
#include <fltk/x.h>
#include <fltk/draw.h>
#include <fltk/xbmImage.h>
using namespace fltk;

/*! \class fltk::xbmImage

  Image based on a 1-bit bitmap from memory. This matches the very
  first type of image provided with X10 in 1980 or so, and unfortunately
  the only one that draws with any efficiency even today...

  Every pixel is either black opaque or black transparent, determined
  by the 1 and 0 bits in the provided bitmap. Perhaps I should add a
  single color as another argument? You can also call the draw(x,y)
  method which will use the current color.
*/
void xbmImage::_draw(const Rectangle& r, const Style* style, Flags flags) const
{
  if (!drawn())
    (const_cast<xbmImage*>(this))->set_alpha_bitmap(array,this->w(),this->h());
  Image::_draw(r, style, flags);
}

/** Draw the bitmap filled with the current color.

    Because bitmaps are only 1 channel, it makes sense to draw them
    with a solid and settable color. This function does so. It will
    draw them with the upper-left corner at the given position.

    Note this has the same name as a virtual function on Symbol, but
    it is not an override of that function.
*/
void xbmImage::draw(int x, int y) const {
  if (!drawn())
    (const_cast<xbmImage*>(this))->set_alpha_bitmap(array, w(), h());
  Image::fill(Rectangle(x, y, w(), h()), 0, 0);
}

//
// End of "$Id: Fl_Bitmap.cxx,v 1.27 2005/01/24 08:07:14 spitzak Exp $".
//
