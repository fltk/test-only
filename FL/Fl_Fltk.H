#ifndef Fl_Fltk_H
#define Fl_Fltk_H
#include "Fl_Device.H"



class Fl_Fltk: public Fl_Device{
public:
	 FL_EXPORT void color(Fl_Color color);
	 FL_EXPORT void color(uchar r, uchar g, uchar b);
	 Fl_Color color(){return fl_color_;};
		 
	 FL_EXPORT void clip(int x, int y, int w, int h);
	 void clip(double x, double y, double w, double h){clip((int)(x+.5),(int)(y+.5),(int)(w+.5),(int)(h+.5));};

	 FL_EXPORT void push_no_clip();
	 FL_EXPORT void pop_clip();
	 
	 FL_EXPORT int not_clipped(int x, int y, int w, int h);
	 int not_clipped(double x, double y, double w, double h){return not_clipped((int)(x+.5), (int)(y+.5), (int)(w+.5), (int)(h+.5));};
	 FL_EXPORT int clip_box(int _x, int _y, int _w, int _h, int& x, int& y, int& w, int& h);
	 int clip_box(double _x, double _y, double _w, double _h, double& x, double& y, double& w, double& h){
		int x_,y_,w_,h_,r;
		r=clip_box((int)(_x+.5), (int)(_y+.5), (int)(_w+.5), (int)(_h+.5), x_, y_, w_, h_);
		x=x_;y=y_;w=w_;h=h_;
		return r;
	 };
	 FL_EXPORT void point(int x, int y);

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
	 FL_EXPORT void begin_complex_polygon();
	 FL_EXPORT void gap();
	 FL_EXPORT void end_complex_polygon();
	 FL_EXPORT void transformed_vertex(double x, double y);
	 FL_EXPORT void translate(double x, double y);

	 FL_EXPORT void font(int face, int size);
	 int font(){return fl_font_;};
	 int size(){return fl_size_;};
	 FL_EXPORT int height();
	 FL_EXPORT double width(const char* s);
	 FL_EXPORT double width(const char* s, int n);
	 FL_EXPORT double width(unsigned int c);
	 FL_EXPORT int descent();
	 FL_EXPORT void draw(const char* s, int x, int y);
	 FL_EXPORT void draw(const char* s, int n, int x, int y);
	 FL_EXPORT void rtl_draw(const char* s, int n, int x, int y);
	 void transformed_draw(const char* s, int n, double x, double y){draw(s,n,(int)(x+.5),(int)(y+.5));};
	 void transformed_draw(const char* s, double x, double y){draw(s,(int)(x+.5),(int)(y+.5));};

	 FL_EXPORT void draw_image(const uchar*, int,int,int,int, int delta=3, int ldelta=0);
	 FL_EXPORT void draw_image_mono(const uchar*, int,int,int,int, int delta=1, int ld=0);
	 FL_EXPORT void draw_image(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=3);
	 FL_EXPORT void draw_image_mono(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=1);
	 FL_EXPORT void rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b);
	
	 FL_EXPORT Fl_Fltk() {type = FL_FLTK_DEVICE;};

};

extern FL_EXPORT Fl_Fltk fltk;

#endif
