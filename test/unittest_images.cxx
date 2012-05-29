//
// "$Id$"
//
// Unit tests for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

#include "unittests.h"

#include <fltk3/Box.h>
#include <fltk3/draw.h>

// Note: currently (March 2010) fltk3::draw_image() supports transparency with
//	 alpha channel only on Apple (Mac OS X), but fltk3::RGBImage->draw()
//	 supports transparency on all platforms !

//
//------- test the image drawing capabilities of this implementation ----------
//

// Some parameters for fine tuning for developers - their
// default values ought to be: CB=1, DX=0, LX=0, IMG=1
#define CB (1)  // 1 to show the checker board background for alpha images, 0 otherwise
#define DX (0)	// additional (undefined (0)) pixels per line, must be >= 0
		// ignored (irrelevant), if LX == 0 (see below)
#define LX (0)	// 0 for default: ld() = 0, i.e. ld() defaults (internally) to w()*d()
		// +1: ld() = (w() + DX) * d()
		// -1 to flip image vertically: ld() = - ((w() + DX) * d())
#define IMG (1)	// 1 to use fltk3::RGBImage for drawing images,
		// 0 to use fltk3::draw_image() instead.
		// Note: as of April 2011, only 1 (fltk3::RGBImage) works correctly with alpha
		// channel, 0 (fltk3::draw_image()) ignores the alpha channel (FLTK 1.3.0).
		// There are plans to support alpha in fltk3::draw_image() in FLTK 1.3.x,
		// but not in FLTK 1.1.x .

class ImageTest : public fltk3::Widget {
public: 
  static fltk3::Widget *create() {
    int x, y;
    uchar *dg, *dga, *drgb, *drgba;
    dg    = img_gray   = (uchar*)malloc((128+DX)*128*1);
    dga   = img_gray_a = (uchar*)malloc((128+DX)*128*2);
    drgb  = img_rgb    = (uchar*)malloc((128+DX)*128*3);
    drgba = img_rgba   = (uchar*)malloc((128+DX)*128*4);
    for (y=0; y<128; y++) {
      for (x=0; x<128; x++) {
        *drgba++ = *drgb++ = *dga++ = *dg++ = y<<1;
        *drgba++ = *drgb++                  = x<<1;
        *drgba++ = *drgb++                  = (127-x)<<1;
        *drgba++           = *dga++         = x+y;
      }
      if (DX > 0 && LX != 0) {
        memset(dg,   0,1*DX); dg    += 1*DX;
        memset(dga,  0,2*DX); dga   += 2*DX;
        memset(drgb, 0,3*DX); drgb  += 3*DX;
        memset(drgba,0,4*DX); drgba += 4*DX;
      }
    }
    if (LX<0) {
      img_gray   += 127*(128+DX);
      img_gray_a += 127*(128+DX)*2;
      img_rgb    += 127*(128+DX)*3;
      img_rgba   += 127*(128+DX)*4;
    }
    i_g    = new fltk3::RGBImage (img_gray  ,128,128,1,LX*(128+DX));
    i_ga   = new fltk3::RGBImage (img_gray_a,128,128,2,LX*(128+DX)*2);
    i_rgb  = new fltk3::RGBImage (img_rgb,   128,128,3,LX*(128+DX)*3);
    i_rgba = new fltk3::RGBImage (img_rgba,  128,128,4,LX*(128+DX)*4);
    return new ImageTest(TESTAREA_X, TESTAREA_Y, TESTAREA_W, TESTAREA_H);
  }
  static uchar *img_gray;
  static uchar *img_gray_a;
  static uchar *img_rgb;
  static uchar *img_rgba;
  static fltk3::RGBImage *i_g;
  static fltk3::RGBImage *i_ga;
  static fltk3::RGBImage *i_rgb;
  static fltk3::RGBImage *i_rgba;
  ImageTest(int x, int y, int w, int h) : fltk3::Widget(x, y, w, h) {
    label("Testing Image Drawing\n\n"
	"This test renders four images, two of them with a checker board\n"
	"visible through the graphics. Color and gray gradients should be\n"
	"visible. This does not test any image formats such as JPEG.");
    align(fltk3::ALIGN_INSIDE|fltk3::ALIGN_BOTTOM|fltk3::ALIGN_LEFT|fltk3::ALIGN_WRAP);
    box(fltk3::BORDER_BOX);
  }
  void draw() {
    fltk3::Widget::draw();

    // top left: RGB

    int xx = 10, yy = 10;
    fltk3::color(fltk3::BLACK); fltk3::rect(xx, yy, 130, 130);
#if IMG
    i_rgb->draw(xx+1,yy+1);
#else
    fltk3::draw_image(img_rgb, xx+1, yy+1, 128, 128, 3, LX*((128+DX)*3));
#endif
    fltk3::draw("RGB", xx+134, yy+64);

    // bottom left: RGBA

    xx = 10; yy = 10+134;
    fltk3::color(fltk3::BLACK); fltk3::rect(xx, yy, 130, 130);	// black frame
    fltk3::color(fltk3::WHITE); fltk3::rectf(xx+1, yy+1, 128, 128);	// white background
#if CB							// checker board
    fltk3::color(fltk3::BLACK); fltk3::rectf(xx+65, yy+1, 64, 64);
    fltk3::color(fltk3::BLACK); fltk3::rectf(xx+1, yy+65, 64, 64);
#endif
#if IMG
    i_rgba->draw(xx+1,yy+1);
#else
    fltk3::draw_image(img_rgba, xx+1, yy+1, 128, 128, 4, LX*((128+DX)*4));
#endif
    fltk3::color(fltk3::BLACK); fltk3::draw("RGBA", xx+134, yy+64);

    // top right: Gray

    xx = 10+200; yy = 10;
    fltk3::color(fltk3::BLACK); fltk3::rect(xx, yy, 130, 130);
#if IMG
    i_g->draw(xx+1,yy+1);
#else
    fltk3::draw_image(img_gray, xx+1, yy+1, 128, 128, 1, LX*((128+DX)*1));
#endif
    fltk3::draw("Gray", xx+134, yy+64);

    // bottom right: Gray+Alpha

    xx = 10+200; yy = 10+134;
    fltk3::color(fltk3::BLACK); fltk3::rect(xx, yy, 130, 130);	// black frame
    fltk3::color(fltk3::WHITE); fltk3::rectf(xx+1, yy+1, 128, 128);	// white background
#if CB							// checker board
    fltk3::color(fltk3::BLACK); fltk3::rectf(xx+65, yy+1, 64, 64);
    fltk3::color(fltk3::BLACK); fltk3::rectf(xx+1, yy+65, 64, 64);
#endif
#if IMG
    i_ga->draw(xx+1,yy+1);
#else
    fltk3::draw_image(img_gray_a, xx+1, yy+1, 128, 128, 2, LX*((128+DX)*2));
#endif
    fltk3::color(fltk3::BLACK); fltk3::draw("Gray+Alpha", xx+134, yy+64);
  }
};

uchar *ImageTest::img_gray = 0;
uchar *ImageTest::img_gray_a = 0;
uchar *ImageTest::img_rgb = 0;
uchar *ImageTest::img_rgba = 0;
fltk3::RGBImage *ImageTest::i_g = 0;
fltk3::RGBImage *ImageTest::i_ga = 0;
fltk3::RGBImage *ImageTest::i_rgb = 0;
fltk3::RGBImage *ImageTest::i_rgba = 0;

UnitTest images("drawing images", ImageTest::create);

//
// End of "$Id$"
//
