//
// "$Id$"
//
// FLUID main entry for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

const char *copyright =
"The FLTK user interface designer version 2.0\n"
"Copyright 1998-2001 by Bill Spitzak and others.\n"
"\n"
"This library is free software; you can redistribute it and/or "
"modify it under the terms of the GNU Library General Public "
"License as published by the Free Software Foundation; either "
"version 2 of the License, or (at your option) any later version.\n"
"\n"
"This library is distributed in the hope that it will be useful, "
"but WITHOUT ANY WARRANTY; without even the implied warranty of "
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. "
"See the GNU Library General Public License for more details.\n"
"\n"
"You should have received a copy of the GNU Library General Public "
"License along with this library; if not, write to the Free Software "
"Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 "
"USA.\n"
"\n"
"Please report bugs to fltk-bugs@fltk.org.";

#include <fltk/run.h>
#include <fltk/visual.h>
#include <fltk/events.h>
#include <fltk/Window.h>
#include <fltk/Box.h>
#include <fltk/Button.h>
#include <fltk/Browser.h>
#include <fltk/MenuBar.h>
#include <fltk/Input.h>
#include <fltk/Tooltip.h>
#include <fltk/ask.h>
#include <fltk/draw.h>
#include <fltk/file_chooser.h>
#include <fltk/ask.h>
#include <fltk/filename.h>
#include <fltk/FileIcon.h>
#include <fltk/Preferences.h>
#include <fltk/ItemGroup.h>
#include <fltk/Item.h>
#include <fltk/Divider.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
# include <fltk/win32.h>	// for MAX_PATH definition
# include <direct.h>
#else
# include <unistd.h>
#endif

#include "about_panel.h"

#include "Fluid_Plugins.h"
#include "FluidType.h"
#include "coding_style.h"
#include "../src/flstring.h"


/////////////////////////////////////////
// Read preferences file 
fltk::Preferences fluid_prefs(fltk::Preferences::USER, "fltk.org", "fluid");

int gridx;
int gridy;
int snap;

static int  read_alignement_prefs() {
    fluid_prefs.get("snap", snap, 3);
    fluid_prefs.get("gridx", gridx, 5);
    fluid_prefs.get("gridy", gridy, 5);
    return 0;
}
static int dummy = read_alignement_prefs();
/////////////////////////////////////////

// File history info...
char	absolute_history[10][1024];
char	relative_history[10][1024];

void	load_history();
void	update_history(const char *);

////////////////////////////////////////////////////////////////

void nyi(fltk::Widget *,void *) {
    fltk::message("That's not yet implemented, sorry");
}

static const char *filename;
void set_filename(const char *c);
int modflag;

#if 0
static char* pwd;
static char in_source_dir;
#endif
void goto_source_dir() {
#if 0
    if (in_source_dir) return;
    if (!filename || !*filename) return;
    const char *p = filename_name(filename);
    if (p <= filename) return; // it is in the current directory
    char buffer[1024];
    strcpy(buffer,filename);
    int n = p-filename; if (n>1) n--; buffer[n] = 0;
    if (!pwd) {
	pwd = getcwd(0,1024);
	if (!pwd) {fprintf(stderr,"getcwd : %s\n",strerror(errno)); return;}
    }
    if (chdir(buffer)<0) {fprintf(stderr, "Can't chdir to %s : %s\n",
	buffer, strerror(errno)); return;}
    in_source_dir = 1;
    fltk::SharedImage::set_root_directory(buffer);
#endif
}

