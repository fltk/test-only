//
// Image file header file for the Fast Light Tool Kit (FLTK).
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

#ifndef fl_draw_image_file_H
#define fl_draw_image_file_H

#include "x.H"

// image file:
typedef int (Fl_Image_File_Measure_F)(char* filename, unsigned char* datas, 
				      int& x, int& y);
typedef Fl_Offscreen (Fl_Image_File_Read_F)(char* filename, 
		 unsigned char *datas, Fl_Offscreen& mask);

struct fl_image_file_struct {
  fl_image_file_struct* l1;
  fl_image_file_struct* l2;
  char* filename;
  unsigned char* datas;
  int w, h;
  Fl_Offscreen id;
  Fl_Offscreen mask;
  unsigned int used;
  Fl_Image_File_Measure_F* measure;
  Fl_Image_File_Read_F* read;
  size_t mem; // Currently the memory taken by the image is calculted in unit 
              // pixel, and not in byte ! (used to limit amount of memory used)
};

fl_image_file_struct* fl_get_image_file_struct(char* filename,  
        Fl_Image_File_Measure_F *measure,  Fl_Image_File_Read_F *read,
	unsigned char *datas=0);
void fl_draw_image_file(fl_image_file_struct *ifs, int X, int Y, int W, int H, 
			int cx, int cy);
void fl_set_images_root_directory(char *d);
void fl_set_image_file_mem_usage_limit(size_t l);


/* Specific image format functions. Add you own file format here. */

// Check the given buffer if is is in the given file format
bool fl_is_png(unsigned char *datas=0, size_t size=0);

// Return the width and heigth of the given image.
// If datas == 0, then get datas from the given file name
int fl_measure_png(char *file, unsigned char *datas, int &w, int &h);

// Create the image in OS dependent format from the given datas
// If datas == 0, then get datas from the given file name.
// In any case the file name need to be provided  for image cache 
// handling purposes.
Fl_Offscreen fl_read_png(char *file, unsigned char *datas, Fl_Offscreen& mask);

bool fl_is_xpm(unsigned char *datas=0, size_t size=0);
int fl_measure_xpm(char *filename, uchar *pdatas, int &w, int &h);
Fl_Offscreen fl_read_xpm(char *filename, uchar *pdatas, Fl_Offscreen &mask);

bool fl_is_gif(unsigned char *datas=0, size_t size=0);
int fl_measure_gif(char *file,uchar* datas, int  &w, int &h);
Fl_Offscreen fl_read_gif(char *file, uchar* datas, Fl_Offscreen &mask);

#endif


