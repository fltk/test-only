//
// "$Id$"
//
// Definition of classes fltk3::GraphicsDriver, fltk3::SurfaceDevice, fltk3::DisplayDevice
// for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2011 by Bill Spitzak and others.
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
/** \file Device.h 
 \brief declaration of classes fltk3::GraphicsDriver, fltk3::SurfaceDevice, 
 fltk3::DisplayDevice, fltk3::DevicePlugin.
 */

#ifndef Fltk3_Device_H
#define Fltk3_Device_H

#include <fltk3/x.h>
#include <fltk3/Object.h>
#include <fltk3/Plugin.h>
#include <fltk3/Image.h>
#include <fltk3/Bitmap.h>
#include <fltk3/Pixmap.h>
#include <fltk3/RGBImage.h>
#include <stdlib.h>

// typedef what the x,y fields in a point are:
#ifdef WIN32
typedef int COORD_T;
#  define XPOINT XPoint
#elif defined(__APPLE__)
typedef float COORD_T;
typedef struct { float x; float y; } QPoint;
#  define XPOINT QPoint
extern float fl_quartz_line_width_;
#else
typedef short COORD_T;
#  define XPOINT XPoint
#endif


extern void gl_start();

class Fl_Font_Descriptor;
class Fl_Surface_Device;

namespace fltk3 {
  
  class GraphicsDriver;
  /** Pointer to the fltk3::GraphicsDriver object that currently handles all graphics operation */
  FLTK3_EXPORT extern GraphicsDriver *graphics_driver;
  
  /**
   signature of image generation callback function.
   \param[in]  data  user data passed to function
   \param[in]  x,y,w position and width of scan line in image
   \param[out] buf   buffer for generated image data. You must copy \p w
   pixels from scanline \p y, starting at pixel \p x
   to this buffer.
   */
  typedef void (*DrawImageCb)(void* data,int x,int y,int w,uchar* buf);
    
  const int REGION_STACK_SIZE = 10;
  const int MATRIX_STACK_SIZE = 32;
  
  /**
   \brief A virtual class subclassed for each graphics driver FLTK uses.
   *
   The virtual methods of this class are those that a graphics driver should implement to
   support all of FLTK drawing functions.
   <br> The public API for drawing operations is functionally presented in \ref drawing and as function lists
   in the \ref fl_drawings and \ref fl_attributes modules. 
   */
  class FLTK3_EXPORT GraphicsDriver : public Object {
    friend void ::gl_start();
  public:
    /** A 2D coordinate transformation matrix
     */
    struct matrix {double a, b, c, d, x, y;};
    struct origin {int x, y;};
  private:
    static const matrix m0;
    fltk3::Font font_; // current font
    fltk3::Fontsize size_; // current font size
    fltk3::Color color_; // current color
    int sptr;
    enum { matrix_stack_size = MATRIX_STACK_SIZE };
    matrix stack[MATRIX_STACK_SIZE];
    matrix m;
    int optr;
    enum { origin_stack_size = MATRIX_STACK_SIZE };
    origin ostack[MATRIX_STACK_SIZE];
    origin o;
    int rstackptr;
    enum { region_stack_max = REGION_STACK_SIZE - 1 };
    fltk3::Region rstack[REGION_STACK_SIZE];
    Fl_Font_Descriptor *font_descriptor_;
  protected:
#ifndef FLTK3_DOXYGEN
    int n, p_size, gap_;
    XPOINT *p;
    enum {LINE, LOOP, POLYGON, POINT_};
    int what;
    int fl_clip_state_number;
    void prepare_circle(double x, double y, double r, int& llx, int& lly, int& w, int& h, double& xt, double& yt);
    void fixloop();
    void region_stack_push(fltk3::Region r);
    void transformed_vertex0(COORD_T x, COORD_T y);
#endif
    
  public:
    
    matrix *fl_matrix; /**< Points to the current coordinate transformation matrix */
    /** Gives the number of vertices in the current path.
     Useful when writing Fl_Graphics_Driver subclasses.
     */
    int vertex_no() { return n; }
    /** Array of vertices in the current path.
     Useful when writing Fl_Graphics_Driver subclasses. XPOINT is a system-dependent type.
     */
    XPOINT *vertices() { return p; }
    
