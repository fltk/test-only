//
// "$Id: Fl_Menu_Item_SW.cxx,v 1.4 1999/05/06 05:52:18 carl Exp $"
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

void Fl_Menu_Item::attr(Attribute a, uchar c) {
  mstyle(&_style);
  MENU_ITEM_STYLE->sbf |= bf(a);
  MENU_ITEM_STYLE->menu_item(a)=c;
}

void Fl_Menu_Item::label(Fl_Labeltype a, const char* b) { attr(LABELTYPE, a); text = b; }
void Fl_Menu_Item::labeltype(Fl_Labeltype a) { attr(LABELTYPE, a); }
void Fl_Menu_Item::labelcolor(uchar a) { attr(LABELCOLOR, a); }
void Fl_Menu_Item::labelfont(uchar a) { attr(LABELFONT, a); }
void Fl_Menu_Item::labelsize(uchar a) { attr(LABELSIZE, a); }
void Fl_Menu_Item::down_box(uchar b) { attr(DOWN_BOX, b); }
void Fl_Menu_Item::down_color(uchar c) { attr(DOWN_COLOR, c); }
void Fl_Menu_Item::light_color(uchar c) { attr(LIGHT_COLOR, c); }
void Fl_Menu_Item::down_labelcolor(uchar c) { attr(DOWN_LABELCOLOR, c); }
