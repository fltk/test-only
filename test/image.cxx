//
// "$Id:"
//
// Test of Fltk::Image types for the Fast Light Tool Kit (FLTK).
//
// Notice that Fltk::Image is for a static, multiple-reuse image, such
// as an icon or postage stamp.  Use fltk::draw_image to go directly
// from an buffered image that changes often.
//
// Copyright 1998-2001 by Bill Spitzak and others.
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

#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/Button.h>
#include <stdio.h>
#include <stdlib.h>
#include <fltk/draw.h>

#ifndef _WIN32
#include "list_visuals.cxx"
#include <fltk/visual.h>
#endif


using namespace fltk;

////////////////////////////////////////////////////////////////

#include <fltk/xbmImage.h>

#include "escherknot.xbm"
xbmImage bitmap(escherknot_bits, escherknot_width, escherknot_height);

////////////////////////////////////////////////////////////////

#include <fltk/xpmImage.h>

#include "porsche.xpm"
xpmImage pixmap(porsche_xpm);

////////////////////////////////////////////////////////////////

#include <fltk/rgbImage.h>

#define WIDTH 75
#define HEIGHT 75
uchar* make_image(int n) {
  uchar* image = new uchar[n*WIDTH*HEIGHT];
  uchar *p = image;
  for (int y = 0; y < HEIGHT; y++) {
    double Y = double(y)/(HEIGHT-1);
    for (int x = 0; x < WIDTH; x++) {
      int a = 255;
      if (n > 3) { // add alpha channel...
	a = x-WIDTH/2;
	int b = y-HEIGHT/2;
	a = (a*a+b*b)*255/(WIDTH/2*HEIGHT/2)-100;
	if (a > 255) a = 255;
	if (a < 0) a = 0;
      }
      double X = double(x)/(WIDTH-1);
      *p++ = uchar(a*((1-X)*(1-Y))); // red in upper-left
      *p++ = uchar(a*((1-X)*Y));	// green in lower-left
      *p++ = uchar(a*(X*Y));	// blue in lower-right
      if (n > 3) *p++ = a;
    }
  }
  return image;
}

rgbImage rgb_image(make_image(3), WIDTH, HEIGHT, 3);
uchar* rgbadata;
rgbImage rgba_image((rgbadata=make_image(4)), WIDTH, HEIGHT, 4);

class ImageTest : public Widget {
  void draw() {
    setcolor(RED);
    fillrect(0,0,w(),h());
    setcolor(BLACK);
    addvertex(0,0);
    addvertex(w(),h());
    strokepath();
    drawimage(rgbadata, 0, 0, WIDTH, HEIGHT, 4);
  }
public:
  ImageTest(int x, int y, int w, int h) : Widget(x,y,w,h) {}
};

////////////////////////////////////////////////////////////////

#include <fltk/ToggleButton.h>
#include <fltk/CheckButton.h>
#include <fltk/Choice.h>
#include <fltk/Item.h>

CheckButton *leftb,*rightb,*topb,*bottomb,*insideb,
  *clipb, *wrapb, *inactiveb, *tileb;
Button *b;
Window *w;

void button_cb(Widget *,void *) {
  int i = 0;
  if (leftb->value()) i |= ALIGN_LEFT;
  if (rightb->value()) i |= ALIGN_RIGHT;
  if (topb->value()) i |= ALIGN_TOP;
  if (bottomb->value()) i |= ALIGN_BOTTOM;
  if (insideb->value()) i |= ALIGN_INSIDE;
  if (clipb->value()) i |= ALIGN_CLIP;
  if (wrapb->value()) i |= ALIGN_WRAP;
  b->clear_flag(ALIGN_MASK);
  b->set_flag(i);
  if (inactiveb->value()) b->deactivate(); else b->activate();
  w->redraw();
}

#include <fltk/TiledImage.h>

TiledImage* tiledimage = 0;

void tile_cb(Widget* button, void*) {
  if (button->value()) {
    if (!tiledimage) tiledimage = new TiledImage(0);
    tiledimage->image((Image*)(b->image()));
    b->image(0);
    b->box(tiledimage);
  } else {
    b->image(tiledimage->image());
    b->box(0);
  }
  w->redraw();
}

void choice_cb(Widget* item, void* data) {
  b->label(item->label());
  if (tileb->value()) {
    tiledimage->image((Image*)data);
  } else {
    b->image((Image*)data);
  }
  w->redraw();
}

