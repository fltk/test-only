//
// "$Id: Fl_Bitmap.cxx,v 1.18 2002/12/10 01:46:16 easysw Exp $"
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <fltk/events.h>
#include <fltk/x.h>
#include <fltk/draw.h>
#include <fltk/xbmImage.h>
using namespace fltk;

#ifdef _WIN32
// replicate XCreateBitmapFromData:
// Written by Matt
// Updated by Mike to just make a simple 1-bit deep bitmap; I don't think it
// needs to do the other stuff since 1-bit bitmaps are used for text, too...
Pixmap fltk::create_bitmap(const uchar* bitmap, int w, int h) {
  // this won't work ehen the user changes display mode during run or
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
  Pixmap r = CreateBitmap(w, h, 1, 1, newarray);
  delete[] newarray;
  return r;
}
#endif

void xbmImage::draw(int x, int y, int, int, Flags flags)
{
  if (!mask) mask = (void*)create_bitmap(array, w, h);
  _draw(x, y, flags);
}

//
// End of "$Id: Fl_Bitmap.cxx,v 1.18 2002/12/10 01:46:16 easysw Exp $".
//
