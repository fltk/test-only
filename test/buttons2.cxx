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
// fltk123: #include <fltk/ToggleButton.h>
#include <fltk/ReturnButton.h>
#include <fltk/RepeatButton.h>
// fltk123: #include <fltk/CheckButton.h>
// fltk123: #include <fltk/RadioButton.h>
// fltk123: #include <fltk/LightButton.h>
// fltk123: #include <fltk/HighlightButton.h>
// fltk123: #include <fltk/MultiImage.h>
// fltk123: #include <fltk/xpmImage.h>
// fltk123: #include <fltk/SharedImage.h>

// fltk123: #include <pixmaps/folder_small.xpm>
// fltk123: #include <pixmaps/folder_small2.xpm>
// fltk123: #include <pixmaps/folder_small3.xpm>

using namespace fltk;

Button * abutton=0;
// fltk123: MultiImage* multi;

void cb_active_butt(Widget*, void*) {
    // fltk123: static bool flip = true;
    // fltk123: if (flip) abutton->activate(); else abutton->deactivate();
    // fltk123: abutton->label(flip ? "Active" : "Inactive");
    // fltk123: flip = !flip;

  // fltk123: abutton->redraw();
}

void rb_cb(Widget*, void*) {
  // fltk123: static bool flip = false;
  // fltk123: if (flip) { //  Fl::theme("essai");
      // fltk123: reset_theme();
  // fltk123: } else {
    // Default Style handling for changing the scheme of all widget at once
    // fltk123: Button::default_style->box_ = fltk::PLASTIC_UP_BOX;
    // fltk123: Button::default_style->color_ = 256-36;
    // fltk123: Widget::default_style->highlight_color(3);
  // fltk123: }
  // fltk123: flip = !flip;
  // fltk123: redraw();
}

const int W = 150;
const int H = 24;
const int B = 10;
const int X0 = B;
const int X1 = (B+W+B);

int main(int argc, char ** argv) {
  Window window(X1+W+B, B+7*(H+B));
  window.begin();
  
  // fltk123: xpmImage fold1(folder_small);
  // fltk123: xpmImage fold3(folder_small3);

  // fltk123: xpmImage fold2(folder_small2);
#if !defined(TESTIMAGES)
  // fltk123: xpmImage ifold2(folder_small2); // use closed folder 2 as inactive image
#else
  // fltk123: register_images();
  // fltk123: SharedImage& ifold2 = *SharedImage::get("images/testimg2.jpg");
  //SharedImage& ifold2 = *SharedImage::get("images/coucou.png"); 
#endif
  // ifold2.inactive();
  // WAS: inactive() is nyi but should not be a problem with new Image...
  // fltk123: MultiImage push_release_img(fold2, PUSHED, fold3);
  // fltk123: MultiImage push_release_hlt_img(fold2, HIGHLIGHT, fold1, PUSHED, fold3);
  
  int Y = B;
  (void) new Button(X0, Y, W, H, "Button");
  ReturnButton* rb = new ReturnButton(X1, Y, W, H, "Flip theme");
  rb->callback(rb_cb);
  rb->tooltip("Push this to revert style to fltk defaults");
  Y += H+B;
  // fltk123: (void) new ToggleButton(X0, Y, W, H, "ToggleButton");
  // fltk123: (void) new RadioButton(X1, Y, W, H, "RadioButton");
  Y += H+B;
  (void) new RepeatButton(X0, Y, W, H, "RepeatButton");
  // fltk123: (void) new RadioButton(X1, Y, W, H, "RadioButton");
  Y += H+B;
  // fltk123: (void) new LightButton(X0, Y, W, H, "LightButton");
  // fltk123: (void) new RadioButton(X1, Y, W, H, "RadioButton");
  Y += H+B;
  // The box of the higlight button will appear when belowmouse
  // when combined with image() as below, you get the border + the image appearing when belowmouse()

  //HighlightButton* hb = 
    // fltk123: new HighlightButton(X0, Y, W, H, "HighlightButton");

  // fltk123: new CheckButton(X1, Y, W, H, "CheckButton");

  Y += H+B;
  Button * b = new Button(X0, Y, W, H, "push/release img");
  // fltk123: b->image(push_release_img); // use default & pushed img
  b = new Button(X1, Y, W, H, "push/rel noborder");
  // fltk123: b->image(push_release_img); // use default & pushed img
  // fltk123: b->box(NO_BOX);
  
  Y += H+B;
  // fltk123: b = new HighlightButton(X0, Y, W, H, "Everything !");
  // to remove the  belowmouse changing image comment this line:
  // fltk123: b->image(push_release_hlt_img);
  b->callback(cb_active_butt);
#if !defined(TESTIMAGES)
  // fltk123: abutton = b = new Button(X1, Y, W, H, "Inactive");
  // fltk123: multi = new MultiImage(fold2,  fltk::PUSHED, fold3,fltk::INACTIVE, ifold2);
  // fltk123: b->image(multi); 
#else
  // fltk123: abutton = b = new Button(X1, Y, W, H*2, "Inactive");
  // fltk123: multi = new MultiImage(fold2,  fltk::PUSHED, fold3,fltk::INACTIVE, ifold2);
  // fltk123: b->image(&fold2,&ifold2);
#endif
  // fltk123: b->activate(0);  

  window.resizable(window);
  window.end();
  window.show(argc,argv);

  rb_cb(0,0);  // Default Style handling for changing the scheme of all widget at once

  return run();
}

//
// End of "$Id$".
//
