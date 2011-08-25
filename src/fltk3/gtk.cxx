//
// "$Id$"
//
// "GTK" drawing routines for the Fast Light Tool Kit (FLTK).
//
// These box types provide a GTK+ look, based on Red Hat's Bluecurve
// theme...
//
// Copyright 2006-2010 by Michael Sweet.
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

// Box drawing code for an obscure box type.
// These box types are in separate files so they are not linked
// in if not used.

#include <fltk3/run.h>
#include <fltk3/draw.h>

extern void fl_internal_boxtype(fltk3::Boxtype, fltk3::BoxDrawF*);

static const int tie_gap = 5;
static const float tie_dk = 0.3f;
static const float tie_lt = 0.45f;
static const int use_tie_lt = 1;


static fltk3::Color gtk_get_color(fltk3::Color c) {
  if (fltk3::draw_box_active()) return c;
  else return fltk3::inactive(c);
}

static void gtk_color(fltk3::Color c) {
  fltk3::color(gtk_get_color(c));
}

/*
fltk3::color(hi);
fltk3::yxline(x+1, y+2, b-2);
fltk3::xyline(x+2, y+1, r-2);
fltk3::color(ol);
*/


static void draw_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Color lt, fltk3::Color dk, fltk3::Boxtype t) {
  float f = (dk==fltk3::WHITE) ? 0.2 : 0.5;
  fltk3::Color ol = gtk_get_color(fltk3::color_average(fltk3::BLACK, c, f));
  fltk3::Color hi = gtk_get_color(fltk3::color_average(lt, c, 0.5));
  if (t & 0xff000000) {
    int r = x+w-1, b = y+h-1, xr = x+w/2, yb = y+h/2;
    fltk3::color(ol);
    switch (t & (fltk3::TIE_LEFT|fltk3::TIE_TOP)) {
      case 0:
        fltk3::color(hi);
        fltk3::yxline(x+1, y+2, yb);
        fltk3::xyline(x+2, y+1, xr);
        fltk3::color(ol);
        fltk3::begin_line();
        fltk3::vertex(x, yb);
        fltk3::vertex(x, y+2);
        fltk3::vertex(x+2, y);
        fltk3::vertex(xr, y);
        fltk3::end_line();
        break;
      case fltk3::TIE_LEFT:
        fltk3::color(hi);
        fltk3::xyline(x, y+1, xr);
        fltk3::color(ol);
        fltk3::xyline(x, y, xr);
        break;
      case fltk3::TIE_TOP:
        fltk3::color(hi);
        fltk3::yxline(x+1, y, yb);
        fltk3::color(ol);
        fltk3::yxline(x, yb, y);
    }
    switch (t & (fltk3::TIE_RIGHT|fltk3::TIE_TOP)) {
      case 0:
        fltk3::color(hi);
        fltk3::xyline(xr, y+1, r-2);
        fltk3::color(ol);
        fltk3::begin_line();
        fltk3::vertex(xr, y);
        fltk3::vertex(r-2, y);
        fltk3::vertex(r, y+2);
        fltk3::vertex(r, yb);
        fltk3::end_line();
        break;
      case fltk3::TIE_RIGHT:
        fltk3::color(hi);
        fltk3::xyline(xr, y+1, r);
        fltk3::color(ol);
        fltk3::xyline(xr, y, r);
        break;
      case fltk3::TIE_TOP:
        fltk3::yxline(r, y, yb);
    }
    switch (t & (fltk3::TIE_RIGHT|fltk3::TIE_BOTTOM)) {
      case 0:
        fltk3::begin_line();
        fltk3::vertex(r, yb);
        fltk3::vertex(r, b-2);
        fltk3::vertex(r-2, b);
        fltk3::vertex(xr, b);
        fltk3::end_line();
        break;
      case fltk3::TIE_RIGHT:
        fltk3::xyline(xr, b, r);
        break;
      case fltk3::TIE_BOTTOM:
        fltk3::yxline(r, yb, b);
    }
    switch (t & (fltk3::TIE_LEFT|fltk3::TIE_BOTTOM)) {
      case 0:
        fltk3::color(hi);
        fltk3::yxline(x+1, yb, b-2);
        fltk3::color(ol);
        fltk3::begin_line();
        fltk3::vertex(xr, b);
        fltk3::vertex(x+2, b);
        fltk3::vertex(x, b-2);
        fltk3::vertex(x, yb);
        fltk3::end_line();
        break;
      case fltk3::TIE_LEFT:
        fltk3::xyline(x, b, xr);
        break;
      case fltk3::TIE_BOTTOM:
        fltk3::color(hi);
        fltk3::yxline(x+1, yb, b);
        fltk3::color(ol);
        fltk3::yxline(x, yb, b);
    }
  } else {
    fltk3::color(hi);
    fltk3::xyline(x + 2, y + 1, x + w - 3);
    fltk3::yxline(x + 1, y + 2, y + h - 3);
    
    fltk3::color(ol);
    fltk3::begin_loop();
    fltk3::vertex(x, y + 2);
    fltk3::vertex(x + 2, y);
    fltk3::vertex(x + w - 3, y);
    fltk3::vertex(x + w - 1, y + 2);
    fltk3::vertex(x + w - 1, y + h - 3);
    fltk3::vertex(x + w - 3, y + h - 1);
    fltk3::vertex(x + 2, y + h - 1);
    fltk3::vertex(x, y + h - 3);
    fltk3::end_loop();
  }
}

