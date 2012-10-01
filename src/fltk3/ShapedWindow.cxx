//
// "$Id"
//
// Window header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <config.h>
#define FLTK3_INTERNALS
#include <fltk3/x.h>
#undef FLTK3_INTERNALS
#include <fltk3/Window.h>
#include <fltk3/ShapedWindow.h>
#include <fltk3/Image.h>
#include <fltk3/RGBImage.h>
#include <fltk3/PostScript.h> // for fltk3::PostScriptGraphicsDriver::swap_byte()

fltk3::ShapedWindow::ShapedWindow(int w, int h, const char* l)
	: Window(w, h, l), shape_(0), lw(0), lh(0), changed(false) {
	  border(false);
#if defined(__APPLE__)
	  mask = NULL;
#endif
	}

fltk3::ShapedWindow::ShapedWindow(int x, int y, int w, int h, const char* l)
	: Window(x, y, w, h, l), shape_(0), lw(0), lh(0), changed(false) {
	  border(false);
#if defined(__APPLE__)
	  mask = NULL;
#endif
	}

fltk3::ShapedWindow::~ShapedWindow() {
  //delete shape_;
#if defined(__APPLE__)
  if (mask) {
    CGDataProviderRelease(CGImageGetDataProvider(mask));
    CGImageRelease(mask);
  }
#endif
}

#if defined(__APPLE__)
static void MyProviderReleaseData (void *info, const void *data, size_t size) {
  delete[] (char*)data;
}
#endif

void fltk3::ShapedWindow::shape(fltk3::Image* b) {
  shape_ = b;
  changed = true;
#if defined(__APPLE__)
  if (mask) {
    CGDataProviderRelease(CGImageGetDataProvider(mask));
    CGImageRelease(mask);
    mask = NULL;
  }
  if (shape_) {
    // reverse mask bits and also perform bitwise inversion of all bytes
    int bytes_per_row = (shape_->w() + 7)/8;
    char *p = (char*)shape_->data()[0];
    char *last = p + bytes_per_row * shape_->h();
    char *q = new char[bytes_per_row * shape_->h()];
    const char *from = q;
    while (p < last) {
      *q++ = fltk3::PostScriptGraphicsDriver::swap_byte(~*p++);
    }
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, from, bytes_per_row * shape_->h(), MyProviderReleaseData);
    mask = CGImageMaskCreate(shape_->w(), shape_->h(), 1, 1, bytes_per_row, provider, NULL, false);
  }
#endif
}

#if USE_X11
# include <X11/extensions/shape.h>
#elif defined(_WIN32)
// needed for VisualC2010
# include <malloc.h>
namespace {
  inline BYTE bit(int x) { return (BYTE)(1 << (x%8)); }

  HRGN bitmap2region(fltk3::Image* image) {
    HRGN hRgn = 0;
    /* Does this need to be dynamically determined, perhaps? */
    const int ALLOC_UNIT = 100;
    DWORD maxRects = ALLOC_UNIT;

    RGNDATA* pData = (RGNDATA*)malloc(sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects));
    pData->rdh.dwSize = sizeof(RGNDATAHEADER);
    pData->rdh.iType = RDH_RECTANGLES;
    pData->rdh.nCount = pData->rdh.nRgnSize = 0;
    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

    const int bytesPerLine = (image->w() + 7)/8;
    BYTE* p, *data = (BYTE*)image->data()[0];
    for (int y = 0; y < image->h(); y++) {
      // each row, left to right
      for (int x = 0; x < image->w(); x++) {
	int x0 = x;
	while (x < image->w()) {
	  p = data + x / 8;
	  if (!((*p) & bit(x))) break; // transparent pixel
	  x++;
	}
	if (x > x0) {
	  RECT *pr;
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
	    } else 
	      hRgn = h;
	    pData->rdh.nCount = 0;
	    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
	  }
	}
      }
      /* Go to next row */
      data += bytesPerLine;
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

}
#elif defined(__APPLE__)
#endif

namespace {
  /*! \todo Fix me so I do something useful for images with more than just an alpha channel */
  fltk3::Image* resize_bitmap(fltk3::Image* image, int w, int h) {
/*    if (image->count() == 1)
      return image->copy(w, h); */
    return image->copy(w, h);
  }

}

void fltk3::ShapedWindow::draw() {
  if ((lw != w() || lh != h() || changed) && shape_) {
    // size of window has changed since last time
    lw = w();
    lh = h();
#if !defined(__APPLE__)
    fltk3::Image* temp = resize_bitmap(shape_, lw, lh);
    fltk3::RGBImage* bitmap = new RGBImage((const uchar*)temp->data()[0], lw, lh, 4);
#endif
#if USE_X11
    ::Pixmap pbitmap = XCreateBitmapFromData(fl_display, fl_xid(this), 
    					(const char*)bitmap->array,
					bitmap->w(), bitmap->h());
    XShapeCombineMask(fl_display, fl_xid(this), ShapeBounding, 0, 0,
                      pbitmap, ShapeSet);
    if (pbitmap != None) XFreePixmap(fl_display, pbitmap);
    delete bitmap;
    delete temp;
#elif defined(_WIN32)
    HRGN region = bitmap2region(bitmap);
    SetWindowRgn(fl_xid(this), region, TRUE);
    delete bitmap;
    delete temp;
#elif defined(__APPLE__)
#else 
    // any other window managers that FLTK3 supports will be added here
#endif
    changed = 0;
  }
#if defined(__APPLE__) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
  if (mask && (CGContextClipToMask != NULL)) CGContextClipToMask(fl_gc, CGRectMake(0,0,w(),h()), mask); // requires Mac OS 10.4
  CGContextSaveGState(fl_gc);
#endif
  // I get the feeling something inside Window::draw() is what's causing the artefacting......
  Window::draw();
#if defined(__APPLE__) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
  CGContextRestoreGState(fl_gc);
#endif
}

//
// End of "$Id".
//
