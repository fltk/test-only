#ifndef Fl_Ps_H
#define Fl_Ps_H
#include "Fl_Device.H"
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Fltk.H>
#include <string.h>
#include <stdio.h>
#include <FL/x.H>




class Fl_Ps:public Fl_Device{

	int **fonts;
	int nb_fonts;
	int lang_level_;
	int font_;
	int size_;
	Fl_Color color_;
	FILE *output;
	int nPages;
	int gap_;
	int g_width_;
	int g_height_;
	int g_orientation_;
	int g_format;
	int width_;
	int height_;
	int shape_;
	int linewidth_;// need for clipping, lang level 1-2
	int linestyle_;//
	unsigned char cr_,cg_,cb_;
	char  linedash_[256];//should be enought
	void concat();  // transform ror scalable dradings...
	void reconcat(); //invert
	void recover(); //recovers the state afrer grestore 
	void reset();
	double c_x_, c_y_, c_w_, c_h_; //no_clip clipping (to margins)
	int colored_, line_styled_, fonted_;
		//these are flags if graphical state (color, painting, font)should be recovered
		// for language level 1-2: it has no clipsace/cliprestore, so gsave / grestore is used

	class Clip{
	public:
		double x, y, w, h;
		Clip *prev;
	};

	Clip * clip_;
    Fl_Offscreen bit;
	int pw;
	int ph;

public:

	FL_EXPORT void page(int lm, int tm, int rm, int bm, int orientation,  int w, int h);
	FL_EXPORT void page(int lm=0, int tm=0, int rm=0, int bm=0, int orientation=PS_UNCHANGED, int format=PS_UNCHANGED);
	FL_EXPORT void fit(int x, int y, int w, int h, int align=FL_ALIGN_CENTER);

	FL_EXPORT void color(Fl_Color c);
	FL_EXPORT void color(uchar r, uchar g, uchar b);
	FL_EXPORT Fl_Color color(){return color_;};
	
	FL_EXPORT void clip(double x, double y, double w, double h);
	void clip(int x, int y, int w, int h){clip((double)x,(double)y,(double)w,(double)h);};
	FL_EXPORT void push_no_clip();
	FL_EXPORT void pop_clip();
	FL_EXPORT int not_clipped(double x, double y, double w, double h);
	int not_clipped(int x, int y, int w, int h){return not_clipped((double)x,(double)y,(double)w,(double)h);};
	FL_EXPORT int clip_box(double x, double y, double w, double h, double &X, double &Y, double &W, double &H);
	FL_EXPORT int clip_box(int _x, int _y, int _w, int _h, int& x, int& y, int& w, int& h);

	FL_EXPORT void point(int x, int y){};

	FL_EXPORT void line_style(int style, int width, char* dashes);

	FL_EXPORT void rect(int x, int y, int w, int h);
	FL_EXPORT void rectf(int x, int y, int w, int h);

	FL_EXPORT void line(int x1, int y1, int x2, int y2);
	FL_EXPORT void line(int x1, int y1, int x2, int y2, int x3, int y3);

	FL_EXPORT void loop(int x1, int y1, int x2 ,int y2, int x3, int y3);
	FL_EXPORT void loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4);

	FL_EXPORT void polygon(int x1, int y1, int x2 ,int y2, int x3, int y3);
	FL_EXPORT void polygon(int x1 ,int y1, int x2, int y2, int x3, int y3, int x4, int y4);

	FL_EXPORT void xyline(int x, int y, int x1);
	FL_EXPORT void xyline(int x, int y, int x1, int y2);
	FL_EXPORT void xyline(int x, int y, int x1, int y2, int x3);

	FL_EXPORT void yxline(int x, int y, int y1);
	FL_EXPORT void yxline(int x, int y, int y1, int x2);
	FL_EXPORT void yxline(int x, int y, int y1, int x2, int y3);

	FL_EXPORT void arc(int x, int y, int w, int h, double a1, double a2);
	FL_EXPORT void pie(int x, int y, int w, int h, double a1, double a2);

	FL_EXPORT void begin_points();
	FL_EXPORT void begin_line();
	FL_EXPORT void begin_loop();
	FL_EXPORT void begin_polygon();
	FL_EXPORT void vertex(double x, double y);
	FL_EXPORT void curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3);
	FL_EXPORT void arc(double x, double y, double r, double start, double a);
	FL_EXPORT void circle(double x, double y, double r);
	FL_EXPORT void end_points();
	FL_EXPORT void end_line();
	FL_EXPORT void end_loop();
	FL_EXPORT void end_polygon();
	void begin_complex_polygon(){begin_polygon();};
	void gap(){gap_=1;};
	void end_complex_polygon(){end_polygon();};
	FL_EXPORT void transformed_vertex(double x, double y);
    FL_EXPORT void translate(double x, double y){fltk.translate(x, y);};

	FL_EXPORT void font(int face, int size);
	int font(){return font_;};
	int size(){return size_;};
	FL_EXPORT int height();
	FL_EXPORT double width(const char* s);
	FL_EXPORT double width(const char* s, int n);
	FL_EXPORT double width(unsigned int c);
	FL_EXPORT int descent();

	FL_EXPORT void transformed_draw(const char* s, int n, double x, double y); //precise text placing
	FL_EXPORT void transformed_draw(const char* s, double x, double y){transformed_draw(s,strlen(s),x,y);};
	void rtl_draw(const char* s, int n, int x, int y);
	void draw(const char* s, int n, int x, int y){transformed_draw(s,n,x,y);};
	void draw(const char* s, int x, int y){draw(s,strlen(s),x,y);};

	FL_EXPORT void draw_scalled_image(const uchar *data, double x, double y, double w, double h, int iw, int ih, int D=3, int LD=0);
	FL_EXPORT void draw_image(const uchar* d, int x,int y,int w,int h, int delta=3, int ldelta=0){draw_scalled_image(d,x,y,w,h,w,h,delta,ldelta);};
	FL_EXPORT void draw_scalled_image_mono(const uchar *data, double x, double y, double w, double h, int iw, int ih, int D=3, int LD=0);
	FL_EXPORT void draw_image_mono(const uchar* d, int x,int y,int w,int h, int delta=1, int ld=0){draw_scalled_image_mono(d,x,y,w,h,w,h,delta,ld);};
	
	FL_EXPORT void draw_scalled_image(Fl_Draw_Image_Cb call, void *data, double x, double y, double w, double h, int iw, int ih, int D);
	FL_EXPORT void draw_scalled_image_mono(Fl_Draw_Image_Cb call, void *data, double x, double y, double w, double h, int iw, int ih, int D);
	FL_EXPORT void draw_image(Fl_Draw_Image_Cb call, void* data, int x,int y,int w,int h, int delta=3){draw_scalled_image(call, data, x, y, w, h, w, h, delta);};
	FL_EXPORT void draw_image_mono(Fl_Draw_Image_Cb call, void* data, int x,int y,int w,int h, int delta=1){draw_scalled_image_mono(call, data, x, y, w, h, w, h, delta);};
	FL_EXPORT void rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b);

	FL_EXPORT int get_width(unsigned int ucs);

	FL_EXPORT Fl_Ps(FILE *o, int lang_level, int orientation, int w, int=0 ); // ps (aslo multi-page) constructor
	FL_EXPORT Fl_Ps(FILE *o, int lang_level, int orientation, int x, int y, int w, int h, int dx=72, int dy=72); //eps constructor
	FL_EXPORT ~Fl_Ps();
};

#endif
