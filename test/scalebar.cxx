//
// "$Id$"
//
// Double-buffering test program for the Fast Light Tool Kit (FLTK).
//
// This demo shows how double buffering helps, by drawing the
// window in a particularily bad way.
//
// The single-buffered window will blink as it updates.  The
// double buffered one will not.  It will take just as long
// (or longer) to update, but often it will appear to be faster.
//
// This demo should work for both the GL and X versions of Fl,
// even though the double buffering mechanism is totally different.
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
#include <fltk3/SingleWindow.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Box.h>
#include <fltk3/draw.h>
#include <fltk3/HorSlider.h>
#include <stdlib.h>
#include <fltk3/math.h>
#include <stdio.h>

// this purposely draws each line 10 times to be slow:
void star(int w, int h, int n) {
  fltk3::push_matrix();
  fltk3::translate(w/2, h/2);
  fltk3::scale(w/2, h/2);
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++)/* for (int k=0; k<10; k++)*/ {
      fltk3::begin_line();
      fltk3::vertex(cos(2*M_PI*i/n+.1), sin(2*M_PI*i/n+.1));
      fltk3::vertex(cos(2*M_PI*j/n+.1), sin(2*M_PI*j/n+.1));
      fltk3::end_line();
    }
  }
  fltk3::pop_matrix();
}

int sides[2] = {20,20};

void slider_cb(fltk3::Widget* o, long v) {
  sides[v] = int(((fltk3::Slider*)o)->value());
  o->parent()->redraw();
}

void bad_draw(int w,int h,int which) {
//   for (int i=0; i<10; i++) {
//     fltk3::color(7); fltk3::rectf(0,0,w,h); fltk3::color(0); star(w,h);
//     fltk3::color(0); fltk3::rectf(0,0,w,h); fltk3::color(7); star(w,h);
//   }
  fltk3::color(fltk3::BLACK); fltk3::rectf(0,0,w,h);
  fltk3::color(fltk3::WHITE); star(w,h,sides[which]);
  //  for (int x=0; x<sides[which]; x++) for (int y=0; y<sides[which]; y++)
  //fltk3::draw_box(fltk3::UP_BOX, 10*x, 10*y, 25,25, fltk3::GRAY);
}

class single_blink_window : public fltk3::SingleWindow {
  void draw() {bad_draw(w(),h(),0); draw_child(*child(0));}
public:
  single_blink_window(int x, int y,int w,int h,const char *l)
    : fltk3::SingleWindow(x,y,w,h,l) {resizable(this);}
};

class double_blink_window : public fltk3::DoubleWindow {
  void draw() {bad_draw(w(),h(),1); draw_child(*child(0));}
public:
  double_blink_window(int x, int y, int w,int h,const char *l)
    : fltk3::DoubleWindow(x,y,w,h,l) {resizable(this);}
};

int main(int argc, char **argv) {
  if (!fltk3::visual(fltk3::DOUBLE))
    printf("Xdbe not supported, faking double buffer with pixmaps.\n");
  fltk3::Window w01(420,420,"fltk3::Single_Window"); w01.box(fltk3::FLAT_BOX);
  single_blink_window w1(10,10,400,400,"fltk3::Single_Window");
  w1.box(fltk3::FLAT_BOX); w1.color(fltk3::BLACK); //w1.position(100,200);
  fltk3::HorSlider slider0(20,370,360,25);
  slider0.range(2,30);
  slider0.step(1);
  slider0.value(sides[0]);
  slider0.callback(slider_cb, 0);
  w1.end();
  w01.end();
  fltk3::Window w02(420,420,"fltk3::Double_Window"); w02.box(fltk3::FLAT_BOX);
  double_blink_window w2(10,10,400,400,"fltk3::Double_Window");
  w2.box(fltk3::FLAT_BOX); w2.color(fltk3::BLACK); //w2.position(600,200);
  fltk3::HorSlider slider1(20,370,360,25);
  slider1.range(2,30);
  slider1.step(1);
  slider1.value(sides[0]);
  slider1.callback(slider_cb, 1);
  w2.end();
  w02.end();
  w01.show(argc, argv);
  w1.show();
  w02.show();
  w2.show();
  return fltk3::run();
}

//
// End of "$Id$".
//
