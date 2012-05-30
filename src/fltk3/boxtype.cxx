//
// "$Id$"
//
// Box drawing code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

/**
  \file fl_boxtype.cxx
  \brief drawing code for common box types.
*/

// Box drawing code for the common box types and the table of
// boxtypes.  Other box types are in separate files so they are not
// linked in if not used.

#include <fltk3/run.h>
#include <fltk3/Widget.h>
#include <fltk3/Box.h>
#include <fltk3/draw.h>
#include <config.h>

////////////////////////////////////////////////////////////////

static uchar active_ramp[24] = {
  fltk3::GRAY_RAMP+0, fltk3::GRAY_RAMP+1, fltk3::GRAY_RAMP+2, fltk3::GRAY_RAMP+3,
  fltk3::GRAY_RAMP+4, fltk3::GRAY_RAMP+5, fltk3::GRAY_RAMP+6, fltk3::GRAY_RAMP+7,
  fltk3::GRAY_RAMP+8, fltk3::GRAY_RAMP+9, fltk3::GRAY_RAMP+10,fltk3::GRAY_RAMP+11,
  fltk3::GRAY_RAMP+12,fltk3::GRAY_RAMP+13,fltk3::GRAY_RAMP+14,fltk3::GRAY_RAMP+15,
  fltk3::GRAY_RAMP+16,fltk3::GRAY_RAMP+17,fltk3::GRAY_RAMP+18,fltk3::GRAY_RAMP+19,
  fltk3::GRAY_RAMP+20,fltk3::GRAY_RAMP+21,fltk3::GRAY_RAMP+22,fltk3::GRAY_RAMP+23};
static uchar inactive_ramp[24] = {
  43, 43, 44, 44,
  44, 45, 45, 46,
  46, 46, 47, 47,
  48, 48, 48, 49,
  49, 49, 50, 50,
  51, 51, 52, 52};
static int draw_it_active = 1;

/**
  Determines if the current draw box is active or inactive. 
  If inactive, the box color is changed by the inactive color.
*/
int fltk3::draw_box_active() { return draw_it_active; }

namespace fltk3 {
  uchar *gray_ramp() {return (draw_it_active?active_ramp:inactive_ramp)-'A';}
}

/**
  Draws a series of line segments around the given box.
  The string \p s must contain groups of 4 letters which specify one of 24
  standard grayscale values, where 'A' is black and 'X' is white.
  The order of each set of 4 characters is: top, left, bottom, right.
  The result of calling fltk3::frame() with a string that is not a multiple
  of 4 characters in length is undefined.
  The only difference between this function and fltk3::frame2() is the order
  of the line segments.
  \param[in] s sets of 4 grayscale values in top, left, bottom, right order
  \param[in] x, y, w, h position and size
*/
void fltk3::frame(const char* s, int x, int y, int w, int h) {
  uchar *g = fltk3::gray_ramp();
  if (h > 0 && w > 0) for (;*s;) {
    // draw top line:
    fltk3::color(g[(int)*s++]);
    fltk3::xyline(x, y, x+w-1);
    y++; if (--h <= 0) break;
    // draw left line:
    fltk3::color(g[(int)*s++]);
    fltk3::yxline(x, y+h-1, y);
    x++; if (--w <= 0) break;
    // draw bottom line:
    fltk3::color(g[(int)*s++]);
    fltk3::xyline(x, y+h-1, x+w-1);
    if (--h <= 0) break;
    // draw right line:
    fltk3::color(g[(int)*s++]);
    fltk3::yxline(x+w-1, y+h-1, y);
    if (--w <= 0) break;
  }
}

