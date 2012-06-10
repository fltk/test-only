//
// "$Id$"
//
// Definition of classes Fl_Device, Fl_Graphics_Driver, Fl_Surface_Device, 
// Fl_Display_Device for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2012 by Bill Spitzak and others.
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

#ifndef Fl_Device_H
#define Fl_Device_H

#include <fltk3/Device.h>
#include <fltk3/Wrapper.h>
#include <fltk3/run.h>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Pixmap.H>

typedef fltk3::Offscreen Fl_Offscreen;
typedef fltk3::DrawImageCb Fl_Draw_Image_Cb;
typedef fltk3::Region Fl_Region;

namespace fltk3 { 
    
class GraphicsDriverWrapper : public fltk3::Wrapper {
protected:
  enum {LINE, LOOP, POLYGON, POINT_};
  int what;
public:
  virtual ~GraphicsDriverWrapper() {}
  
  virtual int height() { 
    return ((fltk3::GraphicsDriver*)_p)->height(); 
    }

  virtual int descent() { 
    return ((fltk3::GraphicsDriver*)_p)->descent(); 
  }
 
  virtual int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H) { 
    return ((fltk3::GraphicsDriver*)_p)->clip_box(x,y,w,h,X,Y,W,H); 
  }
  
  virtual int not_clipped(int x, int y, int w, int h) { 
    return ((fltk3::GraphicsDriver*)_p)->not_clipped(x,y,w,h); 
  }
  
  virtual double width(const char* str, int l) { 
    return ((fltk3::GraphicsDriver*)_p)->width(str, l); 
  }

