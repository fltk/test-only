//
// "$Id$"
//
// Xlib bitmap drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include "Fl_Xlib_Display.H"

Fl_Bitmask fl_create_bitmask(int w, int h, const uchar *data) {
  return XCreateBitmapFromData(fl_display, fl_window, (const char *)data,
                               (w+7)&-8, h);
}

void fl_delete_bitmask(Fl_Bitmask bm) {
  fl_delete_offscreen((Fl_Offscreen)bm);
}

class Fl_Xlib_Bitmap_Cache: public Fl_Image_Cache{
public:
  unsigned id; // for internal use
  Fl_Xlib_Bitmap_Cache(Fl_Image * im, Fl_Device * dev):Fl_Image_Cache(im,dev){};
  ~Fl_Xlib_Bitmap_Cache(){fl_delete_bitmask((Fl_Offscreen)(id));}
};



void Fl_Xlib_Display::draw(Fl_Bitmap * img, int X, int Y, int W, int H, int cx, int cy) {

  Fl_Xlib_Bitmap_Cache *cache = (Fl_Xlib_Bitmap_Cache *) check_image_cache(img);
 
  if (!cache){ // building one)
    cache = new Fl_Xlib_Bitmap_Cache(img,this);
    cache->id = fl_create_bitmask(img->w(), img->h(), img->array);
  }
  XSetStipple(fl_display, fl_gc, cache->id);
  int ox = X-cx; if (ox < 0) ox += img->w();
  int oy = Y-cy; if (oy < 0) oy += img->h();
  XSetTSOrigin(fl_display, fl_gc, ox, oy);
  XSetFillStyle(fl_display, fl_gc, FillStippled);
  XFillRectangle(fl_display, fl_window, fl_gc, X, Y, W, H);
  XSetFillStyle(fl_display, fl_gc, FillSolid);
}




//
// End of "$Id$".
//
