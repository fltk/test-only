//
// "$Id: fluid.cxx,v 1.59 2002/10/26 09:55:30 spitzak Exp $"
//
// FLUID main entry for the Fast Light Tool Kit (FLTK).
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
"Please report bugs to fltk-bugs@easysw.com.";

#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include <fltk/Fl_Box.h>
#include <fltk/Fl_Button.h>
#include <fltk/Fl_Hold_Browser.h>
#include <fltk/Fl_Menu_Bar.h>
#include <fltk/Fl_Input.h>
#include <fltk/Fl_Tooltip.h>
#include <fltk/fl_ask.h>
#include <fltk/fl_draw.h>
#include <fltk/fl_file_chooser.h>
#include <fltk/fl_message.h>
#include <fltk/filename.h>
#include <fltk/Fl_FileIcon.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifdef WIN32
#include <fltk/win32.h>	// for MAX_PATH definition
#endif

#if defined(_WIN32) && !defined(__CYGWIN__)
# include <direct.h>
#else
# include <unistd.h>
#endif

#include "about_panel.h"

#include "Fluid_Plugins.h"
#include "Fl_Type.h"
#include "coding_style.h"

////////////////////////////////////////////////////////////////

void nyi(Fl_Widget *,void *) {
    fl_message("That's not yet implemented, sorry");
}

static const char *filename;
void set_filename(const char *c);
int modflag;

static char* pwd;
static char in_source_dir;
void goto_source_dir() {
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
  Fl_Shared_Image::set_root_directory(buffer);
}

#include "Fluid_Image.h"
void goto_images_dir() {
  if (in_source_dir) return;
  if (!filename || !*filename) return;
  const char *p = filename_name(filename);
  // This is static since Fl_Shared_Image::set_root_directory just copies the pointer
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

  // Call to fl_set_images_root_directory so that images are corretly displayed in FLUID.
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
  Fl_Shared_Image::set_root_directory(buffer);
}

void leave_source_dir() {
  if (!in_source_dir) return;
  if (chdir(pwd)<0) {fprintf(stderr, "Can't chdir to %s : %s\n",
			     pwd, strerror(errno));}
  in_source_dir = 0;
}
  
Fl_Window *main_window;

void save_cb(Fl_Widget *, void *v) {
  const char *c = filename;
  if (v || !c || !*c) {
    if (!(c=fl_file_chooser("Save to:", "*.f[ld]", c))) return;
    set_filename(c);
  }
  if (!write_file(c)) {
    fl_message("Error writing %s: %s", c, strerror(errno));
    return;
  }
  modflag = 0;
}

void exit_cb(Fl_Widget *,void *) {
  if (modflag)
    switch (fl_choice("Save changes before exiting?", "Yes", "No", "Cancel")) {
      case 2 : /* Cancel */
          return;
      case 0 : /* Yes */
          save_cb(NULL, NULL);
	  if (modflag) return;	// Didn't save!
    }

  exit(0);
}

void open_cb(Fl_Widget *, void *v) {
  if (!v && modflag && !fl_ask("Discard changes?")) return;
  const char *c;
  if (!(c = fl_file_chooser("Open:", "*.f[ld]", filename))) return;
  if (!v) set_filename(c);
  if (!read_file(c, v!=0)) {
    fl_message("Can't read %s: %s", c, strerror(errno));
    return;
  }
  if (!v) modflag = 0;
  else modflag = 1;
}

void new_cb(Fl_Widget *, void *v) {
  if (!v && modflag && !fl_ask("Discard changes?")) return;
  const char *c;
  if (!(c = fl_file_chooser("New:", "*.f[ld]", 0))) return;
  delete_all();
  set_filename(c);
  modflag = 0;
}

static int compile_only = 0;
int header_file_set = 0;
int code_file_set = 0;
const char* header_file_name = ".h";
const char* code_file_name = ".cxx";

