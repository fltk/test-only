//
// "$Id: Fl_Device.H,v 1.1.2.4 2004/03/29 22:41:11 rokan Exp $"
//
// Fl_Device dispatching class for the Fast Light Tool Kit (FLTK).
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


#ifndef Fl_Device_H
#define Fl_Device_H

#include <FL/Enumerations.H>
#include <FL/Fl_Widget.H>

class Fl_Image;
class Fl_RGB_Image;
class Fl_Pixmap;
class Fl_Bitmap;


typedef void (*Fl_Draw_Image_Cb)(void* ,int,int,int,uchar*);

class Fl_Image_Cache;

/**
* A base class for all output devices.
*
* Device classes should not be a direct subclasses of Fl_Device but rather of Fl_Display or Fl_Printer
*/

class FL_EXPORT Fl_Device{
protected:
 int type_;
 
 friend class Fl_Image_Cache;

 Fl_Image_Cache * image_caches;
 Fl_Image_Cache * check_image_cache(Fl_Image * im);
 

public:
  /** An RTTI emulation of device classes.
  *
  * @return returns type < 256 if the device is a screen device - subcalls of Fl_Display. */
  int type(){return type_;}; // <256 for screen devices
  
protected: 
  // usesrs should not use drawing methods directly, but set the device as current
  // and use fl_* functions (as declared in fl_draw.H) instead.
  // We rare friendly to them ...

  friend   void fl_color(Fl_Color color);
  friend   void fl_color(uchar r, uchar g, uchar b) ;
  friend   Fl_Color fl_color() ;
  friend   void fl_push_clip(int x, int y, int w, int h) ;
  friend   void fl_push_no_clip() ;
  friend   void fl_pop_clip() ;
  friend   int fl_not_clipped(int x, int y, int w, int h) ;
  friend   int fl_clip_box(int _x, int _y, int _w, int _h, int& x, int& y, int& w, int& h) ;
  friend   void fl_point(int x, int y) ;
  friend   void fl_line_style(int style, int width, char* dashes) ;
  friend   void fl_rect(int x, int y, int w, int h) ;
  friend   void fl_rectf(int x, int y, int w, int h) ;
  friend   void fl_line(int x1, int y1, int x2, int y2) ;
  friend   void fl_line(int x1, int y1, int x2, int y2, int x3, int y3) ;
  friend   void fl_loop(int x1, int y1, int x2 ,int y2, int x3, int y3) ;
  friend   void fl_loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4) ;
  friend   void fl_polygon(int x1, int y1, int x2 ,int y2, int x3, int y3) ;
  friend   void fl_polygon(int x1 ,int y1, int x2, int y2, int x3, int y3, int x4, int y4) ;
  friend   void fl_xyline(int x, int y, int x1) ;
  friend   void fl_xyline(int x, int y, int x1, int y2) ;
  friend   void fl_xyline(int x, int y, int x1, int y2, int x3) ;
  friend   void fl_yxline(int x, int y, int y1) ;
  friend   void fl_yxline(int x, int y, int y1, int x2) ;
  friend   void fl_yxline(int x, int y, int y1, int x2, int y3) ;
  friend   void fl_arc(int x, int y, int w, int h, double a1, double a2) ;
  friend   void fl_pie(int x, int y, int w, int h, double a1, double a2) ;
  friend   void fl_begin_points() ;
  friend   void fl_begin_line() ;
  friend   void fl_begin_loop() ;
  friend   void fl_begin_polygon() ;
  friend   void fl_vertex(double x, double y) ;
  friend   void fl_curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3) ;
  friend   void fl_arc(double x, double y, double r, double start, double a) ;
  friend   void fl_circle(double x, double y, double r) ;
  friend   void fl_end_points() ;
  friend   void fl_end_line() ;
  friend   void fl_end_loop() ;
  friend   void fl_end_polygon() ;
  friend   void fl_begin_complex_polygon() ;
  friend   void fl_gap() ;
  friend   void fl_end_complex_polygon() ;
  friend   void fl_transformed_vertex(double x, double y) ;
  friend   void fl_font(int face, int size) ;
  friend   int fl_font() ;
  friend   int fl_size() ;
  friend   int fl_height() ;
  friend   double fl_width(const char* s, int n) ;
  friend   double fl_width(unsigned c) ;
  friend   int fl_descent() ;
  friend   void fl_draw(const char* s, int n, int x, int y) ;
  friend   void fl_draw_image(const uchar*, int,int,int,int, int delta, int ldelta);
  friend   void fl_draw_image_mono(const uchar*, int,int,int,int, int delta, int ld);
  friend   void fl_draw_image(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta);
  friend   void fl_draw_image_mono(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta);
  friend   void fl_rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b);
  friend class Fl_RGB_Image;
  friend class Fl_Bitmap;
  friend class Fl_Pixmap;
  
