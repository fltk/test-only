//
// "$Id: Fl_Check_Button.cxx,v 1.6 1999/03/18 22:59:04 carl Exp $"
//
// Check button widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Check_Button.H>

#define DEFAULT_STYLE ((Style*)default_style())

// A subclass of Fl_Button that always draws as a diamond box.  This
// diamond is smaller than the widget size and can be surchecked by
// another box type, for compatability with Forms.

Fl_Widget::Style* Fl_Check_Button::_default_style = 0;


Fl_Check_Button::Style::Style() : Fl_Light_Button::Style() {
  widget(COLOR2) = FL_RED;
  widget(BOX) = FL_NO_BOX;
  button(DOWN_BOX) = FL_DIAMOND_DOWN_BOX;
  button(DOWN_LABELCOLOR) = FL_BLACK;
}

void Fl_Check_Button::loadstyle() const {
  if (!Fl::s_check_button) {
    Fl::s_check_button = 1;

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
    Fl::load_attributes("check button", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute button_attributes[] = {
      { "highlight color", FLY_COLOR },
      { "highlight box", FLY_BOX },
      { "down label color", DOWN_LABELCOLOR },
      { 0 }
    };
    Fl::load_attributes("check button", DEFAULT_STYLE->button_, button_attributes);
  }
}

Fl_Check_Button::Fl_Check_Button(int x, int y, int w, int h, const char *l) : Fl_Light_Button(x, y, w, h, l) {}
