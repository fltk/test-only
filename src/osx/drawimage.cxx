//
// "$Id$"
//
// MacOS image drawing code for the Fast Light Tool Kit (FLTK).
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

////////////////////////////////////////////////////////////////

#include <fltk/events.h>
#include <fltk/draw.h>
#include <fltk/x.h>
#include <fltk/Image.h>
using namespace fltk;

extern fltk::Image* fl_current_Image;
class fltk::DrawImageHelper {
public:
  static void setimage(CGImageRef img, fltk::PixelType pixeltype) {
    // If we are making an Image, we have to remember the CGImage so we
    // can use it to draw the Image later. If there is a way to update
    // an existing CGImage this would be less of a kludge.
    if (fl_current_Image->picture)
      CGImageRelease((CGImageRef)(fl_current_Image->picture));
    fl_current_Image->picture = img;
    fl_current_Image->flags =
      (pixeltype == MONO || pixeltype == RGB || pixeltype == BGR) ?
      (Image::OPAQUE|Image::DRAWN) : Image::DRAWN;
  }
};

#define MAXBUFFER 0x40000 // 256k

static void releaser(void*, const void* data, size_t size) {
  delete[] (U32*)data;
}

static void innards(const uchar *buf,
		    fltk::PixelType pixeltype,
		    const fltk::Rectangle &r1,
		    int delta, int linedelta,
		    DrawImageCallback cb, void* userdata)
{
  fltk::Rectangle r(r1); transform(r);
  fltk::Rectangle cr(r); if (!intersect_with_clip(cr)) return;


  int dx = cr.x()-r.x();
  int dy = cr.y()-r.y();
  int x = cr.x();
  int y = cr.y();
  int w = cr.w();
  int h = cr.h();
  if (buf) buf += dx*delta + dy*linedelta;

  static char flip[12] = {0,0,1,0,0,1,1,0,0,1,1,0};

  const uchar *array = buf;
  U32* tmpBuf = 0;
  if (cb) {
    linedelta = (w*delta+3)&-4;
    int n = (linedelta*h+(r1.w()*delta-linedelta)+3)/4;
    tmpBuf = new U32[n];
    array = (uchar*)tmpBuf;
    for (int i=0; i<h; i++) {
      uchar* dest = (uchar*)array+i*linedelta;
      const uchar* ret = cb(userdata, dx, dy+i, w, dest);
      if (ret != dest) memcpy(dest, ret, w*delta);
    }
  } else if (fl_current_Image || flip[pixeltype]) {
    // We must dup the memory in case the source image is temporary...
    int n = (linedelta*h+3)/4;
    tmpBuf = new U32[n];
    memcpy(tmpBuf, array, 4*n);
    array = (uchar*)tmpBuf;
  }
  if (flip[pixeltype]) {
    uchar* p = (uchar*)array;
    if (pixeltype>RGBA && (pixeltype&1)) p++;
    for (int y=0; y<h; y++) {
      uchar* q = p; p += linedelta;
      uchar* e = q+w*delta;
      while (q < e) {uchar t = q[0]; q[0] = q[2]; q[2] = t; q += delta;}
    }
  }

  // create an image context
  static CGColorSpaceRef rgbcolorspace = 0;
  static CGColorSpaceRef graycolorspace = 0;
  if (!rgbcolorspace) rgbcolorspace = CGColorSpaceCreateDeviceRGB();
  CGColorSpaceRef colorspace = rgbcolorspace;
  CGDataProviderRef src =
    CGDataProviderCreateWithData( 0L, array, linedelta*h,
				  tmpBuf ? releaser : 0);
  CGImageAlphaInfo bitmapInfo = kCGImageAlphaNone;
  switch (pixeltype) {
  case MASK:
    bitmapInfo = kCGImageAlphaOnly;
    break;
  case MONO:
    bitmapInfo = kCGImageAlphaNone;
    if (!graycolorspace) graycolorspace = CGColorSpaceCreateDeviceGray();
    colorspace = graycolorspace;
    break;
  case BGR: bitmapInfo = kCGImageAlphaNone; break;
  case RGB: bitmapInfo = kCGImageAlphaNone; break;
  case RGBA: bitmapInfo = kCGImageAlphaPremultipliedLast; break;
  case ABGR: bitmapInfo = kCGImageAlphaPremultipliedFirst; break;
  case BGRA: bitmapInfo = kCGImageAlphaPremultipliedLast; break;
  case ARGB: bitmapInfo = kCGImageAlphaPremultipliedFirst; break;
  case RGBM: bitmapInfo = kCGImageAlphaLast; break;
  case MBGR: bitmapInfo = kCGImageAlphaFirst; break;
  case BGRM: bitmapInfo = kCGImageAlphaLast; break;
  case MRGB: bitmapInfo = kCGImageAlphaFirst; break;
  }
  CGImageRef img;
  if (pixeltype==MASK)
    img = CGImageMaskCreate( w, h,
			 8, // bitsPerComponent
			 8*delta, // bitsPerPixel
			 linedelta, // bytesPerRow
			 src, // provider
			 0L, // decode array (?)
			 false); // shouldInterpolate
  else
    img = CGImageCreate( w, h,
			 8, // bitsPerComponent
			 8*delta, // bitsPerPixel
			 linedelta, // bytesPerRow
			 colorspace,
			 bitmapInfo, // bitmapInfo
			 src, // provider
			 0L, // decode array (?)
			 false, // shouldInterpolate
			 kCGRenderingIntentDefault);
  // draw the image into the destination context
  if (fl_current_Image) {
    DrawImageHelper::setimage(img, pixeltype);
  } else if (img) {
    CGRect rect = { x, y, w, h };
    fltk::begin_quartz_image(rect, Rectangle(0, 0, w, h));
    CGContextDrawImage(quartz_gc, rect, img);
    fltk::end_quartz_image();
    // release all allocated resources
    CGImageRelease(img);
  } else {
#if 0
    // Otherwise creating the image did not work, just draw all the pixels (!)
    CGContextSetShouldAntialias(quartz_gc, false);
    for ( int i=0; i<h; i++ ) {
      const uchar *src = array+i*linedelta;
      for ( int j=0; j<w; j++ ) {
        if ( pixeltype==LUMINANCE )
          color( src[0], src[0], src[0] );
        else
          color( src[0], src[1], src[2] );
        CGContextMoveToPoint(quartz_gc, x+j, y+i);
        CGContextAddLineToPoint(quartz_gc, x+j, y+i);
        CGContextStrokePath(quartz_gc);
        src += delta;

      }
    }
    CGContextSetShouldAntialias(quartz_gc, true);
#endif
  }
  CGDataProviderRelease(src);
}

void fltk::begin_quartz_image(CGRect &rect, const Rectangle &c) {
  CGContextSaveGState(quartz_gc);
  CGAffineTransform mx = CGContextGetCTM(quartz_gc);
  CGRect r2 = rect;
  r2.origin.x -= 0.5f;
  r2.origin.y -= 0.5f;
  CGContextClipToRect(quartz_gc, r2);
  mx.d = -1.0; mx.tx = -mx.tx;
  CGContextConcatCTM(quartz_gc, mx);
  rect.origin.x = rect.origin.x - c.x();
  rect.origin.y = (mx.ty+0.5f) - rect.origin.y - c.h() + c.y();
  rect.size.width = c.w();
  rect.size.height = c.h();
}

void fltk::end_quartz_image() {
  CGContextRestoreGState(quartz_gc);
}

//
// End of "$Id$".
//
