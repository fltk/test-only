//
// "$Id: Bitmap.cxx,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $"
//
// Carbon bitmap drawing routines for the Fast Light Tool Kit (FLTK).
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

#include "Fl_Carbon_Display.H"



// Create a 1-bit mask used for alpha blending
Fl_Bitmask fl_create_alphamask(int w, int h, int d, int ld, const uchar *array) {
  Fl_Bitmask mask;
  int bmw = (w + 7) / 8;
  uchar *bitmap = new uchar[bmw * h];
  uchar *bitptr, bit;
  const uchar *dataptr;
  int x, y;
  static uchar dither[16][16] = { // Simple 16x16 Floyd dither
    { 0,   128, 32,  160, 8,   136, 40,  168,
      2,   130, 34,  162, 10,  138, 42,  170 },
    { 192, 64,  224, 96,  200, 72,  232, 104,
      194, 66,  226, 98,  202, 74,  234, 106 },
    { 48,  176, 16,  144, 56,  184, 24,  152,
      50,  178, 18,  146, 58,  186, 26,  154 },
    { 240, 112, 208, 80,  248, 120, 216, 88,
      242, 114, 210, 82,  250, 122, 218, 90 },
    { 12,  140, 44,  172, 4,   132, 36,  164,
      14,  142, 46,  174, 6,   134, 38,  166 },
    { 204, 76,  236, 108, 196, 68,  228, 100,
      206, 78,  238, 110, 198, 70,  230, 102 },
    { 60,  188, 28,  156, 52,  180, 20,  148,
      62,  190, 30,  158, 54,  182, 22,  150 },
    { 252, 124, 220, 92,  244, 116, 212, 84,
      254, 126, 222, 94,  246, 118, 214, 86 },
    { 3,   131, 35,  163, 11,  139, 43,  171,
      1,   129, 33,  161, 9,   137, 41,  169 },
    { 195, 67,  227, 99,  203, 75,  235, 107,
      193, 65,  225, 97,  201, 73,  233, 105 },
    { 51,  179, 19,  147, 59,  187, 27,  155,
      49,  177, 17,  145, 57,  185, 25,  153 },
    { 243, 115, 211, 83,  251, 123, 219, 91,
      241, 113, 209, 81,  249, 121, 217, 89 },
    { 15,  143, 47,  175, 7,   135, 39,  167,
      13,  141, 45,  173, 5,   133, 37,  165 },
    { 207, 79,  239, 111, 199, 71,  231, 103,
      205, 77,  237, 109, 197, 69,  229, 101 },
    { 63,  191, 31,  159, 55,  183, 23,  151,
      61,  189, 29,  157, 53,  181, 21,  149 },
    { 254, 127, 223, 95,  247, 119, 215, 87,
      253, 125, 221, 93,  245, 117, 213, 85 }
  };

  // Generate a 1-bit "screen door" alpha mask; not always pretty, but
  // definitely fast...  In the future we may be able to support things
  // like the RENDER extension in XFree86, when available, to provide
  // true RGBA-blended rendering.  See:
  //
  //     http://www.xfree86.org/~keithp/render/protocol.html
  //
  // for more info on XRender...
  //
  // MacOS already provides alpha blending support and has its own
  // fl_create_alphamask() function...
  memset(bitmap, 0, bmw * h);

  for (dataptr = array + d - 1, y = 0; y < h; y ++, dataptr += ld)
    for (bitptr = bitmap + y * bmw, bit = 1, x = 0; x < w; x ++, dataptr += d) {
      if (*dataptr > dither[x & 15][y & 15])
	*bitptr |= bit;
      if (bit < 128) bit <<= 1;
      else {
	bit = 1;
	bitptr ++;
      }
    }

  mask = fl_create_bitmask(w, h, bitmap);
  delete[] bitmap;

  return (mask);
}

