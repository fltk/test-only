//
// "$Id$"
//
// Overlay window test program for the Fast Light Tool Kit (FLTK).
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

#include <stdlib.h>
#include <stdio.h>
#include <fltk3/run.h>
#include <fltk3/Window.h>
#include <fltk3/OverlayWindow.h>
#include <fltk3/Button.h>
#include <fltk3/draw.h>

int width=10,height=10;

class overlay : public fltk3::OverlayWindow {
public:
  overlay(int w,int h) : fltk3::OverlayWindow(w,h) {}
  void draw_overlay();
};

void overlay::draw_overlay() {
  fltk3::color(fltk3::RED); fltk3::rect((w()-width)/2,(h()-height)/2,width,height);
}

overlay *ovl;

void bcb1(fltk3::Widget *,void *) {width+=20; ovl->redraw_overlay();}
void bcb2(fltk3::Widget *,void *) {width-=20; ovl->redraw_overlay();}
void bcb3(fltk3::Widget *,void *) {height+=20; ovl->redraw_overlay();}
void bcb4(fltk3::Widget *,void *) {height-=20; ovl->redraw_overlay();}

int arg(int, char **argv, int& i) {
  fltk3::Color n = (fltk3::Color)atoi(argv[i]);
  if (n<=0) return 0;
  i++;
  uchar r,g,b;
  fltk3::get_color(n,r,g,b);
  fltk3::set_color(fltk3::RED,r,g,b);
  return i;
}

int main(int argc, char **argv) {
  int i=0; fltk3::args(argc,argv,i,arg);
  ovl = new overlay(400,400);
  fltk3::Button *b;
  b = new fltk3::Button(50,50,100,100,"wider\n(a)");
  b->callback(bcb1); b->shortcut('a');
  b = new fltk3::Button(250,50,100,100,"narrower\n(b)");
  b->callback(bcb2); b->shortcut('b');
  b = new fltk3::Button(50,250,100,100,"taller\n(c)");
  b->callback(bcb3); b->shortcut('c');
  b = new fltk3::Button(250,250,100,100,"shorter\n(d)");
  b->callback(bcb4); b->shortcut('d');
  ovl->resizable(ovl);
  ovl->end();
  ovl->show(argc,argv);
  ovl->redraw_overlay();
  return fltk3::run();
}

//
// End of "$Id$".
//
