//
// "$Id: label.cxx 6626 2009-01-12 14:52:45Z fabien $"
//
// Label test program for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/Box.h>
#include <fltk3/HorValueSlider.h>
#include <fltk3/ToggleButton.h>
#include <fltk3/Input.h>
#include <fltk3/Choice.h>
#include <fltk3/draw.h>

#include <math.h>
#ifndef M_PI
#define M_PI 3.141592654
#endif

fltk3::ToggleButton *leftb,*rightb,*clipb;
//fltk3::Box *text;
fltk3::Input *input;
fltk3::HorValueSlider *fonts;
fltk3::HorValueSlider *sizes;
fltk3::HorValueSlider *angles;
fltk3::DoubleWindow *window;

//code taken from fltk3::engraved_label.cxx
class Rotated_Label_Box : public fltk3::Widget{
  protected:
  void draw(){
    draw_box();
    fltk3::font(labelfont(), labelsize());
    fltk3::color(labelcolor());
    int dx(0),dy(0);
     
    if(rt_align&fltk3::ALIGN_CLIP)fltk3::push_clip(x(),y(),w(),h());
    else fltk3::push_no_clip();
    fltk3::measure(rt_text,dx,dy);
    if(rt_align&fltk3::ALIGN_LEFT){
      dx=dy=0;
    }else if(rt_align&fltk3::ALIGN_RIGHT){
      dy=(int)(-sin(M_PI*(double)(rt_angle+180)/180.)*(double)dx);
      dx=(int)(cos(M_PI*(double)(rt_angle+180)/180.)*(double)dx);
    }else{
      dy=(int)(sin(M_PI*(double)rt_angle/180.)*(double)dx);
      dx=(int)(-cos(M_PI*(double)rt_angle/180.)*(double)dx);
      dx/=2;dy/=2;
    }
    if(labeltype()==fltk3::SHADOW_LABEL)shadow_label(x()+w()/2+dx,y()+h()/2+dy);
    else if(labeltype()==fltk3::ENGRAVED_LABEL)engraved_label(x()+w()/2+dx,y()+h()/2+dy);
    else if(labeltype()==fltk3::EMBOSSED_LABEL)embossed_label(x()+w()/2+dx,y()+h()/2+dy);
    else{
     fltk3::draw(rt_angle,rt_text,x()+w()/2+dx,y()+h()/2+dy);
    }
    fltk3::pop_clip();
    draw_label();
  }
  void innards(int X, int Y, int data[][3], int n){
    for (int i = 0; i < n; i++) {
      fltk3::color((fltk3::Color)(i < n-1 ? data[i][2] : labelcolor()));
      fltk3::draw(rt_angle,rt_text, X+data[i][0], Y+data[i][1]);
    }
  }

  void shadow_label(int X, int Y){
    static int data[2][3] = {{2,2,fltk3::DARK3},{0,0,0}};
    innards(X, Y, data, 2);
  }

  void engraved_label(int X, int Y){
    static int data[7][3] = {
      {1,0,fltk3::LIGHT3},{1,1,fltk3::LIGHT3},{0,1,fltk3::LIGHT3},
      {-1,0,fltk3::DARK3},{-1,-1,fltk3::DARK3},{0,-1,fltk3::DARK3},
      {0,0,0}};
    innards(X, Y, data, 7);
  }

  void embossed_label(int X, int Y){
    static int data[7][3] = {
      {-1,0,fltk3::LIGHT3},{-1,-1,fltk3::LIGHT3},{0,-1,fltk3::LIGHT3},
      {1,0,fltk3::DARK3},{1,1,fltk3::DARK3},{0,1,fltk3::DARK3},
      {0,0,0}};
    innards(X, Y, data, 7);
  }

  public:
  Rotated_Label_Box(int X, int Y, int W, int H, const char*L=0):
    fltk3::Widget(X,Y,W,H,L),rt_angle(0),rt_align((fltk3::Align)0){
      rt_text=input->value();
    };
  int rt_angle;
  const char* rt_text;
  fltk3::Align rt_align;
}*text;


