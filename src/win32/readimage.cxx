#include <fltk/x.h>
#include <fltk/draw.h>

using namespace fltk;

uchar *				// O - Pixel buffer or NULL if failed
fltk::readimage(uchar *p,	// I - Pixel buffer or NULL to allocate
	PixelType type,		// Type of pixels to store (RGB and RGBA only now)
	const Rectangle& r	// area to read
) {
  int   X = r.x();
  int   Y = r.y();
  int   w = r.w();
  int   h = r.h();
  int	x, y;			// Looping vars
  int	d;			// Depth of image
  uchar	*ptr;			// Pointer in image data


  // Allocate the image data array as needed...
  d = type&3;

  if (!p) p = new uchar[w * h * d];

  // set the alpha to 1 everywhere.
  if (d > 3) memset(p, 0xff, w * h * d);

  // Grab all of the pixels in the image, one at a time...
  // MRS: there has to be a better way than this!
  for (y = 0, ptr = p; y < h; y ++) {
    for (x = 0; x < w; x ++, ptr += d) {
      COLORREF c = GetPixel(dc, X + x, Y + y);

      ptr[0] = c;
      c >>= 8;
      ptr[1] = c;
      c >>= 8;
      ptr[2] = c;
    }
  }

  return p;
}
