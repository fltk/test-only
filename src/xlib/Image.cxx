/
// "$Id$
/
// Xlib image drawing code for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2003 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/


#include <FL/Fl_Image.H
#include "Fl_Xlib_Display.H



void fl_restore_clip(); // from fl_rect.cx

class Fl_Xlib_RGB_Cache: public Fl_Image_Cache
public
  unsigned id; // for internal us
  unsigned mask; // for internal use (mask bitmap)
  Fl_Xlib_RGB_Cache(Fl_Image * im, Fl_Device * dev):Fl_Image_Cache(im,dev),id(0),mask(0){}
  ~Fl_Xlib_RGB_Cache()
    fl_delete_offscreen((Fl_Offscreen)(id))
    if (mask) fl_delete_bitmask((Fl_Bitmask)(mask))
  
}


void Fl_Xlib_Display::draw(Fl_RGB_Image * img, int X, int Y, int W, int H, int cx, int cy) 
  Fl_Xlib_RGB_Cache *cache = (Fl_Xlib_RGB_Cache *) check_image_cache(img)
  if (!cache) { //building on
    cache = new Fl_Xlib_RGB_Cache(img,this)
    cache->id = fl_create_offscreen(img->w(), img->h())
    fl_begin_offscreen((Fl_Offscreen)(cache->id))
    fl_draw_image(img->array, 0, 0, img->w(), img->h(), img->d(), img->ld())
    fl_end_offscreen()
    if (img->d() == 2 || img->d() == 4) 
      cache->mask = fl_create_alphamask(img->w(), img->h(), img->d(), img->ld(), img->array)
    
  

  if (cache->mask) 
    // I can't figure out how to combine a mask with existing region
    // so cut the image down to a clipped rectangle
    int nx, ny; fl_clip_box(X,Y,W,H,nx,ny,W,H)
    cx += nx-X; X = nx
    cy += ny-Y; Y = ny
    // make X use the bitmap as a mask
    XSetClipMask(fl_display, fl_gc, cache->mask)
    int ox = X-cx; if (ox < 0) ox += img->w()
    int oy = Y-cy; if (oy < 0) oy += img->h()
    XSetClipOrigin(fl_display, fl_gc, X-cx, Y-cy)
  
  fl_copy_offscreen(X, Y, W, H, cache->id, cx, cy)
  if (cache->mask) 
    // put the old clip region bac
    XSetClipOrigin(fl_display, fl_gc, 0, 0)
    fl_restore_clip()
  





/
// End of "$Id$"
/
