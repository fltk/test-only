//
// "$Id$"
//
// Another button test program for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/Window.h>
#include <fltk3/Button.h>
#include <fltk3/ReturnButton.h>
#include <fltk3/RepeatButton.h>
#include <fltk3/CheckButton.h>
#include <fltk3/LightButton.h>
#include <fltk3/RoundButton.h>
#include <fltk3/Tooltip.h>

using namespace fltk3;

int main(int argc, char ** argv) {
  Window *window = new Window(320,130);
  window->begin();
  {
    (new Button(10, 10, 130, 30, "Button"))->tooltip("This is a Tooltip.");
    new ReturnButton(150, 10, 160, 30, "ReturnButton");
    new RepeatButton(10,50,130,30,"RepeatButton");
    new LightButton(10,90,130,30,"LightButton");
    new RoundButton(150,50,160,30,"RoundButton");
    new CheckButton(150,90,160,30,"CheckButton");
  }
  window->end();
  window->show(argc,argv);
  return run();
}

//
// End of "$Id$".
//
