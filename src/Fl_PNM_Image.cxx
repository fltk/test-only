/
// "$Id$
/
// Fl_PNM_Image routines
/
// Copyright 1997-2004 by Easy Software Products
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/
// Contents
/
//   Fl_PNM_Image::Fl_PNM_Image() - Load a PNM image..
/

/
// Include necessary header files..
/

#include <FL/Fl.H
#include <FL/Fl_PNM_Image.H
#include <stdio.h
#include <stdlib.h
#include "flstring.h


/
// 'Fl_PNM_Image::Fl_PNM_Image()' - Load a PNM image..
/

Fl_PNM_Image::Fl_PNM_Image(const char *name)	// I - File to rea
  : Fl_RGB_Image(0,0,0) 
  FILE		*fp;		// File pointe
  int		x, y;		// Looping var
  char		line[1024],	// Input lin
		*lineptr;	// Pointer in lin
  uchar		*ptr,		// Pointer to pixel value
		byte,		// Byte from fil
		bit;		// Bit in pixe
  int		format,		// Format of PNM fil
		val,		// Pixel valu
		maxval;		// Maximum pixel valu


  if ((fp = fopen(name, "rb")) == NULL) return

  /
  // Read the file header in the format
  /
  //   Pforma
  //   # comment
  //   # comment
  //   ..
  //   # comment
  //   widt
  //   heigh
  //   max sampl
  /

  lineptr = fgets(line, sizeof(line), fp)
  if (!lineptr) 
    fclose(fp)
    Fl::error("Early end-of-file in PNM file \"%s\"!", name)
    return
  

  lineptr ++

  format = atoi(lineptr)
  while (isdigit(*lineptr)) lineptr ++

  if (format == 7) lineptr = ""

  while (lineptr != NULL && w() == 0) 
    if (*lineptr == '\0' || *lineptr == '#') 
      lineptr = fgets(line, sizeof(line), fp)
    } else if (isdigit(*lineptr)) 
      w(strtol(lineptr, &lineptr, 10))
    } else lineptr ++
  

  while (lineptr != NULL && h() == 0) 
    if (*lineptr == '\0' || *lineptr == '#') 
      lineptr = fgets(line, sizeof(line), fp)
    } else if (isdigit(*lineptr)) 
      h(strtol(lineptr, &lineptr, 10))
    } else lineptr ++
  

  if (format != 1 && format != 4) 
    maxval = 0

    while (lineptr != NULL && maxval == 0) 
      if (*lineptr == '\0' || *lineptr == '#') 
	lineptr = fgets(line, sizeof(line), fp)
      } else if (isdigit(*lineptr)) 
	maxval = strtol(lineptr, &lineptr, 10)
      } else lineptr ++
    
  } else maxval = 1

  // Allocate memory..
  if (format == 1 || format == 2 || format == 4 || format == 5) d(1)
  else d(3)

//  printf("%s = %dx%dx%d\n", name, w(), h(), d())

  array       = new uchar[w() * h() * d()]
  alloc_array = 1

  // Read the image file..
  for (y = 0; y < h(); y ++) 
    ptr = (uchar *)array + y * w() * d()

    switch (format) 
      case 1 
      case 2 
          for (x = w(); x > 0; x --
            if (fscanf(fp, "%d", &val) == 1) *ptr++ = (uchar)(255 * val / maxval)
          break

      case 3 
          for (x = w(); x > 0; x --) 
            if (fscanf(fp, "%d", &val) == 1) *ptr++ = (uchar)(255 * val / maxval)
            if (fscanf(fp, "%d", &val) == 1) *ptr++ = (uchar)(255 * val / maxval)
            if (fscanf(fp, "%d", &val) == 1) *ptr++ = (uchar)(255 * val / maxval)
          
          break

      case 4 
          for (x = w(), byte = (uchar)getc(fp), bit = 128; x > 0; x --) 
	    if (byte & bit) *ptr++ = 255
	    else *ptr++ = 0

            if (bit > 1) bit >>= 1
            else 
              bit  = 128
              byte = (uchar)getc(fp)
            
          
          break

      case 5 
      case 6 
          fread(ptr, w(), d(), fp)
          break

      case 7 : /* XV 3:3:2 thumbnail format *
          for (x = w(); x > 0; x --) 
	    byte = (uchar)getc(fp)

	    *ptr++ = (uchar)(255 * ((byte >> 5) & 7) / 7)
	    *ptr++ = (uchar)(255 * ((byte >> 2) & 7) / 7)
	    *ptr++ = (uchar)(255 * (byte & 3) / 3)
	  
          break
    
  

  fclose(fp)



/
// End of "$Id$"
/
