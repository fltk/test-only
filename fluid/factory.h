//
// "$Id"
//
// WidgetType include file for the Fast Light Tool Kit (FLTK).
//
// This class stores the image labels for widgets in fluid.  This is
// not a class in fltk itself, and this will produce different types of
// code depending on what the image type is.  There are private subclasses
// in Fluid_Image.C for each type of image format.  Right now only xpm
// files are supported.
//
// Copyright 1998-2006 by Bill Spitzak and others.
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
#ifndef fltk_factory_h
#define fltk_factory_h


namespace fltk {
    class Item;
    class ItemGroup;

    FluidType *FluidType_make(const char *tn);
    Item * fluidMenuItem(FluidType& wt,int n) ;
    ItemGroup * fluidMenuGroup(const char * menu_name,int n) ;
    void fill_in_New_Menu(ItemGroup* menu);
    int lookup_symbol(const char *name, int &v, int numberok=0);
}
#endif

// End of "$Id"
