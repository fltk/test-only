//
// "$Id$"
//
// Portable drawing function header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  RK: This  is modified fl_draw.H for Fl_Device support 



#ifndef fl_draw_H
#define fl_draw_H




#include "Enumerations.H"  // for the color names
#include "Fl_Device.H"


// Image class...
class Fl_Image;

// Label flags...
FL_EXPORT extern char fl_draw_shortcut;
extern FL_EXPORT Fl_Color fl_color_;


/** @defgroup drawing Drawing functions
*  This module covers the drawing functions that are provided with FLTK.
*
*/

/**	 @name Drawing Colors
*  @ingroup drawing
* \copydoc _drawing_colors
* @{
*/

/** 
*Function color
*
*/
inline void	fl_color(Fl_Color c){fl_device->color(c);}; // select indexed color

/** @} */


inline void fl_color(int c) {fl_color((Fl_Color)c);} // for back compatability
inline void	fl_color(uchar r, uchar g , uchar b){fl_device->color(r,g,b);}; // select actual color
inline Fl_Color fl_color(){return fl_device->color();}





// clip:

inline void fl_push_clip(int x, int y, int w, int h){fl_device->push_clip(x,y,w,h);};
#define fl_clip fl_push_clip
inline void fl_push_no_clip(){fl_device->push_no_clip();};
inline void fl_pop_clip(){fl_device->pop_clip();};
inline int fl_not_clipped(int x, int y, int w, int h){return fl_device->not_clipped(x,y,w,h);};
inline int fl_clip_box(int _x, int _y, int _w, int _h, int& x, int& y, int& w, int& h){return fl_device->clip_box(_x, _y, _w, _h, x, y, w, h);};



inline void fl_draw(Fl_Widget * w){w->redraw(); w->draw();};

// points:

inline void fl_point(int x, int y){fl_device->point(x,y);};

// line style:
inline void fl_line_style(int style, int width=0, char* dashes=0){fl_device->line_style(style,width,dashes);};


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


// rectangles tweaked to exactly fill the pixel rectangle:
inline void fl_rect(int x, int y, int w, int h){fl_device->rect(x,y,w,h);};
inline void fl_rectf(int x, int y, int w, int h){fl_device->rectf(x,y,w,h);};

inline void fl_rect(int x, int y, int w, int h, Fl_Color c) {fl_color(c); fl_rect(x,y,w,h);}
inline void fl_rectf(int x, int y, int w, int h, Fl_Color c) {fl_color(c); fl_rectf(x,y,w,h);}

// line segments:
inline void fl_line(int x1, int y1, int x2, int y2){fl_device->line(x1, y1, x2, y2);};
inline void fl_line(int x1, int y1, int x2, int y2, int x3, int y3){fl_device->line(x1, y1, x2, y2, x3, y3);};

// closed line segments:
inline void fl_loop(int x1, int y1, int x2 ,int y2, int x3, int y3){fl_device->loop(x1, y1, x2, y2, x3, y3);};
inline void fl_loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4){fl_device->loop(x1, y1, x2, y2, x3, y3, x4, y4);};

// filled polygons
inline void fl_polygon(int x1, int y1, int x2 ,int y2, int x3, int y3){fl_device->polygon(x1, y1, x2, y2, x3, y3);};
inline void fl_polygon(int x1 ,int y1, int x2, int y2, int x3, int y3, int x4, int y4){fl_device->polygon(x1, y1, x2, y2, x3, y3, x4, y4);};

// draw rectilinear lines, horizontal segment first:
inline void fl_xyline(int x, int y, int x1){fl_device->xyline(x,y,x1);};
inline void fl_xyline(int x, int y, int x1, int y2){fl_device->xyline(x,y,x1,y2);};
inline void fl_xyline(int x, int y, int x1, int y2, int x3){fl_device->xyline(x,y,x1,y2,x3);};

// draw rectilinear lines, vertical segment first:
inline void fl_yxline(int x, int y, int y1){fl_device->yxline(x,y,y1);};
inline void fl_yxline(int x, int y, int y1, int x2){fl_device->yxline(x,y,y1,x2);};
inline void fl_yxline(int x, int y, int y1, int x2, int y3){fl_device->yxline(x,y,y1,x2,y3);};




// circular lines and pie slices (code in fl_arci.C):

inline void fl_arc(int x, int y, int w, int h, double a1, double a2){fl_device->arc(x,y,w,h,a1,a2);};
inline void fl_pie(int x, int y, int w, int h, double a1, double a2){fl_device->pie(x,y,w,h,a1,a2);};

FL_EXPORT void fl_chord(int x, int y, int w, int h, double a1, double a2); // nyi

// scalable drawing code (code in fl_vertex.C and fl_arc.C):
FL_EXPORT void fl_push_matrix();
FL_EXPORT void fl_pop_matrix();
FL_EXPORT void fl_scale(double x, double y);
FL_EXPORT void fl_scale(double x);
FL_EXPORT void fl_translate(double x, double y);
FL_EXPORT void fl_rotate(double d);
FL_EXPORT void fl_mult_matrix(double a, double b, double c, double d, double x,double y);


