//
// "$Id: Fl_Bitmap.cxx,v 1.25 2004/03/25 18:13:18 spitzak Exp $"
//
// Bitmap drawing routines for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#include <fltk/events.h>
#include <fltk/x.h>
#include <fltk/draw.h>
#include <fltk/xbmImage.h>
using namespace fltk;

/** Set the cached image to have a 1-bit alpha mask.

    Subclasses can call this inside their _draw() method to set or
    replace the alpha of the image with a 1-bit mask. This is useful
    for image types that have only on/off transparency. The code to
    do so would look something like this:

    \code
    MyImage::draw(x,y,w,h,style,flags) {
      if (!mask) {
        uchar* data = generate_ae_bitmap();
	(const_cast<Image*>(this))->create_bitmap_mask(data,width,height);
	free[] data;
      }
      draw_cache(x,y,w,h,style,flags);
    }
    \endcode

    Each bit of the data is a pixel of alpha, where 1 indicates
    opaque and 0 indicates clear. Each byte supplies 8 bits, the
    high bit being the left-most one. Rows are padded out to the
    next multiple of 8, so the left-most column of every row is
    the high bit of the mask.
*/
void Image::create_bitmap_mask(const uchar* bitmap, int w, int h) {
  w_ = w;
  h_ = h;
#ifdef _WIN32
  // this won't work when the user changes display mode during run or
  // has two screens with differnet depths
  static uchar hiNibble[16] =
  { 0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
    0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0 };
  static uchar loNibble[16] =
  { 0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e,
    0x01, 0x09, 0x05, 0x0d, 0x03, 0x0b, 0x07, 0x0f };
  int Bpr = (w+7)/8;			//: bytes per row
  int pad = Bpr&1, w1 = (w+7)/8, shr = ((w-1)&7)+1;
  uchar *newarray = new uchar[(Bpr+pad)*h], *dst = newarray;
  const uchar* src = bitmap;
  for (int i=0; i<h; i++) {
    //: this is slooow, but we do it only once per pixmap
    for (int j=w1; j>0; j--) {
      uchar b = *src++;
      *dst++ = ( hiNibble[b&15] ) | ( loNibble[(b>>4)&15] );
    }
    if (pad)
      *dst++ = 0;
  }
  mask = (void*)CreateBitmap(w, h, 1, 1, newarray);
  delete[] newarray;
#elif (defined(__APPLE__) && !USE_X11)
  // nyi this is expected to make a GWorld object...
#else
  mask = (void*)XCreateBitmapFromData(xdisplay, xwindow, (char*)bitmap, (w+7)&-8, h);
#endif
}

void xbmImage::_draw(int x, int y, int w, int h, const Style* style, Flags flags) const
{
  if (!mask)	
    (const_cast<xbmImage*>(this))->create_bitmap_mask(array, w_, h_);
  draw_cache(x, y, w, h, style, flags);
}

/** Draw the bitmap filled with the current color.

    Because bitmaps are only 1 channel, it makes sense to draw them
    with a solid and settable color. This function does so. It will
    draw them with the upper-left corner at the given position.

    Note this has the same name as a virtual function on Symbol, but
    it is not an override of that function.
*/
void xbmImage::draw(int x, int y) const {
  if (!mask)	
    (const_cast<xbmImage*>(this))->create_bitmap_mask(array, w_, h_);
  transform(x,y);
  Image::fill(x, y, w_, h_, 0, 0);
}

//
// End of "$Id: Fl_Bitmap.cxx,v 1.25 2004/03/25 18:13:18 spitzak Exp $".
//
