// "$Id$"
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

using namespace fltk;

/*! \class fltk::Image

  This subclass of Symbol draws a very common thing: a fixed-size
  "offscreen" image, containing color and alpha information for
  a rectangle of pixels.

  This is for \e static images. If your image is changing (ie a movie
  playback, or the image in a painting program) then you <i>do not
  want to use this</i>. Just call fltk::drawimage() directly with your
  image buffer. Notice that fltk1 had numerous calls for messing with
  the image, these have been deleted in fltk2 to prevent misuse of
  this object.

  The theoretical API is that you can draw anything using fltk calls
  into the image:

\code
  static fltk::Image image;
  if (!image.drawn()) { // have we been here before?
    // no, draw the image's contents:
    fltk::GSave gsave;
    image.setsize(w,h);
    image.make_current();
    fltk::drawtext(...); // any fltk drawing code you want
  }
  // now draw a lot of copies quickly:
  for (int i=0; i<100; i++)
    image.over(x+(i%10)*w, y+(i/10)*h);
\endcode

  The unfortunate reality is that, due to the limits of the underlying
  graphics API's, the only drawing that is guaranteed to work is to
  do a single fltk::drawimage() with the rectangle exactly equal to
  0,0,w(),h(). Also if you pask fltk::MASK as the pixeltype to
  drawimage(), this is remembered, and over() will fill with the
  current color.

  In theory the current transformation is applied to the image when
  drawing it. Again the unfortunate reality is that on some systems
  anything other than the translation is ignored (however on most
  systems of interest scaling but not rotation works).

  Because Image is a subclass of Symbol, it may be used as a
  Widget::image() or as the box() in a Style. If you give it a name it
  can be drawn with "@name;" in a label. The draw() method will set
  the scale so that the image is scaled to fill the rectangle. Unless
  you have done make_current() it will draw as nothing, so you may
  want to do a subclass where draw() is implemented like this:

\code
  void MyImage::_draw(const Rectangle& r) const {
    if (!drawn()) {
      fltk::GSave gsave;
      (const_cast<MyImage*>(this))->make_current();
      fltk::drawimage(...);
    }
    fltk::Image::_draw(r);
  }
\endcode

  There are a number of subclasses that use the above method to
  draw jpeg or png images, either from in-memory data buffers
  or from files.

  For use as a Style::box(), you can subclass and override the
  inset() method. The draw() function will take the inset at
  the unscaled size and the inset of the drawing rectangle, and
  cut the image into 9 parts, scaling each of them so that the
  inset edges line up.

  <i>There is no destructor</i> due to C++'s lame insistence
  that it be called on static objects. An fltk program may contain
  many static instances and this destruction is a waste of time on
  program exit, plus work must be done to avoid losing the display
  connection before the destruction. If you do want to destroy an
  Image, you must call destroy() before doing so.
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
  destroy() is called. */
void Image::setsize(int w, int h) {
  if (w == w_ && h == h_) return;
  destroy();
  w_ = w;
  h_ = h;
}

/*! \fn void Image::redraw() {
  Delete any cached data, reverting to a transparent rectangle, and
  make drawn() return false.
*/

/*! \fn bool Image::drawn() const
  Returns true if make_current() has been called since the Image
  was constructed or since redraw() was called. Subclasses
  use this to decide if they can call the base class _draw() without
  any more setup.
*/

/*! \fn void Image::make_current();
  Make all the \ref drawing functions draw into the offscreen image,
  possibly creating the arrays used to store it.

  This is designed to be called outside of drawing code, as it will
  replace the current drawing state. To draw into an Image while you
  are drawing a widget (or drawing another Image) you must use a
  GSave object to save the state.
*/

/**
  By default Image assummes the constructor set the w_ and h_
  fields, and returns them.

  For many subclasses (such as ones that read a file!) you certainly
  want to defer this calculation until first use. The way to do this
  is to put zero into the w_ and h_ in the constructor, and override this
  method with your own which calculates the values and sets them if
  it has not done so already.
*/
void Image::_measure(int& W, int& H) const { W=w(); H=h(); }

/*! \fn bool Image::fills_rectangle() const
  Peeks into the internal data and returns true if it knows that
  the alpha is solid 1 everywhere in the rectangle.
*/

#include <fltk/Widget.h>

/*! This is a 1.1 back-compatability function. It is the same as
  doing widget->image(this) and widget->label(0).
*/
void Image::label(Widget* o) {
  o->image(this);
  o->label(0);
}

/**
  This does measure() and then draw(Rectangle(x,y,w(),h()). Thus it
  draws it without any scaling and with the top-left corner at the x,y
  position.
*/
void Image::draw(int x, int y) const {
  int w,h; measure(w,h);
  draw(Rectangle(x,y,w,h));
}

#if USE_X11
# include "x11/Image.cxx"
#elif defined(_WIN32)
# include "win32/Image.cxx"
#elif USE_QUARTZ
# include "osx/Image.cxx"
#else
# error
#endif

/**
  Virtual method from Symbol baseclass, calls over() after
  setting the transform to scale it to fill the rectangle. (If
  you override inset() then this will do a much more complex
  scale and clipping and multiple calls to over() to scale
  the borders and interior to match).

  It is possible this will use drawflags(INACTIVE) to gray out
  the image is a system-specific way. NYI.
*/
void Image::_draw(const fltk::Rectangle& r) const
{
  if (!drawn() || r.empty()) return;
  over(r.x(), r.y());
}

/** \fn void Image::destroy();
  Same as redraw() but it also deallocates as much memory as possible.
*/

//
// End of "$Id$".
//
