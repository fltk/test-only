//
// "$Id: arc.cxx,v 1.16 2004/03/10 07:41:46 spitzak Exp $"
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

#include <fltk/run.h>
#include <fltk/DoubleBufferWindow.h>
#include <fltk/HorizontalValueSlider.h>
#include <fltk/draw.h>

float args[7] = {90, 90, 100, 100, 0, 360, 0};
const char* name[7] = {"X", "Y", "W", "H", "start", "end", "rotate"};

class Drawing : public fltk::Widget {
  void draw() {
    fltk::push_clip(0, 0, w(), h());
    fltk::setcolor(fltk::BLACK);
    fltk::fillrect(0, 0, w(), h());
    fltk::push_matrix();
    //    if (args[6]) {
      fltk::translate(w()/2.0f, h()/2.0f);
      fltk::rotate(args[6]);
      fltk::translate(-w()/2.0f, -h()/2.0f);
      //}
    fltk::addarc(args[0],args[1],args[2],args[3],args[4],args[5]);
    fltk::closepath();
    fltk::addarc(120,120,40,40,0,-360);
    fltk::addellipse(20,20,args[2],args[3]); // hardware circle
    fltk::setcolor(fltk::GRAY33);
    fltk::fillstrokepath(fltk::WHITE);
    fltk::pop_matrix();
    fltk::setcolor(fltk::GRAY66);
    fltk::fillrect(10,270-(int) args[3],(int) args[2],(int) args[3]);
    fltk::setcolor(fltk::GRAY10);
    fltk::fillarc(10,270-(int) args[3],(int) args[2],(int) args[3],(int) args[4],(int) args[5]);
    fltk::pop_clip();
  }
public:
  Drawing(int X,int Y,int W,int H) : fltk::Widget(X,Y,W,H) {}
};

Drawing *d;

void slider_cb(fltk::Widget* o, void* v) {
  fltk::Slider* s = (fltk::Slider*)o;
  args[long(v)] = s->value();
  d->redraw();
}

int main(int argc, char** argv) {
  fltk::DoubleBufferWindow window(300,500);
  window.begin();
  Drawing drawing(10,10,280,280);
  d = &drawing;

  int y = 300;
  for (int n = 0; n<7; n++) {
    fltk::Slider* s =
      new fltk::HorizontalValueSlider(50,y,240,25,name[n]); y += 25;
    if (n<4) {s->minimum(0); s->maximum(300);}
    else if (n==6) {s->minimum(0); s->maximum(360);}
    else {s->minimum(-360); s->maximum(360);}
    s->type(fltk::Slider::HORIZONTAL | fltk::Slider::TICK_ABOVE);
    s->step(1);
    s->value(args[n]);
	s->clear_flag(fltk::ALIGN_MASK);
    s->set_flag(fltk::ALIGN_LEFT);
    s->callback(slider_cb, (void*)n);
  }

  window.end();
  window.resizable(drawing);
  window.show(argc,argv);
  return fltk::run();
}


//
// End of "$Id: arc.cxx,v 1.16 2004/03/10 07:41:46 spitzak Exp $".
//

