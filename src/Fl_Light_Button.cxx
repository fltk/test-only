//
// "$Id: Fl_Light_Button.cxx,v 1.7 1999/04/04 03:45:25 gustavo Exp $"
//
// Lighted button widget for the Fast Light Tool Kit (FLTK).
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

// Subclass of Fl_Button where the "box" indicates whether it is
// pushed or not, and the "down box" is drawn small and square on
// the left to indicate the current state.

// The default down_box of zero draws a rectangle designed to look
// just like Flame's buttons.

#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Light_Button.H>

#define DEFAULT_STYLE ((Style*)default_style())

void Fl_Light_Button::draw() {
  loadstyle();
  Fl_Color col;
  Fl_Boxtype bt;
  if (fly_box() && Fl::belowmouse() == this)
    { bt = fly_box(); col = fly_color(); }
  else
    { bt = box(); col = color(); }

  if (box()) {
    draw_box(this==Fl::pushed() ? down(box()) : bt, col);
  }
  int d = h()/6;
  int W = w()<h() ? w() : h();
  col = (value() || type() == 0) ? selection_color() : color3();
  col = active_r() ? col : inactive(col);
  if (fly_box() && Fl::belowmouse() == this && col == color())
    col = fly_color();
  if (down_box()) {
    // draw other down_box() styles:
    draw_box(down_box(), x()+d, y()+d+1, W-2*d-2, W-2*d-2, col);
  } else {
    // if down_box() is zero, draw light button style:
    int hh = h()-2*d;
    int ww = hh/2+4;
    int xx = d*2;
    if (w()<ww+2*xx) xx = (w()-ww)/2;
    draw_box(FL_MEDIUM_DOWN_BOX, x()+xx, y()+d, ww, hh, col);
    draw_box(FL_THIN_UP_BOX, x()+xx+2, y()+d+2, ww-4, hh-4, col);
  }

  Fl_Color lc = value() ? down_labelcolor() : labelcolor();
  draw_label(x()+W-d, y(), w()-W+d, h(), lc);
}

int Fl_Light_Button::handle(int event) {
  switch (event) {
  case FL_RELEASE:
    if (box() && !Fl::pushed()) redraw();
  default:
    return Fl_Button::handle(event);
  }
}

Fl_Widget::Style* Fl_Light_Button::_default_style = 0;

Fl_Light_Button::Style::Style() : Fl_Button::Style() {
  widget(COLOR2) = FL_RED;

  button(DOWN_BOX) = FL_NO_BOX;
  button(DOWN_LABELCOLOR) = FL_BLACK;
}

void Fl_Light_Button::loadstyle() const {
  if (!Fl::s_light_button) {
    Fl::s_light_button = 1;

    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "selected light color", COLOR2 },
      { "unselected light color", COLOR3 },
      { "box", BOX },
      { 0 }
    };
    Fl::load_attributes("light button", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute button_attributes[] = {
      { "highlight color", FLY_COLOR },
      { "highlight box", FLY_BOX },
      { "down box", DOWN_BOX },
      { "down label color", DOWN_LABELCOLOR },
      { 0 }
    };
    Fl::load_attributes("light button", DEFAULT_STYLE->button_, button_attributes);
  }
}

Fl_Light_Button::Fl_Light_Button(int x, int y, int w, int h, const char *l) : Fl_Button(x, y, w, h, l) {
  align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  type(FL_TOGGLE_BUTTON);
}

//
// End of "$Id: Fl_Light_Button.cxx,v 1.7 1999/04/04 03:45:25 gustavo Exp $".
//
