#ifndef FLUID_PLUGINS_H
#define FLUID_PLUGINS_H

#include <FL/Fl_Menu.H>
#include "Fl_Type.h"

struct Fluid_Plugin {
  // user filled informations
  char* name;
  Fl_Menu_Item* options_menu;
  Fl_Menu_Item* new_menu;
  void 		(*make_panel)();

  // dynamic variables
  Fl_Window*    panel;
  bool          please_show_panel;
  bool		was_visible;
};

#define MAXPLUGINS 32
extern Fluid_Plugin* plugins[MAXPLUGINS];
extern int nbplugins;
extern Fl_Menu_Item Plugins_Options_Menu[];
extern Fl_Menu_Item Plugins_New_Menu[];

// defined in fluid/factory
void fill_in_New_Menu(Fl_Menu_Item* menu);

void read_plugins();

#endif
