//
// "$Id: fl_draw_image.cxx,v 1.20 2002/12/10 02:00:58 easysw Exp $"
//
// Image drawing routines for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#include <fltk/events.h>
#include <fltk/draw.h>
#include <fltk/x.h>

// I hope a simple and portable method of drawing color and monochrome
// images.  To keep this simple, only a single storage type is
// supported: 8 bit unsigned data, byte order RGB, and pixels are
// stored packed into rows with the origin at the top-left.  It is
// possible to alter the size of pixels with the "delta" argument, to
// add alpha or other information per pixel.  It is also possible to
// change the origin and direction of the image data by messing with
// the "delta" and "linedelta", making them negative, though this may
// defeat some of the shortcuts in translating the image for X.

// These files define "innards" and the "DITHER_FILLRECT" macro:
#ifdef _WIN32
# include "fl_draw_image_win32.cxx"
#elif (defined(__APPLE__) && !USE_X11)
# include "fl_draw_image_mac.cxx"
#else
# include "fl_draw_image_x.cxx"
#endif

void fltk::drawimage(const uchar* buf, int x, int y, int w, int h, int d, int l){
  innards(buf,x,y,w,h,d,l,(d<3&&d>-3),0,0);
}
void fltk::drawimage(DrawImageCallback cb, void* data,
		   int x, int y, int w, int h,int d) {
  innards(0,x,y,w,h,d,0,(d<3&&d>-3),cb,data);
}
void fltk::drawimagemono(const uchar* buf, int x, int y, int w, int h, int d, int l){
  innards(buf,x,y,w,h,d,l,1,0,0);
}
void fltk::drawimagemono(DrawImageCallback cb, void* data,
		   int x, int y, int w, int h,int d) {
  innards(0,x,y,w,h,d,0,1,cb,data);
}

#if 0
void fltk::fill_color_rect(int x, int y, int w, int h, Color C) {
  if (!DITHER_FILLRECT) {
    setcolor(C);
    fillrect(x,y,w,h);
  } else {
    uchar c[3];
    split_color(C, c[0], c[1], c[2]);
    innards(c,x,y,w,h,0,0,0,0,0);
  }
}
#endif

//
// End of "$Id: fl_draw_image.cxx,v 1.20 2002/12/10 02:00:58 easysw Exp $".
//
