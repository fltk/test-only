//
// "$Id: fl_draw.H,v 1.9.2.6.2.7.2.2 2003/11/07 03:47:22 easysw Exp $"
//
// Portable drawing function header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#ifndef fl_draw_H
#define fl_draw_H


#include "Enumerations.H"  // for the color names
#include "Fl_Fltk.H"

// Image class...
class Fl_Image;

// Label flags...
FL_EXPORT extern char fl_draw_shortcut;

// Colors:

inline void	fl_color(Fl_Color c){fl->color(c);}; // select indexed color
inline void fl_color(int c) {fl_color((Fl_Color)c);} // for back compatability
inline void	fl_color(uchar r, uchar g , uchar b){fl->color(r,g,b);}; // select actual color
extern FL_EXPORT Fl_Color fl_color_;
inline Fl_Color fl_color(){return fl->color();}



// clip:

inline void fl_clip(int x, int y, int w, int h){fl->clip(x,y,w,h);};
inline void fl_clip(double x, double y, double w, double h){fl->clip(x,y,w,h);};  //moreprecise clipping
#define fl_push_clip fl_clip
inline void fl_push_no_clip(){fl->push_no_clip();};
inline void fl_pop_clip(){fl->pop_clip();};
inline int fl_not_clipped(int x, int y, int w, int h){return fl->not_clipped(x,y,w,h);};
inline int fl_not_clipped(double x, double y, double w, double h){return fl->not_clipped(x,y,w,h);};
inline int fl_clip_box(int _x, int _y, int _w, int _h, int& x, int& y, int& w, int& h){return fl->clip_box(_x, _y, _w, _h, x, y, w, h);};
inline int fl_clip_box(double _x, double _y, double _w, double _h, double& x, double& y, double& w, double& h){return fl->clip_box(_x, _y, _w, _h, x, y, w, h);};


// points:
inline void fl_point(int x, int y){fl->point(x,y);};

// line type:
inline void fl_line_style(int style, int width=0, char* dashes=0){fl->line_style(style,width,dashes);};


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
inline void fl_rect(int x, int y, int w, int h){fl->rect(x,y,w,h);};
inline void fl_rectf(int x, int y, int w, int h){fl->rectf(x,y,w,h);};

inline void fl_rect(int x, int y, int w, int h, Fl_Color c) {fl_color(c); fl_rect(x,y,w,h);}
inline void fl_rectf(int x, int y, int w, int h, Fl_Color c) {fl_color(c); fl_rectf(x,y,w,h);}

// line segments:
inline void fl_line(int x1, int y1, int x2, int y2){fl->line(x1, y1, x2, y2);};
inline void fl_line(int x1, int y1, int x2, int y2, int x3, int y3){fl->line(x1, y1, x2, y2, x3, y3);};

// closed line segments:
inline void fl_loop(int x1, int y1, int x2 ,int y2, int x3, int y3){fl->loop(x1, y1, x2, y2, x3, y3);};
inline void fl_loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4){fl->loop(x1, y1, x2, y2, x3, y3, x4, y4);};

// filled polygons
inline void fl_polygon(int x1, int y1, int x2 ,int y2, int x3, int y3){fl->polygon(x1, y1, x2, y2, x3, y3);};
inline void fl_polygon(int x1 ,int y1, int x2, int y2, int x3, int y3, int x4, int y4){fl->polygon(x1, y1, x2, y2, x3, y3, x4, y4);};

// draw rectilinear lines, horizontal segment first:
inline void fl_xyline(int x, int y, int x1){fl->xyline(x,y,x1);};
inline void fl_xyline(int x, int y, int x1, int y2){fl->xyline(x,y,x1,y2);};
inline void fl_xyline(int x, int y, int x1, int y2, int x3){fl->xyline(x,y,x1,y2,x3);};

// draw rectilinear lines, vertical segment first:
inline void fl_yxline(int x, int y, int y1){fl->yxline(x,y,y1);};
inline void fl_yxline(int x, int y, int y1, int x2){fl->yxline(x,y,y1,x2);};
inline void fl_yxline(int x, int y, int y1, int x2, int y3){fl->yxline(x,y,y1,x2,y3);};




// circular lines and pie slices (code in fl_arci.C):

inline void fl_arc(int x, int y, int w, int h, double a1, double a2){fl->arc(x,y,w,h,a1,a2);};
inline void fl_pie(int x, int y, int w, int h, double a1, double a2){fl->pie(x,y,w,h,a1,a2);};

FL_EXPORT void fl_chord(int x, int y, int w, int h, double a1, double a2); // nyi

// scalable drawing code (code in fl_vertex.C and fl_arc.C):
FL_EXPORT void fl_push_matrix();
FL_EXPORT void fl_pop_matrix();
FL_EXPORT void fl_scale(double x, double y);
FL_EXPORT void fl_scale(double x);
inline void fl_translate(double x, double y){fl->translate(x, y);};
FL_EXPORT void fl_rotate(double d);
FL_EXPORT void fl_mult_matrix(double a, double b, double c, double d, double x,double y);


inline void fl_begin_points(){fl->begin_points();};
inline void fl_begin_line(){fl->begin_line();};
inline void fl_begin_loop(){fl->begin_loop();};
inline void fl_begin_polygon(){fl->begin_polygon();};
inline void fl_vertex(double x, double y){fl->vertex(x,y);};
inline void fl_curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3){fl->curve(x,y,x1,y1,x2,y2,x3,y3);};
inline void fl_arc(double x, double y, double r, double start, double a){fl->arc(x,y,r,start,a);};
inline void fl_circle(double x, double y, double r){fl->circle(x,y,r);};
inline void fl_end_points(){fl->end_points();};
inline void fl_end_line(){fl->end_line();};
inline void fl_end_loop(){fl->end_loop();};
inline void fl_end_polygon(){fl->end_polygon();};
inline void fl_begin_complex_polygon(){fl->begin_complex_polygon();};
inline void fl_gap(){fl->gap();};
inline void fl_end_complex_polygon(){fl->end_complex_polygon();};
inline void fl_transformed_vertex(double x, double y){fl->transformed_vertex(x,y);};





