//
// "$Id: buttons.cxx,v 1.5 1999/03/14 06:46:44 carl Exp $"
//
// Another button test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <stdlib.h>
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Fly_Button.H>

int main(int argc, char ** argv) {
  Fl_Window *window = new Fl_Window(320,170);
  (void) new Fl_Button(10, 10, 130, 30, "Fl_Button");
  (void) new Fl_Return_Button(150, 10, 160, 30, "Fl_Return_Button");
  (void) new Fl_Repeat_Button(10,50,130,30,"Fl_Repeat_Button");
  (void) new Fl_Round_Button(150,50,160,30,"Fl_Round_Button");
  (void) new Fl_Light_Button(10,90,130,30,"Fl_Light_Button");
  (void) new Fl_Check_Button(150,90,160,30,"Fl_Check_Button");
  (void) new Fl_Fly_Button(10,130,130,30,"Fl_Fly_Button");

  window->end();
  window->show(argc,argv);
  return Fl::run();
}

//
// End of "$Id: buttons.cxx,v 1.5 1999/03/14 06:46:44 carl Exp $".
//
