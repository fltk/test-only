// "$Id$"
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

#include <config.h>
#include <fltk/Image.h>
#include <fltk/Widget.h>
#include <fltk/events.h>
#include <fltk/draw.h>
#include <fltk/x.h>

/*! \class fltk::Image

  A rectangular buffer of pixels that the program can write (and
  read?) and can be efficiently drawn on the screen. The draw()
  functions will copy (or "over" composite if there is alpha in the
  pixeltype()) onto the output, transformed by the current transform.

  If you already have a set of pixels sitting in your own memory,
  drawimage() can draw it and is \e much easier to use. You should use
  this class only if you will be drawing the \e same image multiple
  times (with no changes to the pixels), or if you can efficiently use
  the linebuffer() and setpixels() functions to write your image to
  the buffer as you generate it. Otherwise you will have no efficiency
  advantages over drawimage() and it may actually perform worse.

  Because Image is a subclass of Symbol, it may be used as a
  Widget::image() or as the box() in a Style. If you give it a name it
  can be drawn with "@name;" in a label. If resized, the Symbol
  _draw() method will use the inset() call to decide on edge
  thicknesses and will dice the image up into 9 individually-scaled
  pieces, which is very useful for GUI buttons and background images
  (this is similar to how Flash draws buttons).

  There are a number of subclasses such as jpgImage and pngImage that
  display compressed image data, either from in-memory data buffers or
  from files.

  <i>There is no destructor</i> due to C++'s lame insistence
  that it be called on static objects. An fltk program may contain
  many static instances and this destruction is a waste of time on
  program exit, plus work must be done to avoid losing the display
  connection before the destruction. If you do want to destroy an
  Image, you must call destroy() before doing so.
*/

using namespace fltk;

#if USE_X11
# include "x11/Image.cxx"
#elif defined(_WIN32)
# include "win32/Image.cxx"
#elif USE_QUARTZ
# include "osx/Image.cxx"
#else
# error
#endif

/*! \fn Image::Image(const char* name)
  The default constructor sets the width() and height() to -1, which
  indicates that setsize() has not been called. This allows measurement
  to be deferred until fetch() or _measure() is called. The pixeltype()
  is set to RGB32 (0x00rrggbb).

  The optional name is passed to the Symbol constructor and allows
  the image to be drawn by putting "@name;" into a label.
*/

/*! \fn Image::Image(int w, int h, const char* name)
  Does setsize(w,h). This causes the width() and height() to return
  the passed values. No buffer is allocated, call allocate() to do that.
  The pixeltype() is set to RGB32 (0x00rrggbb).
*/

/*! \fn Image::Image(PixelType p, int w, int h, const char* name)
  Does pixeltype(p) and setsize(w,h). No buffer is allocated, call
  allocate() to do that.
*/

/*! \fn Image::Image(const uchar* data, PixelType, int w, int h, int linedelta)
  Initializes the size and pixels by doing setimage(). Note that
  the pointer \a data may be used unchanged. If you want to force
  a copy you should use setpixels() instead.
*/

/*! \fn Image::Image(const uchar* data, PixelType, int w, int h)
  Initializes the size and pixels by doing setimage(). Note that
  the pointer \a data may be used unchanged. If you want to force
  a copy you should use setpixels() instead.
*/

/*! \fn PixelType Image::pixeltype() const
  Return the types of pixels that can be put into the image with
  setpixels(). You can change this with setpixeltype(). It is
  possible the internal data is in a different type, use
  buffer_pixeltype() to find out what that is.
*/

/*! \fn int Image::depth() const
  Same as ::depth(pixeltype()), this returns how many bytes
  each pixel takes in the buffer sent to setpixels().
*/

/*! \fn int Image::width() const
  Return the width of the image in pixels. You can change this with
  setsize().
*/

/*! \fn int Image::height() const
  Return the height of the image in pixels. You can change this with
  setsize().
*/

/*! \fn void Image::setpixeltype(PixelType);
  Change the stored pixeltype. If it is not compatable then
  destroy() is called. */

/*! \fn void Image::setsize(int w, int h)
  Change the size of the stored image. If it is not compatable with
  the current data size (generally if it is larger) then destroy()
  is called. */