    /** \brief The constructor. */
    GraphicsDriver();
    /** \brief see fltk3::rect(int x, int y, int w, int h). */
    virtual void rect(int x, int y, int w, int h) {}
    /** \brief see fltk3::rectf(int x, int y, int w, int h). */
    virtual void rectf(int x, int y, int w, int h) {}
    /** \brief see fltk3::line_style(int style, int width, char* dashes). */
    virtual void line_style(int style, int width=0, char* dashes=0) {}
    /** \brief see fltk3::xyline(int x, int y, int x1). */
    virtual void xyline(int x, int y, int x1) {}
    /** \brief see fltk3::xyline(int x, int y, int x1, int y2). */
    virtual void xyline(int x, int y, int x1, int y2) {}
    /** \brief see fltk3::xyline(int x, int y, int x1, int y2, int x3). */
    virtual void xyline(int x, int y, int x1, int y2, int x3) {}
    /** \brief see fltk3::yxline(int x, int y, int y1). */
    virtual void yxline(int x, int y, int y1) {}
    /** \brief see fltk3::yxline(int x, int y, int y1, int x2). */
    virtual void yxline(int x, int y, int y1, int x2) {}
    /** \brief see fltk3::yxline(int x, int y, int y1, int x2, int y3). */
    virtual void yxline(int x, int y, int y1, int x2, int y3) {}
    /** \brief see fltk3::line(int x, int y, int x1, int y1). */
    virtual void line(int x, int y, int x1, int y1) {}
    /** \brief see fltk3::line(int x, int y, int x1, int y1, int x2, int y2). */
    virtual void line(int x, int y, int x1, int y1, int x2, int y2) {}
    /** \brief see fltk3::draw(const char *str, int n, int x, int y). */
    virtual void draw(const char *str, int n, int x, int y) {}
#ifdef __APPLE__
    virtual void draw(const char *str, int n, float x, float y) { draw(str, n, (int)(x+0.5), (int)(y+0.5));}
#endif
    /** \brief see fltk3::draw(int angle, const char *str, int n, int x, int y). */
    virtual void draw(int angle, const char *str, int n, int x, int y) {}
    /** \brief see fltk3::rtl_draw(const char *str, int n, int x, int y). */
    virtual void rtl_draw(const char *str, int n, int x, int y) {}
    /** \brief see fltk3::color(fltk3::Color c). */
    virtual void color(fltk3::Color c) {color_ = c;}
    /** \brief see fltk3::color(uchar r, uchar g, uchar b). */
    virtual void color(uchar r, uchar g, uchar b) {}
    /** \brief see fltk3::point(int x, int y). */
    virtual void point(int x, int y) {}
    /** \brief see fltk3::loop(int x0, int y0, int x1, int y1, int x2, int y2). */
    virtual void loop(int x0, int y0, int x1, int y1, int x2, int y2) {}
    /** \brief see fltk3::loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3). */
    virtual void loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {}
    /** \brief see fltk3::polygon(int x0, int y0, int x1, int y1, int x2, int y2). */
    virtual void polygon(int x0, int y0, int x1, int y1, int x2, int y2) {}
    /** \brief see fltk3::polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3). */
    virtual void polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {}
    /** \brief see fltk3::begin_points(). */
    virtual void begin_points();
    /** \brief see fltk3::begin_line(). */
    virtual void begin_line();
    /** \brief see fltk3::begin_loop(). */
    virtual void begin_loop();
    /** \brief see fltk3::begin_polygon(). */
    virtual void begin_polygon();
    /** \brief see fltk3::vertex(double x, double y). */
    virtual void vertex(double x, double y);
    /** \brief see fltk3::curve(double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3). */
    virtual void curve(double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3);
    /** \brief see fltk3::circle(double x, double y, double r). */
    virtual void circle(double x, double y, double r) {}
    /** \brief see fltk3::arc(double x, double y, double r, double start, double end). */
    virtual void arc(double x, double y, double r, double start, double end);
    /** \brief see fltk3::arc(int x, int y, int w, int h, double a1, double a2). */
    virtual void arc(int x, int y, int w, int h, double a1, double a2) {}
    /** \brief see fltk3::pie(int x, int y, int w, int h, double a1, double a2). */
    virtual void pie(int x, int y, int w, int h, double a1, double a2) {}
    /** \brief see fltk3::end_points(). */
    virtual void end_points();
    /** \brief see fltk3::end_line(). */
    virtual void end_line() {}
    /** \brief see fltk3::end_loop(). */
    virtual void end_loop();
    /** \brief see fltk3::end_polygon(). */
    virtual void end_polygon() {}
    /** \brief see fltk3::begin_complex_polygon(). */
    virtual void begin_complex_polygon();
    /** \brief see fltk3::gap(). */
    virtual void gap();
    /** \brief see fltk3::end_complex_polygon(). */
    virtual void end_complex_polygon() {}
    /** \brief see fltk3::transformed_vertex(double xf, double yf). */
    virtual void transformed_vertex(double xf, double yf);
    /** \brief see fltk3::push_clip(int x, int y, int w, int h). */
    virtual void push_clip(int x, int y, int w, int h) {}
    /** \brief see fltk3::clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H). */
    virtual int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H) { return 0;}
    /** \brief see fltk3::not_clipped(int x, int y, int w, int h). */
    virtual int not_clipped(int x, int y, int w, int h) { return 1;}
    /** \brief see fltk3::push_no_clip(). */
    virtual void push_no_clip();
    /** \brief see fltk3::pop_clip(). */
    virtual void pop_clip();
    
