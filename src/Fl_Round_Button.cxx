//
// "$Id: Fl_Round_Button.cxx,v 1.6 1999/03/18 22:59:09 carl Exp $"
//
// Round button for the Fast Light Tool Kit (FLTK).
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

// A subclass of Fl_Button that always draws as a round circle.  This
// circle is smaller than the widget size and can be surrounded by
// another box type, for compatability with Forms.

#include <FL/Fl.H>
#include <FL/Fl_Round_Button.H>

#define DEFAULT_STYLE ((Style*)default_style())

Fl_Widget::Style* Fl_Round_Button::_default_style = 0;

Fl_Round_Button::Style::Style() : Fl_Light_Button::Style() {
  widget(COLOR2) = FL_RED;

  button(DOWN_BOX) = FL_ROUND_DOWN_BOX;
}

void Fl_Round_Button::loadstyle() const {
  if (!Fl::s_round_button) {
    Fl::s_round_button = 1;

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
    Fl::load_attributes("round button", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute button_attributes[] = {
      { "highlight color", FLY_COLOR },
      { "highlight box", FLY_BOX },
      { "down label color", DOWN_LABELCOLOR },
      { 0 }
    };
    Fl::load_attributes("round button", DEFAULT_STYLE->button_, button_attributes);
  }
}

Fl_Round_Button::Fl_Round_Button(int x,int y,int w,int h, const char *l) : Fl_Light_Button(x,y,w,h,l) {}

//
// End of "$Id: Fl_Round_Button.cxx,v 1.6 1999/03/18 22:59:09 carl Exp $".
//
