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
using namespace fltk;

#define MAXBUFFER 0x40000 // 256k

/**
 * draw an image based on the input parameters
 *
 * buf:       image source data
 * r:         x,y is upper left to put picture, w,h gives size of data
 * delta:     distance from pixel to pixel in buf in bytes
 * linedelta: distance from line to line in buf in bytes
 * mono:      if set, pixel is one byte - if zero, pixel is 3 byte
 * cb:        callback to copy image data into (RGB?) buffer
 *   buf:       pointer to first byte in image source
 *   x, y:      position in buffer
 *   w:         width (in bytes?)
 *   dst:       destinaation buffer
 * userdata:  ?
 */
static void innards(const uchar *buf,
		    fltk::PixelType pixeltype,
		    const fltk::Rectangle &r,
		    int delta, int linedelta,
		    DrawImageCallback cb, void* userdata)
{
  int X = r.x(), Y = r.y(), W = r.w(), H = r.h();
  const void *array = buf;
  uchar *tmpBuf = 0;
  if (cb) {
    tmpBuf = new uchar[ H*W*delta ];
    for (int i=0; i<H; i++) {
      cb(userdata, 0, i, W, tmpBuf+i*W*delta);
    }
    array = (void*)tmpBuf;
    linedelta = W;
  }
  // create an image context
  CGColorSpaceRef   lut = CGColorSpaceCreateDeviceRGB();
  CGDataProviderRef src = CGDataProviderCreateWithData( 0L, array, linedelta*H*delta, 0L);
  CGImageRef        img = CGImageCreate( W, H, 8, 8*delta, linedelta*delta,
                            lut, delta&1?kCGImageAlphaNone:kCGImageAlphaNoneSkipLast,
                            src, 0L, false, kCGRenderingIntentDefault);
  // draw the image into the destination context
  if (img) {
    CGRect rect = { X, Y, W, H };
    fltk::begin_quartz_image(rect, Rectangle(0, 0, W, H));
    CGContextDrawImage(quartz_gc, rect, img);
    fltk::end_quartz_image();
    // release all allocated resources
    CGImageRelease(img);
  }
  CGColorSpaceRelease(lut);
  CGDataProviderRelease(src);
  if (cb) {
    delete[] tmpBuf;
  }
  if (img) return; // else fall through to slow mode
  // following the very save (and very slow) way to write the image into the give port
  CGContextSetShouldAntialias(quartz_gc, false);
  if ( cb )
  {
    uchar *tmpBuf = new uchar[ W*4 ];
    for ( int i=0; i<H; i++ )
    {
      uchar *src = tmpBuf;
      cb( userdata, 0, i, W, tmpBuf );
      for ( int j=0; j<W; j++ )
      {
        if ( pixeltype==LUMINANCE )
          { color( src[0], src[0], src[0] ); }
        else
          { color( src[0], src[1], src[2] ); }
        CGContextMoveToPoint(quartz_gc, X+j, Y+i);
        CGContextAddLineToPoint(quartz_gc, X+j, Y+i);
        CGContextStrokePath(quartz_gc);
        src+=delta;
      }
    }
    delete[] tmpBuf;
  }
  else
  {
    for ( int i=0; i<H; i++ )
    {
      const uchar *src = buf+i*linedelta;
      for ( int j=0; j<W; j++ )
      {
        if ( pixeltype==LUMINANCE )
          color( src[0], src[0], src[0] );
        else
          color( src[0], src[1], src[2] );
        CGContextMoveToPoint(quartz_gc, X+j, Y+i);
        CGContextAddLineToPoint(quartz_gc, X+j, Y+i);
        CGContextStrokePath(quartz_gc);
        src += delta;
      }
    }
  }
  CGContextSetShouldAntialias(quartz_gc, true);
}

//
// End of "$Id$".
//