// get and use transformed positions:
FL_EXPORT double fl_transform_x(double x, double y);
FL_EXPORT double fl_transform_y(double x, double y);
FL_EXPORT double fl_transform_dx(double x, double y);
FL_EXPORT double fl_transform_dy(double x, double y);
//FL_EXPORT void fl_transformed_vertex(double x, double y);


inline void fl_matrix(double& a, double& b, double& c, double& d, double &x, double& y){
	a=fl_transform_dx(1,0);	c=fl_transform_dx(0,1);	b=fl_transform_dy(1,0);	d=fl_transform_dy(0,1);	x=fl_transform_x(0,0);	y=fl_transform_y(0,0);
}


// current font:

inline void fl_font(int face, int size){fl->font(face,size);};
extern FL_EXPORT int fl_font_;
inline int fl_font() {return fl->font();};
extern FL_EXPORT int fl_size_;
inline int fl_size() {return fl->size();};


// information you can get about the current font:

inline int   fl_height(){return fl->height();};	// using "size" should work ok
inline int fl_height(int, int size) {return size;}

inline int   fl_descent(){return fl->descent();};
inline double fl_width(const char* s){return fl->width(s);};
inline double fl_width(const char* s, int n){return fl->width(s,n);};
inline double fl_width(unsigned int c){return fl->width(c);};


// draw using current font:

inline void fl_draw(const char* s, int x, int y){fl->draw(s,x,y);};
inline void fl_draw(const char* s, int n, int x, int y){fl->draw(s,n,x,y);};
inline void fl_rtl_draw(const char* s, int n, int x, int y){fl->rtl_draw(s,n,x,y);};
inline void fl_transformed_draw(const char* s, int n, double x, double y){fl->transformed_draw(s,n,x,y);};
inline void fl_transformed_draw(const char* s, double x, double y){fl->transformed_draw(s,x,y);};


FL_EXPORT void fl_measure(const char*, int& x, int& y, int draw_symbols = 1);
FL_EXPORT void fl_draw(const char*, int,int,int,int, Fl_Align, Fl_Image* img=0,
                       int draw_symbols = 1);
FL_EXPORT void fl_draw(const char*, int,int,int,int, Fl_Align,
	               void (*callthis)(const char *, int n, int x, int y),
		       Fl_Image* img=0, int draw_symbols = 1);
FL_EXPORT void fl_set_status(int X, int Y, int W, int H);
FL_EXPORT void fl_set_spot(int font, int size, int x, int y, int w, int h);
FL_EXPORT void fl_reset_spot(void);

// boxtypes:
FL_EXPORT void fl_frame(const char* s, int x, int y, int w, int h);
FL_EXPORT void fl_frame2(const char* s, int x, int y, int w, int h);
FL_EXPORT void fl_draw_box(Fl_Boxtype, int x, int y, int w, int h, Fl_Color);

// images:

inline void fl_draw_image(const uchar* im, int x,int y,int w,int h, int delta=3, int ldelta=0){fl->draw_image(im,x,y,w,h,delta,ldelta);};
inline void fl_draw_image_mono(const uchar* im, int x,int y,int w,int h, int delta=1, int ld=0){fl->draw_image_mono(im,x,y,w,h,delta,ld);};
typedef void (*Fl_Draw_Image_Cb)(void*,int,int,int,uchar*);
inline void fl_draw_image(Fl_Draw_Image_Cb cb, void* im, int x,int y,int w,int h, int delta=3){fl->draw_image(cb, im, x,y,w,h,delta);};
inline void fl_draw_image_mono(Fl_Draw_Image_Cb cb, void* im, int x,int y,int w,int h, int delta=1){fl->draw_image(cb, im, x,y,w,h,delta);};
inline void fl_rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b){fl->rectf(x,y,w,h,r,g,b);};


// pixmaps:
FL_EXPORT int fl_draw_pixmap(/*const*/ char* const* data, int x,int y,Fl_Color=FL_GRAY);
FL_EXPORT int fl_measure_pixmap(/*const*/ char* const* data, int &w, int &h);
FL_EXPORT int fl_draw_pixmap(const char* const* data, int x,int y,Fl_Color=FL_GRAY);
FL_EXPORT int fl_measure_pixmap(const char* const* data, int &w, int &h);

// other:
extern FL_EXPORT void fl_scroll(int X, int Y, int W, int H, int dx, int dy,
                         void (*draw_area)(void*, int,int,int,int), void* data);
FL_EXPORT const char* fl_shortcut_label(int);
FL_EXPORT void fl_overlay_rect(int,int,int,int);
FL_EXPORT void fl_overlay_clear();
FL_EXPORT void fl_cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE);

// XForms symbols:
FL_EXPORT int fl_draw_symbol(const char* label,int x,int y,int w,int h, Fl_Color);
FL_EXPORT int fl_add_symbol(const char* name, void (*drawit)(Fl_Color), int scalable);

#endif


//
// End of "$Id: fl_draw.H,v 1.9.2.6.2.7.2.2 2003/11/07 03:47:22 easysw Exp $".
//