  virtual double width(unsigned c) { 
    return ((fltk3::GraphicsDriver*)_p)->width(c); 
  }
  
#define FLTK3_DRIVER_WRAPPER(proto, call) \
virtual void proto { \
      ((fltk3::GraphicsDriver*)_p)->call; \
  }
FLTK3_DRIVER_WRAPPER(font(Fl_Font f, Fl_Fontsize s), font(_1to3_font(f), _1to3_fontsize(s)))
FLTK3_DRIVER_WRAPPER(draw(const char* str, int l, int x, int y), draw(str, l, x, y))
#ifdef __APPLE__
FLTK3_DRIVER_WRAPPER(draw(const char* str, int l, float fx, float fy), draw(str, l, fx, fy))
#endif
FLTK3_DRIVER_WRAPPER(draw(int angle, const char* str, int l, int x, int y), draw(angle, str, l, x, y))
FLTK3_DRIVER_WRAPPER(line(int x, int y, int x1, int y1), line(x, y, x1, y1))
FLTK3_DRIVER_WRAPPER(rect(int x, int y, int w, int h), rect(x, y, w, h))
FLTK3_DRIVER_WRAPPER(rectf(int x, int y, int w, int h), rectf(x, y, w, h))
FLTK3_DRIVER_WRAPPER(color(uchar r, uchar g, uchar b), color(r, g, b))
FLTK3_DRIVER_WRAPPER(color(Fl_Color c), color(_1to3_color(c)))
FLTK3_DRIVER_WRAPPER(line_style(int style, int width, char *dashes), line_style(style, width, dashes))
FLTK3_DRIVER_WRAPPER(push_clip(int x, int y, int w, int h) , push_clip(x, y, w, h))
FLTK3_DRIVER_WRAPPER(pop_clip(), pop_clip())
FLTK3_DRIVER_WRAPPER(draw(Fl_Pixmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy) ,
    draw((fltk3::Pixmap*)pxm->_p, XP,YP,WP,HP,cx,cy) )
FLTK3_DRIVER_WRAPPER(draw_image(Fl_Draw_Image_Cb cb, void *data, int X, int Y, int W, int H, int D) ,
    draw_image( (fltk3::DrawImageCb)cb, data, X,Y,W,H,D) )
FLTK3_DRIVER_WRAPPER(line(int x, int y, int x1, int y1, int x2, int y2), line(x, y, x1, y1, x2, y2))
FLTK3_DRIVER_WRAPPER(xyline(int x, int y, int x1), xyline(x, y, x1))
FLTK3_DRIVER_WRAPPER(xyline(int x, int y, int x1, int y2), xyline(x, y, x1, y2))
FLTK3_DRIVER_WRAPPER(xyline(int x, int y, int x1, int y2, int x3), xyline(x, y, x1, y2, x3))
FLTK3_DRIVER_WRAPPER(yxline(int x, int y, int y1), yxline(x, y, y1))
FLTK3_DRIVER_WRAPPER(yxline(int x, int y, int y1, int x2), yxline(x, y, y1, x2))
FLTK3_DRIVER_WRAPPER(yxline(int x, int y, int y1, int x2, int y3), yxline(x, y, y1, x2, y3))
FLTK3_DRIVER_WRAPPER(rtl_draw(const char* str, int l, int x, int y), rtl_draw(str, l, x, y))
FLTK3_DRIVER_WRAPPER(draw(Fl_RGB_Image* rgb,int XP, int YP, int WP, int HP, int cx, int cy) ,
		     draw( (fltk3::RGBImage*)rgb->_p, XP, YP, WP, HP, cx, cy) )
FLTK3_DRIVER_WRAPPER( draw(Fl_Bitmap* bm, int XP, int YP, int WP, int HP, int cx, int cy) ,
		     draw( (fltk3::Bitmap*)bm->_p, XP, YP, WP, HP, cx, cy) )
FLTK3_DRIVER_WRAPPER(draw_image(const uchar* cb, int X, int Y, int W, int H, int D, int L), draw_image(cb, X, Y, W, H, D, L))
FLTK3_DRIVER_WRAPPER(draw_image_mono(const uchar* cb, int X, int Y, int W, int H, int D, int L), draw_image_mono(cb, X, Y, W, H, D, L))
FLTK3_DRIVER_WRAPPER(draw_image_mono(Fl_Draw_Image_Cb cb, void *data, int X, int Y, int W, int H, int D), draw_image_mono((fltk3::DrawImageCb)cb,data, X, Y, W, H, D))
FLTK3_DRIVER_WRAPPER(text_extents(const char* str, int n, int& dx, int& dy, int& w, int& h), text_extents(str, n, dx, dy, w, h))

FLTK3_DRIVER_WRAPPER(arc(double x, double y, double r, double start, double end), arc(x,y,r,start,end))
FLTK3_DRIVER_WRAPPER(arc(int x, int y, int w, int h, double a1, double a2), arc(x,y,w,h,a1,a2))
FLTK3_DRIVER_WRAPPER(begin_complex_polygon(), begin_complex_polygon())
  virtual void begin_line() { 
    ((fltk3::GraphicsDriver*)_p)->begin_line(); 
    what = LINE;
  }
  virtual void begin_loop() { 
    ((fltk3::GraphicsDriver*)_p)->begin_loop(); 
    what = LOOP;
  }
  virtual void begin_points() { 
    ((fltk3::GraphicsDriver*)_p)->begin_points(); 
    what = POINT_;
  }
  virtual void begin_polygon() { 
    ((fltk3::GraphicsDriver*)_p)->begin_polygon(); 
    what = POLYGON;
  }
  
FLTK3_DRIVER_WRAPPER(circle(double x, double y, double r), circle(x, y, r))
FLTK3_DRIVER_WRAPPER(copy_offscreen (int x, int y, int w, int h, Fl_Offscreen pixmap, int srcx, int srcy),
		     copy_offscreen(x,y,w,h,(fltk3::Offscreen)pixmap,srcx,srcy))
FLTK3_DRIVER_WRAPPER(curve (double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3),
		       curve(X0,Y0,X1,Y1,X2,Y2,X3,Y3))
FLTK3_DRIVER_WRAPPER(end_complex_polygon(), end_complex_polygon())
FLTK3_DRIVER_WRAPPER(end_line(), end_line())
FLTK3_DRIVER_WRAPPER(end_loop(), end_loop())
FLTK3_DRIVER_WRAPPER(end_points(), end_points())
FLTK3_DRIVER_WRAPPER(end_polygon(), end_polygon())
FLTK3_DRIVER_WRAPPER(gap(), gap())
FLTK3_DRIVER_WRAPPER(loop(int x0, int y0, int x1, int y1, int x2, int y2), loop(x0,y0,x1,y1,x2,y2))
FLTK3_DRIVER_WRAPPER(loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3), loop(x0,y0,x1,y1,x2,y2,x3,y3))
FLTK3_DRIVER_WRAPPER(pie(int x, int y, int w, int h, double a1, double a2), pie(x,y,w,h,a1,a2))
FLTK3_DRIVER_WRAPPER(point(int x, int y), point(x,y))
FLTK3_DRIVER_WRAPPER(polygon(int x0, int y0, int x1, int y1, int x2, int y2), polygon(x0,y0,x1,y1,x2,y2))
FLTK3_DRIVER_WRAPPER(polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3), polygon(x0,y0,x1,y1,x2,y2,x3,y3))
FLTK3_DRIVER_WRAPPER(push_no_clip(), push_no_clip())
FLTK3_DRIVER_WRAPPER(transformed_vertex (double xf, double yf), transformed_vertex ( xf, yf))
FLTK3_DRIVER_WRAPPER(vertex (double xf, double yf), vertex ( xf, yf))

};


