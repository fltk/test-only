//
// "$Id: Fl_Image.cxx,v 1.34 2004/01/06 06:43:02 spitzak Exp $"
//
// Image drawing code for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Image.h>
#include <fltk/Widget.h>
#include <fltk/events.h>
#include <fltk/draw.h>
#include <fltk/x.h>

using namespace fltk;

/*! \class fltk::Image

  \brief Cached constant image with optional alpha

  This subclass of Symbol draws a very common thing: a fixed-size
  bitmapped color image with alpha information. The base class
  provides support for caching a rendered version of the image,
  and a function to draw this cached version on the screen.

  The subclasses are required to provide a _draw() method that
  fills in this cache if possible and then calls the base class
  method to draw it. The subclasses are also required to either
  fill in w_ and h_ in the constructor, or provide a _measure()
  method that calculates them.

*/

void fl_restore_clip(); // in rect.C

/** Draw the cached image after _draw() creates it.

    _draw() methods should call this after they have filled in the
    w_, h_, mask, and id fields.
*/
void Image::draw_cache(int Xi, int Yi, int Wi, int Hi, const Style* style, Flags flags) const
{

  // cx,cy = coordinate in image of upper-left pixel to draw
  // X,Y,W,H = box to fill with the image
  if (w()< Wi) Wi = w();
  if (h()< Hi) Hi = h();
  int X,Y,W,H; clip_box(Xi,Yi,Wi,Hi,X,Y,W,H);
  int cx = X-Xi;
  int cy = Y-Yi;
  if (W <= 0 || H <= 0) return;
  // convert to Xlib coordinates:
  transform(X,Y);

  if (mask) {
    if (id) {
      // both color and mask:
#if USE_X11
      // I can't figure out how to combine a mask with existing region,
      // so the mask replaces the region instead. This can draw some of
      // the image outside the current clip region if it is not rectangular.
      XSetClipMask(xdisplay, gc, (Pixmap)mask);
      int ox = X-cx; if (ox < 0) ox += w();
      int oy = Y-cy; if (oy < 0) oy += h();
      XSetClipOrigin(xdisplay, gc, X-cx, Y-cy);
      fl_copy_offscreen(X, Y, W, H, (Pixmap)id, cx, cy);
      // put the old clip region back
      XSetClipOrigin(xdisplay, gc, 0, 0);
      fl_restore_clip();
#elif defined(_WIN32)
# if 0
      HDC new_gc = CreateCompatibleDC(gc);
      SelectObject(new_gc, mask);
      BitBlt(gc, X, Y, W, H, new_gc, cx, cy, SRCAND);
      SelectObject(new_gc, id);
      BitBlt(gc, X, Y, W, H, new_gc, cx, cy, SRCPAINT);
      DeleteDC(new_gc);
# else
      // VP : new code to draw masked image under windows. Maybe not optimal, but works for win2k/95 and probably 98
      setcolor(0);
      setbrush();
      SetTextColor(gc, 0);
      HDC new_gc = CreateCompatibleDC(gc);
      HDC new_gc2 = CreateCompatibleDC(gc);
      SelectObject(new_gc, mask);
      SelectObject(new_gc2, id);
      BitBlt(new_gc2, 0,0,w(),h(), new_gc, 0, 0, SRCAND); // This should be done only once for performance
      // secret bitblt code found in old MSWindows reference manual:
      BitBlt(gc, X, Y, W, H, new_gc, cx, cy, 0xE20746L);
      BitBlt(gc, X, Y, W, H, new_gc2, cx, cy, SRCPAINT);
      DeleteDC(new_gc);
      DeleteDC(new_gc2);
# endif
#elif defined(__APPLE__)
      // OSX version nyi
#else
#error
#endif
    } else {
      Color bg, fg; style->boxcolors(flags, bg, fg);
      setcolor(fg);
      fill(X, Y, W, H, cx, cy);
    }
  } else if (id) {
    // pix only, no mask
    fl_copy_offscreen(X, Y, W, H, (Pixmap)id, cx, cy);
  } // else { no mask or id, probably an error... }
}

/** Draw the cached alpha channel filled with a solid color.

    This draws the cached mask image, filled with the current color.
    It is clipped to the \a XYWH rectangle, and the pixel \a cx,cy
    is placed in the upper-left corner of this rectangle.

    This is used internally by the draw_cache method to draw
    inactive images. It may also be used by subclasses such
    as xbmImage to implement solid color fill.
*/
void Image::fill(int X, int Y, int W, int H, int cx, int cy) const
{
#if USE_X11
  XSetStipple(xdisplay, gc, (Pixmap)mask);
  int ox = X-cx; if (ox < 0) ox += w();
  int oy = Y-cy; if (oy < 0) oy += h();
  XSetTSOrigin(xdisplay, gc, ox, oy);
  XSetFillStyle(xdisplay, gc, FillStippled);
  XFillRectangle(xdisplay, xwindow, gc, X, Y, W, H);
  XSetFillStyle(xdisplay, gc, FillSolid);
#elif defined(_WIN32)
  HDC tempdc = CreateCompatibleDC(gc);
  SelectObject(tempdc, (HGDIOBJ)mask);
  SetTextColor(gc, 0); // VP : seems necessary at least under win95
  setbrush();
  //SelectObject(gc, brush);
  // secret bitblt code found in old MSWindows reference manual:
  BitBlt(gc, X, Y, W, H, tempdc, cx, cy, 0xE20746L);
  DeleteDC(tempdc);
#elif defined(__APPLE__)
  // OSX version nyi
#else
#error
#endif
}

/*! By default Image assummes the constructor set the w_ and h_
  fields, and returns them.

  For many subclasses (such as ones that read a file!) you certainly
  want to defer this calculation until first use. The way to do this
  is to put zero into the w_ and h_ in the constructor, and override this
  method with your own which calculates the values and sets them if
  it has not done so already.
*/
void Image::_measure(float& W, float& H) const { W=w(); H=h(); }

/*! Get rid of the cached image (the mask and id objects) that
  were created by _draw(). */
void Image::destroy_cache() {
#if USE_X11
  if (mask) XFreePixmap(xdisplay, (Pixmap)mask);
  if (id) fl_delete_offscreen((Pixmap)id);
#elif defined(_WIN32)
  if (mask) DeleteObject((Pixmap)mask);
  if (id) fl_delete_offscreen((Pixmap)id);
#elif defined(__APPLE__)
  if (mask) DisposeGWorld((Pixmap)mask);
  if (id) fl_delete_offscreen((Pixmap)id);
#else
#error
#endif
  mask = 0;
  id = 0;
}

/*! The destructor calls destroy_cache(). */
Image::~Image() {
  destroy_cache();
}

#include <fltk/Widget.h>

/*! This is a 1.1 back-compatability function. It is the same as
  doing widget->image(this) and widget->label(0).
*/
void Image::label(Widget* o) {
  o->image(this);
  o->label(0);
}

//
// End of "$Id: Fl_Image.cxx,v 1.34 2004/01/06 06:43:02 spitzak Exp $".
//
