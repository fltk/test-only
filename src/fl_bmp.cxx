//
// "$Id: fl_bmp.cxx,v 1.9 2000/07/14 08:35:01 clip Exp $"
//
// Adapted to FLTK by Vincent Penne (vincent.penne@wanadoo.fr)
//
// Copyright 1997-1999 by Distributed Simulation Technology Inc.
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
// Please report all bugs and problems to /dev/null
//   


/////////////////////////////////////////////////////////////////////////////
//
// BMP Library
//
/////////////////////////////////////////////////////////////////////////////

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include <FL/Fl_Shared_Image.H>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int PIXEL_SIZE = 4;  // Number of bytes a pixel requires.
const int IMG_NON_TRANSPARENT = 0;
#define BMP_DEBUG    0

typedef struct
{
  char          id1;
  char          id2;
  unsigned long fileSize;
  unsigned long reserved;
  unsigned long dataOffSet;
} BITMAP_FILE_HEADER;

typedef struct
{
  unsigned long  headerSize;      //DWORD
  long           pixelWidth;      //LONG
  long           pixelHeight;     //LONG
  unsigned short numPlanes;       //WORD
  unsigned short bitsPerPixel;    //WORD
  unsigned long  compression;     //DWORD
  unsigned long  imageDataSize;   //DWORD
  long           xPixelsPerMeter; //LONG
  long           yPixelsPerMeter; //LONG
  unsigned long  colorsUsed;      //DWORD
  unsigned long  importantColors; //DWORD
} BITMAP_INFO_HEADER;

typedef struct
{
  unsigned char r, g, b, u;
} RGB_QUAD;

#ifndef BI_RGB
// Compression specifications
#define BI_RGB       0
#define BI_RLE8      1
#define BI_RLE4      2
#define BI_BITFIELDS 3
#endif

typedef unsigned char uchar;
static FILE *bmpFile;
static uchar* bmpDatas;

int Fl_BMP_Image::test(uchar* buffer, size_t size)
{
  return !strncmp((char*)buffer, "BM", size<2? size:2);
}

inline void SetError(const char* /*s*/)
{
  //  fprintf(stderr, s);
}

static uchar GETC()
{
  if (bmpDatas) return *bmpDatas++;
  else return fgetc(bmpFile);
}

//////////////////////////////////////////////////////////////////////////
//
// Method:    ReadLittleEndianWORD
//
// Purpose:   Read a word in Little Endian byte ordering.
//
// Arguments: f    Pointer to the image file
//
// Returns:   The word read from the file
//
//////////////////////////////////////////////////////////////////////////
static unsigned short ReadLittleEndianWORD()
{
  char c1, c2;

  c1 = GETC();
  c2 = GETC();

  return (c2 << 8) + c1;
}

//////////////////////////////////////////////////////////////////////////
//
// Method:    ReadLittleEndianDWORD
//
// Purpose:   Read a double word in Little Endian byte ordering.
//
// Arguments: f    Pointer to the image file
//
// Returns:   The double word read from the file
//
//////////////////////////////////////////////////////////////////////////
static unsigned short ReadLittleEndianDWORD()
{
  char c1, c2, c3, c4;

  c1 = GETC();
  c2 = GETC();
  c3 = GETC();
  c4 = GETC();

  return (c4 << 24) + (c3 << 16) + (c2 << 8) + c1;
}

//////////////////////////////////////////////////////////////////////////
//
// Method:    ReadLittleEndianUINT
//
// Purpose:   Read an unsigned integer in Little Endian byte ordering.
//
// Arguments: f    Pointer to the image file
//
// Returns:   The unsigned integer read from the file
//
//////////////////////////////////////////////////////////////////////////
#if 0
static short ReadLittleEndianUINT()
{
  char c1, c2;

  c1 = GETC();
  c2 = GETC();

  return (c2 << 8) + c1;
}
#endif