inline void fl_begin_points(){fl_device->begin_points();};
inline void fl_begin_line(){fl_device->begin_line();};
inline void fl_begin_loop(){fl_device->begin_loop();};
inline void fl_begin_polygon(){fl_device->begin_polygon();};
inline void fl_vertex(double x, double y){fl_device->vertex(x,y);};
inline void fl_curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3){fl_device->curve(x,y,x1,y1,x2,y2,x3,y3);};
inline void fl_arc(double x, double y, double r, double start, double a){fl_device->arc(x,y,r,start,a);};
inline void fl_circle(double x, double y, double r){fl_device->circle(x,y,r);};
inline void fl_end_points(){fl_device->end_points();};
inline void fl_end_line(){fl_device->end_line();};
inline void fl_end_loop(){fl_device->end_loop();};
inline void fl_end_polygon(){fl_device->end_polygon();};
inline void fl_begin_complex_polygon(){fl_device->begin_complex_polygon();};
inline void fl_gap(){fl_device->gap();};
inline void fl_end_complex_polygon(){fl_device->end_complex_polygon();};
inline void fl_transformed_vertex(double x, double y){fl_device->transformed_vertex(x,y);};




// get and use transformed positions:
FL_EXPORT double fl_transform_x(double x, double y);
FL_EXPORT double fl_transform_y(double x, double y);
FL_EXPORT double fl_transform_dx(double x, double y);
FL_EXPORT double fl_transform_dy(double x, double y);
//FL_EXPORT void fl_transformed_vertex(double x, double y);





// current font:

inline void fl_font(int face, int size){fl_device->font(face,size);};
extern FL_EXPORT int fl_font_;
inline int fl_font() {return fl_device->font();};
extern FL_EXPORT int fl_size_;
inline int fl_size() {return fl_device->size();};


// information you can get about the current font:

inline int   fl_height(){return fl_device->height();};	// using "size" should work ok
inline int fl_height(int, int size) {return size;}

inline int   fl_descent(){return fl_device->descent();};
FL_EXPORT double fl_width(const char* s);
inline double fl_width(const char* s, int n){return fl_device->width(s,n);};
inline double fl_width(unsigned int c){return fl_device->width(c);};


// draw using current font:

//inline void fl_draw(const char* s, int x, int y){fl_device->draw(s,x,y);};
FL_EXPORT void fl_draw(const char* s, int x, int y);
inline void fl_draw(const char* s, int n, int x, int y){fl_device->draw(s,n,x,y);};
//inline void fl_transformed_draw(const char* s, int n, double x, double y){fl_device->transformed_draw(s,n,x,y);};
//inline void fl_transformed_draw(const char* s, double x, double y){fl_device->transformed_draw(s,x,y);};


FL_EXPORT void fl_measure(const char*, int& x, int& y, int draw_symbols = 1);
FL_EXPORT void fl_draw(const char*, int,int,int,int, Fl_Align, Fl_Image* img=0,
                       int draw_symbols = 1);
FL_EXPORT void fl_draw(const char*, int,int,int,int, Fl_Align,
	               void (*callthis)(const char *, int n, int x, int y),
		       Fl_Image* img=0, int draw_symbols = 1);



// boxtypes:
FL_EXPORT void fl_frame(const char* s, int x, int y, int w, int h);
FL_EXPORT void fl_frame2(const char* s, int x, int y, int w, int h);
FL_EXPORT void fl_draw_box(Fl_Boxtype, int x, int y, int w, int h, Fl_Color);

// images:

inline void fl_draw_image(const uchar* im, int x,int y,int w,int h, int delta=3, int ldelta=0){fl_device->draw_image(im,x,y,w,h,delta,ldelta);};
inline void fl_draw_image_mono(const uchar* im, int x,int y,int w,int h, int delta=1, int ld=0){fl_device->draw_image_mono(im,x,y,w,h,delta,ld);};
typedef void (*Fl_Draw_Image_Cb)(void* ,int,int,int,uchar*);
inline void fl_draw_image(Fl_Draw_Image_Cb cb, void* im, int x,int y,int w,int h, int delta=3){fl_device->draw_image(cb, im, x,y,w,h,delta);};
inline void fl_draw_image_mono(Fl_Draw_Image_Cb cb, void* im, int x,int y,int w,int h, int delta=1){fl_device->draw_image(cb, im, x,y,w,h,delta);};
inline void fl_rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b){fl_device->rectf(x,y,w,h,r,g,b);};

FL_EXPORT uchar *fl_read_image(uchar *p, int x,int y, int w, int h, int alpha=0);

// pixmaps:
FL_EXPORT int fl_draw_pixmap(/*const*/ char* const* data, int x,int y,Fl_Color=FL_GRAY);
FL_EXPORT int fl_draw_pixmap(/*const*/ char*const* di, int x, int y, uchar bg_r, uchar bg_g, uchar bg_b);
FL_EXPORT int fl_measure_pixmap(/*const*/ char* const* data, int &w, int &h);
FL_EXPORT int fl_draw_pixmap(const char* const* data, int x,int y,Fl_Color=FL_GRAY);
FL_EXPORT int fl_draw_pixmap(const char*const* di, int x, int y, uchar bg_r, uchar bg_g, uchar bg_b);
FL_EXPORT int fl_measure_pixmap(const char* const* data, int &w, int &h);

// other:
extern FL_EXPORT void fl_scroll(int X, int Y, int W, int H, int dx, int dy, void (*draw_area)(void*, int,int,int,int), void* data);
FL_EXPORT const char* fl_shortcut_label(int);
FL_EXPORT void fl_overlay_rect(int,int,int,int);
FL_EXPORT void fl_overlay_clear();
FL_EXPORT void fl_cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE);

// XForms symbols:
FL_EXPORT int fl_draw_symbol(const char* label,int x,int y,int w,int h, Fl_Color);
FL_EXPORT int fl_add_symbol(const char* name, void (*drawit)(Fl_Color), int scalable);


#endif

//
// End of "$Id$".
//