/*! \fn void Image::setpixels(const uchar* data, const Rectangle&, int linedelta)
  Replace the given rectangle of buffer() with the supplied data,
  which must be in the pixeltype(). \a linedelta is the distance
  between each row of pixels in \a data. The rectangle is assummed
  to fit inside the width() and height().
*/

/** \fn void Image::setpixels(const uchar* data, const fltk::Rectangle& r)
  Figures out the linedelta for you as depth()*r.w().
*/

/*! \fn void Image::setpixels(const uchar* data, int y);
  Same as setpixels(data,Rectangle(0,y,width(),1)), sets the entire
  row of pixels.
*/

/*! \fn void Image::setimage(const uchar* d, PixelType p, int w, int h, int linedelta)
  This is equivalent to:
\code
  setsize(w,h);
  setpixeltype(p);
  setpixels(d,Rectangle(w,h),linedelta);
\endcode
  except, if possible, \a d is used as buffer() (throwing
  away the const!). This will happen if the pixeltype and linedelta
  are of types that it can handle unchanged and if the image memory
  does not need to be allocated by the system (currently OS/X only).
*/

/*! \fn void Image::setimage(const uchar* d, PixelType p, int w, int h)
  Figures out linedelta for you as w*depth(p).
*/

/*! \fn uchar* Image::linebuffer(int y)
  Return a pointer to a buffer that you can write up to width() pixels
  in pixeltype() to and then call setpixels(buffer,y) with. This can
  avoid doing any copying of the data if the internal format and
  pixeltype() are compatable, because it will return a pointer
  directly into the buffer and setpixels will detect this and do
  nothing.
*/

/*! \fn uchar* Image::buffer()
  Creates if necessary and returns a pointer to the internal pixel
  buffer. This is probably going to be shared memory with the graphics
  system, it may have a different pixeltype, size, and linedelta that
  the Image. If you are able to figure out the type you can read and
  write the pixels directly.

  The non-const version will sync any previous drawing with the
  display, on the assumption that you are about to write to the
  buffer.
*/

/*! \fn PixelType Image::buffer_pixeltype() const
  Return the type of pixels stored in buffer().  Likely to be
  ARGB32. On older (non-XRender) X system the types 1 and 2 indicate 1
  and 2-byte data, but there is no api to figure out anything more
  about this data.
*/

/*! \fn PixelType Image::buffer_depth() const
  Returns the number of bytes per pixel stored in buffer(). This
  is the same as ::depth(buffer_pixeltype()).
*/

/*! \fn int Image::buffer_width() const
  Return the width in pixels of buffer().
*/

/*! \fn int Image::buffer_height() const
  Return the height in pixels of buffer();
*/

/*! \fn int Image::buffer_linedelta() const
  Return the distance between each row of pixels in buffer().
*/

/*! \fn void Image::buffer_changed()
  Call this if you modify the contents of buffer(). On some systems
  the memory is not actually shared with the window system, and this
  will cause draw() to copy the buffer to the system's memory.
  setpixels() calls this for you.
*/

/*! \fn void Image::destroy()
  Destroys the buffer() and any related system structures.
*/

/*!
  This is called by the draw() functions once after the Image is
  created or after refetch() has been called.  This allows subclasses
  to defer reading files and calling setpixels() calls until the first
  draw() or measure(). This should return true if successful, false on
  any error (though fltk does not do anything useful with errors).

  The base class does nothing and returns true, thus leaving the
  image unchanged.

  Sample implementation:
\code
  bool MyImage::fetch() {
    setsize(get_width(file), get_height(file));
    setpixeltype(my_pixeltype);
    for (int y=0; y<height(); y++) {
      uchar* buffer = linebuffer(y);
      get_line_of_pixels(file, buffer, y);
      setpixels(buffer, y);
    }
    return true;
  }
\endcode
*/
bool Image::fetch() {return true;}

/*! \fn void Image::refetch()
  Cause fetch() to be called again. This is useful for a file image
  if the file name or contents have changed.
*/

/*! \fn void Image::fetch_if_needed() const
  Call fetch() if it has not been called or if refetch() was called.
*/

/**
  Returns width() and height().

  The default constructor sets an internal flag that indicates that
  fetch() must be called before the width() and height() are known.
  This is useful for subclasses that read a file and figure out both
  the size and pixels at the same time.
*/
void Image::_measure(int& W, int& H) const {
  if (flags & MEASUREFETCH)
    fetch_if_needed();
  W = w_;
  H = h_;
}

