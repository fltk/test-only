//
// "$Id"
//
// Fluid menus declaration for the Fast Light Tool Kit (FLTK).
//
// The widget describing an Fl_Window.  This is also all the code
// for interacting with the overlay, which allows the user to
// select, move, and resize the children widgets.
//
// Copyright 1998-2005 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
#ifndef fltk_fluid_menus_h
#define fltk_fluid_menus_h
#include "FluidType.h"

namespace fltk {
    class MenuBar;
    class ItemGroup;
    class Item;
    class xpmImage;

    MenuBar* build_hierarchy(MenuBar* menubar);
    void fill_in_New_Menu(ItemGroup* menu);
    Item * fluidMenuItem(FluidType& wt,int n=-1);
    Item * fluidMenuItem(FluidType& wt, xpmImage *i);
    ItemGroup * fluidMenuGroup(const char * menu_name,int n=-1);
    ItemGroup * fluidMenuGroup(const char * menu_name,xpmImage *i);
}

extern fltk::Item *history_item[10],*undo_item[2],*iwidget_bin,*itooltip,*isource_view;
extern fltk::ItemGroup* newMenu;
extern fltk::MenuBar* Main_Menu;

#define DECL_MENUCB(n) void n(Widget *, void *)
#define DECL_MENUCB2(n,type) void n(type *, void *)
#define IMPL_MENUCB(n) void n(Widget *, void *) {}

#endif
//
// End of "$Id"
//

