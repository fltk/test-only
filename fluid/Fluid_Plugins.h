//
// "$Id: Fluid_Plugins.h,v 1.15 2001/08/05 10:48:38 spitzak Exp $"
//
// Plugins headers for the Fast Light Tool Kit (FLTK).
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
#ifndef FLUID_PLUGINS_H
#define FLUID_PLUGINS_H

struct Fl_Menu_Item;
class Fl_Group;

#define PLUGINS_EXTENSION ".flpl"

struct Fluid_Plugin {
  // statically user filled informations
  char* 	name;
  Fl_Menu_Item* options_menu;
  Fl_Menu_Item* new_menu;
  void 		(*make_panel)();

  // dynamic variables
  int           please_show_panel; // Set it to 1 when a widget use 
                                   // some settings of this panel

  Fl_Group*    panel;		   // This has to be filled by the
                                   // make panel function

  // dynamic variables used by fluid only
  int		was_visible;
  int		panel_is_orphan;
};

#define MAXPLUGINS 32
extern Fluid_Plugin* plugins[MAXPLUGINS];
extern int nbplugins;
extern Fl_Menu_Item Plugins_Options_Menu[];
extern Fl_Menu_Item Plugins_New_Menu[];

// defined in fluid/factory
void fill_in_New_Menu(Fl_Menu_Item* menu);

void read_plugins();

#if defined(_WIN32)
# if defined(FLUID_PLUGIN)
#  define FLUID_PLUGIN_API __declspec( dllexport )
#  define FLUID_API __declspec( dllimport )
# else
#  define FLUID_API __declspec( dllexport )
# endif
#else
# define FLUID_API
# define FLUID_PLUGIN_API
#endif

#endif

//
// End of "$Id: Fluid_Plugins.h,v 1.15 2001/08/05 10:48:38 spitzak Exp $"
//
