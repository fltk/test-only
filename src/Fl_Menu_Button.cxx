//
// "$Id: Fl_Menu_Button.cxx,v 1.5 1999/03/14 06:46:32 carl Exp $"
//
// Menu button widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>

#define DEFAULT_STYLE ((Style*)default_style())

void Fl_Menu_Button::draw() {
  loadstyle();
  if (!box() || type()) return;
  Fl_Color col;
  Fl_Boxtype bt;
  if (fly_box() && Fl::belowmouse() == this)
    { bt = fly_box(); col = fly_color(); }
  else
    { bt = box(); col = color(); }
  draw_box(bt, col);
  draw_label();
  if (box() == FL_FLAT_BOX) return; // for XForms compatability
  int H = (labelsize()-3)&-2;
  int X = x()+w()-H*2;
  int Y = y()+(h()-H)/2;
  if (active_r()) {
    fl_color(FL_DARK3); fl_line(X+H/2, Y+H, X, Y, X+H, Y);
    fl_color(FL_LIGHT3); fl_line(X+H, Y, X+H/2, Y+H);
  } else {
    fl_color((Fl_Color)44); fl_line(X+H/2, Y+H, X, Y, X+H, Y);
    fl_color((Fl_Color)52); fl_line(X+H, Y, X+H/2, Y+H);
  }
}

const Fl_Menu_Item* Fl_Menu_Button::popup() {
  const Fl_Menu_Item* m;
  if (!box() || type()) {
    m = menu()->popup(Fl::event_x(), Fl::event_y(), label(), mvalue(), this);
  } else {
    m = menu()->pulldown(x(), y(), w(), h(), 0, this);
  }
  picked(m);
  return m;
}

int Fl_Menu_Button::handle(int e) {
  if (!menu() || !menu()->text) return 0;
  switch (e) {
  case FL_ENTER:
  case FL_LEAVE:
    if (box() && fly_box()) redraw();
    return (box() && !type()) ? 1 : 0;
  case FL_PUSH:
    if (!box()) {
      if (Fl::event_button() != 3) return 0;
    } else if (type()) {
      if (!(type() & (1 << (Fl::event_button()-1)))) return 0;
    }
    popup();
    return 1;
  case FL_SHORTCUT:
    if (Fl_Widget::test_shortcut()) {popup(); return 1;}
    return test_shortcut() != 0;
  default:
    return 0;
  }
}

Fl_Menu_Button::Style Fl_Menu_Button::_default_style;

Fl_Menu_Button::Style::Style() : Fl_Menu_::Style() {
  sbf = 0;

  widget(BOX) = FL_MEDIUM_UP_BOX;

  menu_button(FLY_COLOR) = 51;
  menu_button(FLY_BOX) = FL_MEDIUM_UP_BOX;
}

void Fl_Menu_Button::loadstyle() {
  if (!Fl::s_menu_button) {
    Fl::s_menu_button = 1;

    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "down color", COLOR2 },
      { "box", BOX },
      { 0 }
    };
    Fl::load_attributes("menu button", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute menu_attributes[] = {
      { "down box", DOWN_BOX },
      { "text font", TEXTFONT },
      { "text size", TEXTSIZE },
      { "text color", TEXTCOLOR },
      { 0 }
    };
    Fl::load_attributes("menu button", DEFAULT_STYLE->menu_, menu_attributes);

    static Fl::Attribute menu_button_attributes[] = {
      { "highlight color", FLY_COLOR },
      { "highlight box", FLY_BOX },
      { 0 }
    };
    Fl::load_attributes("menu button", DEFAULT_STYLE->menu_button_,
                                 menu_button_attributes);
  }
}

Fl_Menu_Button::Fl_Menu_Button(int X,int Y,int W,int H,const char *l) : Fl_Menu_(X,Y,W,H,l) {}

Fl_Boxtype Fl_Menu_Button::fly_box() const {
  if (style && (WIDGET_STYLE->sbf & bf(BOX)) && !(MENU_BUTTON_STYLE->sbf & bf(FLY_BOX)))
    return (Fl_Boxtype)WIDGET_STYLE->widget(BOX);
  if (!style || !(MENU_BUTTON_STYLE->sbf & bf(FLY_BOX)))
    return (Fl_Boxtype)DEFAULT_STYLE->menu_button(FLY_BOX);
  return (Fl_Boxtype)MENU_BUTTON_STYLE->menu_button(FLY_BOX);
}

Fl_Color Fl_Menu_Button::fly_color() const {
  if (!style || !(MENU_BUTTON_STYLE->sbf & bf(FLY_COLOR)))
    return (Fl_Color)DEFAULT_STYLE->menu_button(FLY_COLOR);
  return (Fl_Color)MENU_BUTTON_STYLE->menu_button(FLY_COLOR);
}

//
// End of "$Id: Fl_Menu_Button.cxx,v 1.5 1999/03/14 06:46:32 carl Exp $".
//
