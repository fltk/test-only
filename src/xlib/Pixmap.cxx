//
// "$Id$"
//
// Xlib Pixmap drawing code for the Fast Light Tool Kit (FLTK).
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

// Draws X pixmap data, keeping it stashed in a server pixmap so it
// redraws fast.

// See fl_draw_pixmap.cxx for code used to get the actual data into pixmap.
// Implemented without using the xpm library (which I can't use because
// it interferes with the color cube used by fl_draw_image).

#include "Fl_Xlib_Display.H"

extern uchar **fl_mask_bitmap; // used by fl_draw_pixmap.cxx to store mask
void fl_restore_clip(); // in fl_rect.cxx




class Fl_Xlib_Pixmap_Cache: public Fl_Image_Cache{
public:
  unsigned id; // for internal use
  unsigned mask; // for internal use (mask bitmap)
  Fl_Xlib_Pixmap_Cache(Fl_Image * im, Fl_Device * dev):Fl_Image_Cache(im,dev),mask(0){};
  ~Fl_Xlib_Pixmap_Cache(){
     fl_delete_offscreen((Fl_Offscreen)id);
     if(mask) fl_delete_bitmask((Fl_Bitmask)mask);
}
};


void Fl_Xlib_Display::draw(Fl_Pixmap * img, int X, int Y, int W, int H, int cx, int cy) {

  Fl_Xlib_Pixmap_Cache *cache = (Fl_Xlib_Pixmap_Cache *) check_image_cache(img);
  if (!cache){ // building one
    cache = new Fl_Xlib_Pixmap_Cache(img,this);
    cache->id = fl_create_offscreen(img->w(), img->h());
    fl_begin_offscreen((Fl_Offscreen)(cache->id));
    uchar *bitmap = 0;
    fl_mask_bitmap = &bitmap;
    fl_draw_pixmap(img->data(), 0, 0, FL_BLACK);
    fl_mask_bitmap = 0;
    if (bitmap) {
      cache->mask = fl_create_bitmask(img->w(), img->h(), bitmap);
      delete[] bitmap;
    }
    fl_end_offscreen();
  }

  if (cache->mask) {
    // I can't figure out how to combine a mask with existing region,
    // so cut the image down to a clipped rectangle:
    int nx, ny; fl_clip_box(X,Y,W,H,nx,ny,W,H);
    cx += nx-X; X = nx;
    cy += ny-Y; Y = ny;
    // make X use the bitmap as a mask:
    XSetClipMask(fl_display, fl_gc, cache->mask);
    int ox = X-cx; if (ox < 0) ox += img->w();
    int oy = Y-cy; if (oy < 0) oy += img->h();
    XSetClipOrigin(fl_display, fl_gc, X-cx, Y-cy);
  }
  fl_copy_offscreen(X, Y, W, H, cache->id, cx, cy);
  if (cache->mask) {
    // put the old clip region back
    XSetClipOrigin(fl_display, fl_gc, 0, 0);
    fl_restore_clip();
  }

}



//
// End of "$Id$".
//
