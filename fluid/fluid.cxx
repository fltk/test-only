//
// "$Id$"
//
// FLUID main entry for the Fast Light Tool Kit (FLTK).
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

const char *copyright =
"The FLTK user interface designer version 2.0\n"
"Copyright 1998-2006 by Bill Spitzak and others.\n"
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
#include <fltk/MenuBuild.h>
#include <fltk/string.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
# include <fltk/win32.h>	// for MAX_PATH definition
# include <direct.h>
#else
# include <unistd.h>
#endif

#include "about_panel.h"
#include "alignment_panel.h"
#include "widget_panel.h"

#include "Fluid_Plugins.h"
#include "FluidType.h"
#include "coding_style.h"
#include "fluid_menus.h"
#include "factory.h"
#include "undo.h"

using namespace fltk;
/////////////////////////////////////////
// Read preferences file 
Preferences fluid_prefs(Preferences::USER, "fltk.org", "fluid");

int gridx;
int gridy;
int snap;
int show_tooltip;

static int  read_alignement_prefs() {
    fluid_prefs.get("snap", snap, 3);
    fluid_prefs.get("gridx", gridx, 5);
    fluid_prefs.get("gridy", gridy, 5);
    fluid_prefs.get("show_tooltips", show_tooltip, 1);
    Tooltip::enable(show_tooltip ? true : false);
    
    return 0;
}
static int dummy = read_alignement_prefs();
/////////////////////////////////////////

// File history info...
char	absolute_history[10][1024];
char	relative_history[10][1024];

void	load_history();
void	update_history(const char *);
void	set_alignment_window();

////////////////////////////////////////////////////////////////

void nyi(Widget *,void *) {
    message("That's not yet implemented, sorry");
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
    SharedImage::set_root_directory(buffer);
#endif
}