#include "Fluid_Image.h"
void goto_images_dir() {
#if 0
    if (in_source_dir) return;
    if (!filename || !*filename) return;
    const char *p = filename_name(filename);
    // This is static since fltk::SharedImage::set_root_directory just copies the pointer
    static char buffer[1024];
    if (p <= filename)
	strcpy(buffer, images_dir);
    else
    {
	strcpy(buffer,filename);
	int n = p-filename; if (n>1) n--; buffer[n] = 0;
	if(buffer[strlen(buffer)-1]!='/' && buffer[strlen(buffer)-1]!='\\')
	    strcat(buffer, "/");
	strcat(buffer, images_dir);
    }
    if (!pwd) {
	pwd = getcwd(0,1024);
	if (!pwd) {fprintf(stderr,"getcwd : %s\n",strerror(errno)); return;}
    }
    if (chdir(buffer)<0) {fprintf(stderr, "Can't chdir to %s : %s\n",
	buffer, strerror(errno)); return;}
    in_source_dir = 1;
    
    // Call to fltk::set_images_root_directory so that images are corretly displayed in FLUID.
    // Construct the path name verbosely because images are loaded from draw() function and
    // we do not know what is the cwd at this time
#ifdef _WIN32
    if (buffer[1] != ':') {
#endif
	// this is skipped on Win32 if the path has a drive letter (like "C:\whatever")
	memmove(buffer+strlen(pwd)+1, buffer, strlen(buffer));
	strcpy(buffer, pwd);
	buffer[strlen(pwd)]='/';
#ifdef _WIN32
    }
#endif
    fltk::SharedImage::set_root_directory(buffer);
#endif
}

void leave_source_dir() {
#if 0
    if (!in_source_dir) return;
    if (chdir(pwd)<0) {fprintf(stderr, "Can't chdir to %s : %s\n",
			     pwd, strerror(errno));}
    in_source_dir = 0;
#endif
}

fltk::Window *main_window;

void save_cb(fltk::Widget *, void *v) {
    const char *c = filename;
    if (v || !c || !*c) {
	if (!(c=fltk::file_chooser("Save to:", "*.f[ld]", c))) return;
	set_filename(c);
    }
    if (!write_file(c)) {
	fltk::message("Error writing %s: %s", c, strerror(errno));
	return;
    }
    modflag = 0;
}

void exit_cb(fltk::Widget *,void *) {
    if (modflag)
	switch (fltk::choice("Save changes before exiting?", "Yes", "No", "Cancel")) {
      case 2 : /* Cancel */
          return;
      case 0 : /* Yes */
          save_cb(NULL, NULL);
	  if (modflag) return;	// Didn't save!
    }
    
    exit(0);
}

void open_cb(fltk::Widget *, void *v) {
    if (!v && modflag && !fltk::ask("Discard changes?")) return;
    const char *c;
    if (!(c = fltk::file_chooser("Open:", "*.f[ld]", filename))) return;
    if (!v) set_filename(c);
    if (!read_file(c, v!=0)) {
	fltk::message("Can't read %s: %s", c, strerror(errno));
	return;
    }
    if (!v) modflag = 0;
    else modflag = 1;
}

void open_history_cb(fltk::Widget *, void *v) {
    if (modflag) {
	switch (fltk::choice("Do you want to save changes to this user\n"
	    "interface before opening another one?", "Cancel",
	    "Save", "Don't Save"))
	{
	case 0 : /* Cancel */
	    return;
	case 1 : /* Save */
	    save_cb(NULL, NULL);
	    if (modflag) return;	// Didn't save!
	}
    }
    const char *oldfilename = filename;
    filename = NULL;
    set_filename((char *)v);
    // undo_suspend(); nyi
    if (!read_file(filename, 0)) {
	//  undo_resume(); nyi
	//  undo_clear(); nyi
	fltk::message("Can't read %s: %s", filename, strerror(errno));
	free((void *)filename);
	filename = oldfilename;
	if (main_window) main_window->label(filename);
	return;
    }
    modflag = 0;
    //  undo_resume(); nyi
    //  undo_clear(); nyi
    if (oldfilename) free((void *)oldfilename);
}

void new_cb(fltk::Widget *, void *v) {
    if (!v && modflag && !fltk::ask("Discard changes?")) return;
    const char *c;
    if (!(c = fltk::file_chooser("New:", "*.f[ld]", 0))) return;
    delete_all();
    set_filename(c);
    modflag = 0;
}

static int compile_only = 0;
int header_file_set = 0;
int code_file_set = 0;
const char* header_file_name = ".h";
const char* code_file_name = ".cxx";

