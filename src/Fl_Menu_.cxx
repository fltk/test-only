//
// "$Id: Fl_Menu_.cxx,v 1.12 1999/03/19 19:41:08 carl Exp $"
//
// Common menu code for the Fast Light Tool Kit (FLTK).
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

// This is a base class for all items that have a menu:
//	Fl_Menu_Bar, Fl_Menu_Button, Fl_Choice
// This provides storage for a menu item, functions to add/modify/delete
// items, and a call for when the user picks a menu item.

// More code in Fl_Menu_add.C

#include <FL/Fl.H>
#include <FL/Fl_Menu_.H>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_STYLE ((Style*)default_style())

void Fl_Menu_::loadstyle() const {
  if (!Fl::s_menu_) {
    Fl::s_menu_ = 1;

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
    Fl::load_attributes("menu", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute menu_attributes[] = {
      { "down box", DOWN_BOX },
      { "text font", TEXTFONT },
      { "text size", TEXTSIZE },
      { "text color", TEXTCOLOR },
      { 0 }
    };
    Fl::load_attributes("menu", DEFAULT_STYLE->menu_, menu_attributes);
  }
}

static Fl_Menu_Item* _find(const char *label, Fl_Menu_Item *menu) {
  char *p, l[128];
  Fl_Menu_Item *m;

  strncpy(l, label, sizeof(l));
  l[sizeof(l) - 1] = (char)0;

  if ((p = strchr(l, '/'))) *p++ = (char)0;

  for (m = menu; m && m->label() && strcmp(m->label(), l); m = m->next()) ;
  if (!m || !m->label()) return 0;
  if (p) {
    if (m->flags&FL_SUBMENU_POINTER)
      return _find(p, (Fl_Menu_Item *)m->user_data());
    else
      return _find(p, ++m);
  }
  return m;
}

Fl_Menu_Item* Fl_Menu_::find(const char *label) const
{ return _find(label, menu()); }

int Fl_Menu_::value(const Fl_Menu_Item* m) {
  clear_changed();
  if (value_ != m) {value_ = m; return 1;}
  return 0;
}

// When user picks a menu item, call this.  It will do the callback.
// Unfortunatly this also casts away const for the checkboxes, but this
// was necessary so non-checkbox menus can really be declared const...
const Fl_Menu_Item* Fl_Menu_::picked(const Fl_Menu_Item* v) {
  if (v) {
    if (v->radio()) {
      if (!v->value()) { // they are turning on a radio item
	set_changed();
	((Fl_Menu_Item*)v)->setonly();
      }
      redraw();
    } else if (v->flags & FL_MENU_TOGGLE) {
      set_changed();
      ((Fl_Menu_Item*)v)->flags ^= FL_MENU_VALUE;
      redraw();
    } else if ((when()&FL_WHEN_RELEASE) || v != value_) { // normal item
      set_changed();
    }
    value_ = v;
    if (when()&(FL_WHEN_CHANGED|FL_WHEN_RELEASE)) {
      if (changed() || when()&FL_WHEN_NOT_CHANGED) {
	clear_changed();
	if (value_ && value_->callback_) value_->do_callback((Fl_Widget*)this);
	else do_callback();
      }
    }
  }
  return v;
}

// turn on one of a set of radio buttons
void Fl_Menu_Item::setonly() {
  flags |= FL_MENU_RADIO | FL_MENU_VALUE;
  Fl_Menu_Item* j;
  for (j = this; ; ) {	// go down
    if (j->flags & FL_MENU_DIVIDER) break; // stop on divider lines
    j++;
    if (!j->text || !j->radio()) break; // stop after group
    j->clear();
  }
  for (j = this-1; ; j--) { // go up
    if (!j->text || (j->flags&FL_MENU_DIVIDER) || !j->radio()) break;
    j->clear();
  }
}

Fl_Widget::Style* Fl_Menu_::_default_style = 0;

Fl_Menu_::Style::Style() : Fl_Widget::Style() {
  sbf = 0;

  widget(BOX) = FL_MEDIUM_UP_BOX;

  menu(DOWN_BOX) = FL_MEDIUM_UP_BOX2;
  menu(TEXTFONT) = FL_HELVETICA;
  menu(TEXTSIZE) = 10;
  menu(TEXTCOLOR) = FL_BLACK;
}

Fl_Menu_::Fl_Menu_(int X,int Y,int W,int H,const char* l) : Fl_Widget(X,Y,W,H,l) {
  set_flag(SHORTCUT_LABEL);
  when(FL_WHEN_RELEASE_ALWAYS);
  value_ = menu_ = 0;
  alloc = 0;
}

int Fl_Menu_::size() const {
  if (!menu_) return 0;
  return menu_->size();
}

void Fl_Menu_::menu(const Fl_Menu_Item* m) {
  clear();
  value_ = menu_ = (Fl_Menu_Item*)m;
}

Fl_Menu_::~Fl_Menu_() {
  clear();
}

void Fl_Menu_::clear() {
  if (alloc) {
    if (alloc>1) for (int i = size(); i--;)
      if (menu_[i].text) free((void*)menu_[i].text);
    delete[] menu_;
    menu_ = 0;
    alloc = 0;
  }
}

Fl_Font Fl_Menu_::textfont() const {
  if (!_style || !(MENU_STYLE->sbf & bf(TEXTFONT)))
    return (Fl_Font)DEFAULT_STYLE->menu(TEXTFONT);
  return (Fl_Font)MENU_STYLE->menu(TEXTFONT);
}

uchar Fl_Menu_::textsize() const {
  if (!_style || !(MENU_STYLE->sbf & bf(TEXTSIZE)))
    return DEFAULT_STYLE->menu(TEXTSIZE);
  return MENU_STYLE->menu(TEXTSIZE);
}

Fl_Color Fl_Menu_::textcolor() const {
  if (!_style || !(MENU_STYLE->sbf & bf(TEXTCOLOR)))
    return (Fl_Color)DEFAULT_STYLE->menu(TEXTCOLOR);
  return (Fl_Color)MENU_STYLE->menu(TEXTCOLOR);
}

Fl_Boxtype Fl_Menu_::down_box() const {
  if (!_style || !(MENU_STYLE->sbf & bf(DOWN_BOX)))
    return (Fl_Boxtype)DEFAULT_STYLE->menu(DOWN_BOX);
  return (Fl_Boxtype)MENU_STYLE->menu(DOWN_BOX);
}

Fl_Color Fl_Menu_::down_color() const {return selection_color();}

void Fl_Menu_::copy(const Fl_Menu_Item* m, void* user_data) {
  int n = m->size()+1;
  Fl_Menu_Item* newMenu = new Fl_Menu_Item[n];
  memcpy(newMenu, m, n*sizeof(Fl_Menu_Item));
  menu(newMenu);
  alloc = 1; // make destructor free it
  // for convienence, provide way to change all the user data pointers:
  if (user_data) for (; n--;) {
    if (newMenu->callback_) newMenu->user_data_ = user_data;
    newMenu++;
  }
}

//
// End of "$Id: Fl_Menu_.cxx,v 1.12 1999/03/19 19:41:08 carl Exp $".
//
