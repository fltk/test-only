//
// "$Id$"
//
// Draws the image for the manual
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

#include <fltk/xpmImage.h>
#include "folder_small.xpm"
#include "folder_small2.xpm"
#include "folder_small3.xpm"

using namespace fltk;

Button * abutton=0;

void cb_active_butt(Widget*, void*) {
    static bool flip = true;
    if (flip) abutton->activate(); else abutton->deactivate();
    abutton->label(flip ? "Active" : "Inactive");
    flip = !flip;

  abutton->redraw();
}

void rb_cb(Widget*, void*) {
//  Fl::theme("essai");
  reset_theme();
  redraw();
}

const int W = 150;
const int H = 24;
const int B = 10;
const int X0 = B;
const int X1 = (B+W+B);

int main(int argc, char ** argv) {
  Window window(X1+W+B, B+7*(H+B));
  window.begin();
  
  xpmImage fold1(folder_small);
  xpmImage fold2(folder_small2);
  xpmImage fold3(folder_small3);


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
  // The box of the higlight button will appear when belowmouse
  // when combined with image() as below, you get the border + the image appearing when belowmouse()

  //HighlightButton* hb = 
      new HighlightButton(X0, Y, W, H, "HighlightButton");
  //hb->image(new xpmImage(folder_small),fltk::BELOWMOUSE);

  new CheckButton(X1, Y, W, H, "CheckButton");

  Y += H+B;
  
  Button * b = new Button(X0, Y, W, H, "push/release img");
  b->image(&fold2, 0, 0, &fold3); // use default & pushed img

  b = new Button(X1, Y, W, H, "push/rel noborder");
  b->box(NO_BOX);
  b->image(&fold2, 0, 0, &fold3); // use default & pushed img
  
  Y += H+B;

  b = new HighlightButton(X0, Y, W, H, "Everything !");
  b->image(&fold2,0 ,0, &fold3); // demonstrate the different ways to affect images
  // to remove the  belowmouse changing image comment this line:
  b->image(fold1, fltk::BELOWMOUSE);
  b->callback(cb_active_butt);

  abutton = b = new Button(X1, Y, W, H, "Inactive");
  b->image(&fold3,&fold2);
  b->activate(0);  

  window.resizable(window);
  window.end();
  window.show(argc,argv);
  
  // Default Style handling for changing the scheme of all widget at once
  Button::default_style->box_ = fltk::PLASTIC_UP_BOX;
  Button::default_style->color_ = 256-36;
  Widget::default_style->highlight_color(3);
  return run();
}

//
// End of "$Id$".
//
