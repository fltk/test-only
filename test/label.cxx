//
// "$Id$"
//
// Label test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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

// fltk123: still using wrong group offsets

#include <fltk3/run.h>
#include <fltk3/DoubleBufferWindow.h>
#include <fltk3/Widget.h>
#include <fltk3/HorValueSlider.h>
#include <fltk3/ToggleButton.h>
#include <fltk3/Input.h>
#include <fltk3/Choice.h>

using namespace fltk3;

ToggleButton *leftb,*rightb,*topb,*bottomb,*insideb,*clipb,*wrapb;
Widget *text;
Input *input;
HorValueSlider *fonts;
HorValueSlider *sizes;
DoubleBufferWindow *window;


void button_cb(Widget *,void *) {
  int i = 0;
  if (leftb->value()) i |= ALIGN_LEFT;
  if (rightb->value()) i |= ALIGN_RIGHT;
  if (topb->value()) i |= ALIGN_TOP;
  if (bottomb->value()) i |= ALIGN_BOTTOM;
  if (insideb->value()) i |= ALIGN_INSIDE;
  if (clipb->value()) i |= ALIGN_CLIP;
  if (wrapb->value()) i |= ALIGN_WRAP;
  text->align(i);
  window->redraw();
}

void font_cb(Widget *,void *) {
  text->labelfont(int(fonts->value()));
  window->redraw();
}

void size_cb(Widget *,void *) {
  text->labelsize(int(sizes->value()));
  window->redraw();
}

void input_cb(Widget *,void *) {
  text->label(input->value());
  window->redraw();
}

void normal_cb(Widget *,void *) {
  text->labeltype(NORMAL_LABEL);
  window->redraw();
}

void symbol_cb(Widget *,void *) {
  text->labeltype(NORMAL_LABEL);
  if (input->value()[0] != '@') {
    input->static_value("@->");
    text->label("@->");
  }
  window->redraw();
}

void shadow_cb(Widget *,void *) {
  text->labeltype(SHADOW_LABEL);
  window->redraw();
}

void embossed_cb(Widget *,void *) {
  text->labeltype(EMBOSSED_LABEL);
  window->redraw();
}

void engraved_cb(Widget *,void *) {
  text->labeltype(ENGRAVED_LABEL);
  window->redraw();
}

MenuItem choices[] = {
  {"NORMAL_LABEL",0,normal_cb},
  {"SYMBOL_LABEL",0,symbol_cb},
  {"SHADOW_LABEL",0,shadow_cb},
  {"ENGRAVED_LABEL",0,engraved_cb},
  {"EMBOSSED_LABEL",0,embossed_cb},
  {0}
};

int main(int argc, char **argv) {
  window = new DoubleBufferWindow(400,400);
  window->begin();
  {
    
    input = new Input(50,375,350,25);
    input->static_value("The quick brown fox jumped over the lazy dog.");
    // fltk123: input->when(WHEN_CHANGED);
    input->callback(input_cb);
    
    sizes= new HorValueSlider(50,350,350,25,"Size:");
    sizes->align(ALIGN_LEFT);
    sizes->bounds(1,64);
    sizes->step(1);
    sizes->value(14);
    sizes->callback(size_cb);
    
    fonts=new HorValueSlider(50,325,350,25,"Font:");
    fonts->align(ALIGN_LEFT);
    fonts->bounds(0,15);
    fonts->step(1);
    fonts->value(0);
    fonts->callback(font_cb);
    
    Group *g = new Group(50,300,350,25);
    g->begin();
    {
      leftb = new ToggleButton(50,300,50,25,"left");
      leftb->callback(button_cb);
      rightb = new ToggleButton(100,300,50,25,"right");
      rightb->callback(button_cb);
      topb = new ToggleButton(150,300,50,25,"top");
      topb->callback(button_cb);
      bottomb = new ToggleButton(200,300,50,25,"bottom");
      bottomb->callback(button_cb);
      insideb = new ToggleButton(250,300,50,25,"inside");
      insideb->callback(button_cb);
      wrapb = new ToggleButton(300,300,50,25,"wrap");
      wrapb->callback(button_cb);
      clipb = new ToggleButton(350,300,50,25,"clip");
      clipb->callback(button_cb);
      g->resizable(insideb);
    }
    g->end();
    
    Choice *c = new Choice(50,275,200,25);
    c->menu(choices);
    
    text = new Widget(100,75,200,100,input->value());
    text->box(BORDER_BOX);
    text->align(ALIGN_CENTER);
    window->resizable(text);
  }
  window->end();
  window->show(argc,argv);
  return run();
}

//
// End of "$Id$".
//