void write_cb(fltk::Widget *, void *) {
    if (!filename) {
	save_cb(0,0);
	if (!filename) return;
    }
    char cname[1024];
    char hname[1024];
    if (code_file_name[0] == '.' && isalpha(code_file_name[1])) {
	strcpy(cname,fltk::filename_name(filename));
	strcpy((char*)fltk::filename_ext(cname), code_file_name);
    } else {
	strcpy(cname, code_file_name);
    }
    if (header_file_name[0] == '.' && isalpha(header_file_name[1])) {
	strcpy(hname,fltk::filename_name(filename));
	strcpy((char*)fltk::filename_ext(hname), header_file_name);
    } else {
	strcpy(hname, header_file_name);
    }
    if (!compile_only) goto_source_dir();
    int x = write_code(cname,hname);
    if (!compile_only) leave_source_dir();
    strcat(cname, "/"); strcat(cname,header_file_name);
    if (compile_only) {
	if (!x) {fprintf(stderr,"%s : %s\n",cname,strerror(errno)); exit(1);}
    } else {
	if (!x) {
	    fltk::message("Can't write %s: %s", cname, strerror(errno));
	} else {
	    fltk::message("Wrote %s", cname, 0);
	}
    }
}

void openwidget_cb(fltk::Widget *, void *) {
    if (!FluidType::current) {
	fltk::message("Please select a widget");
	return;
    }
    FluidType::current->open();
}

void toggle_overlays(fltk::Widget *,void *);

void select_all_cb(fltk::Widget *,void *);

void group_cb(fltk::Widget *, void *);

void ungroup_cb(fltk::Widget *, void *);

extern int pasteoffset;
static int ipasteoffset;

static char* cutfname() {
#ifdef _WIN32
    static char name[MAX_PATH+16] = "";
    
    if (!name[0]) {
	if (!GetTempPath(sizeof(name), name)) strcpy(name,"\\"); // failure
	
	strcat(name, ".fluidcutbuffer");
    }
    
    return name;
#else
    static char name[1024] = "";
    if (!name[0]) filename_normalize(name, 1024, "~/.fluid_cut_buffer", 0);
    return name;
#endif
}

void copy_cb(fltk::Widget*, void*) {
    if (!FluidType::current) return;
    ipasteoffset = 10;
    if (!write_file(cutfname(),1)) {
	fltk::message("Can't write %s: %s", cutfname(), strerror(errno));
	return;
    }
}

extern void select_only(FluidType *);
void cut_cb(fltk::Widget *, void *) {
    if (!FluidType::current) return;
    ipasteoffset = 0;
    FluidType *p = FluidType::current->parent;
    while (p && p->selected) p = p->parent;
    if (!write_file(cutfname(),1)) {
	fltk::message("Can't write %s: %s", cutfname(), strerror(errno));
	return;
    }
    delete_all(1);
    if (p) select_only(p);
}

extern int force_parent, gridx, gridy;

void paste_cb(fltk::Widget*, void*) {
    if (ipasteoffset) force_parent = 1;
    pasteoffset = ipasteoffset;
    if (gridx>1) pasteoffset = ((pasteoffset-1)/gridx+1)*gridx;
    if (gridy>1) pasteoffset = ((pasteoffset-1)/gridy+1)*gridy;
    if (!read_file(cutfname(), 1)) {
	fltk::message("Can't read %s: %s", cutfname(), strerror(errno));
    }
    pasteoffset = 0;
    ipasteoffset += 10;
    force_parent = 0;
}

void earlier_cb(fltk::Widget*,void*);

void later_cb(fltk::Widget*,void*);

void sort(FluidType *parent); // in WidgetType.cxx

static void sort_cb(fltk::Widget *,void *) {
    for (FluidType* f = FluidType::first; f; f = f->next_brother) sort(f);
}

void show_alignment_cb(fltk::Widget *, void *);

void set_images_dir_cb(fltk::Widget *, void *);

void about_cb(fltk::Widget *, void *) {
    if (!about_panel) make_about_panel(copyright);
    copyright_box->hide();
    display_group->show();
    about_panel->show();
}

void tt_cb(fltk::Widget *w, void *) {
    fltk::Tooltip::enable(w->value());
}

#include <string.h>
#include <fltk/ask.h>

#include <fltk/StyleSet.h>
fltk::StyleSet* fluid_style_set;
fltk::StyleSet* style_set;
char* theme;

