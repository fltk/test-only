//
// "$Id: clock.cxx,v 1.8 2000/07/21 00:31:52 clip Exp $"
//
// Clock test program for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Shaped_Window.H>
#include <FL/Fl_Round_Clock.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Item.H>
#include <FL/fl_ask.H>
#include <stdlib.h>

#include "circle.xbm" // bitmap mask for window shape

class ClockWindow : public Fl_Shaped_Window {
  public:
    ClockWindow(int W, int H, const char *l = 0) : Fl_Shaped_Window(W,H,l) {}
    int handle(int);
};

int ClockWindow::handle(int e) {
  static int bx, by;
  static int button1 = 0;
  if (e == FL_PUSH) button1 = (Fl::event_button() == 1);
  if (button1) switch(e) {
    case FL_DRAG:
      position(x()+Fl::event_x_root()-bx, y()+Fl::event_y_root()-by);
    case FL_PUSH:
      bx = Fl::event_x_root(); by = Fl::event_y_root();
      show(); // so the window will click-to-front
      return 1;
  }
  return Fl_Shaped_Window::handle(e);
}

Fl_Item *about_item, *exit_item;
void callback(Fl_Widget*, void* d) {
  if (d == (void*)exit_item) exit(0);
  fl_message("FLTK-- Copyright 2000 Bill Spitzak and others");
}

int main(int argc, char **argv) {
  ClockWindow window(220, 220, "Fl_Round_Clock");
  window.color(FL_BLACK);
  window.clear_border(); // some window managers do this for you!
  Fl_Round_Clock clock(2,2,216,216);
  Fl_Menu_Button popup(0, 0, 220, 220);
  popup.type(Fl_Menu_Button::POPUP3);
  popup.begin();
  about_item = new Fl_Item("About clock");
  exit_item = new Fl_Item("Exit clock");
  popup.end();
  popup.callback(callback);
  // window.resizable(popup); - Not today, maybe never!
  window.end();
  window.xclass("Fl_Clock");
  window.show(argc, argv);
  Fl_Bitmap shape(circle_bits, circle_width, circle_height); // window shape data
  window.shape(shape);

  return Fl::run();
}

//
// End of "$Id: clock.cxx,v 1.8 2000/07/21 00:31:52 clip Exp $".
//