class GraphicsDriver_I : public GraphicsDriver { 
public: 
  virtual ~GraphicsDriver_I() { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallDtor) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallDtor; 
      delete ((Wrapper*)pWrapper); 
    }
  }
  GraphicsDriver_I() : GraphicsDriver() { } 
  void font(Font f, Fontsize s) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      ((GraphicsDriverWrapper*)pWrapper)->font(_3to1_font(f), _3to1_fontsize(s)); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver; 
    }
    else GraphicsDriver::font(f, s); 
  } 
  int height() { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      int h = ((GraphicsDriverWrapper*)pWrapper)->height(); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver;
      return h;
    }
    return GraphicsDriver::height(); 
  } 
  int descent() { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      int d = ((GraphicsDriverWrapper*)pWrapper)->descent(); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver;
      return d;
    }
    return GraphicsDriver::descent(); 
  } 
  int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      int d = ((GraphicsDriverWrapper*)pWrapper)->clip_box(x,y,w,h,X,Y,W,H); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver;
      return d;
    }
    return GraphicsDriver::clip_box(x,y,w,h,X,Y,W,H); 
  } 
  int not_clipped(int x, int y, int w, int h) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      int d = ((GraphicsDriverWrapper*)pWrapper)->not_clipped(x,y,w,h); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver;
      return d;
    }
    return GraphicsDriver::not_clipped(x,y,w,h); 
  } 
  double width(const char* str, int l) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      double w = ((GraphicsDriverWrapper*)pWrapper)->width(str, l); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver;
      return w;
    }
    return GraphicsDriver::width(str, l); 
  } 
  double width(unsigned c) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      double w = ((GraphicsDriverWrapper*)pWrapper)->width(c); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver;
      return w;
    }
    return GraphicsDriver::width(c); 
  } 
  void draw_image(fltk3::DrawImageCb cb, void *data, int X, int Y, int W, int H, int D) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDrawImage) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDrawImage; 
      ((GraphicsDriverWrapper*)pWrapper)->draw_image( (Fl_Draw_Image_Cb)cb, data, X+origin_x(),Y+origin_y(),W,H,D); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDrawImage; 
    } 
    else GraphicsDriver::draw_image( (Fl_Draw_Image_Cb)cb, data, X,Y,W,H,D); 
  }   
  
#define FLTK3_DRIVER_VF(proto, call) \
void proto { \
  if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { \
    pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; \
    ((GraphicsDriverWrapper*)pWrapper)->call; \
    pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver; \
  } \
  else GraphicsDriver::call; \
} 
#define FLTK3_DRIVER_VF1(proto, call1) \
void proto { \
  if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { \
    pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; \
    ((GraphicsDriverWrapper*)pWrapper)->call1; \
    pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver; \
  } \
} 
  FLTK3_DRIVER_VF1(draw(const char* str, int l, int x, int y), draw(str, l, x+origin_x(), y+origin_y()))
