//
// "$Id: fl_draw_image_file.H,v 1.4 1999/08/26 19:30:03 vincent Exp $"
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
#include <FL/Fl_Image.H>

// image file:
typedef int (Fl_Image_File_Measure_F)(char* filename, unsigned char* datas, 
				      int& x, int& y);
typedef Fl_Offscreen (Fl_Image_File_Read_F)(char* filename, 
		 unsigned char *datas, Fl_Offscreen& mask);

// The class fl_image_file_struct is used to maintain a binary tree of all
// images used in FLTK. There is one and only one fl_image_file_struct object
// per image, thus one should never try to create or destroy such an object
// by himself, use fl_get_image_file_struct instead !

class fl_image_file_struct : public Fl_Image {
  friend class Fl_Image_File;

protected:
  fl_image_file_struct() { }; // Constructor is private on purpose

  size_t count();
  static void check_mem_usage();

  fl_image_file_struct* l1;
  fl_image_file_struct* l2;
  char* filename;
  unsigned char* datas;
  unsigned int used;

  Fl_Image_File_Measure_F* measure;
  Fl_Image_File_Read_F* read;
  size_t mem; // Currently the memory taken by the image is calculted in unit 
              // pixel, and not in byte !

  void prepare_image_file();
  static void fl_image_file_struct::insert(fl_image_file_struct*& p, fl_image_file_struct* ifs);
  static fl_image_file_struct* fl_image_file_struct::find(fl_image_file_struct* ifs, char* name);


public:
  friend fl_image_file_struct* fl_get_image_file_struct(char* filename,  
        Fl_Image_File_Measure_F *measure,  Fl_Image_File_Read_F *read,
	unsigned char *datas=0);

  void draw(int X, int Y, int W, int H, 
	    int cx, int cy);
};

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
Fl_Offscreen fl_read_png(char *file, unsigned char *datas, Fl_Offscreen& mask);

bool fl_is_xpm(unsigned char *datas=0, size_t size=0);
int fl_measure_xpm(char *filename, uchar *pdatas, int &w, int &h);
Fl_Offscreen fl_read_xpm(char *filename, uchar *pdatas, Fl_Offscreen &mask);

bool fl_is_gif(unsigned char *datas=0, size_t size=0);
int fl_measure_gif(char *file,uchar* datas, int  &w, int &h);
Fl_Offscreen fl_read_gif(char *file, uchar* datas, Fl_Offscreen &mask);

bool fl_is_bmp(unsigned char *datas=0, size_t size=0);
int fl_measure_bmp(char *file,uchar* datas, int  &w, int &h);
Fl_Offscreen fl_read_bmp(char *file, uchar* datas, Fl_Offscreen &mask);

#endif

//
// End of "$Id: fl_draw_image_file.H,v 1.4 1999/08/26 19:30:03 vincent Exp $"
//
