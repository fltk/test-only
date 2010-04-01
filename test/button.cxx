//
// "$Id$"
//
// Button/callback test program for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/ask.h>
#include <fltk3/Window.h>
#include <fltk3/Button.h>

using namespace fltk3;

void beepcb(Widget *, void *) {
  beep();
}

void exitcb(Widget *, void *) {
  exit(0);
}

int main(int argc, char ** argv) {
  Window *window = new Window(320,65);
  window->begin();
  Button *b1 = new Button(20, 20, 80, 25, "&Beep");
  b1->callback(beepcb,0);
  new Button(120,20, 80, 25, "&no op");
  Button *b3 = new Button(220,20, 80, 25, "E&xit");
  b3->callback(exitcb,0);
  window->end();
  window->show(argc,argv);
  return run();
}

//
// End of "$Id$".
//