/*! \fn bool Image::fills_rectangle() const
  Returns true if the pixeltype does not support alpha.
*/
bool Image::fills_rectangle() const {
  return
    pixeltype_ != MASK &&
    pixeltype_ != RGBA &&
    pixeltype_ < ARGB32;
}

#include <fltk/Widget.h>

/**
  This is a 1.1 back-compatability function. It is the same as
  doing widget->image(this) and widget->label(0).
*/
void Image::label(Widget* o) {
  o->image(this);
  o->label(0);
}

/**
  Does measure() and then draw(Rectangle(0,0,w,h),Rectangle(x,y,w,h).
  Thus the top-left corner is placed at x,y and no scaling (other
  than due to the current transformation) is done.
*/
void Image::draw(int x, int y) const {
  int w,h; measure(w,h);
  draw(fltk::Rectangle(0,0,w,h), fltk::Rectangle(x,y,w,h));
}

/*! \fn void Image::draw(const Rectangle& from, const Rectangle& to) const

  Draws the subrectangle \a from of the image, transformed to fill
  the rectangle \a to (as transformed by the CTM). If the image has
  an alpha channel, an "over" operation is done.

  Due to lame graphics systems, this is not fully operational on all
  systems:
  * X11 without XRender extension: no transformations are done, the
  image is centered in the output area.
  * X11 with XRender: rotations fill the bounding box of the destination
  rectangle, drawing extra triangular areas outside the source rectangle.
  Somewhat bad filtering when making images smaller. xbmImage does
  not transform.
  * Windows: Only scaling, no rotations. Bad filtering. xbmImage does
  not do any transformations.
  * OS/X: works well in all cases.
*/

/**
  Resizes the image to fit in the rectangle. This is the virtual
  method from the Symbol base class, so this is what is called if
  the image is used as a label or box type.

  If the destination rectangle is not the same size, inset() is
  used to figure out the edge thicknesses. The image is then
  diced into 9 rectangles in a 3x3 grid by the insets, and
  each piece is scaled individually. This is very useful for
  scaling paintings of buttons. Note that if the insets are zero
  (the default) then the whole image is scaled as one piece.
  If you want, inset() can return different thicknesses depending
  on the size, producing very interesting scaling.

  It is possible this will use drawflags(INACTIVE) to gray out
  the image in a system-specific way. NYI.
*/
void Image::_draw(const fltk::Rectangle& r) const
{
  if (r.empty()) return;
  int w,h; measure(w,h);
  // quickly handle no-scaling:
  if (r.w()==w && r.h()==h) {
  NOINSETS:
    draw(fltk::Rectangle(0,0,w,h), r);
    return;
  }
  // now check the insets and use them to scale pieces individually:
  fltk::Rectangle in(0,0,w,h); this->inset(in);
  if (!in.x() && !in.y() && in.w()==w && in.h()==h) goto NOINSETS;
  int fx[4]; fx[0]=0; fx[1]=in.x(); fx[2]=in.r(); fx[3]=w;
  int fy[4]; fy[0]=0; fy[1]=in.y(); fy[2]=in.b(); fy[3]=h;
  fltk::Rectangle out(r); this->inset(out);
  int tx[4]; tx[0]=r.x(); tx[1]=out.x(); tx[2]=out.r(); tx[3]=r.r();
  int ty[4]; ty[0]=r.y(); ty[1]=out.y(); ty[2]=out.b(); ty[3]=r.b();
  for (int y=0; y<3; y++) if (fy[y+1]>fy[y] && ty[y+1]>ty[y]) {
    for (int x=0; x<3; x++) if (fx[x+1]>fx[x] && tx[x+1]>tx[x]) {
      draw(fltk::Rectangle(fx[x],fy[y],fx[x+1]-fx[x],fy[y+1]-fy[y]),
	   fltk::Rectangle(tx[x],ty[y],tx[x+1]-tx[x],ty[y+1]-ty[y]));
    }
  }
}

/*! \fn unsigned long Image::mem_used() const;
  Returns how much memory the image is using for buffer() and for
  any other structures it created. Returns zero if buffer() has
  not been called.
*/

