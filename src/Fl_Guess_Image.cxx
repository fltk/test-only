//
// "$Id: Fl_Guess_Image.cxx,v 1.2 1999/10/22 03:11:20 vincent Exp $"
//
// Guessing image type code for the Fast Light Tool Kit (FLTK).
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

// Draw an image that is stored compressed in a file or in memory. 
// Keep uncompressed images in memory for later use. 

#include <config.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Bitmap.H>
#include <FL/x.H>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bitmap used when we couldn't guess the filetype
#define nosuch_width 16
#define nosuch_height 16
static unsigned char nosuch_bits[] = {
   0xff, 0xf0, 0x81, 0x88, 0xd5, 0x90, 0x69, 0xa8, 0x55, 0x94, 0x69, 0xaa,
   0x55, 0x94, 0x69, 0xaa, 0xd5, 0x94, 0xa9, 0xa8, 0x55, 0x95, 0xa9, 0xa9,
   0x55, 0x95, 0xa9, 0xab, 0x01, 0x81, 0xff, 0xff};
Fl_Bitmap nosuch_bitmap(nosuch_bits, nosuch_width, nosuch_height);

extern char *fl_shared_image_root;

struct Fl_Image_Type fl_image_filetypes[] = {
  { "XPM", Fl_XPM_Image::test, Fl_XPM_Image::get},
  { "GIF", Fl_GIF_Image::test, Fl_GIF_Image::get},
  { "PNG", Fl_PNG_Image::test, Fl_PNG_Image::get},
  { "BMP", Fl_BMP_Image::test, Fl_BMP_Image::get},
  { "JPEG", Fl_JPEG_Image::test, Fl_JPEG_Image::get},
  { 0, Fl_UNKNOWN_Image::test, Fl_UNKNOWN_Image::get }
};

Fl_Image_Type* Fl_Shared_Image::guess(char* name, unsigned char *datas)
{
  int loaded = 0;
  size_t size = 1024;
  if (!datas) {
    datas = new uchar[1025];
    datas[1024] = 0; // null-terminate so strstr() works
    char s[1024];
    if (name[0] == '/') strcpy(s, name);
    else {
      if(!fl_shared_image_root) fl_shared_image_root=strdup("");
      strcpy(s, fl_shared_image_root);
      strcat(s, name);
    }
    FILE* fp = fopen(s, "rb");
    if (!fp)
      return fl_image_filetypes + 
	sizeof(fl_image_filetypes)/sizeof(fl_image_filetypes[0]) - 1;
    size = fread(datas, 1, size, fp);
    fclose(fp);
    loaded = 1;
  }
  Fl_Image_Type* ft;
  for (ft=fl_image_filetypes; ft->name; ft++)
    if (ft->test(datas, size)) break;
  if(loaded) free(datas);
  return ft;
}

//
// End of "$Id: Fl_Guess_Image.cxx,v 1.2 1999/10/22 03:11:20 vincent Exp $"
//
