/
// "$Id$
/
// Image file header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-1999 by Bill Spitzak and others
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
// Please report all bugs and problems to "fltk-bugs@easysw.com"
/

#include <FL/Fl_Shaped_Window.H
#include <FL/x.H

#ifdef _WIN3
static HRGN bitmap2region(Fl_Bitmap*)
#elif (defined(__MACOS__) && !USE_X11
// Not yet implemented for Appl
#elif NANO_
// for microwindows too ;-
#elif DJGP
// for microwindows too ;-
#els
#include <X11/extensions/shape.h
#endi

// maybe one day we'll want to be able to resize the clip mas
// when the window resize
static Fl_Bitmap* resize_bitmap(Fl_Bitmap*, int, int)

void Fl_Shaped_Window::draw() 
  if ((lw != w() || lh != h() || changed) && shape_) 
    // size of window has change since last tim
    lw = w(); lh = h()
    Fl_Bitmap* mask = resize_bitmap(shape_, w(), h())
#ifdef _WIN3
    HRGN region = bitmap2region(mask)
    SetWindowRgn(fl_xid(this), region, TRUE)
#elif (defined(__MACOS__) && !USE_X11
    // not yet implemented for Appl
#elif NANO_
#elif DJGP
#els
    Pixmap pmask = XCreateBitmapFromData(fl_display, fl_xid(this)
                  (const char*)mask->array, mask->w(), mask->h())
    hide()
    XShapeCombineMask(fl_display, fl_xid(this), ShapeBounding, 0, 0
                      pmask, ShapeSet)
    show()
    if (pmask != None) XFreePixmap(fl_display, pmask)
#endi
    changed = 0
  
  Fl_Window::draw()


// maybe one day we'll want to be able to resize the clip mas
// bitmap when the window is resize
static Fl_Bitmap* resize_bitmap(Fl_Bitmap* bitmap, int /*W*/, int /*H*/) 
  return bitmap; // CET - FIXME - someday..


#ifdef _WIN3
#include <malloc.h
static inline BYTE bit(int x) { return (BYTE)(1 << (x % 8)); 

// Windows uses regions only to specify the clip mask of a window therefor
// we must convert our bitmap to a region
// Much of this code is "borrowed" from the Windows version of GT
// (also LGPLed).  Their code was based on code originally written b
// Jean-Edouard Lachand-Robert.  Ain't open source great
/
// Modified by me to use an Fl_Bitmap, to not hog memory, to not leak memor
// (I hope) and to allow bitmaps of arbitrary dimensions. -CE
static HRGN bitmap2region(Fl_Bitmap* bitmap) 
  HRGN hRgn = 0
  /* For better performances, we will use the ExtCreateRegion(
   * function to create the region. This function take a RGNDAT
   * structure on entry. We will add rectangles by amount o
   * ALLOC_UNIT number in this structure
   *
  #define ALLOC_UNIT  10
  DWORD maxRects = ALLOC_UNIT

  RGNDATA* pData = (RGNDATA*)malloc(sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects))
  pData->rdh.dwSize = sizeof(RGNDATAHEADER)
  pData->rdh.iType = RDH_RECTANGLES
  pData->rdh.nCount = pData->rdh.nRgnSize = 0
  SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0)

  const int bpl = (bitmap->w()+7)/8; // number of bytes per line of pixel
  BYTE* p8 = (BYTE*)bitmap->array
  BYTE* p
  for (int y = 0; y < bitmap->h(); y++) 
    /* Scan each bitmap row from left to right*
    for (int x = 0; x < bitmap->w(); x++) 
      /* Search for a continuous range of "non transparent pixels"*
      int x0 = x
      while (x < bitmap->w()) 
        p = p8 + x / 8
        if (!((*p) & bit(x))) break; /* This pixel is "transparent"*
        x++
      

      if (x > x0) 
        RECT* pr
        /* Add the pixels (x0, y) to (x, y+1) as a new rectangl
         * in the regio
         *
        if (pData->rdh.nCount >= maxRects) 
          maxRects += ALLOC_UNIT
          pData = (RGNDATA*)realloc(pData, sizeof(RGNDATAHEADER
                                    + (sizeof(RECT)*maxRects))
    	
        pr = (RECT*)&pData->Buffer
        SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1)
        if (x0 < pData->rdh.rcBound.left
          pData->rdh.rcBound.left = x0
        if (y < pData->rdh.rcBound.top
          pData->rdh.rcBound.top = y
        if (x > pData->rdh.rcBound.right
          pData->rdh.rcBound.right = x
        if (y+1 > pData->rdh.rcBound.bottom
          pData->rdh.rcBound.bottom = y+1
        pData->rdh.nCount++

        /* On Windows98, ExtCreateRegion() may fail if th
         * number of rectangles is too large (ie: 
         * 4000). Therefore, we have to create the region b
         * multiple steps
         *
        if (pData->rdh.nCount == 2000) 
          HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER
                                   + (sizeof(RECT)*maxRects), pData)
          if (hRgn) 
            CombineRgn(hRgn, hRgn, h, RGN_OR)
            DeleteObject(h)
          } else hRgn = h
          pData->rdh.nCount = 0
          SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0)
        
      
    

    /* Go to next row *
    p8 += bpl
  

  /* Create or extend the region with the remaining rectangles*
  HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER
		      + (sizeof(RECT)*maxRects), pData)
  if (hRgn) 
    CombineRgn(hRgn, hRgn, h, RGN_OR)
    DeleteObject(h)
  } else hRgn = h

  free(pData); // I've created the region so I can free this now, right

  return hRgn


#endi

/
// End of "$Id$
/
