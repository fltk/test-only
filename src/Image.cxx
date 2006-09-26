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
#include <fltk/string.h>
#include <ctype.h>

using namespace fltk;

/*! \class fltk::Image

  This subclass of Symbol draws a very common thing: a fixed-size
  "offscreen" image, containing color and alpha information for
  a rectangle of pixels. Call over() or draw(rectangle) to composite
  a copy on the screen, transformed by the current transform.

  This is for \e static images. If your image is changing (ie a movie
  playback, or the image in a painting program) then you <i>do not
  want to use this</i>. Just call fltk::drawimage() directly with your
  image buffer. Notice that fltk1 had numerous calls for messing with
  the image, these have been deleted in fltk2 to prevent misuse of
  this object. Well, no, they have reappeared. I guess nobody actually
  reads this documentation...

  Planned fixed implementation to allow an OPTIONAL buffer to mess
  with the data:

  * Normal drawing will go directly from the source data (the inline_data
  or the file) to the system "picture" image. DO NOT RELY ON THE BUFFER
  BEING CREATED!!!!

  * "fetch()" will create a buffer, load it with the same data as
  the normal drawing will do. Calling it repeatedly will "revert"
  the buffer back to the initial data. An attempt will be made to
  reuse the buffer created by the "picture" object on Win32 and OS/X.
  When fetch() is called from read(), it automatically dispose the buffer
  after use, only manual/previous call to fetch() permits
  to apply generic data algorithms on it.
  * an "unfetch()" can be used to dispose of the local buffer, 
    to achieve that simply call the dealloc_data() method.

  * Merge rgbImage into this base class, allow creation of an Image
  with arbitrary buffer and pixeltype.

  Subclasses must implement the update() method. In \e theory this can
  use any fltk drawing functions to draw the contents of the image. In
  reality on most platforms the only thing that works is a single
  fltk::drawimage().

  Because Image is a subclass of Symbol, it may be used as a
  Widget::image() or as the box() in a Style. If you give it a name it
  can be drawn with "@name;" in a label. Overriding the inset() method
  can produce very useful images for buttons with fixed-thickness
  edges.

  There are a number of subclasses that draw jpeg or png images,
  either from in-memory data buffers or from files.

  <i>There is no destructor</i> due to C++'s lame insistence
  that it be called on static objects. An fltk program may contain
  many static instances and this destruction is a waste of time on
  program exit, plus work must be done to avoid losing the display
  connection before the destruction. If you do want to destroy an
  Image, you must call destroy() before doing so.
*/

/*! \fn Image::Image(const char* name=0)
  The default constructor makes a zero-sized image. This will not
  draw anything, you have to call setsize() on it. Most image
  subclasses use this initial zero size as an indication that they
  have not analyzed the data and figured out the size.

  The optional name is passed to the Symbol constructor and allows
  the image to be drawn by puttin "@name;" into a label.
*/

  
  /*! \fn Image::Image(int w, int h, const char* name=0, const char * const * d=0) 
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

/*! \fn int Image::depth() const
  Return the depth of the image = number of bytes per pixel.
*/
/*! \fn int Image::line_size() const
  Size in bytes of a horiz. line,  generally equals (but not always) = w() * d() 
*/

/*! \fn int Image::count() const
  Number of raster images data inside this Image (generally 1)
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
  Returns true if make_current() has been called. On some platforms this will
  return false if the platform is incabable of compositing the stored
  data without update() being called again.
*/

/*! \fn void Image::make_current();

  Make all the drawing functions (see \link draw.h <fltk/draw.h>
  \endlink) draw into the offscreen image.  Or so we wish. On all
  current platforms the only call that works is a single
  fltk::drawimage() with the rectangle set to 0,0,w(),h().

  To draw into an Image while you are drawing a widget (or drawing
  another Image) you must use a GSave object to save the state. See
  update() for sample code.
*/

/*! \fn void Image::update();

  Subclasses must implement this function to draw the offscreen image.
  It will be called if drawn() is false.

  Sample implementation:
\code
  void MyImage::update() {
    set_size(figure_out_width(),figure_out_height());
    GSave gsave;
    make_current();
    drawimage(imagebuffer(), RGB, Rectangle(0,0,w(),h()));
  }
\endcode

  If you don't call make_current(), then nothing will be drawn. This
  is useful if you encounter an error, such as a non-existent image
  file.
*/