void button_cb(fltk3::Widget *,void *) {
  int i = 0;
  if (leftb->value()) i |= fltk3::ALIGN_LEFT;
  if (rightb->value()) i |= fltk3::ALIGN_RIGHT;
  if (clipb->value()) i |= fltk3::ALIGN_CLIP;
  text->rt_align=(fltk3::Align)i;
  window->redraw();
}

void font_cb(fltk3::Widget *,void *) {
  text->labelfont(int(fonts->value()));
  window->redraw();
}

void size_cb(fltk3::Widget *,void *) {
  text->labelsize(int(sizes->value()));
  window->redraw();
}
void angle_cb(fltk3::Widget *,void *) {
  text->rt_angle=(int)angles->value();
  window->redraw();
}

void input_cb(fltk3::Widget *,void *) {
  text->rt_text=input->value();
  window->redraw();
}

void normal_cb(fltk3::Widget *,void *) {
  text->labeltype(fltk3::NORMAL_LABEL);
  window->redraw();
}

void shadow_cb(fltk3::Widget *,void *) {
  text->labeltype(fltk3::SHADOW_LABEL);
  window->redraw();
}

void embossed_cb(fltk3::Widget *,void *) {
  text->labeltype(fltk3::EMBOSSED_LABEL);
  window->redraw();
}

void engraved_cb(fltk3::Widget *,void *) {
  text->labeltype(fltk3::ENGRAVED_LABEL);
  window->redraw();
}

fltk3::MenuItem choices[] = {
  {"fltk3::NORMAL_LABEL",0,normal_cb},
  {"fltk3::SHADOW_LABEL",0,shadow_cb},
  {"fltk3::ENGRAVED_LABEL",0,engraved_cb},
  {"fltk3::EMBOSSED_LABEL",0,embossed_cb},
  {0}};

int main(int argc, char **argv) {
  window = new fltk3::DoubleWindow(400,425);

  angles= new fltk3::HorValueSlider(50,400,350,25,"Angle:");
  angles->align(fltk3::ALIGN_LEFT);
  angles->bounds(-360,360);
  angles->step(1);
  angles->value(0);
  angles->callback(angle_cb);

  input = new fltk3::Input(50,375,350,25);
  input->static_value("Rotate Me!!!");
  input->when(fltk3::WHEN_CHANGED);
  input->callback(input_cb);

  sizes= new fltk3::HorValueSlider(50,350,350,25,"Size:");
  sizes->align(fltk3::ALIGN_LEFT);
  sizes->bounds(1,64);
  sizes->step(1);
  sizes->value(14);
  sizes->callback(size_cb);

  fonts=new fltk3::HorValueSlider(50,325,350,25,"Font:");
  fonts->align(fltk3::ALIGN_LEFT);
  fonts->bounds(0,15);
  fonts->step(1);
  fonts->value(0);
  fonts->callback(font_cb);

  fltk3::Group *g = new fltk3::Group(50,300,350,25);
  leftb = new fltk3::ToggleButton(50,300,50,25,"left");
  leftb->callback(button_cb);
  rightb = new fltk3::ToggleButton(100,300,50,25,"right");
  rightb->callback(button_cb);
  clipb = new fltk3::ToggleButton(350,300,50,25,"clip");
  clipb->callback(button_cb);
  g->resizable(rightb);
  g->end();

  fltk3::Choice *c = new fltk3::Choice(50,275,200,25);
  c->menu(choices);

  text= new Rotated_Label_Box(100,75,200,100,"Widget with rotated text");
  text->box(fltk3::FRAME_BOX);
  text->align(fltk3::ALIGN_BOTTOM);
  window->resizable(text);
  window->end();
  window->show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id: label.cxx 6626 2009-01-12 14:52:45Z fabien $".
//