void set_theme(const char* s) {
#if 0
    if (!s || !*s) {
	if (!theme) return;
	free((void*)theme);
	theme = 0;
    } else {
	if (theme) {
	    if (!strcmp(theme, s)) return;
	    free((void*)theme);
	}
	theme = strdup(s);
    }
    style_set->make_current();
    fltk::Theme f = fltk::Style::load_theme(theme);
    fltk::Style::theme(f);
    // set the scheme...
    fltk::Style::reload_theme();
    fluid_style_set->make_current();
    fltk::redraw();
    if (!f) fltk::alert("Error loading theme \"%s\" (plugin not found?)",
	theme ? theme : "");
#endif
}

void theme_cb(fltk::Widget *, void *) {
    const char* s = fltk::input("Enter the theme name:", theme);
    if (!s) return;
    set_theme(s);
}

////////////////////////////////////////////////////////////////
#include <FL/Fl_Menu_Item.H>

extern Fl_Menu_Item New_Menu[];


////////////////////////////////////////////////////////////////
// New fltk2 menu generation
////////////////////////////////////////////////////////////////
#define NEWM

static fltk::Item*  history_item[10];
extern void fill_in_New_Menu(fltk::ItemGroup* menu);

static void build_hierarchy(fltk::MenuBar* menubar) {
  fltk::ItemGroup* g;
  fltk::Item* o; 

  menubar->begin();
    g=new fltk::ItemGroup("&File",0,0);
	new fltk::Item("New", 0, new_cb, 0);
	new fltk::Item("Open...", fltk::CTRL+'o', open_cb, 0);
	new fltk::Item("Save", fltk::CTRL+'s', save_cb, 0);
	new fltk::Item("Save As...", fltk::CTRL+'S', save_cb, (void*)1);
	new fltk::Item("Merge...", fltk::CTRL+'i', open_cb, (void*)1);
	new fltk::Divider();
	new fltk::Item("Write code", fltk::CTRL+'w', write_cb, 0);
	history_item[0] = new fltk::Item(relative_history[0], fltk::CTRL+'0', open_history_cb, absolute_history[0]);
	history_item[1] = new fltk::Item(relative_history[1], fltk::CTRL+'1', open_history_cb, absolute_history[1]);
	history_item[2] = new fltk::Item(relative_history[2], fltk::CTRL+'2', open_history_cb, absolute_history[2]);
	history_item[3] = new fltk::Item(relative_history[3], fltk::CTRL+'3', open_history_cb, absolute_history[3]);
	history_item[4] = new fltk::Item(relative_history[4], fltk::CTRL+'4', open_history_cb, absolute_history[4]);
	history_item[5] = new fltk::Item(relative_history[5], fltk::CTRL+'5', open_history_cb, absolute_history[5]);
	history_item[6] = new fltk::Item(relative_history[6], fltk::CTRL+'6', open_history_cb, absolute_history[6]);
	history_item[7] = new fltk::Item(relative_history[7], fltk::CTRL+'7', open_history_cb, absolute_history[7]);
	history_item[8] = new fltk::Item(relative_history[8], fltk::CTRL+'8', open_history_cb, absolute_history[8]);
	history_item[9] = new fltk::Item(relative_history[9], fltk::CTRL+'9', open_history_cb, absolute_history[9]);
	new fltk::Divider();
	new fltk::Item("Quit", fltk::CTRL+'q', exit_cb);
    g->end();
    
    g=new fltk::ItemGroup("&Edit",0,0);
	new fltk::Item("Cut", fltk::CTRL+'x', cut_cb);
	new fltk::Item("Copy", fltk::CTRL+'c', copy_cb);
	new fltk::Item("Paste", fltk::CTRL+'v', paste_cb);
	new fltk::Item("Select All", fltk::CTRL+'a', select_all_cb, 0, FL_MENU_DIVIDER);
	new fltk::Item("Edit this widget", fltk::ReturnKey, openwidget_cb);
	new fltk::Item("Sort these widgets", 0, sort_cb);
	new fltk::Item("Move widget earlier", fltk::F2Key, earlier_cb);
	new fltk::Item("Move widget later", fltk::F3Key, later_cb);
	//new fltk::Item("Show", fltk::F5Key, show_cb);
	//new fltk::Item("Hide", fltk::F6Key, hide_cb);
	new fltk::Item("Group", fltk::F7Key, group_cb);
	new fltk::Item("Ungroup", fltk::F8Key, ungroup_cb,0);
	//new fltk::Item("Deactivate", 0, nyi);
	//new fltk::Item("Activate", 0, nyi, 0, FL_MENU_DIVIDER);
	new fltk::Item("Show Overlays",fltk::ALT+'o',toggle_overlays,0,FL_MENU_TOGGLE|FL_MENU_VALUE);
	new fltk::Item("Preferences",fltk::CTRL+'p',show_alignment_cb);
	new fltk::Item("Coding Style", 0, show_coding_style_cb);
	new fltk::Item("Theme", 0, theme_cb);
	new fltk::Item("Set images root directory", fltk::CTRL+'d', set_images_dir_cb);
    g->end();
    
    g=new fltk::ItemGroup("&New", 0, 0);
      fill_in_New_Menu(g);
    g->end();
    
    g=new fltk::ItemGroup("&Plugins", 0, 0, (void *)Plugins_Options_Menu);
    g->end();
    
    g=new fltk::ItemGroup("&Help",0,0);
	new fltk::Item("About fluid",0,about_cb);
	o= new fltk::Item("Tooltips", 0, tt_cb, 0);
	o->type(fltk::Item::TOGGLE);
	//new fltk::Item("Manual",0,nyi);
    g->end();
  menubar->end();
}

