//
// "$Id: draw.h,v 1.3 2003/02/21 18:16:30 spitzak Exp $"
//
// The fltk drawing library
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

#ifndef fltk_draw_h
#define fltk_draw_h

#include "Flags.h" // for alignment values
#include "Color.h"

namespace fltk {

struct Font;

// current transformation:
FL_API void push_matrix();
FL_API void pop_matrix();
FL_API void scale(float x, float y);
FL_API void scale(float x);
FL_API void translate(float x, float y);
FL_API void translate(int x, int y);
FL_API void rotate(float d);
FL_API void concat(float, float, float, float, float, float);

// get and use transformed positions:
FL_API void transform(float& x, float& y);
FL_API void transform(int& x, int& y);
FL_API void transform_distance(float& x, float& y);

// clip:
FL_API void push_clip(int x, int y, int w, int h);
FL_API void clip_out(int x, int y, int w, int h);
FL_API void pop_clip();
FL_API int not_clipped(int x, int y, int w, int h);
FL_API int clip_box(int, int, int, int, int& x, int& y, int& w, int& h);

// Colors:
FL_API void setcolor(Color);
extern FL_API Color current_color_;
inline Color getcolor() {return current_color_;}

// line type:
FL_API void line_style(int style, int width=0, char* dashes=0);
enum {
  SOLID	= 0,
  DASH	= 1,
  DOT	= 2,
  DASHDOT	= 3,
  DASHDOTDOT	= 4,

  CAP_FLAT	= 0x100,
  CAP_ROUND	= 0x200,
  CAP_SQUARE	= 0x300,

  JOIN_MITER	= 0x1000,
  JOIN_ROUND	= 0x2000,
  JOIN_BEVEL	= 0x3000
};

// Build the path:
FL_API void newpath();
FL_API void addvertex(float x, float y);
FL_API void addvertex(int x, int y);
FL_API void addvertices(int n, const float v[][2]);
FL_API void addvertices(int n, const int v[][2]);
FL_API void addvertices_transformed(int n, const float v[][2]);
FL_API void addcurve(float,float, float,float, float,float, float,float);
FL_API void addarc(float x,float y,float w,float h, float a1, float a2);
FL_API void addcircle(float x, float y, float r);
FL_API void addellipse(float x, float y, float w, float h);
FL_API void closepath();

// draw the path:
FL_API void drawpoints();
FL_API void strokepath();
FL_API void fillpath();
FL_API void fillstrokepath(Color);

// drawing that bypasses the path mechanism:
FL_API void strokerect(int x, int y, int w, int h);
FL_API void fillrect(int x, int y, int w, int h);
FL_API void drawpoint(int x, int y);
FL_API void drawline(int,int, int,int);
enum {FILLPIE, FILLARC, STROKEPIE, STROKEARC};
FL_API void fillpie(int x,int y,int w,int h,float a,float a2,int what=FILLPIE);
inline void strokepie(int x,int y,int w,int h,float a,float a2) {
  fillpie(x,y,w,h,a,a2,STROKEPIE);}
inline void fillarc(int x,int y,int w,int h,float a,float a2) {
  fillpie(x,y,w,h,a,a2,FILLARC);}
inline void strokearc(int x,int y,int w,int h,float a,float a2) {
  fillpie(x,y,w,h,a,a2,STROKEARC);}

// current font+size:
FL_API void setfont(Font*, float size);
FL_API void setfont(const char*, float size);
FL_API void setfont(const char*, int attributes, float size);

// change the encoding used to draw bytes (depreciated)
extern FL_API const char* encoding_;
inline const char* get_encoding() {return encoding_;}
FL_API void set_encoding(const char*);

// information you can get about the current font+size+encoding:
extern FL_API Font* current_font_;
extern FL_API float current_size_; // should be 2x2 transformation matrix
inline Font* getfont() {return current_font_;}
inline float getsize() {return current_size_;}

// measure things in the current font:
FL_API float getwidth(const char*);
FL_API float getwidth(const char*, int n);
FL_API float getascent();
FL_API float getdescent();

// draw using current font:
FL_API void drawtext_transformed(const char*, int n, float x, float y);
FL_API void drawtext(const char*, float x, float y);
FL_API void drawtext(const char*, int n, float x, float y);

// the "fancy" label text formatter:
FL_API void measure(const char*, int& w, int& h, Flags = 0);
FL_API void drawtext(const char*, int,int,int,int, Flags);

// "fancy" modifications, these may change!
FL_API int add_symbol(const char* name, void (*drawit)(Color), int scalable);
FL_API int draw_symbol(const char* label, int x,int y,int w,int h, Color);
extern FL_API const int* column_widths_;
inline const int* column_widths() {return column_widths_;}
inline void column_widths(const int* i) {column_widths_ = i;}

// images:
FL_API void drawimage(const uchar*, int,int,int,int, int delta=3, int ldelta=0);
FL_API void drawimagemono(const uchar*, int,int,int,int, int delta=1, int ld=0);
typedef void (*DrawImageCallback)(void*,int,int,int,uchar*);
FL_API void drawimage(DrawImageCallback, void*, int,int,int,int, int delta=3);
FL_API void drawimagemono(DrawImageCallback, void*, int,int,int,int, int delta=1);
FL_API uchar *readimage(uchar *p, int x,int y, int w, int h, int alpha=0);

// depreciated:
FL_API int draw_xpm(const char*const* data, int x, int y, Color=GRAY75);
FL_API int measure_xpm(const char*const* data, int &w, int &h);
FL_API void set_mask_bitmap(uchar **ppBitmap);
FL_API void scrollrect(int X, int Y, int W, int H, int dx, int dy,
		       void (*draw_area)(void*, int,int,int,int), void*);
FL_API void overlay_rect(int,int,int,int);
FL_API void overlay_clear();

}

#endif

//
// End of "$Id: draw.h,v 1.3 2003/02/21 18:16:30 spitzak Exp $".
//
