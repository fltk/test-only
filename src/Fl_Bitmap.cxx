//
// "$Id: Fl_Bitmap.cxx,v 1.7 1999/08/16 07:31:12 bill Exp $"
//
// Bitmap drawing routines for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Bitmap.H>

#ifdef WIN32
// replicate XCreateBitmapFromData:
// Written by Matt
ulong fl_create_bitmap(const uchar* bitmap, int w, int h) {
  // this won't work ehen the user changes display mode during run or
  // has two screens with differnet depths
  static uchar hiNibble[16] =
  { 0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
    0x10, 0x90, 0x50, 0xd0, 0x20, 0xb0, 0x70, 0xf0 };
  static uchar loNibble[16] =
  { 0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e,
    0x01, 0x09, 0x05, 0x0d, 0x02, 0x0b, 0x07, 0x0f };
  int np  = GetDeviceCaps(fl_gc, PLANES);//: was always one on sample machines
  int bpp = GetDeviceCaps(fl_gc, BITSPIXEL);//: 1,4,8,16,24,32 and more?
  int Bpr = (bpp*w+7)/8;			//: bytes per row
  int pad = Bpr&1, w1 = (w+7)/8, shr = ((w-1)&7)+1;
  if (bpp==4) shr = (shr+1)/2;
  uchar *newarray = new uchar[(Bpr+pad)*h], *dst = newarray, *src = bitmap;
  for (int i=0; i<h; i++) {
    //: this is slooow, but we do it only once per pixmap
    for (int j=w1; j>0; j--) {
      uchar b = *src++;
      if (bpp==1) {
	*dst++ = ( hiNibble[b&15] ) | ( loNibble[(b>>4)&15] );
      } else if (bpp==4) {
	for (int k=(j==1)?shr:4; k>0; k--) {
	  *dst++ = "\377\360\017\000"[b&3];
	  b = b >> 2;
	}
      } else {
	for (int k=(j==1)?shr:8; k>0; k--) {
	  if (b&1) {
	    *dst++=0;
	    if (bpp>8) *dst++=0;
	    if (bpp>16) *dst++=0;
	    if (bpp>24) *dst++=0;
	  } else {
	    *dst++=0xff;
	    if (bpp>8) *dst++=0xff;
	    if (bpp>16) *dst++=0xff;
	    if (bpp>24) *dst++=0xff;
	  }
	  b = b >> 1;
	}
      }
    }
    dst += pad;
  }
  ulong r = (ulong)CreateBitmap(w, h, np, bpp, newarray);
  delete[] newarray;
  return r;
}
#endif

void Fl_Bitmap::draw(int X, int Y, int W, int H, int cx, int cy)
{
  if (!mask) mask = fl_create_bitmap(array, w, h);
  _draw(X, Y, W, H, cx, cy);
}

//
// End of "$Id: Fl_Bitmap.cxx,v 1.7 1999/08/16 07:31:12 bill Exp $".
//