/**
  By default Image assumes the constructor set the w_ and h_
  fields, and returns them.

  For many subclasses (such as ones that read a file!) you certainly
  want to defer this calculation until first use. The way to do this
  is to put zero into the w_ and h_ in the constructor, and override this
  method with your own which calculates the values and sets them if
  it has not done so already. Typically this is done by calling update()
  on the assumption that the image will be needed soon.
*/
void Image::_measure(int& W, int& H) const { W=w(); H=h(); }

/*! \fn bool Image::fills_rectangle() const
  Peeks into the internal data and returns true if it knows that
  the alpha is solid 1 everywhere in the rectangle.
*/

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
  This does measure() and then draw(Rectangle(x,y,w(),h()). Thus it
  draws it without any scaling and with the top-left corner at the x,y
  position.
*/
void Image::draw(int x, int y) const {
  int w,h; measure(w,h);
  draw(Rectangle(x,y,w,h));
}

/*! \fn void Image::over(const Rectangle& from, const Rectangle& to) const

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
  Virtual method from Symbol baseclass, calls over() to resize
  the source image to fill the destination rectangle.

  If you override inset() then this will call inset() for the
  original and the output rectangle. The source and destination
  are diced into 9 rectangles in a 3x3 grid by the insets, and
  each piece is scaled individually. This is very useful for
  scaling paintings of buttons.

  It is possible this will use drawflags(INACTIVE_R) to gray out
  the image is a system-specific way. NYI.
*/
void Image::_draw(const fltk::Rectangle& r) const
{
  if (r.empty()) return;
  if (!(flags&DRAWN)) const_cast<Image*>(this)->update(); // get the size right
  // quickly handle no-scaling:
  if (r.w()==w_ && r.h()==h_) {
  NOINSETS:
    over(fltk::Rectangle(0,0,w_,h_), r);
    return;
  }
  // now check the insets and use them to scale pieces individually:
  fltk::Rectangle in(0,0,w_,h_); this->inset(in);
  if (!in.x() && !in.y() && in.w()==w_ && in.h()==h_) goto NOINSETS;
  int fx[4]; fx[0]=0; fx[1]=in.x(); fx[2]=in.r(); fx[3]=w_;
  int fy[4]; fy[0]=0; fy[1]=in.y(); fy[2]=in.b(); fy[3]=h_;
  fltk::Rectangle out(r); this->inset(out);
  int tx[4]; tx[0]=r.x(); tx[1]=out.x(); tx[2]=out.r(); tx[3]=r.r();
  int ty[4]; ty[0]=r.y(); ty[1]=out.y(); ty[2]=out.b(); ty[3]=r.b();
  for (int y=0; y<3; y++) if (fy[y+1]>fy[y] && ty[y+1]>ty[y]) {
    for (int x=0; x<3; x++) if (fx[x+1]>fx[x] && tx[x+1]>tx[x]) {
      over(fltk::Rectangle(fx[x],fy[y],fx[x+1]-fx[x],fy[y+1]-fy[y]),
	   fltk::Rectangle(tx[x],ty[y],tx[x+1]-tx[x],ty[y+1]-ty[y]));
    }
  }
}

// Image Allocation / DeAllocation
//! alloc data, sets the pixel type,  and make owned data destroyed automatically
uchar * Image::alloc_pixels(int w,int h, PixelType p=UNDEFINED) { 
    int size= (p!= UNDEFINED) ? (w*h*fltk::depth(p)) : 
	p_ !=UNDEFINED ? (w*h*fltk::depth(p_)) : 0 ;
    if (!size) return 0;
    dealloc_data();
    owned_ = true;
    count_=1; // force the buffer to be a pixels buffer
    nb_data_ = size;
    data_ = (const char**) new uchar[size] ;
    // we dont need to 0 the buffer for pixels buffer
    return (uchar*) data_;
}

//! alloc data, sets the pixel type,  and make owned data destroyed automatically
const char ** Image::alloc_data(int  size ) { // alloc data, sets the pixel type,  and will destroy owned data
    dealloc_data();
    owned_ = true;
    count_=0; // force the buffer to be a data buffer
    nb_data_ = size /(sizeof(const char *));
    data_ = (const char**) new uchar[size] ;
    memset((const char**) data_,0,size);   // 0 the buffer so we can delete the allocated lines 
			    // later in a safer way
    return (const char**) data_;
}

//! dealloc potentially owned data, harmless if called more than once
void Image::dealloc_data() { 
    if (owned_ && data_ ) {
	if (count_==0 && nb_data_>0) {
	// data buffers need more care than pixels buffers
	    int i;
	    for (i=0; i<nb_data_; i++) delete [] const_cast<char*>(data_[i]);
	}
	delete [] ((uchar*)data_); // deletes data or pixels buffers
	if (colors_ && ncolors_>0) delete [] colors_; 
	colors_ = 0;
    }
    data_ = 0;
    nb_data_ = 0;
    owned_ = false;
    count_=0;
}

/** \fn void Image::destroy();
  Same as redraw() but it also deallocates as much memory as possible.
*/

/** \fn void Image::copy_data(); 
  Copy the pixels or data buffer if it is not owned, useful for destructive image manip,
  like color_average()
*/
void Image::copy_data() {
    if (!data() || owned_) return; // don't copy already owned data
    
    if (count_==0) { // pixmap data
	
	char	**new_data,	// New data array
		**new_row;	// Current row in image
	int	i,		// Looping var
		ncolors,	// Number of colors in image
		chars_per_pixel,// Characters per color
		chars_per_line;	// Characters per line 
	int W,H;

	// Figure out how many colors there are, and how big they are...
	sscanf(data()[0],"%d %d %d %d", &W, &H,&ncolors, &chars_per_pixel);
	if (w()==-1) {w(W); h(H);} // use this opportunity to measure on the fly this pixmap
	chars_per_line = chars_per_pixel * w() + 1;
	
	// Allocate memory for the new array...
	if (ncolors < 0) new_data = new char *[h() + 2];
	else new_data = new char *[h() + ncolors + 1];
	
	new_data[0] = new char[strlen(data()[0]) + 1];
	strcpy(new_data[0], data()[0]);
	
	// Copy colors...
	if (ncolors < 0) {
	    // Copy FLTK colormap values...
	    ncolors = -ncolors;
	    new_row = new_data + 1;
	    *new_row = new char[ncolors * 4];
	    memcpy(*new_row, data()[1], ncolors * 4);
	    ncolors = 1;
	    new_row ++;
	} else {
	    // Copy standard XPM colormap values...
	    for (i = 0, new_row = new_data + 1; i < ncolors; i ++, new_row ++) {
		*new_row = new char[strlen(data()[i + 1]) + 1];
		strcpy(*new_row, data()[i + 1]);
	    }
	}
	
	// Copy image data...
	for (i = 0; i < h(); i ++, new_row ++) {
	    *new_row = new char[chars_per_line];
	    memcpy(*new_row, data()[i + ncolors + 1], chars_per_line);
	}
	
	// Update pointers...
	data((const char **)new_data);
	nb_data_ =  h() + ncolors + 1;  
	owned_=true;
    }
    else if (count_>=1) {
	if (w()==-1) {int W=w(),H=h(); _measure(W,H);w(W); h(H);}
	size_t s = d()*h();
	uchar* newbuf = new uchar[s];
	memcpy(newbuf, pixels(), s);
	pixels(newbuf);
	nb_data_=(int)s;
	owned_=true;
    }

}

/** Generic algorithm to make all pixels of a buffer tend
    to a color c average with a blending coefficient i.<br>
    useful for desaturating an image if c is a grey color
*/
void Image::color_average(Color c, float i) {
    // Delete any existing pixmap/mask objects...
    if (!data()) 
	fetch();
    if (!data()) return; // no data() or pixels after fetch: 
		         // seems that no image is set his object yet
    copy_data(); // make sure we own the data
    
    uchar		r, g, b;
    unsigned	ia, ir, ig, ib;
    fltk::split_color(c, r, g, b);
    if (i < 0.0f) i = 0.0f;
    else if (i > 1.0f) i = 1.0f;
    // Get the color to blend with...
    ia = (unsigned)(256 * i);
    ir = r * (256 - ia);
    ig = g * (256 - ia);
    ib = b * (256 - ia);

    if (count_==0 ) {// pixmap data
	// Update the colormap to do the blend...
	char		line[255];	// New colormap line
	int		color,		// Looping var
	    ncolors,	// Number of colors in image
	    chars_per_pixel;// Characters per color
	sscanf(data()[0],"%*d%*d%d%d", &ncolors, &chars_per_pixel);
	
	if (ncolors < 0) {
	    // Update FLTK colormap...
	    ncolors = -ncolors;
	    uchar *cmap = (uchar *)(data()[1]);
	    for (color = 0; color < ncolors; color ++, cmap += 4) {
		cmap[1] = (ia * cmap[1] + ir) >> 8;
		cmap[2] = (ia * cmap[2] + ig) >> 8;
		cmap[3] = (ia * cmap[3] + ib) >> 8;
	    }
	} else {
	    // Update standard XPM colormap...
	    for (color = 0; color < ncolors; color ++) {
		// look for "c word", or last word if none:
		const char *p = data()[color + 1] + chars_per_pixel + 1;
		const char *previous_word = p;
		for (;;) {
		    while (*p && isspace(*p)) p++;
		    char what = *p++;
		    while (*p && !isspace(*p)) p++;
		    while (*p && isspace(*p)) p++;
		    if (!*p) {p = previous_word; break;}
		    if (what == 'c') break;
		    previous_word = p;
		    while (*p && !isspace(*p)) p++;
		}

                Color c = fltk::color(p);
		if (c) {
                  split_color(c, r,g,b);
		    r = (ia * r + ir) >> 8;
		    g = (ia * g + ig) >> 8;
		    b = (ia * b + ib) >> 8;
		    
		    if (chars_per_pixel > 1) sprintf(line, "%c%c c #%02X%02X%02X",
			data()[color + 1][0],
			data()[color + 1][1], r, g, b);
		    else sprintf(line, "%c c #%02X%02X%02X", data()[color + 1][0], r, g, b);
		    
		    delete[] (char *)data()[color + 1];
		    ((char **)data())[color + 1] = new char[strlen(line) + 1];
		    strcpy((char *)data()[color + 1], line);
		}
	    }
	}
    } 
    else if (count_>=1) { // pixels data
	int y, x,l=ld();
	uchar * pixBuffer = (uchar*) pixels();
		
	switch (pixel_type()) {
	case ARGB32: //aarrggbb
	case RGB32:  
	    for (y=0; y<h_; y++) { // for each image row
		pixBuffer = ((uchar*)pixels())+y*l;
		for(x=0; x<l; x+=4) {
		    unsigned * org = ((unsigned *) (&pixBuffer[x]));
		    unsigned trans = 
			(pixBuffer[x+3] <<24)	+ 
			(((ia * pixBuffer[x+2] + ir)>>8) <<16) +
			(((ia * pixBuffer[x+1] + ig)>>8) <<8) +
			((ia * pixBuffer[x+0] + ib)>>8);
		      *org= trans;

		}
	    }
	    break;    
	case RGB:
	    for (y=0; y<h_; y++) { // for each image row
		pixBuffer = ((uchar*)pixels())+y*l;
		for(x=0; x<l; x+=3) {
		    pixBuffer[x]  = (ia * pixBuffer[x]     + ir) >>8;
		    pixBuffer[x+1]= (ia * pixBuffer[x+1]   + ig) >>8;
		    pixBuffer[x+2]= (ia * pixBuffer[x+2]   + ib) >>8;
		}
	    }
	    break;
	default:
	    // TODO : MONO
	    break;
	}
    }
}
//! Common constructors init here as too many variables are affected to make this inlined
void Image::init(int w, int h, const char * const * d) {
    picture=0;
    flags= 0;
    w_= w; h_ = h;
    p_=UNDEFINED;
    ld_= count_=0;
    data_=d;
    nb_data_=0;
    owned_=0;
    colors_=0;
    transp_index_ = -1; // means no transparency
}
//
// End of "$Id$".
//
