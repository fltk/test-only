//
// "$Id$"
//
// MacOS image drawing code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2006 by Bill Spitzak and others.
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
      (pixeltype == MONO || pixeltype == RGB || pixeltype == RGB32) ?
      (Image::OPAQUE|Image::DRAWN) : Image::DRAWN;
  }
};

#define MAXBUFFER 0x40000 // 256k

static void releaser(void*, const void* data, size_t size) {
  delete[] (U32*)data;
}

extern void fl_quartz_draw_image(CGImageRef img, int w, int h,
				 const fltk::Rectangle& from,
				 const fltk::Rectangle& to);

static void innards(const uchar *buf,
		    fltk::PixelType pixeltype,
		    const fltk::Rectangle &r1,
		    int linedelta,
		    DrawImageCallback cb, void* userdata)
{
  const int w = r1.w();
  const int h = r1.h();
  const int delta = depth(pixeltype);

  const uchar *array = buf;
  U32* tmpBuf = 0;
  if (cb) {
    linedelta = (w*delta+3)&-4;
    int n = (linedelta*h+(w*delta-linedelta)+3)/4;
    tmpBuf = new U32[n];
    array = (uchar*)tmpBuf;
    for (int i=0; i<h; i++) {
      uchar* dest = (uchar*)array+i*linedelta;
      const uchar* ret = cb(userdata, 0, i, w, dest);
      if (ret != dest) memcpy(dest, ret, w*delta);
    }
  } else if (fl_current_Image) {
    // We must dup the memory in case the source image is temporary...
    int n = (linedelta*h+3)/4;
    tmpBuf = new U32[n];
    memcpy(tmpBuf, array, 4*n);
    array = (uchar*)tmpBuf;
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
  case RGBx: bitmapInfo = kCGImageAlphaNone; break;
  case RGB: bitmapInfo = kCGImageAlphaNone; break;  case RGBA: bitmapInfo = kCGImageAlphaPremultipliedLast; break;
#if !(WORDS_BIGENDIAN)
# warning not tested yet:  RGB32/ARGB32 image format on intel osx
#endif
  case RGB32: bitmapInfo = kCGImageAlphaNoneSkipFirst; break;
  case ARGB32: bitmapInfo = kCGImageAlphaPremultipliedFirst; break;
    // These *may* be wrong on little-endian (Intel). Probably Apple will
    // add some way of specifying these patterns on Intel:
  default: break; // please gcc
  }
  CGImageRef img;
  if (pixeltype==MASK)
    img = CGImageMaskCreate( w, h,
			 8, // bitsPerComponent
			 8*delta, // bitsPerPixel
			 linedelta, // bytesPerRow
			 src, // provider
			 0L, // decode array (?)
			 true); // shouldInterpolate
  else
    img = CGImageCreate( w, h,
			 8, // bitsPerComponent
			 8*delta, // bitsPerPixel
			 linedelta, // bytesPerRow
			 colorspace,
			 bitmapInfo, // bitmapInfo
			 src, // provider
			 0L, // decode array (?)
			 true, // shouldInterpolate
			 kCGRenderingIntentDefault);
  // draw the image into the destination context
  if (fl_current_Image) {
    DrawImageHelper::setimage(img, pixeltype);
  } else if (img) {
    fl_quartz_draw_image(img, w, h, fltk::Rectangle(w,h), r1);
    CGImageRelease(img);
  }
  CGDataProviderRelease(src);
}

extern void fl_set_quartz_ctm();

void fl_quartz_draw_image(CGImageRef img, int w, int h,
			  const fltk::Rectangle& from,
			  const fltk::Rectangle& to) {
  CGContextSaveGState(quartz_gc);
  fl_set_quartz_ctm();
  if (!from.x() && !from.y() && from.w()==w && from.h()==h) {
    CGRect rect = {{to.x(), -to.y()}, {to.w(), -to.h()}};
    CGContextDrawImage(quartz_gc, rect, img);
  } else {
#if 0 // does not work before Tiger!!!
    CGRect irect = {from.x(), from.y(), from.w(), from.h()};
    CGImageRef clip = CGImageCreateWithImageInRect(img, irect);
    CGRect rect = {to.x(), -to.y(), to.w(), -to.h()};
    CGContextDrawImage(quartz_gc, rect, clip);
    CGImageRelease(clip);
#endif
  }
  CGContextRestoreGState(quartz_gc);
}

//
// End of "$Id$".
//
