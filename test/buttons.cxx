//
// "$Id: buttons.cxx,v 1.20 2004/08/01 22:28:24 spitzak Exp $"
//
// Draws the image for the manual
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
#include <fltk/ToggleButton.h>
#include <fltk/ReturnButton.h>
#include <fltk/RepeatButton.h>
#include <fltk/CheckButton.h>
#include <fltk/RadioButton.h>
#include <fltk/LightButton.h>
#include <fltk/HighlightButton.h>

using namespace fltk;

void rb_cb(Widget*, void*) {
//  Fl::theme("essai");
  reset_theme();
  redraw();
}

const int W = 130;
const int H = 24;
const int B = 10;
const int X0 = B;
const int X1 = (B+W+B);

int main(int argc, char ** argv) {
  Window window(X1+W+B, B+5*(H+B));
  window.begin();

  int Y = B;
  (void) new Button(X0, Y, W, H, "Button");
  ReturnButton* rb = new ReturnButton(X1, Y, W, H, "ReturnButton");
  rb->callback(rb_cb);
  rb->tooltip("Push this to revert style to fltk defaults");
  Y += H+B;
  (void) new ToggleButton(X0, Y, W, H, "ToggleButton");
  (void) new RadioButton(X1, Y, W, H, "RadioButton");
  Y += H+B;
  (void) new RepeatButton(X0, Y, W, H, "RepeatButton");
  (void) new RadioButton(X1, Y, W, H, "RadioButton");
  Y += H+B;
  (void) new LightButton(X0, Y, W, H, "LightButton");
  (void) new RadioButton(X1, Y, W, H, "RadioButton");
  Y += H+B;
  (void) new HighlightButton(X0, Y, W, H, "HighlightButton");
  (void) new CheckButton(X1, Y, W, H, "CheckButton");
  
  window.resizable(window);
  window.end();
  window.show(argc,argv);
  Button::default_style->box_ = fltk::PLASTIC_UP_BOX;
  Button::default_style->color_ = 256-36;
  Widget::default_style->highlight_color(3);
  return run();
}

//
// End of "$Id: buttons.cxx,v 1.20 2004/08/01 22:28:24 spitzak Exp $".
//
