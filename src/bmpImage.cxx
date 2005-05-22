//
// "$Id$"
//
// bmpImage routines.
//
// Copyright 1997-2003 by Easy Software Products.
// Copyright 1998-2004 by Bill Spitzak and others.
// Image support donated by Matthias Melcher, Copyright 2000.
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
#include <fltk/SharedImage.h> // defines bmpImage.h
#include <fltk/events.h>
#include <fltk/error.h>
#include <fltk/draw.h>
#include <fltk/x.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace fltk;

static FILE *bmpFile;
static uchar* bmpDatasStart;
static uchar* bmpDatas;

static uchar GETC()
{
  if (bmpDatas) return *bmpDatas++;
  else return fgetc(bmpFile);
}

#undef FREAD
static int FREAD(void *buf, int size)
{
  if (bmpDatas) {
    memcpy(buf, bmpDatas, size);
    bmpDatas += size;
    return size;
  }
  return fread(buf, 1, size, bmpFile);
}

static int FSEEK(int offset)
{
  if (bmpDatas) {
    bmpDatas = bmpDatasStart + offset;
    return offset;
  }
  return fseek(bmpFile, offset, SEEK_SET);
}

//
// BMP definitions...
//

#ifndef BI_RGB
#  define BI_RGB       0             // No compression - straight BGR data
#  define BI_RLE8      1             // 8-bit run-length compression
#  define BI_RLE4      2             // 4-bit run-length compression
#  define BI_BITFIELDS 3             // RGB bitmap with RGB masks
#endif // !BI_RGB


//
// Local functions...
//

static int		read_long();
static unsigned short	read_word();
static unsigned int	read_dword();

bool bmpImage::test(const uchar* buffer, unsigned size)
{
  return !strncmp((char*)buffer, "BM", size<2? size:2);
}

void bmpImage::_measure(int &W, int &H) const
{
  if (w() >= 0) {
    W = w();
    H = h();
    return;
  }

  bmpDatas = (uchar*)datas;
  bmpDatasStart = bmpDatas;

  if (!datas) {
    if ((bmpFile = fopen(get_filename(), "rb")) == NULL) {
      fltk::warning("Cannot open BMP file '%s'", get_filename());
      const_cast<bmpImage*>(this)->setsize(0,0);
      return;
    }
  }

  // Get the header...
  uchar byte = (uchar)GETC();	// Check "BM" sync chars
  uchar bit  = (uchar)GETC();
  if (byte != 'B' || bit != 'M') {
    if (!datas) fclose(bmpFile);
    return;
  }

  read_dword();	// Skip size
  read_word();	// Skip reserved stuff
  read_word();
  read_dword(); // Read offset to image data

  // Then the bitmap information...
  int info_size = read_dword();

  if (info_size < 40) {
    // Old Windows/OS2 BMP header...
    W = read_word();
    H = read_word();
    const_cast<bmpImage*>(this)->setsize((int)W,(int)H);
  } else {
    // New BMP header...
    W = read_long();
    H = read_long();
    const_cast<bmpImage*>(this)->setsize((int)W,(int)H);
  }

  if (!datas) fclose(bmpFile);

  return;
}