    void push_origin();
    void pop_origin();
    void translate_origin(int dx, int dy);
    void origin(int x, int y);
    int origin_x() { return o.x; }
    int origin_y() { return o.y; }
    
    /** \brief see fltk3::push_matrix(). */
    void push_matrix();
    /** \brief see fltk3::pop_matrix(). */
    void pop_matrix();
    /** \brief see fltk3::mult_matrix(double a, double b, double c, double d, double x, double y). */
    void mult_matrix(double a, double b, double c, double d, double x, double y);
    /** \brief see fltk3::scale(double x, double y). */
    inline void scale(double x, double y) { mult_matrix(x,0,0,y,0,0); }
    /** \brief see fltk3::scale(double x). */
    inline void scale(double x) { mult_matrix(x,0,0,x,0,0); }
    /** \brief see fltk3::translate(double x, double y). */
    inline void translate(double x,double y) { mult_matrix(1,0,0,1,x,y); }
    /** \brief see fltk3::rotate(double d). */
    void rotate(double d);
    /** \brief see fltk3::transform_x(double x, double y). */
    double transform_x(double x, double y);
    /** \brief see fltk3::transform_y(double x, double y). */
    double transform_y(double x, double y);
    /** \brief see fltk3::transform_dx(double x, double y). */
    double transform_dx(double x, double y);
    /** \brief see fltk3::transform_dy(double x, double y). */
    double transform_dy(double x, double y);
    /** \brief see fltk3::clip_region(). */
    inline fltk3::Region clip_region() { return rstack[rstackptr]; }
    /** \brief see fltk3::clip_region(fltk3::Region r). */
    void clip_region(fltk3::Region r);
    /** \brief see fltk3::restore_clip(). */
    virtual void restore_clip() {}
    
