//
// "$Id$"
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
#include <fltk3/Pixmap.h>
#include <fltk3/draw.h>

#include "pixmaps/blast.xpm"

fltk3::ToggleButton *imageb, *imageovertextb, *imagenexttotextb, *imagebackdropb;
fltk3::ToggleButton *leftb,*rightb,*topb,*bottomb,*insideb,*clipb,*wrapb;
fltk3::Box *text;
fltk3::Input *input;
fltk3::HorValueSlider *fonts;
fltk3::HorValueSlider *sizes;
fltk3::DoubleWindow *window;
fltk3::Pixmap *img;

void button_cb(fltk3::Widget *,void *) {
  int i = 0;
  if (leftb->value()) i |= fltk3::ALIGN_LEFT;
  if (rightb->value()) i |= fltk3::ALIGN_RIGHT;
  if (topb->value()) i |= fltk3::ALIGN_TOP;
  if (bottomb->value()) i |= fltk3::ALIGN_BOTTOM;
  if (insideb->value()) i |= fltk3::ALIGN_INSIDE;
  if (clipb->value()) i |= fltk3::ALIGN_CLIP;
  if (wrapb->value()) i |= fltk3::ALIGN_WRAP;
  if (imageovertextb->value()) i |= fltk3::ALIGN_TEXT_OVER_IMAGE;
  if (imagenexttotextb->value()) i |= fltk3::ALIGN_IMAGE_NEXT_TO_TEXT;
  if (imagebackdropb->value()) i |= fltk3::ALIGN_IMAGE_BACKDROP;
  text->align(i);
  window->redraw();
}

void image_cb(fltk3::Widget *,void *) {
  if (imageb->value())
    text->image(img);
  else 
    text->image(0);
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

void input_cb(fltk3::Widget *,void *) {
  text->label(input->value());
  window->redraw();
}

void normal_cb(fltk3::Widget *,void *) {
  text->labeltype(fltk3::NORMAL_LABEL);
  window->redraw();
}

void symbol_cb(fltk3::Widget *,void *) {
  text->labeltype(fltk3::SYMBOL_LABEL);
  if (input->value()[0] != '@') {
    input->static_value("@->");
    text->label("@->");
  }
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
  {"fltk3::SYMBOL_LABEL",0,symbol_cb},
  {"fltk3::SHADOW_LABEL",0,shadow_cb},
  {"fltk3::ENGRAVED_LABEL",0,engraved_cb},
  {"fltk3::EMBOSSED_LABEL",0,embossed_cb},
  {0}};

int main(int argc, char **argv) {
  img = new fltk3::Pixmap(blast_xpm);
  
  window = new fltk3::DoubleWindow(400,400);

  input = new fltk3::Input(50,375,350,25);
  input->static_value("The quick brown fox jumped over the lazy dog.");
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

  fltk3::Group *g = new fltk3::Group(50,275,350,50);
  imageb = new fltk3::ToggleButton(0,0,50,25,"image");
  imageb->callback(image_cb);
  imageovertextb = new fltk3::ToggleButton(50,0,50,25,"I - T");
  imageovertextb->callback(button_cb);
  imagenexttotextb = new fltk3::ToggleButton(100,0,50,25,"I | T");
  imagenexttotextb->callback(button_cb);
  imagebackdropb = new fltk3::ToggleButton(150,0,50,25,"back");
  imagebackdropb->callback(button_cb);
  leftb = new fltk3::ToggleButton(0,25,50,25,"left");
  leftb->callback(button_cb);
  rightb = new fltk3::ToggleButton(50,25,50,25,"right");
  rightb->callback(button_cb);
  topb = new fltk3::ToggleButton(100,25,50,25,"top");
  topb->callback(button_cb);
  bottomb = new fltk3::ToggleButton(150,25,50,25,"bottom");
  bottomb->callback(button_cb);
  insideb = new fltk3::ToggleButton(200,25,50,25,"inside");
  insideb->callback(button_cb);
  wrapb = new fltk3::ToggleButton(250,25,50,25,"wrap");
  wrapb->callback(button_cb);
  clipb = new fltk3::ToggleButton(300,25,50,25,"clip");
  clipb->callback(button_cb);
  g->resizable(insideb);
  g->end();

  fltk3::Choice *c = new fltk3::Choice(50,250,200,25);
  c->menu(choices);

  text= new fltk3::Box(fltk3::FRAME_BOX,100,75,200,100,input->value());
  text->align(fltk3::ALIGN_CENTER);
  window->resizable(text);
  window->end();
  window->show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
