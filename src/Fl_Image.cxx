//
// "$Id: Fl_Image.cxx,v 1.51 2005/01/25 09:49:11 spitzak Exp $"
//
// Image drawing code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <config.h>
#include <fltk/Image.h>
#include <fltk/Widget.h>
#include <fltk/events.h>
#include <fltk/draw.h>
#include <fltk/x.h>

#if !USE_X11 && defined(_WIN32)
// Extra bitblt functions:
#define NOTSRCAND       0x00220326 /* dest = (NOT source) AND dest */
#define NOTSRCINVERT    0x00990066 /* dest = (NOT source) XOR dest */
#define SRCORREVERSE    0x00DD0228 /* dest = source OR (NOT dest) */
#define SRCNAND         0x007700E6 /* dest = NOT (source AND dest) */
#define MASKPAT         0x00E20746 /* dest = (src & pat) | (!src & dst) */
#define COPYFG          0x00CA0749 /* dest = (pat & src) | (!pat & dst) */
#define COPYBG          0x00AC0744 /* dest = (!pat & src) | (pat & dst) */

#if defined(__MINGW32__) || defined(__CYGWIN__)
// MinGW headers does not declare AlphaBlend..
extern "C" {
  WINGDIAPI BOOL  WINAPI AlphaBlend(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
}
#endif
#endif

/*
We support both "old" 1-bit alpha in a seperate object, and "new"
8-bit alpha merged in with the rgb data. This is stored in the following
way:

if (rgb) {
  if (alpha == rgb) {
    rgb = 4-channel 8-bit image, do not use alpha independently
  } else if (alpha) {
    rgb = 3-channel 8-bit image, alpha = 1-bit image
  } else {
    rgb = 3-channel image, pretend alpha == 1 everywhere
  }
} else if (alpha) {
  act like rgb==0, alpha = 1-bit alpha image (bitmap)
} else {
  image has not been set, don't draw anything
}

*/

using namespace fltk;

/*! \class fltk::Image

  This subclass of Symbol draws a very common thing: a fixed-size
  "offscreen" image, possibly with alpha information.

  The base class lets the program draw into this offscreen image, and
  then copy or composite the resulting image into the windows that are
  visible to the user. There are also many subclasses that take
  in-memory data such as a jpeg image, and in draw() they copy that
  data to the offscreen image if it has not been done before, before
  calling this base class to draw the image into the window.

  <i>If you are changing the image a lot (for instance a movie
  playback or a painting program) you probably don't want to use
  this.</i> Just call fltk::drawimage() directly with your image buffer.

  Treatment of alpha is seriously primitive on X11 and GDI32. About
  the only thing that works is fltk::drawimage() with a 4-channel
  image (and even then it is reduced to a horrible "screen door"
  alpha), and calling set_alpha_bitmap().

  <i>There is no destructor</i> due to C++'s lame insistence
  that it be called on static objects. An fltk program may contain
  many static instances and this destruction is a waste of time on
  program exit, plus work must be done to avoid losing the display
  connection before the destruction. If you do want to destroy an
  Image, you must call destroy_cache() before doing so.
*/

/*! \fn Image::Image(const char* name)
  The default constructor makes a zero-sized image. This will not
  draw anything, you have to call setsize() on it. Most image
  subclasses use this initial zero size as an indication that they
  have not analyzed the data and figured out the size.

  The optional name is passed to the Symbol constructor and allows
  the image to be drawn by puttin "@name;" into a label.
*/

/*! \fn Image::Image(int w, int h, const char* name)
  This constructor sets the width and height. The initial
  image is an opaque black rectangle of that size.

  The optional name is passed to the Symbol constructor and allows
  the image to be drawn by puttin "@name;" into a label.
*/

/*! \fn int Image::width() const
  Return the width of the image in pixels. You can change this with
  setsize().
*/
/*! \fn int Image::height() const
  Return the height of the image in pixels. You can change this with
  setsize().
*/

/*! Change the size of the stored image. If it is different then
  destroy_cache() is called. */
void Image::setsize(int w, int h) {
  if (w == w_ && h == h_) return;
  destroy_cache();
  w_ = w;
  h_ = h;
}

/*! Throw away any drawn data, restoring back to a transparent rectangle.
  You must subsequently to make_current() to start drawing it again.
*/
void Image::destroy_cache() {
#if USE_X11
  stop_drawing((XWindow)rgb);
  if (alpha && alpha!=rgb) {XFreePixmap(xdisplay, (Pixmap)alpha); alpha = 0;}
  if (rgb) {XFreePixmap(xdisplay, (Pixmap)rgb); rgb = 0;}
#elif defined(_WIN32)
  stop_drawing((HBITMAP)rgb);
  if (alpha && alpha != rgb) {DeleteObject((HBITMAP)alpha); alpha = 0;}
  if (rgb) {DeleteObject((HBITMAP)rgb); rgb = 0;}
#elif defined(__APPLE__)
  stop_drawing((GWorldPtr)rgb);
  if (alpha && alpha != rgb) {DisposeGWorld((GWorldPtr)alpha); alpha = 0;}
  if (rgb) {DisposeGWorld((GWorldPtr)rgb); rgb = 0;}
#else
#error
#endif
}

/*! \fn bool Image::drawn() const
  Returns true if make_current() has been called since the Image
  was constructed or since destroy_cache() was called. Subclasses
  use this to decide if they can call the base class _draw() without
  any more setup.
*/

/*! Make all the \ref drawing functions draw into the offscreen image,
  possibly creating the arrays used to store it.

  This is designed to be called outside of drawing code, as it will
  replace the current drawing state. To draw into an Image while you
  are drawing a widget (or drawing another Image) you must use a
  ImageDraw object to save the state.
*/
void Image::make_current() {
  if (!rgb) {
    open_display();
    if (w_<1) w_ = 1;
    if (h_<1) h_ = 1;
#if USE_X11
    rgb = (void*)(XCreatePixmap(xdisplay, xwindow, w_, h_, xvisual->depth));
#elif defined(_WIN32)
    //rgb = (void*)(CreateCompatibleBitmap(getDC(), w_, h_));

    // Use CreateDIBSection instead, it seems to be only reliable way to
    // make AlphaBlend function working correctly always..

    BITMAPINFO bmi;
    // zero the memory for the bitmap info
    memset(&bmi, 0, sizeof(BITMAPINFO));
    // setup bitmap info 
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w_;
    bmi.bmiHeader.biHeight = h_;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = w_ * h_ * 4;

    rgb = (void*)CreateDIBSection(getDC(), &bmi, DIB_RGB_COLORS, NULL, NULL, 0x0);    
#elif defined(__APPLE__)
    // ???
#endif
  }
#if USE_X11
  draw_into((XWindow)rgb);
#elif defined(_WIN32)
  draw_into((HBITMAP)rgb);
#elif defined(__APPLE__)
  draw_into((GWorldPtr)rgb);
#endif
}

/** Set the alpha channel directly to a 1-bit alpha mask.

    This method provides direct access to the primitive alpha channel
    setting code that is on X11 and GDI32. Any existing alpha channel
    is directly replaced with the 1-bit mask, and the image size is
    set to w,h.

    Each bit of the data is a pixel of alpha, where 1 indicates
    opaque and 0 indicates clear. Each byte supplies 8 bits, the
    high bit being the left-most one. Rows are padded out to the
    next multiple of 8, so the left-most column of every row is
    the high bit of the mask.

    Subclasses can use this if they have a 1-bit mask:

    \code
    MyImage::draw(x,y,w,h,style,flags) {
      if (!drawn()) {
        ImageDraw(const_cast<Image*>(this));
	draw_rgb_part();
        uchar* data = generate_ae_bitmap();
	const_cast<Image*>(this)->set_alpha_bitmap(bitmap, w, h);
	free[] data;
      }
      Image::_draw(x,y,w,h,style,flags);
    }
    \endcode
*/
void Image::set_alpha_bitmap(const uchar* bitmap, int w, int h) {
  if (!rgb) {w_ = w; h_ = h;}
#if USE_X11
  if (alpha && alpha != rgb) XFreePixmap(xdisplay, (Pixmap)alpha);
  alpha = (void*)XCreateBitmapFromData(xdisplay, xwindow, (char*)bitmap, (w+7)&-8, h);
#elif defined(_WIN32)
  if (alpha && alpha != rgb) DeleteObject((HBITMAP)alpha);
  // this won't work when the user changes display mode during run or
  // has two screens with differnet depths
  static uchar hiNibble[16] =
  { 0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
    0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0 };
  static uchar loNibble[16] =
  { 0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e,
    0x01, 0x09, 0x05, 0x0d, 0x03, 0x0b, 0x07, 0x0f };
  int Bpr = (w+7)/8;			//: bytes per row
  int pad = Bpr&1, w1 = (w+7)/8; //shr = ((w-1)&7)+1;
  uchar *newarray = new uchar[(Bpr+pad)*h], *dst = newarray;
  const uchar* src = bitmap;
  for (int i=0; i<h; i++) {
    //: this is slooow, but we do it only once per pixmap
    for (int j=w1; j>0; j--) {
      uchar b = *src++;
      *dst++ = ( hiNibble[b&15] ) | ( loNibble[(b>>4)&15] );
    }
    if (pad)
      *dst++ = 0;
  }
  alpha = (void*)CreateBitmap(w, h, 1, 1, newarray);
  delete[] newarray;
#elif defined(__APPLE__)
  // nyi this is expected to make a GWorld object...
#else
#endif
}

/*! \class fltk::ImageDraw
  The constructor of this class saves enough information so that the
  current location graphics are being drawn can be restored by the
  destructor. It then does image->make_current(). Typical usage:

  \code
  if (!image.drawn()) {
    ImageDraw x(image);
    draw_graphics_for_image();
    // destructor of the ImageDraw happens here
  }
  // we can now draw our image into the window:
  image.draw(x,y,w,h);
  \endcode
*/

#ifdef _WIN32
extern HDC fl_bitmap_dc;
#endif

ImageDraw::ImageDraw(Image* image) {
  push_matrix();
#if USE_X11
  data[0] = (void*)(xwindow);
#elif defined(_WIN32)
  // make it not destroy the previous dc:
  data[0] = (void*)dc;
  data[1] = (void*)(fl_bitmap_dc); fl_bitmap_dc = 0;
#elif defined(__APPLE__)
  GrafPtr prevPort; GDHandle prevGD;
  GetGWorld(&prevPort, &prevGD);
  data[0] = (void*)prevPort;
  data[1] = (void*)prevGD;
#else
# error
#endif
  image->make_current();
  push_no_clip();
}

ImageDraw::~ImageDraw() {
#if USE_X11
  xwindow = (XWindow)(data[0]);
#elif defined(_WIN32)
  dc = (HDC)(data[0]);
  DeleteDC(fl_bitmap_dc);
  fl_bitmap_dc = (HDC)(data[1]);
#elif defined(__APPLE__)
  SetGWorld((GrafPtr)(data[0]), (GDHandle)(data[1]));
#else
# error
#endif
  pop_clip();
  pop_matrix();
}

void fl_restore_clip(); // in rect.C

// This macro creates rectangle R and modifies src_x,src_y to a region
// that is visible and clipped to the size of the image. It will call
// return if it is invisible.
#define clip_code() \
  fltk::Rectangle ir(r1); transform(ir); \
  fltk::Rectangle R(ir); if (!intersect_with_clip(R)) return; \
  src_x += R.x()-ir.x(); \
  if (src_x < 0) {R.move_x(-src_x); src_x = 0;} \
  if (src_x >= w_) return; \
  if (src_x+R.w() > w_) R.w(w_-src_x); \
  if (R.w() <= 0) return; \
  src_y += R.y()-ir.y(); \
  if (src_y < 0) {R.move_y(-src_y); src_y = 0;} \
  if (src_y >= h_) return; \
  if (src_y+R.h() > h_) R.h(h_-src_y); \
  if (R.h() <= 0) return

/*! Copy a rectangle of rgb from the cached image to the current output.
  This is the same as over() except it pretends the alpha is all 1's.

  The image is positioned so the pixel at src_x, src_y is placed at
  the top-left of the rectangle \a r (or the equivalent if src_x,src_y
  are outside the image). The part of the image that then intersects
  \a r is then drawn.
*/
void Image::copy(const fltk::Rectangle& r1, int src_x, int src_y) const {
  // handle undrawn images like the documentation says, as black:
  if (!rgb) {setcolor(BLACK); fillrect(r1); return;}
  clip_code();
#if USE_X11
  XCopyArea(xdisplay, (Pixmap)rgb, xwindow, gc,
	    src_x, src_y, R.w(), R.h(), R.x(), R.y());
#elif defined(_WIN32)
  HDC new_dc = CreateCompatibleDC(dc);
  SelectObject(new_dc, (HBITMAP)rgb);
  BitBlt(dc, R.x(), R.y(), R.w(), R.h(), new_dc, src_x, src_y, SRCCOPY);
  DeleteDC(new_dc);
#elif defined(__APPLE__)
  // NYI!
#else
#error
#endif
}

// Link against msimg32 lib to get the AlphaBlend function:
#if defined(_MSC_VER)
# pragma comment(lib, "msimg32.lib")
#endif

/*! Merge the image over whatever is in the current output. If this
  image has no alpha this is the same as copy. If there is an alpha
  channel then pixels are replaced as a "non premultiplied over"
  operation. If A is the Image's pixel, a is the alpha normalized
  to 0-1, and B is the pixel already in the output, the new output
  pixel is A*a+B*(1-a).

  The image is positioned so the pixel at src_x, src_y is placed at
  the top-left of the rectangle \a r (or the equivalent if src_x,src_y
  are outside the image). The part of the image that then intersects
  \a r is then drawn.
*/
void Image::over(const fltk::Rectangle& r1, int src_x, int src_y) const {

  // Draw bitmaps as documented, the rgb pretends to be black:
  if (!rgb) { setcolor(BLACK); fill(r1, src_x, src_y); return; }

  // Don't waste time for solid white alpha:  
  if (!alpha) { copy(r1, src_x, src_y); return; }

  // okay now we know we have rgb and alpha, draw it:
  clip_code();
#if USE_X11
  // I can't figure out how to combine a mask with existing region,
  // so the mask replaces the region instead. This can draw some of
  // the image outside the current clip region if it is not rectangular.
  if (alpha != rgb) XSetClipMask(xdisplay, gc, (Pixmap)alpha);
  // alpha == rgb indicates a real alpha is in the source pixmap. I think
  // the Render extension is needed to draw that...
  XSetClipOrigin(xdisplay, gc, R.x()-src_x, R.y()-src_y);
  XCopyArea(xdisplay, (Pixmap)rgb, xwindow, gc,
	    src_x, src_y, R.w(), R.h(), R.x(), R.y());
  // put the old clip region back:
  XSetClipOrigin(xdisplay, gc, 0, 0);
  fl_restore_clip();
#elif defined(_WIN32)
  if (alpha == rgb) {
    HDC new_dc = CreateCompatibleDC(dc);
    SelectObject(new_dc, (HGDIOBJ)rgb);	
    BLENDFUNCTION m_bf;
    m_bf.BlendOp = AC_SRC_OVER;
    m_bf.BlendFlags = 0;
    m_bf.AlphaFormat = 0x1;
    m_bf.SourceConstantAlpha = 0xFF;
    AlphaBlend(dc, R.x(), R.y(), R.w(), R.h(),
	       new_dc, src_x, src_y, R.w(), R.h(), m_bf);
    DeleteDC(new_dc);
    return;
  }
  // Various attempts to get GDI32 to obey 1-bit mask channel. ARRGH!
# if 1
  HDC new_dc = CreateCompatibleDC(dc);
  // Old version, are we sure this does not work?
  SetTextColor(dc, 0);
  SelectObject(new_dc, (HBITMAP)alpha);
  BitBlt(dc, R.x(), R.y(), R.w(), R.h(), new_dc, src_x, src_y, NOTSRCAND);
  SelectObject(new_dc, (HBITMAP)rgb);
  BitBlt(dc, R.x(), R.y(), R.w(), R.h(), new_dc, src_x, src_y, SRCPAINT);
  DeleteDC(new_dc);
# else
# if 0
  // I found this in the documentation. It works, but (unbelivable!) it
  // blinks worse than the more complicated code below. Darn you, Gates!
  HDC new_dc = CreateCompatibleDC(dc);
  SelectObject(new_dc, (HBITMAP)rgb);
  MaskBlt(dc, R.x(), R.y(), R.w(), R.h(), new_dc, src_x, src_y,
	  (HBITMAP)alpha, src_x, src_y,
	  MAKEROP4(SRCCOPY,0xEE0000));
  DeleteDC(new_dc);
# else
  // VP : new code to draw masked image under windows.
  // Maybe not optimal, but works for win2k/95 and probably 98
  // WAS: This can probably be fixed by having set_bitmap_alpha mangle
  // the rgb buffer to do this "premultiply". However I really suspect
  // there is a direct method of doing this...
  //setbrush();
  SetTextColor(dc, 0);
  HDC new_dc = CreateCompatibleDC(dc);
  SelectObject(new_dc, (HBITMAP)alpha);
  BitBlt(dc, R.x(), R.y(), R.w(), R.h(), new_dc, src_x, src_y, NOTSRCAND);
  HDC new_dc2 = CreateCompatibleDC(dc);
  SelectObject(new_dc2, (HBITMAP)rgb);
  BitBlt(new_dc2, 0, 0, w_, h_, new_dc, 0, 0, SRCAND); // This should be done only once for performance
  BitBlt(dc, R.x(), R.y(), R.w(), R.h(), new_dc2, src_x, src_y, SRCPAINT);
  DeleteDC(new_dc2);
  DeleteDC(new_dc);
# endif
# endif
#elif defined(__APPLE__)
  // OSX version nyi
#else
#error
#endif
}

/** Draw the alpha channel in the current color.

  The alpha is used to mix each output pixel between it's current
  color and the color set with fltk::setcolor(). Another way to state
  this is that it acts like the rgb data is a solid rectangle of the
  current color, and then draws the image, including any alpha
  channel.

  This is very useful for making a 1-channel image that works just like
  an anti-aliased character in a font, and can be drawn in any color.
  It is also used to draw inactive images.
*/
void Image::fill(const fltk::Rectangle& r1, int src_x, int src_y) const
{
  // If there is no alpha channel then act like it is all white
  // and thus a rectangle should be drawn:
  if (!alpha) {fillrect(r1); return;}

  clip_code();
#if USE_X11
  // alpha == rgb indicates a real alpha is in the source pixmap. I think
  // the Render extension is needed to draw that, this draws a solid rectangle:
  if (alpha != rgb) {
    XSetStipple(xdisplay, gc, (Pixmap)alpha);
    int ox = R.x()-src_x; if (ox < 0) ox += w_;
    int oy = R.y()-src_y; if (oy < 0) oy += h_;
    XSetTSOrigin(xdisplay, gc, ox, oy);
    XSetFillStyle(xdisplay, gc, FillStippled);
  }
  XFillRectangle(xdisplay, xwindow, gc, R.x(), R.y(), R.w(), R.h());
  XSetFillStyle(xdisplay, gc, FillSolid);
#elif defined(_WIN32)
  HDC tempdc = CreateCompatibleDC(dc);
  if (alpha == rgb) {
    // This is still not correct.. According to:
    //   "if C is the current color set with fltk::setcolor(),
    //   A is the alpha, and the current display is B,
    //   replace each pixel with B*(1-A)+A*C."
    // (ie ignore the rgb and act like the current color is a solid rectangle
    //fillrect(X,Y,w,h);
    SelectObject(tempdc, (HGDIOBJ)rgb);	
    BLENDFUNCTION m_bf;
    m_bf.BlendOp = AC_SRC_OVER;
    m_bf.BlendFlags = 0;
    m_bf.AlphaFormat = 0x1;
    m_bf.SourceConstantAlpha = 50;
    AlphaBlend(dc, R.x(), R.y(), R.w(), R.h(), tempdc,
	       src_x, src_y, R.w(), R.h(), m_bf);
  } else {
    // 1-bit alpha
    SetTextColor(dc, 0); // VP : seems necessary at least under win95
    setbrush();
    SelectObject(tempdc, (HBITMAP)alpha);
    // On my machine this does not draw the right color! But lots of
    // documentation indicates that this should work:
    BitBlt(dc, R.x(), R.y(), R.w(), R.h(), tempdc, src_x, src_y, MASKPAT);
  }
  DeleteDC(tempdc);
#elif defined(__APPLE__)
  // OSX version nyi
#else
# error
#endif
}

/** Virtual method from Symbol baseclass, draws the image.

  If the INACTIVE flag is on, this tries to draw the image inactive
  by calling fill() twice with gray colors. Otherwise it calls over().
*/
void Image::_draw(const fltk::Rectangle& r, const Style* style, Flags flags) const
{  
  if (flags & INACTIVE) {
    Color bg, fg; style->boxcolors(flags, bg, fg);
    setcolor(GRAY90);
    fill(r,-1,-1);
    setcolor(fg);
    fill(r,0,0);
  } else {
    over(r,0,0);
  }
}

/*! By default Image assummes the constructor set the w_ and h_
  fields, and returns them.

  For many subclasses (such as ones that read a file!) you certainly
  want to defer this calculation until first use. The way to do this
  is to put zero into the w_ and h_ in the constructor, and override this
  method with your own which calculates the values and sets them if
  it has not done so already.
*/
void Image::_measure(int& W, int& H) const { W=w(); H=h(); }

/*! If the image has no alpha, it claims to fill the box. This is
  only true if you draw the size it returned from measure() or
  smaller. */
const BoxInfo* Image::boxinfo() const {
  if (!rgb || alpha) return Symbol::boxinfo();
  static const BoxInfo filled = {0,0,0,0,true};
  return &filled;
}

#include <fltk/Widget.h>

/*! This is a 1.1 back-compatability function. It is the same as
  doing widget->image(this) and widget->label(0).
*/
void Image::label(Widget* o) {
  o->image(this);
  o->label(0);
}

//
// End of "$Id: Fl_Image.cxx,v 1.51 2005/01/25 09:49:11 spitzak Exp $".
//