#include "Fluid_Image.h"
void goto_images_dir() {
#if 0
    if (in_source_dir) return;
    if (!filename || !*filename) return;
    const char *p = filename_name(filename);
    // This is static since SharedImage::set_root_directory just copies the pointer
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
    
    // Call to set_images_root_directory so that images are corretly displayed in FLUID.
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
    SharedImage::set_root_directory(buffer);
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

Window *main_window;

char position_window(Window *w, const char *prefsName, int Visible, int X, int Y, int W=0, int H=0 ) {
    Preferences pos(fluid_prefs, prefsName);
    if (prevpos_button && prevpos_button->value() ) {
	pos.get("x", X, X);
	pos.get("y", Y, Y);
	if ( W!=0 ) {
	    pos.get("w", W, W);
	    pos.get("h", H, H);
	    w->resize( X, Y, W, H );
	}
	else
	    w->position( X, Y );
    }
    pos.get("visible", Visible, Visible);
    return Visible;
}
void save_position(Window *w, const char *prefsName) {
    Preferences pos(fluid_prefs, prefsName);
    pos.set("x", w->x());
    pos.set("y", w->y());
    pos.set("w", w->w());
    pos.set("h", w->h());
    pos.set("visible", (int)(w->shown() && w->visible()));
}

void save_cb(Widget *, void *v) {
    const char *c = filename;
    if (v || !c || !*c) {
	if (!(c=file_chooser("Save to:", "*.f[ld]", c))) return;
	set_filename(c);
    }
    if (!write_file(c)) {
	message("Error writing %s: %s", c, strerror(errno));
	return;
    }
    modflag = 0;
}

void exit_cb(Widget *,void *) {
    if (modflag)
	switch (choice("Save changes before exiting?", "Yes", "No", "Cancel")) {
      case 2 : /* Cancel */
          return;
      case 0 : /* Yes */
          save_cb(NULL, NULL);
	  if (modflag) return;	// Didn't save!
    }
    
    if (widgetbin_panel) {
	save_position(widgetbin_panel,"widgetbin_pos");
	delete widgetbin_panel;
    }
    exit(0);
}

void open_cb(Widget *, void *v) {
    if (!v && modflag && !ask("Discard changes?")) return;
    const char *c;
    if (!(c = file_chooser("Open:", "*.f[ld]", filename))) return;
    Undo::suspend();

    if (!v) set_filename(c);
    if (!read_file(c, v!=0)) {
	message("Can't read %s: %s", c, strerror(errno));
    } else {
	if (!v) modflag = 0;
	else modflag = 1;
    }
    Undo::resume();
    Undo::clear();
}

void open_history_cb(Widget *, void *v) {
    if (modflag) {
	switch (choice("Do you want to save changes to this user\n"
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
    Undo::suspend(); 
    if (!read_file(filename, 0)) {
	Undo::resume();
	Undo::clear();
	message("Can't read %s: %s", filename, strerror(errno));
	free((void *)filename);
	filename = oldfilename;
	if (main_window) main_window->label(filename);
	return;
    }
    modflag = 0;
    Undo::resume();
    Undo::clear();
    if (oldfilename) free((void *)oldfilename);
}

void new_cb(Widget *, void *v) {
    if (!v && modflag && !ask("Discard changes?")) return;
    const char *c;
    if (!(c = file_chooser("New:", "*.f[ld]", 0))) return;
    delete_all();
    set_filename(c);
    modflag = 0;
}

static int compile_only = 0;
int header_file_set = 0;
int code_file_set = 0;
const char* header_file_name = ".h";
const char* code_file_name = ".cxx";

void write_cb(Widget *, void *) {
    if (!filename) {
	save_cb(0,0);
	if (!filename) return;
    }
    char cname[1024];
    char hname[1024];
    if (code_file_name[0] == '.' && isalpha(code_file_name[1])) {
	strcpy(cname,filename_name(filename));
	strcpy((char*)filename_ext(cname), code_file_name);
    } else {
	strcpy(cname, code_file_name);
    }
    if (header_file_name[0] == '.' && isalpha(header_file_name[1])) {
	strcpy(hname,filename_name(filename));
	strcpy((char*)filename_ext(hname), header_file_name);
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
	    message("Can't write %s: %s", cname, strerror(errno));
	} else {
	    message("Wrote %s", cname, 0);
	}
    }
}

void openwidget_cb(Widget *, void *) {
    if (!FluidType::current) {
	message("Please select a widget");
	return;
    }
    FluidType::current->open();
}

void toggle_overlays(Widget *,void *);

void select_all_cb(Widget *,void *);

void group_cb(Widget *, void *);

void ungroup_cb(Widget *, void *);

extern int pasteoffset;
static int ipasteoffset;

static char* cutfname() {
#if defined (_WIN32) && !defined(__CYGWIN__)
    static char name[1024] = "";
    if (!name[0]) {
	if (!GetTempPath(sizeof(name), name)) strcpy(name,"\\"); // failure
	strcat(name, ".fluidcutbuffer");
    }
    return name;
#else
    static char name[1024] = "";
    if (!name[0]) filename_absolute(name, 1024, "~/.fluid_cut_buffer");
    return name;
#endif
}

void copy_cb(Widget*, void*) {
    if (!FluidType::current) return;
    Undo::checkpoint();
    ipasteoffset = 10;
    if (!write_file(cutfname(),1)) {
	message("Can't write %s: %s", cutfname(), strerror(errno));
	return;
    }
}

extern void select_only(FluidType *);
void cut_cb(Widget *, void *) {
    if (!FluidType::current) return;
    Undo::checkpoint();
    ipasteoffset = 0;
    FluidType *p = FluidType::current->parent;
    while (p && p->selected) p = p->parent;
    if (!write_file(cutfname(),1)) {
	message("Can't write %s: %s", cutfname(), strerror(errno));
	return;
    }
    delete_all(1);
    if (p) select_only(p);
}

extern int force_parent, gridx, gridy;

void paste_cb(Widget*, void*) {
    Undo::checkpoint();
    if (ipasteoffset) force_parent = 1;
    pasteoffset = ipasteoffset;
    if (gridx>1) pasteoffset = ((pasteoffset-1)/gridx+1)*gridx;
    if (gridy>1) pasteoffset = ((pasteoffset-1)/gridy+1)*gridy;
    if (!read_file(cutfname(), 1)) {
	message("Can't read %s: %s", cutfname(), strerror(errno));
    }
    pasteoffset = 0;
    ipasteoffset += 10;
    force_parent = 0;
}

void earlier_cb(Widget*,void*);
void later_cb(Widget*,void*);
void sort(FluidType *parent); // in WidgetType.cxx

void sort_cb(Widget *,void *) {
    FluidType* f = FluidType::first;
    if(!f) return;
    Undo::checkpoint();
    for (; f; f = f->next_brother) sort(f);
}

void show_alignment_cb(Widget *, void *);

void set_images_dir_cb(Widget *, void *);

void about_cb(Widget *, void *) {
    if (!about_panel) make_about_panel(copyright);
    copyright_box->hide();
    display_group->show();
    about_panel->show();
}

void tt_cb(Widget *w, void *) {
    show_tooltip = ((Button*)w)->value(); 
    Tooltip::enable(show_tooltip ? true : false);
    fluid_prefs.set("show_tooltips", show_tooltip );
}

#include <string.h>
#include <fltk/ask.h>

#include <fltk/StyleSet.h>
StyleSet* fluid_style_set;
StyleSet* style_set;
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
    Theme f = Style::load_theme(theme);
    Style::theme(f);
    // set the scheme...
    Style::reload_theme();
    fluid_style_set->make_current();
    redraw();
    if (!f) alert("Error loading theme \"%s\" (plugin not found?)",
	theme ? theme : "");
#endif
}

void theme_cb(Widget *, void *) {
    const char* s = input("Enter the theme name:", ::theme);
    if (!s) return;
    set_theme(s);
}


void toggle_widgetbin_cb(Widget *o, void * v) {
    if (!widgetbin_panel) {
	make_widgetbin();
	widgetbin_panel->callback(toggle_widgetbin_cb);
	if (!position_window(widgetbin_panel,"widgetbin_pos", 1, 320, 30)) return;
    }
    
    if (widgetbin_panel->visible()) {
	widgetbin_panel->hide();
	iwidget_bin->label("Show Widget &Bin...");
    } else {
	widgetbin_panel->show();
	iwidget_bin->label("Hide Widget &Bin");
    }
}


////////////////////////////////////////////////////////////////
// New fltk2 menu generation
////////////////////////////////////////////////////////////////


#define BROWSERWIDTH 300
#define BROWSERHEIGHT 500
#define WINWIDTH 300
#define MENUHEIGHT 23
#define WINHEIGHT (BROWSERHEIGHT+MENUHEIGHT)


MenuBar* menubar;
Browser *widget_browser;

////////////////////////////////////////////////////////////////
void make_main_window() {
    if (!main_window) {
	Widget *o;
	main_window = new Window(WINWIDTH,WINHEIGHT,"fluid");
	main_window->box(NO_BOX);
	main_window->begin();
	o = widget_browser = (Browser *) make_widget_browser(0,MENUHEIGHT,BROWSERWIDTH,BROWSERHEIGHT);
	//  o->text_box(FLAT_BOX);
	main_window->resizable(o);
	menubar = new MenuBar(0,0,BROWSERWIDTH,MENUHEIGHT);
	menubar->box(FLAT_BOX);
	build_hierarchy(menubar);
	if (show_tooltip) itooltip->set_flag(VALUE);
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
    if (c && strlen(c)) update_history(c);
    
    if (main_window) main_window->label(filename);
    
    /* Change directory to .fl file directory
    * so generated .h/.cxx files goes there also.
    */
    char curdir[1024]; 
    const char *start = c;
    const char *end = filename_name(c);
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
	fluid_prefs.get( Preferences::Name("file%d", i), absolute_history[i], "", sizeof(absolute_history[i]));
	if (absolute_history[i][0]) {
	    // Make a relative version of the filename for the menu...
	    filename_relative(relative_history[i], sizeof(relative_history[i]),
		absolute_history[i]);
	    history_item[i]->show();
	} else break;
    }
    
    for (; i < 10; i ++) {
	history_item[i]->hide();
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
    
    filename_absolute(absolute, sizeof(absolute), flname);
    
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
	
	filename_relative(relative_history[0], sizeof(relative_history[0]),
	    absolute_history[0]);
	
	// Update the menu items as needed...
	for (i = 0; i < max_files; i ++) {
	    fluid_prefs.set( Preferences::Name("file%d", i), absolute_history[i]);
	    if (!absolute_history[i][0]) break;
	}
	
	for (; i < 10; i ++) {
	    fluid_prefs.set( Preferences::Name("file%d", i), "");
	    history_item[i]->hide();
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
    if (!args(argc,argv,i,::arg) || i < argc-1) {
	fprintf(stderr,"usage: %s <switches> name.fl\n"
	    " -c : write .cxx and .h and exit\n"
	    " -o <name> : .cxx output filename, or extension if <name> starts with '.'\n"
	    " -h <name> : .h output filename, or extension if <name> starts with '.'\n"
	    "%s\n", argv[0], help);
	return 1;
    }
    const char *c = argv[i];
    
    fluid_style_set = new StyleSet();
    style_set = new StyleSet();
    
    register_images();
    read_plugins();
    make_main_window();
    load_coding_style();
    
    if (c) set_filename(c);
    if (!compile_only) {
	visual(DOUBLE_BUFFER|INDEXED_COLOR);
	FileIcon::load_system_icons();
	main_window->callback(exit_cb);
	main_window->show(argc,argv);
	set_alignment_window();
	toggle_widgetbin_cb(0,0);
	// FIXME NOT IMPLEMENTED: toggle_sourceview_cb(0,0);
	if (!c && openlast_button->value() && absolute_history[0][0]) {
	    // Open previous file when no file specified...
	    open_history_cb(0, absolute_history[0]);
	}
    }
    Undo::suspend();
    if (c && !read_file(c,0)) {
	if (compile_only) {
	    Undo::resume();
	    fprintf(stderr,"%s : %s\n", c, strerror(errno));
	    exit(1);
	}
	message("Can't read %s: %s", c, strerror(errno));
    }
    Undo::resume();
    if (compile_only) {write_cb(0,0); exit(0);}
    modflag = 0;
#ifndef _WIN32
    signal(SIGINT,sigint);
#endif
    return run();
}

//
// End of "$Id$".
//
