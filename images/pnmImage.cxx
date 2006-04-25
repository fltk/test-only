// "$Id: pnmImage.cxx 4288 2005-04-16 00:13:17Z mike $"
//
// pnmImage routines.
//
// Copyright 1997-2006 by Easy Software Products.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
// Contents:
//
//   pnmImage::pnmImage() - Load a PNM image...
//

//
// Include necessary header files...
//

#include <fltk/run.h>
#include <fltk/error.h>
#include <fltk/pnmImage.h>
#include <fltk/string.h>
#include <fltk/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

using namespace fltk;

// Read the pnmfile, and allocate a block of image data and return it.
// Caller is responsible for deleting the data.
// Return false on failure.
// If arrayp is null then this quits after getting the width & height

bool readpnm(const char* name,
	     int& width,
	     int& height,
	     PixelType& pixeltype,
	     unsigned char** arrayp)
{
  FILE		*fp;		// File pointer
  int		x, y;		// Looping vars
  char		line[1024],	// Input line
		*lineptr;	// Pointer in line
  uchar		*ptr,		// Pointer to pixel values
		byte,		// Byte from file
		bit;		// Bit in pixel
  int		format,		// Format of PNM file
		val,		// Pixel value
		maxval;		// Maximum pixel value
  
  if ((fp = fopen(name, "rb")) == NULL) return false;

  //
  // Read the file header in the format:
  //
  //   Pformat
  //   # comment1
  //   # comment2
  //   ...
  //   # commentN
  //   width
  //   height
  //   max sample
  //

  lineptr = fgets(line, sizeof(line), fp);
  format = maxval = 0;
  if (lineptr) {
    lineptr ++;
    format = atoi(lineptr);
    while (isdigit(*lineptr)) lineptr ++;
    if (format == 7) lineptr = (char *)"";
  }
  int depth;
  if (format == 1 || format == 2 || format == 4 || format == 5)
    {pixeltype = MONO; depth = 1;}
  else
    {pixeltype = RGB; depth = 3;}

  while (lineptr != NULL) {
    if (*lineptr == '\0' || *lineptr == '#') {
      lineptr = fgets(line, sizeof(line), fp);
    } else if (isdigit(*lineptr)) {
      width = strtol(lineptr, &lineptr, 10);
      break;
    } else lineptr ++;
  }

  while (lineptr != NULL) {
    if (*lineptr == '\0' || *lineptr == '#') {
      lineptr = fgets(line, sizeof(line), fp);
    } else if (isdigit(*lineptr)) {
      height = strtol(lineptr, &lineptr, 10);
      break;
    } else lineptr ++;
  }

  if (format != 1 && format != 4) {
    while (lineptr != NULL) {
      if (*lineptr == '\0' || *lineptr == '#') {
	lineptr = fgets(line, sizeof(line), fp);
      } else if (isdigit(*lineptr)) {
	maxval = strtol(lineptr, &lineptr, 10);
	break;
      } else lineptr ++;
    }
  } else maxval = 1;

  if (!lineptr) {
    fclose(fp);
    fltk::error("Early end-of-file in PNM file \"%s\"!", name);
    return false;
  }

  if (!arrayp) {fclose(fp); return true;}

  // Allocate memory ...
  uchar * array  = new uchar[width*height*depth];
  *arrayp = array;

  // Read the image file ...
  unsigned n;
  switch (format) {
  case 1:
  case 2:
  case 3:
    ptr = array;
    for (n = width*height*depth; n--;) {
      if (fscanf(fp, "%d", &val) == 1) *ptr++ = (uchar)(255 * val / maxval);
      else break;
    }
    break;
  case 4:
    for (y = 0; y < height; y ++) {
      ptr = (uchar *)array + y * width * depth;
      for (x = width, byte = (uchar)getc(fp), bit = 128; x > 0; x --) {
	if (byte & bit) *ptr++ = 255;
	else *ptr++ = 0;
	if (bit > 1) bit >>= 1;
	else {
	  bit  = 128;
	  byte = (uchar)getc(fp);
	}
      }
    }
    break;
  case 5:
  case 6:
    // packed binary data
    fread(array, 1, width*height*depth, fp);
    break;
  case 7: /* XV 3:3:2 thumbnail format */
    ptr = array;
    for (n = width*height*depth; n--;) {
      byte = (uchar)getc(fp);
      *ptr++ = (uchar)(255 * ((byte >> 5) & 7) / 7);
      *ptr++ = (uchar)(255 * ((byte >> 2) & 7) / 7);
      *ptr++ = (uchar)(255 * (byte & 3) / 3);
    }
    break;
  }
  fclose(fp);
  return true;
}

void pnmImage::_measure(int& W, int& H) const
{
  if (w() >= 0) { 
    W = w(); H = h(); 
    return; 
  }
  PixelType pixeltype;
  if (!readpnm(get_filename(), W, H, pixeltype, 0)) {
    const_cast<pnmImage*>(this)->setsize(0,0);
    W = H = 0;
  } else {
    const_cast<pnmImage*>(this)->setsize(W, H);
  }
}

void pnmImage::update()
{
  int w, h; PixelType pixeltype;
  uchar* array = 0;
  if (!readpnm(get_filename(), w, h, pixeltype, &array)) {
    delete[] array;
    return;
  }
  GSave gsave;
  make_current();
  drawimage(array, pixeltype, Rectangle(w,h));
  delete[] array;
}

bool pnmImage::test(const uchar* block, unsigned length) {
  if (block[0] != 'P') return false;
  if (block[1] < '1' || block[1] > '7') return false;
  if (!isspace(block[2])) return false;
  return true;
}


//
// End of "$Id: pnmImage.cxx 4288 2005-04-16 00:13:17Z mike $".
//
