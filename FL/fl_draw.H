//
// "$Id$"
//
// Portable drawing function header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper finished
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

#ifndef fl_draw_H
#define fl_draw_H

#include <fltk3/draw.h>
#include <FL/x.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Device.H>

class Fl_Image;

// FL_EXPORT extern char fl_draw_shortcut;

inline void fl_color(Fl_Color c) {
  fltk3::color(c);
}

inline void fl_color(int c) {
  fltk3::color(c);
}

inline void fl_color(uchar r, uchar g, uchar b) {
  fltk3::color(r, g, b);
}

inline Fl_Color fl_color() {
  return fltk3::_3to1_color(fltk3::color());
}

inline void fl_push_clip(int x, int y, int w, int h) {
  fltk3::push_clip(x, y, w, h);
}

inline void fl_push_no_clip() {
  fltk3::push_no_clip();
}

inline void fl_pop_clip() {
  fltk3::pop_clip();
}

inline int fl_not_clipped(int x, int y, int w, int h) {
  return fltk3::not_clipped(x, y, w, h);
}

inline int fl_clip_box(int x , int y, int w, int h, int& X, int& Y, int& W, int& H) {
  return fltk3::clip_box(x, y, w, h, X, Y, W, H);
}

inline void fl_restore_clip() {
  fltk3::restore_clip();
}

inline void fl_clip_region(Fl_Region r) {
  fltk3::clip_region((fltk3::Region)r);
}

inline Fl_Region fl_clip_region() { 
  return (Fl_Region)fltk3::clip_region();
}

inline void fl_point(int x, int y) {
  fltk3::point(x, y);
}

inline void fl_line_style(int style, int width=0, char* dashes=0) {
  fltk3::line_style(style, width, dashes);
}

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

inline void fl_rect(int x, int y, int w, int h) {
  fltk3::rect(x, y, w, h);
}

inline void fl_rect(int x, int y, int w, int h, Fl_Color c) {
  fltk3::rect(x, y, w, h, c);
}

inline void fl_rectf(int x, int y, int w, int h) { 
  fltk3::rectf(x, y, w, h);
}

inline void fl_rectf(int x, int y, int w, int h, Fl_Color c) {
  fltk3::rectf(x, y, w, h, c);
}

inline void fl_rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b) {
  fltk3::rectf(x, y, w, h, r, g, b);
}

inline void fl_line(int x, int y, int x1, int y1) {
  fltk3::line(x, y, x1, y1);
}

inline void fl_line(int x, int y, int x1, int y1, int x2, int y2) {
  fltk3::line(x, y, x1, y1, x2, y2);
}

inline void fl_loop(int x, int y, int x1, int y1, int x2, int y2) {
  fltk3::loop(x, y, x1, y1, x2, y2);
}

inline void fl_loop(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  fltk3::loop(x, y, x1, y1, x2, y2, x3, y3);
}

inline void fl_polygon(int x, int y, int x1, int y1, int x2, int y2) {
  fltk3::polygon(x,y,x1,y1,x2,y2); 
}

inline void fl_polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  fltk3::polygon(x,y,x1,y1,x2,y2,x3,y3); 
}

inline void fl_xyline(int x, int y, int x1) {
  fltk3::xyline(x,y,x1);
}

inline void fl_xyline(int x, int y, int x1, int y2) {
  fltk3::xyline(x,y,x1,y2);
}

inline void fl_xyline(int x, int y, int x1, int y2, int x3) {
  fltk3::xyline(x,y,x1,y2,x3);
}

inline void fl_yxline(int x, int y, int y1) {
  fltk3::yxline(x,y,y1);
}

inline void fl_yxline(int x, int y, int y1, int x2) {
  fltk3::yxline(x,y,y1,x2);
}

inline void fl_yxline(int x, int y, int y1, int x2, int y3) {
  fltk3::yxline(x,y,y1,x2,y3);
}

inline void fl_arc(int x, int y, int w, int h, double a1, double a2) {
  fltk3::arc(x,y,w,h,a1,a2); 
}

inline void fl_pie(int x, int y, int w, int h, double a1, double a2) {
  fltk3::pie(x,y,w,h,a1,a2); 
}

inline void fl_chord(int x, int y, int w, int h, double a1, double a2) {
  fltk3::chord(x, y, w, h, a1, a2);
}

inline void fl_push_matrix() { 
  fltk3::push_matrix(); 
}

inline void fl_pop_matrix() { 
  fltk3::pop_matrix(); 
}

inline void fl_scale(double x, double y) { 
  fltk3::scale(x, y); 
}

