/
// "$Id $
/
// Carbon Pixmap drawing code for the Fast Light Tool Kit (FLTK)
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



#include "Fl_Carbon_Display.H

extern uchar **fl_mask_bitmap; // used by fl_draw_pixmap.cxx to store mas
//void fl_restore_clip(); // in fl_rect.cx


class Fl_Carbon_Pixmap_Cache: public Fl_Image_Cache
public
  void * id; // for internal us
  void * mask; // for internal use (mask bitmap
  Fl_Carbon_Pixmap_Cache(Fl_Image * im, Fl_Device * dev):Fl_Image_Cache(im,dev),mask(0){}
  ~Fl_Carbon_Pixmap_Cache()
     fl_delete_offscreen((Fl_Offscreen)id)
     if(mask) fl_delete_bitmask((Fl_Bitmask)mask)

}




void Fl_Carbon_Display::draw(Fl_Pixmap * img, int X, int Y, int W, int H, int cx, int cy) 

  Fl_Carbon_Pixmap_Cache *cache = (Fl_Carbon_Pixmap_Cache *) check_image_cache(img)
  if (!cache){ // building on
    cache = new Fl_Carbon_Pixmap_Cache(img,this)
    cache->id = fl_create_offscreen(img->w(), img->h())
    fl_begin_offscreen((Fl_Offscreen)(cache->id))
    uchar *bitmap = 0
    fl_mask_bitmap = &bitmap
    fl_draw_pixmap(img->data(), 0, 0, FL_BLACK)
    fl_mask_bitmap = 0
    if (bitmap) 
      cache->mask = fl_create_bitmask(img->w(), img->h(), bitmap)
      delete[] bitmap
    

    fl_end_offscreen()
  
 
  if (cache->mask) 
    Rect src, dst
    src.left = cx; src.right = cx+W
    src.top = cy; src.bottom = cy+H
    dst.left = X; dst.right = X+W
    dst.top = Y; dst.bottom = Y+H
    RGBColor rgb
    rgb.red = 0xffff; rgb.green = 0xffff; rgb.blue = 0xffff
    RGBBackColor(&rgb)
    rgb.red = 0x0000; rgb.green = 0x0000; rgb.blue = 0x0000
    RGBForeColor(&rgb)
    CopyMask(GetPortBitMapForCopyBits((GrafPtr)(cache->id))
	     GetPortBitMapForCopyBits((GrafPtr)(cache->mask))
	     GetPortBitMapForCopyBits(GetWindowPort(fl_window))
             &src, &src, &dst)
  } else 
    fl_copy_offscreen(X, Y, W, H, (Fl_Offscreen)(cache->id), cx, cy)
  







/
// End of "$Id$"
/
