//
// "$Id$"
//
// FLUID main entry for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

#ifndef FLUID3_H
#define FLUID3_H

#include <fltk3/filename.h>

namespace fltk3 {
  class Widget;
  class Window;
  class MenuBar;
  struct MenuItem;
  class Preferences;
  class Choice;
  class DoubleWindow;
  class Button;
  class ReturnButton;
  class TabGroup;
};


const int MAX_RET_BUF = 10;
const int BROWSERWIDTH = 300;
const int BROWSERHEIGHT = 500;
const int WINWIDTH = 300;
const int MENUHEIGHT = 25;
const int WINHEIGHT = BROWSERHEIGHT + MENUHEIGHT;


extern fltk3::Preferences fluid_prefs;
extern fltk3::MenuItem *save_item;
extern fltk3::MenuItem *history_item;
extern fltk3::MenuItem *widgetbin_item;
extern fltk3::MenuItem *sourceview_item;
extern fltk3::MenuItem Main_Menu[];
extern fltk3::Window *main_window;
extern fltk3::MenuBar *main_menubar;


extern int gridx;
extern int gridy;
extern int snap;
extern int show_guides;
extern int show_comments;
extern int show_coredevmenus;
extern int force_parent;
extern char absolute_history[10][FLTK3_PATH_MAX];
extern char relative_history[10][FLTK3_PATH_MAX];
extern char *tmp_ret_buf[MAX_RET_BUF];
extern int tmp_ret_sze[MAX_RET_BUF];
extern int curr_ret_buf;
extern int exit_early;
extern int compile_only;
extern int compile_strings;
extern int header_file_set;
extern int code_file_set;
extern const char* header_file_name;
extern const char* code_file_name;
extern int i18n_type;
extern const char* i18n_include;
extern const char* i18n_function;
extern const char* i18n_file;
extern const char* i18n_set;
extern char i18n_program[FLTK3_PATH_MAX];
extern const char *filename;
extern int modflag;


char *get_temporary_return_buffer(int size);
void goto_source_dir();
void leave_source_dir();
char project_is_workspace();
char position_window(fltk3::Window *w, const char *prefsName, int Visible, int X, int Y, int W=0, int H=0 );
void save_position(fltk3::Window *w, const char *prefsName);
void show_help(const char *name);
void fixup_coordinates();
void make_main_window();
void createRelativeFromAbsolutePath(int i);
void load_history();
void update_history(const char *flname);
void do_shell_command(fltk3::ReturnButton*, void*);
void show_shell_window();
void set_filename(const char *c);
void update_sourceview_position();
void update_sourceview_position_cb(fltk3::TabGroup*, void*);
void update_sourceview_cb(fltk3::Button*, void*);
void update_sourceview_timer(void*);
void set_modflag(int mf);


#endif

//
// End of "$Id$".
//