/*! \fn unsigned long Image::total_mem_used()
  Sum of all mem_used() calls to all Images.  This is used by
  SharedImage to decide when to clear out cached images.
*/

////////////////////////////////////////////////////////////////
// drawimage()

static Image* reused_image;

/*!

  Draw a image (a rectangle of pixels) stored in your program's
  memory. The current transformation (scale, rotate) is applied.

  - \a pointer points at the first byte of the top-left pixel.
  - \a type describes how to interpret the bytes of each pixel.
  - \a rectangle: the image is put in the top-left corner
  and the width and height declare how many pixels the image has.
  - \a line_delta is how much to add to \a pointer to go 1 pixel down.
  By setting \a line_delta to larger than \a depth(type)*r.w() you can
  crop a picture out of a larger buffer. You can also set it negative
  for images that are stored with bottom-to-top in memory,
  notice that in this case \a pointer still points at the top-left
  pixel, which is at the \e end of your buffer minus one line_delta.

  The X version of FLTK will abort() if the default visual is one
  it cannot use for images. To avoid this call fltk::visual(fltk::RGB)
  at the start of your program.
*/
void fltk::drawimage(const uchar* pointer, fltk::PixelType type,
		     const Rectangle& r,
		     int line_delta) {
  if (innards(pointer, type, r, line_delta, 0, 0)) return;
  // Fake it using a temporary image
  if (!reused_image) reused_image = new Image();
  reused_image->setimage(pointer, type, r.w(), r.h(), line_delta);
  reused_image->draw(Rectangle(r.w(),r.h()), r);
}

/*!
  Same except \a line_delta is set to <i>r</i>.w() times
  depth(<i>type</i>), indicating the rows are packed together one
  after another with no gap.
*/
void fltk::drawimage(const uchar* pointer, fltk::PixelType type,
		     const Rectangle& r) {
  drawimage(pointer, type, r, depth(type)*r.w());
}

/*! \typedef fltk::DrawImageCallback

  Type of function passed to drawimage(). It must return a pointer
  to a horizontal row of \a w pixels, starting with the pixel at
  \a x and \a y (relative to the top-left corner of the image, not
  to the coordinate space drawimage() is called in). These pixels
  must be in the format described by \a type passed to drawimage()
  and must be the \a delta apart passed to drawimage().

  \a userdata is the same as the argument passed to drawimage().
  This can be used to point at a structure of information about
  the image.

  Due to cropping, less than the whole image may be requested. So the
  callback may get an \a x greater than zero, the first \a y passed to
  it may be greater than zero, and \a x+w may be less than the width
  of the image.  The passed \a buffer contains room for at least the
  number of pixels specified by the width passed to drawimage(). You
  can use this as temporary storage to construct a row of the image,
  and return a pointer offset by \a x into it.

*/

/**
  Call the passed function to provide each scan line of the
  image. This lets you generate the image as it is being drawn, or do
  arbitrary decompression of stored data (provided it can be
  decompressed to individual scan lines easily).

  \a callback is called with the void* \a data argument (this can be
  used to point at a structure of information about the image), the x,
  y, and number of pixels desired from the image, measured from the
  upper-left corner of the image. It is also given a buffer of at
  least \a w pixels that can be used as temporary storage, for
  instance to decompress a line read from a file. You can then return
  a pointer to this buffer, or to somewhere inside it.

  The callback must return n pixels of the format described by \a
  type.

  The \a xywh rectangle describes the area to draw. The callback is
  called with y values between 0 and h-1. Due to cropping not
  all pixels may be asked for. You can assumme y will be asked for in
  increasing order.
*/
void fltk::drawimage(DrawImageCallback cb,
		     void* userdata, fltk::PixelType type,
		     const Rectangle& r) {
  if (innards(0, type, r, 0, cb, userdata)) return;
  // fake it using an Image object
  // Fake it using a temporary image
  if (!reused_image) reused_image = new Image();
  reused_image->setpixeltype(type);
  reused_image->setsize(r.w(), r.h());
  for (int y=0; y<r.h(); y++)
    reused_image->setpixels(cb(userdata, 0, y, r.w(), reused_image->linebuffer(y)), y);
  reused_image->draw(Rectangle(r.w(),r.h()), r);
}

//
// End of "$Id$".
//