    // Images
    /** \brief see fltk3::draw_image(const uchar* buf, int X,int Y,int W,int H, int D, int L). */
    virtual   void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0) {}
    /** \brief see fltk3::draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D, int L). */
    virtual   void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0) {}
    /** \brief see fltk3::draw_image(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D). */
    virtual   void draw_image(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D=3) {}
    /** \brief see fltk3::draw_image_mono(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D). */
    virtual   void draw_image_mono(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D=1) {}
    // Image classes
    /** \brief Draws an fltk3::RGBImage object to the device. 
     *
     Specifies a bounding box for the image, with the origin (upper left-hand corner) of 
     the image offset by the cx and cy arguments.
     */
    virtual   void draw(fltk3::RGBImage * rgb,int XP, int YP, int WP, int HP, int cx, int cy) {}
    /** \brief Draws an fltk3::Pixmap object to the device. 
     *
     Specifies a bounding box for the image, with the origin (upper left-hand corner) of 
     the image offset by the cx and cy arguments.
     */
    virtual   void draw(fltk3::Pixmap * pxm,int XP, int YP, int WP, int HP, int cx, int cy) {}
    /** \brief Draws an fltk3::Bitmap object to the device. 
     *
     Specifies a bounding box for the image, with the origin (upper left-hand corner) of 
     the image offset by the cx and cy arguments.
     */
    virtual void draw(fltk3::Bitmap *bm, int XP, int YP, int WP, int HP, int cx, int cy) {}
    
    /** \brief see fltk3::font(fltk3::Font face, fltk3::Fontsize size). */
    virtual void font(fltk3::Font face, fltk3::Fontsize size) {font_ = face; size_ = size;}
    /** \brief see fltk3::font(void). */
    fltk3::Font font() {return font_; }
    /** \brief see fltk3::size(). */
    fltk3::Fontsize size() {return size_; }
    /** \brief see fltk3::width(const char *str, int n). */
    virtual double width(const char *str, int n) {return 0;}
    /** \brief see fltk3::width(unsigned int n). */
    virtual inline double width(unsigned int c) { char ch = (char)c; return width(&ch, 1); }
    /** \brief see fltk3::text_extents(const char*, int n, int& dx, int& dy, int& w, int& h). */
    virtual void text_extents(const char*, int n, int& dx, int& dy, int& w, int& h);
    /** \brief see fltk3::height(). */
    virtual int height() {return size();}
    /** \brief see fltk3::descent(). */
    virtual int descent() {return 0;}
    /** \brief see fltk3::color(void). */
    fltk3::Color color() {return color_;}
    /** Returns a pointer to the current Fl_Font_Descriptor for the graphics driver */
    inline Fl_Font_Descriptor *font_descriptor() { return font_descriptor_;}
    /** Sets the current Fl_Font_Descriptor for the graphics driver */
    inline void font_descriptor(Fl_Font_Descriptor *d) { font_descriptor_ = d;}
    /** see fltk3::copy_offscreen() */
    virtual void copy_offscreen(int x, int y, int w, int h, fltk3::Offscreen pixmap, int srcx, int srcy);
    /** see fltk3::can_do_alpha_blending() */
    virtual char can_do_alpha_blending() {return 0;}
    /** \brief The destructor */
    virtual ~GraphicsDriver() { if (p) free(p); }
  };
  
#if defined(__APPLE__) || defined(FLTK3_DOXYGEN)
  /**
   \brief The Mac OS X-specific graphics class.
   *
   This class is implemented only on the Mac OS X platform.
   */
  class FLTK3_EXPORT QuartzGraphicsDriver : public fltk3::GraphicsDriver {
  public:
    void color(fltk3::Color c);
    void color(uchar r, uchar g, uchar b);
    void draw(const char* str, int n, int x, int y);
#ifdef __APPLE__
    void draw(const char *str, int n, float x, float y);
#endif
    void draw(int angle, const char *str, int n, int x, int y);
    void rtl_draw(const char* str, int n, int x, int y);
    void font(fltk3::Font face, fltk3::Fontsize size);
    void draw(fltk3::Pixmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
    void draw(fltk3::Bitmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
    void draw(fltk3::RGBImage *img, int XP, int YP, int WP, int HP, int cx, int cy);
    void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0);
    void draw_image(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D=3);
    void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0);
    void draw_image_mono(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D=1);
    double width(const char *str, int n);
    double width(unsigned int c);
    void text_extents(const char*, int n, int& dx, int& dy, int& w, int& h);
    int height();
    int descent();
    void end_points();
    void end_line();
    void end_polygon();
    void end_complex_polygon();
    void circle(double x, double y, double r);
    void arc(int x,int y,int w,int h,double a1,double a2);
    void pie(int x,int y,int w,int h,double a1,double a2);
    void rect(int x, int y, int w, int h);
    void rectf(int x, int y, int w, int h);
    void xyline(int x, int y, int x1);
    void xyline(int x, int y, int x1, int y2);
    void xyline(int x, int y, int x1, int y2, int x3);
    void yxline(int x, int y, int y1);
    void yxline(int x, int y, int y1, int x2);
    void yxline(int x, int y, int y1, int x2, int y3);
    void line(int x, int y, int x1, int y1);
    void line(int x, int y, int x1, int y1, int x2, int y2);
    void loop(int x, int y, int x1, int y1, int x2, int y2);
    void loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
    void polygon(int x0, int y0, int x1, int y1, int x2, int y2);
    void polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3);
    void point(int x, int y);
    void restore_clip();
    void push_clip(int x, int y, int w, int h);
    int not_clipped(int x, int y, int w, int h);
    int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H);
    void line_style(int style, int width=0, char* dashes=0);
    void transformed_vertex(double xf, double yf);
    void copy_offscreen(int x, int y, int w, int h, fltk3::Offscreen pixmap, int srcx, int srcy);
    char can_do_alpha_blending();
