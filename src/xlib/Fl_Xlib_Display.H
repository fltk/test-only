//
// "$Id: Fl_Xlib_Display.H,v 1.1.2.1 2004/03/28 10:30:32 rokan Exp $"
//
// Xlib display class for the Fast Light Tool Kit (FLTK).
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


#ifndef Fl_Xlib_Display_H
#define Fl_Xlib_Display_H
#include <FL/Fl_Display.H>



//#include <FL/Fl_Image.H>
//#include <FL/Fl_Pixmap.H>
//#include <FL/Fl_Bitmap.H>

class Fl_Image;
class Fl_RGB_Image;
class Fl_Pixmap;
class Fl_Bitmap;



//typedef void (*Fl_Draw_Image_Cb)(void* ,int,int,int,uchar*);





class FL_EXPORT Fl_Xlib_Display: public Fl_Display{
protected:
     friend class Fl_PS_Printer; //RK: temporary hack for font sizes
     friend class Fl_GDI_Printer;

     void color(Fl_Color color);
     void color(uchar r, uchar g, uchar b);
     Fl_Color color(){return fl_color_;};

     void push_clip(int x, int y, int w, int h);


     void push_no_clip();
     void pop_clip();
     int not_clipped(int x, int y, int w, int h);
     int clip_box(int _x, int _y, int _w, int _h, int& x, int& y, int& w, int& h);


     void point(int x, int y);
     void line_style(int style, int width=0, char* dashes=0);

     void rect(int x, int y, int w, int h);
     void rectf(int x, int y, int w, int h);

     void line(int x1, int y1, int x2, int y2);
     void line(int x1, int y1, int x2, int y2, int x3, int y3);

     void loop(int x1, int y1, int x2 ,int y2, int x3, int y3);
     void loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4);

     void polygon(int x1, int y1, int x2 ,int y2, int x3, int y3);
     void polygon(int x1 ,int y1, int x2, int y2, int x3, int y3, int x4, int y4);

     void xyline(int x, int y, int x1);
     void xyline(int x, int y, int x1, int y2);
     void xyline(int x, int y, int x1, int y2, int x3);

     void yxline(int x, int y, int y1);
     void yxline(int x, int y, int y1, int x2);
     void yxline(int x, int y, int y1, int x2, int y3);

     void arc(int x, int y, int w, int h, double a1, double a2);
     void pie(int x, int y, int w, int h, double a1, double a2);

     void begin_points();
     void begin_line();
     void begin_loop();
     void begin_polygon();
     void vertex(double x, double y);
     //void curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3);
     //void arc(double x, double y, double r, double start, double a);
     void circle(double x, double y, double r);
     void end_points();
     void end_line();
     void end_loop();
     void end_polygon();
     void begin_complex_polygon();
     void gap();
     void end_complex_polygon();
     void transformed_vertex(double x, double y);

     void font(int face, int size);
     int font(){return fl_font_;};
     int size(){return fl_size_;};
     int height();
     //double width(const char* s);
     double width(const char* s, int n);
     double width(unsigned c);
     int descent();
     //void draw(const char* s, int x, int y);
     void draw(const char* s, int n, int x, int y);



     void draw_image(const uchar*, int,int,int,int, int delta=3, int ldelta=0);
     void draw_image_mono(const uchar*, int,int,int,int, int delta=1, int ld=0);
     void draw_image(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=3);
     void draw_image_mono(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=1);
     void rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b);
     void draw(Fl_Pixmap * pxm,int XP, int YP, int WP, int HP, int cx, int cy);
     void draw(Fl_RGB_Image * rgb,int XP, int YP, int WP, int HP, int cx, int cy);
     void draw(Fl_Bitmap * bmp,int XP, int YP, int WP, int HP, int cx, int cy);
public:
     Fl_Xlib_Display(){type_ = FL_XLIB_DISPLAY;};
};


#endif

//
// End of "$Id: Fl_Xlib_Display.H,v 1.1.2.1 2004/03/28 10:30:32 rokan Exp $".
//
