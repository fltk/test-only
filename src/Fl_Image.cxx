//
// "$Id: Fl_Image.cxx,v 1.29 2003/02/21 18:16:34 spitzak Exp $"
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

// WAS: I took out the indirection pointers as that will not work, because
// it bypasses the virtual draw function on Image. We either need to
// make the _draw() function able to print a pixmap+bitmap onto another
// device, or we need each Image subclass to be able to print onto
// a device.

void fl_restore_clip(); // in rect.C

// Most subclasses have draw() create the "mask" and "id" the first time
// they are called and then call this method to quickly get them onto the
// screen:

void Image::_draw(int XP, int YP, Flags)
{
  // Calculate X,Y,W,H which is the box we are going to fill, and
  // calculate cx, cy which is the coordinate in the image of X,Y:
  int X,Y,W,H; clip_box(XP,YP,w,h,X,Y,W,H);
  int cx = X-XP;
  int cy = Y-YP;
  // clip the box down to the size of image, quit if empty:
  //if (cx < 0) {W += cx; X -= cx; cx = 0;}
  if (cx+W > w) W = w-cx;
  if (W <= 0) return;
  //if (cy < 0) {H += cy; Y -= cy; cy = 0;}
  if (cy+H > h) H = h-cy;
  if (H <= 0) return;
  // convert to Xlib coordinates:
  transform(X,Y);

  if (mask) {
    if (id) {
      // both color and mask:
#ifdef _WIN32
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
      SetTextColor(gc, 0);
      HDC new_gc = CreateCompatibleDC(gc);
      HDC new_gc2 = CreateCompatibleDC(gc);
      SelectObject(new_gc, mask);
      SelectObject(new_gc2, id);
      BitBlt(new_gc2, 0, 0, w, h, new_gc, 0, 0, SRCAND); // This should be done only once for performance
      // secret bitblt code found in old MSWindows reference manual:
      BitBlt(gc, X, Y, W, H, new_gc, cx, cy, 0xE20746L);
      BitBlt(gc, X, Y, W, H, new_gc2, cx, cy, SRCPAINT);
      DeleteDC(new_gc);
      DeleteDC(new_gc2);
# endif
#elif (defined(__APPLE__) && !USE_X11)
      // OSX version nyi
#else
      // I can't figure out how to combine a mask with existing region,
      // so the mask replaces the region instead. This can draw some of
      // the image outside the current clip region if it is not rectangular.
      XSetClipMask(xdisplay, gc, (Pixmap)mask);
      int ox = X-cx; if (ox < 0) ox += w;
      int oy = Y-cy; if (oy < 0) oy += h;
      XSetClipOrigin(xdisplay, gc, X-cx, Y-cy);
      fl_copy_offscreen(X, Y, W, H, (Pixmap)id, cx, cy);
      // put the old clip region back
      XSetClipOrigin(xdisplay, gc, 0, 0);
      fl_restore_clip();
#endif
    } else {
      // mask only
#ifdef _WIN32
      HDC tempdc = CreateCompatibleDC(gc);
      SelectObject(tempdc, (HGDIOBJ)mask);
      SetTextColor(gc, 0); // VP : seems necessary at least under win95
      //SelectObject(gc, brush);
      // secret bitblt code found in old MSWindows reference manual:
      BitBlt(gc, X, Y, W, H, tempdc, cx, cy, 0xE20746L);
      DeleteDC(tempdc);
#elif (defined(__APPLE__) && !USE_X11)
      // OSX version nyi
#else
      XSetStipple(xdisplay, gc, (Pixmap)mask);
      int ox = X-cx; if (ox < 0) ox += w;
      int oy = Y-cy; if (oy < 0) oy += h;
      XSetTSOrigin(xdisplay, gc, ox, oy);
      XSetFillStyle(xdisplay, gc, FillStippled);
      XFillRectangle(xdisplay, xwindow, gc, X, Y, W, H);
      XSetFillStyle(xdisplay, gc, FillSolid);
#endif
    }
  } else if (id) {
    // pix only, no mask
    fl_copy_offscreen(X, Y, W, H, (Pixmap)id, cx, cy);
  } // else { no mask or id, probably an error... }
}

// The default version of measure returns constants from the structure
// that should have been set by a subclasses' constructor:
void Image::measure(int& W, int& H) { W=w; H=h; }

Image::~Image() {
  if (mask) delete_bitmap((Pixmap)mask);
  if (id) fl_delete_offscreen((Pixmap)id);
}

#include <fltk/Widget.h>
// this could probably be inline but they cause the above header
// files to be included by Image.h:

void Image::label(Widget* o) {
  o->image(this);
  o->label(0);
}

//
// End of "$Id: Fl_Image.cxx,v 1.29 2003/02/21 18:16:34 spitzak Exp $".
//