#ifdef __APPLE__
  void draw(const char* str, int l, float fx, float fy) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      ((GraphicsDriverWrapper*)pWrapper)->draw(str, l, fx+origin_x(), fy+origin_y()); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver; 
    } 
    else GraphicsDriver::draw(str, l, fx, fy); 
  } 
#endif
  FLTK3_DRIVER_VF1(draw(int angle, const char* str, int l, int x, int y), draw(angle, str, l, x+origin_x(), y+origin_y()))
  FLTK3_DRIVER_VF1(line(int x, int y, int x1, int y1), line(x+origin_x(), y+origin_y(), x1+origin_x(), y1+origin_y()))
  FLTK3_DRIVER_VF1(rect(int x, int y, int w, int h), rect(x+origin_x(), y+origin_y(), w, h))
  FLTK3_DRIVER_VF1(rectf(int x, int y, int w, int h), rectf(x+origin_x(), y+origin_y(), w, h))
  FLTK3_DRIVER_VF(color(uchar r, uchar g, uchar b), color(r, g, b))
  FLTK3_DRIVER_VF(color(fltk3::Color c), color(fltk3::_3to1_color(c)))
  FLTK3_DRIVER_VF(line_style(int style, int width, char *dashes), line_style(style, width, dashes))
  void push_clip(int x, int y, int w, int h) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsPushClip) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsPushClip; 
      ((GraphicsDriverWrapper*)pWrapper)->push_clip(x+origin_x(), y+origin_y(), w, h); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsPushClip; 
    } 
  } 
  void pop_clip() { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsPopClip) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsPopClip; 
      ((GraphicsDriverWrapper*)pWrapper)->pop_clip(); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsPopClip; 
    } 
  } 
  FLTK3_DRIVER_VF1(line(int x, int y, int x1, int y1, int x2, int y2), line(x+origin_x(), y+origin_y(), x1+origin_x(), y1+origin_y(), x2+origin_x(), y2+origin_y()))
  FLTK3_DRIVER_VF1(xyline(int x, int y, int x1), xyline(x+origin_x(), y+origin_y(), x1+origin_x()))
  FLTK3_DRIVER_VF1(xyline(int x, int y, int x1, int y2), xyline(x+origin_x(), y+origin_y(), x1+origin_x(), y2+origin_y()))
  FLTK3_DRIVER_VF1(xyline(int x, int y, int x1, int y2, int x3), xyline(x+origin_x(), y+origin_y(), x1+origin_x(), y2+origin_y(), x3+origin_x()))
  FLTK3_DRIVER_VF1(yxline(int x, int y, int y1), yxline(x+origin_x(), y+origin_y(), y1+origin_y()))
  FLTK3_DRIVER_VF1(yxline(int x, int y, int y1, int x2), yxline(x+origin_x(), y+origin_y(), y1+origin_y(), x2+origin_x()))
  FLTK3_DRIVER_VF1(yxline(int x, int y, int y1, int x2, int y3), yxline(x+origin_x(), y+origin_y(), y1+origin_y(), x2+origin_x(), y3+origin_y()))
  FLTK3_DRIVER_VF1(rtl_draw(const char* str, int l, int x, int y), rtl_draw(str, l, x+origin_x(), y+origin_y()))
  
  void draw(fltk3::RGBImage* rgb,int XP, int YP, int WP, int HP, int cx, int cy) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      Fl_RGB_Image *rgb1 = (Fl_RGB_Image*)rgb->wrapper();
      if (!rgb1) rgb1 = new Fl_RGB_Image(rgb);
      ((GraphicsDriverWrapper*)pWrapper)->draw(rgb1, XP+origin_x(), YP+origin_y(), WP, HP, cx, cy); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver; 
    } 
  }
  void draw(fltk3::Bitmap* bm,int XP, int YP, int WP, int HP, int cx, int cy) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      Fl_Bitmap *bm1 = (Fl_Bitmap*)bm->wrapper();
      if (!bm1) bm1 = (Fl_Bitmap*)new Fl_Image(bm);
      ((GraphicsDriverWrapper*)pWrapper)->draw(bm1, XP+origin_x(), YP+origin_y(), WP, HP, cx, cy); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver; 
    } 
  } 
  void draw(fltk3::Pixmap* px,int XP, int YP, int WP, int HP, int cx, int cy) { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsDriver) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsDriver; 
      Fl_Pixmap *px1 = (Fl_Pixmap*)px->wrapper();
      if (!px1) px1 = (Fl_Pixmap*)new Fl_Image(px);
      ((GraphicsDriverWrapper*)pWrapper)->draw(px1, XP, YP, WP, HP, cx, cy); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsDriver; 
    } 
  }
  
  FLTK3_DRIVER_VF1(draw_image(const uchar* b, int X, int Y, int W, int H, int D, int L), draw_image(b, X+origin_x(), Y+origin_y(), W, H, D, L))
  FLTK3_DRIVER_VF1(draw_image_mono(const uchar* cb, int X, int Y, int W, int H, int D, int L), draw_image_mono(cb, X+origin_x(), Y+origin_y(), W, H, D, L))
  FLTK3_DRIVER_VF1(draw_image_mono(fltk3::DrawImageCb cb, void *data, int X, int Y, int W, int H, int D), draw_image_mono((Fl_Draw_Image_Cb)cb,data, X+origin_x(), Y+origin_y(), W, H, D))
  FLTK3_DRIVER_VF(text_extents(const char* str, int n, int& dx, int& dy, int& w, int& h), text_extents(str, n, dx, dy, w, h))
  FLTK3_DRIVER_VF(arc(double x, double y, double r, double start, double end), arc(x,y,r,start,end))
  FLTK3_DRIVER_VF1(arc(int x, int y, int w, int h, double a1, double a2), arc(x+origin_x(),y+origin_y(),w,h,a1,a2))
  FLTK3_DRIVER_VF(begin_complex_polygon(), begin_complex_polygon())
  FLTK3_DRIVER_VF(begin_line(), begin_line())
  FLTK3_DRIVER_VF(begin_loop(), begin_loop())
  FLTK3_DRIVER_VF(begin_points(), begin_points())
  FLTK3_DRIVER_VF(begin_polygon(), begin_polygon())
  FLTK3_DRIVER_VF1(circle(double x, double y, double r), circle(x+origin_x(), y+origin_y(), r))
  FLTK3_DRIVER_VF(copy_offscreen(int x, int y, int w, int h, fltk3::Offscreen pixmap, int srcx, int srcy),
		       copy_offscreen(x,y,w,h,(Fl_Offscreen)pixmap,srcx,srcy))
  FLTK3_DRIVER_VF(curve(double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3),
		       curve(X0,Y0,X1,Y1,X2,Y2,X3,Y3))
  FLTK3_DRIVER_VF(end_complex_polygon(), end_complex_polygon())

  void end_line() { 
    if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallGraphicsEndLine) ) { 
      pWrapper->pVCalls |= Wrapper::pVCallGraphicsEndLine; 
      ((GraphicsDriverWrapper*)pWrapper)->end_line(); 
      pWrapper->pVCalls &= ~Wrapper::pVCallGraphicsEndLine; 
    } 
    else GraphicsDriver::end_line(); 
  } 
  
  FLTK3_DRIVER_VF(end_loop(), end_loop())
  FLTK3_DRIVER_VF(end_points(), end_points())
  FLTK3_DRIVER_VF(end_polygon(), end_polygon())
  FLTK3_DRIVER_VF(gap(), gap())
  FLTK3_DRIVER_VF1(loop(int x0, int y0, int x1, int y1, int x2, int y2), loop(x0+origin_x(),y0+origin_y(),x1+origin_x(),y1+origin_y(),x2+origin_x(),y2+origin_y()))
  FLTK3_DRIVER_VF1(loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3), loop(x0+origin_x(),y0+origin_y(),x1+origin_x(),y1+origin_y(),x2+origin_x(),y2+origin_y(),x3+origin_x(),y3+origin_y()))
  FLTK3_DRIVER_VF1(pie(int x, int y, int w, int h, double a1, double a2), pie(x+origin_x(),y+origin_y(),w,h,a1,a2))
  FLTK3_DRIVER_VF1(point(int x, int y), point(x+origin_x(),y+origin_y()))
  FLTK3_DRIVER_VF1(polygon(int x0, int y0, int x1, int y1, int x2, int y2), polygon(x0+origin_x(),y0+origin_y(),x1+origin_x(),y1+origin_y(),x2+origin_x(),y2+origin_y()))
  FLTK3_DRIVER_VF1(polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3), polygon(x0+origin_x(),y0+origin_y(),x1+origin_x(),y1+origin_y(),x2+origin_x(),y2+origin_y(),x3+origin_x(),y3+origin_y()))
  FLTK3_DRIVER_VF(push_no_clip(), push_no_clip())
  FLTK3_DRIVER_VF(transformed_vertex(double xf, double yf), transformed_vertex(xf, yf))
  FLTK3_DRIVER_VF(vertex(double xf, double yf), vertex(xf, yf))
}; 

}
   
