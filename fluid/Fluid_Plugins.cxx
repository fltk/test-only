#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/dirent.h>
#include <FL/Fl_DLL.H>
#include "Fluid_Plugins.h"

Fluid_Plugin* plugins[MAXPLUGINS];
int nbplugins;

#ifndef FL_NO_DLL
static int nboptions;
static int nbnew;
#endif
Fl_Menu_Item Plugins_Options_Menu[MAXPLUGINS+1];
Fl_Menu_Item Plugins_New_Menu[MAXPLUGINS+1];

static void ReadPlugin(char* s, char* location)
{
#ifndef FL_NO_DLL
  if(nbplugins >= MAXPLUGINS) return;

  FLDLhandle handle;
  if(!strcmp(s+strlen(s)-sizeof(FLDLext)+1, FLDLext)) {
    char s2[256];

    sprintf(s2, "%s%s", location, s);
    handle = FLDLopen(s2 );

    if(handle) {

      bool used = 0;
      s[strlen(s)-strlen(FLDLext)] = 0;

      Fluid_Plugin *d = (Fluid_Plugin*) FLDLsym( handle, "fluid_plugin");
      if(d) {
	int i;
	for(i=0; i<nbplugins; i++) if(!strcmp(d->name, plugins[i]->name)) break;
	if(i<nbplugins) 
	  fprintf(stderr, "Duplicate FLUID plugin, only the first one will be loaded ...\n");
	else {
	  fprintf(stderr, "Found FLUID plugin '%s' ...\n", s);
	  used = 1;
	  if(d->options_menu) {
	    Plugins_Options_Menu[nboptions].text = d->name;
	    Plugins_Options_Menu[nboptions].user_data_ = d->options_menu;
	    Plugins_Options_Menu[nboptions].flags_ = FL_SUBMENU_POINTER;
	    nboptions++;
	  }
	  if(d->new_menu) {
	    Plugins_New_Menu[nbnew].text = d->name;
	    Plugins_New_Menu[nbnew].user_data_ = d->new_menu;
	    Plugins_New_Menu[nbnew].flags_ = FL_SUBMENU_POINTER;
	    fill_in_New_Menu(d->new_menu);
	    nbnew++;
	  }
	  plugins[nbplugins++] = d;
	}
      }

      if(!used) FLDLclose(handle);

    } else {
      fprintf(stderr, "FLUID plugin error : %s\n", FLDLerror());
    }
  }
#endif
}

static void ReadPlugins(char* location)
{
  dirent **d = 0;
  int n = filename_list(location, &d);

  for (int i = n; i > 0;) 
  {
    ReadPlugin(d[i-1]->d_name, location);
    free((void*)(d[--i]));
  }
  free((void*)d);

}

void read_plugins()
{
  // Priority order in case of duplicate plugin name is from the most local to the most global
  ReadPlugins("plugins/");
  ReadPlugins("./");
#ifndef WIN32
  char s[256];
  sprintf(s, "%s/.fluid/plugins/", getenv("HOME"));
  ReadPlugins(s);
  ReadPlugins("/usr/local/lib/fltk/fluid/");
#endif
}
