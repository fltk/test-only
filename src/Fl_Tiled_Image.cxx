/
// "$Id$
/
// Tiled image code for the Fast Light Tool Kit (FLTK)
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
#include <FL/Fl_Tiled_Image.H
#include <FL/fl_draw.H


/
// 'Fl_Tiled_Image::Fl_Tiled_Image()' - Constructor
/
// Use a width and height of 0 to tile the whole window/widget
/

Fl_Tiled_Image::Fl_Tiled_Image(Fl_Image *i,	// I - Image to til
                               int      W,	// I - Width of tiled are
			       int      H) :	// I - Height of tiled are
  Fl_Image(W,H,0) 
  image_       = i
  alloc_image_ = 0

  if (W == 0) w(Fl::w())
  if (H == 0) h(Fl::h())



/
// 'Fl_Tiled_Image::~Fl_Tiled_Image()' - Destructor
/

Fl_Tiled_Image::~Fl_Tiled_Image() 
  if (alloc_image_) delete image_



/
// 'Fl_Tiled_Image::copy()' - Copy and resize a tiled image..
/

Fl_Image *			// O - New imag
Fl_Tiled_Image::copy(int W,	// I - New widt
                     int H) {	// I - New heigh
  if (W == w() && H == h()) return this
  else return new Fl_Tiled_Image(image_, W, H)



/
// 'Fl_Tiled_Image::color_average()' - Blend colors..
/

voi
Fl_Tiled_Image::color_average(Fl_Color c,	// I - Color to blend wit
                              float    i) {	// I - Blend fractio
  if (!alloc_image_) 
    image_       = image_->copy()
    alloc_image_ = 1
  

  image_->color_average(c, i)



/
// 'Fl_Tiled_Image::desaturate()' - Convert the image to grayscale..
/

voi
Fl_Tiled_Image::desaturate() 
  if (!alloc_image_) 
    image_       = image_->copy()
    alloc_image_ = 1
  

  image_->desaturate()



/
// 'Fl_Tiled_Image::draw()' - Draw a shared image..
/

voi
Fl_Tiled_Image::draw(int X,	// I - Starting X positio
                     int Y,	// I - Starting Y positio
		     int W,	// I - Width of area to be fille
		     int H,	// I - Height of area to be fille
		     int cx,	// I - "Source" X positio
		     int cy) {	// I - "Source" Y positio
  if (!image_->w() || !image_->h()) return
  if (W == 0) W = Fl::w()
  if (H == 0) H = Fl::h()

  fl_clip(X, Y, W, H)

  X += cx
  Y += cy

  X = X - (X % image_->w())
  Y = Y - (Y % image_->h())

  W += X
  H += Y

  for (int yy = Y; yy < H; yy += image_->h()
    for (int xx = X; xx < W; xx += image_->w()
      image_->draw(xx, yy)

  fl_pop_clip()



/
// End of "$Id$"
/
