//
// "$Id: Fl_Choice.cxx,v 1.16 1999/05/04 22:54:21 carl Exp $"
//
// Choice widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Choice.H>
#include <FL/fl_draw.H>

#define DEFAULT_STYLE ((Style*)default_style())

// Emulates the Forms choice widget.  This is almost exactly the same
// as an Fl_Menu_Button.  The only difference is the appearance of the
// button: it draws the text of the current pick and a down-arrow.

extern char fl_draw_shortcut;

void Fl_Choice::loadstyle() const {
  if (!Fl::s_choice) {
    Fl::s_choice = 1;

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
    Fl::load_attributes("choice", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute menu_attributes[] = {
      { "down box", DOWN_BOX },
      { "text font", TEXTFONT },
      { "text size", TEXTSIZE },
      { "text color", TEXTCOLOR },
      { 0 }
    };
    Fl::load_attributes("choice", DEFAULT_STYLE->menu_, menu_attributes);

    static Fl::Attribute choice_attributes[] = {
      { "highlight color", FLY_COLOR },
      { "highlight box", FLY_BOX },
      { 0 }
    };
    Fl::load_attributes("choice", DEFAULT_STYLE->choice_, choice_attributes);
  }
}

void Fl_Choice::draw() {
  loadstyle();
  Fl_Color col;
  Fl_Boxtype bt;
  if (fly_box() && Fl::belowmouse() == this)
    { bt = fly_box(); col = fly_color(); }
  else
    { bt = box(); col = color(); }
  draw_box(bt, col);
  if (box() == FL_FLAT_BOX) return; // for XForms compatability
  int H = labelsize()/2+1;
  draw_box(FL_THIN_UP_BOX,x()+w()-3*H,y()+(h()-H)/2,2*H,H,col);
  if (mvalue()) {
    // Shouldn't do this, but we can handle it
    Fl_Menu_Item m = *mvalue();
    if (active_r()) m.activate(); else m.deactivate();
    int BW = Fl::box_dx(box());
    fl_clip(x(), y(), w()-3*H, h());
    fl_draw_shortcut = 2; // hack value to make '&' disappear
    m.draw(x()+BW, y(), w()-2*BW-3*H, h(), this);
    fl_draw_shortcut = 0;
    m._style = 0; // so that the real menu item's style doesn't get deallocated
    fl_pop_clip();
  }
  draw_label();
}

Fl_Widget::Style* Fl_Choice::_default_style = 0;

Fl_Choice::Style::Style() : Fl_Menu_::Style() {
  sbf = 0;

  widget(BOX) = FL_MEDIUM_UP_BOX;

  menu(TEXTSIZE) = 14;

  choice(FLY_COLOR) = 51;
  choice(FLY_BOX) = FL_MEDIUM_UP_BOX;
}

Fl_Choice::Fl_Choice(int x,int y,int w,int h, const char *l) : Fl_Menu_(x,y,w,h,l) {
  align(FL_ALIGN_LEFT);
  when(FL_WHEN_RELEASE);
}

int Fl_Choice::value(int v) {
  if (!Fl_Menu_::value(v)) return 0;
  redraw();
  return 1;
}

int Fl_Choice::handle(int e) {
  if (!menu() || !menu()->text) return 0;
  const Fl_Menu_Item* v;
  switch (e) {
  case FL_PUSH:
    Fl::event_is_click(0);
  J1:
    v = menu()->pulldown(x(), y(), w(), h(), mvalue(), this);
    if (!v || v->submenu()) return 1;
    if (v != mvalue()) redraw();
    picked(v);
    return 1;
  case FL_SHORTCUT:
    if (Fl_Widget::test_shortcut()) goto J1;
    v = menu()->test_shortcut();
    if (!v) return 0;
    if (v != mvalue()) redraw();
    picked(v);
    return 1;
  case FL_ENTER:
  case FL_LEAVE:
    if (takesevents() && fly_box()) redraw();
    return 1;
  default:
    return 0;
  }
}

Fl_Boxtype Fl_Choice::fly_box() const {
  if (_style && (WIDGET_STYLE->sbf & bf(BOX)) && !(CHOICE_STYLE->sbf & bf(FLY_BOX)))
    return (Fl_Boxtype)WIDGET_STYLE->widget(BOX);
  if (!_style || !(CHOICE_STYLE->sbf & bf(FLY_BOX)))
    return (Fl_Boxtype)DEFAULT_STYLE->choice(FLY_BOX);
  return (Fl_Boxtype)CHOICE_STYLE->choice(FLY_BOX);
}

Fl_Color Fl_Choice::fly_color() const {
  if (!_style || !(CHOICE_STYLE->sbf & bf(FLY_COLOR)))
    return (Fl_Color)DEFAULT_STYLE->choice(FLY_COLOR);
  return (Fl_Color)CHOICE_STYLE->choice(FLY_COLOR);
}

//
// End of "$Id: Fl_Choice.cxx,v 1.16 1999/05/04 22:54:21 carl Exp $".
//
