//
// "$Id: buttons.cxx,v 1.14 2001/01/02 00:20:28 clip Exp $"
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
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Highlight_Button.H>
#include <FL/Fl_Style.H>

void rb_cb(Fl_Widget*, void*) {
//  Fl::theme("essai");
  Fl_Style::revert();
}

int main(int argc, char ** argv) {
  Fl_Window *window = new Fl_Window(320,130);
  (void) new Fl_Button(10, 10, 130, 24, "Fl_Button");
  Fl_Return_Button* rb = new Fl_Return_Button(150, 10, 130, 24, "Fl_Return_Button");
  rb->callback(rb_cb);
  (void) new Fl_Repeat_Button(10,40,130,24,"Fl_Repeat_Button");
  (void) new Fl_Radio_Button(150,40,130,24,"Fl_Radio_Button");
  (void) new Fl_Radio_Button(150,70,130,24,"Fl_Radio_Button");
  (void) new Fl_Light_Button(10,70,130,24,"Fl_Light_Button");
  (void) new Fl_Check_Button(150,100,130,24,"Fl_Check_Button");
  (void) new Fl_Highlight_Button(10,100,130,24,"Fl_Highlight_Button");
  
  window->resizable(window);
  window->end();
  window->show(argc,argv);
  return Fl::run();
}

//
// End of "$Id: buttons.cxx,v 1.14 2001/01/02 00:20:28 clip Exp $".
//