/**
  Draws a series of line segments around the given box.
  The string \p s must contain groups of 4 letters which specify one of 24
  standard grayscale values, where 'A' is black and 'X' is white.
  The order of each set of 4 characters is: bottom, right, top, left.
  The result of calling fltk3::frame2() with a string that is not a multiple
  of 4 characters in length is undefined.
  The only difference between this function and fltk3::frame() is the order
  of the line segments.
  \param[in] s sets of 4 grayscale values in bottom, right, top, left order
  \param[in] x, y, w, h position and size
*/
void fltk3::frame2(const char* s, int x, int y, int w, int h) {
  uchar *g = fltk3::gray_ramp();
  if (h > 0 && w > 0) for (;*s;) {
    // draw bottom line:
    fltk3::color(g[(int)*s++]);
    fltk3::xyline(x, y+h-1, x+w-1);
    if (--h <= 0) break;
    // draw right line:
    fltk3::color(g[(int)*s++]);
    fltk3::yxline(x+w-1, y+h-1, y);
    if (--w <= 0) break;
    // draw top line:
    fltk3::color(g[(int)*s++]);
    fltk3::xyline(x, y, x+w-1);
    y++; if (--h <= 0) break;
    // draw left line:
    fltk3::color(g[(int)*s++]);
    fltk3::yxline(x, y+h-1, y);
    x++; if (--w <= 0) break;
  }
}

/** Draws a box of type fltk3::NO_BOX */
void fl_no_box(int, int, int, int, fltk3::Color, fltk3::Box*) {}

void fl_flat_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*) {
  fltk3::rectf(x, y, w, h, c);
}

/** Draws a frame of type fltk3::THIN_DOWN_FRAME */
void fl_classic_thin_down_frame(int x, int y, int w, int h, fltk3::Color, fltk3::Box*) {
  fltk3::frame2("WWHH",x,y,w,h);
}

/** Draws a box of type fltk3::THIN_DOWN_BOX */
void fl_classic_thin_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) {
  fl_classic_thin_down_frame(x,y,w,h,c,t);
  fltk3::color(draw_it_active ? c : fltk3::inactive(c));
  fltk3::rectf(x+1, y+1, w-2, h-2);
}

/** Draws a frame of type fltk3::THIN_UP_FRAME */
void fl_classic_thin_up_frame(int x, int y, int w, int h, fltk3::Color, fltk3::Box*) {
  fltk3::frame2("HHWW",x,y,w,h);
}

/** Draws a box of type fltk3::THIN_UP_BOX */
void fl_classic_thin_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) {
  fl_classic_thin_up_frame(x,y,w,h,c,t);
  fltk3::color(draw_it_active ? c : fltk3::inactive(c));
  fltk3::rectf(x+1, y+1, w-2, h-2);
}

/** Draws a frame of type fltk3::UP_FRAME */
void fl_classic_up_frame(int x, int y, int w, int h, fltk3::Color, fltk3::Box*) {
#if BORDER_WIDTH == 1
  fltk3::frame2("HHWW",x,y,w,h);
#else
#if BORDER_WIDTH == 2
  fltk3::frame2("AAWWMMTT",x,y,w,h);
#else
  fltk3::frame("AAAAWWJJUTNN",x,y,w,h);
#endif
#endif
}

#define D1 BORDER_WIDTH
#define D2 (BORDER_WIDTH+BORDER_WIDTH)

/** Draws a box of type fltk3::UP_BOX */
void fl_classic_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) {
  fl_classic_up_frame(x,y,w,h,c,t);
  fltk3::color(draw_it_active ? c : fltk3::inactive(c));
  fltk3::rectf(x+D1, y+D1, w-D2, h-D2);
}

/** Draws a frame of type fltk3::DOWN_FRAME */
void fl_classic_down_frame(int x, int y, int w, int h, fltk3::Color, fltk3::Box*) {
#if BORDER_WIDTH == 1
  fltk3::frame2("WWHH",x,y,w,h);
#else
#if BORDER_WIDTH == 2
  fltk3::frame2("WWMMPPAA",x,y,w,h);
#else
  fltk3::frame("NNTUJJWWAAAA",x,y,w,h);
#endif
#endif
}

/** Draws a box of type fltk3::DOWN_BOX */
void fl_classic_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) {
  fl_classic_down_frame(x,y,w,h,c,t);
  fltk3::color(c); fltk3::rectf(x+D1, y+D1, w-D2, h-D2);
}

