//
// "$Id$"
//
// Curve test program for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/HorValueSlider.h>
#include <fltk3/draw.h>
#include <fltk3/ToggleButton.h>

double args[9] = {
  20,20, 50,200, 100,20, 200,200, 0};
const char* name[9] = {
  "X0", "Y0", "X1", "Y1", "X2", "Y2", "X3", "Y3", "rotate"};

int points;

class Drawing : public fltk3::Widget {
  void draw() {
    fltk3::push_clip(x(),y(),w(),h());
    fltk3::color(fltk3::DARK3);
    fltk3::rectf(x(),y(),w(),h());
    fltk3::push_matrix();
    if (args[8]) {
      fltk3::translate(x()+w()/2.0, y()+h()/2.0);
      fltk3::rotate(args[8]);
      fltk3::translate(-(x()+w()/2.0), -(y()+h()/2.0));
    }
    fltk3::translate(x(),y());
    if (!points) {
    fltk3::color(fltk3::WHITE);
    fltk3::begin_complex_polygon();
   fltk3::curve(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7]);
    fltk3::end_complex_polygon();
    }
    fltk3::color(fltk3::BLACK);
    fltk3::begin_line();
    fltk3::vertex(args[0],args[1]);
    fltk3::vertex(args[2],args[3]);
    fltk3::vertex(args[4],args[5]);
    fltk3::vertex(args[6],args[7]);
    fltk3::end_line();
    fltk3::color(points ? fltk3::WHITE : fltk3::RED);
    points ? fltk3::begin_points() : fltk3::begin_line();
   fltk3::curve(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7]);
    points ? fltk3::end_points() : fltk3::end_line();
    fltk3::pop_matrix();
    fltk3::pop_clip();
  }
public:
  Drawing(int X,int Y,int W,int H) : fltk3::Widget(X,Y,W,H) {}
};

Drawing *d;

void points_cb(fltk3::Widget* o, void*) {
  points = ((fltk3::ToggleButton*)o)->value();
  d->redraw();
}

void slider_cb(fltk3::Widget* o, void* v) {
  fltk3::Slider* s = (fltk3::Slider*)o;
  args[fl_intptr_t(v)] = s->value();
  d->redraw();
}

int main(int argc, char** argv) {
  fltk3::DoubleWindow window(300,555);
  Drawing drawing(10,10,280,280);
  d = &drawing;

  int y = 300;
  for (int n = 0; n<9; n++) {
    fltk3::Slider* s = new fltk3::HorValueSlider(50,y,240,25,name[n]); y += 25;
    s->minimum(0); s->maximum(280);
    if (n == 8) s->maximum(360);
    s->step(1);
    s->value(args[n]);
    s->align(fltk3::ALIGN_LEFT);
    s->callback(slider_cb, (void*)n);
  }
  fltk3::ToggleButton but(50,y,50,25,"points");
  but.callback(points_cb);

  window.end();
  window.show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
