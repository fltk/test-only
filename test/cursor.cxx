//
// "$Id$"
//
// Cursor test program for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/HorValueSlider.h>
#include <fltk3/Choice.h>
#include <fltk3/draw.h>
#include <fltk3/Box.h>

fltk3::Color fg = fltk3::BLACK;
fltk3::Color bg = fltk3::WHITE;
fltk3::Cursor cursor = fltk3::CURSOR_DEFAULT;

fltk3::HorValueSlider *cursor_slider;

void choice_cb(fltk3::Widget *, void *v) {
  cursor = (fltk3::Cursor)(fl_intptr_t)v;
  cursor_slider->value(cursor);
  fltk3::cursor(cursor,fg,bg);
}

fltk3::MenuItem choices[] = {
  {"fltk3::CURSOR_DEFAULT",0,choice_cb,(void*)fltk3::CURSOR_DEFAULT},
  {"fltk3::CURSOR_ARROW",0,choice_cb,(void*)fltk3::CURSOR_ARROW},
  {"fltk3::CURSOR_CROSS",0,choice_cb,(void*)fltk3::CURSOR_CROSS},
  {"fltk3::CURSOR_WAIT",0,choice_cb,(void*)fltk3::CURSOR_WAIT},
  {"fltk3::CURSOR_INSERT",0,choice_cb,(void*)fltk3::CURSOR_INSERT},
  {"fltk3::CURSOR_HAND",0,choice_cb,(void*)fltk3::CURSOR_HAND},
  {"fltk3::CURSOR_HELP",0,choice_cb,(void*)fltk3::CURSOR_HELP},
  {"fltk3::CURSOR_MOVE",0,choice_cb,(void*)fltk3::CURSOR_MOVE},
  {"fltk3::CURSOR_NS",0,choice_cb,(void*)fltk3::CURSOR_NS},
  {"fltk3::CURSOR_WE",0,choice_cb,(void*)fltk3::CURSOR_WE},
  {"fltk3::CURSOR_NWSE",0,choice_cb,(void*)fltk3::CURSOR_NWSE},
  {"fltk3::CURSOR_NESW",0,choice_cb,(void*)fltk3::CURSOR_NESW},
  {"fltk3::CURSOR_NONE",0,choice_cb,(void*)fltk3::CURSOR_NONE},
#if 0
  {"fltk3::CURSOR_N",0,choice_cb,(void*)fltk3::CURSOR_N},
  {"fltk3::CURSOR_NE",0,choice_cb,(void*)fltk3::CURSOR_NE},
  {"fltk3::CURSOR_E",0,choice_cb,(void*)fltk3::CURSOR_E},
  {"fltk3::CURSOR_SE",0,choice_cb,(void*)fltk3::CURSOR_SE},
  {"fltk3::CURSOR_S",0,choice_cb,(void*)fltk3::CURSOR_S},
  {"fltk3::CURSOR_SW",0,choice_cb,(void*)fltk3::CURSOR_SW},
  {"fltk3::CURSOR_W",0,choice_cb,(void*)fltk3::CURSOR_W},
  {"fltk3::CURSOR_NW",0,choice_cb,(void*)fltk3::CURSOR_NW},
#endif
  {0}
};

void setcursor(fltk3::Widget *o, void *) {
  fltk3::HorValueSlider *slider = (fltk3::HorValueSlider *)o;
  cursor = fltk3::Cursor((int)slider->value());
  fltk3::cursor(cursor,fg,bg);
}

void setfg(fltk3::Widget *o, void *) {
  fltk3::HorValueSlider *slider = (fltk3::HorValueSlider *)o;
  fg = fltk3::Color((int)slider->value());
  fltk3::cursor(cursor,fg,bg);
}

void setbg(fltk3::Widget *o, void *) {
  fltk3::HorValueSlider *slider = (fltk3::HorValueSlider *)o;
  bg = fltk3::Color((int)slider->value());
  fltk3::cursor(cursor,fg,bg);
}

// draw the label without any ^C or \nnn conversions:
class CharBox : public fltk3::Box {
  void draw() {
    fltk3::font(fltk3::FREE_FONT,14);
    fltk3::draw(label(), x()+w()/2, y()+h()/2);
  }
public:
  CharBox(int X, int Y, int W, int H, const char* L) : fltk3::Box(X,Y,W,H,L) {}
};

int main(int argc, char **argv) {
  fltk3::DoubleWindow window(400,300);

  fltk3::Choice choice(80,100,200,25,"Cursor:");
  choice.menu(choices);
  choice.callback(choice_cb);
  choice.when(fltk3::WHEN_RELEASE|fltk3::WHEN_NOT_CHANGED);

  fltk3::HorValueSlider slider1(80,180,310,30,"Cursor:");
  cursor_slider = &slider1;
  slider1.align(fltk3::ALIGN_LEFT);
  slider1.step(1);
  slider1.precision(0);
  slider1.bounds(0,100);
  slider1.value(0);
  slider1.callback(setcursor);
  slider1.value(cursor);

  fltk3::HorValueSlider slider2(80,220,310,30,"fgcolor:");
  slider2.align(fltk3::ALIGN_LEFT);
  slider2.step(1);
  slider2.precision(0);
  slider2.bounds(0,255);
  slider2.value(0);
  slider2.callback(setfg);
  slider2.value(fg);

  fltk3::HorValueSlider slider3(80,260,310,30,"bgcolor:");
  slider3.align(fltk3::ALIGN_LEFT);
  slider3.step(1);
  slider3.precision(0);
  slider3.bounds(0,255);
  slider3.value(0);
  slider3.callback(setbg);
  slider3.value(bg);

#if 0
  // draw the manual's diagram of cursors...
  window.size(400,800);
  int y = 300;
  fltk3::set_font(fltk3::FREE_FONT, "cursor");
  char buf[100]; char *p = buf;
  for (fltk3::Menu* m = choices; m->label(); m++) {
    fltk3::Box* b = new fltk3::Box(35,y,150,25,m->label());
    b->align(fltk3::ALIGN_LEFT|fltk3::ALIGN_INSIDE);
    int n = (int)(m->argument());
    if (n == fltk3::CURSOR_NONE) break;
    if (n == fltk3::CURSOR_DEFAULT) n = fltk3::CURSOR_ARROW;
    p[0] = (char)((n-1)*2);
    p[1] = 0;
    b = new CharBox(15,y,20,20,p); p+=2;
    y += 25;
  }
#endif

  window.resizable(window);
  window.end();
  window.show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