static void draw_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Color lt, fltk3::Color dk, fltk3::Boxtype t) {
  float f = (dk==fltk3::WHITE) ? 0.5 : 1.0;
  draw_frame(x, y, w, h, c, lt, dk, t);
  if (t & 0xff000000) {
    int r = x+w-1, b = y+h-1;
    int x2 = x+2, r2 = r-2;
    int y2 = y+2, b2 = b-2;
    if (t & fltk3::TIE_LEFT)  x2 = x;
    if (t & fltk3::TIE_RIGHT) r2 = r;
    if (t & fltk3::TIE_TOP) {
      y2 = y;
      if (use_tie_lt) {
        gtk_color(fltk3::color_average(fltk3::WHITE, c, tie_lt));
        fltk3::xyline(x+tie_gap, y, r-tie_gap);
        fltk3::color(c);
        fltk3::xyline(x2, y, x+tie_gap-1);
        fltk3::xyline(r-tie_gap+1, y, r2+1);
        fltk3::rectf(x2, y+1, r2-x2+1, 4);
      } else {
        fltk3::color(c);
        fltk3::rectf(x2, y, r2-x2+2, 5);
      }
    } else {
      gtk_color(fltk3::color_average(lt, c, 0.4f*f));
      fltk3::xyline(x2, y+2, r2);
      gtk_color(fltk3::color_average(lt, c, 0.2f*f));
      fltk3::xyline(x2, y+3, r2);
      gtk_color(fltk3::color_average(lt, c, 0.1f*f));
      fltk3::xyline(x2, y+4, r2);
    }
    gtk_color(c);
    fltk3::rectf(x2, y + 5, r2-x2+1, h - 7);
    if (t & fltk3::TIE_BOTTOM) {
      b2 = b;
      gtk_color(fltk3::color_average(fltk3::BLACK, c, tie_dk));
      fltk3::xyline(x+tie_gap, b, r-tie_gap);
      fltk3::color(c);
      fltk3::xyline(x2, b, x+tie_gap-1);
      fltk3::xyline(r-tie_gap+1, b, r2);
      fltk3::rectf(x2, y+h-4, r2-x2+1, 3);
    } else {
      gtk_color(fltk3::color_average(dk, c, 0.025f/f));
      fltk3::xyline(x2, b-3, r2);
      gtk_color(fltk3::color_average(dk, c, 0.05f/f));
      fltk3::xyline(x2, b-2, r2);
      gtk_color(fltk3::color_average(dk, c, 0.1f/f));
      fltk3::xyline(x2, b-1, r2);
    }
    if (t & fltk3::TIE_LEFT) {
      if (use_tie_lt) {
        gtk_color(fltk3::color_average(fltk3::WHITE, c, tie_lt));
        fltk3::yxline(x, y+tie_gap, b-tie_gap);
        //gtk_color(c);
        //fltk3::yxline(x, y2, y+tie_gap-1);
        //fltk3::yxline(x, b-tie_gap+1, b2);
      }
    }
    if (t & fltk3::TIE_RIGHT) {
      gtk_color(fltk3::color_average(fltk3::BLACK, c, tie_dk));
      fltk3::yxline(r, y+tie_gap, b-tie_gap);
    } else {
      gtk_color(fltk3::color_average(dk, c, 0.1f));
      fltk3::yxline(r-1, y2, b2);
    }
  } else {
    gtk_color(fltk3::color_average(lt, c, 0.4f));
    fltk3::xyline(x + 2, y + 2, x + w - 3);
    gtk_color(fltk3::color_average(lt, c, 0.2f));
    fltk3::xyline(x + 2, y + 3, x + w - 3);
    gtk_color(fltk3::color_average(lt, c, 0.1f));
    fltk3::xyline(x + 2, y + 4, x + w - 3);
    gtk_color(c);
    fltk3::rectf(x + 2, y + 5, w - 4, h - 7);
    gtk_color(fltk3::color_average(dk, c, 0.025f));
    fltk3::xyline(x + 2, y + h - 4, x + w - 3);
    gtk_color(fltk3::color_average(dk, c, 0.05f));
    fltk3::xyline(x + 2, y + h - 3, x + w - 3);
    gtk_color(fltk3::color_average(dk, c, 0.1f));
    fltk3::xyline(x + 2, y + h - 2, x + w - 3);
    fltk3::yxline(x + w - 2, y + 2, y + h - 3);
  }
}

