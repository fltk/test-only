//
// PNG reading code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

//
// provide functions to measure and decompress PNG files

#include <FL/Fl_Image_File.H>
#include "config.h"
#if HAVE_PNG
#include <png.h>
#include <stdlib.h>

static png_bytep datas;

static void read_data_fn(png_structp png_ptr, png_bytep d, png_size_t length)
{
  memcpy(d, datas, length);
  datas += length;
}
#endif


bool fl_is_png(unsigned char *datas, size_t size)
{
#if !HAVE_PNG
  return 0;
#else
  return png_check_sig((png_byte*)datas, size);
#endif
}

int fl_measure_png(char *filename, uchar *pdatas, int &w, int &h)
{
#if !HAVE_PNG
  return w=0;
#else
  png_structp png_ptr;
  png_infop info_ptr;
  png_uint_32 width, height;
  int bit_depth, color_type;

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0,0,0);

  if (png_ptr == NULL)
    return w=0;
  info_ptr = png_create_info_struct(png_ptr);
  FILE *fp=0;
  if(pdatas)
  {
    datas=(png_bytep)pdatas;
    png_set_read_fn(png_ptr, NULL, read_data_fn);
  }
  else
     fp = fopen(filename, "rb");
  if (info_ptr == NULL || (datas == NULL && fp == NULL))
  {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return w=0;
  }

  if(pdatas)
  {
    if (png_sig_cmp(pdatas, (png_size_t)0, 8))
      goto error;
  }
  else
  {
    unsigned char buf[8];
    if (fread(buf, 1, 8, fp) != 8 || png_sig_cmp(buf, (png_size_t)0, 8))
      goto error;
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
  }

  if (setjmp(png_ptr->jmpbuf))
  {
  error:
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    if(fp) fclose(fp);
    return w=0;
  }

  png_read_info(png_ptr, info_ptr);

  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,&color_type,
	       NULL,NULL,NULL);

  w=width;
  h=height;

  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
  if(fp) fclose(fp);
  return w;
#endif
}

Fl_Offscreen fl_read_png(char *filename, uchar *pdatas, Fl_Offscreen &mask)
{
#if !HAVE_PNG
  return 0;
#else
  //  printf("reading '%s' ...\n", filename);
  png_structp png_ptr;
  png_infop info_ptr;
  png_uint_32 width, height;
  int bit_depth, color_type;

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0,0,0);

  if (png_ptr == NULL)
    return 0;
  info_ptr = png_create_info_struct(png_ptr);

  FILE *fp=0;
  if(pdatas)
  {
    datas=(png_bytep)pdatas;
    png_set_read_fn(png_ptr, datas, read_data_fn);
  }
  else
     fp = fopen(filename, "rb");
  if (info_ptr == NULL || (datas == NULL && fp == NULL))
  {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return 0;
  }

  unsigned char *buffer=0;
  png_bytep *row_pointers=0;

  if(pdatas)
  {
    if (png_sig_cmp(pdatas, (png_size_t)0, 8))
      goto error;
  }
  else
  {
    unsigned char buf[8];
    if (fread(buf, 1, 8, fp) != 8 || png_sig_cmp(buf, (png_size_t)0, 8))
      goto error;
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
  }

  if (setjmp(png_ptr->jmpbuf))
  {
  error:
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    if(buffer) free(buffer);
    if(row_pointers) delete[] row_pointers;
    if(fp) fclose(fp);
    return 0;
  }

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

  int d=3;
  if (color_type & PNG_COLOR_MASK_ALPHA)
    d=4; //    png_set_strip_alpha(png_ptr); 
  // png_set_strip_alpha doesn't seem to work ... too bad
 
  int  rowbytes = png_get_rowbytes(png_ptr, info_ptr);
  buffer=(unsigned char *) malloc(rowbytes*height);

  row_pointers = new png_bytep[height];
  for(png_uint_32 i=0; i<height; i++)
    row_pointers[i]=buffer+rowbytes*i;
  png_read_image(png_ptr, row_pointers);
  delete row_pointers;
  row_pointers=0;

  png_read_end(png_ptr, NULL);

  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
  if(fp) fclose(fp);

  Fl_Offscreen id = fl_create_offscreen(width, height);
  fl_begin_offscreen(id);
  fl_draw_image(buffer, 0, 0, width, height, d);
  fl_end_offscreen();
  mask = 0;

  free(buffer);

  return id;
#endif
}
