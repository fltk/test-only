//
// "$Id: label.cxx,v 1.16 2003/01/19 07:55:01 spitzak Exp $"
//
// Label test program for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl_Menu_Item.H>
#include <fltk/run.h>
#include <fltk/DoubleBufferWindow.h>
#include <fltk/Box.h>
#include <fltk/HorizontalValueSlider.h>
#include <fltk/ToggleButton.h>
#include <fltk/Input.h>
#include <fltk/Choice.h>
#include <fltk/draw.h>
using namespace fltk;

ToggleButton *leftb,*rightb,*topb,*bottomb,*insideb,*clipb,*wrapb;
Widget *textbox;
Input *input;
HorizontalValueSlider *fontslider;
HorizontalValueSlider *sizes;
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
  textbox->align(i);
  window->redraw();
}

void font_cb(Widget *,void *) {
  textbox->labelfont(font(int(fontslider->value())));
  window->redraw();
}

void size_cb(Widget *,void *) {
  textbox->labelsize(int(sizes->value()));
  window->redraw();
}

void input_cb(Widget *,void *) {
  textbox->label(input->value());
  window->redraw();
}

void normal_cb(Widget *,void *) {
  textbox->labeltype(NORMAL_LABEL);
  window->redraw();
}

void symbol_cb(Widget *,void *) {
  textbox->labeltype(SYMBOL_LABEL);
  if (input->value()[0] != '@') {
    input->static_value("@->");
    textbox->label("@->");
  }
  window->redraw();
}

void shadow_cb(Widget *,void *) {
  textbox->labeltype(SHADOW_LABEL);
  window->redraw();
}

void embossed_cb(Widget *,void *) {
  textbox->labeltype(EMBOSSED_LABEL);
  window->redraw();
}

void engraved_cb(Widget *,void *) {
  textbox->labeltype(ENGRAVED_LABEL);
  window->redraw();
}

Fl_Menu_Item choices[] = {
  {"NORMAL_LABEL",0,normal_cb},
  {"SYMBOL_LABEL",0,symbol_cb},
  {"SHADOW_LABEL",0,shadow_cb},
  {"ENGRAVED_LABEL",0,engraved_cb},
  {"EMBOSSED_LABEL",0,embossed_cb},
  {0}};

int main(int argc, char **argv) {
  window = new DoubleBufferWindow(400,400);
  window->begin();

  input = new Input(50,375,350,25);
  input->static_value("The quick brown fox jumps over the lazy dog.");
  input->when(WHEN_CHANGED);
  input->callback(input_cb);

  sizes= new HorizontalValueSlider(50,350,350,25,"Size:");
  sizes->type(Slider::HORIZONTAL|Slider::TICK_ABOVE);
  sizes->clear_flag(ALIGN_MASK);
  sizes->set_flag(ALIGN_LEFT);
  sizes->range(1,64);
  sizes->step(1);
  sizes->value(14);
  sizes->callback(size_cb);

  fontslider=new HorizontalValueSlider(50,325,350,25,"Font:");
  fontslider->type(Slider::HORIZONTAL|Slider::TICK_ABOVE);
  fontslider->clear_flag(ALIGN_MASK);
  fontslider->set_flag(ALIGN_LEFT);
  fontslider->range(0,15);
  fontslider->step(1);
  fontslider->value(0);
  fontslider->callback(font_cb);

  Group *g = new Group(50,300,350,25);
  g->begin();
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
  g->fix_old_positions();
  g->end();

  Choice *c = new Choice(50,275,200,25);
  c->menu(choices);

  textbox= new Widget(100,75,200,100,input->value());
  textbox->box(ENGRAVED_BOX);
  textbox->clear_flag(ALIGN_MASK);
  textbox->set_flag(ALIGN_CENTER);
  window->resizable(textbox);
  window->end();
  window->show(argc,argv);
  return run();
}

//
// End of "$Id: label.cxx,v 1.16 2003/01/19 07:55:01 spitzak Exp $".
//
