//
// "$Id: gl_overlay.cxx,v 1.7 2001/07/23 09:50:05 spitzak Exp $"
//
// OpenGL overlay test program for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include <fltk/Fl_Hor_Slider.h>
#include <fltk/Fl_Toggle_Button.h>
#include <fltk/math.h>

#if !HAVE_GL
#include <fltk/Fl_Box.h>
class shape_window : public Fl_Box {
public:	
  int sides;
  shape_window(int x,int y,int w,int h,const char *l=0)
    :Fl_Box(FL_DOWN_BOX,x,y,w,h,l){
      label("This demo does\nnot work without GL");
  }
};
#else
#include <fltk/gl.h>
#include <fltk/Fl_Gl_Window.h>

class shape_window : public Fl_Gl_Window {
  void draw();
  void draw_overlay();
public:
  int sides;
  int overlay_sides;
  shape_window(int x,int y,int w,int h,const char *l=0);
};

shape_window::shape_window(int x,int y,int w,int h,const char *l) :
Fl_Gl_Window(x,y,w,h,l) {
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
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POLYGON);
  for (int i=0; i<sides; i++) {
    double ang = i*2*M_PI/sides;
    glColor3f(float(i)/sides,float(i)/sides,float(i)/sides);
    glVertex3f(cos(ang),sin(ang),0);
  }
  glEnd();
  gl_color(FL_WHITE);
  gl_font(FL_HELVETICA, FL_NORMAL_SIZE);
  gl_draw("text", .1, .5);
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
  gl_color(FL_RED);
  glBegin(GL_LINE_LOOP);
  for (int i=0; i<overlay_sides; i++) {
    double ang = i*2*M_PI/overlay_sides;
    glVertex3f(cos(ang),sin(ang),0);
  }
  glEnd();
  glEnd();
  gl_font(FL_HELVETICA, FL_NORMAL_SIZE);
  gl_draw("overlay text", .1, .6);
}
#endif

// when you change the data, as in this callback, you must call redraw():
void sides_cb(Fl_Widget *o, void *p) {
  shape_window *sw = (shape_window *)p;
  sw->sides = int(((Fl_Slider *)o)->value());
  sw->redraw();
}

#if HAVE_GL
void overlay_sides_cb(Fl_Widget *o, void *p) {
  shape_window *sw = (shape_window *)p;
  sw->overlay_sides = int(((Fl_Slider *)o)->value());
  sw->redraw_overlay();
}
#endif
#include <stdio.h>
int main(int argc, char **argv) {

  Fl_Window window(300, 370);

  shape_window sw(10, 75, window.w()-20, window.h()-90);
//sw.mode(FL_RGB);
  window.resizable(&sw);

  Fl_Hor_Slider slider(60, 5, window.w()-70, 30, "Sides:");
  slider.clear_flag(FL_ALIGN_MASK);
  slider.set_flag(FL_ALIGN_LEFT);
  slider.callback(sides_cb,&sw);
  slider.value(sw.sides);
  slider.step(1);
  slider.range(3,40);

  Fl_Hor_Slider oslider(60, 40, window.w()-70, 30, "Overlay:");
  oslider.clear_flag(FL_ALIGN_MASK);
  oslider.set_flag(FL_ALIGN_LEFT);
#if HAVE_GL
  oslider.callback(overlay_sides_cb,&sw);
  oslider.value(sw.overlay_sides);
#endif
  oslider.step(1);
  oslider.range(3,40);

  window.end();
  window.show(argc,argv);
#if HAVE_GL
  printf("Can do overlay = %d\n", sw.can_do_overlay());
  sw.show();
  sw.redraw_overlay();
#else
  sw.show();
#endif

  return Fl::run();
}

//
// End of "$Id: gl_overlay.cxx,v 1.7 2001/07/23 09:50:05 spitzak Exp $".
//
