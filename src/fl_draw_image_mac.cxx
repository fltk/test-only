//
// "$Id: fl_draw_image_mac.cxx,v 1.12 2005/01/25 17:48:53 spitzak Exp $"
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
  if (!linedelta) linedelta = r.w()*delta;

  // theoretically, if the current GPort permits, we could write
  // directly into it, avoiding the temporary GWorld. For now I
  // will go the safe way... .
  char direct = 0;
  GWorldPtr gw;
  Rect bounds;
  bounds.left=0; bounds.right=r.w(); bounds.top=0; bounds.bottom=r.h();
  QDErr err = NewGWorld( &gw, 32, &bounds, 0L, 0L, useTempMem );
  bool mono = (pixeltype==LUMINANCE);
  if (err==noErr && gw) {
    PixMapHandle pm = GetGWorldPixMap( gw );
    if ( pm ) {
      LockPixels( pm );
      if ( *pm ) {
        uchar *base = (uchar*)GetPixBaseAddr( pm );
        if ( base ) {
          PixMapPtr pmp = *pm;
          // make absolutely sure that we can use a direct memory write to
          // create the pixmap!
          if ( pmp->pixelType == 16 || pmp->pixelSize == 32 || pmp->cmpCount == 3 || pmp->cmpSize == 8 ) {
            int rowBytes = pmp->rowBytes & 0x3fff;
            if ( cb )
            {
              uchar *tmpBuf = new uchar[ r.w()*delta ];
              if ( mono ) delta -= 1; else delta -= 3; 
              for ( int i=0; i<r.h(); i++ )
              {
                uchar *dst = base + i*rowBytes;
                const uchar* src = cb( userdata, 0, i, r.w(), tmpBuf );
                if ( mono ) {
                  for ( int j=0; j<r.w(); j++ )
                    { uchar c = *src++; *dst++ = 0; *dst++ = c; *dst++ = c; *dst++ = c; src += delta; }
                } else {
                  for ( int j=0; j<r.w(); j++ )
                    { *dst++ = 0; *dst++ = *src++; *dst++ = *src++; *dst++ = *src++; src += delta; }
                }
              }
              delete[] tmpBuf;
            }
            else
            {
              if ( mono ) delta -= 1; else delta -= 3; 
              for ( int i=0; i<r.h(); i++ )
              {
                const uchar *src = buf+i*linedelta;
                uchar *dst = base + i*rowBytes;
                if ( mono ) {
                  for ( int j=0; j<r.w(); j++ )
                    { uchar c = *src++; *dst++ = 0; *dst++ = c; *dst++ = c; *dst++ = c; src += delta; }
                } else {
                  for ( int j=0; j<r.w(); j++ )
                    { *dst++ = 0; *dst++ = *src++; *dst++ = *src++; *dst++ = *src++; src += delta; }
                }
              }
            }
#if 0
	    // WHAT GOES HERE???
//           copy_offscreen( X, Y, W, H, gw, 0, 0 );
//	     direct = 1;
#endif
          }
        }
      }

      UnlockPixels( pm );
    }

    DisposeGWorld( gw );
  }

  if ( direct )
    return;

  // following the very safe (and very slow) way to write the image into the give port
  if ( cb )
  {
    uchar *tmpBuf = new uchar[ r.w()*3 ];
    for ( int i=0; i<r.h(); i++ )
    {
      const uchar* src = cb( userdata, 0, i, r.w(), tmpBuf );
      for ( int j=0; j<r.w(); j++ )
      {
        if ( mono )          
          { color( src[0], src[0], src[0] ); src++; }
        else
          { color( src[0], src[1], src[2] ); src+=3; }
        MoveTo( r.x()+j, r.y()+i );
        Line( 0, 0 );
      }
    }
    delete[] tmpBuf;
  }
  else
  {
    for ( int i=0; i<r.h(); i++ )
    {
      const uchar *src = buf+i*linedelta;
      for ( int j=0; j<r.w(); j++ )
      {
        if ( mono )          
          color( src[0], src[0], src[0] );
        else
          color( src[0], src[1], src[2] );
        MoveTo( r.x()+j, r.y()+i );
        Line( 0, 0 );
        src += delta;
      }
    }
  }

