//
// "$Id: button.cxx,v 1.9 2004/02/05 07:21:21 spitzak Exp $"
//
// Example2 program from the manual
//
// Copyright 1998-2004 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <stdlib.h>
#include <stdio.h>
#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/Button.h>
using namespace fltk;

void beepcb(Widget *, void *) {
  printf("\007"); fflush(stdout);
}

void exitcb(Widget *, void *) {
  exit(0);
}

int main(int argc, char ** argv) {
  Window *window = new Window(320,65);
  window->begin();
  Button *b1 = new Button(20, 20, 80, 25, "&Beep");
  b1->callback(beepcb,0);
  /*Button *b2 =*/ new Button(120,20, 80, 25, "&no op");
  Button *b3 = new Button(220,20, 80, 25, "E&xit");
  b3->callback(exitcb,0);
  window->end();
  window->show(argc,argv);
  return run();
}

//
// End of "$Id: button.cxx,v 1.9 2004/02/05 07:21:21 spitzak Exp $".
//