class FL_EXPORT Fl_Graphics_Driver : public fltk3::GraphicsDriverWrapper {
public:
  struct matrix {double a, b, c, d, x, y;};
protected:
  matrix *fl_matrix;
public:
  Fl_Graphics_Driver() {
    _p = new fltk3::GraphicsDriver_I();
    _p->wrapper(this);
    fl_matrix = (matrix*)((fltk3::GraphicsDriver*)_p)->fl_matrix;
  }
  int vertex_no() { 
    return ((fltk3::GraphicsDriver_I*)_p)->fltk3::GraphicsDriver::vertex_no(); 
  }
  XPOINT *vertices() { 
    return ((fltk3::GraphicsDriver_I*)_p)->fltk3::GraphicsDriver::vertices(); 
  }
  int vertex_kind() {
    return ((fltk3::GraphicsDriver_I*)_p)->fltk3::GraphicsDriver::vertex_kind(); 
    }
  void fixloop() {
    ((fltk3::GraphicsDriver_I*)_p)->fltk3::GraphicsDriver::fixloop();
    }
  void prepare_circle(double x, double y, double r, int& llx, int& lly, int& w, int& h, double& xt, double& yt) {
    ((fltk3::GraphicsDriver_I*)_p)->fltk3::GraphicsDriver::prepare_circle(x, y, r, llx, lly, w, h, xt, yt);
  }
  Fl_Font font() { 
    return fltk3::_3to1_font( ((fltk3::GraphicsDriver_I*)_p)->fltk3::GraphicsDriver::font() );
  }
  Fl_Fontsize size() { 
    return fltk3::_3to1_fontsize( ((fltk3::GraphicsDriver_I*)_p)->size() );
  }
  void font(Fl_Font f, Fl_Fontsize s) { 
    ((fltk3::GraphicsDriver_I*)_p)->font(fltk3::_1to3_font(f), fltk3::_1to3_fontsize(s));
  }
  Fl_Color color() {
    return fltk3::_3to1_color( ((fltk3::GraphicsDriver_I*)_p)->fltk3::GraphicsDriver::color() );
    }
  void color(Fl_Color c) { 
    ((fltk3::GraphicsDriver_I*)_p)->color(fltk3::_1to3_color(c));
  }
  void color(uchar r, uchar g, uchar b) { 
    ((fltk3::GraphicsDriver_I*)_p)->color(r,g,b);
  }
  Fl_Region clip_region() {
    return ((fltk3::GraphicsDriver_I*)_p)->clip_region();
  }
  void clip_region(Fl_Region r) {
    ((fltk3::GraphicsDriver_I*)_p)->clip_region(r);
  }
  void mult_matrix (double a, double b, double c, double d, double x, double y) {
    ((fltk3::GraphicsDriver_I*)_p)->mult_matrix(a,b,c,d,x,y);
  }
  void pop_matrix() {
    ((fltk3::GraphicsDriver_I*)_p)->pop_matrix();
  }
  void push_matrix() {
    ((fltk3::GraphicsDriver_I*)_p)->push_matrix();
  }
  void restore_clip() {
    ((fltk3::GraphicsDriver_I*)_p)->restore_clip();
  }
  void rotate(double d) {
    ((fltk3::GraphicsDriver_I*)_p)->rotate(d);
  }
  void scale(double x, double y) {
    ((fltk3::GraphicsDriver_I*)_p)->scale(x,y);
  }
  void scale(double x) {
    ((fltk3::GraphicsDriver_I*)_p)->scale(x);
  }
  double transform_dx(double x, double y) {
    return ((fltk3::GraphicsDriver_I*)_p)->transform_dx(x,y);
  }
  double transform_dy(double x, double y) {
    return ((fltk3::GraphicsDriver_I*)_p)->transform_dy(x,y);
  }
  double transform_x(double x, double y) {
    return ((fltk3::GraphicsDriver_I*)_p)->transform_x(x,y);
  }
  double transform_y(double x, double y) {
    return ((fltk3::GraphicsDriver_I*)_p)->transform_y(x,y);
  }
  void translate(double x, double y) {
    ((fltk3::GraphicsDriver_I*)_p)->translate(x,y);
  }
};

