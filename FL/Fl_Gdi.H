//
// "$Id: Fl_Gdi.H,v 1.1.2.1 2003/11/07 04:02:04 easysw Exp $"
//
// WIN32 GDI printing device for the Fast Light Tool Kit (FLTK).
//
// Copyright (c) 2002  O'ksi'D
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
// Please report all bugs and problems to "oksid@bluewin.ch".
//

#ifdef WIN32
#ifndef Fl_Gdi_H
#define Fl_Gdi_H
#include "Fl_Device.H"
#include <windows.h>
#include "Fl_Fltk.H"

extern FL_EXPORT Fl_Color fl_color_;
extern FL_EXPORT int fl_size_;
extern FL_EXPORT int fl_font_;
extern FL_EXPORT HDC fl_gc;

class FL_EXPORT Fl_Gdi: public Fl_Device
{
public:
	 int R;
	 int B;
	 int T1;
	 int L1;
	 double s1;
	 int page_;

	 void page(int lm=0, int tm=0, int rm=0, int bm=0, int orientation=PS_UNCHANGED, int format=PS_UNCHANGED);
	 void fit(int x, int y, int w, int h, int align=FL_ALIGN_CENTER);

	 void color(Fl_Color color) {
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.color(color);
		 fl_gc = o;
	 };
	 void color(uchar r, uchar g, uchar b) {
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.color(r,g,b);
		 fl_gc = o;
	 };
	 Fl_Color color(){return fl_color_;};
		 
	 void clip(int x, int y, int w, int h) {
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.clip((int)(x*s+L), (int)(y*s+T), (int)(w*s), (int)(h*s));
		 fl_gc = o;
	 };

	 void push_no_clip() {
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.push_no_clip();
		 fl_gc = o;
	 };
	 void pop_clip() { 
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.pop_clip();
		 fl_gc = o;
	 };
	 
	 int not_clipped(int x, int y, int w, int h) {
		 o = fl_gc;
		 fl_gc = gc;
		 int ret = fltk.not_clipped((int)(x*s+L),(int)(y*s+T),(int)(w*s),(int)(h*s));
		 fl_gc = o;
		 return ret;
	 };
	 int clip_box(int _x, int _y, int _w, int _h, int& x, int& y, int& w, int& h) {
		 x= (int)(x*s);y=(int)(y*s);w=(int)(y*s);h=(int)(h*s);
		 x += L; y += T;
		 o = fl_gc;
		 fl_gc = gc;
		 int ret = fltk.clip_box((int)(_x*s+L), (int)(_y*s+T), (int)(_w*s), (int)(_h*s), x, y, w, h);
		 x -= L; y -= T;
		 x = (int)(x/s); y =(int)(y/s); w = (int)(w/s); h = (int)(h/s);
		 fl_gc = o;	 
		 return ret;
	 };
	
	 void point(int x, int y) {
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.point((int)(x*s+L), (int)(y*s+T));
		 fl_gc = o;
	 };

	 void line_style(int style, int width, char* dashes){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.line_style(style, (int)(width * s), dashes);
		 fl_gc = o;
	 };

	 void rect(int x, int y, int w, int h){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.rect((int)(x*s+L), (int)(y*s+T), (int)(w*s), (int)(h*s));
		 fl_gc = o;
	 };
	 void rectf(int x, int y, int w, int h){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.rectf((int)(x*s+L), (int)(y*s+T), (int)(w*s), (int)(h*s));
		 fl_gc = o;
	 };

	 void line(int x1, int y1, int x2, int y2){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.line((int)(x1*s+L),(int)(y1*s+T),(int)(x2*s+L),(int)(y2*s+T));
		 fl_gc = o;
	 };
	 void line(int x1, int y1, int x2, int y2, int x3, int y3){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.line((int)(x1*s+L),(int)(y1*s+T),(int)(x2*s+L),
			 (int)(y2*s+T),(int)(x3*s+L),(int)(y3*s+T));
		 fl_gc = o;
	 };