inline void fl_scale(double x) { 
  fltk3::scale(x); 
}

inline void fl_translate(double x, double y) { 
  fltk3::translate(x, y); 
}

inline void fl_rotate(double d) { 
  fltk3::rotate(d); 
}

inline void fl_mult_matrix(double a, double b, double c, double d, double x,double y) {
  fltk3::mult_matrix(a, b, c, d, x, y); 
}

inline void fl_begin_points() {
  fltk3::begin_points(); 
}

inline void fl_begin_line() {
  fltk3::begin_line(); 
}

inline void fl_begin_loop() {
  fltk3::begin_loop(); 
}

inline void fl_begin_polygon() {
  fltk3::begin_polygon(); 
}

inline void fl_vertex(double x, double y) {
  fltk3::vertex(x,y); 
}

inline void fl_curve(double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3) {
  fltk3::curve(X0,Y0,X1,Y1,X2,Y2,X3,Y3); 
}

inline void fl_arc(double x, double y, double r, double start, double end) {
  fltk3::arc(x,y,r,start,end); 
}

inline void fl_circle(double x, double y, double r) {
  fltk3::circle(x,y,r); 
}

inline void fl_end_points() {
  fltk3::end_points(); 
}

inline void fl_end_line() {
  fltk3::end_line(); 
}

inline void fl_end_loop() {
  fltk3::end_loop(); 
}

inline void fl_end_polygon() {
  fltk3::end_polygon(); 
}

inline void fl_begin_complex_polygon() {
  fltk3::begin_complex_polygon(); 
}

inline void fl_gap() {
  fltk3::gap(); 
}

inline void fl_end_complex_polygon() {
  fltk3::end_complex_polygon(); 
}

inline double fl_transform_x(double x, double y) {
  return fltk3::transform_x(x, y); 
}

inline double fl_transform_y(double x, double y) {
  return fltk3::transform_y(x, y); 
}

inline double fl_transform_dx(double x, double y) {
  return fltk3::transform_dx(x, y); 
}

inline double fl_transform_dy(double x, double y) {
  return fltk3::transform_dy(x, y); 
}

inline void fl_transformed_vertex(double xf, double yf) {
  fltk3::transformed_vertex(xf,yf); 
}

inline void fl_font(Fl_Font face, Fl_Fontsize size) { 
  fltk3::font(fltk3::_1to3_font(face), fltk3::_1to3_fontsize(size)); 
}


inline Fl_Font fl_font() {
  return fltk3::_3to1_font( fltk3::font() );
}

inline Fl_Fontsize fl_size() {
  return fltk3::_3to1_fontsize( fltk3::size() );
}

inline int fl_height() {
  return fltk3::height();
}

inline int fl_height(int font, int size) {
  return fltk3::height(font, size);
}

inline int fl_descent() {
  return fltk3::descent();
}

inline double fl_width(const char* txt) {
  return fltk3::width(txt);
}

inline double fl_width(const char* txt, int n) {
  return fltk3::width(txt, n);
}

inline double fl_width(unsigned int c) {
  return fltk3::width(c);
}

inline void fl_text_extents(const char *s, int& dx, int& dy, int& w, int& h) {
  fltk3::text_extents(s, dx, dy, w, h);
}

inline void fl_text_extents(const char *t, int n, int& dx, int& dy, int& w, int& h) {
  fltk3::text_extents(t, n, dx, dy, w, h);
}

inline const char *fl_latin1_to_local(const char *t, int n=-1) {
  return fltk3::latin1_to_local(t, n);
}

inline const char *fl_local_to_latin1(const char *t, int n=-1) {
  return fltk3::local_to_latin1(t, n);
}

inline const char *fl_mac_roman_to_local(const char *t, int n=-1) {
  return fltk3::mac_roman_to_local(t, n);
}

inline const char *fl_local_to_mac_roman(const char *t, int n=-1) {
  return fltk3::local_to_mac_roman(t, n);
}

inline void fl_draw(const char* str, int x, int y) {
  fltk3::draw(str, x, y);
}

inline void fl_draw(int angle, const char* str, int x, int y) {
  fltk3::draw(angle, str, x, y);
}

inline void fl_draw(const char* str, int n, int x, int y) {
  fltk3::draw(str,n,x,y); 
}

inline void fl_draw(int angle,const char* str, int n, int x, int y) {
  fltk3::draw(angle,str,n,x,y); 
}

inline void fl_rtl_draw(const char* str, int n, int x, int y) {
  fltk3::rtl_draw(str,n,x,y); 
}