/** Draws a frame of type fltk3::ENGRAVED_FRAME */
void fl_engraved_frame(int x, int y, int w, int h, fltk3::Color, fltk3::Box*) {
  fltk3::frame("HHWWWWHH",x,y,w,h);
}

/** Draws a box of type fltk3::ENGRAVED_BOX */
void fl_engraved_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) {
  fl_engraved_frame(x,y,w,h,c,t);
  fltk3::color(draw_it_active ? c : fltk3::inactive(c));
  fltk3::rectf(x+2, y+2, w-4, h-4);
}

/** Draws a frame of type fltk3::EMBOSSED_FRAME */
void fl_embossed_frame(int x, int y, int w, int h, fltk3::Color, fltk3::Box*) {
  fltk3::frame("WWHHHHWW",x,y,w,h);
}

/** Draws a box of type fltk3::EMBOSSED_BOX */
void fl_embossed_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) {
  fl_embossed_frame(x,y,w,h,c,t);
  fltk3::color(draw_it_active ? c : fltk3::inactive(c));
  fltk3::rectf(x+2, y+2, w-4, h-4);
}

/**
  Draws a bounded rectangle with a given position, size and color.
  Equivalent to drawing a box of type fltk3::BORDER_BOX.
*/
void fl_rectbound(int x, int y, int w, int h, fltk3::Color bgcolor, fltk3::Box*) {
  fltk3::color(draw_it_active ? fltk3::BLACK : fltk3::inactive(fltk3::BLACK));
  fltk3::rect(x, y, w, h);
  fltk3::color(draw_it_active ? bgcolor : fltk3::inactive(bgcolor));
  fltk3::rectf(x+1, y+1, w-2, h-2);
}
#define fl_border_box fl_rectbound	/**< allow consistent naming */

/**
  Draws a frame of type fltk3::BORDER_FRAME.
*/
void fl_border_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*) {
  fltk3::color(draw_it_active ? c : fltk3::inactive(c));
  fltk3::rect(x, y, w, h);
}

////////////////////////////////////////////////////////////////

void fl_diamond_down_box(int x,int y,int w,int h,fltk3::Color bgcolor, fltk3::Box*);
void fl_diamond_up_box(int x,int y,int w,int h,fltk3::Color bgcolor, fltk3::Box*);
void fl_shadow_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_shadow_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);

void fl_classic_round_up_box(int x, int y, int w, int h, fltk3::Color bgcolor, fltk3::Box*);
void fl_classic_round_down_box(int x, int y, int w, int h, fltk3::Color bgcolor, fltk3::Box*);
void fl_rflat_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_rounded_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_rounded_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_rshadow_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);

void fl_oval_shadow_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_oval_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_oval_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_oval_flat_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);

void fl_plastic_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_plastic_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_plastic_up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_plastic_down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_plastic_thin_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_plastic_up_round(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_plastic_down_round(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);

void fl_gtk_up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_gtk_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_gtk_down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_gtk_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_gtk_thin_up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_gtk_thin_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_gtk_thin_down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_gtk_thin_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_gtk_round_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_gtk_round_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);

