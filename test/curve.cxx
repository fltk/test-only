//
// "$Id: curve.cxx,v 1.8 2001/02/20 06:59:50 spitzak Exp $"
//
// Curve test program for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Toggle_Button.H>

double args[9] = {
  20,20, 50,200, 100,20, 200,200, 0};
const char* name[9] = {
  "X0", "Y0", "X1", "Y1", "X2", "Y2", "X3", "Y3", "rotate"};

int points;

class Drawing : public Fl_Widget {
  void draw() {
    fl_push_clip(0,0,w(),h());
    fl_color(FL_DARK3);
    fl_rectf(0,0,w(),h());
    fl_push_matrix();

    if (args[8]) {
      fl_translate(w()/2.0, h()/2.0);
      fl_rotate(args[8]);
      fl_translate(-w()/2.0, -h()/2.0);
    }

    fl_color(FL_BLACK);
    fl_vertex(args[0],args[1]);
    fl_vertex(args[2],args[3]);
    fl_vertex(args[4],args[5]);
    fl_vertex(args[6],args[7]);
    fl_stroke();

    fl_curve(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7]);
    if (points) {
      fl_color(FL_WHITE);
      fl_points();
    } else {
      fl_color(FL_WHITE);
      fl_fill_stroke(FL_RED);
    }

    fl_pop_matrix();
    fl_pop_clip();
  }
public:
  Drawing(int X,int Y,int W,int H) : Fl_Widget(X,Y,W,H) {}
};

Drawing *d;

void points_cb(Fl_Widget* o, void*) {
  points = ((Fl_Toggle_Button*)o)->value();
  d->redraw();
}

void slider_cb(Fl_Widget* o, void* v) {
  Fl_Slider* s = (Fl_Slider*)o;
  args[long(v)] = s->value();
  d->redraw();
}

int main(int argc, char** argv) {
  Fl_Double_Window window(300,555);
  Drawing drawing(10,10,280,280);
  d = &drawing;

  int y = 300;
  for (int n = 0; n<9; n++) {
    Fl_Slider* s = new Fl_Hor_Value_Slider(50,y,240,25,name[n]); y += 25;
    s->minimum(0); s->maximum(280);
    if (n == 8) s->maximum(360);
    s->step(1);
    s->value(args[n]);
	s->clear_flag(FL_ALIGN_MASK);
    s->set_flag(FL_ALIGN_LEFT);
    s->callback(slider_cb, (void*)n);
  }
  Fl_Toggle_Button but(50,y,50,25,"points");
  but.callback(points_cb);

  window.end();
  window.show(argc,argv);
  return Fl::run();
}

//
// End of "$Id: curve.cxx,v 1.8 2001/02/20 06:59:50 spitzak Exp $".
//
