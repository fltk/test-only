/
// "$Id$
/
// WIN32 image reading routines for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
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

/
// 'fl_read_image()' - Read an image from the current window
/

uchar *				// O - Pixel buffer or NULL if faile
fl_read_image(uchar *p,		// I - Pixel buffer or NULL to allocat
              int   X,		// I - Left positio
	      int   Y,		// I - Top positio
	      int   w,		// I - Width of area to rea
	      int   h,		// I - Height of area to rea
	      int   alpha) {	// I - Alpha value for image (0 for none
  int	x, y;			// Looping var
  int	d;			// Depth of imag
  uchar	*ptr;			// Pointer in image dat


  // Allocate the image data array as needed..
  d = alpha ? 4 : 3

  if (!p) p = new uchar[w * h * d]

  // Initialize the default colors/alpha in the whole image..
  memset(p, alpha, w * h * d)

  // Grab all of the pixels in the image, one at a time..
  // MRS: there has to be a better way than this
  for (y = 0, ptr = p; y < h; y ++) 
    for (x = 0; x < w; x ++, ptr += d) 
      COLORREF c = GetPixel(fl_gc, X + x, Y + y)

      ptr[0] = c
      c >>= 8
      ptr[1] = c
      c >>= 8
      ptr[2] = c
    
  

  return p



/
// End of "$Id$"
/