// \todo Mac : the above function does not support subregions yet
#ifdef later_we_do_this
// \todo Mac : the following code is taken from drawimage_win32 and needs to be modified for Mac Carbon
//  if (!linedelta) linedelta = r.w()*delta;

  int x, y, w, h;
  clip_box(X,Y,W,H,x,y,w,h);
  if (w<=0 || h<=0) return;
  if (buf) buf += (x-X)*delta + (y-Y)*linedelta;

//  static U32 bmibuffer[256+12];
//  BITMAPINFO &bmi = *((BITMAPINFO*)bmibuffer);
//  if (!bmi.bmiHeader.biSize) {
//    bmi.bmiHeader.biSize = sizeof(bmi)-4; // does it use this to determine type?
//    bmi.bmiHeader.biPlanes = 1;
//    bmi.bmiHeader.biCompression = BI_RGB;
//    bmi.bmiHeader.biXPelsPerMeter = 0;
//    bmi.bmiHeader.biYPelsPerMeter = 0;
//    bmi.bmiHeader.biClrUsed = 0;
//    bmi.bmiHeader.biClrImportant = 0;
//  }
//  if (mono) {
//    for (int i=0; i<256; i++) {
//      bmi.bmiColors[i].rgbBlue = i;
//      bmi.bmiColors[i].rgbGreen = i;
//      bmi.bmiColors[i].rgbRed = i;
//      bmi.bmiColors[i].rgbReserved = i;
//    }
//  }
//  bmi.bmiHeader.biWidth = w;
//  bmi.bmiHeader.biBitCount = mono ? 8 : 24;
  int pixelsize = mono ? 1 : 3;
  int linesize = (pixelsize*w+3)&~3;
  
  static U32* buffer;
  int blocking = h;
  {int size = linesize*h;
  if (size > MAXBUFFER) {
    size = MAXBUFFER;
    blocking = MAXBUFFER/linesize;
  }
  static long buffer_size;
  if (size > buffer_size) {
    delete[] buffer;
    buffer_size = size;
    buffer = new U32[(size+3)/4];
  }}
//  bmi.bmiHeader.biHeight = blocking;
  static U32* line_buffer;
  if (!buf) {
    int size = W*delta;
    static int line_buf_size;
    if (size > line_buf_size) {
      delete[] line_buffer;
      line_buf_size = size;
      line_buffer = new U32[(size+3)/4];
    }
  }
  for (int j=0; j<h; ) {
    int k;
    for (k = 0; j<h && k<blocking; k++, j++) {
      const uchar* from;
      if (!buf) { // run the converter:
	from = cb(userdata, x-X, y-Y+j, w, (uchar*)line_buffer);
      } else {
	from = buf;
	buf += linedelta;
      }
      uchar *to = (uchar*)buffer+(blocking-k-1)*linesize;
      if (mono) {
	for (int i=w; i--; from += delta) *to++ = *from;
      } else {
	for (int i=w; i--; from += delta, to += 3) {
	  uchar r = from[0];
	  to[0] = from[2];
	  to[1] = from[1];
	  to[2] = r;
        }
      }
    }
//    SetDIBitsToDevice(gc, x, y+j-k, w, k, 0, 0, 0, k,
//		      (LPSTR)((uchar*)buffer+(blocking-k)*linesize),
//		      &bmi,
//		      DIB_RGB_COLORS
//		      );
  }
#endif
}

//
// End of "$Id: fl_draw_image_mac.cxx,v 1.12 2005/01/25 17:48:53 spitzak Exp $".
//
