//
// "$Id$"
//
// OpenGL overlay test program for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/ToggleButton.h>
#include <fltk3/math.h>

#if !HAVE_GL
#include <fltk3/Box.h>
class shape_window : public fltk3::Widget {
public:	
  int sides;
  shape_window(int x,int y,int w,int h,const char *l=0)
    :fltk3::Widget(fltk3::DOWN_BOX,x,y,w,h,l){
      label("This demo does\nnot work without GL");
  }
};
#else
#include <fltk3gl/gl.h>
#include <fltk3gl/GLWindow.h>

class shape_window : public fltk3::GLWindow {
  void draw();
  void draw_overlay();
public:
  int sides;
  int overlay_sides;
  shape_window(int x,int y,int w,int h,const char *l=0);
};

shape_window::shape_window(int x,int y,int w,int h,const char *l) :
fltk3::GLWindow(x,y,w,h,l) {
  sides = overlay_sides = 3;
}

void shape_window::draw() {
// the valid() property may be used to avoid reinitializing your
// GL transformation for each redraw:
  if (!valid()) {
    valid(1);
    glLoadIdentity();
    glViewport(0,0,w(),h());
  }
// draw an amazing but slow graphic:
  glClear(GL_COLOR_BUFFER_BIT);
  //  for (int j=1; j<=1000; j++) {
    glBegin(GL_POLYGON);
    for (int j=0; j<sides; j++) {
      double ang = j*2*M_PI/sides;
      glColor3f(float(j)/sides,float(j)/sides,float(j)/sides);
      glVertex3f(cos(ang),sin(ang),0);
    }
    glEnd();
  // }
}

void shape_window::draw_overlay() {
// the valid() property may be used to avoid reinitializing your
// GL transformation for each redraw:
  if (!valid()) {
    valid(1);
    glLoadIdentity();
    glViewport(0,0,w(),h());
  }
// draw an amazing graphic:
  gl_color(fltk3::RED);
  glBegin(GL_LINE_LOOP);
  for (int j=0; j<overlay_sides; j++) {
    double ang = j*2*M_PI/overlay_sides;
    glVertex3f(cos(ang),sin(ang),0);
  }
  glEnd();
}
#endif

// when you change the data, as in this callback, you must call redraw():
void sides_cb(fltk3::Widget *o, void *p) {
  shape_window *sw = (shape_window *)p;
  sw->sides = int(((fltk3::Slider *)o)->value());
  sw->redraw();
}

#if HAVE_GL
void overlay_sides_cb(fltk3::Widget *o, void *p) {
  shape_window *sw = (shape_window *)p;
  sw->overlay_sides = int(((fltk3::Slider *)o)->value());
  sw->redraw_overlay();
}
#endif
#include <stdio.h>
int main(int argc, char **argv) {

  fltk3::Window window(300, 370);

  shape_window sw(10, 75, window.w()-20, window.h()-90);
  sw.mode(fltk3::RGB);
  window.resizable(&sw);

  fltk3::HorSlider slider(60, 5, window.w()-70, 30, "Sides:");
  slider.align(fltk3::ALIGN_LEFT);
  slider.callback(sides_cb,&sw);
  slider.value(sw.sides);
  slider.step(1);
  slider.bounds(3,40);

  fltk3::HorSlider oslider(60, 40, window.w()-70, 30, "Overlay:");
  oslider.align(fltk3::ALIGN_LEFT);
#if HAVE_GL
  oslider.callback(overlay_sides_cb,&sw);
  oslider.value(sw.overlay_sides);
#endif
  oslider.step(1);
  oslider.bounds(3,40);

  window.end();
  window.show(argc,argv);
#if HAVE_GL
  printf("Can do overlay = %d\n", sw.can_do_overlay());
  sw.show();
  sw.redraw_overlay();
#else
  sw.show();
#endif

  return fltk3::run();
}

//
// End of "$Id$".
//
