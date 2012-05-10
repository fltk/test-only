//
// "$Id$"
//
// fltk3::ScrollGroup test program for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/ScrollGroup.h>
#include <fltk3/LightButton.h>
#include <fltk3/Choice.h>
#include <fltk3/Box.h>
#include <string.h>
#include <stdio.h>
#include <fltk3/draw.h>
#include <fltk3/math.h>

class Drawing : public fltk3::Widget {
  void draw();
public:
  Drawing(int X,int Y,int W,int H,const char* L) : fltk3::Widget(X,Y,W,H,L) {
    align(fltk3::ALIGN_TOP);
    box(fltk3::FLAT_BOX);
    color(fltk3::WHITE);
  }
};

void Drawing::draw() {
  draw_box();
  fltk3::push_matrix();
  fltk3::translate(w()/2, h()/2);
  fltk3::scale(w()/2, h()/2);
  fltk3::color(fltk3::BLACK);
  for (int i = 0; i < 20; i++) {
    for (int j = i+1; j < 20; j++) {
      fltk3::begin_line();
      fltk3::vertex(cos(M_PI*i/10+.1), sin(M_PI*i/10+.1));
      fltk3::vertex(cos(M_PI*j/10+.1), sin(M_PI*j/10+.1));
      fltk3::end_line();
    }
  }
  fltk3::pop_matrix();
}

fltk3::ScrollGroup* thescroll;

void box_cb(fltk3::Widget* o, void*) {
  thescroll->box(((fltk3::Button*)o)->value() ? fltk3::DOWN_FRAME : fltk3::NO_BOX);
  thescroll->redraw();
}

void type_cb(fltk3::Widget*, void* v) {
  thescroll->type((uchar)((fl_intptr_t)v));
  thescroll->redraw();
}

fltk3::MenuItem choices[] = {
  {"0", 0, type_cb, (void*)0},
  {"HORIZONTAL", 0, type_cb, (void*)fltk3::ScrollGroup::HORIZONTAL},
  {"VERTICAL", 0, type_cb, (void*)fltk3::ScrollGroup::VERTICAL},
  {"BOTH", 0, type_cb, (void*)fltk3::ScrollGroup::BOTH},
  {"HORIZONTAL_ALWAYS", 0, type_cb, (void*)fltk3::ScrollGroup::HORIZONTAL_ALWAYS},
  {"VERTICAL_ALWAYS", 0, type_cb, (void*)fltk3::ScrollGroup::VERTICAL_ALWAYS},
  {"BOTH_ALWAYS", 0, type_cb, (void*)fltk3::ScrollGroup::BOTH_ALWAYS},
  {0}
};

void align_cb(fltk3::Widget*, void* v) {
  thescroll->scrollbar.align((uchar)((fl_intptr_t)v));
  thescroll->redraw();
}

fltk3::MenuItem align_choices[] = {
  {"left+top", 0, align_cb, (void*)(fltk3::ALIGN_LEFT+fltk3::ALIGN_TOP)},
  {"left+bottom", 0, align_cb, (void*)(fltk3::ALIGN_LEFT+fltk3::ALIGN_BOTTOM)},
  {"right+top", 0, align_cb, (void*)(fltk3::ALIGN_RIGHT+fltk3::ALIGN_TOP)},
  {"right+bottom", 0, align_cb, (void*)(fltk3::ALIGN_RIGHT+fltk3::ALIGN_BOTTOM)},
  {0}
};

int main(int argc, char** argv) {
  fltk3::Window window(5*75,400);
  window.box(fltk3::NO_BOX);
  fltk3::ScrollGroup scroll(0,0,5*75,300);

  int n = 0;
  for (int y=0; y<16; y++) for (int x=0; x<5; x++) {
    char buf[20]; sprintf(buf,"%d",n++);
    fltk3::Button* b = new fltk3::Button(x*75,y*25+(y>=8?5*75:0),75,25);
    b->copy_label(buf);
    b->color(n);
    b->labelcolor(fltk3::WHITE);
  }
  Drawing drawing(0,8*25,5*75,5*75,0);
  scroll.end();
  window.resizable(scroll);

  fltk3::Box box(0,300,5*75,window.h()-300); // gray area below the scroll
  box.box(fltk3::FLAT_BOX);

  fltk3::LightButton but1(150, 310, 200, 25, "box");
  but1.callback(box_cb);
  
  fltk3::Choice choice(150, 335, 200, 25, "type():");
  choice.menu(choices);
  choice.value(3);

  fltk3::Choice achoice(150, 360, 200, 25, "scrollbar.align():");
  achoice.menu(align_choices);
  achoice.value(3);

  thescroll = &scroll;

  //scroll.box(fltk3::DOWN_BOX);
  //scroll.type(fltk3::ScrollGroup::VERTICAL);
  window.end();
  window.show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
