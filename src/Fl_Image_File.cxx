//
// Image drawing code for the Fast Light Tool Kit (FLTK).
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

// Draw an image that is stored in a file. 
// Keep images in memory for later use. 


#include <FL/Fl.H>
#include <FL/Fl_Image_File.H>
#include <FL/x.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>
#include <stdlib.h>
#include <string.h>

Fl_Image_File::Fl_Image_File(char* f,  Fl_Image_File_Measure_F *m,
                             Fl_Image_File_Read_F *r, unsigned char *d) { 
  ifs=fl_get_image_file_struct(f, m, r, d);
  w=ifs->w;
  h=ifs->h;
}

Fl_Image_File::~Fl_Image_File() { 
}

void Fl_Image_File::draw(int X, int Y, int W, int H, int cx,int cy) {
  if (ifs->w==0) return;
  ifs->prepare_image_file();
  ifs->_draw(X, Y, W, H, cx, cy);
}

void Fl_Image_File::measure(int& W, int& H) {
  W = ifs->w;
  H = ifs->h;
}
