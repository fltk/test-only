//
// "$Id: Fl_Pack.cxx,v 1.24 2002/12/10 02:00:45 easysw Exp $"
//
// Packing widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/PackedGroup.h>
#include <fltk/layout.h>
#include <fltk/Box.h>
using namespace fltk;

// Resizes all the child widgets to be the full width and stacks them
// vertically. All widgets before the resizable (or all of them if
// there is no resizable) are pushed against the top of this widget,
// all widgets after the resizable are pushed against the bottom, and
// the resizable fills the remaining space.

// A child widget can change it's height by calling layout() on itself
// and this will rearrange all other widgets to accomodate the new height.

// If resizable is not set, the PackedGroup itself resizes to surround the
// items, allowing it to be imbedded in a surrounding PackedGroup.

// The code can support widgets that extend vertically, these are put
// against the left edge (or the right if after the resizable()). This
// is done by setting VERTICAL_LAYOUT in the type().

#define is_vertical(widget) (type()&1 || widget->flags()&PACK_VERTICAL)

PackedGroup::PackedGroup(int x,int y,int w ,int h,const char *l)
: Group(x, y, w, h, l) {
  spacing_ = 0;
  type(VERTICAL);
  //resizable(0);
}

void PackedGroup::layout() {
  for (int iter = 0; iter < 2; iter++) {
    if (!layout_damage()) break;

    // we only need to do something special if the group is resized:
    if (!(layout_damage() & (LAYOUT_WH|LAYOUT_DAMAGE)) || !children()) {
      Group::layout();
      if (!(layout_damage() & LAYOUT_DAMAGE)) break;
    }

    // clear the layout flags, so any resizes of children will set them again:
    Widget::layout();

    // This is the rectangle to lay out the remaining widgets in:
    int x = 0;
    int y = 0;
    int r = this->w();
    int b = this->h();
    box()->inset(x,y,r,b);

    bool saw_horizontal = false;
    bool saw_vertical = false;

    // layout all the top & left widgets (the ones before the resizable):
    int i; for (i = 0; i < children(); i++) {
      Widget* widget = child(i);
      if (widget->contains(resizable())) break;
      if (!widget->visible()) continue;
      if (is_vertical(widget)) {
	widget->resize(x, y, widget->w(), b-y);
	widget->layout();
	x = widget->x()+widget->w()+spacing_;
	saw_vertical = true;
      } else { // put along top edge:
	widget->resize(x, y, r-x, widget->h());
	widget->layout();
	y = widget->y()+widget->h()+spacing_;
	saw_horizontal = true;
      }
    }

    int resizable_index = i;

    // layout all the bottom & right widgets by going backwards:
    for (i = children()-1; i > resizable_index; i--) {
      Widget* widget = child(i);
      if (!widget->visible()) continue;
      if (is_vertical(widget)) {
	int W = widget->w();
	widget->resize(r-W, y, W, b-y);
	widget->layout();
	r = widget->x()-spacing_;
	saw_vertical = true;
      } else { // put along top edge:
	int H = widget->h();
	widget->resize(x, b-H, r-x, H);
	widget->layout();
	b = widget->y()-spacing_;
	saw_horizontal = true;
      }
    }

    // Lay out the resizable widget to fill the remaining space:
    if (resizable_index < children()) {
      Widget* widget = child(resizable_index);
      widget->resize(x, y, r-x, b-y);
      widget->layout();
    }

    // A non-resizable widget will become the size of it's items:
    int W = w();
    if (r < x || !resizable() && !saw_horizontal) W -= (r-x);
    int H = h();
    if (b < y || !resizable() && !saw_vertical) H -= (b-y);
    size(W,H);
  }
  redraw();
}

//
// End of "$Id: Fl_Pack.cxx,v 1.24 2002/12/10 02:00:45 easysw Exp $".
//
