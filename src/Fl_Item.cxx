//
// "$Id: Fl_Item.cxx,v 1.34 2004/01/27 06:32:37 spitzak Exp $"
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

/*! \class fltk::Item

  This widget is designed to be put into fltk::Menu and fltk::Browser widgets. 

  Windows is unfortunatly inconsistent about how it draws menu and
  browser items, so to duplicate this the draw() method assummes
  the background has already been drawn by the caller. It does not
  erase it, but draws the text atop it. More inconsistencies have
  forced us to add the set_style() call so this may change...

  The browser draws the focus dotted box and the hierarchy indicators,
  and the shortcut key assigments in menus is drawn by the menu
  code. You cannot turn these off or modify them, unfortunately I was
  unable to get these into the child widgets and still replicate the
  standard Windows UI appearance.

  In a MultiBrowser, selected() indicates if the widget is
  currently turned on in the browser.
*/

static NamedStyle style("Item", 0, &Item::default_style);
/*! The default style is entirely blank and interits from whatever
  the parent is. Normally this is Widget::default_style, but if
  you call set_style() it will be that style. */
NamedStyle* Item::default_style = &::style;

/*! Unlike other widgets the constructor does not take any dimensions,
  since it is assummed the container widget will size this
  correctly.
*/
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
  // make sure we don't make a loop:
  for (const Style* p = style; p; p = p->parent_)
    if (p == &::style) return;
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
    int gw = int(textsize())+2;
    draw_glyph(0, x+3, y+((h-gw)>>1), gw, gw, flags());
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

/*! Returns 0 always. Items do not accept \e any events. Any results
  of clicking on them is handled by the parent Menu or Browser. */
int Item::handle(int) {return 0;}

////////////////////////////////////////////////////////////////

/*! \class fltk::ItemGroup

  This widget describes a set of items that are to be put inside a
  fltk::Menu or fltk::Browser widget. It indicates the title of a
  submenu, or a level of hierarchy in the browser. Any child widgets
  are the items in that submenu, or the items under this parent in the
  browser.

  If this widget is told to draw, it draws just like fltk::Item
  draws. See that for more details. The triangle indicating a submenu
  is not drawn by this, it is drawn by the parent menu.

  Because this is an fltk::Menu subclass, you can also call popup()
  and add() and other methods to manipulate the items inside it.

  In a Browser, the value() indicates if the widget is open or not. In
  a MultiBrowser, the selected() indicates if the widget is currently
  selected.

*/

#include <fltk/ItemGroup.h>
#include <fltk/damage.h>

/*! Unlike other widgets the constructor does not take any dimensions,
  since it is assummed the container widget will size this
  correctly. */
ItemGroup::ItemGroup(const char* l) : Menu(0,0,0,0,l) {
  style(Item::default_style);
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
  int w = 250, h = 250; measure(label(), w, h, flags());
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

/*! Returns 0 always. Items do not accept \e any events. Any results
  of clicking on them is handled by the parent Menu or Browser. */
int ItemGroup::handle(int) {return 0;}

////////////////////////////////////////////////////////////////

/*! \class fltk::Divider
  This widget is designed to go into Menu and Browser widgets and draw
  an inset horizontal line across them. It has the OUTPUT flag set
  so the user cannot choose it.
*/

#include <fltk/Divider.h>

/*! Unlike other widgets the constructor does not take any dimensions,
  since it is assummed the container widget will size this
  correctly. */
Divider::Divider() : Widget(0,0,0,0) {
  style(Item::default_style);
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

/*! Resizes to a 2x2 square */
void Divider::layout() {
  if (!w()) w(2);
  if (!h()) h(2);
}

/*! Returns 0 always. Items do not accept \e any events. Any results
  of clicking on them is handled by the parent Menu or Browser. */
int Divider::handle(int) {return 0;}