inline void fl_measure(const char* str, int& x, int& y, int draw_symbols = 1) {
  fltk3::measure(str, x, y, draw_symbols);
}

inline void fl_draw(const char* str, int x, int y, int w, int h, Fl_Align align, Fl_Image* img=0, int draw_symbols = 1) {
  fltk3::draw(str, x, y, w, h, fltk3::_1to3_align(align), fltk3::_1to3_image(img), draw_symbols);
}

#if 0 // FIXME: 123
inline void fl_draw(const char* str, int x, int y, int w, int h,
                       Fl_Align align,
                       void (*callthis)(const char *,int,int,int),
                    Fl_Image* img=0, int draw_symbols = 1) {
}

#endif

inline void fl_frame(const char* s, int x, int y, int w, int h) {
  fltk3::frame(s, x, y, w, h);
}

inline void fl_frame2(const char* s, int x, int y, int w, int h) {
  fltk3::frame2(s, x, y, w, h);
}

inline void fl_draw_box(Fl_Boxtype b, int x, int y, int w, int h, Fl_Color c) {
  fltk3::draw_box(fltk3::_1to3_boxtype(b), x, y, w, h, fltk3::_1to3_color(c));
}

inline void fl_draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0) { 
  fltk3::draw_image(buf, X, Y, W, H, D, L); 
}

inline void fl_draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0) { 
  fltk3::draw_image_mono(buf, X, Y, W, H, D, L); 
}

inline void fl_draw_image(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=3) {
  fltk3::draw_image(cb, data, X, Y, W, H, D); 
}


#if 0 // FIXME: 123

FL_EXPORT void fl_draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=1);

FL_EXPORT char fl_can_do_alpha_blending();

#endif

inline uchar *fl_read_image(uchar *p,int X,int Y,int W,int H,int alpha=0) {
  return fltk3::read_image(p, X, Y, W, H, alpha);
}

#if 0

FL_EXPORT int fl_draw_pixmap(/*const*/ char* const* data, int x,int y,Fl_Color=FL_GRAY);

FL_EXPORT int fl_draw_pixmap(const char* const* cdata, int x,int y,Fl_Color=FL_GRAY);

FL_EXPORT int fl_measure_pixmap(/*const*/ char* const* data, int &w, int &h);

FL_EXPORT int fl_measure_pixmap(const char* const* cdata, int &w, int &h);


FL_EXPORT void fl_scroll(int X, int Y, int W, int H, int dx, int dy,
                         void (*draw_area)(void*, int,int,int,int), void* data);
#endif

inline const char* fl_shortcut_label(unsigned int shortcut) {
  return fltk3::shortcut_label(shortcut);
}

inline const char* fl_shortcut_label(unsigned int shortcut, const char **eom) {
  return fltk3::shortcut_label(shortcut, eom);
}

inline unsigned int fl_old_shortcut(const char* s) {
  return fltk3::old_shortcut(s);
}

inline void fl_overlay_rect(int x,int y,int w,int h) {
  fltk3::overlay_rect(x, y, w, h);
}

inline void fl_overlay_clear() {
  fltk3::overlay_clear();
}

inline void fl_cursor(Fl_Cursor c, Fl_Color fg=FL_BLACK, Fl_Color bg=FL_WHITE) {
  fltk3::cursor((fltk3::Cursor)c, fltk3::_1to3_color(fg), fltk3::_1to3_color(bg));
}

inline const char* fl_expand_text(const char* from, char* buf, int maxbuf,
                                     double maxw, int& n, double &width,
                                     int wrap, int draw_symbols = 0) {
  return fltk3::expand_text(from, buf, maxbuf, maxw, n, width, wrap, draw_symbols);
}

inline void fl_set_status(int X, int Y, int W, int H) {
  fltk3::set_status(X, Y, W, H);
}

inline void fl_set_spot(int font, int size, int X, int Y, int W, int H, Fl_Window *win=0) {
  fltk3::set_spot(fltk3::_1to3_font(font), size, X, Y, W, H, fltk3::_1to3_window(win));
}

inline void fl_reset_spot(void) {
  fltk3::reset_spot();
}

inline int fl_draw_symbol(const char* label,int x,int y,int w,int h, Fl_Color c) {
  return fltk3::draw_symbol(label, x, y, w, h, fltk3::_1to3_color(c));
}

#if 0 // FIXME: 123

FL_EXPORT int fl_add_symbol(const char* name, void (*drawit)(Fl_Color), int scalable);

#endif


#endif

//
// End of "$Id$".
//
