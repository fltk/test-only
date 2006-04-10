//
// "$Id$"
//
// PNG reading code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

//
// provides functions to measure and decompress PNG files

#include <config.h>

#if HAVE_LIBPNG

# ifdef HAVE_LOCAL_PNG_H
# include "libpng/png.h"
#elif defined(HAVE_PNG_H)
# include <png.h>
#elif defined(HAVE_LIBPNG_PNG_H)
# include <libpng/png.h>
#endif

# include <stdlib.h>

static png_bytep cur_datas;

static void read_data_fn(png_structp /*png_ptr*/,png_bytep d,png_size_t length)
{
  memcpy(d, cur_datas, length);
  cur_datas += length;
}

// Dummy function to remove gcc's nasty warning about longjmp:
static void declare_now(void*) { }
#endif

#include <fltk/x.h>
#include <fltk/error.h>
#include <fltk/SharedImage.h>

using namespace fltk;

bool pngImage::test(const uchar* datas, unsigned size)
{
#if !HAVE_LIBPNG
  return 0;
#else
  return png_check_sig((png_byte*)datas, (int)size)!=0;
#endif
}

void fltk::pngImage::_measure(int &W, int &H) const {

#if !HAVE_LIBPNG
  const_cast<pngImage*>(this)->setsize(0,0);
  W = H = 0;
#else
  if (w() >= 0) { 
    W = w(); H = h(); 
    return; 
  }

  png_structp png_ptr;
  png_infop info_ptr;
  png_uint_32 width, height;
  int bit_depth, color_type;

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0,0,0);

  if (png_ptr == NULL) {
    const_cast<pngImage*>(this)->setsize(0,0);
    W = H = 0;
    return;
  }
  info_ptr = png_create_info_struct(png_ptr);
  FILE *fp=0;
  declare_now(&fp);
  if(pixels())
  {
    cur_datas=(png_bytep)pixels();
    png_set_read_fn(png_ptr, NULL, read_data_fn);
  }
  else
     fp = fopen(get_filename(), "rb");
  if (info_ptr == NULL || (pixels()== NULL && fp == NULL))
  {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    const_cast<pngImage*>(this)->setsize(0,0);
    W = H = 0;
    return;
  }

  if(pixels())
  {
    if (png_sig_cmp((uchar*)pixels(), (png_size_t)0, 8))
      goto error;
  }
  else
  {
    uchar buf[8];
    if (fread(buf, 1, 8, fp) != 8 || png_sig_cmp(buf, (png_size_t)0, 8))
      goto error;
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
  }

  if (setjmp(png_ptr->jmpbuf))
  {
    goto error;
  }

  png_read_info(png_ptr, info_ptr);

  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,&color_type,
	       NULL,NULL,NULL);

  const_cast<pngImage*>(this)->setsize(width, height);
  W = width;
  H = height;

  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
  if(fp) fclose(fp);
  return;

 error:
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
  if(fp) fclose(fp);
  const_cast<pngImage*>(this)->setsize(0,0);
  W = H = 0;
  return;
#endif
}

#if HAVE_LIBPNG
static const uchar* drawimage_cb(void *v, int/*x*/, int/*y*/, int/*w*/, uchar* b)
{
  png_read_row((png_structp)v, b, NULL);
  return b;
}
#endif


void fltk::pngImage::read()
{
#if HAVE_LIBPNG
# if USE_PROGRESSIVE_DRAW
  //  printf("reading '%s' ...\n", filename);
  png_structp png_ptr;
  png_infop info_ptr;
  png_uint_32 width, height;
  int bit_depth, color_type;

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0,0,0);

  if (png_ptr == NULL) {
    return;
  }
  info_ptr = png_create_info_struct(png_ptr);

  FILE *fp=0;
  declare_now(&fp);
  if(pixels())
  {
    cur_datas=(png_bytep)pixels();
    png_set_read_fn(png_ptr, cur_datas, read_data_fn);
  }
  else
     fp = fopen(get_filename(), "rb");
  if (info_ptr == NULL || (pixels() == NULL && fp == NULL))
  {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return;
  }

  uchar* buffer=0;
  int d=3;
  declare_now(&d);

  if(pixels())
  {
    if (png_sig_cmp((uchar*)pixels(), (png_size_t)0, 8))
      goto error;
  }
  else
  {
    uchar buf[8];
    if (fread(buf, 1, 8, fp) != 8 || png_sig_cmp(buf, (png_size_t)0, 8))
      goto error;
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
  }

  if (setjmp(png_ptr->jmpbuf))
    goto error;

  png_read_info(png_ptr, info_ptr);

  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,&color_type,
	       NULL,NULL,NULL);

  if (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8) 
    png_set_expand(png_ptr);

  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) 
    png_set_expand(png_ptr);

