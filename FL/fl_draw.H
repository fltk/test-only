//
// "$Id: fl_draw.H,v 1.17 1999/10/17 22:27:07 vincent Exp $"
//
// Portable drawing function header file for the Fast Light Tool Kit (FLTK).
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

#ifndef fl_draw_H
#define fl_draw_H

#include "Fl_Flags.H" // for alignment values
#include "Fl_Color.H"
#include "Fl_Font.H"

// Colors:
FL_API void fl_color(Fl_Color);
inline void fl_color(uchar r, uchar g, uchar b) {fl_color(fl_rgb(r,g,b));}
extern FL_API Fl_Color fl_color_;
inline Fl_Color fl_color() {return fl_color_;}

// clip:
FL_API void fl_clip(int x, int y, int w, int h);
FL_API void fl_clip_out(int x, int y, int w, int h);
FL_API void fl_push_no_clip();
FL_API void fl_pop_clip();
FL_API int fl_not_clipped(int x, int y, int w, int h);
FL_API int fl_clip_box(int, int, int, int, int& x, int& y, int& w, int& h);

// line type:
FL_API void fl_line_style(int style, int width=0, char* dashes=0);
enum {
  FL_SOLID	= 0,
  FL_DASH	= 1,
  FL_DOT	= 2,
  FL_DASHDOT	= 3,
  FL_DASHDOTDOT	= 4,

  FL_CAP_FLAT	= 0x100,
  FL_CAP_ROUND	= 0x200,
  FL_CAP_SQUARE	= 0x300,

  FL_JOIN_MITER	= 0x1000,
  FL_JOIN_ROUND	= 0x2000,
  FL_JOIN_BEVEL	= 0x3000
};

// points:
FL_API void fl_point(int x, int y);

// rectangles tweaked to exactly fill the pixel rectangle:
FL_API void fl_rect(int x, int y, int w, int h);
FL_API void fl_rectf(int x, int y, int w, int h);

// line segments:
FL_API void fl_line(int,int, int,int);
FL_API void fl_line(int,int, int,int, int,int);

// closed line segments:
FL_API void fl_loop(int,int, int,int, int,int);
FL_API void fl_loop(int,int, int,int, int,int, int,int);

// filled polygons
FL_API void fl_polygon(int,int, int,int, int,int);
FL_API void fl_polygon(int,int, int,int, int,int, int,int);

// draw rectilinear lines, horizontal segment first:
FL_API void fl_xyline(int x, int y, int x1);
FL_API void fl_xyline(int x, int y, int x1, int y2);
FL_API void fl_xyline(int x, int y, int x1, int y2, int x3);

// draw rectilinear lines, vertical segment first:
FL_API void fl_yxline(int x, int y, int y1);
FL_API void fl_yxline(int x, int y, int y1, int x2);
FL_API void fl_yxline(int x, int y, int y1, int x2, int y3);

// circular lines and pie slices (code in fl_arci.C):
FL_API void fl_arc(int x, int y, int w, int h, double a1, double a2);
FL_API void fl_pie(int x, int y, int w, int h, double a1, double a2);
FL_API void fl_chord(int x, int y, int w, int h, double a1, double a2); // nyi

// scalable drawing code (code in fl_vertex.C and fl_arc.C):
FL_API void fl_push_matrix();
FL_API void fl_pop_matrix();
FL_API void fl_scale(double x, double y);
FL_API void fl_scale(double x);
FL_API void fl_translate(double x, double y);
FL_API void fl_rotate(double d);
FL_API void fl_mult_matrix(double a, double b, double c, double d, double x,double y);
FL_API void fl_begin_points();
FL_API void fl_begin_line();
FL_API void fl_begin_loop();
FL_API void fl_begin_polygon();
FL_API void fl_vertex(double x, double y);
FL_API void fl_curve(double, double, double, double, double, double, double, double);
FL_API void fl_arc(double x, double y, double r, double start, double a);
FL_API void fl_circle(double x, double y, double r);
FL_API void fl_end_points();
FL_API void fl_end_line();
FL_API void fl_end_loop();
FL_API void fl_end_polygon();
FL_API void fl_begin_complex_polygon();
FL_API void fl_gap();
FL_API void fl_end_complex_polygon();
// get and use transformed positions:
FL_API double fl_transform_x(double x, double y);
FL_API double fl_transform_y(double x, double y);
FL_API double fl_transform_dx(double x, double y);
FL_API double fl_transform_dy(double x, double y);
FL_API void fl_transformed_vertex(double x, double y);

// current font+size+encoding:
extern FL_API const char* fl_encoding;
FL_API void fl_font(const Fl_Font, unsigned size);
FL_API void fl_font(const Fl_Font, unsigned size, const char* encoding);
extern FL_API Fl_FontSize* fl_font_;
inline Fl_FontSize* fl_font() {return fl_font_;}
FL_API void fl_font(Fl_FontSize*);

// information you can get about the current font+size+encoding:
FL_API int   fl_height();	// using "size" should work ok
FL_API int   fl_descent();
FL_API double fl_width(const char*);
FL_API double fl_width(const char*, int n);
FL_API double fl_width(uchar);

// draw using current font:
FL_API void fl_draw(const char*, int x, int y);
FL_API void fl_draw(const char*, int n, int x, int y);
FL_API void fl_measure(const char*, int& x, int& y);
FL_API void fl_draw(const char*, int,int,int,int, Fl_Flags);
FL_API void fl_draw(const char*, int,int,int,int, Fl_Flags,
	               void (*callthis)(const char *, int n, int x, int y));

// images:
FL_API void fl_draw_image(const uchar*, int,int,int,int, int delta=3, int ldelta=0);
FL_API void fl_draw_image_mono(const uchar*, int,int,int,int, int delta=1, int ld=0);
typedef void (*Fl_Draw_Image_Cb)(void*,int,int,int,uchar*);
FL_API void fl_draw_image(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=3);
FL_API void fl_draw_image_mono(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=1);
FL_API void fl_rectf(int x, int y, int w, int h, Fl_Color);
inline void fl_rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b) {
  fl_rectf(x,y,w,h,fl_rgb(r,g,b));}

// pixmaps:
FL_API int fl_draw_pixmap(const char*const* data, int x,int y,Fl_Color=FL_GRAY);
FL_API int fl_measure_pixmap(const char*const* data, int &w, int &h);

// other:
extern FL_API void fl_scroll(int X, int Y, int W, int H, int dx, int dy,
	                        void (*draw_area)(void*, int,int,int,int), void* data);
FL_API const char* fl_shortcut_label(int);
FL_API void fl_overlay_rect(int,int,int,int);
FL_API void fl_overlay_clear();
FL_API void fl_cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE);

// XForms symbols:
FL_API int fl_draw_symbol(const char* label,int x,int y,int w,int h, Fl_Color);
FL_API int fl_add_symbol(const char* name, void (*drawit)(Fl_Color), int scalable);

// back compatability only:
//void fl_frame(const char* s, int x, int y, int w, int h);
//void fl_frame2(const char* s, int x, int y, int w, int h);
#define fl_draw_box(b,x,y,w,h,c) ((b)->draw(x,y,w,h,c))

#endif

//
// End of "$Id: fl_draw.H,v 1.17 1999/10/17 22:27:07 vincent Exp $".
//
