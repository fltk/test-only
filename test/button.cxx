/
// "$Id$
/
// Button/callback test program for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2003 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#include <stdlib.h
#include <stdio.h
#include <FL/Fl.H
#include <FL/Fl_Window.H
#include <FL/Fl_Button.H

void beepcb(Fl_Widget *, void *) 
  printf("\007"); fflush(stdout)


void exitcb(Fl_Widget *, void *) 
  exit(0)


#if 
// test Fl::add_fd()..
void stdin_cb(int, void*) 
  char buf[1000]
  gets(buf)
  printf("stdin callback\n")

#endi

int main(int argc, char ** argv) 
  Fl_Window *window = new Fl_Window(320,65)
  Fl_Button *b1 = new Fl_Button(20, 20, 80, 25, "&Beep")
  b1->callback(beepcb,0)
  /*Fl_Button *b2 =*/ new Fl_Button(120,20, 80, 25, "&no op")
  Fl_Button *b3 = new Fl_Button(220,20, 80, 25, "E&xit")
  b3->callback(exitcb,0)
  window->end()
  window->show(argc,argv)
#if 
  Fl::add_fd(0, stdin_cb)
#endi
  return Fl::run()


/
// End of "$Id$"
/
