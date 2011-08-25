//
// "$Id$"
//
// FLTK images library core.
//
// Copyright 1997-2010 by Easy Software Products.
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
//   fltk3::register_images() - Register the image formats.
//   fl_check_images()    - Check for a supported image format.
//

//
// Include necessary header files...
//

#include <fltk3/SharedImage.h>
#include <fltk3images/BMPImage.h>
#include <fltk3images/GIFImage.h>
#include <fltk3images/JPEGImage.h>
#include <fltk3images/PNGImage.h>
#include <fltk3images/PNMImage.h>
#include <stdio.h>
#include <stdlib.h>
#include "../fltk3/flstring.h"


//
// Define a simple global image registration function that registers
// the extra image formats that aren't part of the core FLTK library.
//

static fltk3::Image	*fl_check_images(const char *name, uchar *header, int headerlen);


/**
\brief Register the image formats.
 *
 This function is provided in the fltk_images library and 
 registers all of the "extra" image file formats that are not part
 of the core FLTK library.
*/
void fltk3::register_images() {
  fltk3::SharedImage::add_handler(fl_check_images);
}


//
// 'fl_check_images()' - Check for a supported image format.
//

fltk3::Image *					// O - Image, if found
fl_check_images(const char *name,		// I - Filename
                uchar      *header,		// I - Header data from file
		int) {				// I - Amount of data (not used)
  if (memcmp(header, "GIF87a", 6) == 0 ||
      memcmp(header, "GIF89a", 6) == 0)	// GIF file
    return new fltk3::GIFImage(name);

  if (memcmp(header, "BM", 2) == 0)	// BMP file
    return new fltk3::BMPImage(name);

  if (header[0] == 'P' && header[1] >= '1' && header[1] <= '7')
					// Portable anymap
    return new fltk3::PNMImage(name);

#ifdef HAVE_LIBPNG
  if (memcmp(header, "\211PNG", 4) == 0)// PNG file
    return new fltk3::PNGImage(name);
#endif // HAVE_LIBPNG

#ifdef HAVE_LIBJPEG
  if (memcmp(header, "\377\330\377", 3) == 0 &&
					// Start-of-Image
      header[3] >= 0xc0 && header[3] <= 0xef)
	   				// APPn for JPEG file
    return new fltk3::JPEGImage(name);
#endif // HAVE_LIBJPEG

  return 0;
}


//
// End of "$Id$".
//
