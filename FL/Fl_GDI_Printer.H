//

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


#ifndef Fl_GDI_Printer_H
#define Fl_GDI_Printer_H
#include "Fl_Printer.H"
#include <windows.h>
#include <FL/x.H>
#include <commdlg.h>


extern FL_EXPORT Fl_Color fl_color_;
extern FL_EXPORT int fl_size_;
extern FL_EXPORT int fl_font_;
extern FL_EXPORT HDC fl_gc;


struct FL_EXPORT Fl_Brush {
    HBRUSH brush;
    unsigned short usage;
    Fl_XMap* backref;
};

#define FL_N_BRUSH 16 
class FL_EXPORT Fl_GDI_Printer: public Fl_Printer{

  HPEN tmppen;
  HBRUSH tmpbrush;
  HPEN savepen;

  Fl_Brush brushes[FL_N_BRUSH];
  Fl_XMap fl_xmap[256];
  Fl_XMap xmap;
  
  HDC gc_;
  DEVMODE * mode_;

  long ix, iy, ox, oy;
  long WOx, VEx, WEx, VOx;
  long WOy, VEy, WEy, VOy;
  int style_;
  int width_;
  char * dashes_;

  long max_res_;
  uchar * mask;
  void sty(int style, int width, char *dashes, int vex = 1);
  
  
  void set_page(int page);
  void set_subpixel();
  void set_normal();
 


public:
    Fl_Device * set_current();
//    void delete_mode(int i){delete_mode_=i;};

protected:
    void color(Fl_Color c);
    Fl_Color color();
    void color(uchar r, uchar g, uchar b);


    //void draw(Fl_Widget * w);
    void draw(const char* str, int n, int x, int y);
public:
    void page(double pw, double ph, int media = 0);
    void page(int format);
    void place(double x, double y,  double tx, double ty,  double scale = 1);
protected:
    int height();
    double width(const char* s, int n);
    double width(unsigned c);
    int descent();
    void font(int face, int size);
    int font();
    int size();
    void line_style(int style, int width, char * dashes);
    void push_no_clip();
    void pop_clip();
    void rectf(int x, int y, int w, int h);
    void rect(int x, int y, int w, int h);
    void point(int x, int y);
    void arc(int x, int y, int w, int h, double a1, double a2);
    void pie(int x, int y, int w, int h, double a1, double a2);
    void xyline(int x, int y, int x1);
    void yxline(int x, int y, int y1);
    void xyline(int x, int y, int x1, int y2);
    void xyline(int x, int y, int x1, int y2, int x3);
    void yxline(int x, int y, int y1, int x2);
    void yxline(int x, int y, int y1, int x2, int y3);
    void line(int x, int y, int x1, int y1);
    void line(int x, int y, int x1, int y1, int x2, int y2);
    void loop(int x1, int y1, int x2 ,int y2, int x3, int y3);
    void loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4);
    void polygon(int x1, int y1, int x2 ,int y2, int x3, int y3);
    void polygon(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4);


    //  these need to be redefined to support more precise (subpixel)  path ("double") drawings

    void begin_points();
    void begin_line();
    void begin_loop();
    void begin_polygon();
    void vertex(double x, double y);
    void curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3);
    void arc(double x, double y, double r, double start, double a);
    void circle(double x, double y, double r);
    void end_points();
    void end_line();
    void end_loop();
    void end_polygon();
    void begin_complex_polygon();
    void gap();
    void end_complex_polygon();
    void transformed_vertex(double x, double y);


    

     void draw_image(const uchar*, int,int,int,int, int delta=3, int ldelta=0);
     void draw_image_mono(const uchar*, int,int,int,int, int delta=1, int ld=0);
     void draw_image(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=3);
     void draw_image_mono(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=1);
     void rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b);
     void draw(Fl_Pixmap * pxm,int XP, int YP, int WP, int HP, int cx, int cy);
     void draw(Fl_RGB_Image * rgb,int XP, int YP, int WP, int HP, int cx, int cy);
     void draw(Fl_Bitmap * bmp,int XP, int YP, int WP, int HP, int cx, int cy);



    // Some clipping primitives must be re-implemented as core gdi clipping functions work in device-coordinates, not "logical" (in MS terminology)
    void push_clip(int x, int y, int w, int h);
    int not_clipped(int x, int y, int w, int h);
    int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H);



public:
  Fl_GDI_Printer(HDC gc, DEVMODE * mode);
  ~Fl_GDI_Printer();
};


#endif
