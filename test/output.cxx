//
// "$Id$"
//
// Output test program for the Fast Light Tool Kit (FLTK).
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

#include <fltk3/run.h>
#include <fltk3/ValueInput.h> // necessary for bug in mingw32?
#include <fltk3/DoubleWindow.h>
#include <fltk3/Box.h>
#include <fltk3/HorValueSlider.h>
#include <fltk3/ToggleButton.h>
#include <fltk3/Input.h>
#include <fltk3/draw.h>
#include <fltk3/Output.h>
#include <fltk3/MultilineOutput.h>

fltk3::Output *text;
fltk3::MultilineOutput *text2;
fltk3::Input *input;
fltk3::ValueSlider *fonts;
fltk3::ValueSlider *sizes;
fltk3::DoubleWindow *window;

void font_cb(fltk3::Widget *,void *) {
  text->textfont(int(fonts->value()));
  text->redraw();
  text2->textfont(int(fonts->value()));
  text2->redraw();
}

void size_cb(fltk3::Widget *,void *) {
  text->textsize(int(sizes->value()));
  text->redraw();
  text2->textsize(int(sizes->value()));
  text2->redraw();
}

void input_cb(fltk3::Widget *,void *) {
  text->value(input->value());
  text2->value(input->value());
}

int main(int argc, char **argv) {
  window = new fltk3::DoubleWindow(400,400);

  input = new fltk3::Input(50,375,350,25);
  input->static_value("The quick brown fox\njumped over\nthe lazy dog.");
  input->when(fltk3::WHEN_CHANGED);
  input->callback(input_cb);

  sizes = new fltk3::HorValueSlider(50,350,350,25,"Size");
  sizes->align(fltk3::ALIGN_LEFT);
  sizes->bounds(1,64);
  sizes->step(1);
  sizes->value(14);
  sizes->callback(size_cb);

  fonts = new fltk3::HorValueSlider(50,325,350,25,"Font");
  fonts->align(fltk3::ALIGN_LEFT);
  fonts->bounds(0,15);
  fonts->step(1);
  fonts->value(0);
  fonts->callback(font_cb);

  text2 = new fltk3::MultilineOutput(100,150,200,100,"fltk3::Multiline_Output");
  text2->value(input->value());
  text2->align(fltk3::ALIGN_BOTTOM);
  text2->tooltip("This is an fltk3::Multiline_Output widget.");
  window->resizable(text2);

  text = new fltk3::Output(100,90,200,30,"fltk3::Output");
  text->value(input->value());
  text->align(fltk3::ALIGN_BOTTOM);
  text->tooltip("This is an fltk3::Output widget.");

  window->end();
  window->show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