void fl_gtk_up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype t) {
  draw_frame(x, y, w, h, c, fltk3::WHITE, fltk3::BLACK, t);
}

void fl_gtk_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype t) {
  draw_box(x, y, w, h, c, fltk3::WHITE, fltk3::BLACK, t);
}

void fl_gtk_down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype t) {
#if 1
  draw_frame(x, y, w, h, c, fltk3::BLACK, fltk3::WHITE, t);
#else
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.5));
  fltk3::begin_loop();
    fltk3::vertex(x, y + 2);
    fltk3::vertex(x + 2, y);
    fltk3::vertex(x + w - 3, y);
    fltk3::vertex(x + w - 1, y + 2);
    fltk3::vertex(x + w - 1, y + h - 3);
    fltk3::vertex(x + w - 3, y + h - 1);
    fltk3::vertex(x + 2, y + h - 1);
    fltk3::vertex(x, y + h - 3);
  fltk3::end_loop();

  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.1f));
  fltk3::xyline(x + 2, y + 1, x + w - 3);
  fltk3::yxline(x + 1, y + 2, y + h - 3);

  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.05f));
  fltk3::yxline(x + 2, y + h - 2, y + 2, x + w - 2);
#endif
}


void fl_gtk_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype t) {
#if 1
  draw_box(x, y, w, h, c, fltk3::BLACK, fltk3::WHITE, t);
#else
  fl_gtk_down_frame(x, y, w, h, c, t);

  gtk_color(c);
  fltk3::rectf(x + 3, y + 3, w - 5, h - 4);
  fltk3::yxline(x + w - 2, y + 3, y + h - 3);
#endif
}


void fl_gtk_thin_up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype) {
  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.6f));
  fltk3::xyline(x + 1, y, x + w - 2);
  fltk3::yxline(x, y + 1, y + h - 2);

  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.4f));
  fltk3::xyline(x + 1, y + h - 1, x + w - 2);
  fltk3::yxline(x + w - 1, y + 1, y + h - 2);
}


void fl_gtk_thin_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype t) {
  fl_gtk_thin_up_frame(x, y, w, h, c, t);

  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.4f));
  fltk3::xyline(x + 1, y + 1, x + w - 2);
  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.2f));
  fltk3::xyline(x + 1, y + 2, x + w - 2);
  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.1f));
  fltk3::xyline(x + 1, y + 3, x + w - 2);
  gtk_color(c);
  fltk3::rectf(x + 1, y + 4, w - 2, h - 8);
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.025f));
  fltk3::xyline(x + 1, y + h - 4, x + w - 2);
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.05f));
  fltk3::xyline(x + 1, y + h - 3, x + w - 2);
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.1f));
  fltk3::xyline(x + 1, y + h - 2, x + w - 2);
}


void fl_gtk_thin_down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype) {
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.4f));
  fltk3::xyline(x + 1, y, x + w - 2);
  fltk3::yxline(x, y + 1, y + h - 2);

  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.6f));
  fltk3::xyline(x + 1, y + h - 1, x + w - 2);
  fltk3::yxline(x + w - 1, y + 1, y + h - 2);
}


void fl_gtk_thin_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype t) {
  fl_gtk_thin_down_frame(x, y, w, h, c, t);

  gtk_color(c);
  fltk3::rectf(x + 1, y + 1, w - 2, h - 2);
}

//------------------------
// new GTK+ style for round buttons
#if 1

static void arc_i(int x,int y,int w,int h,double a1,double a2) {
  fltk3::arc(x,y,w,h,a1,a2);
}

enum {UPPER_LEFT, LOWER_RIGHT, CLOSED, FILL};

