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

Fl_Color fl_transparent_color;

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

  // clip the box down to the size of image, quit if empty:
  if (cx < 0) {W += cx; X -= cx; cx = 0;}
  if (cx+W > ifs->w) W = ifs->w-cx;
  if (W <= 0) return;
  if (cy < 0) {H += cy; Y -= cy; cy = 0;}
  if (cy+H > ifs->h) H = ifs->h-cy;
  if (H <= 0) return;

  fl_draw_image_file(ifs, X, Y, W, H, cx, cy);
}

void Fl_Image_File::measure(int& w, int& h) {
  w = ifs->w;
  h = ifs->h;
}

// to be removed ?
/*static void image_file_labeltype(
    const Fl_Label* o, int x, int y, int w, int h, Fl_Align a) {
  Fl_Image_File* b = (Fl_Image_File*)(o->value);
  if(b->ifs->w == 0) return;
  int cx;
  if (a & FL_ALIGN_LEFT) cx = 0;
  else if (a & FL_ALIGN_RIGHT) cx = b->ifs->w-w;
  else cx = (b->ifs->w-w)/2;
  int cy;
  if (a & FL_ALIGN_TOP) cy = 0;
  else if (a & FL_ALIGN_BOTTOM) cy = b->ifs->h-h;
  else cy = (b->ifs->h-h)/2;
  if(a & FL_ALIGN_TILED)
    b->draw_tiled(x, y, w, h, cx, cy);
  else
    b->draw(x,y,w,h,cx,cy);
}

static void image_file_measure(const Fl_Label* o, int& w, int& h) {
  Fl_Image_File* b = (Fl_Image_File*)(o->value);
#ifdef WIN32
  fl_transparent_color = b->transparent;
#endif
  b->measure(w, h);
}

void Fl_Image_File::label(Fl_Widget* o) {
#ifdef WIN32
  transparent = o->color();
#endif
  Fl::set_labeltype(_FL_IMAGE_FILE_LABEL, 
		    image_file_labeltype, image_file_measure);
  o->label(_FL_IMAGE_FILE_LABEL, (const char*)this);
}

void Fl_Image_File::label(Fl_Menu_Item* o) {
  Fl::set_labeltype(_FL_IMAGE_FILE_LABEL, 
		    image_file_labeltype, image_file_measure);
  o->label(_FL_IMAGE_FILE_LABEL, (const char*)this);
}
*/