void write_cb(Fl_Widget *, void *) {
  if (!filename) {
    save_cb(0,0);
    if (!filename) return;
  }
  char cname[1024];
  char hname[1024];
  if (*code_file_name == '.') {
    strcpy(cname,filename_name(filename));
    filename_setext(cname, code_file_name);
  } else {
    strcpy(cname, code_file_name);
  }
  if (*header_file_name == '.') {
    strcpy(hname,filename_name(filename));
    filename_setext(hname, header_file_name);
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
      fl_message("Can't write %s: %s", cname, strerror(errno));
    } else {
      fl_message("Wrote %s", cname, 0);
    }
  }
}

void openwidget_cb(Fl_Widget *, void *) {
  if (!Fl_Type::current) {
    fl_message("Please select a widget");
    return;
  }
  Fl_Type::current->open();
}

void toggle_overlays(Fl_Widget *,void *);

void select_all_cb(Fl_Widget *,void *);

void group_cb(Fl_Widget *, void *);

void ungroup_cb(Fl_Widget *, void *);

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
  static char name[256] = "~/.fluid_cut_buffer";
  static char beenhere;
  if (!beenhere) {beenhere = 1; filename_expand(name,name);}
  return name;
#endif
}

void copy_cb(Fl_Widget*, void*) {
  if (!Fl_Type::current) return;
  ipasteoffset = 10;
  if (!write_file(cutfname(),1)) {
    fl_message("Can't write %s: %s", cutfname(), strerror(errno));
    return;
  }
}

extern void select_only(Fl_Type *);
void cut_cb(Fl_Widget *, void *) {
  if (!Fl_Type::current) return;
  ipasteoffset = 0;
  Fl_Type *p = Fl_Type::current->parent;
  while (p && p->selected) p = p->parent;
  if (!write_file(cutfname(),1)) {
    fl_message("Can't write %s: %s", cutfname(), strerror(errno));
    return;
  }
  delete_all(1);
  if (p) select_only(p);
}

extern int force_parent, gridx, gridy;

void paste_cb(Fl_Widget*, void*) {
  if (ipasteoffset) force_parent = 1;
  pasteoffset = ipasteoffset;
  if (gridx>1) pasteoffset = ((pasteoffset-1)/gridx+1)*gridx;
  if (gridy>1) pasteoffset = ((pasteoffset-1)/gridy+1)*gridy;
  if (!read_file(cutfname(), 1)) {
    fl_message("Can't read %s: %s", cutfname(), strerror(errno));
  }
  pasteoffset = 0;
  ipasteoffset += 10;
  force_parent = 0;
}

void earlier_cb(Fl_Widget*,void*);

void later_cb(Fl_Widget*,void*);

void sort(Fl_Type *parent); // in Fl_Widget_Type.cxx

static void sort_cb(Fl_Widget *,void *) {
  for (Fl_Type* f = Fl_Type::first; f; f = f->next_brother) sort(f);
}

void show_alignment_cb(Fl_Widget *, void *);

void set_images_dir_cb(Fl_Widget *, void *);

void about_cb(Fl_Widget *, void *) {
  if (!about_panel) make_about_panel(copyright);
  copyright_box->hide();
  display_group->show();
  about_panel->show();
}

void tt_cb(Fl_Widget *w, void *) {
  Fl_Tooltip::enable(w->value());
}

#include <string.h>
#include <fltk/fl_ask.h>

#include <fltk/Fl_Style_Set.h>
Fl_Style_Set* fluid_style_set;
Fl_Style_Set* style_set;
char* theme;

void set_theme(const char* s) {
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
  Fl_Theme f = Fl_Style::load_theme(theme);
  Fl_Style::theme(f);
  // set the scheme...
  Fl_Style::reload_theme();
  fluid_style_set->make_current();
  Fl::redraw();
  if (!f) fl_alert("Error loading theme \"%s\" (plugin not found?)",
		   theme ? theme : "");
}

void theme_cb(Fl_Widget *, void *) {
  const char* s = fl_input("Enter the theme name:", theme);
  if (!s) return;
  set_theme(s);
}

////////////////////////////////////////////////////////////////
#include <fltk/Fl_Menu_Item.h>

extern Fl_Menu_Item New_Menu[];