static void draw(int which, int x,int y,int w,int h, int inset)
{
  if (inset*2 >= w) inset = (w-1)/2;
  if (inset*2 >= h) inset = (h-1)/2;
  x += inset;
  y += inset;
  w -= 2*inset;
  h -= 2*inset;
  int d = w <= h ? w : h;
  if (d <= 1) return;
  void (*f)(int,int,int,int,double,double);
  f = (which==FILL) ? fltk3::pie : arc_i;
  if (which >= CLOSED) {
    f(x+w-d, y, d, d, w<=h ? 0 : -90, w<=h ? 180 : 90);
    f(x, y+h-d, d, d, w<=h ? 180 : 90, w<=h ? 360 : 270);
  } else if (which == UPPER_LEFT) {
    f(x+w-d, y, d, d, 45, w<=h ? 180 : 90);
    f(x, y+h-d, d, d, w<=h ? 180 : 90, 225);
  } else { // LOWER_RIGHT
    f(x, y+h-d, d, d, 225, w<=h ? 360 : 270);
    f(x+w-d, y, d, d, w<=h ? 360 : 270, 360+45);
  }
  if (which == FILL) {
    if (w < h)
      fltk3::rectf(x, y+d/2, w, h-(d&-2));
    else if (w > h)
      fltk3::rectf(x+d/2, y, w-(d&-2), h);
  } else {
    if (w < h) {
      if (which != UPPER_LEFT) fltk3::yxline(x+w-1, y+d/2-1, y+h-d/2+1);
      if (which != LOWER_RIGHT) fltk3::yxline(x, y+d/2-1, y+h-d/2+1);
    } else if (w > h) {
      if (which != UPPER_LEFT) fltk3::xyline(x+d/2-1, y+h-1, x+w-d/2+1);
      if (which != LOWER_RIGHT) fltk3::xyline(x+d/2-1, y, x+w-d/2+1);
    }
  }
}

void fl_gtk_round_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype) {
  fltk3::color(c);
  draw(FILL,	    x,   y, w,   h, 2);

  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.025f));
  draw(LOWER_RIGHT, x+1, y, w-2, h, 2);
  draw(LOWER_RIGHT, x,   y, w,   h, 3);
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.05f));
  draw(LOWER_RIGHT, x+1, y, w-2, h, 1);
  draw(LOWER_RIGHT, x,   y, w,   h, 2);
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.1f));
  draw(LOWER_RIGHT, x+1, y, w-2, h, 0);
  draw(LOWER_RIGHT, x,   y, w,   h, 1);

  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.1f));
  draw(UPPER_LEFT,  x,   y, w,   h, 4);
  draw(UPPER_LEFT,  x+1, y, w-2, h, 3);
  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.2f));
  draw(UPPER_LEFT,  x,   y, w,   h, 3);
  draw(UPPER_LEFT,  x+1, y, w-2, h, 2);
  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.4f));
  draw(UPPER_LEFT,  x,   y, w,   h, 2);
  draw(UPPER_LEFT,  x+1, y, w-2, h, 1);
  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.5f));
  draw(UPPER_LEFT,  x,   y, w,   h, 1);
  draw(UPPER_LEFT,  x+1, y, w-2, h, 0);

  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.5f));
  draw(CLOSED,	    x,   y, w,   h, 0);
}

void fl_gtk_round_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype) {
  fltk3::color(c);
  draw(FILL,	    x,   y, w,   h, 2);

  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.05f));
  draw(UPPER_LEFT,  x,   y, w,   h, 2);
  draw(UPPER_LEFT,  x+1, y, w-2, h, 1);
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.1f));
  draw(UPPER_LEFT,  x,   y, w,   h, 1);
  draw(UPPER_LEFT,  x+1, y, w-2, h, 0);

  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.5f));
  draw(CLOSED,	    x,   y, w,   h, 0);
}

#else

void fl_gtk_round_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype) {
  gtk_color(c);
  fltk3::pie(x, y, w, h, 0.0, 360.0);
  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.5f));
  fltk3::arc(x, y, w, h, 45.0, 180.0);
  gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.25f));
  fltk3::arc(x, y, w, h, 180.0, 405.0);
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.5f));
  fltk3::arc(x, y, w, h, 225.0, 360.0);
}


void fl_gtk_round_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Boxtype) {
  gtk_color(c);
  fltk3::pie(x, y, w, h, 0.0, 360.0);
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.2));
  fltk3::arc(x + 1, y, w, h, 90.0, 210.0);
  gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.6));
  fltk3::arc(x, y, w, h, 0.0, 360.0);
}

#endif


//
// End of "$Id$".
//
