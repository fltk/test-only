//
// "$Id: Fl_Shaped_Window.cxx,v 1.7 2001/08/07 07:06:17 spitzak Exp $"
//
// Image file header file for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl_Shaped_Window.h>
#include <fltk/x.h>

#ifdef _WIN32
#include <malloc.h>
static HRGN bitmap2region(Fl_Bitmap*);
#else
#include <X11/extensions/shape.h>
#endif

// maybe one day we'll want to be able to resize the clip mask
// when the window resized
static Fl_Bitmap* resize_bitmap(Fl_Bitmap*, int, int);

void Fl_Shaped_Window::draw() {
  if ((lw != w() || lh != h() || changed) && shape_) {
    // size of window has change since last time
    lw = w(); lh = h();
    Fl_Bitmap* mask = resize_bitmap(shape_, w(), h());
#ifndef _WIN32
    Pixmap pmask = XCreateBitmapFromData(fl_display, fl_xid(this),
                  (const char*)mask->array, mask->width(), mask->height());
    hide();
    XShapeCombineMask(fl_display, fl_xid(this), ShapeBounding, 0, 0,
                      pmask, ShapeSet);
    show();
    if (pmask != None) XFreePixmap(fl_display, pmask);
#else
    HRGN region = bitmap2region(mask);
    SetWindowRgn(fl_xid(this), region, TRUE);
#endif
    changed = 0;
  }
  Fl_Double_Window::draw();
}

// maybe one day we'll want to be able to resize the clip mask
// bitmap when the window is resized
static Fl_Bitmap* resize_bitmap(Fl_Bitmap* bitmap, int /*W*/, int /*H*/) {
  return bitmap; // CET - FIXME - someday...
}

#ifdef _WIN32
static inline BYTE bit(int x) { return (BYTE)(1 << (x % 8)); }

// Windows uses regions only to specify the clip mask of a window therefore
// we must convert our bitmap to a region.
// Much of this code is "borrowed" from the Windows version of GTK
// (also LGPLed).  Their code was based on code originally written by
// Jean-Edouard Lachand-Robert.  Ain't open source great?
//
// Modified by me to use an Fl_Bitmap, to not hog memory, to not leak memory
// (I hope) and to allow bitmaps of arbitrary dimensions. -CET
static HRGN bitmap2region(Fl_Bitmap* bitmap) {
  HRGN hRgn = 0;
  /* For better performances, we will use the ExtCreateRegion()
   * function to create the region. This function take a RGNDATA
   * structure on entry. We will add rectangles by amount of
   * ALLOC_UNIT number in this structure.
   */
  #define ALLOC_UNIT  100
  DWORD maxRects = ALLOC_UNIT;

  RGNDATA* pData = (RGNDATA*)malloc(sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects));
  pData->rdh.dwSize = sizeof(RGNDATAHEADER);
  pData->rdh.iType = RDH_RECTANGLES;
  pData->rdh.nCount = pData->rdh.nRgnSize = 0;
  SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

  const int bpl = (bitmap->width()+7)/8; // number of bytes per line of pixels
  BYTE* p8 = (BYTE*)bitmap->array;
  BYTE* p;
  for (int y = 0; y < bitmap->height(); y++) {
    /* Scan each bitmap row from left to right*/
    for (int x = 0; x < bitmap->width(); x++) {
      /* Search for a continuous range of "non transparent pixels"*/
      int x0 = x;
      while (x < bitmap->width()) {
        p = p8 + x / 8;
        if (!((*p) & bit(x))) break; /* This pixel is "transparent"*/
        x++;
      }

      if (x > x0) {
        RECT* pr;
        /* Add the pixels (x0, y) to (x, y+1) as a new rectangle
         * in the region
         */
        if (pData->rdh.nCount >= maxRects) {
          maxRects += ALLOC_UNIT;
          pData = (RGNDATA*)realloc(pData, sizeof(RGNDATAHEADER)
                                    + (sizeof(RECT)*maxRects));
    	}
        pr = (RECT*)&pData->Buffer;
        SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
        if (x0 < pData->rdh.rcBound.left)
          pData->rdh.rcBound.left = x0;
        if (y < pData->rdh.rcBound.top)
          pData->rdh.rcBound.top = y;
        if (x > pData->rdh.rcBound.right)
          pData->rdh.rcBound.right = x;
        if (y+1 > pData->rdh.rcBound.bottom)
          pData->rdh.rcBound.bottom = y+1;
        pData->rdh.nCount++;

        /* On Windows98, ExtCreateRegion() may fail if the
         * number of rectangles is too large (ie: >
         * 4000). Therefore, we have to create the region by
         * multiple steps.
         */
        if (pData->rdh.nCount == 2000) {
          HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER)
                                   + (sizeof(RECT)*maxRects), pData);
          if (hRgn) {
            CombineRgn(hRgn, hRgn, h, RGN_OR);
            DeleteObject(h);
          } else hRgn = h;
          pData->rdh.nCount = 0;
          SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
        }
      }
    }

    /* Go to next row */
    p8 += bpl;
  }

  /* Create or extend the region with the remaining rectangles*/
  HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER)
		      + (sizeof(RECT)*maxRects), pData);
  if (hRgn) {
    CombineRgn(hRgn, hRgn, h, RGN_OR);
    DeleteObject(h);
  } else hRgn = h;

  free(pData); // I've created the region so I can free this now, right?

  return hRgn;

}
#endif

//
// End of "$Id: Fl_Shaped_Window.cxx,v 1.7 2001/08/07 07:06:17 spitzak Exp $"
//
