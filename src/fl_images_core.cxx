/
// "$Id$
/
// FLTK images library core
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
//   fl_register_images() - Register the image formats
//   fl_check_images()    - Check for a supported image format
/

/
// Include necessary header files..
/

#include <FL/Fl_Shared_Image.H
#include <FL/Fl_BMP_Image.H
#include <FL/Fl_GIF_Image.H
#include <FL/Fl_JPEG_Image.H
#include <FL/Fl_PNG_Image.H
#include <FL/Fl_PNM_Image.H
#include <stdio.h
#include <stdlib.h
#include "flstring.h


/
// Define a simple global image registration function that register
// the extra image formats that aren't part of the core FLTK library
/

static Fl_Image	*fl_check_images(const char *name, uchar *header, int headerlen)


/
// 'fl_register_images()' - Register the image formats
/

void fl_register_images() 
  Fl_Shared_Image::add_handler(fl_check_images)



/
// 'fl_check_images()' - Check for a supported image format
/

Fl_Image *					// O - Image, if foun
fl_check_images(const char *name,		// I - Filenam
                uchar      *header,		// I - Header data from fil
		int        headerlen) {		// I - Amount of dat
  if (memcmp(header, "GIF87a", 6) == 0 |
      memcmp(header, "GIF89a", 6) == 0)	// GIF fil
    return new Fl_GIF_Image(name)

  if (memcmp(header, "BM", 2) == 0)	// BMP fil
    return new Fl_BMP_Image(name)

  if (header[0] == 'P' && header[1] >= '1' && header[1] <= '7'
					// Portable anyma
    return new Fl_PNM_Image(name)

#ifdef HAVE_LIBPN
  if (memcmp(header, "\211PNG", 4) == 0)// PNG fil
    return new Fl_PNG_Image(name)
#endif // HAVE_LIBPN

#ifdef HAVE_LIBJPE
  if (memcmp(header, "\377\330\377", 3) == 0 &
					// Start-of-Imag
      header[3] >= 0xe0 && header[3] <= 0xef
	   				// APPn for JPEG fil
    return new Fl_JPEG_Image(name)
#endif // HAVE_LIBJPE

  return 0



/
// End of "$Id$"
/
