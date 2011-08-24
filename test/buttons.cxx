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
#include <fltk3/Box.h>

int main(int argc, char ** argv) {
  fltk3::Window *window = new fltk3::Window(310,250);
  fltk3::Button *b = new fltk3::Button(10, 10, 160, 30, "fltk3::Button");
  b->tooltip("This is a Tooltip.");
  new fltk3::ReturnButton(10, 50, 160, 30, "fltk3::ReturnButton");
  new fltk3::RepeatButton(10,90,160,30,"fltk3::RepeatButton");
  new fltk3::LightButton(10,130,160,30,"fltk3::LightButton");
  new fltk3::RoundButton(10,170,160,30,"fltk3::RoundButton");
  new fltk3::CheckButton(10,210,160,30,"fltk3::CheckButton");

  new fltk3::Box(180,20,120,30,"fltk3::Button");
  { fltk3::Button* o = new fltk3::Button(180, 50, 30, 30, "7");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_BOTTOM));
    o->shortcut('7');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(210, 50, 30, 30, "8");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_BOTTOM|fltk3::TIE_LEFT));
    o->shortcut('8');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(240, 50, 30, 30, "9");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_BOTTOM|fltk3::TIE_LEFT));
    o->shortcut('9');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(180, 80, 30, 30, "4");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_BOTTOM|fltk3::TIE_TOP));
    o->shortcut('4');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(210, 80, 30, 30, "5");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_BOTTOM|fltk3::TIE_LEFT|fltk3::TIE_TOP));
    o->shortcut('5');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(240, 80, 30, 30, "6");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_BOTTOM|fltk3::TIE_LEFT|fltk3::TIE_TOP));
    o->shortcut('6');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(180, 110, 30, 30, "1");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_BOTTOM|fltk3::TIE_TOP));
    o->shortcut('1');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(210, 110, 30, 30, "2");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_BOTTOM|fltk3::TIE_LEFT|fltk3::TIE_TOP));
    o->shortcut('2');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(240, 110, 30, 30, "3");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_BOTTOM|fltk3::TIE_LEFT|fltk3::TIE_TOP));
    o->shortcut('3');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(180, 140, 60, 30, "0");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_TOP));
    o->shortcut('0');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(240, 140, 30, 30, ".");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_RIGHT|fltk3::TIE_LEFT|fltk3::TIE_TOP));
    o->shortcut('.');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(270, 50, 30, 60, "-");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_BOTTOM|fltk3::TIE_LEFT));
    o->shortcut('-');
  } // fltk3::Button* o
  { fltk3::Button* o = new fltk3::Button(270, 110, 30, 60, "+/=");
    o->box(fltk3::Boxtype(fltk3::UP_BOX|fltk3::TIE_LEFT|fltk3::TIE_TOP));
    o->shortcut('=');
  } // fltk3::Button* o
  fltk3::Box *tie = new fltk3::Box(180,170,120,60,"multiple buttons can be tied together visually using the fltk3::TIE_... flags with the box type");
  tie->align(fltk3::ALIGN_WRAP);
  tie->labelsize(11);
  
  window->end();
  window->show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
