//
// "$Id: Fl_Menu_Item_SW.cxx,v 1.3 1999/05/05 20:48:36 carl Exp $"
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

#include <FL/Fl_Menu_Item.H>

void Fl_Menu_Item::label(Fl_Labeltype a, const char* b) {
  mstyle(&_style);
  MENU_ITEM_STYLE->sbf |= bf(LABELTYPE);
  MENU_ITEM_STYLE->menu_item(LABELTYPE)=a;
  text = b;
}

void Fl_Menu_Item::labeltype(Fl_Labeltype a) {
  mstyle(&_style);
  MENU_ITEM_STYLE->sbf |= bf(LABELTYPE);
  MENU_ITEM_STYLE->menu_item(LABELTYPE)=a;
}

void Fl_Menu_Item::labelcolor(uchar a) {
  mstyle(&_style);
  MENU_ITEM_STYLE->sbf |= bf(LABELCOLOR);
  MENU_ITEM_STYLE->menu_item(LABELCOLOR)=a;
}

void Fl_Menu_Item::labelfont(uchar a) {
  mstyle(&_style);
  MENU_ITEM_STYLE->sbf |= bf(LABELFONT);
  MENU_ITEM_STYLE->menu_item(LABELFONT)=a;
}

void Fl_Menu_Item::labelsize(uchar a) {
  mstyle(&_style);
  MENU_ITEM_STYLE->sbf |= bf(LABELSIZE);
  MENU_ITEM_STYLE->menu_item(LABELSIZE)=a;
}

void Fl_Menu_Item::down_box(uchar b) {
  mstyle(&_style);
  MENU_ITEM_STYLE->sbf |= bf(DOWN_BOX);
  MENU_ITEM_STYLE->menu_item(DOWN_BOX)=b;
}

void Fl_Menu_Item::down_color(uchar c) {
  mstyle(&_style);
  MENU_ITEM_STYLE->sbf |= bf(DOWN_COLOR);
  MENU_ITEM_STYLE->menu_item(DOWN_COLOR)=c;
}

void Fl_Menu_Item::light_color(uchar c) {
  mstyle(&_style);
  MENU_ITEM_STYLE->sbf |= bf(LIGHT_COLOR);
  MENU_ITEM_STYLE->menu_item(LIGHT_COLOR)=c;
}

void Fl_Menu_Item::down_labelcolor(uchar c) {
  mstyle(&_style);
  MENU_ITEM_STYLE->sbf |= bf(DOWN_LABELCOLOR);
  MENU_ITEM_STYLE->menu_item(DOWN_LABELCOLOR)=c;
}
