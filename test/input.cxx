//
// "$Id$"
//
// Input field test program for the Fast Light Tool Kit (FLTK).
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

#include <stdio.h>
#include <fltk3/run.h>
#include <fltk3/Window.h>
#include <fltk3/Input.h>
#include <fltk3/FloatInput.h>
#include <fltk3/IntInput.h>
#include <fltk3/SecretInput.h>
#include <fltk3/MultilineInput.h>
#include <fltk3/Button.h>
#include <fltk3/ToggleButton.h>
#include <fltk3/LightButton.h>
#include <fltk3/ColorChooser.h>

void cb(fltk3::Widget *ob) {
  printf("Callback for %s '%s'\n",ob->label(),((fltk3::Input*)ob)->value());
}

int when = 0;
fltk3::Input *input[5];

void toggle_cb(fltk3::Widget *o, long v) {
  if (((fltk3::ToggleButton*)o)->value()) when |= v; else when &= ~v;
  for (int i=0; i<5; i++) input[i]->when(when);
}

void test(fltk3::Input *i) {
  if (i->changed()) {
    i->clear_changed(); printf("%s '%s'\n",i->label(),i->value());
    char utf8buf[10];
    int last = fltk3::utf8encode(i->index(i->position()), utf8buf);
    utf8buf[last] = 0;
    printf("Symbol at cursor position: %s\n", utf8buf);
  }
}

void button_cb(fltk3::Widget *,void *) {
  for (int i=0; i<5; i++) test(input[i]);
}

void color_cb(fltk3::Widget* button, void* v) {
  fltk3::Color c;
  switch ((fl_intptr_t)v) {
  case 0: c = fltk3::BACKGROUND2_COLOR; break;
  case 1: c = fltk3::SELECTION_COLOR; break;
  default: c = fltk3::FOREGROUND_COLOR; break;
  }
  uchar r,g,b; fltk3::get_color(c, r,g,b);
  if (fltk3::color_chooser(0,r,g,b)) {
    fltk3::set_color(c,r,g,b); fltk3::redraw();
    button->labelcolor(fltk3::contrast(fltk3::BLACK,c));
    button->redraw();
  }
}

void tabnav_cb(fltk3::Widget *w, void *v) {
  fltk3::LightButton *b = (fltk3::LightButton*)w;
  fltk3::MultilineInput *fmi = (fltk3::MultilineInput*)v;
  fmi->tab_nav(b->value() ? 1 : 0);
}

void arrownav_cb(fltk3::Widget *w, void *v) {
  fltk3::LightButton *b = (fltk3::LightButton*)w;
  fltk3::option(fltk3::OPTION_ARROW_FOCUS, b->value() ? true : false);
}

int main(int argc, char **argv) {
  // the following two lines set the correct color scheme, so that 
  // calling fltk3::contrast below will return good results
  fltk3::args(argc, argv);
  fltk3::get_system_colors();
  fltk3::Window *window = new fltk3::Window(400,425);
  window->color(0x99bb9900);

  int y = 10;
  input[0] = new fltk3::Input(70,y,300,30,"Normal:"); y += 35;
  input[0]->tooltip("Normal input field");
  // input[0]->cursor_color(fltk3::SELECTION_COLOR);
  // input[0]->maximum_size(20);
  // input[0]->static_value("this is a testgarbage");
  input[1] = new fltk3::FloatInput(70,y,300,30,"Float:"); y += 35;
  input[1]->tooltip("Input field for floating-point number (F1)");
  input[1]->shortcut(fltk3::FKey+1);
  input[2] = new fltk3::IntInput(70,y,300,30,"Int:"); y += 35;
  input[2]->tooltip("Input field for integer number (F2)");
  input[2]->shortcut(fltk3::FKey+2);
  input[3] = new fltk3::SecretInput(70,y,300,30,"&Secret:"); y += 35;
  input[3]->tooltip("Input field for password (Alt-S)");
  input[4] = new fltk3::MultilineInput(70,y,300,100,"&Multiline:"); y += 105;
  input[4]->tooltip("Input field for short text with newlines (Alt-M)");
  input[4]->wrap(1);

  for (int i = 0; i < 4; i++) {
    input[i]->when(0); input[i]->callback(cb);
  }
  int y1 = y;

  fltk3::Button *b;
  b = new fltk3::ToggleButton(10,y,200,25,"fltk3::WHEN_CHANGED");
  b->box(fltk3::UP_BOX);
  b->callback(toggle_cb, fltk3::WHEN_CHANGED); y += 25;
  b->tooltip("Do callback each time the text changes");
  b = new fltk3::ToggleButton(10,y,200,25,"fltk3::WHEN_RELEASE");
  b->box(fltk3::UP_BOX);
  b->callback(toggle_cb, fltk3::WHEN_RELEASE); y += 25;
  b->tooltip("Do callback when widget loses focus");
  b = new fltk3::ToggleButton(10,y,200,25,"fltk3::WHEN_ENTER_KEY");
  b->box(fltk3::UP_BOX);
  b->callback(toggle_cb, fltk3::WHEN_ENTER_KEY); y += 25;
  b->tooltip("Do callback when user hits Enter key");
  b = new fltk3::ToggleButton(10,y,200,25,"fltk3::WHEN_NOT_CHANGED");
  b->box(fltk3::UP_BOX);
  b->callback(toggle_cb, fltk3::WHEN_NOT_CHANGED); y += 25;
  b->tooltip("Do callback even if the text is not changed");
  y += 5;
  
  b = new fltk3::Button(10,y,200,25,"&print changed()"); y += 25;
  b->callback(button_cb);
  b->tooltip("Print widgets that have changed() flag set");

  y += 5;
  b = new fltk3::LightButton(10,y,100,25," Tab Nav");
  b->box(fltk3::UP_BOX);
  b->tooltip("Control tab navigation for the multiline input field");
  b->callback(tabnav_cb, (void*)input[4]);
  b->value(input[4]->tab_nav() ? 1 : 0);
  b = new fltk3::LightButton(110,y,100,25," Arrow Nav"); y += 25;
  b->box(fltk3::UP_BOX);
  b->tooltip("Control horizontal arrow key focus navigation behavior.\n"
             "e.g. fltk3::OPTION_ARROW_FOCUS");
  b->callback(arrownav_cb);
  b->value(input[4]->tab_nav() ? 1 : 0);
  b->value(fltk3::option(fltk3::OPTION_ARROW_FOCUS) ? 1 : 0);

  b = new fltk3::Button(220,y1,120,25,"color"); y1 += 25;
  b->box(fltk3::UP_BOX);
  b->color(input[0]->color()); b->callback(color_cb, (void*)0);
  b->tooltip("Color behind the text");
  b = new fltk3::Button(220,y1,120,25,"selection_color"); y1 += 25;
  b->box(fltk3::UP_BOX);
  b->color(input[0]->selection_color()); b->callback(color_cb, (void*)1);
  b->labelcolor(fltk3::contrast(fltk3::BLACK,b->color()));
  b->tooltip("Color behind selected text");
  b = new fltk3::Button(220,y1,120,25,"textcolor"); y1 += 25;
  b->box(fltk3::UP_BOX);
  b->color(input[0]->textcolor()); b->callback(color_cb, (void*)2);
  b->labelcolor(fltk3::contrast(fltk3::BLACK,b->color()));
  b->tooltip("Color of the text");

  window->end();
  window->show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
