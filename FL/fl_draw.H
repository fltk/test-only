//
// "$Id: fl_draw.H,v 1.28 2001/07/16 19:38:17 robertk Exp $"
//
// The fltk drawing library
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
#include <FL/x.H>

// current transformation:
extern FL_API int fl_x_;
extern FL_API int fl_y_;
FL_API void fl_push_matrix();
FL_API void fl_pop_matrix();
FL_API void fl_scale(double x, double y);
FL_API void fl_scale(double x);
FL_API void fl_translate(double x, double y);
FL_API void fl_rotate(double d);
FL_API void fl_mult_matrix(double, double, double, double, double, double);

// get and use transformed positions:
FL_API double fl_transform_x(double x, double y);
FL_API double fl_transform_y(double x, double y);
FL_API double fl_transform_dx(double x, double y);
FL_API double fl_transform_dy(double x, double y);
FL_API void fl_transformed_vertex(double x, double y);

// clip:
FL_API void fl_push_clip(int x, int y, int w, int h);
FL_API void fl_clip_out(int x, int y, int w, int h);
FL_API void fl_push_no_clip();
FL_API void fl_pop_clip();
FL_API int fl_not_clipped(int x, int y, int w, int h);
FL_API int fl_clip_box(int, int, int, int, int& x, int& y, int& w, int& h);

// Colors:
FL_API void fl_color(Fl_Color);
inline void fl_color(uchar r, uchar g, uchar b) {fl_color(fl_rgb(r,g,b));}
extern FL_API Fl_Color fl_color_;
inline Fl_Color fl_color() {return fl_color_;}

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

// Build the path:
FL_API void fl_newpath();
FL_API void fl_vertex(double x, double y);
FL_API void fl_vertex(int x, int y); // ignores scale & rotation!
FL_API void fl_closepath();
FL_API void fl_curve(double,double,double,double,double,double,double,double);
FL_API void fl_arc(double x,double y,double w,double h, double a1, double a2);
FL_API void fl_circle(double x, double y, double r);
FL_API void fl_ellipse(double x, double y, double w, double h);

// draw the path:
FL_API void fl_points();
FL_API void fl_stroke();
FL_API void fl_fill();
FL_API void fl_fill_stroke(Fl_Color);

// drawing that bypasses the path mechanism:
FL_API void fl_rect(int x, int y, int w, int h);
FL_API void fl_rectf(int x, int y, int w, int h);
FL_API void fl_point(int x, int y);
FL_API void fl_line(int,int, int,int);
enum {FL_PIE, FL_CHORD, FL_ARC};
FL_API void fl_pie(int x,int y,int w,int h,double a,double a2,int what=FL_PIE);

// current font+size:
FL_API void fl_font(Fl_Font, unsigned size);
FL_API void fl_font(const char*, unsigned size);
FL_API void fl_font(const char*, int attributes, unsigned size);

// change the encoding used to draw bytes (depreciated)
FL_API void fl_encoding(const char*);
extern FL_API const char* fl_encoding_;
inline const char* fl_encoding() {return fl_encoding_;}

// information you can get about the current font+size+encoding:
extern FL_API Fl_Font fl_font_;
extern FL_API unsigned fl_size_;
inline Fl_Font fl_font() {return fl_font_;}
inline unsigned fl_size() {return fl_size_;}
FL_API const char *fl_fontname(Fl_Font, int * = 0);

// Ideal case for an abstract base class, but this is faster
typedef void (*Fl_FontFunc)(Fl_Font, unsigned);
typedef void * (*Fl_Font_LoadFunc)(const char *, const char *, int);
typedef void (*Fl_Font_UnloadFunc)(void *);
typedef int (*Fl_Font_HeightFunc)();
typedef int (*Fl_Font_DescentFunc)();
typedef int (*Fl_Font_WidthFunc)(const char *, int);
typedef void (*Fl_Font_DrawFunc)(const char *, int, int, int);
typedef void (*Fl_Font_ClipFunc)(Region);
typedef int (*Fl_Font_ListFunc)(Fl_Font *&);
//typedef void * (*Fl_Font_FindFunc)(const char *, const char *);
//typedef int (*Fl_Font_StrlenFunc)(const char *);

