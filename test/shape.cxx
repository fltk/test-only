//
// "$Id$"
//
// Tiny OpenGL demo program for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#include <fltk3/run.h>
#include <fltk3/Window.h>
#include <fltk3/HorSlider.h>
#include <fltk3/math.h>

#if HAVE_GL

#include <fltk3/gl.h>
#include <fltk3/GlWindow.h>

class shape_window : public fltk3::GlWindow {
  void draw();
public:
  int sides;
  shape_window(int x,int y,int w,int h,const char *l=0);
};

shape_window::shape_window(int x,int y,int w,int h,const char *l) :
fltk3::GlWindow(x,y,w,h,l) {
  sides = 3;
}

void shape_window::draw() {
// the valid() property may be used to avoid reinitializing your
// GL transformation for each redraw:
  if (!valid()) {
    valid(1);
    glLoadIdentity();
    glViewport(0, 0, w(), h());
  }
// draw an amazing graphic:
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(.5,.6,.7);
  glBegin(GL_POLYGON);
  for (int j=0; j<sides; j++) {
    double ang = j*2*M_PI/sides;
    glVertex3f(cos(ang),sin(ang),0);
  }
  glEnd();
}

#else

#include <fltk3/Box.h>
class shape_window : public fltk3::Box {
public:	
  int sides;
  shape_window(int x,int y,int w,int h,const char *l=0)
    :fltk3::Box(fltk3::DOWN_BOX,x,y,w,h,l){
      label("This demo does\nnot work without GL");
  }
};

#endif

// when you change the data, as in this callback, you must call redraw():
void sides_cb(fltk3::Widget *o, void *p) {
  shape_window *sw = (shape_window *)p;
  sw->sides = int(((fltk3::Slider *)o)->value());
  sw->redraw();
}

int main(int argc, char **argv) {

  fltk3::Window window(300, 330);

// the shape window could be it's own window, but here we make it
// a child window:
  shape_window sw(10, 10, 280, 280);
// make it resize:
  window.resizable(&sw);
  //  window.size_range(300,330,0,0,1,1,1);
// add a knob to control it:
  fltk3::HorSlider slider(50, 295, window.w()-60, 30, "Sides:");
  slider.align(fltk3::ALIGN_LEFT);
  slider.callback(sides_cb,&sw);
  slider.value(sw.sides);
  slider.step(1);
  slider.bounds(3,40);

  window.end();
  window.show(argc,argv);
    
  return fltk3::run();
}

//
// End of "$Id$".
//
