#ifndef FLUID_PLUGINS_H
#define FLUID_PLUGINS_H

#include <FL/Fl_Menu.H>
#include <FL/Fl_DLL.H>
#include "Fl_Type.h"

#define PLUGINS_EXTENSION ".fluidplugin"

struct Fluid_Plugin {
  // statically user filled informations
  char* 	name;
  Fl_Menu_Item* options_menu;
  Fl_Menu_Item* new_menu;
  void 		(*make_panel)();

  // dynamic variables
  bool          please_show_panel; // Set it to 1 when a widget use 
                                   // some settings of this panel

  Fl_Window*    panel;		   // This has to be filled by the
                                   // make panel function

  // dynamic variables used by fluid only
  bool		was_visible;
  bool		panel_is_orphan;
};

#define MAXPLUGINS 32
extern Fluid_Plugin* plugins[MAXPLUGINS];
extern int nbplugins;
extern Fl_Menu_Item Plugins_Options_Menu[];
extern Fl_Menu_Item Plugins_New_Menu[];

// defined in fluid/factory
void fill_in_New_Menu(Fl_Menu_Item* menu);

void read_plugins();

// windows specific strange things happening here ...
#ifdef WIN32
# ifdef FLUID_INDLL
#  define FLUID_EXPORT __declspec( dllimport )
# elif defined(OUTDLL)
#  define FLUID_EXPORT __declspec( dllexport )
# else
#  define FLUID_EXPORT
# endif


# ifdef FLUID_OUTDLL
#  define FLUID_IMPORT __declspec( dllimport )
# elif defined(INDLL)
#  define FLUID_IMPORT __declspec( dllexport )
# else
#  define FLUID_IMPORT
# endif
#endif


#endif
