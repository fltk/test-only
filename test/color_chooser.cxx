//
// "$Id$"
//
// Color chooser test program for the Fast Light Tool Kit (fltk).
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
#undef USE_XFT // this breaks the <x.h> header file
#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/visual.h>
#include <fltk/Button.h>
#include <fltk/show_colormap.h>
#include <fltk/ColorChooser.h>
#include <fltk/rgbImage.h>
#include <fltk/x.h>
#include <fltk/error.h>
#include <fltk/draw.h>
#include <stdlib.h>
#include <stdio.h>

#if USE_X11
#include "list_visuals.cxx"
#endif

int width = 75;
int height = 75;
uchar *image;

void make_image() {
  image = new uchar[3*width*height];
  uchar *p = image;
  for (int y = 0; y < height; y++) {
    double Y = double(y)/(height-1);
    for (int x = 0; x < width; x++) {
      double X = double(x)/(width-1);
      *p++ = uchar(255*((1-X)*(1-Y))); // red in upper-left
      *p++ = uchar(255*((1-X)*Y));	// green in lower-left
      *p++ = uchar(255*(X*Y));	// blue in lower-right
    }
  }
}

class Pens : public fltk::Widget {
  void draw();
public:
  Pens(int X, int Y, int W, int H, const char* L)
    : fltk::Widget(X,Y,W,H,L) {}
};
void Pens::draw() {
  // use every color in the gray ramp:
  for (int i = 0; i < 3*8; i++) {
    fltk::setcolor((fltk::Color)(fltk::GRAY00+i));
    fltk::drawline(i, 0, i, h());
  }
}

fltk::Color c = fltk::GRAY75;
#define fullcolor_cell ((fltk::Color)16)

void cb1(fltk::Widget *, void *v) {
  c = fltk::show_colormap(c);
  fltk::Widget* b = (fltk::Widget*)v;
  b->color(c);
  b->parent()->redraw();
}

void cb2(fltk::Widget *, void *v) {
  uchar r,g,b;
//  Fl::get_color(c,r,g,b);
  fltk::split_color(c,r,g,b);
  if (!fltk::color_chooser("New color:",r,g,b)) return;
  c = fullcolor_cell;
  fltk::set_color_index(fullcolor_cell, fltk::color(r,g,b));
  fltk::Widget* bx = (fltk::Widget*)v;
  bx->color(fullcolor_cell);
  bx->parent()->redraw();
}

int main(int argc, char ** argv) {
  fltk::set_color_index(fullcolor_cell, fltk::color(145,159,170));
  fltk::Window window(400,400);
  window.begin();
  fltk::Widget box(50,50,300,300);
  box.box(fltk::THIN_DOWN_BOX);
  c = fullcolor_cell;
  box.color(c);
  fltk::Button b1(140,120,120,30,"fltk::show_colormap");
  b1.callback(cb1,&box);
  fltk::Button b2(140,160,120,30,"fltk::choose_color");
  b2.callback(cb2,&box);
  fltk::Widget image_box(140,200,120,120,0);
  make_image();
  (new fltk::rgbImage(image, width, height))->label(&image_box);
  fltk::Widget b(140,320,120,0,"Example of fltk::draw_image()");
  Pens p(80,200,3*8,120,"lines");
  p.set_flag(fltk::ALIGN_TOP);
  int i = 1;
  if (!fltk::args(argc,argv,i) || i != argc-1) {
    printf("usage: %s <switches> visual-number\n"
	   " - : default visual\n"
	   " r : call fltk::visual(fltk::RGB)\n"
	   " c : call fltk::own_colormap()\n",argv[0]);
#if USE_X11
    printf(" # : use this visual with an empty colormap:\n");
    list_visuals();
#endif
    puts(fltk::help);
    exit(1);
  }
  if (argv[i][0] == 'r') {
    if (!fltk::visual(fltk::RGB_COLOR)) printf("fltk::visual(fltk::RGB_COLOR) returned false.\n");
  } else if (argv[i][0] == 'c') {
    fltk::own_colormap();
  } else if (argv[i][0] != '-') {
#if USE_X11
    int visid = atoi(argv[i]);
    fltk::open_display();
    XVisualInfo templt; int num;
    templt.visualid = visid;
    fltk::xvisual= XGetVisualInfo(fltk::xdisplay, VisualIDMask, &templt, &num);
    if (!fltk::xvisual) fltk::fatal("No visual with id %d",visid);
    fltk::xcolormap = XCreateColormap(fltk::xdisplay, RootWindow(fltk::xdisplay,fltk::xscreen),
				  fltk::xvisual->visual, AllocNone);
    fltk::xpixel(fltk::BLACK); // make sure black is allocated
#else
    fltk::fatal("Visual id's not supported on this platform");
#endif
  }
  window.show(argc,argv);
  return fltk::run();
}

//
// End of "$Id$".
//