#ifndef _WIN32
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
#endif

int main(int argc, char **argv) {

#ifndef _WIN32
  int i = 1;
  if (args(argc,argv,i,::arg) < argc) {
    fprintf(stderr," -v # : use visual\n%s\n",help);
    exit(1);
  }

  if (visid >= 0) {
    open_display();
    XVisualInfo templt; int num;
    templt.visualid = visid;
    xvisual = XGetVisualInfo(xdisplay, VisualIDMask, &templt, &num);
    if (!xvisual) {
      fprintf(stderr, "No visual with id %d, use one of:\n",visid);
      list_visuals();
      exit(1);
    }
    xcolormap = XCreateColormap(xdisplay, RootWindow(xdisplay,xscreen),
				xvisual->visual, AllocNone);
    xpixel(BLACK); // make sure black is allocated in overlay visuals
  } else {
    visual(RGB_COLOR);
  }
#endif

  Window window(300,300); ::w = &window;
  window.begin();
  ToggleButton b(100,55,100,100,"Pixmap"); ::b = &b;
  b.image(pixmap);
  b.tooltip("This ToggleButton has:\n"
	    "image() set to the Image class selected below.\n"
	    "label() set to the name of that class.\n"
	    "align() set to the flags selected below.\n"
	    "Be sure to resize the window to see how it lays out");
  ImageTest it(0,0,100,100);
#define BWIDTH 60
#define BHEIGHT 21

  Group controls(10, 300-3*BHEIGHT-20, 280, 3*BHEIGHT+10);
  controls.box(ENGRAVED_BOX);
  controls.begin();

  Choice choice(5, 5, 110, BHEIGHT);
  choice.begin();
  Item i1("xbmImage");
  i1.callback(choice_cb, &bitmap);
  Item i2("xpmImage");
  i2.callback(choice_cb, &pixmap);
  Item i3("rgbImage");
  i3.callback(choice_cb, &rgb_image);
  Item i4("rgbaImage");
  i4.callback(choice_cb, &rgba_image);
  choice.end();
  choice.value(1); // set it to pixmap
  choice.tooltip("Subclass of Image to use");

  tileb= new CheckButton(115, 5, BWIDTH, BHEIGHT, "tiled");
  tileb->callback(tile_cb);
  tileb->tooltip("Use a TiledImage object around the Image");

  int y = 5+BHEIGHT;
  int x = 5;
  topb = new CheckButton(x, y, BWIDTH, BHEIGHT, "top"); x += BWIDTH;
  topb->callback(button_cb);
  topb->tooltip("ALIGN_TOP");
  bottomb= new CheckButton(x, y, BWIDTH, BHEIGHT, "bottom");x += BWIDTH;
  bottomb->callback(button_cb);
  bottomb->tooltip("ALIGN_BOTTOM");
  leftb = new CheckButton(x, y, BWIDTH, BHEIGHT, "left"); x += BWIDTH;
  leftb->callback(button_cb);
  leftb->tooltip("ALIGN_LEFT");
  rightb = new CheckButton(x, y, BWIDTH, BHEIGHT, "right"); x += BWIDTH;
  rightb->callback(button_cb);
  rightb->tooltip("ALIGN_RIGHT");
  y += BHEIGHT;
  x = 5;
  insideb= new CheckButton(x, y, BWIDTH, BHEIGHT, "inside");x += BWIDTH;
  insideb->callback(button_cb);
  insideb->tooltip("ALIGN_INSIDE");
  clipb= new CheckButton(x, y, BWIDTH, BHEIGHT, "clip"); x += BWIDTH;
  clipb->callback(button_cb);
  clipb->tooltip("ALIGN_CLIP");
  wrapb= new CheckButton(x, y, BWIDTH, BHEIGHT, "wrap"); x += BWIDTH;
  wrapb->callback(button_cb);
  wrapb->tooltip("ALIGN_WRAP");
  inactiveb= new CheckButton(x, y, BWIDTH, BHEIGHT, "inactive"); x += BWIDTH;
  inactiveb->callback(button_cb);
  inactiveb->tooltip("deactivate()");

  controls.end();

  Widget box(10,0,290,controls.y());
  box.hide();
  window.resizable(box);

  window.end();
  window.show(argc, argv);
  return run();
}

//
// End of "$Id: image.cxx,v 1.20 2004/06/04 08:35:11 spitzak Exp $".
//