#define HISTORY_ITEM 7
Fl_Menu_Item Main_Menu[] = {
    {"&File",0,0,0,FL_SUBMENU},
    {"New", 0, new_cb, 0},
    {"Open...", fltk::CTRL+'o', open_cb, 0},
    {"Save", fltk::CTRL+'s', save_cb, 0},
    {"Save As...", fltk::CTRL+'S', save_cb, (void*)1},
    {"Merge...", fltk::CTRL+'i', open_cb, (void*)1, FL_MENU_DIVIDER},
    {"Write code", fltk::CTRL+'w', write_cb, 0},
    {relative_history[0], fltk::CTRL+'0', open_history_cb, absolute_history[0]},
    {relative_history[1], fltk::CTRL+'1', open_history_cb, absolute_history[1]},
    {relative_history[2], fltk::CTRL+'2', open_history_cb, absolute_history[2]},
    {relative_history[3], fltk::CTRL+'3', open_history_cb, absolute_history[3]},
    {relative_history[4], fltk::CTRL+'4', open_history_cb, absolute_history[4]},
    {relative_history[5], fltk::CTRL+'5', open_history_cb, absolute_history[5]},
    {relative_history[6], fltk::CTRL+'6', open_history_cb, absolute_history[6]},
    {relative_history[7], fltk::CTRL+'7', open_history_cb, absolute_history[7]},
    {relative_history[8], fltk::CTRL+'8', open_history_cb, absolute_history[8]},
    {relative_history[9], fltk::CTRL+'9', open_history_cb, absolute_history[9], FL_MENU_DIVIDER},
    {"Quit", fltk::CTRL+'q', exit_cb},
    {0},
    {"&Edit",0,0,0,FL_SUBMENU},
    //  {"Undo", fltk::ALT+'z', nyi},
    {"Cut", fltk::CTRL+'x', cut_cb},
    {"Copy", fltk::CTRL+'c', copy_cb},
    {"Paste", fltk::CTRL+'v', paste_cb},
    {"Select All", fltk::CTRL+'a', select_all_cb, 0, FL_MENU_DIVIDER},
    {"Edit this widget", fltk::ReturnKey, openwidget_cb},
    {"Sort these widgets", 0, sort_cb},
    {"Move widget earlier", fltk::F2Key, earlier_cb},
    {"Move widget later", fltk::F3Key, later_cb},
    //{"Show", fltk::F5Key, show_cb},
    //{"Hide", fltk::F6Key, hide_cb},
    {"Group", fltk::F7Key, group_cb},
    {"Ungroup", fltk::F8Key, ungroup_cb,0, FL_MENU_DIVIDER},
    //{"Deactivate", 0, nyi},
    //{"Activate", 0, nyi, 0, FL_MENU_DIVIDER},
    {"Show Overlays",fltk::ALT+'o',toggle_overlays,0,FL_MENU_TOGGLE|FL_MENU_VALUE},
    {"Preferences",fltk::CTRL+'p',show_alignment_cb},
    {"Coding Style", 0, show_coding_style_cb},
    {"Theme", 0, theme_cb},
    {"Set images root directory", fltk::CTRL+'d', set_images_dir_cb},
    {0},
    {"&New", 0, 0, (void *)New_Menu, FL_SUBMENU_POINTER},
    {"&Plugins", 0, 0, (void *)Plugins_Options_Menu, FL_SUBMENU_POINTER},
    {"&Help",0,0,0,FL_SUBMENU},
    {"About fluid",0,about_cb},
    {"Tooltips", 0, tt_cb, 0, FL_MENU_TOGGLE|FL_MENU_VALUE},
    //{"Manual",0,nyi},
    {0},
    {0}};
    
