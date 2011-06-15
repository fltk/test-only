//
// "$Id: buttons.cxx 7903 2010-11-28 21:06:39Z matt $"
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
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Tooltip.H>

int main(int argc, char **argv)
{
  Fl_Window *w = new Fl_Window(0,0,100,100);
  
  Fl_Group *flow_panel = new Fl_Group(2,2,100,100);
  Fl_Button *button1 = new Fl_Button(2,2,80,40,"Hello");
  Fl_Window *window = new Fl_Window(2,2,300,300);
  
  Fl_Group::current(NULL);
  
  flow_panel->begin();
  flow_panel->add(button1);
  flow_panel->end();
  
  window->begin();
  window->add(flow_panel);
  window->end();
  window->show();
  flow_panel->show();
  button1->show();
  
  Fl::run();
  
  return 0;
}
