//
// "$Id: Fl_Item.cxx,v 1.13 2001/03/22 20:18:27 robertk Exp $"
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

#include <FL/Fl_Item.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Image.H>
#include <string.h>
extern char fl_draw_shortcut;

static const int _default_column[1] = { 0 };

const int *Fl_Item::one_column_ = _default_column;

// Style used to draw menu items.  This controls the font and label color,
// how the toggle/radio buttons are drawn, and other "contents" of the
// menu item that can also be drawn into a browser, even though the
// browser has different selection drawing rules.  The background color,
// selected color and box drawn around selected items is controlled
// by the Menu style in Fl_Menu.cxx.

static void mi_revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
}

static Fl_Named_Style* style =
  new Fl_Named_Style("Item", mi_revert, &style);

#include <stdio.h>

Fl_Item::Fl_Item(const char* l) : Fl_Widget(0,0,0,0,l) {
  style(::style);
  set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  mcolumns = one_column_;
  column_char_ = '\t';
}

void Fl_Item::draw() {
  draw_box();

  int x = 0; int y = 0; int w = this->w(); int h = this->h();
  box()->inset(x,y,w,h);

  if (type() == FL_TOGGLE_ITEM || type()==FL_RADIO_ITEM) {
    // if pushed, preview the resulting state:
    int lflags = flags();
    if (pushed()) {
      if (type()==FL_TOGGLE_ITEM) lflags ^= FL_VALUE;
      else lflags |= FL_VALUE;
    }
    draw_glyph(type()==FL_RADIO_ITEM ? FL_GLYPH_ROUND : FL_GLYPH_CHECK,
	       x+3, y+(h-13)/2, 13, 13, lflags);
    x += 15; w -= 15;
  }

  //  handle multi_column label
  if(label() && column_char() && strchr(label(), column_char())) {
    char *pOldLabel = (char *)label();
	char *pRemainder = (char *)label();
	int i = 0;
	while(column_widths()[i])
	{
	  char hold;
	  int cw = column_widths()[i];
	  char *pTemp = pRemainder;
	  pRemainder = strchr(pTemp, column_char());
	  if (pRemainder) {
	    hold = *pRemainder;
		*pRemainder = '\0';
      }
	  label(pTemp);
	  draw_label(x, y, cw, h, flags());
	  x += cw;
	  w -= cw;
      if(pRemainder) 
        *pRemainder++ = hold;		
	  ++i;
	}
	if(pRemainder) {
	  label(pRemainder);
	  draw_label(x, y, w, h, flags());
	}
	label(pOldLabel);	// restore label when done drawing
  }
  else draw_label(x+3, y, w-6, h, flags());
}

// Measure the space the draw() will take:
// maybe this should be the default layout() function for Fl_Widget?
void Fl_Item::layout() {
  if (w() && h()) return; // already at the correct size
  int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  fl_font(label_font(), label_size());
  if (!(flags() & FL_NO_SHORTCUT_LABEL)) fl_draw_shortcut = 1;
  int h; int w = 0; 
  // handle multi columns
  fl_measure(label(), w, h);
  if(column_widths()[0] && label() && column_char() &&
						strchr(label(), column_char()))
  	w = this->w() - 6; // why 6?  Beats me, but it matches draw()...
  fl_draw_shortcut = 0;
  if (type()==FL_TOGGLE_ITEM || type()==FL_RADIO_ITEM) w += 15;
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

#include <FL/Fl_Item_Group.H>

Fl_Item_Group::Fl_Item_Group(const char* l) : Fl_Group(0,0,0,0,l) {
  style(::style);
  align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  // end()?
}

void Fl_Item_Group::draw() {
//  ((Fl_Item*)this)->Fl_Item::draw();
  draw_box();

  int x = 0; int y = 0; int w = this->w(); int h = this->h();
  box()->inset(x,y,w,h);

  if (type() == FL_TOGGLE_ITEM || type()==FL_RADIO_ITEM) {
    // if pushed, preview the resulting state:
    int lflags = flags();
    if (pushed()) {
      if (type()==FL_TOGGLE_ITEM) lflags ^= FL_VALUE;
      else lflags |= FL_VALUE;
    }
    draw_glyph(type()==FL_RADIO_ITEM ? FL_GLYPH_ROUND : FL_GLYPH_CHECK,
	       x+3, y+(h-13)/2, 13, 13, lflags);
    x += 15; w -= 15;
  }

  draw_label(x+3, y, w-6, h, flags());
}

void Fl_Item_Group::layout() {
//  ((Fl_Item*)this)->Fl_Item::layout();
  if (w() && h()) return; // already at the correct size
  int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  fl_font(label_font(), label_size());
  if (!(flags() & FL_NO_SHORTCUT_LABEL)) fl_draw_shortcut = 1;
  int h; int w = 0; 
  fl_measure(label(), w, h);
  fl_draw_shortcut = 0;
  if (type()==FL_TOGGLE_ITEM || type()==FL_RADIO_ITEM) w += 15;
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

#include <FL/Fl_Divider.H>

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
