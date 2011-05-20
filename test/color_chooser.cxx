//
// "$Id$"
//
// Color chooser test program for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/Window.h>
#include <fltk3/Box.h>
#include <fltk3/Button.h>
#include <fltk3/show_colormap.h>
#include <fltk3/ColorChooser.h>
#include <fltk3/Image.h>
#include <fltk3/x.h>
#include <fltk3/draw.h>

#include <stdlib.h>
#include <stdio.h>
#if !defined(WIN32) && !defined(__APPLE__)
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

class Pens : public fltk3::Box {
  void draw();
public:
  Pens(int X, int Y, int W, int H, const char* L)
  : fltk3::Box(X,Y,W,H,L) {}
};
void Pens::draw() {
  // use every color in the gray ramp:
  for (int i = 0; i < 3*8; i++) {
    fltk3::color((fltk3::Color)(fltk3::GRAY_RAMP+i));
    fltk3::line(x()+i, y(), x()+i, y()+h());
  }
}

fltk3::Color c = fltk3::GRAY;
#define fullcolor_cell (fltk3::FREE_COLOR)

void cb1(fltk3::Widget *, void *v) {
  c = fltk3::show_colormap(c);
  fltk3::Box* b = (fltk3::Box*)v;
  b->color(c);
  b->parent()->redraw();
}

void cb2(fltk3::Widget *, void *v) {
  uchar r,g,b;
  fltk3::get_color(c,r,g,b);
  if (!fltk3::color_chooser("New color:",r,g,b,3)) return;
  c = fullcolor_cell;
  fltk3::set_color(fullcolor_cell,r,g,b);
  fltk3::Box* bx = (fltk3::Box*)v;
  bx->color(fullcolor_cell);
  bx->parent()->redraw();
}

int main(int argc, char ** argv) {
  fltk3::set_color(fullcolor_cell,145,159,170);
  fltk3::Window window(400,400);
  fltk3::Box box(50,50,300,300);
  box.box(fltk3::THIN_DOWN_BOX);
  c = fullcolor_cell;
  box.color(c);
  fltk3::Button b1(140,120,120,30,"fltk3::show_colormap");
  b1.callback(cb1,&box);
  fltk3::Button b2(140,160,120,30,"fltk3::choose_color");
  b2.callback(cb2,&box);
  fltk3::Box image_box(140,200,120,120,0);
  make_image();
  (new fltk3::RGBImage(image, width, height))->label(&image_box);
  fltk3::Box b(140,320,120,0,"Example of fltk3::draw_image()");
  Pens p(80,200,3*8,120,"lines");
  p.align(fltk3::ALIGN_TOP);
  int i = 1;
  if (!fltk3::args(argc,argv,i) || i < argc-1) {
    printf("usage: %s <switches> visual-number\n"
           " - : default visual\n"
           " r : call fltk3::visual(fltk3::RGB)\n"
           " c : call fltk3::own_colormap()\n",argv[0]);
#if !defined(WIN32) && !defined(__APPLE__)
    printf(" # : use this visual with an empty colormap:\n");
    list_visuals();
#endif
    puts(fltk3::help);
    exit(1);
  }
  if (i!=argc) {
    if (argv[i][0] == 'r') {
      if (!fltk3::visual(fltk3::RGB)) printf("fltk3::visual(fltk3::RGB) returned false.\n");
    } else if (argv[i][0] == 'c') {
      fltk3::own_colormap();
    } else if (argv[i][0] != '-') {
#if !defined(WIN32) && !defined(__APPLE__)
      int visid = atoi(argv[i]);
      fltk3::open_display();
      XVisualInfo templt; int num;
      templt.visualid = visid;
      fltk3::visual = XGetVisualInfo(fltk3::display, VisualIDMask, &templt, &num);
      if (!fltk3::visual) fltk3::fatal("No visual with id %d",visid);
      fltk3::colormap = XCreateColormap(fltk3::display, RootWindow(fltk3::display,fltk3::screen),
                                    fltk3::visual->visual, AllocNone);
      fltk3::xpixel(fltk3::BLACK); // make sure black is allocated
#else
      fltk3::fatal("Visual id's not supported on MSWindows or MacOS.");
#endif
    }
  }
  window.show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