void Fl_BMP_Image::measure(int &W, int &H)
{
  if (w>=0) { 
    W=w; H=h; 
    return; 
  }

  bmpDatas = datas;

  BITMAP_FILE_HEADER fileHeader;
  BITMAP_INFO_HEADER infoHeader;

  if (!datas)
    if ((bmpFile = fopen(get_filename(), "rb")) == NULL)
      {
	SetError("Error while opening BMP texture file.");
	w = W = 0;
	return;
      }

  {
    // Read BMP file header structure
    fileHeader.id1 = GETC();
    fileHeader.id2 = GETC();

    // Check to make sure this is a BMP file
    if ((fileHeader.id1 != 'B') || (fileHeader.id2 != 'M'))
    {
      if (!datas) fclose(bmpFile);
      SetError("Error reading BMP texture file, not a legitimate BMP file.");
      w = W = 0;
      return;
    }

    // Reading Little Endian because BMPs are Intel based
    fileHeader.fileSize = ReadLittleEndianDWORD();
    fileHeader.reserved = ReadLittleEndianDWORD();
    fileHeader.dataOffSet = ReadLittleEndianDWORD();

    // Read BMP info header structure
    infoHeader.headerSize = ReadLittleEndianDWORD();
    w = infoHeader.pixelWidth = ReadLittleEndianDWORD();
    h = infoHeader.pixelHeight = ReadLittleEndianDWORD();
  }

  if (!datas) fclose(bmpFile);

  W=w;
  H=h;
  return;
}