//
// Load a BMP image file.
//
void bmpImage::read()
{
  bmpDatas = (uchar*)datas;
  bmpDatasStart = bmpDatas;

  int		info_size,	// Size of info header
		depth,		// Depth of image (bits)
		bDepth = 3,	// Depth of image (bytes)
		compression,	// Type of compression
		colors_used,	// Number of colors used
		color,		// Color of RLE pixel
		repcount,	// Number of times to repeat
		temp,		// Temporary color
		align,		// Alignment bytes
		dataSize,	// number of bytes in image data set
		row_order=1;    // 1 = normal, -1 = flipped
  long		offbits;	// Offset to image data
  uchar		bit,		// Bit in image
		byte;		// Byte in image
  uchar		*ptr;		// Pointer into pixels
  uchar		colormap[256][3];// Colormap
  uchar		mask = 0;	// single bit mask follows image data
  bool		use_5_6_5 = false; // Use 565 instead of 555 for 16-bit data

  if (!datas) {
    if ((bmpFile = fopen(get_filename(), "rb")) == NULL) {
      fltk::warning("Cannot open BMP file '%s'", get_filename());
      return;
    }
  }

  // Get the header...
  byte = (uchar)GETC();	// Check "BM" sync chars
  bit  = (uchar)GETC();
  if (byte != 'B' || bit != 'M') {
    if (!datas) fclose(bmpFile);
    return;
  }

  read_dword();		// Skip size
  read_word();		// Skip reserved stuff
  read_word();
  offbits = (long)read_dword();// Read offset to image data

  // Then the bitmap information...
  info_size = read_dword();

//  printf("offbits = %ld, info_size = %d\n", offbits, info_size);

  if (info_size < 40) {
    // Old Windows/OS2 BMP header...
    int W = read_word();
    int H = read_word();
    this->setsize(W,H);
    read_word();
    depth = read_word();
    compression = BI_RGB;
    colors_used = 0;

    repcount = info_size - 12;
  } else {
    // New BMP header...
    int W = read_long();
    int H = read_long();
    if (H < 0) {row_order = -1; H = -H;}
    this->setsize(W,H);
    read_word();
    depth = read_word();
    compression = read_dword();
    dataSize = read_dword();
    read_long();
    read_long();
    colors_used = read_dword();
    read_dword();

    repcount = info_size - 40;

    if (!compression && depth>=8 && w()>32/depth) {
      int Bpp = depth/8;
      int maskSize = (((w()*Bpp+3)&~3)*h()) + (((((w()+7)/8)+3)&~3)*h());
      if (maskSize==2*dataSize) {
	mask = 1;
	h_ = (h_/2);
	bDepth = 4;
      }
    }
  }

//  printf("w() = %d, h() = %d, depth = %d, compression = %d, colors_used = %d, repcount = %d\n",
//         w(), h(), depth, compression, colors_used, repcount);

  // Skip remaining header bytes...
  while (repcount > 0) {
    GETC();
    repcount --;
  }

  // Check header data...
  if (!w() || !h() || !depth) {
    if (!datas) fclose(bmpFile);
    return;
  }

  // Get colormap...
  if (colors_used == 0 && depth <= 8)
    colors_used = 1 << depth;

  for (repcount = 0; repcount < colors_used; repcount ++) {
    // Read BGR color...
    FREAD(colormap[repcount], 3);
    //fread(colormap[repcount], 1, 3, );

    // Skip pad byte for new BMP files...
    if (info_size > 12) GETC();
  }

  // Read first dword of colormap. It tells us if 5:5:5 or 5:6:5 for 16 bit
  if (depth == 16)
    use_5_6_5 = (read_dword() == 0xf800);

  // Setup image and buffers...
  if (offbits) FSEEK(offbits);

  uchar *array = new uchar[w() * h() * bDepth];
  //alloc_array = 1;

  // Read the image data...
  color = 0;
  repcount = 0;
  align = 0;
  byte  = 0;
  temp  = 0;

  for (int y = 0; y < h(); y++) {
    ptr = (uchar *)array + (row_order>0?h()-y-1:y) * w() * bDepth;

    int x;

    switch (depth)
    {
      case 1 : // Bitmap
	  for (x = w(), bit = 128; x > 0; x --) {
	    if (bit == 128) byte = (uchar)GETC();

	    if (byte & bit) {
	      *ptr++ = colormap[1][2];
	      *ptr++ = colormap[1][1];
	      *ptr++ = colormap[1][0];
	    } else {
	      *ptr++ = colormap[0][2];
	      *ptr++ = colormap[0][1];
	      *ptr++ = colormap[0][0];
	    }

	    if (bit > 1)
	      bit >>= 1;
	    else
	      bit = 128;
	  }

	  // Read remaining bytes to align to 32 bits...
	  for (temp = (w() + 7) / 8; temp & 3; temp ++) {
	    GETC();
	  }
	  break;

      case 4 : // 16-color
	  for (x = w(), bit = 0xf0; x > 0; x --) {
	    // Get a new repcount as needed...
	    if (repcount == 0) {
	      if (compression != BI_RLE4) {
		repcount = 2;
		color = -1;
	      } else {
		while (align > 0) {
		  align --;
		  GETC();
		}

		if ((repcount = GETC()) == 0) {
		  if ((repcount = GETC()) == 0) {
		    // End of line...
		    x ++;
		    continue;
		  } else if (repcount == 1) {
		    // End of image...
		    break;
		  } else if (repcount == 2) {
		    // Delta...
		    repcount = GETC() * GETC() * w();
		    color = 0;
		  } else {
		    // Absolute...
		    color = -1;
		    align = ((4 - (repcount & 3)) / 2) & 1;
		  }
		} else {
		  color = GETC();
		}
	      }
	    }

	    // Get a new color as needed...
	    repcount --;

	    // Extract the next pixel...
	    if (bit == 0xf0) {
	      // Get the next color byte as needed...
	      if (color < 0) temp = GETC();
	      else temp = color;

	      // Copy the color value...
	      *ptr++ = colormap[(temp >> 4) & 15][2];
	      *ptr++ = colormap[(temp >> 4) & 15][1];
	      *ptr++ = colormap[(temp >> 4) & 15][0];

	      bit  = 0x0f;
	    } else {
	      bit  = 0xf0;

	      // Copy the color value...
	      *ptr++ = colormap[temp & 15][2];
	      *ptr++ = colormap[temp & 15][1];
	      *ptr++ = colormap[temp & 15][0];
	    }

	  }

	  if (!compression) {
	    // Read remaining bytes to align to 32 bits...
	    for (temp = (w() + 1) / 2; temp & 3; temp ++) {
	      GETC();
	    }
	  }
	  break;

      case 8 : // 256-color
	  for (x = w(); x > 0; x --) {
	    // Get a new repcount as needed...
	    if (compression != BI_RLE8) {
	      repcount = 1;
	      color = -1;
	    }

	    if (repcount == 0) {
	      while (align > 0) {
		align --;
		GETC();
	      }

	      if ((repcount = GETC()) == 0) {
		if ((repcount = GETC()) == 0) {
		  // End of line...
		  x ++;
		  continue;
		} else if (repcount == 1) {
		  // End of image...
		  break;
		} else if (repcount == 2) {
		  // Delta...
		  repcount = GETC() * GETC() * w();
		  color = 0;
		} else {
		  // Absolute...
		  color = -1;
		  align = (2 - (repcount & 1)) & 1;
		}
	      } else {
		color = GETC();
	      }
	    }

	    // Get a new color as needed...
	    if (color < 0) temp = GETC();
	    else temp = color;

	    repcount --;

	    // Copy the color value...
	    *ptr++ = colormap[temp][2];
	    *ptr++ = colormap[temp][1];
	    *ptr++ = colormap[temp][0];
	    if (mask) ptr++;
	  }

	  if (!compression) {
	    // Read remaining bytes to align to 32 bits...
	    for (temp = w(); temp & 3; temp ++) {
	      GETC();
	    }
	  }
	  break;

      case 16 : // 16-bit 5:5:5 RGB
	  for (x = w(); x > 0; x --, ptr += bDepth) {
	    uchar b = GETC(), a = GETC() ;
	    if (use_5_6_5) {
	      ptr[0] = (uchar)(( b << 3 ) & 0xf8);
	      ptr[1] = (uchar)(((a << 5) & 0xe0) | ((b >> 3) & 0x1c));
	      ptr[2] = (uchar)(a & 0xf8);
	    } else {
	      ptr[2] = (uchar)((b << 3) & 0xf8);
	      ptr[1] = (uchar)(((a << 6) & 0xc0) | ((b >> 2) & 0x38));
	      ptr[0] = (uchar)((a<<1) & 0xf8);
	    }
	  }

	  // Read remaining bytes to align to 32 bits...
	  for (temp = w() * 3; temp & 3; temp ++) {
	    GETC();
	  }
	  break;

      case 24 : // 24-bit RGB
	  for (x = w(); x > 0; x --, ptr += bDepth) {
	    ptr[2] = (uchar)GETC();
	    ptr[1] = (uchar)GETC();
	    ptr[0] = (uchar)GETC();
	  }

	  // Read remaining bytes to align to 32 bits...
	  for (temp = w() * 3; temp & 3; temp ++) {
	    GETC();
	  }
	  break;
    }
  }

  if (mask) {
    for (int y = 0; y < h(); y++) {
      ptr = (uchar *)array + (row_order>0?h()-y-1:y) * w() * bDepth + 3;
      for (int x = w(), bit = 128; x > 0; x --, ptr+=bDepth) {
	if (bit == 128) byte = (uchar)GETC();
	if (byte & bit)
	  *ptr = 0;
	else
	  *ptr = 255;
	if (bit > 1)
	  bit >>= 1;
	else
	  bit = 128;
      }
      // Read remaining bytes to align to 32 bits...
      for (temp = (w() + 7) / 8; temp & 3; temp ++)
	GETC();
    }
  }

  {GSave gsave;
  make_current();
  drawimage(array, PixelType(bDepth), Rectangle(w_, h_), bDepth);}

  // Close the file and return...
  if (!datas) fclose(bmpFile);

  delete []array;
}


//
// 'read_word()' - Read a 16-bit unsigned integer.
//

static unsigned short	// O - 16-bit unsigned integer
read_word() {
  unsigned char b0, b1;	// Bytes from file

  b0 = (uchar)GETC();
  b1 = (uchar)GETC();

  return ((b1 << 8) | b0);
}


//
// 'read_dword()' - Read a 32-bit unsigned integer.
//

static unsigned int		// O - 32-bit unsigned integer
read_dword() {
  unsigned char b0, b1, b2, b3;	// Bytes from file

  b0 = (uchar)GETC();
  b1 = (uchar)GETC();
  b2 = (uchar)GETC();
  b3 = (uchar)GETC();

  return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


//
// 'read_long()' - Read a 32-bit signed integer.
//

static int			// O - 32-bit signed integer
read_long() {
  unsigned char b0, b1, b2, b3;	// Bytes from file

  b0 = (uchar)GETC();
  b1 = (uchar)GETC();
  b2 = (uchar)GETC();
  b3 = (uchar)GETC();

  return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

//
// End of "$Id$"
//
