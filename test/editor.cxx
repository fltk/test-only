//
// "$Id: editor.cxx,v 1.4 2000/08/04 10:22:01 clip Exp $"
//
// A simple text editor program for the Fast Light Tool Kit (FLTK).
//
// This program is described in Chapter 4 of the FLTK Programmer's Guide.
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

//
// Include necessary headers...
//

#include <stdio.h>			// Standard library files
#include <stdlib.h>
#include <string.h>

#include <FL/Fl.H>			// Main FLTK header file
#include <FL/Fl_Group.H>		// Fl_Group header file
#include <FL/Fl_Window.H>		// Fl_Window header file
#include <FL/fl_ask.H>			// FLTK convenience functions
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>		// Fl_Menu_Bar header file
#include <FL/Fl_Input.H>		// Fl_Input header file
#include <FL/Fl_Button.H>		// Fl_Button header file
#include <FL/Fl_Return_Button.H>	// Fl_Return_Button header file
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>


Fl_Window          *window;
Fl_Menu_Bar        *menubar;
Fl_Text_Editor     *editor;
Fl_Window          *replace_dlg;
Fl_Input           *replace_find;
Fl_Input           *replace_with;
Fl_Button          *replace_all;
Fl_Return_Button   *replace_next;
Fl_Button          *replace_cancel;

int                changed = 0;
char               filename[1024] = "";
char               search[256] = "";


void set_changed(int);
void save_cb(void);
void saveas_cb(void);
void find2_cb(void);

int check_save(void) {
  if (!changed) return 1;

  if (fl_ask("The current file has not been saved.\n"
             "Would you like to save it now?")) {
    // Save the file...
    save_cb();

    return !changed;
  }
  else return (1);
}

void load_file(char *newfile) {
  FILE *fp;
  char buffer[8192];

  fp = fopen(newfile, "r");
  if (fp != NULL) {
    // Was able to open file; let's read from it...
    strcpy(filename, newfile);
    editor->buffer()->select(0, editor->buffer()->length());
    editor->buffer()->remove_selection();
    while (fgets(buffer, sizeof(buffer), fp))
      editor->buffer()->append(buffer);

    fclose(fp);
    set_changed(0);
    editor->show_insert_position();
  } else {
    // Couldn't open file - say so...
    fl_alert("Unable to open \'%s\' for reading!");
  }
}

void save_file(char *newfile) {
  FILE *fp;

  fp = fopen(newfile, "w");
  if (fp != NULL) {
    // Was able to create file; let's write to it...
    strcpy(filename, newfile);

    const char* line;
    for (int pos = 0;
         pos < editor->buffer()->length() &&
         ( line = editor->buffer()->line_text(pos) );
         pos = editor->buffer()->line_end(pos) + 1)
    {
      int r = fprintf(fp, "%s\n", line);
      free((void*)line);
      if (r < 1) {
        fl_alert("Unable to write file!");
        fclose(fp);
        return;
      }
    }

    fclose(fp);
    changed = 1;
    set_changed(0);
  } else {
    // Couldn't open file - say so...
    fl_alert("Unable to create \'%s\' for writing!");
  }
}

void set_changed(int c) {
  if (c != changed) {
    char title[1024];
    char *slash;

    changed = c;

    if (filename[0] == '\0') strcpy(title, "Untitled");
    else {
      slash = strrchr(filename, '/');
      if (slash == NULL) slash = strrchr(filename, '\\');

      if (slash != NULL) strcpy(title, slash + 1);
      else strcpy(title, filename);
    }

    if (changed) strcat(title, " (modified)");

    window->label(title);
  }
}

void changed_cb(int, int nInserted, int nDeleted,int, const char*, void*) {
  if (nInserted || nDeleted) set_changed(1);
}

void copy_cb(void) {
  Fl_Text_Editor::kf_copy(0, editor);
}

void cut_cb(void) {
  Fl_Text_Editor::kf_cut(0, editor);
}

void delete_cb(void) {
  editor->buffer()->remove_selection();
}

void find_cb(void) {
  const char *val;

  val = fl_input("Search String:", search);
  if (val != NULL) {
    // User entered a string - go find it!
    strcpy(search, val);
    find2_cb();
  }
}

void find2_cb(void) {
  if (search[0] == '\0') {
    // Search string is blank; get a new one...
    find_cb();
    return;
  }

  int pos = editor->insert_position();
  int found = editor->buffer()->search_forward(pos, search, &pos);
  if (found) {
    // Found a match; select and update the position...
    editor->buffer()->select(pos, pos+strlen(search));
    editor->insert_position(pos+strlen(search));
    editor->show_insert_position();
  }
  else fl_alert("No occurrences of \'%s\' found!", search);
}

void new_cb(void) {
  if (changed)
    if (!check_save()) return;

  filename[0] = '\0';
  editor->buffer()->select(0, editor->buffer()->length());
  editor->buffer()->remove_selection();
  set_changed(0);
}

void open_cb(void) {
  char *newfile;

  if (changed)
    if (!check_save()) return;

  newfile = fl_file_chooser("Open File?", "*", filename);
  if (newfile != NULL) load_file(newfile);
}

void paste_cb(void) {
  Fl_Text_Editor::kf_paste(0, editor);
}

void quit_cb(void) {
  if (changed)
    if (!check_save())
      return;

  window->hide();
}

void replace_cb(void) {
  replace_dlg->show();
}