FL_EXPORT extern Fl_Graphics_Driver *fl_graphics_driver;

#ifdef __APPLE__
class FL_EXPORT Fl_Quartz_Graphics_Driver : public Fl_Graphics_Driver {
public:
  Fl_Quartz_Graphics_Driver() {
    _p->wrapper(NULL);
    delete _p;
    _p = new fltk3::QuartzGraphicsDriver();
    _p->wrapper(this);
  }
};
#elif defined(WIN32)
class FL_EXPORT Fl_GDI_Graphics_Driver : public Fl_Graphics_Driver {
public:
  Fl_GDI_Graphics_Driver() {
    _p->wrapper(NULL);
    delete _p;
    _p = new fltk3::GDIGraphicsDriver();
    _p->wrapper(this);
  }
};
#else
class FL_EXPORT Fl_Xlib_Graphics_Driver : public Fl_Graphics_Driver {
public:
  Fl_Xlib_Graphics_Driver() {
    _p->wrapper(NULL);
    delete _p;
    _p = new fltk3::XlibGraphicsDriver();
    _p->wrapper(this);
  }
};
#endif

class FL_EXPORT Fl_Surface_Device : public fltk3::Wrapper {
protected:
  Fl_Surface_Device(Fl_Graphics_Driver* g) {
    _p = new fltk3::SurfaceDevice( (g && g->_p) ? (fltk3::GraphicsDriver*)g->_p : NULL );
    _p->wrapper(this);
  }
public:
  void set_current(void) {
    ((fltk3::SurfaceDevice*)_p)->set_current();
    fl_graphics_driver = driver();
  }
  
