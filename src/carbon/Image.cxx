/
// "$Id$
/
// Carbon image drawing code for the Fast Light Tool Kit (FLTK)
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

 
class Fl_Carbon_RGB_Cache: public Fl_Image_Cache
public
  void *id; // for internal us
  void *mask; // for internal use (mask bitmap
  Fl_Carbon_RGB_Cache(Fl_Image * im, Fl_Device * dev):Fl_Image_Cache(im,dev),id(0),mask(0){}
  ~Fl_Carbon_RGB_Cache()
    fl_delete_offscreen((Fl_Offscreen)(id))
    if (mask) fl_delete_bitmask((Fl_Bitmask)(mask))
  
}




void Fl_Carbon_Display::draw(Fl_RGB_Image * img, int X, int Y, int W, int H, int cx, int cy) 
  Fl_Carbon_RGB_Cache *cache = (Fl_Carbon_RGB_Cache *) check_image_cache(img)
  if (!cache) { //building on
    cache = new Fl_Carbon_RGB_Cache(img,this)
    cache->id = fl_create_offscreen(img->w(), img->h())
    fl_begin_offscreen((Fl_Offscreen)(cache->id))
    fl_draw_image(img->array, 0, 0, img->w(), img->h(), img->d(), img->ld())
    fl_end_offscreen()
    if (img->d() == 2 || img->d() == 4) 
      cache->mask = fl_create_alphamask(img->w(), img->h(), img->d(), img->ld(), img->array)
    
  
 
 
  if (cache->mask) 
    Rect src, dst
    src.left = 0; src.right = W
    src.top = 0; src.bottom = H
    dst.left = X; dst.right = X+W
    dst.top = Y; dst.bottom = Y+H
    RGBColor img
    img.red = 0xffff; img.green = 0xffff; img.blue = 0xffff
    RGBBackColor(&img)
    img.red = 0x0000; img.green = 0x0000; img.blue = 0x0000
    RGBForeColor(&img)

#if 
    // MRS: This *should* work, but doesn't on my system (iBook); change t
    //      "#if 1" and restore the corresponding code in Fl_Bitmap.cx
    //      to test the real alpha channel support
    CopyDeepMask(GetPortBitMapForCopyBits((GrafPtr)img->id)
	         GetPortBitMapForCopyBits((GrafPtr)img->mask),
	         GetPortBitMapForCopyBits(GetWindowPort(fl_window))
                 &src, &src, &dst, blend, NULL)
#else // Fallback to screen-door transparency..
    CopyMask(GetPortBitMapForCopyBits((GrafPtr)(cache->id))
	     GetPortBitMapForCopyBits((GrafPtr)(cache->mask)),
	     GetPortBitMapForCopyBits(GetWindowPort(fl_window))
             &src, &src, &dst)
#endif // 
  } else 
    fl_copy_offscreen(X, Y, W, H, (Fl_Offscreen)(cache->id), cx, cy)
  





/
// End of "$Id$"
/