#define BROWSERWIDTH 300
#define BROWSERHEIGHT 500
#define WINWIDTH 300
#define MENUHEIGHT 23
#define WINHEIGHT (BROWSERHEIGHT+MENUHEIGHT)
    
    extern void fill_in_New_Menu();

    fltk::MenuBar* menubar;
    
    ////////////////////////////////////////////////////////////////
    void make_main_window() {
	if (!main_window) {
	    fltk::Widget *o;
	    main_window = new fltk::Window(WINWIDTH,WINHEIGHT,"fluid");
	    main_window->box(fltk::NO_BOX);
	    main_window->begin();
	    o = make_widget_browser(0,MENUHEIGHT,BROWSERWIDTH,BROWSERHEIGHT);
	    //  o->text_box(fltk::FLAT_BOX);
	    main_window->resizable(o);
	    menubar = new fltk::MenuBar(0,0,BROWSERWIDTH,MENUHEIGHT);
	    menubar->box(fltk::FLAT_BOX);
	    
#ifdef NEWM
	    build_hierarchy(menubar);
#else
	    fill_in_New_Menu();
	    Main_Menu->add_to(menubar);
#endif
	    // this is removed because the new ctrl+bindings mess up emacs in
	    // the text fields:
	    //    menubar->global();
	    main_window->end();
	    load_history();
	}
    }
    
    ////////////////////////////////////////////////////////////////
    void set_filename(const char *c) {
	if (filename) free((void *)filename);
	filename = strdup(c);
	if (main_window) main_window->label(filename);
	
	/* Change directory to .fl file directory
	* so generated .h/.cxx files goes there also.
	*/
	char curdir[1024]; 
	const char *start = c;
	const char *end = fltk::filename_name(c);
	int len = end-start;
	strncpy(curdir, start, len);
	curdir[len] = 0;
	chdir(curdir);
    }
    
    ////////////////////////////////////////////////////////////////
    // Load file history from preferences...
    void load_history() {
	int	i;		// Looping var
	int	max_files;
	
	fluid_prefs.get("recent_files", max_files, 5);
	if (max_files > 10) max_files = 10;
	
	for (i = 0; i < max_files; i ++) {
	    fluid_prefs.get( fltk::Preferences::Name("file%d", i), absolute_history[i], "", sizeof(absolute_history[i]));
	    if (absolute_history[i][0]) {
		// Make a relative version of the filename for the menu...
		fltk::filename_relative(relative_history[i], sizeof(relative_history[i]),
		    absolute_history[i]);
#ifdef NEWM
		history_item[i]->show();
#else
		if (i == 9) (*(fltk::Item*)&Main_Menu[i + HISTORY_ITEM]).flags = FL_MENU_DIVIDER;
		else Main_Menu[i + HISTORY_ITEM].flags = 0;
#endif
	    } else break;
	}
	
	for (; i < 10; i ++) {
#ifdef NEWM
	    history_item[i]->hide();
#else
	    if (i) Main_Menu[i + HISTORY_ITEM - 1].flags |= FL_MENU_DIVIDER;
	    Main_Menu[i + HISTORY_ITEM].hide();
#endif
	}
	menubar->redraw();
    }
    
    // Update file history from preferences...
    void update_history(const char *flname) {
	int	i;		// Looping var
	char	absolute[1024];
	int	max_files;
	
	
	fluid_prefs.get("recent_files", max_files, 5);
	if (max_files > 10) max_files = 10;
	
	fltk::filename_absolute(absolute, sizeof(absolute), flname);
	
	for (i = 0; i < max_files; i ++)
#if defined(WIN32) || defined(__APPLE__)
	    if (!strcasecmp(absolute, absolute_history[i])) break;
#else
	    if (!strcmp(absolute, absolute_history[i])) break;
#endif // WIN32 || __APPLE__
	    
	    if (i == 0) return;
	    
	    if (i >= max_files) i = max_files - 1;
	    
	    // Move the other flnames down in the list...
	    memmove(absolute_history + 1, absolute_history,
		i * sizeof(absolute_history[0]));
	    memmove(relative_history + 1, relative_history,
		i * sizeof(relative_history[0]));
	    
	    // Put the new file at the top...
	    strlcpy(absolute_history[0], absolute, sizeof(absolute_history[0]));
	    
	    fltk::filename_relative(relative_history[0], sizeof(relative_history[0]),
		absolute_history[0]);
	    
	    // Update the menu items as needed...
	    for (i = 0; i < max_files; i ++) {
		fluid_prefs.set( fltk::Preferences::Name("file%d", i), absolute_history[i]);
		if (absolute_history[i][0]) {
		    //if (i == 9) Main_Menu[i + HISTORY_ITEM].flags = FL_MENU_DIVIDER;
		    //else Main_Menu[i + HISTORY_ITEM].flags = 0;
		} else break;
	    }
	    
	    for (; i < 10; i ++) {
		fluid_prefs.set( fltk::Preferences::Name("file%d", i), "");
		//if (i) Main_Menu[i + HISTORY_ITEM - 1].flags |= FL_MENU_DIVIDER;
		Main_Menu[i + HISTORY_ITEM].hide();
	    }
    }
    ////////////////////////////////////////////////////////////////
    
    static int arg(int argc, char** argv, int& i) {
	if (argv[i][1] == 'c' && !argv[i][2]) {compile_only = 1; i++; return 1;}
	if (argv[i][1] == 'o' && !argv[i][2] && i+1 < argc) {
	    code_file_name = argv[i+1];
	    code_file_set  = 1;
	    i += 2;
	    return 2;
	}
	if (argv[i][1] == 'h' && !argv[i][2]) {
	    header_file_name = argv[i+1];
	    header_file_set  = 1;
	    i += 2;
	    return 2;
	}
	return 0;
    }
    