	 void loop(int x1, int y1, int x2 ,int y2, int x3, int y3){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.loop((int)(x1*s+L), (int)(y1*s+T), (int)(x2*s+L), 
			 (int)(y2*s+T), (int)(x3*s+L), (int)(y3*s+T));
		 fl_gc = o;
	 };
	 void loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.loop((int)(x1*s+L), (int)(y1*s+T), (int)(x2*s+L), 
			 (int)(y2*s+T), (int)(x3*s+L), (int)(y3*s+T), 
			 (int)(x4*s+L), (int)(y4*s+T));
		 fl_gc = o;
	 };

	 void polygon(int x1, int y1, int x2 ,int y2, int x3, int y3){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.polygon((int)(x1*s+L), (int)(y1*s+T), (int)(x2*s+L), 
			 (int)(y2*s+T), (int)(x3*s+L), (int)(y3*s+T));
		 fl_gc = o;
	 };
	 void polygon(int x1 ,int y1, int x2, int y2, int x3, int y3, int x4, int y4){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.polygon((int)(x1*s+L), (int)(y1*s+T), (int)(x2*s+L), 
			 (int)(y2*s+T), (int)(x3*s+L), (int)(y3*s+T), 
			 (int)(x4*s+L), (int)(y4*s+T));
		 fl_gc = o;
	 };

	 void xyline(int x, int y, int x1){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.xyline((int)(x*s+L), (int)(y*s+T), (int)(x1*s+L));
		 fl_gc = o;
	 };
	 void xyline(int x, int y, int x1, int y2){
		  o = fl_gc;
		 fl_gc = gc;
		 fltk.xyline((int)(x*s+L), (int)(y*s+T), (int)(x1*s+L), 
			 (int)(y2*s+T));
		 fl_gc = o;
	 };
	 void xyline(int x, int y, int x1, int y2, int x3){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.xyline((int)(x*s+L), (int)(y*s+T), 
			 (int)(x1*s+L), (int)(y2*s+T), (int)(x3*s+L));
		 fl_gc = o;
	 };

	 void yxline(int x, int y, int y1){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.yxline((int)(x*s+L), (int)(y*s+T), (int)(y1*s+T));
		 fl_gc = o;
	 };
	 void yxline(int x, int y, int y1, int x2){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.yxline((int)(x*s+L), (int)(y*s+T), 
			 (int)(y1*s+T), (int)(x2*s+L));
		 fl_gc = o;
	 };
	 void yxline(int x, int y, int y1, int x2, int y3){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.yxline((int)(x*s+L), (int)(y*s+T), (int)(y1*s+T), 
			 (int)(x2*s+L), (int)(y3*s+T));
		 fl_gc = o;
	 };

	 void arc(int x, int y, int w, int h, double a1, double a2){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.arc((int)(x*s+L), (int)(y*s+T), (int)(w*s), (int)(h*s), a1, a2);
		 fl_gc = o;
	 };

	 void pie(int x, int y, int w, int h, double a1, double a2){
	 	 o = fl_gc;
		 fl_gc = gc;
		 fltk.pie((int)(x*s+L), (int)(y*s+T), (int)(w*s), (int)(h*s), a1, a2);
		 fl_gc = o;
	 };

	 void begin_points(){
         o = fl_gc;
		 fl_gc = gc;
		 fltk.begin_points();
		 fl_gc = o;
	 };
	 void begin_line(){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.begin_line();
		 fl_gc = o;
	 };
	 void begin_loop(){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.begin_loop();
		 fl_gc = o;
	 };
	 void begin_polygon(){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.begin_polygon();
		 fl_gc = o;
	 };
	 void vertex(double x, double y){
		 fltk.vertex(x, y);
	 };
	 void curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3){
	 	 o = fl_gc;
		 fl_gc = gc;
		 fltk.curve(x, y, x1, y1, x2, y2, x3, y3);
		 fl_gc = o;
	 };
	 void arc(double x, double y, double r, double start, double a){
	 	 o = fl_gc;
		 fl_gc = gc;
		 fltk.arc(x, y, r, start, a);
		 fl_gc = o;
	 };
	 void circle(double x, double y, double r){
	 	 o = fl_gc;
		 fl_gc = gc;
		 fltk.circle(x, y, r);
		 fl_gc = o;
	 };
	 void end_points(){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.end_points();
		 fl_gc = o;
	 };
	 void end_line(){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.end_line();
		 fl_gc = o;
	 };
	 void end_loop(){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.end_loop();
		 fl_gc = o;
	 };
	 void end_polygon(){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.end_polygon();
		 fl_gc = o;
	 };
	 void begin_complex_polygon(){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.begin_complex_polygon();
		 fl_gc = o;
	 };
	 void gap(){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.gap();
		 fl_gc = o;
	 };
	 void end_complex_polygon(){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.end_complex_polygon();
		 fl_gc = o;
	 };
	 void transformed_vertex(double x, double y){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.transformed_vertex(x, y);
		 fl_gc = o;
	 };
	 void translate(double x, double y) {
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.translate(x, y);
		 fl_gc = o;
	 }

	 void font(int face, int size) {
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.font(face, (int)(size * s));
		 fl_gc = o;		
	 };
	 int font(){return fl_font_;};
	 int size(){return (int)(fl_size_ / s);};
	 int height(){ return (int)(fltk.height() / s);};
	 double width(const char* c){return fltk.width(c) / s;};
	 double width(const char* c, int n){return fltk.width(c, n) / s;};
	 double width(unsigned int c) {return (fltk.width(c) / s);};
	 int descent(){return (int)(fltk.descent() / s);};
	 void draw(const char* c, int x, int y){draw(c, strlen(c), x, y);};
	 void draw(const char* c, int n, int x, int y){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.draw(c, n, (int)(x*s+L), (int)(y*s+T));
		 fl_gc = o;
	 };
	 void rtl_draw(const char* c, int n, int x, int y){
	     o = fl_gc;
		 fl_gc = gc;
		 fltk.rtl_draw(c, n, (int)(x*s+L), (int)(y*s+T));
		 fl_gc = o;
	 };
	
	 void draw_image(const uchar*, int,int,int,int, int delta=3, int ldelta=0);
	 void draw_image_mono(const uchar*, int,int,int,int, int delta=1, int ld=0);
	 void draw_image(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=3);
	 void draw_image_mono(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=1);

	 void rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b){
		 o = fl_gc;
		 fl_gc = gc;
		 fltk.rectf((int)(x*s+L),(int)(y*s+T),(int)(w*s),(int)(h*s), r, g, b);
		 fl_gc = o;
	 };
     
	 Fl_Gdi() {type = FL_GDI_DEVICE; s = 4.0; gc = 0; page_=0;};
	 ~Fl_Gdi(); 

	 void clip(double x,double y,double w,double h){clip((int)x, (int)y, (int)w, (int)h);};
	 int not_clipped(double x,double y,double w,double h){return not_clipped((int)x, (int)y, (int)w, (int)h);};
	 int clip_box(double _x,double _y,double _w,double _h,double &x,double &y,double &w,double &h){
		 int xx = (int)x, yy = (int)y, ww = (int)w, hh = (int)h;
		 int ret = fltk.clip_box((int)(_x), (int)(_y), (int)(_w), (int)(_h), xx, yy, ww, hh);
		 x = xx; y = yy; w = ww; h = hh; 
		 return ret;
		
	 };
	 void transformed_draw(const char *c,double x,double y){draw(c, (int)x, (int)y);};
	 void transformed_draw(const char *c,int n,double x,double y){draw(c, n, (int)x, (int)y);};

};

#endif
#endif