void replace2_cb() {
  const char *find = replace_find->value();
  const char *replace = replace_with->value();

  if (find[0] == '\0') {
    // Search string is blank; get a new one...
    replace_dlg->show();
    return;
  }

  replace_dlg->hide();

  int pos = editor->insert_position();
  int found = editor->buffer()->search_forward(pos, find, &pos);

  if (found) {
    // Found a match; update the position and replace text...
    editor->buffer()->select(pos, pos+strlen(find));
    editor->buffer()->remove_selection();
    editor->buffer()->insert(pos, replace);
    editor->buffer()->select(pos, pos+strlen(replace));
    editor->insert_position(pos+strlen(replace));
    editor->show_insert_position();
  }
  else fl_alert("No occurrences of \'%s\' found!", find);
}

void replall_cb() {
  const char *find = replace_find->value();
  const char *replace = replace_with->value();

  find = replace_find->value();
  if (find[0] == '\0') {
    // Search string is blank; get a new one...
    replace_dlg->show();
    return;
  }

  replace_dlg->hide();

  editor->insert_position(0);
  int times = 0;

  // Loop through the whole string
  for (int found = 1; found;) {
    int pos = editor->insert_position();
    found = editor->buffer()->search_forward(pos, find, &pos);

    if (found) {
      // Found a match; update the position and replace text...
      editor->buffer()->select(pos, pos+strlen(find));
      editor->buffer()->remove_selection();
      editor->buffer()->insert(pos, replace);
      editor->insert_position(pos+strlen(replace));
      editor->show_insert_position();
      times++;
    }
  }

  if (times) fl_message("Replaced %d occurrences.", times);
  else fl_alert("No occurrences of \'%s\' found!", find);
}

void replcan_cb() {
  replace_dlg->hide();
}

void save_cb(void) {
  if (filename[0] == '\0') {
    // No filename - get one!
    saveas_cb();
    return;
  }
  else save_file(filename);
}

void saveas_cb(void) {
  char *newfile;

  newfile = fl_file_chooser("Save File As?", "*", filename);
  if (newfile != NULL) save_file(newfile);
}

void undo_cb(void) {
  fl_alert("Undo not implemented!");
}

Fl_Menu_Item menuitems[] = {
  { "&File", 0, 0, 0, FL_SUBMENU },
    { "&New",        FL_ALT + 'n', (Fl_Callback *)new_cb },
    { "&Open...",    FL_ALT + 'o', (Fl_Callback *)open_cb, 0, FL_MENU_DIVIDER },
    { "&Save",       FL_ALT + 's', (Fl_Callback *)save_cb },
    { "Save &As...", FL_ALT + FL_SHIFT + 's', (Fl_Callback *)saveas_cb, 0, FL_MENU_DIVIDER },
    { "&Quit", FL_ALT + 'q', (Fl_Callback *)quit_cb },
    { 0 },

  { "&Edit", 0, 0, 0, FL_SUBMENU },
    { "&Undo",       FL_ALT + 'z', (Fl_Callback *)undo_cb, 0, FL_MENU_DIVIDER },
    { "Cu&t",        FL_ALT + 'x', (Fl_Callback *)cut_cb },
    { "&Copy",       FL_ALT + 'c', (Fl_Callback *)copy_cb },
    { "&Paste",      FL_ALT + 'v', (Fl_Callback *)paste_cb },
    { "&Delete",     0, (Fl_Callback *)delete_cb },
    { 0 },

  { "&Search", 0, 0, 0, FL_SUBMENU },
    { "&Find...",       FL_ALT + 'f', (Fl_Callback *)find_cb },
    { "F&ind Again",    FL_ALT + 'g', (Fl_Callback *)find2_cb },
    { "&Replace...",    FL_ALT + 'r', (Fl_Callback *)replace_cb },
    { "Re&place Again", FL_ALT + 't', (Fl_Callback *)replace2_cb },
    { 0 },

  { 0 }
};

int main(int argc, char **argv) {
  Fl_Text_Buffer textbuf;
  textbuf.add_modify_callback(changed_cb, 0);

  window = new Fl_Window(512, 384, "Untitled");
    menubar = new Fl_Menu_Bar(0, 0, 512, 30);
    menubar->menu(menuitems);

    editor = new Fl_Text_Editor(0, 30, 512, 354);
    editor->buffer(textbuf);
    editor->text_font(FL_COURIER);
  window->end();
  window->resizable(editor);
  window->callback((Fl_Callback *)quit_cb);

  replace_dlg = new Fl_Window(300, 105, "Replace");
    replace_find = new Fl_Input(70, 10, 210, 25, "Find:");
	replace_find->clear_flag(FL_ALIGN_MASK);
    replace_find->set_flag(FL_ALIGN_LEFT);

    replace_with = new Fl_Input(70, 40, 210, 25, "Replace:");
	replace_with->clear_flag(FL_ALIGN_MASK);
    replace_with->set_flag(FL_ALIGN_LEFT);

    replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
    replace_all->callback((Fl_Callback *)replall_cb);

    replace_next = new Fl_Return_Button(105, 70, 120, 25, "Replace Next");
    replace_next->callback((Fl_Callback *)replace2_cb);

    replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");
    replace_cancel->callback((Fl_Callback *)replcan_cb);
  replace_dlg->end();
  replace_dlg->set_modal();

  window->show(1, argv);

  if (argc > 1) load_file(argv[1]);

  return Fl::run();
}

//
// End of "$Id: editor.cxx,v 1.4 2000/08/04 10:22:01 clip Exp $".
//
