//
// "$Id: arc.cxx,v 1.13 2002/12/10 02:01:04 easysw Exp $"
//
// Arc drawing test program for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/Fl_Double_Window.h>
#include <fltk/Fl_Hor_Value_Slider.h>
#include <fltk/fl_draw.h>

float args[7] = {90, 90, 100, 100, 0, 360, 0};
const char* name[7] = {"X", "Y", "W", "H", "start", "end", "rotate"};

class Drawing : public Fl_Widget {
  void draw() {
    fl_push_clip(0, 0, w(), h());
    fl_color(FL_DARK3);
    fl_rectf(0, 0, w(), h());
    fl_push_matrix();
    //    if (args[6]) {
      fl_translate(w()/2.0f, h()/2.0f);
      fl_rotate(args[6]);
      fl_translate(-w()/2.0f, -h()/2.0f);
      //}
    fl_arc(args[0],args[1],args[2],args[3],args[4],args[5]);
    fl_closepath();
    fl_arc(120,120,40,40,0,-360);
    fl_ellipse(20,20,args[2],args[3]); // hardware circle
    fl_color(FL_WHITE);
    fl_fill_stroke(FL_RED);
    fl_pop_matrix();
    fl_pop_clip();
  }
public:
  Drawing(int X,int Y,int W,int H) : Fl_Widget(X,Y,W,H) {}
};

Drawing *d;

void slider_cb(Fl_Widget* o, void* v) {
  Fl_Slider* s = (Fl_Slider*)o;
  args[long(v)] = s->value();
  d->redraw();
}

int main(int argc, char** argv) {
  Fl_Double_Window window(300,500);
  Drawing drawing(10,10,280,280);
  d = &drawing;

  int y = 300;
  for (int n = 0; n<7; n++) {
    Fl_Slider* s = new Fl_Hor_Value_Slider(50,y,240,25,name[n]); y += 25;
    if (n<4) {s->minimum(0); s->maximum(300);}
    else if (n==6) {s->minimum(0); s->maximum(360);}
    else {s->minimum(-360); s->maximum(360);}
    s->type(Fl_Slider::HORIZONTAL | Fl_Slider::TICK_ABOVE);
    s->step(1);
    s->value(args[n]);
	s->clear_flag(FL_ALIGN_MASK);
    s->set_flag(FL_ALIGN_LEFT);
    s->callback(slider_cb, (void*)n);
  }

  window.end();
  window.show(argc,argv);
  return Fl::run();
}


//
// End of "$Id: arc.cxx,v 1.13 2002/12/10 02:01:04 easysw Exp $".
//

