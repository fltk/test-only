/
// "$Id$
/
// Image drawing code for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#include <FL/Fl.H
#include <FL/fl_draw.H
#include <FL/x.H
#include <FL/Fl_Widget.H
#include <FL/Fl_Menu_Item.H
#include <FL/Fl_Image.H
#include "flstring.h
#include <FL/Fl_Device.H


// void fl_restore_clip(); // from fl_rect.cx

/
// Base image class..
/

 
Fl_Image_Cache::~Fl_Image_Cache()
  if(next
    next->prev = prev
  if(prev
     prev->next = next; //removing from chain
  else // is firs
     device->image_caches = next
 image->cache_ = 0
}


Fl_Image_Cache::Fl_Image_Cache(Fl_Image * im, Fl_Device * dev)
  :prev(0), image(im), device(dev)
  im->cache_ = this
  // inserting cache in front of device cache lis
  if((next = dev->image_caches)
    dev->image_caches->prev = this
  dev->image_caches = this;
}



Fl_Image::~Fl_Image() 
  uncache()


void Fl_Image::draw(int XP, int YP, int, int, int, int) 
  draw_empty(XP, YP)


void Fl_Image::draw_empty(int X, int Y) 
  if (w() > 0 && h() > 0) 
    fl_color(FL_BLACK)
    fl_rect(X, Y, w(), h())
    fl_line(X, Y, X + w() - 1, Y + h() - 1)
    fl_line(X, Y + h() - 1, X + w() - 1, Y)
  


Fl_Image *Fl_Image::copy(int W, int H) 
  return new Fl_Image(W, H, d())


void Fl_Image::color_average(Fl_Color, float) 


void Fl_Image::desaturate() 


void Fl_Image::label(Fl_Widget* widget) 
  widget->image(this)


void Fl_Image::label(Fl_Menu_Item* m) 
  Fl::set_labeltype(_FL_IMAGE_LABEL, labeltype, measure)
  m->label(_FL_IMAGE_LABEL, (const char*)this)


voi
Fl_Image::labeltype(const Fl_Label *lo,		// I - Labe
                    int            lx,		// I - X positio
		    int            ly,		// I - Y positio
		    int            lw,		// I - Width of labe
		    int            lh,		// I - Height of labe
		    Fl_Align       la) {	// I - Alignmen
  Fl_Image	*img;				// Image pointe
  int		cx, cy;				// Image positio

  img = (Fl_Image *)(lo->value)

  if (la & FL_ALIGN_LEFT) cx = 0
  else if (la & FL_ALIGN_RIGHT) cx = img->w() - lw
  else cx = (img->w() - lw) / 2

  if (la & FL_ALIGN_TOP) cy = 0
  else if (la & FL_ALIGN_BOTTOM) cy = img->h() - lh
  else cy = (img->h() - lh) / 2

  fl_color((Fl_Color)lo->color)

  img->draw(lx, ly, lw, lh, cx, cy)


voi
Fl_Image::measure(const Fl_Label *lo,		// I - Labe
                  int            &lw,		// O - Width of imag
		  int            &lh) {		// O - Height of imag
  Fl_Image *img;				// Image pointe

  img = (Fl_Image *)(lo->value)

  lw = img->w()
  lh = img->h()



/
// RGB image class..
/


Fl_RGB_Image::~Fl_RGB_Image() 
  // uncache(); // RK: implemented in base clas
  if (alloc_array) delete[] (uchar *)array






Fl_Image *Fl_RGB_Image::copy(int W, int H) 
   Fl_RGB_Image  *new_image;     // New RGB imag
   uchar         *new_array;     // New array for image dat
  	
  // Optimize the simple copy where the width and height are the same
  // or when we are copying an empty image..
  if ((W == w() && H == h()) |
      !w() || !h() || !d() || !array) 
	     if (array) 
  	     // Make a copy of the image data and return a new Fl_RGB_Image..
  	     new_array = new uchar[w() * h() * d()]
  	     memcpy(new_array, array, w() * h() * d())
  	     new_image = new Fl_RGB_Image(new_array, w(), h(), d(), ld())
  	     new_image->alloc_array = 1
  	     return new_image
  	   } else return new Fl_RGB_Image(array, w(), h(), d(), ld())
  
  if (W <= 0 || H <= 0) return 0

  // OK, need to resize the image data; allocate memory and
  uchar         *new_ptr;       // Pointer into new arra
  const uchar	*old_ptr;	// Pointer into old arra
  int		c,		// Channel numbe
		sy,		// Source coordinat
		dx, dy,		// Destination coordinate
		xerr, yerr,	// X & Y error
		xmod, ymod,	// X & Y modul
		xstep, ystep;	// X & Y step increment


  // Figure out Bresenheim step/modulus values..
  xmod   = w() % W
  xstep  = (w() / W) * d()
  ymod   = h() % H
  ystep  = h() / H

  // Allocate memory for the new image..
  new_array = new uchar [W * H * d()]
  new_image = new Fl_RGB_Image(new_array, W, H, d())
  new_image->alloc_array = 1

  // Scale the image using a nearest-neighbor algorithm..
  for (dy = H, sy = 0, yerr = H, new_ptr = new_array; dy > 0; dy --) 
    for (dx = W, xerr = W, old_ptr = array + sy * (w() * d() + ld())
	 dx > 0
	 dx --) 
      for (c = 0; c < d(); c ++) *new_ptr++ = old_ptr[c]

      old_ptr += xstep
      xerr    -= xmod

      if (xerr <= 0) 
	xerr    += W
	old_ptr += d()
      
    

    sy   += ystep
    yerr -= ymod
    if (yerr <= 0) 
      yerr += H
      sy ++
    
  

  return new_image


void Fl_RGB_Image::color_average(Fl_Color c, float i) 
  // Don't average an empty image..
  if (!w() || !h() || !d() || !array) return

  // Delete any existing pixmap/mask objects..
  uncache()

  // Allocate memory as needed..
  uchar		*new_array
		*new_ptr

  if (!alloc_array) new_array = new uchar[h() * w() * d()]
  else new_array = (uchar *)array

  // Get the color to blend with..
  uchar		r, g, b
  unsigned	ia, ir, ig, ib

  Fl::get_color(c, r, g, b)
  if (i < 0.0f) i = 0.0f
  else if (i > 1.0f) i = 1.0f

  ia = (unsigned)(256 * i)
  ir = r * (256 - ia)
  ig = g * (256 - ia)
  ib = b * (256 - ia)

  // Update the image data to do the blend..
  const uchar	*old_ptr
  int		x, y

  if (d() < 3) 
    ig = (r * 31 + g * 61 + b * 8) / 100 * (256 - ia)

    for (new_ptr = new_array, old_ptr = array, y = 0; y < h(); y ++, old_ptr += ld()
      for (x = 0; x < w(); x ++) 
	*new_ptr++ = (*old_ptr++ * ia + ig) >> 8
	if (d() > 1) *new_ptr++ = *old_ptr++
      
  } else 
    for (new_ptr = new_array, old_ptr = array, y = 0; y < h(); y ++, old_ptr += ld()
      for (x = 0; x < w(); x ++) 
	*new_ptr++ = (*old_ptr++ * ia + ir) >> 8
	*new_ptr++ = (*old_ptr++ * ia + ig) >> 8
	*new_ptr++ = (*old_ptr++ * ia + ib) >> 8
	if (d() > 3) *new_ptr++ = *old_ptr++
      
  

  // Set the new pointers/values as needed..
  if (!alloc_array) 
    array       = new_array
    alloc_array = 1

    ld(0)
  


void Fl_RGB_Image::desaturate() 
  // Don't desaturate an empty image..
  if (!w() || !h() || !d() || !array) return

  // Can only desaturate color images..
  if (d() < 3) return

  // Delete any existing pixmap/mask objects..
  uncache()

  // Allocate memory for a grayscale image..
  uchar		*new_array
		*new_ptr
  int		new_d

  new_d     = d() - 2
  new_array = new uchar[h() * w() * new_d]

  // Copy the image data, converting to grayscale..
  const uchar	*old_ptr
  int		x, y

  for (new_ptr = new_array, old_ptr = array, y = 0; y < h(); y ++, old_ptr += ld()
    for (x = 0; x < w(); x ++, old_ptr += d()) 
      *new_ptr++ = (uchar)((31 * old_ptr[0] + 61 * old_ptr[1] + 8 * old_ptr[2]) / 100)
      if (d() > 3) *new_ptr++ = old_ptr[3]
    

  // Free the old array as needed, and then set the new pointers/values..
  if (alloc_array) delete[] (uchar *)array

  array       = new_array
  alloc_array = 1

  ld(0)
  d(new_d)




void Fl_RGB_Image::draw(int XP, int YP, int WP, int HP, int cx, int cy) 

  // Device independent par
  // Don't draw an empty image..
  if (!d() || !array) 
    draw_empty(XP, YP)
    return
  

  // account for current clip region (faster on Irix)
  int X,Y,W,H; fl_clip_box(XP,YP,WP,HP,X,Y,W,H)
  cx += X-XP; cy += Y-YP
  // clip the box down to the size of image, quit if empty
  if (cx < 0) {W += cx; X -= cx; cx = 0;
  if (cx+W > w()) W = w()-cx
  if (W <= 0) return
  if (cy < 0) {H += cy; Y -= cy; cy = 0;
  if (cy+H > h()) H = h()-cy
  if (H <= 0) return
 
  // RK: draw to Devic
  fl_device->draw(this, X, Y, W, H, cx, cy)
 



void Fl_RGB_Image::label(Fl_Widget* widget) 
  widget->image(this)


void Fl_RGB_Image::label(Fl_Menu_Item* m) 
  Fl::set_labeltype(_FL_IMAGE_LABEL, labeltype, measure)
  m->label(_FL_IMAGE_LABEL, (const char*)this)


#ifdef __APPLE__ // RK: HACK different devices should be implemented as a seperate libray and using modified makefile
  #include "carbon/Image.cxx
#elif defined(WIN32
  #include "win/Image.cxx
#els
  #include "xlib/Image.cxx
#endif  // __APPLE_



/
// End of "$Id$"
/