  void driver(Fl_Graphics_Driver *d) {
    ((fltk3::SurfaceDevice*)_p)->driver((fltk3::GraphicsDriver*)d->_p);
  }
  
  Fl_Graphics_Driver *driver() {
    fltk3::GraphicsDriver* g = ((fltk3::SurfaceDevice*)_p)->driver();
    if (!g->wrapper()) {
      fltk3::GraphicsDriverWrapper* w = new fltk3::GraphicsDriverWrapper;
      w->_p = g;
      g->wrapper(w);
    }
    return (Fl_Graphics_Driver*)g->wrapper();
  }
  
  static Fl_Surface_Device *surface() {
    fltk3::SurfaceDevice* s = fltk3::SurfaceDevice::surface(); 
    if (!s->wrapper()) {
      Wrapper* w = new Wrapper;
      w->_p = s;
      s->wrapper(w);
    }
    return (Fl_Surface_Device*)s->wrapper();
  }
};

class FL_EXPORT Fl_Display_Device : public Fl_Surface_Device {
public:
  Fl_Display_Device(Fl_Graphics_Driver *g) : Fl_Surface_Device(NULL) {
    _p->wrapper(NULL);
    delete _p;
    _p = new fltk3::DisplayDevice((fltk3::GraphicsDriver*)g->_p);
    _p->wrapper(this);
  }
  static Fl_Display_Device* display_device() { 
    fltk3::DisplayDevice* d = fltk3::DisplayDevice::display_device();
    if (!d->wrapper()) {
      Wrapper* w = new Wrapper;
      w->_p = d;
      d->wrapper(w);
    }
    return (Fl_Display_Device*)d->wrapper();
  }
};


#endif // Fl_Device_H

//
// End of "$Id$".
//
