//
// "$Id$"
//
// Mandelbrot set demo for the Fast Light Tool Kit (FLTK).
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

#include "mandelbrot_ui.h"
#include <fltk3/draw.h>
#include <fltk3/Button.h>
#include <fltk3/Printer.h>
#include <stdio.h>
#include <stdlib.h>

Drawing_Window mbrot;
Drawing_Window jbrot;

void idle(void*) {
  if (!mbrot.d->idle() && !(jbrot.d && jbrot.d->idle())) fltk3::remove_idle(idle);
}

void set_idle() {
  fltk3::add_idle(idle);
}

static void window_callback(fltk3::Widget*, void*) {exit(0);}

static void print(fltk3::Widget *o, void *data)
{
  fltk3::Printer printer;
  fltk3::Window *win = o->window();
  if(!win->visible()) return;
  win->make_current();
  uchar *image_data = fltk3::read_image(NULL, 0, 0, win->w(), win->h(), 0);
  if( printer.start_job(1) ) return;
  if( printer.start_page() ) return;
  printer.scale(.7f, .7f);
  fltk3::draw_image(image_data, 0,0, win->w(), win->h());
  printer.end_page();
  delete image_data;
  printer.end_job();
}

int main(int argc, char **argv) {
  mbrot.make_window();
    mbrot.window->begin();
    fltk3::Button* o = new fltk3::Button(0, 0, 0, 0, NULL);
    o->callback(print,NULL);
    o->shortcut(fltk3::CTRL+'p');
    mbrot.window->end();

  mbrot.d->X = -.75;
  mbrot.d->scale = 2.5;
  mbrot.update_label();
  int i = 0;
  if (fltk3::args(argc,argv,i) < argc) fltk3::fatal(fltk3::help);
  fltk3::visual(fltk3::RGB);
  mbrot.window->callback(window_callback);
  mbrot.window->show(argc,argv);
  fltk3::run();
  return 0;
}

void Drawing_Window::update_label() {
  char buffer[128];
  sprintf(buffer, "%+.10f", d->X); x_input->value(buffer);
  sprintf(buffer, "%+.10f", d->Y); y_input->value(buffer);
  sprintf(buffer, "%.2g", d->scale); w_input->value(buffer);
}

void Drawing_Area::draw() {
  draw_box();
  drawn = 0;
  set_idle();
}

#ifdef __APPLE__
const int slice = 100; // plot drawn by blocks of slice lines
#else
const int slice = 1;
#endif

int Drawing_Area::idle() {
  if (!window()->visible()) return 0;
  int rest = H - drawn;
  if (rest > slice) rest = slice;
  if (rest && drawn + rest <= nextline) {    
    window()->make_current();
    int yy = drawn+y()+4;
    if (yy >= sy && yy <= sy+sh) erase_box();
    fltk3::draw_image_mono(buffer+drawn*W,x()+3,yy,W,rest,1,W);
    drawn+=rest;
    return 1;
  }
  if (nextline < H) {
    if (!buffer) buffer = new uchar[W*H];
    double yy = Y+(H/2-nextline)*scale/W;
    double yi = yy; if (julia) yy = jY;
    uchar *p = buffer+nextline*W;
    for (int xi = 0; xi < W; xi++) {
      double xx = X+(xi-W/2)*scale/W;
      double wx = xx; double wy = yi;
      if (julia) xx = jX;
      for (int i=0; ; i++) {
	if (i >= iterations) {*p = 0; break;}
	double t = wx*wx - wy*wy + xx;
	wy = 2*wx*wy + yy;
	wx = t;
	if (wx*wx + wy*wy > 4) {
	  wx = t = 1-double(i)/(1<<10);
	  if (t <= 0) t = 0; else for (i=brightness; i--;) t*=wx;
	  *p = 255-int(254*t);
	  break;
	}
      }
      p++;
    }
    nextline++;
  }
  return drawn < H;
}

void Drawing_Area::erase_box() {
  window()->make_current();
  fltk3::overlay_clear();
}

int Drawing_Area::handle(int event) {
  static int ix, iy;
  static int dragged;
  static int button;
  int x2,y2;
  switch (event) {
  case fltk3::PUSH:
    erase_box();
    ix = fltk3::event_x(); if (ix<0) ix=0; if (ix>=w()) ix=w()-1;
    iy = fltk3::event_y(); if (iy<0) iy=0; if (iy>=h()) iy=h()-1;
    dragged = 0;
    button = fltk3::event_button();
    return 1;
  case fltk3::DRAG:
    dragged = 1;
    erase_box();
    x2 = fltk3::event_x(); if (x2<0) x2=0; if (x2>=w()) x2=w()-1;
    y2 = fltk3::event_y(); if (y2<0) y2=0; if (y2>=h()) y2=h()-1;
    if (button != 1) {ix = x2; iy = y2; return 1;}
    if (ix < x2) {sx = ix; sw = x2-ix;} else {sx = x2; sw = ix-x2;}
    if (iy < y2) {sy = iy; sh = y2-iy;} else {sy = y2; sh = iy-y2;}
    window()->make_current();
    fltk3::overlay_rect(dx_window()+sx,dy_window()+sy,sw,sh);
    return 1;
  case fltk3::RELEASE:
    if (button == 1) {
      erase_box();
      if (dragged && sw > 3 && sh > 3) {
	X = X + (sx+sw/2-W/2)*scale/W;
	Y = Y + (-sy-sh/2+H/2)*scale/W;
	scale = sw*scale/W;
      } else if (!dragged) {
	scale = 2*scale;
	if (julia) {
	  if (scale >= 4) {
	    scale = 4;
	    X = Y = 0;
	  }
	} else {
	  if (scale >= 2.5) {
	    scale = 2.5;
	    X = -.75;
	    Y = 0;
	  }
	}
      } else return 1;
      ((Drawing_Window*)(user_data()))->update_label();
      new_display();
    } else if (!julia) {
      if (!jbrot.d) {
	jbrot.make_window();
	jbrot.d->julia = 1;
	jbrot.d->X = 0;
	jbrot.d->Y = 0;
	jbrot.d->scale = 4;
	jbrot.update_label();
      }
      jbrot.d->jX = X + (ix-W/2)*scale/W;
      jbrot.d->jY = Y + (H/2-iy)*scale/W;
      static char s[128];
      sprintf(s, "Julia %.7f %.7f",jbrot.d->jX,jbrot.d->jY);
      jbrot.window->label(s);
      jbrot.window->show();
      jbrot.d->new_display();
    }
    return 1;
  }
  return 0;
}

void Drawing_Area::new_display() {
  drawn = nextline = 0;
  set_idle();
}

void Drawing_Area::resize(int XX,int YY,int WW,int HH) {
  if (WW != w() || HH != h()) {
    W = WW-6;
    H = HH-8;
    if (buffer) {delete[] buffer; buffer = 0; new_display();}
  }
  Widget::resize(XX,YY,WW,HH);
}

//
// End of "$Id$".
//
