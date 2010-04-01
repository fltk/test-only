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
#include <fltk3/DoubleBufferWindow.h>
#include <fltk3/Widget.h>

using namespace fltk3;

int N = 0;
#define W 200
#define H 50
#define ROWS 14

DoubleBufferWindow *window;

void bt(const char *name, Boxtype type, int square=0) {
  int x = N%4;
  int y = N/4;
  N++;
  x = x*W+10;
  y = y*H+10;
  Widget *b = new Widget(x,y,square ? H-20 : W-20,H-20,name);
  b->box(type);
  b->labelsize(11);
  if (square) b->align(ALIGN_RIGHT);
}

int main(int argc, char ** argv) {
  window = new DoubleBufferWindow(4*W,ROWS*H,"boxtype test for FLTK 3");
  window->box(FLAT_BOX);
  window->begin();
  args(argc, argv);
  get_system_colors();
  window->color(12);// light blue
  bt("NO_BOX",NO_BOX);
  bt("FLAT_BOX",FLAT_BOX);
  N += 2; // go to start of next row to line up boxes & frames
  bt("UP_BOX",UP_BOX);
  bt("DOWN_BOX",DOWN_BOX);
  bt("UP_FRAME",UP_FRAME);
  bt("DOWN_FRAME",DOWN_FRAME);
  bt("THIN_UP_BOX",THIN_UP_BOX);
  bt("THIN_DOWN_BOX",THIN_DOWN_BOX);
  bt("THIN_UP_FRAME",THIN_UP_FRAME);
  bt("THIN_DOWN_FRAME",THIN_DOWN_FRAME);
  bt("ENGRAVED_BOX",ENGRAVED_BOX);
  bt("EMBOSSED_BOX",EMBOSSED_BOX);
  bt("ENGRAVED_FRAME",ENGRAVED_FRAME);
  bt("EMBOSSED_FRAME",EMBOSSED_FRAME);
  bt("BORDER_BOX",BORDER_BOX);
  bt("SHADOW_BOX",SHADOW_BOX);
  bt("BORDER_FRAME",BORDER_FRAME);
  bt("SHADOW_FRAME",SHADOW_FRAME);
  bt("ROUNDED_BOX",ROUNDED_BOX);
  bt("RSHADOW_BOX",RSHADOW_BOX);
  bt("ROUNDED_FRAME",ROUNDED_FRAME);
  bt("RFLAT_BOX",RFLAT_BOX);
  bt("OVAL_BOX",OVAL_BOX);
  bt("OSHADOW_BOX",OSHADOW_BOX);
  bt("OVAL_FRAME",OVAL_FRAME);
  bt("OFLAT_BOX",OFLAT_BOX);
  bt("ROUND_UP_BOX",ROUND_UP_BOX);
  bt("ROUND_DOWN_BOX",ROUND_DOWN_BOX);
  bt("DIAMOND_UP_BOX",DIAMOND_UP_BOX);
  bt("DIAMOND_DOWN_BOX",DIAMOND_DOWN_BOX);

  bt("PLASTIC_UP_BOX",PLASTIC_UP_BOX);
  bt("PLASTIC_DOWN_BOX",PLASTIC_DOWN_BOX);
  bt("PLASTIC_UP_FRAME",PLASTIC_UP_FRAME);
  bt("PLASTIC_DOWN_FRAME",PLASTIC_DOWN_FRAME);
  bt("PLASTIC_THIN_UP_BOX",PLASTIC_THIN_UP_BOX);
  bt("PLASTIC_THIN_DOWN_BOX",PLASTIC_THIN_DOWN_BOX);
  N += 2;
  bt("PLASTIC_ROUND_UP_BOX",PLASTIC_ROUND_UP_BOX);
  bt("PLASTIC_ROUND_DOWN_BOX",PLASTIC_ROUND_DOWN_BOX);
  N += 2;

  bt("GTK_UP_BOX",GTK_UP_BOX);
  bt("GTK_DOWN_BOX",GTK_DOWN_BOX);
  bt("GTK_UP_FRAME",GTK_UP_FRAME);
  bt("GTK_DOWN_FRAME",GTK_DOWN_FRAME);
  bt("GTK_THIN_UP_BOX",GTK_THIN_UP_BOX);
  bt("GTK_THIN_DOWN_BOX",GTK_THIN_DOWN_BOX);
  bt("GTK_THIN_UP_FRAME",GTK_THIN_UP_FRAME);
  bt("GTK_THIN_DOWN_FRAME",GTK_THIN_DOWN_FRAME);
  bt("GTK_ROUND_UP_BOX",GTK_ROUND_UP_BOX);
  bt("GTK_ROUND_DOWN_BOX",GTK_ROUND_DOWN_BOX);
  window->resizable(window);
  window->end();
  window->show();
  return run();
}

//
// End of "$Id$".
//
