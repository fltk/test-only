//
// "$Id: Fl_Item.cxx,v 1.31 2003/11/04 08:10:59 spitzak Exp $"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/Item.h>
#include <fltk/Image.h>
#include <fltk/CheckButton.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <string.h>

using namespace fltk;

// Style used to draw menu items.  This controls the font and label color,
// how the toggle/radio buttons are drawn, and other "contents" of the
// menu item that can also be drawn into a browser, even though the
// browser has different selection drawing rules.  The background color,
// selected color and box drawn around selected items is controlled
// by the Menu style in PopupMenu.cxx.

static void revert(Style* s) {
  //s->buttonbox = NO_BOX;
  //s->color = GRAY75;
  //s->box = DOWN_BOX;
  // s->buttonbox = NO_BOX; // makes it look more like Windows
  //s->buttoncolor = WHITE;
  //s->glyph = CheckButton::default_glyph;
}
static NamedStyle style("Item", revert, &Item::default_style);
NamedStyle* Item::default_style = &::style;

Item::Item(const char* l) : Widget(0,0,0,0,l) {
  // we need to defer setting the glyph to here because C++ has no way
  // to make sure the check button style is constructed before this style:
  if (!default_style->glyph_)
    default_style->glyph_ = CheckButton::default_style->glyph_;
  style(default_style);
  set_flag(ALIGN_LEFT|ALIGN_INSIDE);
}

/** Modify the parent of the Item::default_style to this style.
    If no style settings have been done to an Item, it will use the
    textfont, textsize, textcolor, and possibly other settings
    inherited from this style to draw itself. This is used by menus
    and browsers to cause all the elements inside them to draw
    using their settings.

    Use Item::clear_style() to put this back so that \a style can
    be deleted. This is the same as setting it to Widget::default_style.
*/
void Item::set_style(const Style* style) {
  ::style.parent_ = style;
}

/** By default an item just draws it's label using the textcolor.
    If flags() has SELECTED on it uses selection_textcolor. This
    assummes the caller has already drawn the background.

    The current version can also draw check or radio buttons
    but this functionality may be removed.
*/
void Item::draw() {
  //if (buttonbox() != NO_BOX) draw_buttonbox();
  int x = 0; int y = 0; int w = this->w(); int h = this->h();
  //box()->inset(x,y,w,h);

  if (type()) {
    // if pushed, preview the resulting state:
    int lflags = flags();
    if (pushed()) {
      if (type() != RADIO) lflags ^= VALUE;
      else lflags |= VALUE;
    }
    int gw = int(textsize())+2;
    draw_glyph(0, x+3, y+((h-gw)>>1), gw, gw, lflags);
    x += gw+3; w -= gw+3;
  }
  draw_label(x, y, w, h, style(), flags()|OUTPUT);
}

/** Measure the space the draw() will take and set w() and h().
    Maybe this should be the default layout() function for Widget? */
void Item::layout() {
  if (w() && h()) return; // already at the correct size
  //int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  setfont(textfont(), textsize());
  int w = 250, h = 250; measure(label(), w, h, flags());
  if (type()) w += 15;
  if (image()) {
    float W, H;
    image()->measure(W, H);
    if (H > h) h = int(H);
    w += int(W);
  }
  this->w(w/*-dw*/+6);
  this->h(h/*-dh*/);
  Widget::layout();
}

// Items do not accept ANY events.
int Item::handle(int) {return 0;}

////////////////////////////////////////////////////////////////

#include <fltk/ItemGroup.h>
#include <fltk/damage.h>

ItemGroup::ItemGroup(const char* l) : Menu(0,0,0,0,l) {
  style(::style);
  align(ALIGN_LEFT|ALIGN_INSIDE);
}

// implementation of draw & layout should be identical to Item type()==0

void ItemGroup::draw() {
  if (damage()&~DAMAGE_CHILD) {
    //if (box() != NO_BOX) draw_box();
    int x = 0; int y = 0; int w = this->w(); int h = this->h();
    //box()->inset(x,y,w,h);
    draw_label(x, y, w, h, style(), flags()|OUTPUT);
  }
}

void ItemGroup::layout() {
  if (w() && h()) return; // already at the correct size
  //int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  setfont(textfont(), textsize());
  int h; int w = 0; 
  measure(label(), w, h, flags());
  if (image()) {
    float W, H;
    image()->measure(W, H);
    if (H > h) h = int(H);
    w += int(W);
  }
  this->w(w/*-dw*/+6);
  this->h(h/*-dh*/);
  Widget::layout();
}

// Items do not accept ANY events.
int ItemGroup::handle(int) {return 0;}

////////////////////////////////////////////////////////////////
// Divider, does not share any code, but closely related:

#include <fltk/Divider.h>

Divider::Divider() : Widget(0,0,0,0) {
  style(::style);
  set_flag(ALIGN_LEFT|ALIGN_INSIDE|OUTPUT);
}

void Divider::draw() {
  setcolor(GRAY33);
  if (w() > h()) {
    drawline(0, 0, w()-1, 0);
    setcolor(WHITE);
    drawline(0, 1, w()-1, 1);
  } else if (h()) {
    drawline(0, 0, 0, h()-1);
    setcolor(WHITE);
    drawline(1, 0, 1, h()-1);
  }
}

void Divider::layout() {
  if (!w()) w(2);
  if (!h()) h(2);
}

// Items do not accept ANY events.
int Divider::handle(int) {return 0;}