void Fl_BMP_Image::read()
{
  id = mask = 0;

  bmpDatas = datas;

  BITMAP_FILE_HEADER fileHeader;
  BITMAP_INFO_HEADER infoHeader;
  RGB_QUAD *palette = 0;
  register int w, h;
  unsigned int index;
  unsigned char *pRgbBuf;
  unsigned char *rgbBuf;
  int scanLinePad;
  int linePad;

  if (!datas)
    if ((bmpFile = fopen(get_filename(), "rb")) == NULL)
      {
	SetError("Error while opening BMP texture file.");
	return;
      }

  {
    // Read BMP file header structure
    fileHeader.id1 = GETC();
    fileHeader.id2 = GETC();

    // Check to make sure this is a BMP file
    if ((fileHeader.id1 != 'B') || (fileHeader.id2 != 'M'))
    {
      if (!datas) fclose(bmpFile);
      SetError("Error reading BMP texture file, not a legitimate BMP file.");
      return;
    }

    // Reading Little Endian because BMPs are Intel based
    fileHeader.fileSize = ReadLittleEndianDWORD();
    fileHeader.reserved = ReadLittleEndianDWORD();
    fileHeader.dataOffSet = ReadLittleEndianDWORD();

    // Read BMP info header structure
    infoHeader.headerSize = ReadLittleEndianDWORD();
    infoHeader.pixelWidth = ReadLittleEndianDWORD();
    infoHeader.pixelHeight = ReadLittleEndianDWORD();
    infoHeader.numPlanes = ReadLittleEndianWORD();
    infoHeader.bitsPerPixel = ReadLittleEndianWORD();
    infoHeader.compression = ReadLittleEndianDWORD();
    infoHeader.imageDataSize = ReadLittleEndianDWORD();
    infoHeader.xPixelsPerMeter = ReadLittleEndianDWORD();
    infoHeader.yPixelsPerMeter = ReadLittleEndianDWORD();
    infoHeader.colorsUsed = ReadLittleEndianDWORD();
    infoHeader.importantColors = ReadLittleEndianDWORD();

    // Store the dimensions of the image
    int _width = infoHeader.pixelWidth;
    int _height = infoHeader.pixelHeight;
    int _texWidth = _width;
    int _texHeight = _height;

    // Allocate memory based on image size
    rgbBuf = new unsigned char[_texWidth * _texHeight * PIXEL_SIZE];
    if (!rgbBuf)
    {
      SetError("Error reading BMP texture file, out of memory error.");
      goto error;
    }
    memset(rgbBuf, 0, (_texWidth * _texHeight * PIXEL_SIZE));
    scanLinePad = (_texWidth - _width) * PIXEL_SIZE;

    if (infoHeader.colorsUsed == 0)
    {
      switch (infoHeader.bitsPerPixel)
      {
	case 1:
	  infoHeader.colorsUsed = 2;
	  break;
	case 2:
	  infoHeader.colorsUsed = 4;
	  break;
	case 4:
	  infoHeader.colorsUsed = 16;
	  break;
	case 8:
	  infoHeader.colorsUsed = 256;
	  break;
      }
    }

    #if BMP_DEBUG
      printf("Header size %d\n",infoHeader.headerSize);
      printf("Pixel width %d\n",infoHeader.pixelWidth);
      printf("Pixel height %d\n",infoHeader.pixelHeight);
      printf("Planes is   %d\n",infoHeader.numPlanes);
      printf("Bits/pixels %d\n",infoHeader.bitsPerPixel);
      
      printf("Compression is %d\n",infoHeader.compression);
      printf("Image size in bytes is %d\n",infoHeader.imageDataSize);
      printf("XPels is %d\n",infoHeader.xPixelsPerMeter);
      printf("YPels is %d\n",infoHeader.yPixelsPerMeter);
      printf("Colors used is %d\n",infoHeader.colorsUsed);
      printf("Important colors are %d\n",infoHeader.importantColors);
    #endif

    // Allocate memory for color map
    palette = (RGB_QUAD *)malloc(infoHeader.colorsUsed * sizeof(RGB_QUAD));
    if (!palette)
    {
      SetError("Error reading BMP texture file, out of memory error.");
      goto error;
    }

    // Read color map
    for (index = 0; index < infoHeader.colorsUsed; index++)
    {
      palette[index].b = GETC();
      palette[index].g = GETC();
      palette[index].r = GETC();
      palette[index].u = GETC();
    }

    // Read BMP image data
    pRgbBuf = rgbBuf;
    switch (infoHeader.bitsPerPixel)
    {
      case 1:
	int bit, mask, bitIndex;
	int notMult8;

	// No compression for 1 bit per pixel
	if (infoHeader.compression != BI_RGB)
	{
	  SetError("Error reading BMP texture file, " \
		   "compression and bits per pixel mismatch.");
	  goto error;
	}

	notMult8 = _width % 8;

        // BMP file lines are word (4 byte) aligned
	if (notMult8)
	  linePad = 4 - ((_width / 8 + 1) % 4);
	else
	  linePad = (_width / 8) % 4;

	for (h = 0; h < _height; h++)
	{
	  for (w = 0; w < _width / 8; w++)
	  {
	    index = GETC();
	    for (bit = 7; bit >= 0; bit--)
	    {
	      mask = 1 << bit;
	      bitIndex = (index & mask) ? 1 : 0;
	      
	      *pRgbBuf = palette[bitIndex].r;
	      pRgbBuf++;
	      *pRgbBuf = palette[bitIndex].g;
	      pRgbBuf++;
	      *pRgbBuf = palette[bitIndex].b;
	      pRgbBuf++;
	      *pRgbBuf = IMG_NON_TRANSPARENT; // alpha
	      pRgbBuf++;
	    }
	  }

	  // Handle a non-multiple of 8 pixels on a line
	  if (notMult8)
	    for (w = 0; w < notMult8; w++)
	    {
	      index = GETC();
	      for (bit = 7; bit >= 8 - notMult8; bit--)
	      {
		mask = 1 << bit;
		bitIndex = (index & mask) ? 1 : 0;
		
		*pRgbBuf = palette[bitIndex].r;
		pRgbBuf++;
		*pRgbBuf = palette[bitIndex].g;
		pRgbBuf++;
		*pRgbBuf = palette[bitIndex].b;
		pRgbBuf++;
		*pRgbBuf = IMG_NON_TRANSPARENT; // alpha
		pRgbBuf++;
	      }
	    }

	  // Skip padding bytes in file
          for (w = 0; w < linePad; w++)
	    GETC();

          // Skip padding bytes in memory
	  pRgbBuf += scanLinePad;
	}
	break;
	

      case 4:
	int upperIndex, lowerIndex;
	int oddPixels;

	// No compression
	if (infoHeader.compression == BI_RGB)
	{
	  oddPixels = _width % 2;
	  // BMP file lines are word (4 byte) aligned
	  if (oddPixels)
	    linePad = 4 - ((_width / 2 + 1) % 4);
	  else
	    linePad = (_width / 2) % 4;

	  for (h = 0; h < _height; h++)
	  {
	    for (w = 0; w < _width / 2; w++)
	    {
	      index = GETC();
	      // Upper four bits
	      upperIndex = (index >> 4) & 15;
	      
	      *pRgbBuf = palette[upperIndex].r;
	      pRgbBuf++;
	      *pRgbBuf = palette[upperIndex].g;
	      pRgbBuf++;
	      *pRgbBuf = palette[upperIndex].b;
	      pRgbBuf++;
	      *pRgbBuf = IMG_NON_TRANSPARENT; // alpha
	      pRgbBuf++;
	      
	      // Lower four bits
	      lowerIndex = index & 15;
	    
	      *pRgbBuf = palette[lowerIndex].r;
	      pRgbBuf++;
	      *pRgbBuf = palette[lowerIndex].g;
	      pRgbBuf++;
	      *pRgbBuf = palette[lowerIndex].b;
	      pRgbBuf++;
	      *pRgbBuf = IMG_NON_TRANSPARENT; // alpha
	      pRgbBuf++;
	    }
	    
	    // Handle odd number of pixels on a line
	    if (oddPixels)
	    {
	      index = GETC();
	      // Upper four bits
	      upperIndex = (index >> 4) & 15;
	      
	      *pRgbBuf = palette[upperIndex].r;
	      pRgbBuf++;
	      *pRgbBuf = palette[upperIndex].g;
	      pRgbBuf++;
	      *pRgbBuf = palette[upperIndex].b;
	      pRgbBuf++;
	      *pRgbBuf = IMG_NON_TRANSPARENT; // alpha
	      pRgbBuf++;
	    }
	    
	    // Skip padding bytes in file
	    for (w = 0; w < linePad; w++)
	      GETC();
	    
	    // Skip padding bytes in memory
	    pRgbBuf += scanLinePad;
	  }
	}

	// todo: test RLE compression
	// Run Length Encoding compression
	else if (infoHeader.compression == BI_RLE4)
	{
	  //todo: fill in code
	  SetError("Error reading BMP texture file, compression not " \
		   "supported.");
	  goto error;
	}

	else
	{
	  SetError("Error reading BMP texture file, " \
		   "compression and bits per pixel mismatch.");
	  goto error;
	}
	break;
	

      case 8:
	// No compression
	if (infoHeader.compression == BI_RGB)
	{
	  // BMP file lines are word (4 byte) aligned
	  linePad = (_width % 4) ? 4 - (_width % 4) : 0;
	  
	  for (h = 0; h < _height; h++)
	  {
	    for (w = 0; w < _width; w++)
	    {
	      index = GETC();
	      *pRgbBuf = palette[index].r;
	      pRgbBuf++;
	      *pRgbBuf = palette[index].g;
	      pRgbBuf++;
	      *pRgbBuf = palette[index].b;
	      pRgbBuf++;
	      *pRgbBuf = IMG_NON_TRANSPARENT; // alpha
	      pRgbBuf++;
	    }
	    
	    // Skip padding bytes in file
	    for (w = 0; w < linePad; w++)
	      GETC();
	    
	    // Skip padding bytes in memory
	    pRgbBuf = pRgbBuf + (scanLinePad);
	  }
	}

	//todo: test RLE compression
	// Run Length Encoding compression
	else if (infoHeader.compression == BI_RLE8)
	{
	  SetError("Error reading BMP texture file, compression not " \
		   "supported.");
	  goto error;
#ifdef TODO
	  int first, second;
	  int notEOF = TRUE;
	  int pixelCount = 0;
	  int w;

	  while (notEOF)
	  {
	    first = GETC();
	    pixelCount++;
	    second = GETC();
	    pixelCount++;

	    if (first == 0) // Escape sequence
	    {
	      switch (second)
	      {
		case 0: // End of line
		  // Read the width of image
		  for (w = 0; w < _width - pixelCount; w++)
		    pRgbBuf++;
		  pixelCount = 0;

		  // Skip padding bytes in memory
		  pRgbBuf = pRgbBuf + (scanLinePad);
		  break;

		case 1: // End of bitmap
		  notEOF = FALSE;

		  // Read the width of image
		  for (w = 0; w < _width - pixelCount; w++)
		    pRgbBuf++;
		  pixelCount = 0;

		  // Skip padding bytes in memory
		  pRgbBuf = pRgbBuf + (scanLinePad);
		  break;

		case 2: // Delta to next pixel
		  //todo: fill in code
		  break;
		  
		default: // Absolute mode (03H <= second <= FFH)
		  // Copy pixels from palette
		  if ((second > 2) && (second < 256))
		    while (second-- && notEOF)
		    {
		      int linePad;
		      
		      index = GETC();
		      
		      *pRgbBuf = palette[index].r;
		      pRgbBuf++;
		      *pRgbBuf = palette[index].g;
		      pRgbBuf++;
		      *pRgbBuf = palette[index].b;
		      pRgbBuf++;
		      *pRgbBuf = IMG_NON_TRANSPARENT; // alpha
		      pRgbBuf++;
		      
		      // Absolute mode runs are word (4 byte) aligned
		      linePad = (index % 4) ? 4 - (index % 4) : 0;
		      // Skip padding bytes in file
		      while (linePad--)
		      {
			GETC();
			pixelCount++;
		      }
		    }
		  break;
	      }
	    }

	    else // Encoded mode, repeat pixel from color palette
	    {
	      int linePad;
	      // Encoded mode runs are word (4 byte) aligned
	      linePad = (first % 4) ? 4 - (first % 4) : 0;
	      
	      while (first--)
	      {
		*pRgbBuf = palette[second].r;
		pRgbBuf++;
		*pRgbBuf = palette[second].g;
		pRgbBuf++;
		*pRgbBuf = palette[second].b;
		pRgbBuf++;
		*pRgbBuf = IMG_NON_TRANSPARENT; // alpha
		pRgbBuf++;
	      }
	      // Skip padding bytes in file
	      while (linePad--)
	      {
		GETC();
		pixelCount++;
	      }
	    }
	  }
#endif
	}
	  
	else
	{
	  SetError("Error reading BMP texture file, " \
		   "compression and bits per pixel mismatch.");
	  goto error;
	}
	break;
	

      case 16:
	int upper8Bits, lower8Bits;
	int redValue, greenValue, blueValue;
	
	// No compression
	if (infoHeader.compression == BI_RGB)
	{
	  // BMP file lines are word (4 byte) aligned
	  linePad = _width % 4;
	
	  for (h = 0; h < _height; h++)
	  {
	    for (w = 0; w < _width; w++)
	    {
	      upper8Bits = GETC();
	      lower8Bits = GETC();

	      // Red is most significant 5 bits
	      redValue = (124 & upper8Bits) >> 2; // 124 = (31 << 2)
	      *pRgbBuf = redValue;
	      pRgbBuf++;

	      // Green is next 5 bits
	      greenValue = (3 & upper8Bits) << 3;
	      greenValue = greenValue | (224 & lower8Bits); // 224 = (7 << 5)
	      *pRgbBuf = greenValue;
	      pRgbBuf++;

	      // Blue is least significant 5 bits
	      blueValue = 31 & lower8Bits;
	      *pRgbBuf = blueValue;
	      pRgbBuf++;
	      
	      *pRgbBuf = IMG_NON_TRANSPARENT; // alpha
	      pRgbBuf++;
	    }
	    
	    // Skip padding bytes in file
	    for (w = 0; w < linePad; w++)
	      GETC();
	    
	    // Skip padding bytes in memory
	    pRgbBuf += scanLinePad;
	  }
	}

	else if (infoHeader.compression == BI_BITFIELDS)
	{
	  //todo: fill in code
	  SetError("Error reading BMP texture file, " \
		   "compression not supported.");
	  goto error;
	}

	else
	{
	  SetError("Error reading BMP texture file, " \
		   "compression and bits per pixel mismatch.");
	  goto error;
	}
	break;
	

      case 24:
	int red, green, blue;

	// No compression for 24 bits per pixel
	if (infoHeader.compression != BI_RGB)
	{
	  SetError("Error reading BMP texture file, " \
		   "compression and bits per pixel mismatch.");
	  goto error;
	}

	// BMP file lines are word (4 byte) aligned
	linePad = (_width * 3 % 4) ? 4 - (_width * 3 % 4) : 0;

	for (h = 0; h < _height; h++)
	{
	  for (w = 0; w < _width; w++)
	  {
	    blue = GETC();
	    green = GETC();
	    red = GETC();
	    *pRgbBuf = red;
	    pRgbBuf++;
	    *pRgbBuf = green;
	    pRgbBuf++;
	    *pRgbBuf = blue;
	    pRgbBuf++;
	    *pRgbBuf = IMG_NON_TRANSPARENT; // alpha
	    pRgbBuf++;
	  }

          // Skip padding bytes in file
          for (w = 0; w < linePad; w++)
	    GETC();

          // Skip padding bytes in memory
	  pRgbBuf += scanLinePad;
	}
	break;


      case 32:
	// No compression
	if (infoHeader.compression == BI_RGB)
	{
	  for (h = 0; h < _height; h++)
	  {
	    for (w = 0; w < _width; w++)
	    {
	      *pRgbBuf = GETC();
	      pRgbBuf++;
	      *pRgbBuf = GETC();
	      pRgbBuf++;
	      *pRgbBuf = GETC();
	      pRgbBuf++;
	      *pRgbBuf = IMG_NON_TRANSPARENT; // alpha
	      pRgbBuf++;
	    }
	    
	    // Skip padding bytes in memory
	    pRgbBuf += scanLinePad;
	  }
	}

	else if (infoHeader.compression == BI_BITFIELDS)
	{
	  //todo: fill in code
	  SetError("Error reading BMP texture file, " \
		   "compression not supported.");
	  goto error;
	}

	else
	{
	  SetError("Error reading BMP texture file, " \
		   "compression and bits per pixel mismatch.");
	  goto error;
	}
	break;

	
      default:
	SetError("Error reading BMP texture file, " \
		 "unknown number of bits per pixel.");
	goto error;
    }
    
    if(!datas) fclose(bmpFile);

    id = fl_create_offscreen(_width, _height);
    fl_begin_offscreen(id);
    fl_draw_image(rgbBuf, 0, 0, _width, _height, PIXEL_SIZE);
    fl_end_offscreen();

    delete []palette;
    delete rgbBuf;
    return;
  }

error:
  if(palette)
    delete []palette;
  delete rgbBuf;
  return;
}

//
// End of "$Id: fl_bmp.cxx,v 1.9 2000/07/14 08:35:01 clip Exp $"
//