struct FL_API Fl_Font_Renderer {
  Fl_FontFunc         font;     // set the FLTK font
  Fl_Font_LoadFunc    load;     // load a font
  Fl_Font_UnloadFunc  unload;   // unload a font
  Fl_Font_HeightFunc  height;   // get height of font.  May not equal size!
  Fl_Font_DescentFunc descent;  // get descent of font
  Fl_Font_WidthFunc   width;    // get pixel width of string
  Fl_Font_DrawFunc    draw;     // draw a string
  Fl_Font_ClipFunc    clip;     // set clipping region for font drawing
  Fl_Font_ListFunc    list;     // list fonts renderer knows about
//Fl_Font_FindFunc    find;     // find the font that matches
//Fl_Font_StrlenFunc  strlen;   // calculate char length of properly terminated string
  Fl_Font_            *fonts;   // pointer to fonts array
};
extern FL_API Fl_Font_Renderer* fl_font_renderer;

// information you can get about the current font+size+encoding:
FL_API int fl_height();	// using fl_size() does not work!
FL_API int fl_descent();

// measure things in the current font:
FL_API int fl_width(const char*);
FL_API int fl_width(const char*, int n);
FL_API int fl_width(uchar);
inline int fl_height(Fl_Font f, int s) { fl_font(f, s); return fl_height(); }
// draw using current font:
FL_API void fl_draw(const char*, int x, int y);
FL_API void fl_draw(const char*, int n, int x, int y);

// the "fancy" text formatter:
FL_API void fl_measure(const char*, int& w, int& h);
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

// XPM interpreter (probably should be part of Fl_Pixmap):
FL_API int fl_draw_pixmap(const char*const* data, int x,int y,Fl_Color=FL_GRAY);
FL_API int fl_measure_pixmap(const char*const* data, int &w, int &h);
FL_API void fl_set_mask_bitmap(uchar **ppBitmap);

// for GUI use only:
extern FL_API void fl_scroll(int X, int Y, int W, int H, int dx, int dy,
			     void (*draw_area)(void*, int,int,int,int), void*);
FL_API void fl_overlay_rect(int,int,int,int);
FL_API void fl_overlay_clear();
FL_API void fl_cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE);

// XForms symbols (should not be here...)
FL_API int fl_draw_symbol(const char* label,int x,int y,int w,int h, Fl_Color);
FL_API int fl_add_symbol(const char* name, void (*drawit)(Fl_Color), int scalable);

#ifndef FLTK_2	// back compatability only:
#define fl_clip(x,y,w,h) fl_push_clip(x,y,w,h)
#define fl_begin() fl_newpath()
//void fl_frame(const char* s, int x, int y, int w, int h);
//void fl_frame2(const char* s, int x, int y, int w, int h);
#define fl_draw_box(b,x,y,w,h,c) ((b)->draw(x,y,w,h,c))
#define fl_shortcut_label(l) Fl::key_name(l)
#define fl_begin_points()
#define fl_end_points() fl_draw_points()
#define fl_begin_line()
#define fl_end_line() fl_stroke()
#define fl_begin_loop()
#define fl_end_loop() fl_closepath();fl_stroke()
#define fl_begin_polygon()
#define fl_end_polygon() fl_fill()
#define fl_begin_complex_polygon()
#define fl_end_complex_polygon() fl_fill()
#define fl_gap() fl_closepath()
// defining this messes up the above fl_arc function:
//#define fl_arc(x,y,w,h,a,b) fl_pie(x,y,w,h,a,b,FL_ARC)
//#define fl_chord(x,y,w,h,a,b) fl_pie(x,y,w,h,a,b,FL_CHORD)
inline void fl_rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b) {
  fl_rectf(x,y,w,h,fl_rgb(r,g,b));}
#if 0 // these have no emulation
FL_API void fl_line(int,int, int,int, int,int);
FL_API void fl_loop(int,int, int,int, int,int);
FL_API void fl_loop(int,int, int,int, int,int, int,int);
FL_API void fl_polygon(int,int, int,int, int,int);
FL_API void fl_polygon(int,int, int,int, int,int, int,int);
FL_API void fl_xyline(int x, int y, int x1);
FL_API void fl_xyline(int x, int y, int x1, int y2);
FL_API void fl_xyline(int x, int y, int x1, int y2, int x3);
FL_API void fl_yxline(int x, int y, int y1);
FL_API void fl_yxline(int x, int y, int y1, int x2);
FL_API void fl_yxline(int x, int y, int y1, int x2, int y3);
#endif
#endif

#endif

//
// End of "$Id: fl_draw.H,v 1.28 2001/07/16 19:38:17 robertk Exp $".
//