///////////// Definitions of drawing primitives /////////////////////////
  
  virtual   void color(Fl_Color color) = 0;
  virtual   void color(uchar r, uchar g, uchar b) = 0;
  virtual   Fl_Color color() = 0;

  // Following "int" primitives disregard transformation matrix
  
  virtual   void draw(const char* s, int n, int x, int y) = 0;
  
  virtual   void push_clip(int x, int y, int w, int h) = 0;
  virtual   void push_no_clip() = 0;
  virtual   void pop_clip() = 0;
  virtual   int not_clipped(int x, int y, int w, int h) = 0;
  virtual   int clip_box(int _x, int _y, int _w, int _h, int& x, int& y, int& w, int& h) = 0;
  
  virtual   void line_style(int style, int width=0, char* dashes=0) = 0;

  virtual   void point(int x, int y) = 0;
  virtual   void rect(int x, int y, int w, int h) = 0;
  virtual   void rectf(int x, int y, int w, int h) = 0;
  virtual   void rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b)=0;

  virtual   void line(int x1, int y1, int x2, int y2) = 0;
  virtual   void line(int x1, int y1, int x2, int y2, int x3, int y3) = 0;

  virtual   void loop(int x1, int y1, int x2 ,int y2, int x3, int y3) = 0;
  virtual   void loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4) = 0;

  virtual   void polygon(int x1, int y1, int x2 ,int y2, int x3, int y3) = 0;
  virtual   void polygon(int x1 ,int y1, int x2, int y2, int x3, int y3, int x4, int y4) = 0;

  virtual   void xyline(int x, int y, int x1) = 0;
  virtual   void xyline(int x, int y, int x1, int y2) = 0;
  virtual   void xyline(int x, int y, int x1, int y2, int x3) = 0;

  virtual   void yxline(int x, int y, int y1) = 0;
  virtual   void yxline(int x, int y, int y1, int x2) = 0;
  virtual   void yxline(int x, int y, int y1, int x2, int y3) = 0;

  virtual   void arc(int x, int y, int w, int h, double a1, double a2) = 0;
  virtual   void pie(int x, int y, int w, int h, double a1, double a2) = 0;

  // "double" (path) primisives should perform drawings with sub-pixel precision on devices which allow that
  // (such as printers). They also obey transformation matrix -with the exception of transformed_vertex
  
  virtual   void begin_points() = 0;
  virtual   void begin_line() = 0;
  virtual   void begin_loop() = 0;
  virtual   void begin_polygon() = 0;
  virtual   void begin_complex_polygon() = 0;
  virtual   void gap() = 0;
  
  virtual   void vertex(double x, double y) = 0;
  virtual   void transformed_vertex(double x, double y) = 0;
  virtual   void curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3) = 0;
  virtual   void arc(double x, double y, double r, double start, double a) = 0;
  virtual   void circle(double x, double y, double r) = 0;
  
  virtual   void end_points() = 0;
  virtual   void end_line() = 0;
  virtual   void end_loop() = 0;
  virtual   void end_polygon() = 0;
  virtual   void end_complex_polygon() = 0;

  // Fonts
  virtual   void font(int face, int size) = 0;
  virtual   int font() = 0;
  virtual   int size() = 0;
  virtual   int height() = 0;
  virtual   double width(const char* s, int n) = 0;
  virtual   double width(unsigned c) = 0;
  virtual   int descent() = 0;

  // Images
  virtual   void draw_image(const uchar*, int,int,int,int, int delta=3, int ldelta=0)=0;
  virtual   void draw_image_mono(const uchar*, int,int,int,int, int delta=1, int ld=0)=0;
  virtual   void draw_image(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=3)=0;
  virtual   void draw_image_mono(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=1)=0;

  
  // Image classes
  virtual   void draw(Fl_Pixmap * pxm,int XP, int YP, int WP, int HP, int cx, int cy)=0;
  virtual   void draw(Fl_RGB_Image * rgb,int XP, int YP, int WP, int HP, int cx, int cy)=0;
  virtual   void draw(Fl_Bitmap * bmp,int XP, int YP, int WP, int HP, int cx, int cy)=0;

public:

  /** Sets the device as current, all following drawings will be ditected to that device.
  * Returns previously active device. */
  virtual Fl_Device * set_current();
  /** Returns currently active device */
  static Fl_Device * current();
  
  virtual   ~Fl_Device();
  Fl_Device():image_caches(0){};

};


extern FL_EXPORT Fl_Device * fl_device;

#endif


//
// End of "$Id: Fl_Device.H,v 1.1.2.4 2004/03/29 22:41:11 rokan Exp $".
//
