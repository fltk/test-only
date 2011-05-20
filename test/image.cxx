//
// "$Id$"
//
// fltk3::Image test program for the Fast Light Tool Kit (FLTK).
//
// Notice that fltk3::Image is for a static, multiple-reuse image, such
// as an icon or postage stamp.  Use fltk3::draw_image to go directly
// from an buffered image that changes often.
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

#include <fltk3/run.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Button.h>
#include <fltk3/Image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int width = 100;
int height = 100;
uchar *image;

void make_image() {
  image = new uchar[4*width*height];
  uchar *p = image;
  for (int y = 0; y < height; y++) {
    double Y = double(y)/(height-1);
    for (int x = 0; x < width; x++) {
      double X = double(x)/(width-1);
      *p++ = uchar(255*((1-X)*(1-Y))); // red in upper-left
      *p++ = uchar(255*((1-X)*Y));	// green in lower-left
      *p++ = uchar(255*(X*Y));	// blue in lower-right
      X -= 0.5;
      Y -= 0.5;
      int alpha = (int)(255 * sqrt(X * X + Y * Y));
      if (alpha < 255) *p++ = uchar(alpha);	// alpha transparency
      else *p++ = 255;
      Y += 0.5;
    }
  }
}

#include <fltk3/ToggleButton.h>

fltk3::ToggleButton *leftb,*rightb,*topb,*bottomb,*insideb,*overb,*inactb;
fltk3::Button *b;
fltk3::DoubleWindow *w;

void button_cb(fltk3::Widget *,void *) {
  int i = 0;
  if (leftb->value()) i |= fltk3::ALIGN_LEFT;
  if (rightb->value()) i |= fltk3::ALIGN_RIGHT;
  if (topb->value()) i |= fltk3::ALIGN_TOP;
  if (bottomb->value()) i |= fltk3::ALIGN_BOTTOM;
  if (insideb->value()) i |= fltk3::ALIGN_INSIDE;
  if (overb->value()) i |= fltk3::ALIGN_TEXT_OVER_IMAGE;
  b->align(i);
  if (inactb->value()) b->deactivate();
  else b->activate();
  w->redraw();
}

#include <fltk3/x.h>
#if !defined(WIN32) && !defined(__APPLE__)
#include "list_visuals.cxx"
#endif

int visid = -1;
int arg(int argc, char **argv, int &i) {
  if (argv[i][1] == 'v') {
    if (i+1 >= argc) return 0;
    visid = atoi(argv[i+1]);
    i += 2;
    return 2;
  }
  return 0;
}

int main(int argc, char **argv) {
#if !defined(WIN32) && !defined(__APPLE__)
  int i = 1;

  fltk3::args(argc,argv,i,arg);

  if (visid >= 0) {
    fltk3::open_display();
    XVisualInfo templt; int num;
    templt.visualid = visid;
    fltk3::visual = XGetVisualInfo(fltk3::display, VisualIDMask, &templt, &num);
    if (!fltk3::visual) {
      fprintf(stderr, "No visual with id %d, use one of:\n",visid);
      list_visuals();
      exit(1);
    }
    fltk3::colormap = XCreateColormap(fltk3::display, RootWindow(fltk3::display,fltk3::screen),
				fltk3::visual->visual, AllocNone);
    fltk3::xpixel(fltk3::BLACK); // make sure black is allocated in overlay visuals
  } else {
    fltk3::visual(fltk3::RGB);
  }
#endif

  fltk3::DoubleWindow window(400,400); ::w = &window;
  window.color(fltk3::WHITE);
  fltk3::Button b(140,160,120,120,"Image w/Alpha"); ::b = &b;

  fltk3::RGBImage *rgb;
  fltk3::Image *dergb;

  make_image();
  rgb = new fltk3::RGBImage(image, width, height,4);
  dergb = rgb->copy();
  dergb->inactive();

  b.image(rgb);
  b.deimage(dergb);

  leftb = new fltk3::ToggleButton(25,50,50,25,"left");
  leftb->callback(button_cb);
  rightb = new fltk3::ToggleButton(75,50,50,25,"right");
  rightb->callback(button_cb);
  topb = new fltk3::ToggleButton(125,50,50,25,"top");
  topb->callback(button_cb);
  bottomb = new fltk3::ToggleButton(175,50,50,25,"bottom");
  bottomb->callback(button_cb);
  insideb = new fltk3::ToggleButton(225,50,50,25,"inside");
  insideb->callback(button_cb);
  overb = new fltk3::ToggleButton(25,75,100,25,"text over");
  overb->callback(button_cb);
  inactb = new fltk3::ToggleButton(125,75,100,25,"inactive");
  inactb->callback(button_cb);
  window.resizable(window);
  window.end();
  window.show(argc, argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