#if ! defined(FLTK3_DOXYGEN)
    static fltk3::Offscreen create_offscreen_with_alpha(int w, int h);
#endif
  };
#endif
  
#if defined(WIN32) || defined(FLTK3_DOXYGEN)
  /**
   \brief The MSWindows-specific graphics class.
   *
   This class is implemented only on the MSWindows platform.
   */
  class FLTK3_EXPORT GDIGraphicsDriver : public fltk3::GraphicsDriver {
    friend class GDIPrinterGraphicsDriver;
    int numcount;
    int counts[20];
    void draw_bitmap(fltk3::Bitmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy, int to_display);
  public:
    void color(fltk3::Color c);
    void color(uchar r, uchar g, uchar b);
    void draw(const char* str, int n, int x, int y);
    void draw(int angle, const char *str, int n, int x, int y);
    void rtl_draw(const char* str, int n, int x, int y);
    void font(fltk3::Font face, fltk3::Fontsize size);
    void draw(fltk3::Pixmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
    void draw(fltk3::Bitmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy) {
      draw_bitmap(pxm, XP, YP, WP, HP, cx, cy, 1);
      }
    void draw(fltk3::RGBImage *img, int XP, int YP, int WP, int HP, int cx, int cy);
    void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0);
    void draw_image(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D=3);
    void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0);
    void draw_image_mono(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D=1);
    double width(const char *str, int n);
    double width(unsigned int c);
    void text_extents(const char*, int n, int& dx, int& dy, int& w, int& h);
    int height();
    int descent();
    void end_points();
    void end_line();
    void end_polygon();
    void begin_complex_polygon();
    void gap();
    void end_complex_polygon();
    void circle(double x, double y, double r);
    void arc(int x,int y,int w,int h,double a1,double a2);
    void pie(int x,int y,int w,int h,double a1,double a2);
    void rect(int x, int y, int w, int h);
    void rectf(int x, int y, int w, int h);
    void xyline(int x, int y, int x1);
    void xyline(int x, int y, int x1, int y2);
    void xyline(int x, int y, int x1, int y2, int x3);
    void yxline(int x, int y, int y1);
    void yxline(int x, int y, int y1, int x2);
    void yxline(int x, int y, int y1, int x2, int y3);
    void line(int x, int y, int x1, int y1);
    void line(int x, int y, int x1, int y1, int x2, int y2);
    void loop(int x, int y, int x1, int y1, int x2, int y2);
    void loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
    void polygon(int x0, int y0, int x1, int y1, int x2, int y2);
    void polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3);
    void point(int x, int y);
    void restore_clip();
    void push_clip(int x, int y, int w, int h);
    int not_clipped(int x, int y, int w, int h);
    int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H);
    void line_style(int style, int width=0, char* dashes=0);
    void copy_offscreen(int x, int y, int w, int h, fltk3::Offscreen pixmap, int srcx, int srcy);
    char can_do_alpha_blending();
#if ! defined(FLTK3_DOXYGEN)
    void copy_offscreen_with_alpha(int x,int y,int w,int h,HBITMAP pixmap,int srcx,int srcy);
