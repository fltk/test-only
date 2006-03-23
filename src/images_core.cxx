//
// "$Id: fl_images_core.cxx 4288 2005-04-16 00:13:17Z mike $"
//
// FLTK images library core.
//
// Copyright 1997-2005 by Easy Software Products.
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
//   fl_register_images() - Register the image formats.
//   fl_check_images()    - Check for a supported image format.
//

//
// Include necessary header files...
//

#include <fltk/SharedImage.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace fltk;

//
// Define a simple global image registration function that registers
// the extra image formats that aren't part of the core FLTK library.
//

static SharedImage	*fltk_check_images(const char *name, uchar *header, int headerlen);


static bool mInitialised=false;
//! register_images_ext() - Register the image formats extensions.
FL_API void fltk::register_images() {
  if(mInitialised) return ;
  SharedImage::add_handler(fltk_check_images);
  mInitialised = true;
}

//! unregister_images_ext() - unregister the image formats extensions.
FL_API void fltk::unregister_images() {
  if (!mInitialised) return;
  SharedImage::add_handler(fltk_check_images);
  mInitialised = false;
}


//
// 'fl_check_images()' - Check for a supported image format.
//

SharedImage *					// O - Image, if found
fltk_check_images(const char *name,		// I - Filename
                uchar      *header,		// I - Header data from file
		int        headerlen) {		// I - Amount of data
  if (memcmp(header, "/* XPM */", 9) == 0) // XPM file
      return xpmFileImage::get(name);
/*
  if (header[0] == 'P' && header[1] >= '1' && header[1] <= '7')
					// Portable anymap
    return new Fl_PNM_Image(name);
*/

  if (memcmp(header, "\211PNG", 4) == 0)// PNG file
    return pngImage::get(name);
  if (memcmp(header, "\377\330\377", 3) == 0 &&
					// Start-of-Image
      header[3] >= 0xe0 && header[3] <= 0xef)
	   				// APPn for JPEG file
    return jpegImage::get(name);

  return 0;
}


//
// End of "$Id: fl_images_core.cxx 4288 2005-04-16 00:13:17Z mike $".
//
