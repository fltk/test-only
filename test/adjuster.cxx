//
// "$Id$"
//
// Adjuster test program for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/run.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Adjuster.h>
#include <fltk3/Box.h>

using namespace fltk3;

void adjcb(Widget *o, void *v) {
  Adjuster *a = (Adjuster*)o;
  Widget *b = (Widget *)v;
  a->format((char *)(b->label()));
  b->redraw();
}

int main(int argc, char ** argv) {
   DoubleWindow window(320,100,argv[0]);

   char buf1[100];
   Widget b1(DOWN_BOX,20,30,80,25,buf1);
   b1.color(WHITE);
   Adjuster a1(20+80,30,3*25,25);
   a1.callback(adjcb,&b1);
   adjcb(&a1,&b1);

   char buf2[100];
   Widget b2(DOWN_BOX,20+80+4*25,30,80,25,buf2);
   b2.color(WHITE);
   Adjuster a2(b2.x()+b2.w(),10,25,3*25);
   a2.callback(adjcb,&b2);
   adjcb(&a2,&b2);

   window.resizable(window);
   window.end();
   window.show(argc, argv);
   return run();
}

//
// End of "$Id$".
//