#endif
  };
  
  class FLTK3_EXPORT GDIPrinterGraphicsDriver : public fltk3::GDIGraphicsDriver {
  private:
    void draw(fltk3::Pixmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
    void draw(fltk3::Bitmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy) {
      draw_bitmap(pxm, XP, YP, WP, HP, cx, cy, 0);
    }
  };
#endif
  
#if !(defined(__APPLE__) || defined(WIN32))
  /**
   \brief The Xlib-specific graphics class.
   *
   This class is implemented only on the Xlib platform.
   */
  class XlibGraphicsDriver : public fltk3::GraphicsDriver {
  public:
    void color(fltk3::Color c);
    void color(uchar r, uchar g, uchar b);
    void draw(const char* str, int n, int x, int y);
    void draw(int angle, const char *str, int n, int x, int y);
    void rtl_draw(const char* str, int n, int x, int y);
    void font(fltk3::Font face, fltk3::Fontsize size);
    void draw(fltk3::Pixmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
    void draw(fltk3::Bitmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
    void draw(fltk3::RGBImage *img, int XP, int YP, int WP, int HP, int cx, int cy);
    void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0);
    void draw_image(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D=3);
    void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0);
    void draw_image_mono(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D=1);
    double width(const char *str, int n);
    double width(unsigned int c);
    void text_extents(const char*, int n, int& dx, int& dy, int& w, int& h);
    int height();
    int descent();
    void end_points();
    void end_line();
    void end_polygon();
    void end_complex_polygon();
    void circle(double x, double y, double r);
    void arc(int x,int y,int w,int h,double a1,double a2);
    void pie(int x,int y,int w,int h,double a1,double a2);
    void rect(int x, int y, int w, int h);
    void rectf(int x, int y, int w, int h);
    void xyline(int x, int y, int x1);
    void xyline(int x, int y, int x1, int y2);
    void xyline(int x, int y, int x1, int y2, int x3);
    void yxline(int x, int y, int y1);
    void yxline(int x, int y, int y1, int x2);
    void yxline(int x, int y, int y1, int x2, int y3);
    void line(int x, int y, int x1, int y1);
    void line(int x, int y, int x1, int y1, int x2, int y2);
    void loop(int x, int y, int x1, int y1, int x2, int y2);
    void loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
    void polygon(int x0, int y0, int x1, int y1, int x2, int y2);
    void polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3);
    void point(int x, int y);
    void restore_clip();
    void push_clip(int x, int y, int w, int h);
    int not_clipped(int x, int y, int w, int h);
    int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H);
    void line_style(int style, int width=0, char* dashes=0);
    void copy_offscreen(int x, int y, int w, int h, fltk3::Offscreen pixmap, int srcx, int srcy);
    char can_do_alpha_blending();
  };
#endif
  
  /**
   \brief A surface that's susceptible to receive graphical output.
   */
  class FLTK3_EXPORT SurfaceDevice : public Object {
    friend class ::Fl_Surface_Device;
    /** \brief The graphics driver in use by this surface. */
    fltk3::GraphicsDriver *_driver;
    static SurfaceDevice *_surface; // the surface that currently receives graphics output
  protected:
    /** \brief Constructor that sets the graphics driver to use for the created surface. */
    SurfaceDevice(fltk3::GraphicsDriver *graphics_driver) {_driver = graphics_driver; };
  public:
    virtual void set_current(void);
    /** \brief Sets the graphics driver of this drawing surface. */
    inline void driver(fltk3::GraphicsDriver *graphics_driver) {_driver = graphics_driver;};
    /** \brief Returns the graphics driver of this drawing surface. */
    inline fltk3::GraphicsDriver *driver() {return _driver; };
    /** \brief the surface that currently receives graphics output */
    static inline fltk3::SurfaceDevice *surface() {return _surface; };
    /** \brief The destructor. */
    virtual ~SurfaceDevice();
  };
  
  /**
   \brief A display to which the computer can draw.
   */
  class FLTK3_EXPORT DisplayDevice : public fltk3::SurfaceDevice {
    static DisplayDevice *_display; // the platform display device
  public:
    /** \brief A constructor that sets the graphics driver used by the display */
    DisplayDevice(fltk3::GraphicsDriver *graphics_driver);
    /** Returns the platform display device. */
    static inline DisplayDevice *display_device() {return _display;};
  };
  
  /**
   This plugin socket allows the integration of new device drivers for special
   window or screen types. It is currently used to provide an automated printing
   service for OpenGL windows, if linked with fltk_gl.
   */
  class FLTK3_EXPORT DevicePlugin : public fltk3::Plugin {
  public:
    /** \brief The constructor */
    DevicePlugin(const char *name)
    : fltk3::Plugin(klass(), name) { }
    /** \brief Returns the class name */
    virtual const char *klass() { return "fltk:device"; }
    /** \brief Returns the plugin name */
    virtual const char *name() = 0;
    /** \brief Prints a widget 
     \param w the widget
     \param x,y offsets where to print relatively to coordinates origin
     \param height height of the current drawing area
     */
    virtual int print(fltk3::Widget* w, int x, int y, int height) = 0;
  };
  
}

#endif // Fltk3_Device_H

//
// End of "$Id$".
//
