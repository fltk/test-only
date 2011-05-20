//
// "$Id$"
//
// Boxtype test program for the Fast Light Tool Kit (FLTK).
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

#include <stdlib.h>
#include <stdio.h>
#include <fltk3/run.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Box.h>

int N = 0;
#define W 200
#define H 50
#define ROWS 14

fltk3::DoubleWindow *window;

void bt(const char *name, fltk3::Boxtype type, int square=0) {
  int x = N%4;
  int y = N/4;
  N++;
  x = x*W+10;
  y = y*H+10;
  fltk3::Box *b = new fltk3::Box(type,x,y,square ? H-20 : W-20,H-20,name);
  b->labelsize(11);
  if (square) b->align(fltk3::ALIGN_RIGHT);
}

int main(int argc, char ** argv) {
  window = new fltk3::DoubleWindow(4*W,ROWS*H);
  window->box(fltk3::FLAT_BOX);
#if 0 // this code uses the command line arguments to set arbitrary color schemes
  fltk3::args(argc, argv);
  fltk3::get_system_colors();
#elif 0 // this code uses a single color to define a scheme
  fltk3::args(argc, argv);
  fltk3::get_system_colors();
  fltk3::background(113,113,198);
#else // this code uses the nice bright blue background to show box vs. frame types
  fltk3::args(argc, argv);
  fltk3::get_system_colors();
  window->color(12);// light blue
#endif
  bt("fltk3::NO_BOX",fltk3::NO_BOX);
  bt("fltk3::FLAT_BOX",fltk3::FLAT_BOX);
  N += 2; // go to start of next row to line up boxes & frames
  bt("fltk3::UP_BOX",fltk3::UP_BOX);
  bt("fltk3::DOWN_BOX",fltk3::DOWN_BOX);
  bt("fltk3::UP_FRAME",fltk3::UP_FRAME);
  bt("fltk3::DOWN_FRAME",fltk3::DOWN_FRAME);
  bt("fltk3::THIN_UP_BOX",fltk3::THIN_UP_BOX);
  bt("fltk3::THIN_DOWN_BOX",fltk3::THIN_DOWN_BOX);
  bt("fltk3::THIN_UP_FRAME",fltk3::THIN_UP_FRAME);
  bt("fltk3::THIN_DOWN_FRAME",fltk3::THIN_DOWN_FRAME);
  bt("fltk3::ENGRAVED_BOX",fltk3::ENGRAVED_BOX);
  bt("fltk3::EMBOSSED_BOX",fltk3::EMBOSSED_BOX);
  bt("fltk3::ENGRAVED_FRAME",fltk3::ENGRAVED_FRAME);
  bt("fltk3::EMBOSSED_FRAME",fltk3::EMBOSSED_FRAME);
  bt("fltk3::BORDER_BOX",fltk3::BORDER_BOX);
  bt("fltk3::SHADOW_BOX",fltk3::SHADOW_BOX);
  bt("fltk3::BORDER_FRAME",fltk3::BORDER_FRAME);
  bt("fltk3::SHADOW_FRAME",fltk3::SHADOW_FRAME);
  bt("fltk3::ROUNDED_BOX",fltk3::ROUNDED_BOX);
  bt("fltk3::RSHADOW_BOX",fltk3::RSHADOW_BOX);
  bt("fltk3::ROUNDED_FRAME",fltk3::ROUNDED_FRAME);
  bt("fltk3::RFLAT_BOX",fltk3::RFLAT_BOX);
  bt("fltk3::OVAL_BOX",fltk3::OVAL_BOX);
  bt("fltk3::OSHADOW_BOX",fltk3::OSHADOW_BOX);
  bt("fltk3::OVAL_FRAME",fltk3::OVAL_FRAME);
  bt("fltk3::OFLAT_BOX",fltk3::OFLAT_BOX);
  bt("fltk3::ROUND_UP_BOX",fltk3::ROUND_UP_BOX);
  bt("fltk3::ROUND_DOWN_BOX",fltk3::ROUND_DOWN_BOX);
  bt("fltk3::DIAMOND_UP_BOX",fltk3::DIAMOND_UP_BOX);
  bt("fltk3::DIAMOND_DOWN_BOX",fltk3::DIAMOND_DOWN_BOX);

  bt("fltk3::PLASTIC_UP_BOX",fltk3::PLASTIC_UP_BOX);
  bt("fltk3::PLASTIC_DOWN_BOX",fltk3::PLASTIC_DOWN_BOX);
  bt("fltk3::PLASTIC_UP_FRAME",fltk3::PLASTIC_UP_FRAME);
  bt("fltk3::PLASTIC_DOWN_FRAME",fltk3::PLASTIC_DOWN_FRAME);
  bt("fltk3::PLASTIC_THIN_UP_BOX",fltk3::PLASTIC_THIN_UP_BOX);
  bt("fltk3::PLASTIC_THIN_DOWN_BOX",fltk3::PLASTIC_THIN_DOWN_BOX);
  N += 2;
  bt("fltk3::PLASTIC_ROUND_UP_BOX",fltk3::PLASTIC_ROUND_UP_BOX);
  bt("fltk3::PLASTIC_ROUND_DOWN_BOX",fltk3::PLASTIC_ROUND_DOWN_BOX);
  N += 2;

  bt("fltk3::GTK_UP_BOX",fltk3::GTK_UP_BOX);
  bt("fltk3::GTK_DOWN_BOX",fltk3::GTK_DOWN_BOX);
  bt("fltk3::GTK_UP_FRAME",fltk3::GTK_UP_FRAME);
  bt("fltk3::GTK_DOWN_FRAME",fltk3::GTK_DOWN_FRAME);
  bt("fltk3::GTK_THIN_UP_BOX",fltk3::GTK_THIN_UP_BOX);
  bt("fltk3::GTK_THIN_DOWN_BOX",fltk3::GTK_THIN_DOWN_BOX);
  bt("fltk3::GTK_THIN_UP_FRAME",fltk3::GTK_THIN_UP_FRAME);
  bt("fltk3::GTK_THIN_DOWN_FRAME",fltk3::GTK_THIN_DOWN_FRAME);
  bt("fltk3::GTK_ROUND_UP_BOX",fltk3::GTK_ROUND_UP_BOX);
  bt("fltk3::GTK_ROUND_DOWN_BOX",fltk3::GTK_ROUND_DOWN_BOX);
  window->resizable(window);
  window->end();
  window->show();
  return fltk3::run();
}

//
// End of "$Id$".
//
