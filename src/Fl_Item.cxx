//
// "$Id: Fl_Item.cxx,v 1.16 2002/01/28 08:02:59 spitzak Exp $"
//
// Widget designed to be an item in a menu or browser.
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

#include <fltk/Fl_Item.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Image.h>
#include <fltk/Fl_Check_Button.h>
#include <string.h>

// Style used to draw menu items.  This controls the font and label color,
// how the toggle/radio buttons are drawn, and other "contents" of the
// menu item that can also be drawn into a browser, even though the
// browser has different selection drawing rules.  The background color,
// selected color and box drawn around selected items is controlled
// by the Menu style in Fl_Menu.cxx.

static void revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
  s->color = FL_GRAY;
  s->button_box = FL_DOWN_BOX;
  // s->button_box = FL_NO_BOX; // makes it look more like Windows
  s->button_color = FL_WHITE;
  s->glyph = Fl_Check_Button::default_glyph;
}
static Fl_Named_Style style("Item", revert, &Fl_Item::default_style);
Fl_Named_Style* Fl_Item::default_style = &::style;

Fl_Item::Fl_Item(const char* l) : Fl_Widget(0,0,0,0,l) {
  style(default_style);
  set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
}

void Fl_Item::draw() {
  draw_box();

  int x = 0; int y = 0; int w = this->w(); int h = this->h();
  box()->inset(x,y,w,h);

  if (type()) {
    // if pushed, preview the resulting state:
    int lflags = flags();
    if (pushed()) {
      if (type() != RADIO) lflags ^= FL_VALUE;
      else lflags |= FL_VALUE;
    }
    int gw = text_size()+2;
    draw_glyph(0, x+3, y+(h-gw)/2, gw, gw, lflags);
    x += gw+3; w -= gw+3;
  }
  draw_label(x+3, y, w-6, h, flags());
}

// Measure the space the draw() will take:
// maybe this should be the default layout() function for Fl_Widget?
void Fl_Item::layout() {
  if (w() && h()) return; // already at the correct size
  int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  fl_font(label_font(), label_size());
  int w = 250, h = 250; fl_measure(label(), w, h, flags());
  if (type()) w += 15;
  if (image()) {
    int W, H;
    image()->measure(W, H);
    if (H > h) h = H;
    w += W;
  }
  this->w(w-dw+6);
  this->h(h-dh);
  Fl_Widget::layout();
}

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Item_Group.h>

Fl_Item_Group::Fl_Item_Group(const char* l) : Fl_Group(0,0,0,0,l) {
  style(::style);
  align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
}

// implementation of draw & layout should be identical to Fl_Item type()==0

void Fl_Item_Group::draw() {
  draw_box();
  int x = 0; int y = 0; int w = this->w(); int h = this->h();
  box()->inset(x,y,w,h);
  draw_label(x+3, y, w-6, h, flags());
}

void Fl_Item_Group::layout() {
  if (w() && h()) return; // already at the correct size
  int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  fl_font(label_font(), label_size());
  int h; int w = 0; 
  fl_measure(label(), w, h, flags());
  if (image()) {
    int W, H;
    image()->measure(W, H);
    if (H > h) h = H;
    w += W;
  }
  this->w(w-dw+6);
  this->h(h-dh);
  Fl_Widget::layout();
}

////////////////////////////////////////////////////////////////
// Fl_Divider, does not share any code, but closely related:

#include <fltk/Fl_Divider.h>

Fl_Divider::Fl_Divider() : Fl_Widget(0,0,0,0) {
  style(::style);
  set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_OUTPUT);
}

void Fl_Divider::draw() {
  fl_color(FL_DARK3);
  fl_line(0, 0, w(), 0);
  fl_color(FL_LIGHT3);
  fl_line(0, 1, w(), 1);
}

void Fl_Divider::layout() {
  h(2);
}