/*  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) 
      png_set_expand(png_ptr);*/

  if (bit_depth == 16)
    png_set_strip_16(png_ptr);

  if (bit_depth < 8)
    png_set_packing(png_ptr);

  if (color_type == PNG_COLOR_TYPE_GRAY || 
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png_ptr);

/*  if (png_get_gAMA(png_ptr, info_ptr, &gamma))
    png_set_gamma(png_ptr, screen_gamma, gamma);
  else
    png_set_gamma(png_ptr, screen_gamma, 0.50);*/

  png_read_update_info(png_ptr, info_ptr);

  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,&color_type,
	       NULL,NULL,NULL);

  if (color_type & PNG_COLOR_MASK_ALPHA)
    d=4; //    png_set_strip_alpha(png_ptr); 
  // png_set_strip_alpha doesn't seem to work ... too bad
 
  { // We use a block because GSave creates a local
    // and we have 'goto error' before this point
    GSave gsave;
    make_current();
    drawimage(drawimage_cb, png_ptr, PixelType(d), Rectangle(width, height));
  }

  png_read_end(png_ptr, NULL);

 error:
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
  if (fp) fclose(fp);
  if (buffer) free(buffer);
# else
  fetch(); // reuse fetch code
  GSave gsave;
  make_current();
  drawimage(pixels(), pixel_type(), Rectangle(width(), height()));
# endif //USE_PROGRESSIVE_DRAW
#endif
}

//! fetch a pngImage in a pixel buffer and update imaage info consequently
bool pngImage::fetch() {
  int		i;			// Looping var
  FILE		*fp;			// File pointer
  int		channels;		// Number of color channels
  png_structp	pp;			// PNG read pointer
  png_infop	info;			// PNG info pointers
  png_bytep	*rows;			// PNG row pointers

  const char * png = get_filename();

  // Open the PNG file...
  if ((fp = fopen(png, "rb")) == NULL) return false;

  // Setup the PNG data structures...
  pp   = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info = png_create_info_struct(pp);

  if (setjmp(pp->jmpbuf))
  {
    fltk::warning("PNG file \"%s\" contains errors!\n", png);
    return false;
  }

  // Initialize the PNG read "engine"...
  png_init_io(pp, fp);

  // Get the image dimensions and convert to grayscale or RGB...
  png_read_info(pp, info);

  if (info->color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_expand(pp);

  if (info->color_type & PNG_COLOR_MASK_COLOR)
    channels = 3;
  else
    channels = 1;

  if ((info->color_type & PNG_COLOR_MASK_ALPHA) || info->num_trans)
    channels ++;

  w((int)(info->width));
  h((int)(info->height));
  pixel_type(channels ==4 ? ARGB32 : channels ==3 ? RGB : MONO );

  if (info->bit_depth < 8)
  {
    png_set_packing(pp);
    png_set_expand(pp);
  }
  else if (info->bit_depth == 16)
    png_set_strip_16(pp);

  // Handle transparency...
  if (png_get_valid(pp, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(pp);

  uchar * array = alloc_pixels(w(),h(),pixel_type()); // this unified array is automatically deallocated

  // Allocate pointers...
  rows = new png_bytep[h()];
  // initialize rows to point on the new allocated buffer
  for (i = 0; i < h(); i ++) rows[i] = (png_bytep)(array + i * ld());

  // Read the image, handling interlacing as needed...
  for (i = png_set_interlace_handling(pp); i > 0; i --)
    png_read_rows(pp, rows, NULL, h());

#ifdef WIN32
  // Some Windows graphics drivers don't honor transparency when RGB == white
  if (channels == 4) {
    // Convert RGB to 0 when alpha == 0...
    uchar *ptr = array;
    for (i = w() * h(); i > 0; i --, ptr += 4)
      if (!ptr[3]) ptr[0] = ptr[1] = ptr[2] = 0;
  }
#endif // WIN32

  // Free memory and return...
  delete[] rows;

  png_read_end(pp, info);
  png_destroy_read_struct(&pp, &info, NULL);

  fclose(fp);

  return true;
}

//
// End of "$Id$"
//
