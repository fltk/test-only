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

#ifndef Fl_Image_File_H
#define Fl_Image_File_H

#include "fl_draw.H"
#include "fl_draw_image_file.H"
#include "Fl_Image.H"

class Fl_Widget;
struct Fl_Menu_Item;

struct Fl_Image_File : public Fl_Image {
  // to be removed
/*#ifdef WIN32
  Fl_Color transparent;
#endif  */
  fl_image_file_struct *ifs;

  Fl_Image_File(char* f, Fl_Image_File_Measure_F *m, Fl_Image_File_Read_F *r, 
		unsigned char *datas=NULL);
  ~Fl_Image_File();
  void draw(int X, int Y, int W, int H, int cx=0, int cy=0);
  void measure(int& W, int& H);
};

struct Fl_PNG_Image : Fl_Image_File {
  Fl_PNG_Image(char* f, unsigned char *datas=NULL) : 
    Fl_Image_File(f, fl_measure_png, fl_read_png, datas) { }
};

struct Fl_GIF_Image : Fl_Image_File {
  Fl_GIF_Image(char* f, unsigned char *datas=NULL) : 
    Fl_Image_File(f, fl_measure_gif, fl_read_gif, datas) { }
};

struct Fl_XPM_Image : Fl_Image_File {
  Fl_XPM_Image(char* f, unsigned char *datas=NULL) : 
    Fl_Image_File(f, fl_measure_xpm, fl_read_xpm, datas) { }
};

#endif