Fl_Menu_Item Main_Menu[] = {
{"&File",0,0,0,FL_SUBMENU},
  {"New", 0, new_cb, 0},
  {"Open...", FL_CTRL+'o', open_cb, 0},
  {"Save", FL_CTRL+'s', save_cb, 0},
  {"Save As...", FL_CTRL+'S', save_cb, (void*)1},
  {"Merge...", FL_CTRL+'i', open_cb, (void*)1, FL_MENU_DIVIDER},
  {"Write code", FL_CTRL+'w', write_cb, 0},
  {"Quit", FL_CTRL+'q', exit_cb},
  {0},
{"&Edit",0,0,0,FL_SUBMENU},
//  {"Undo", FL_ALT+'z', nyi},
  {"Cut", FL_CTRL+'x', cut_cb},
  {"Copy", FL_CTRL+'c', copy_cb},
  {"Paste", FL_CTRL+'v', paste_cb},
  {"Select All", FL_CTRL+'a', select_all_cb, 0, FL_MENU_DIVIDER},
  {"Edit this widget", FL_Enter, openwidget_cb},
  {"Sort these widgets", 0, sort_cb},
  {"Move widget earlier", FL_F(2), earlier_cb},
  {"Move widget later", FL_F(3), later_cb},
//{"Show", FL_F(5), show_cb},
//{"Hide", FL_F(6), hide_cb},
  {"Group", FL_F(7), group_cb},
  {"Ungroup", FL_F(8), ungroup_cb,0, FL_MENU_DIVIDER},
//{"Deactivate", 0, nyi},
//{"Activate", 0, nyi, 0, FL_MENU_DIVIDER},
  {"Show Overlays",FL_ALT+'o',toggle_overlays,0,FL_MENU_TOGGLE|FL_MENU_VALUE},
  {"Preferences",FL_CTRL+'p',show_alignment_cb},
  {"Coding Style", 0, show_coding_style_cb},
  {"Theme", 0, theme_cb},
  {"Set images root directory", FL_CTRL+'d', set_images_dir_cb},
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

Fl_Menu_Bar* menubar;

void make_main_window() {
  if (!main_window) {
    Fl_Widget *o;
    main_window = new Fl_Window(WINWIDTH,WINHEIGHT,"fluid");
    main_window->box(FL_NO_BOX);
    o = make_widget_browser(0,MENUHEIGHT,BROWSERWIDTH,BROWSERHEIGHT);
//    o->text_box(FL_FLAT_BOX);
    main_window->resizable(o);
    menubar = new Fl_Menu_Bar(0,0,BROWSERWIDTH,MENUHEIGHT);
    menubar->box(FL_FLAT_BOX);
    fill_in_New_Menu();
    Main_Menu->add_to(menubar);
    // this is removed because the new ctrl+bindings mess up emacs in
    // the text fields:
    //    menubar->global();
    main_window->end();
  }
}

void set_filename(const char *c) {
  if (filename) free((void *)filename);
  filename = strdup(c);
  if (main_window) main_window->label(filename);
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
  if (!Fl::args(argc,argv,i,arg) || i < argc-1) {
    fprintf(stderr,"usage: %s <switches> name.fl\n"
" -c : write .cxx and .h and exit\n"
" -o <name> : .cxx output filename, or extension if <name> starts with '.'\n"
" -h <name> : .h output filename, or extension if <name> starts with '.'\n"
"%s\n", argv[0], Fl::help);
    return 1;
  }
  const char *c = argv[i];

  fluid_style_set = new Fl_Style_Set();
  style_set = new Fl_Style_Set();

  read_plugins();
  make_main_window();
  load_coding_style();

  if (c) set_filename(c);
  if (!compile_only) {
    Fl::visual(FL_DOUBLE|FL_INDEX);
    Fl_FileIcon::load_system_icons();
    main_window->callback(exit_cb);
    main_window->show(argc,argv);
  }

  if (c && !read_file(c,0)) {
    if (compile_only) {
      fprintf(stderr,"%s : %s\n", c, strerror(errno));
      exit(1);
    }
    fl_message("Can't read %s: %s", c, strerror(errno));
  }
  if (compile_only) {write_cb(0,0); exit(0);}
  modflag = 0;
#ifndef _WIN32
  signal(SIGINT,sigint);
#endif
  return Fl::run();
}

//
// End of "$Id: fluid.cxx,v 1.59 2002/10/26 09:55:30 spitzak Exp $".
//