static struct {
  fltk3::BoxDrawF *f;
  uchar dx, dy, dw, dh;
  int set;
} fl_box_table[256] = {
// must match list in enumerations.h!!!
  {fl_no_box,                   0,0,0,0,1},		
  {fl_flat_box,                 0,0,0,0,1}, // fltk3::FLAT_BOX
  {fl_gtk_up_box,		2,2,4,4,0}, // FL_GTK_UP_BOX,
  {fl_gtk_down_box,		2,2,4,4,0}, // FL_GTK_DOWN_BOX,
  {fl_gtk_up_frame,		2,2,4,4,0}, // FL_GTK_UP_FRAME,
  {fl_gtk_down_frame,           2,2,4,4,0}, // FL_GTK_DOWN_FRAME,
  {fl_gtk_thin_up_box,		1,1,2,2,0}, // FL_GTK_THIN_ROUND_UP_BOX,
  {fl_gtk_thin_down_box,	1,1,2,2,0}, // FL_GTK_THIN_ROUND_DOWN_BOX,
  {fl_gtk_thin_up_frame,	1,1,2,2,0}, // FL_GTK_THIN_UP_FRAME,
  {fl_gtk_thin_down_frame,	1,1,2,2,0}, // FL_GTK_THIN_DOWN_FRAME,
  {fl_engraved_box,             2,2,4,4,1},
  {fl_embossed_box,             2,2,4,4,1},
  {fl_engraved_frame,           2,2,4,4,1},
  {fl_embossed_frame,           2,2,4,4,1},
  {fl_border_box,               1,1,2,2,1},
  {fl_shadow_box,               1,1,5,5,0}, // FL_SHADOW_BOX,
  {fl_border_frame,             1,1,2,2,1},
  {fl_shadow_frame,             1,1,5,5,0}, // FL_SHADOW_FRAME,
  
  {fl_rounded_box,              1,1,2,2,0}, // FL_ROUNDED_BOX,
  {fl_rshadow_box,              1,1,2,2,0}, // FL_RSHADOW_BOX,
  {fl_rounded_frame,            1,1,2,2,0}, // FL_ROUNDED_FRAME
  {fl_rflat_box,		0,0,0,0,0}, // FL_RFLAT_BOX,
  {fl_gtk_round_up_box,		2,2,4,4,0}, // FL_GTK_ROUND_UP_BOX,
  {fl_gtk_round_down_box,	2,2,4,4,0}, // FL_GTK_ROUND_DOWN_BOX,
  {fl_diamond_up_box,		0,0,0,0,0}, // FL_DIAMOND_UP_BOX
  {fl_diamond_down_box,		0,0,0,0,0}, // FL_DIAMOND_DOWN_BOX
  
  {fl_oval_box,                 1,1,2,2,0}, // FL_OVAL_BOX,
  {fl_oval_shadow_box,          1,1,2,2,0}, // FL_OVAL_SHADOW_BOX,
  {fl_oval_frame,               1,1,2,2,0}, // FL_OVAL_FRAME
  {fl_oval_flat_box,		0,0,0,0,0}, // FL_OVAL_FLAT_BOX,
  
  {fl_plastic_up_box,		4,4,8,8,0}, // FL_PLASTIC_UP_BOX,
  {fl_plastic_down_box,		2,2,4,4,0}, // FL_PLASTIC_DOWN_BOX,
  {fl_plastic_up_frame,		2,2,4,4,0}, // FL_PLASTIC_UP_FRAME,
  {fl_plastic_down_frame,	2,2,4,4,0}, // FL_PLASTIC_DOWN_FRAME,
  {fl_plastic_thin_up_box,	2,2,4,4,0}, // FL_PLASTIC_THIN_UP_BOX,
  {fl_plastic_down_box,		2,2,4,4,0}, // FL_PLASTIC_THIN_DOWN_BOX,
  {fl_plastic_up_round,		2,2,4,4,0}, // FL_PLASTIC_ROUND_UP_BOX,
  {fl_plastic_down_round,	2,2,4,4,0}, // FL_PLASTIC_ROUND_DOWN_BOX,
  
  {fl_classic_up_box,           D1,D1,D2,D2,1},
  {fl_classic_down_box,         D1,D1,D2,D2,1},
  {fl_classic_up_frame,         D1,D1,D2,D2,1},
  {fl_classic_down_frame,       D1,D1,D2,D2,1},
  {fl_classic_thin_up_box,      1,1,2,2,1},
  {fl_classic_thin_down_box,    1,1,2,2,1},
  {fl_classic_thin_up_frame,    1,1,2,2,1},
  {fl_classic_thin_down_frame,  1,1,2,2,1},
  {fl_classic_round_up_box,     2,2,4,4,0},
  {fl_classic_round_down_box,   2,2,4,4,0},
  
  {fl_gtk_up_box,               3,3,6,6,0}, // FL_FREE_BOX+0
  {fl_gtk_down_box,             3,3,6,6,0}, // FL_FREE_BOX+1
  {fl_gtk_up_box,               3,3,6,6,0}, // FL_FREE_BOX+2
  {fl_gtk_down_box,             3,3,6,6,0}, // FL_FREE_BOX+3
  {fl_gtk_up_box,               3,3,6,6,0}, // FL_FREE_BOX+4
  {fl_gtk_down_box,             3,3,6,6,0}, // FL_FREE_BOX+5
  {fl_gtk_up_box,               3,3,6,6,0}, // FL_FREE_BOX+6
  {fl_gtk_down_box,             3,3,6,6,0}, // FL_FREE_BOX+7
};