Fl_Bitmask fl_create_bitmask(int w, int h, const uchar *array) {
  Rect srcRect;
  srcRect.left = 0; srcRect.right = w;
  srcRect.top = 0; srcRect.bottom = h;
  GrafPtr savePort;

  GetPort(&savePort); // remember the current port

  Fl_Bitmask gw;
  NewGWorld( &gw, 1, &srcRect, 0L, 0L, 0 );
  PixMapHandle pm = GetGWorldPixMap( gw );
  if ( pm ) 
  {
    LockPixels( pm );
    if ( *pm ) 
    {
      uchar *base = (uchar*)GetPixBaseAddr( pm );
      if ( base ) 
      {
        PixMapPtr pmp = *pm;
        // verify the parameters for direct memory write
        if ( pmp->pixelType == 0 || pmp->pixelSize == 1 || pmp->cmpCount == 1 || pmp->cmpSize == 1 ) 
        {
          static uchar reverse[16] =	/* Bit reversal lookup table */
          { 0x00, 0x88, 0x44, 0xcc, 0x22, 0xaa, 0x66, 0xee, 0x11, 0x99, 0x55, 0xdd, 0x33, 0xbb, 0x77, 0xff };
          uchar *dst = base;
          const uchar *src = array;
          int rowBytesSrc = (w+7)>>3 ;
          int rowPatch = (pmp->rowBytes&0x3fff) - rowBytesSrc;
          for ( int j=0; j<h; j++,dst+=rowPatch )
            for ( int i=0; i<rowBytesSrc; i++,src++ )
              *dst++ = (reverse[*src & 0x0f] & 0xf0) | (reverse[(*src >> 4) & 0x0f] & 0x0f);
        }
      }
      UnlockPixels( pm );
    }
  }

  SetPort(savePort);
  return gw;               /* tell caller we succeeded! */
}

void fl_delete_bitmask(Fl_Bitmask id) {
  if (id) DisposeGWorld(id);
}




//COPYDEEPMASK
#if 0
// Create an 8-bit mask used for alpha blending
Fl_Bitmask fl_create_alphamask(int w, int h, int d, int ld, const uchar *array) {
  Rect srcRect;
  srcRect.left = 0; srcRect.right = w;
  srcRect.top = 0; srcRect.bottom = h;
  GrafPtr savePort;

  GetPort(&savePort); // remember the current port

  Fl_Bitmask gw;
  NewGWorld( &gw, 8, &srcRect, 0L, 0L, 0 );
  PixMapHandle pm = GetGWorldPixMap( gw );
  if ( pm ) 
  {
    LockPixels( pm );
    if ( *pm ) 
    {
      uchar *base = (uchar*)GetPixBaseAddr( pm );
      if ( base ) 
      {
        PixMapPtr pmp = *pm;
        // verify the parameters for direct memory write
        if ( pmp->pixelType == 0 || pmp->pixelSize == 8 || pmp->cmpCount == 1 || pmp->cmpSize == 8 ) 
        {
	  // Copy alpha values from the source array to the pixmap...
	  array += d - 1;
          int rowoffset = (pmp->rowBytes & 0x3fff) - w;
	  for (int y = h; y > 0; y --, array += ld, base += rowoffset) {
	    for (int x = w; x > 0; x --, array += d) {
	      *base++ = 255 /*255 - *array*/;
	    }
	  }
        }
      }
      UnlockPixels( pm );
    }
  }

  SetPort(savePort);
  return gw;               /* tell caller we succeeded! */
}

#endif // COPYDEEPMASK



class Fl_Bitmap_Cache: public Fl_Image_Cache{
public:
  unsigned id; // for internal use
  Fl_Bitmap_Cache(Fl_Image * im, Fl_Device * dev):Fl_Image_Cache(im,dev){};
  ~Fl_Bitmap_Cache(){fl_delete_bitmask((Fl_Offscreen)(id));}
};




void Fl_Carbon_Display::draw(Fl_Bitmap * img, int X, int Y, int W, int H, int cx, int cy) {

  Fl_Bitmap_Cache *cache = (Fl_Bitmap_Cache *) check_image_cache(img); 
  if (!cache){ // building one)
    cache = new Fl_Bitmap_Cache(img,this);
    cache->id = fl_create_bitmask(img->w(), img->h(), img->array);
  }

  GrafPtr dstPort;
  GetPort( &dstPort );
  Rect src, dst;
  GetPortBounds( (Fl_Offscreen)(cache->id), &src );
  SetRect( &src, cx, cy, cx+W, cy+H );
  SetRect( &dst, X, Y, X+W, Y+H );
  CopyBits(GetPortBitMapForCopyBits((Fl_Offscreen)(cache->id)),	// srcBits
	   GetPortBitMapForCopyBits(dstPort),	// dstBits
	   &src,		 		// src bounds
	   &dst, 				// dst bounds
	   srcOr, 				// mode
	   0L);					// mask region
}





//
// End of "$Id: Bitmap.cxx,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $".
//
