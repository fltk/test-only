/
// "$Id$
/
// Image drawing code for the Fast Light Tool Kit (FLTK)
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

#include <config.h
#include "Fl_Win_Display.H


#if HAVE_ALPHABLEN
#  ifdef __MINGW32_
// MinGW headers does not declare AlphaBlend.
extern "C" 
  WINGDIAPI BOOL  WINAPI AlphaBlend(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION)

#  endi
#  if defined(_MSC_VER
#    pragma comment(lib, "msimg32.lib"
#  endi
#endi



class Fl_Win_RGB_Cache: public Fl_Image_Cache
public
  void * id; // for internal us
#if !HAVE_ALPHABLEN
  void * mask; // for internal use (mask bitmap)
#endi

  Fl_Win_RGB_Cache(Fl_Image * im, Fl_Device * dev):Fl_Image_Cache(im,dev),id(0

#if !HAVE_ALPHABLEN
    ,mask(0
#endi
  {}

  ~Fl_Win_RGB_Cache()
    fl_delete_offscreen((Fl_Offscreen)(id))
#if !HAVE_ALPHABLEN
    if (mask) fl_delete_bitmask((Fl_Bitmask)(mask))
#endi
  

}

#if HAVE_ALPHABLEN
extern int fl_draw_alpha
#endi



void Fl_Win_Display::draw(Fl_RGB_Image * img, int X, int Y, int W, int H, int cx, int cy) 
  Fl_Win_RGB_Cache *cache = (Fl_Win_RGB_Cache *) check_image_cache(img)
  if (!cache) { //building on
    cache = new Fl_Win_RGB_Cache(img,this)
    cache->id = fl_create_offscreen(img->w(), img->h())
#if HAVE_ALPHABLEN
    fl_draw_alpha = 1
#endi
    fl_begin_offscreen((Fl_Offscreen)(cache->id))
    fl_draw_image(img->array, 0, 0, img->w(), img->h(), img->d(), img->ld())
    fl_end_offscreen()
#if HAVE_ALPHABLEN
    fl_draw_alpha = 0
#endi
    if (img->d() == 2 || img->d() == 4) 

#if !HAVE_ALPHABLEN
       cache->mask = fl_create_alphamask(img->w(), img->h(), img->d(), img->ld(), img->array)
#endi
    
  
  if (img->d() == 2 || img->d() == 4) 
    HDC new_gc = CreateCompatibleDC(fl_gc)
#if HAVE_ALPHABLEN
    SelectObject(new_gc, (HGDIOBJ)cache->id);
    BLENDFUNCTION m_bf
    m_bf.BlendOp = AC_SRC_OVER
    m_bf.BlendFlags = 0
    m_bf.AlphaFormat = 0x1
    m_bf.SourceConstantAlpha = 0xFF
    AlphaBlend(fl_gc, X,Y,W,H, new_gc, cx, cy, W, H,m_bf);  
#els
    SelectObject(new_gc, (void*)cache->mask)
    BitBlt(fl_gc, X, Y, W, H, new_gc, cx, cy, SRCAND)
    SelectObject(new_gc, (void*)(cache->id))
    BitBlt(fl_gc, X, Y, W, H, new_gc, cx, cy, SRCPAINT)
#endi
    DeleteDC(new_gc)
  } else 
    fl_copy_offscreen(X, Y, W, H, (Fl_Offscreen)(cache->id), cx, cy)
  




/
// End of "$Id$"
/