/**
  Returns the X offset for the given boxtype.
  \see box_dy()
*/
int fltk3::box_dx(fltk3::Box* t) {return t->dx();}

/**
    Returns the Y offset for the given boxtype.

    These functions return the offset values necessary for a given
    boxtype, useful for computing the area inside a box's borders, to
    prevent overdrawing the borders.

    For instance, in the case of a boxtype like fltk3::DOWN_BOX
    where the border width might be 2 pixels all around, the above 
    functions would return 2, 2, 4, and 4 for box_dx, 
    box_dy, box_dw, and box_dh respectively.

    An example to compute the area inside a widget's box():
    \code
         int X = yourwidget->x() + fltk3::box_dx(yourwidget->box());
         int Y = yourwidget->y() + fltk3::box_dy(yourwidget->box());
         int W = yourwidget->w() - fltk3::box_dw(yourwidget->box());
         int H = yourwidget->h() - fltk3::box_dh(yourwidget->box());
    \endcode
    These functions are mainly useful in the draw() code 
    for deriving custom widgets, where one wants to avoid drawing 
    over the widget's own border box().
*/
int fltk3::box_dy(fltk3::Box* t) {return t->dy();}

/**
  Returns the width offset for the given boxtype.
  \see box_dy().
*/
int fltk3::box_dw(fltk3::Box* t) {return t->dw();}

/**
  Returns the height offset for the given boxtype.
  \see box_dy().
*/
int fltk3::box_dh(fltk3::Box* t) {return t->dh();}

/**
  Sets the drawing function for a given box type.
  \param[in] t box type
  \param[in] f box drawing function
*/
void fl_internal_boxtype(fltk3::Box* t, fltk3::BoxDrawF* f) {
  // FIXME: if (!fl_box_table[t&255].set) {
  // FIXME:   fl_box_table[t&255].f   = f;
  // FIXME:   fl_box_table[t&255].set = 1;
  // FIXME: }
}

/** Gets the current box drawing function for the specified box type. */
fltk3::BoxDrawF *fltk3::get_boxtype(fltk3::Box* t) {
  return 0L; // FIXME: fl_box_table[t&255].f;
}
/** Sets the function to call to draw a specific boxtype. */
void fltk3::set_boxtype(fltk3::Box* t, fltk3::BoxDrawF* f,
		      uchar a, uchar b, uchar c, uchar d) {
  // FIXME: fl_box_table[t&255].f   = f;
  // FIXME: fl_box_table[t&255].set = 1;
  // FIXME: fl_box_table[t&255].dx  = a;
  // FIXME: fl_box_table[t&255].dy  = b;
  // FIXME: fl_box_table[t&255].dw  = c;
  // FIXME: fl_box_table[t&255].dh  = d;
}


/** Copies the from boxtype. */
void fltk3::set_boxtype(fltk3::Box* to, fltk3::Box* from) {
  // FIXME: fl_box_table[to&255] = fl_box_table[from&255];
}


/**
  Draws a box using given type, position, size and color.
  \param[in] t box type
  \param[in] x, y, w, h position and size
  \param[in] c color
*/
void fltk3::draw_box(fltk3::Box* t, int x, int y, int w, int h, fltk3::Color c) {
  if (t) {
    fltk3::color(c);
    t->draw(fltk3::Rectangle(x, y, w, h));
  }
}


//extern fltk3::Widget *fl_boxcheat; // hack set by fltk3::Window.cxx
/** Draws the widget box according its box style */
void fltk3::Widget::draw_box() const {
  if (box_) {
    draw_box(box_, 0, 0, w_, h_, color_);
  }
  draw_backdrop();
}