#ifndef _WIN32
    
#include <signal.h>
#ifdef _sigargs
#define SIGARG _sigargs
#else
#ifdef __sigargs
#define SIGARG __sigargs
#else
#define SIGARG int // you may need to fix this for older systems
#endif
#endif
    
    extern "C" {
	static void sigint(SIGARG) {
	    signal(SIGINT,sigint);
	    exit_cb(0,0);
	}
    }
#endif
    
    int main(int argc,char **argv) {
	int i = 1;
	if (!fltk::args(argc,argv,i,arg) || i < argc-1) {
	    fprintf(stderr,"usage: %s <switches> name.fl\n"
		" -c : write .cxx and .h and exit\n"
		" -o <name> : .cxx output filename, or extension if <name> starts with '.'\n"
		" -h <name> : .h output filename, or extension if <name> starts with '.'\n"
		"%s\n", argv[0], fltk::help);
	    return 1;
	}
	const char *c = argv[i];
	
	fluid_style_set = new fltk::StyleSet();
	style_set = new fltk::StyleSet();
	
	read_plugins();
	make_main_window();
	load_coding_style();
	
	if (c) set_filename(c);
	if (!compile_only) {
	    fltk::visual(fltk::DOUBLE_BUFFER|fltk::INDEXED_COLOR);
	    fltk::FileIcon::load_system_icons();
	    main_window->callback(exit_cb);
	    main_window->show(argc,argv);
	}
	
	if (c && !read_file(c,0)) {
	    if (compile_only) {
		fprintf(stderr,"%s : %s\n", c, strerror(errno));
		exit(1);
	    }
	    fltk::message("Can't read %s: %s", c, strerror(errno));
	}
	if (compile_only) {write_cb(0,0); exit(0);}
	modflag = 0;
#ifndef _WIN32
	signal(SIGINT,sigint);
#endif
	return fltk::run();
    }
    
    //
    // End of "$Id$".
    //
