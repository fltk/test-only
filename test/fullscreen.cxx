//
// "$Id$"
//
// Fullscreen test program for the Fast Light Tool Kit (FLTK).
//
// This demo shows how to do many of the window manipulations that
// are popular on SGI programs, even though X does not really like
// them.  You can toggle the border on/off, change the visual to
// switch between single/double buffer, and make the window take
// over the screen.
//
// Normally the program makes a single window with a child GL window.
// This simulates a program where the 3D display is surrounded by
// control knobs.  Running the program with an argument will
// make it make a seperate GL window from the controls window.  This
// simulates a (older?) style program where the graphics display is
// a different window than the controls.
//
// This program reports how many times it redraws the window to
// stdout, so you can see how much time it is wasting.  It appears
// to be impossible to prevent X from sending redundant resize
// events, so there are extra redraws.  But the way I have the
// code arranged here seems to be keeping that to a minimu.
//
// Apparently unavoidable bugs:
//
// Turning the border on causes an unnecessary redraw.
//
// Turning off full screen when the border is on causes an unnecessary
// resize and redraw when the program turns the border on.
//
// If it is a seperate window, turning double buffering on and off
// will cause the window to raise, deiconize, and possibly move.  You
// can avoid this by making the fltk3::GlWindow a child of a normal
// window.
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
#include <fltk3/SingleWindow.h>
#include <fltk3/HorSlider.h>
#include <fltk3/LightButton.h>
#include <fltk3/math.h>
#include <stdio.h>

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
  printf("drawing size %d %d\n",w(),h());
  if (!valid()) {
    valid(1);
//  printf("init\n");
    glLoadIdentity();
    glViewport(0,0,w(),h());
  }
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(.5,.6,.7);
  glBegin(GL_POLYGON);
  for (int j = 0; j < sides; j ++) {
    double ang = j*2*M_PI/sides;
    glVertex3f(cos(ang),sin(ang),0);
  }
  glEnd();
}

#else

#include <fltk3/draw.h>

class shape_window : public fltk3::Window {
  void draw();
public:
  int sides;
  shape_window(int x,int y,int w,int h,const char *l=0);
};

shape_window::shape_window(int x,int y,int w,int h,const char *l) :
fltk3::Window(x,y,w,h,l) {
  sides = 3;
}

void shape_window::draw() {
  fltk3::color(0);
  fltk3::rectf(0,0,w(),h());
  fltk3::font(0,20);
  fltk3::color(7);
  fltk3::draw("This requires GL",0,0,w(),h(),fltk3::ALIGN_CENTER);
}

#endif

void sides_cb(fltk3::Widget *o, void *p) {
  shape_window *sw = (shape_window *)p;
  sw->sides = int(((fltk3::Slider *)o)->value());
  sw->redraw();
}

#if HAVE_GL
void double_cb(fltk3::Widget *o, void *p) {
  shape_window *sw = (shape_window *)p;
  int d = ((fltk3::Button *)o)->value();
  sw->mode(d ? fltk3::Mode(fltk3::DOUBLE|fltk3::RGB) : fltk3::RGB);
}
#else
void double_cb(fltk3::Widget *, void *) {}
#endif

void border_cb(fltk3::Widget *o, void *p) {
  fltk3::Window *w = (fltk3::Window *)p;
  int d = ((fltk3::Button *)o)->value();
  w->border(d);
#if defined(WIN32) || defined(__APPLE__)
  int wx = w->x(), wy = w->y();
  w->hide(); w->show();
  w->position(wx, wy);
#endif
}

int px,py,pw,ph;
fltk3::Button *border_button;
void fullscreen_cb(fltk3::Widget *o, void *p) {
  fltk3::Window *w = (fltk3::Window *)p;
  int d = ((fltk3::Button *)o)->value();
  if (d) {
    px = w->x();
    py = w->y();
    pw = w->w();
    ph = w->h();
#ifndef WIN32//necessary because fullscreen removes border
	border_button->value(0);
	border_button->do_callback();
#endif
    w->fullscreen();
  } else {
    w->fullscreen_off(px,py,pw,ph);
  }
}

#include <stdlib.h>

void exit_cb(fltk3::Widget *, void *) {
  exit(0);
}

#define NUMB 5

int twowindow = 0;
int initfull = 0;
int arg(int, char **argv, int &i) {
  if (argv[i][1] == '2') {twowindow = 1; i++; return 1;}
  if (argv[i][1] == 'f') {initfull = 1; i++; return 1;}
  return 0;
}

int main(int argc, char **argv) {

  int i=0;
  if (fltk3::args(argc,argv,i,arg) < argc)
    fltk3::fatal("Options are:\n -2 = 2 windows\n -f = startup fullscreen\n%s",fltk3::help);

  fltk3::SingleWindow window(300,300+30*NUMB); window.end();

  shape_window sw(10,10,window.w()-20,window.h()-30*NUMB-20);
#if HAVE_GL
  sw.mode(fltk3::RGB);
#endif

  fltk3::Window *w;
  if (twowindow) {	// make it's own window
    sw.resizable(&sw);
    w = &sw;
    window.set_modal();	// makes controls stay on top when fullscreen pushed
    argc--;
    sw.show();
  } else {		// otherwise make a subwindow
    window.add(sw);
    window.resizable(&sw);
    w = &window;
  }

  window.begin();

  int y = window.h()-30*NUMB-5;
  fltk3::HorSlider slider(50,y,window.w()-60,30,"Sides:");
  slider.align(fltk3::ALIGN_LEFT);
  slider.callback(sides_cb,&sw);
  slider.value(sw.sides);
  slider.step(1);
  slider.bounds(3,40);
  y+=30;

  fltk3::LightButton b1(50,y,window.w()-60,30,"Double Buffered");
  b1.callback(double_cb,&sw);
  y+=30;

  fltk3::LightButton b2(50,y,window.w()-60,30,"Border");
  b2.callback(border_cb,w);
  b2.set();
  border_button = &b2;
  y+=30;

  fltk3::LightButton b3(50,y,window.w()-60,30,"FullScreen");
  b3.callback(fullscreen_cb,w);
  y+=30;

  fltk3::Button eb(50,y,window.w()-60,30,"Exit");
  eb.callback(exit_cb);
  y+=30;

  if (initfull) {b3.set(); b3.do_callback();}

  window.end();
  window.show(argc,argv);

  return fltk3::run();
}

//
// End of "$Id$".
//
