//
// "$Id: Fl_Image.cxx,v 1.15 2000/11/29 21:43:22 vincentp Exp $"
//
// Image drawing code for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include <FL/Fl_Image.H>

// tiled image with minimal redraw
void Fl_Image::draw_tiled(int x, int y, int pw, int ph, int cx, int cy) {
  if (w<0) measure(w, h);
  if (w==0) return;

  int X,Y,W,H; fl_clip_box(x, y, pw, ph, X, Y, W, H);
  if(W <= 0 || H <= 0) return;
  cx += X-x; cy += Y-y;

  int temp = -cx % w;
  cx = (temp>0 ? w : 0) - temp;
  temp = -cy % h;
  cy = (temp>0 ? h : 0) - temp;

  int ccx=cx;
  while(-cy<H) {
    while(-cx<W) {
      draw(X, Y, W, H, cx, cy);
      cx -= w;
    }
    cy -= h;
    cx = ccx;
  }
}
  
// Most subclasses have draw() create the "mask" and "id" and then
// call this method:

void fl_restore_clip(); // in fl_rect.C

void Fl_Image::_draw(int XP, int YP, int WP, int HP, int cx, int cy)
{
  // account for current clip region (faster on Irix):
  int X,Y,W,H; fl_clip_box(XP,YP,WP,HP,X,Y,W,H);
  cx += X-XP; cy += Y-YP;
  // clip the box down to the size of image, quit if empty:
  if (cx < 0) {W += cx; X -= cx; cx = 0;}
  if (cx+W > w) W = w-cx;
  if (W <= 0) return;
  if (cy < 0) {H += cy; Y -= cy; cy = 0;}
  if (cy+H > h) H = h-cy;
  if (H <= 0) return;
  if (mask) {
    if (id) {
      // both color and mask:
#ifdef WIN32
# if 0
      HDC new_gc = CreateCompatibleDC(fl_gc);
      SelectObject(new_gc, (void*)mask);
      BitBlt(fl_gc, X, Y, W, H, new_gc, cx, cy, SRCAND);
      SelectObject(new_gc, (void*)id);
      BitBlt(fl_gc, X, Y, W, H, new_gc, cx, cy, SRCPAINT);
      DeleteDC(new_gc);
# else
      // VP : new code to draw masked image under windows. Maybe not optimal, but works for win2k/95 and probably 98
      fl_color(0);
      SetTextColor(fl_gc, 0);
      HDC new_gc = CreateCompatibleDC(fl_gc);
      HDC new_gc2 = CreateCompatibleDC(fl_gc);
      SelectObject(new_gc, (void*)mask);
      SelectObject(new_gc2, (void*)id);
      BitBlt(new_gc2, 0, 0, w, h, new_gc, 0, 0, SRCAND); // This should be done only once for performance
      // secret bitblt code found in old MSWindows reference manual:
      BitBlt(fl_gc, X, Y, W, H, new_gc, cx, cy, 0xE20746L);
      BitBlt(fl_gc, X, Y, W, H, new_gc2, cx, cy, SRCPAINT);
      DeleteDC(new_gc);
      DeleteDC(new_gc2);
# endif
#else
      // I can't figure out how to combine a mask with existing region,
      // so cut the image down to a clipped rectangle:
      int nx, ny; fl_clip_box(X,Y,W,H,nx,ny,W,H);
      cx += nx-X; X = nx;
      cy += ny-Y; Y = ny;
      // make X use the bitmap as a mask:
      XSetClipMask(fl_display, fl_gc, mask);
      int ox = X-cx; if (ox < 0) ox += w;
      int oy = Y-cy; if (oy < 0) oy += h;
      XSetClipOrigin(fl_display, fl_gc, X-cx, Y-cy);
      fl_copy_offscreen(X, Y, W, H, id, cx, cy);
      // put the old clip region back
      XSetClipOrigin(fl_display, fl_gc, 0, 0);
      fl_restore_clip();
#endif
    } else {
      // mask only
#ifdef WIN32
      HDC tempdc = CreateCompatibleDC(fl_gc);
      SelectObject(tempdc, (HGDIOBJ)mask);
      SetTextColor(fl_gc, 0); // VP : seems necessary at least under win95
      SelectObject(fl_gc, fl_brush);
      // secret bitblt code found in old MSWindows reference manual:
      BitBlt(fl_gc, X, Y, W, H, tempdc, cx, cy, 0xE20746L);
      DeleteDC(tempdc);
#else
      XSetStipple(fl_display, fl_gc, mask);
      int ox = X-cx; if (ox < 0) ox += w;
      int oy = Y-cy; if (oy < 0) oy += h;
      XSetTSOrigin(fl_display, fl_gc, ox, oy);
      XSetFillStyle(fl_display, fl_gc, FillStippled);
      XFillRectangle(fl_display, fl_window, fl_gc, X, Y, W, H);
      XSetFillStyle(fl_display, fl_gc, FillSolid);
#endif
    }
  } else if (id) {
    // pix only, no mask
    fl_copy_offscreen(X, Y, W, H, id, cx, cy);
  } // else { no mask or id, probably an error... }
}

// give the measure of the image
void Fl_Image::measure(int& W, int& H) { W=w; H=h; }

// old code from Fl_Image that created the pixmap with fl_draw_image:
//   if (!id) {
//     id = (ulong)fl_create_offscreen(w, h);
//     fl_begin_offscreen(id);
//     fl_draw_image(array, 0, 0, w, h, d, ld);
//     fl_end_offscreen();
//   }

Fl_Image::~Fl_Image() {
  if (mask) fl_delete_bitmap(mask);
  if (id) fl_delete_offscreen(id);
}

#include <FL/Fl_Widget.H>
// this could probably be inline but they cause the above header
// files to be included by Fl_Image.H:

void Fl_Image::label(Fl_Widget* o) {
  o->image(this);
  o->label(0);
}

//
// End of "$Id: Fl_Image.cxx,v 1.15 2000/11/29 21:43:22 vincentp Exp $".
//
