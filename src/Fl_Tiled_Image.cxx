//
// "$Id: Fl_Tiled_Image.cxx,v 1.10 2004/08/01 22:28:23 spitzak Exp $"
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

/*! \class fltk::TiledImage

  Draws the image supplied to it's constructor repeatedly to cover
  the area provided to draw(). Clips the images as necessary to
  fit them in the rectangle.
*/

#include <fltk/TiledImage.h>
#include <fltk/draw.h>
using namespace fltk;

/*! \fn TiledImage::TiledImage(Symbol*)

  A pointer to the image must be provided to the constructor. If it
  is null then the TiledImage draws nothing.
*/

/*! Returns the maximum of the passed size and the size of the image.
  This makes the alignment code center the tiled image if desired.
*/
void TiledImage::_measure(float& w, float& h) const {
  if (!image_) return;
  float iw = w;
  float ih = h;
  image_->measure(iw,ih);
  if (iw > w) w = iw;
  if (ih > h) h = ih;
}

/*! Repeatedly draws the image to fill the area, putting the top-left
  corner at \a x,y. This checks the current clip region and does
  minimal drawing of only the visible portions of the image.
*/
void TiledImage::_draw(int x, int y, int w, int h, const Style* style, Flags flags) const
{
  if (!image_) return;
  float fW = (float)w;
  float fH = (float)h;
  image_->measure(fW,fH); if (fW <= 0 || fH <= 0) return;
  int iw = int(fW);
  int ih = int(fH);
  int cx = 0;
  int cy = 0;
  // Perhaps this should use the Align flags to set cx, cy.

  // Figure out the smallest rectangle enclosing this and the clip region:
  int X,Y,W,H; clip_box(x,y,w,h, X, Y, W, H);
  if (W <= 0 || H <= 0) return;
  cx += X-int(x); cy += Y-int(y);
  push_clip(X, Y, W, H);

  int temp = -cx % iw;
  cx = (temp>0 ? iw : 0) - temp;
  temp = -cy % ih;
  cy = (temp>0 ? ih : 0) - temp;

  int ccx=cx;
  while (-cy < H) {
    while (-cx < W) {
      image_->draw(X-cx, Y-cy, iw, ih, style, flags);
      cx -= iw;
    }
    cy -= ih;
    cx = ccx;
  }
  pop_clip();
}

const BoxInfo* TiledImage::boxinfo() const {
  if (image_) return image_->boxinfo();
  else return Symbol::boxinfo();
}

//
// End of "$Id: Fl_Tiled_Image.cxx,v 1.10 2004/08/01 22:28:23 spitzak Exp $".
//
