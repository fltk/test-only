//
// "$Id: doublebuffer.cxx,v 1.10 2002/09/16 00:29:06 spitzak Exp $"
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
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <fltk/Fl.h>
#include <fltk/Fl_Single_Window.h>
#include <fltk/Fl_Double_Window.h>
#include <fltk/Fl_Box.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Hor_Slider.h>
#include <stdlib.h>
#include <fltk/math.h>
#include <stdio.h>

int sides[2] = {20,20};

void slider_cb(Fl_Widget* o, long v) {
  sides[v] = int(((Fl_Slider*)o)->value());
  o->parent()->redraw();
}

// A very blinky drawing function:
void bad_draw(int w, int h, int n) {
  fl_color(FL_BLACK);
  fl_rectf(0,0,w,h);
  fl_push_matrix();
  fl_translate(w/2, h/2);
  fl_scale(w/2, h/2);
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      fl_color((i+j)%(5*8*5)+FL_COLOR_CUBE);
      fl_vertex(cosf(2*M_PI*(j-1)/n+.1), sinf(2*M_PI*(j-1)/n+.1));
      fl_vertex(cosf(2*M_PI*i/n+.1), sinf(2*M_PI*i/n+.1));
      fl_vertex(cosf(2*M_PI*j/n+.1), sinf(2*M_PI*j/n+.1));
      fl_fill_stroke(FL_WHITE);
    }
  }
  fl_pop_matrix();
}

class single_blink_window : public Fl_Single_Window {
  void draw() {
    bad_draw(w(), h(), sides[0]);
    // redraw the slider:
    child(0)->set_damage(FL_DAMAGE_ALL);
    update_child(*child(0));
  }
public:
  single_blink_window(int x, int y,int w,int h,const char *l)
    : Fl_Single_Window(x,y,w,h,l) {resizable(this);}
};

class double_blink_window : public Fl_Double_Window {
  void draw() {
    bad_draw(w(), h(), sides[1]);
    // redraw the slider:
    child(0)->set_damage(FL_DAMAGE_ALL);
    update_child(*child(0));
  }
public:
  double_blink_window(int x, int y, int w,int h,const char *l)
    : Fl_Double_Window(x,y,w,h,l) {resizable(this);}
};

int main() {
  if (!Fl::visual(FL_DOUBLE))
    printf("Xdbe not supported, faking double buffer with pixmaps.\n");
  Fl_Window w01(420,420,"Fl_Single_Window"); w01.box(FL_FLAT_BOX);
  single_blink_window w1(10,10,400,400,"Fl_Single_Window");
  w1.box(FL_FLAT_BOX); w1.color(FL_BLACK); //w1.position(100,200);
  Fl_Hor_Slider slider0(20,370,360,25);
  slider0.range(2,30);
  slider0.step(1);
  slider0.value(sides[0]);
  slider0.callback(slider_cb, 0);
  w1.end();
  w01.resizable(w1);
  w01.end();
  Fl_Window w02(420,420,"Fl_Double_Window"); w02.box(FL_FLAT_BOX);
  double_blink_window w2(10,10,400,400,"Fl_Double_Window");
  w2.box(FL_FLAT_BOX); w2.color(FL_BLACK); //w2.position(600,200);
  Fl_Hor_Slider slider1(20,370,360,25);
  slider1.range(2,30);
  slider1.step(1);
  slider1.value(sides[0]);
  slider1.callback(slider_cb, 1);
  w2.end();
  w02.resizable(w2);
  w02.end();
  w01.show();
  w02.show();
  return Fl::run();
}

//
// End of "$Id: doublebuffer.cxx,v 1.10 2002/09/16 00:29:06 spitzak Exp $".
//
