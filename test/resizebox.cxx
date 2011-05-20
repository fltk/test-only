//
// "$Id$"
//
// Resize box test program for the Fast Light Tool Kit (FLTK).
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

#define W1 (big ? 60 : 40)
#define B 0
#define W3 (5*W1+6*B)

#include <fltk3/run.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Box.h>
#include <fltk3/RadioButton.h>
#include <fltk3/draw.h>
#include <fltk3/message.h>

fltk3::DoubleWindow *window;
fltk3::Box *box;

int big = 0;

void b_cb(fltk3::Widget *,long w) {
  if (window->w() != W3 || window->h() != W3) {
    fltk3::message("Put window back to minimum size before changing");
    return;
  }
  window->init_sizes();
  switch (w) {
  case 0: box->hide(); window->box(fltk3::FLAT_BOX); window->resizable(0); return;
  case 8: box->resize(W1+B,W1,2*W1,B); break;
  case 2: box->resize(W1+B,W1+B+2*W1,2*W1,B); break;
  case 4: box->resize(W1+B,W1,B,2*W1); break;
  case 6: box->resize(W1+B+2*W1,W1+B,B,2*W1); break;
  }
  window->box(fltk3::NO_BOX);
  if (w == 6 || w == 4)
    box->label("re\nsiz\nab\nle");
  else box->label("resizable");
  box->show();
  window->resizable(box);
  window->redraw();
}

int main(int argc, char **argv) {
  window = new fltk3::DoubleWindow(W3,W3);
  window->box(fltk3::NO_BOX);
  fltk3::Box *n;
  for (int x = 0; x<4; x++) for (int y = 0; y<4; y++) {
    if ((x==1 || x==2) && (y==1 || y==2)) continue;
    n = new fltk3::Box(fltk3::FRAME_BOX,x*(B+W1)+B,y*(B+W1)+B,W1,W1,0);
    n->color(x+y+8);
  }
  n = new fltk3::Box(fltk3::FRAME_BOX,B,4*W1+5*B,4*W1+3*B,W1,0);
  n->color(12);
  n = new fltk3::Box(fltk3::FRAME_BOX,4*W1+5*B,B,W1,5*W1+4*B,0);
  n->color(13);
  n = new fltk3::Box(fltk3::FRAME_BOX,W1+B+B,W1+B+B,2*W1+B,2*W1+B,0);
  n->color(8);

  fltk3::Button *b = new fltk3::RadioButton(W1+B+50,W1+B+30,20,20,"@6>");
  b->callback(b_cb,6);
  (new fltk3::RadioButton(W1+B+30,W1+B+10,20,20,"@8>"))->callback(b_cb,8);
  (new fltk3::RadioButton(W1+B+10,W1+B+30,20,20,"@4>"))->callback(b_cb,4);
  (new fltk3::RadioButton(W1+B+30,W1+B+50,20,20,"@2>"))->callback(b_cb,2);
  (new fltk3::RadioButton(W1+B+30,W1+B+30,20,20,"off"))->callback(b_cb,0);

  box = new fltk3::Box(fltk3::FLAT_BOX,0,0,0,0,"resizable");
  box->color(fltk3::DARK2);
  b->set();
  b->do_callback();
  window->end();

  window->size_range(W3,W3);
  window->show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
