//
// "$Id: Fluid_Plugins.h,v 1.8 1999/09/20 01:51:16 vincent Exp $"
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

class Fl_Menu_Item;
class Fl_Window;

#define PLUGINS_EXTENSION ".fluidplugin"

struct Fluid_Plugin {
  // statically user filled informations
  char* 	name;
  Fl_Menu_Item* options_menu;
  Fl_Menu_Item* new_menu;
  void 		(*make_panel)();

  // dynamic variables
  int           please_show_panel; // Set it to 1 when a widget use 
                                   // some settings of this panel

  Fl_Window*    panel;		   // This has to be filled by the
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

// Need to FLUID_EXPORT the fluid_plugin symbol in each plugins
// FLUID_IMPORT is for symbols in fluid that the plugin want to access
#if defined(WIN32) && defined(FLUID_PLUGIN)
# define FLUID_EXPORT __declspec( dllexport )
# define FLUID_IMPORT __declspec( dllimport )
#else
# define FLUID_EXPORT
# define FLUID_IMPORT
#endif

#endif

//
// End of "$Id: Fluid_Plugins.h,v 1.8 1999/09/20 01:51:16 vincent Exp $"
//