/** If fltk3::ALIGN_IMAGE_BACKDROP is set, the image or deimage will be drawn */
void fltk3::Widget::draw_backdrop() const {
  if (align() & fltk3::ALIGN_IMAGE_BACKDROP) {
    const fltk3::Image *img = image();
    // if there is no image, we will not draw the deimage either
    if (img && deimage() && !active_r())
      img = deimage();
    if (img) 
      ((fltk3::Image*)img)->draw((w_-img->w())/2, (h_-img->h())/2);
  }
}


/** 
 Draws a box of type t, of color c at the widget's position and size. 
 */
void fltk3::Widget::draw_box(fltk3::Box* t, fltk3::Color c) const 
{
  draw_box(t, 0, 0, w_, h_, c);
}


/** Draws a box of type t, of color c at the position X,Y and size W,H. */
void fltk3::Widget::draw_box(fltk3::Box* t, int X, int Y, int W, int H, fltk3::Color c) const {
  if (t) {
    draw_it_active = active_r();
    fltk3::color(c);
    t->draw(fltk3::Rectangle(X, Y, W, H));
    draw_it_active = 1;
  }
}

//------------------------------------------------------------------------------

void fltk3::BorderFrame::_draw(const Rectangle &r) const
{
  fl_border_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::BorderFrame borderFrame("borderFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::BORDER_FRAME = &borderFrame;

//------------------------------------------------------------------------------

void fltk3::EngravedBox::_draw(const Rectangle &r) const
{
  fl_engraved_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::EngravedBox engravedBox("engravedBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::ENGRAVED_BOX = &engravedBox;

//------------------------------------------------------------------------------

void fltk3::EmbossedBox::_draw(const Rectangle &r) const
{
  fl_embossed_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::EmbossedBox embossedBox("embossedBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::EMBOSSED_BOX = &embossedBox;

//------------------------------------------------------------------------------

void fltk3::BorderBox::_draw(const Rectangle &r) const
{
  fl_border_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::BorderBox borderBox("borderBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::BORDER_BOX = &borderBox;

//------------------------------------------------------------------------------

void fltk3::ShadowBox::_draw(const Rectangle &r) const
{
  fl_shadow_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ShadowBox shadowBox("shadowBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::SHADOW_BOX = &shadowBox;

//------------------------------------------------------------------------------

void fltk3::RoundedBox::_draw(const Rectangle &r) const
{
  fl_rounded_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::RoundedBox roundedBox("roundedBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::ROUNDED_BOX = &roundedBox;

//------------------------------------------------------------------------------

void fltk3::RShadowBox::_draw(const Rectangle &r) const
{
  fl_rshadow_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::RShadowBox rShadowBox("rShadowBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::RSHADOW_BOX = &rShadowBox;

//------------------------------------------------------------------------------

void fltk3::RFlatBox::_draw(const Rectangle &r) const
{
  fl_rflat_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::RFlatBox rFlatBox("rFlatBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::RFLAT_BOX = &rFlatBox;

//------------------------------------------------------------------------------

void fltk3::RoundUpBox::_draw(const Rectangle &r) const
{
  fl_gtk_round_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::RoundUpBox roundUpBox("roundUpBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::ROUND_UP_BOX = &roundUpBox;

//------------------------------------------------------------------------------

void fltk3::RoundDownBox::_draw(const Rectangle &r) const
{
  fl_gtk_round_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::RoundDownBox roundDownBox("roundDownBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::ROUND_DOWN_BOX = &roundDownBox;

//------------------------------------------------------------------------------

void fltk3::DiamondUpBox::_draw(const Rectangle &r) const
{
  fl_diamond_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::DiamondUpBox diamondUpBox("diamondUpBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::DIAMOND_UP_BOX = &diamondUpBox;

//------------------------------------------------------------------------------

void fltk3::DiamondDownBox::_draw(const Rectangle &r) const
{
  fl_diamond_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::DiamondDownBox diamondDownBox("diamondDownBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::DIAMOND_DOWN_BOX = &diamondDownBox;

//------------------------------------------------------------------------------

void fltk3::OvalBox::_draw(const Rectangle &r) const
{
  fl_oval_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::OvalBox ovalBox("ovalBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::OVAL_BOX = &ovalBox;

//------------------------------------------------------------------------------

void fltk3::OShadowBox::_draw(const Rectangle &r) const
{
  fl_oval_shadow_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::OShadowBox oShadowBox("oShadowBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::OSHADOW_BOX = &oShadowBox;

//------------------------------------------------------------------------------

void fltk3::OFlatBox::_draw(const Rectangle &r) const
{
  fl_oval_flat_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::OFlatBox oFlatBox("oFlatBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::OFLAT_BOX = &oFlatBox;

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

void fltk3::ClassicUpBox::_draw(const Rectangle &r) const
{
  fl_classic_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ClassicUpBox classicUpBox("classicUpBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::CLASSIC_UP_BOX = &classicUpBox;

//------------------------------------------------------------------------------

void fltk3::ClassicDownBox::_draw(const Rectangle &r) const
{
  fl_classic_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ClassicDownBox classicDownBox("classicDownBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::CLASSIC_DOWN_BOX = &classicDownBox;

//------------------------------------------------------------------------------

void fltk3::ClassicThinUpBox::_draw(const Rectangle &r) const
{
  fl_classic_thin_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ClassicThinUpBox classicThinUpBox("classicThinUpBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::CLASSIC_THIN_UP_BOX = &classicThinUpBox;

//------------------------------------------------------------------------------

void fltk3::ClassicThinDownBox::_draw(const Rectangle &r) const
{
  fl_classic_thin_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ClassicThinDownBox classicThinDownBox("classicThinDownBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::CLASSIC_THIN_DOWN_BOX = &classicThinDownBox;

//------------------------------------------------------------------------------

void fltk3::ClassicRoundUpBox::_draw(const Rectangle &r) const
{
  fl_classic_round_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ClassicRoundUpBox classicRoundUpBox("classicRoundUpBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::CLASSIC_ROUND_UP_BOX = &classicRoundUpBox;

//------------------------------------------------------------------------------

void fltk3::ClassicRoundDownBox::_draw(const Rectangle &r) const
{
  fl_classic_round_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ClassicRoundDownBox classicRoundDownBox("classicRoundDownBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::CLASSIC_ROUND_DOWN_BOX = &classicRoundDownBox;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

void fltk3::UpFrame::_draw(const Rectangle &r) const
{
  fl_gtk_up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::UpFrame upFrame("upFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::UP_FRAME = &upFrame;

//------------------------------------------------------------------------------

void fltk3::DownFrame::_draw(const Rectangle &r) const
{
  fl_gtk_down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::DownFrame downFrame("downFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::DOWN_FRAME = &downFrame;

//------------------------------------------------------------------------------

void fltk3::ThinUpFrame::_draw(const Rectangle &r) const
{
  fl_gtk_thin_up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ThinUpFrame thinUpFrame("thinUpFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::THIN_UP_FRAME = &thinUpFrame;

//------------------------------------------------------------------------------

void fltk3::ThinDownFrame::_draw(const Rectangle &r) const
{
  fl_gtk_thin_down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ThinDownFrame thinDownFrame("thinDownFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::THIN_DOWN_FRAME = &thinDownFrame;

//------------------------------------------------------------------------------

void fltk3::EngravedFrame::_draw(const Rectangle &r) const
{
  fl_engraved_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::EngravedFrame engravedFrame("engravedFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::ENGRAVED_FRAME = &engravedFrame;

//------------------------------------------------------------------------------

void fltk3::EmbossedFrame::_draw(const Rectangle &r) const
{
  fl_embossed_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::EmbossedFrame embossedFrame("embossedFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::EMBOSSED_FRAME = &embossedFrame;

//------------------------------------------------------------------------------

void fltk3::ShadowFrame::_draw(const Rectangle &r) const
{
  fl_shadow_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ShadowFrame shadowFrame("shadowFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::SHADOW_FRAME = &shadowFrame;

//------------------------------------------------------------------------------

void fltk3::RoundedFrame::_draw(const Rectangle &r) const
{
  fl_rounded_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::RoundedFrame roundedFrame("roundedFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::ROUNDED_FRAME = &roundedFrame;

//------------------------------------------------------------------------------

void fltk3::OvalFrame::_draw(const Rectangle &r) const
{
  fl_oval_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::OvalFrame ovalFrame("ovalFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::OVAL_FRAME = &ovalFrame;

//------------------------------------------------------------------------------

void fltk3::ClassicUpFrame::_draw(const Rectangle &r) const
{
  fl_classic_up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ClassicUpFrame classicUpFrame("classicUpFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::CLASSIC_UP_FRAME = &classicUpFrame;

//------------------------------------------------------------------------------

void fltk3::ClassicDownFrame::_draw(const Rectangle &r) const
{
  fl_classic_down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ClassicDownFrame classicDownFrame("classicDownFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::CLASSIC_DOWN_FRAME = &classicDownFrame;

//------------------------------------------------------------------------------

void fltk3::ClassicThinUpFrame::_draw(const Rectangle &r) const
{
  fl_classic_thin_up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ClassicThinUpFrame classicThinUpFrame("classicThinUpFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::CLASSIC_THIN_UP_FRAME = &classicThinUpFrame;

//------------------------------------------------------------------------------

void fltk3::ClassicThinDownFrame::_draw(const Rectangle &r) const
{
  fl_classic_thin_down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::ClassicThinDownFrame classicThinDownFrame("classicThinDownFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::CLASSIC_THIN_DOWN_FRAME = &classicThinDownFrame;

//------------------------------------------------------------------------------


namespace fltk3 {
  
  const int box_lut_size = 80;
  
  static Box* box_lut[box_lut_size] = {
    NO_BOX,
    FLAT_BOX,
    UP_BOX,
    DOWN_BOX,
    UP_FRAME,
    DOWN_FRAME,
    THIN_UP_BOX,
    THIN_DOWN_BOX,
    THIN_UP_FRAME,
    THIN_DOWN_FRAME,
    ENGRAVED_BOX,
    EMBOSSED_BOX,
    ENGRAVED_FRAME,
    EMBOSSED_FRAME,
    BORDER_BOX,
    SHADOW_BOX,
    BORDER_FRAME,
    SHADOW_FRAME,
    ROUNDED_BOX,
    RSHADOW_BOX,
    ROUNDED_FRAME,
    RFLAT_BOX,
    ROUND_UP_BOX,
    ROUND_DOWN_BOX,
    DIAMOND_UP_BOX,
    DIAMOND_DOWN_BOX,
    OVAL_BOX,
    OSHADOW_BOX,
    OVAL_FRAME,
    OFLAT_BOX,
    PLASTIC_UP_BOX,
    PLASTIC_DOWN_BOX,
    PLASTIC_UP_FRAME,
    PLASTIC_DOWN_FRAME,
    PLASTIC_THIN_UP_BOX,
    PLASTIC_THIN_DOWN_BOX,
    PLASTIC_ROUND_UP_BOX,
    PLASTIC_ROUND_DOWN_BOX,
    CLASSIC_UP_BOX,
    CLASSIC_DOWN_BOX,
    CLASSIC_UP_FRAME,
    CLASSIC_DOWN_FRAME,
    CLASSIC_THIN_UP_BOX,
    CLASSIC_THIN_DOWN_BOX,
    CLASSIC_THIN_UP_FRAME,
    CLASSIC_THIN_DOWN_FRAME,
    CLASSIC_ROUND_UP_BOX,
    CLASSIC_ROUND_DOWN_BOX,
  };
  
  int _3to1_boxtype_i(Box* k) {
    int i;
    for (i=0; i<box_lut_size; i++) {
      if (box_lut[i]==k)
        return i;
    }
    return 2;
  }
  
  Box* _1to3_boxtype_i(int k) {
    Box* b = box_lut[k%box_lut_size];
    if (b) {
      return b;
    } else {
      return UP_BOX;
    }
  }
  
}


//
// End of "$Id$".
//
