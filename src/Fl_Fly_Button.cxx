//
// "$Id: Fl_Fly_Button.cxx,v 1.1 1999/03/14 06:46:30 carl Exp $"
//
// Fly button widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Fly_Button.H>
#include <FL/fl_draw.H>

#define DEFAULT_STYLE ((Style*)default_style())

Fl_Fly_Button::Style Fl_Fly_Button::_default_style;

Fl_Fly_Button::Style::Style() : Fl_Button::Style() {
  widget(BOX) = FL_FLAT_BOX;
}

void Fl_Fly_Button::loadstyle() {
  if (!Fl::s_fly_button) {
    Fl::s_fly_button = 1;

    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "down color", COLOR2 },
      { 0 }
    };
    Fl::load_attributes("highlight button", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute button_attributes[] = {
      { "highlight color", FLY_COLOR },
      { "highlight box", FLY_BOX },
      { "down box", DOWN_BOX },
      { "down label color", DOWN_LABELCOLOR },
      { 0 }
    };
    Fl::load_attributes("highlight button", DEFAULT_STYLE->button_, button_attributes);
  }
}

Fl_Fly_Button::Fl_Fly_Button(int x,int y,int w,int h,const char *l) : Fl_Button(x,y,w,h,l) {}

//
// End of "$Id: Fl_Fly_Button.cxx,v 1.1 1999/03/14 06:46:30 carl Exp $".
//
