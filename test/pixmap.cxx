//
// "$Id$"
//
// Pixmap label test program for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/Button.h>
#include <fltk3/Pixmap.h>
#include <stdio.h>

#include "pixmaps/porsche.xpm"

#include <fltk3/ToggleButton.h>

fltk3::ToggleButton *leftb,*rightb,*topb,*bottomb,*insideb,*overb,*inactb;
fltk3::Button *b;
fltk3::DoubleWindow *w;

void button_cb(fltk3::Widget *,void *) {
  int i = 0;
  if (leftb->value()) i |= fltk3::ALIGN_LEFT;
  if (rightb->value()) i |= fltk3::ALIGN_RIGHT;
  if (topb->value()) i |= fltk3::ALIGN_TOP;
  if (bottomb->value()) i |= fltk3::ALIGN_BOTTOM;
  if (insideb->value()) i |= fltk3::ALIGN_INSIDE;
  if (overb->value()) i |= fltk3::ALIGN_TEXT_OVER_IMAGE;
  b->align(i);
  if (inactb->value()) b->deactivate();
  else b->activate();
  w->redraw();
}

int dvisual = 0;
int arg(int, char **argv, int &i) {
  if (argv[i][1] == '8') {dvisual = 1; i++; return 1;}
  return 0;
}

#include <fltk3/MultiLabel.h>

int main(int argc, char **argv) {
  int i = 1;
  if (fltk3::args(argc,argv,i,arg) < argc)
    fltk3::fatal(" -8 # : use default visual\n%s\n",fltk3::help);

  fltk3::DoubleWindow window(400,400); ::w = &window;
  fltk3::Button b(140,160,120,120,"Pixmap"); ::b = &b;
  fltk3::Pixmap *pixmap = new fltk3::Pixmap(porsche_xpm);
  fltk3::Pixmap *depixmap;
  depixmap = (fltk3::Pixmap *)pixmap->copy();
  depixmap->inactive();

  b.image(pixmap);
  b.deimage(depixmap);

  leftb = new fltk3::ToggleButton(25,50,50,25,"left");
  leftb->callback(button_cb);
  rightb = new fltk3::ToggleButton(75,50,50,25,"right");
  rightb->callback(button_cb);
  topb = new fltk3::ToggleButton(125,50,50,25,"top");
  topb->callback(button_cb);
  bottomb = new fltk3::ToggleButton(175,50,50,25,"bottom");
  bottomb->callback(button_cb);
  insideb = new fltk3::ToggleButton(225,50,50,25,"inside");
  insideb->callback(button_cb);
  overb = new fltk3::ToggleButton(25,75,100,25,"text over");
  overb->callback(button_cb);
  inactb = new fltk3::ToggleButton(125,75,100,25,"inactive");
  inactb->callback(button_cb);
  if (!dvisual) fltk3::visual(fltk3::RGB);
  window.resizable(window);
  window.end();
  window.show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
