//
// "$Id: input.cxx,v 1.21 2000/05/30 07:42:19 bill Exp $"
//
// Input field test program for the Fast Light Tool Kit (FLTK).
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

#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/fl_show_colormap.H>

void cb(Fl_Widget *ob) {
  printf("Callback for %s '%s'\n",ob->label(),((Fl_Input*)ob)->value());
}

int when = 0;
Fl_Input *input[5];

void toggle_cb(Fl_Widget *o, long v) {
  if (((Fl_Toggle_Button*)o)->value()) when |= v; else when &= ~v;
  for (int i=0; i<5; i++) input[i]->when(when);
}

void test(Fl_Input *i) {
  if (i->changed()) {
    i->clear_changed(); printf("%s '%s'\n",i->label(),i->value());
  }
}

void button_cb(Fl_Widget *,void *) {
  for (int i=0; i<5; i++) test(input[i]);
}

void color_cb(Fl_Widget* button, void* v) {
  Fl_Style& s = *Fl_Input::default_style;
  Fl_Color c;
  
  switch ((long)v) {
    case 0: c = s.text_background; break;
    case 1: c = s.selection_color; break;
    default: c = s.text_color; break;
  }
  c = fl_show_colormap(c);
  switch ((long)v) {
    case 0: s.text_background = c; break;
    case 1: s.selection_color = c; break;
    default: s.text_color = c; break;
  }
  s.selection_text_color = fl_contrast(s.text_color, s.selection_color);
  button->color(c);
  button->label_color(fl_contrast(FL_BLACK,(Fl_Color)c));
  Fl::redraw();
}

int main(int argc, char **argv) {
  Fl_Window *window = new Fl_Window(400,350);

  int y = 10;
  input[0] = new Fl_Input(70,y,300,23,"Normal:"); y += 27;
  // input[0]->cursor_color(FL_SELECTION_COLOR);
  //  input[0]->maximum_size(20);
  // input[0]->static_value("this is a testgarbage");
  input[1] = new Fl_Float_Input(70,y,300,23,"Float:"); y += 27;
  input[2] = new Fl_Int_Input(70,y,300,23,"Int:"); y += 27;
  input[3] = new Fl_Secret_Input(70,y,300,23,"Secret:"); y += 27;
  input[4] = new Fl_Multiline_Input(70,y,300,100,"Multiline:"); y += 105;

  for (int i = 0; i < 4; i++) {
    input[i]->when(0); input[i]->callback(cb);
  }
  int y1 = y;

  Fl_Button *b;
  b = new Fl_Toggle_Button(10,y,200,23,"FL_WHEN_&CHANGED");
  b->callback(toggle_cb, FL_WHEN_CHANGED); y += 23;
  b = new Fl_Toggle_Button(10,y,200,23,"FL_WHEN_&RELEASE");
  b->callback(toggle_cb, FL_WHEN_RELEASE); y += 23;
  b = new Fl_Toggle_Button(10,y,200,23,"FL_WHEN_&ENTER_KEY");
  b->callback(toggle_cb, FL_WHEN_ENTER_KEY); y += 23;
  b = new Fl_Toggle_Button(10,y,200,23,"FL_WHEN_&NOT_CHANGED");
  b->callback(toggle_cb, FL_WHEN_NOT_CHANGED); y += 23;
  y += 5;
  b = new Fl_Button(10,y,200,23,"&print changed()");
  b->callback(button_cb);

  b = new Fl_Button(220,y1,100,23,"color"); y1 += 23;
  b->color(input[0]->color()); b->callback(color_cb, (void*)0);
  b->label_color(fl_contrast(FL_BLACK,b->color()));
  b = new Fl_Button(220,y1,100,23,"selection_color"); y1 += 23;
  b->color(input[0]->selection_color()); b->callback(color_cb, (void*)1);
  b->label_color(fl_contrast(FL_BLACK,b->color()));
  b = new Fl_Button(220,y1,100,23,"text_color"); y1 += 23;
  b->color(input[0]->text_color()); b->callback(color_cb, (void*)2);
  b->label_color(fl_contrast(FL_BLACK,b->color()));

  window->resizable(window);
  window->end();
  window->show(argc,argv);
// CET - FIXME - crashes windows  Fl::lock();
  return Fl::run();
}

//
// End of "$Id: input.cxx,v 1.21 2000/05/30 07:42:19 bill Exp $".
//
