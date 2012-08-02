//
// "$Id$"
//
// Spinner widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#  include <fltk3/Spinner.h>
#  include <fltk3/Button.h>
#  include <fltk3/Box.h>
#  include <fltk3/draw.h>
#  include <stdio.h>
#  include <stdlib.h>


fltk3::Spinner::Spinner(int X, int Y, int W, int H, const char *L)
: fltk3::Group(X, Y, W, H, L),
  input_(0, 0, W - H / 2 - 2, H),
  up_button_(W - H / 2 - 2, 0, H / 2 + 2, H / 2, ""),
  down_button_(W - H / 2 - 2, H - H / 2, H / 2 + 2, H / 2, "") 
{
  end();
  
  value_   = 1.0;
  minimum_ = 1.0;
  maximum_ = 100.0;
  step_    = 1.0;
  format_  = "%g";
  
  align(fltk3::ALIGN_LEFT);
  
  input_.value("1");
  input_.type(fltk3::INT_INPUT);
  input_.when(fltk3::WHEN_ENTER_KEY | fltk3::WHEN_RELEASE);
  input_.callback((fltk3::Callback *)sb_cb, this);
  //input_.box(Boxtype(input_.box()|TIE_RIGHT));
  input_.box(input_.box());
  
  up_button_.callback((fltk3::Callback *)sb_cb, this);
  //up_button_.box(Boxtype(up_button_.box()|TIE_LEFT|TIE_BOTTOM));
  up_button_.box(up_button_.box());
  
  down_button_.callback((fltk3::Callback *)sb_cb, this);
  //down_button_.box(Boxtype(down_button_.box()|TIE_LEFT|TIE_TOP));
  down_button_.box(down_button_.box());
}


fltk3::Spinner::UpButton::UpButton(int x, int y, int w, int h, const char *l)
: fltk3::RepeatButton(x, y, w, h, l)
{
  visible_focus(0);
}


void fltk3::Spinner::UpButton::draw()
{
  Box *b = value() ? ( down_box() ? down_box() : fltk3::down(box()) ) : box();
  Color c = value() ? selection_color() : color();
  fltk3::Button::draw(b, 0, 0, w(), h(), c, 
                      fltk3::Box::TIE_LEFT|fltk3::Box::TIE_BOTTOM);
  fltk3::font(labelfont(), labelsize()/2);
  fltk3::color(fltk3::color_average(color(), fltk3::BLACK, 0.5));
  fltk3::draw("@8>", 0, 1, w(), h()-1, fltk3::ALIGN_INSIDE);
}


fltk3::Spinner::DownButton::DownButton(int x, int y, int w, int h, const char *l)
: fltk3::RepeatButton(x, y, w, h, l)
{
  visible_focus(0);
}


void fltk3::Spinner::DownButton::draw()
{
  Box *b = value() ? ( down_box() ? down_box() : fltk3::down(box()) ) : box();
  Color c = value() ? selection_color() : color();
  fltk3::Button::draw(b, 0, 0, w(), h(), c, 
                      fltk3::Box::TIE_LEFT|fltk3::Box::TIE_TOP);
  fltk3::font(labelfont(), labelsize()/2);
  fltk3::color(fltk3::color_average(color(), fltk3::BLACK, 0.5));
  fltk3::draw("@2>", 0, 0, w(), h()-2, fltk3::ALIGN_INSIDE);
}


fltk3::Spinner::SpInput::SpInput(int x, int y, int w, int h, const char *l)
: fltk3::Input(x, y, w, h, l)
{
}


void fltk3::Spinner::SpInput::draw()
{
  fltk3::Input::draw(box(), 0, 0, w(), h(), color(), 
                     fltk3::Box::TIE_RIGHT|fltk3::Box::TIE_HIDDEN);
}


void fltk3::Spinner::sb_cb(fltk3::Widget *w, fltk3::Spinner *sb) {
  double v;		// New value
  
  if (w == &(sb->input_)) {
    // Something changed in the input field...
    v = atof(sb->input_.value());
    
    if (v < sb->minimum_) {
      sb->value_ = sb->minimum_;
      sb->update();
    } else if (v > sb->maximum_) {
      sb->value_ = sb->maximum_;
      sb->update();
    } else sb->value_ = v;
  } else if (w == &(sb->up_button_)) {
    // Up button pressed...
    v = sb->value_ + sb->step_;
    
    if (v > sb->maximum_) sb->value_ = sb->minimum_;
    else sb->value_ = v;
    
    sb->update();
  } else if (w == &(sb->down_button_)) {
    // Down button pressed...
    v = sb->value_ - sb->step_;
    
    if (v < sb->minimum_) sb->value_ = sb->maximum_;
    else sb->value_ = v;
    
    sb->update();
  }
  
  sb->do_callback();
}


void fltk3::Spinner::update() {
  char s[255];		// Value string
  
  if (format_[0]=='%'&&format_[1]=='.'&&format_[2]=='*') {  // precision argument
                                                            // this code block is a simplified version of
                                                            // fltk3::Valuator::format() and works well (but looks ugly)
    int c = 0;
    char temp[64], *sp = temp;
    sprintf(temp, "%.12f", step_);
    while (*sp) sp++;
    sp--;
    while (sp>temp && *sp=='0') sp--;
    while (sp>temp && (*sp>='0' && *sp<='9')) { sp--; c++; }
    sprintf(s, format_, c, value_);
  } else {
    sprintf(s, format_, value_);
  }
  input_.value(s);
}


int fltk3::Spinner::handle(int event) {
  switch (event) {
    case fltk3::KEYDOWN :
    case fltk3::SHORTCUT :
      if (fltk3::event_key() == fltk3::UpKey) {
        up_button_.do_callback();
        return 1;
      } else if (fltk3::event_key() == fltk3::DownKey) {
        down_button_.do_callback();
        return 1;
      } else return 0;
      
    case fltk3::FOCUS :
      if (input_.take_focus()) return 1;
      else return 0;
  }
  
  return Group::handle(event);
}


void fltk3::Spinner::resize(int X, int Y, int W, int H) {
  Group::resize(X,Y,W,H);      
  input_.resize(0, 0, W - H / 2 - 2, H);
  up_button_.resize(W - H / 2 - 2, 0, H / 2 + 2, H / 2);
  down_button_.resize(W - H / 2 - 2, H - H / 2, H / 2 + 2, H / 2);
}


/** See double fltk3::Spinner::step() const */
void fltk3::Spinner::step(double s) {
  step_ = s;
  if (step_ != (int)step_) input_.type(fltk3::FLOAT_INPUT);
  else input_.type(fltk3::INT_INPUT);
  update();
}


void fltk3::Spinner::type(uchar v) { 
  if (v==fltk3::FLOAT_INPUT) {
    format("%.*f");
  } else {
    format("%.0f");
  }
  input_.type(v); 
}


//
// End of "$Id$".
//
