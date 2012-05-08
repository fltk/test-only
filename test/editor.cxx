//
// "$Id$"
//
// A simple text editor program for the Fast Light Tool Kit (FLTK).
//
// This program is described in Chapter 4 of the FLTK Programmer's Guide.
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

//
// Include necessary headers...
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifdef __MWERKS__
# define fltk3::DLL
#endif

#include <fltk3/run.h>
#include <fltk3/Group.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/ask.h>
#include <fltk3/NativeFileChooser.h>
#include <fltk3/MenuBar.h>
#include <fltk3/Input.h>
#include <fltk3/Button.h>
#include <fltk3/ReturnButton.h>
#include <fltk3/TextBuffer.h>
#include <fltk3/TextEditor.h>
#include <fltk3/filename.h>

int                changed = 0;
char               filename[FLTK3_PATH_MAX] = "";
char               title[FLTK3_PATH_MAX];
fltk3::TextBuffer     *textbuf = 0;


// Syntax highlighting stuff...
#define TS 14 // default editor textsize
fltk3::TextBuffer     *stylebuf = 0;
fltk3::TextDisplay::StyleTableEntry
styletable[] = {	// Style table
  { fltk3::BLACK,      fltk3::COURIER,           TS }, // A - Plain
  { fltk3::DARK_GREEN, fltk3::HELVETICA_ITALIC,  TS }, // B - Line comments
  { fltk3::DARK_GREEN, fltk3::HELVETICA_ITALIC,  TS }, // C - Block comments
  { fltk3::BLUE,       fltk3::COURIER,           TS }, // D - Strings
  { fltk3::DARK_RED,   fltk3::COURIER,           TS }, // E - Directives
  { fltk3::DARK_RED,   fltk3::COURIER_BOLD,      TS }, // F - Types
  { fltk3::BLUE,       fltk3::COURIER_BOLD,      TS }, // G - Keywords
};
const char         *code_keywords[] = {	// List of known C/C++ keywords...
  "and",
  "and_eq",
  "asm",
  "bitand",
  "bitor",
  "break",
  "case",
  "catch",
  "compl",
  "continue",
  "default",
  "delete",
  "do",
  "else",
  "false",
  "for",
  "goto",
  "if",
  "new",
  "not",
  "not_eq",
  "operator",
  "or",
  "or_eq",
  "return",
  "switch",
  "template",
  "this",
  "throw",
  "true",
  "try",
  "while",
  "xor",
  "xor_eq"
};
const char         *code_types[] = {	// List of known C/C++ types...
  "auto",
  "bool",
  "char",
  "class",
  "const",
  "const_cast",
  "double",
  "dynamic_cast",
  "enum",
  "explicit",
  "extern",
  "float",
  "friend",
  "inline",
  "int",
  "long",
  "mutable",
  "namespace",
  "private",
  "protected",
  "public",
  "register",
  "short",
  "signed",
  "sizeof",
  "static",
  "static_cast",
  "struct",
  "template",
  "typedef",
  "typename",
  "union",
  "unsigned",
  "virtual",
  "void",
  "volatile"
};


//
// 'compare_keywords()' - Compare two keywords...
//

extern "C" {
  int
  compare_keywords(const void *a,
                   const void *b) {
    return (strcmp(*((const char **)a), *((const char **)b)));
  }
}

//
// 'style_parse()' - Parse text and produce style data.
//

void
style_parse(const char *text,
            char       *style,
	    int        length) {
  char	     current;
  int	     col;
  int	     last;
  char	     buf[255],
  *bufptr;
  const char *temp;
  
  // Style letters:
  //
  // A - Plain
  // B - Line comments
  // C - Block comments
  // D - Strings
  // E - Directives
  // F - Types
  // G - Keywords
  
  for (current = *style, col = 0, last = 0; length > 0; length --, text ++) {
    if (current == 'B' || current == 'F' || current == 'G') current = 'A';
    if (current == 'A') {
      // Check for directives, comments, strings, and keywords...
      if (col == 0 && *text == '#') {
        // Set style to directive
        current = 'E';
      } else if (strncmp(text, "//", 2) == 0) {
        current = 'B';
	for (; length > 0 && *text != '\n'; length --, text ++) *style++ = 'B';
        
        if (length == 0) break;
      } else if (strncmp(text, "/*", 2) == 0) {
        current = 'C';
      } else if (strncmp(text, "\\\"", 2) == 0) {
        // Quoted quote...
	*style++ = current;
	*style++ = current;
	text ++;
	length --;
	col += 2;
	continue;
      } else if (*text == '\"') {
        current = 'D';
      } else if (!last && (islower((*text)&255) || *text == '_')) {
        // Might be a keyword...
	for (temp = text, bufptr = buf;
	     (islower((*temp)&255) || *temp == '_') && bufptr < (buf + sizeof(buf) - 1);
	     *bufptr++ = *temp++);
        
        if (!islower((*temp)&255) && *temp != '_') {
	  *bufptr = '\0';
          
          bufptr = buf;
          
	  if (bsearch(&bufptr, code_types,
	              sizeof(code_types) / sizeof(code_types[0]),
		      sizeof(code_types[0]), compare_keywords)) {
	    while (text < temp) {
	      *style++ = 'F';
	      text ++;
	      length --;
	      col ++;
	    }
            
	    text --;
	    length ++;
	    last = 1;
	    continue;
	  } else if (bsearch(&bufptr, code_keywords,
	                     sizeof(code_keywords) / sizeof(code_keywords[0]),
		             sizeof(code_keywords[0]), compare_keywords)) {
	    while (text < temp) {
	      *style++ = 'G';
	      text ++;
	      length --;
	      col ++;
	    }
            
	    text --;
	    length ++;
	    last = 1;
	    continue;
	  }
	}
      }
    } else if (current == 'C' && strncmp(text, "*/", 2) == 0) {
      // Close a C comment...
      *style++ = current;
      *style++ = current;
      text ++;
      length --;
      current = 'A';
      col += 2;
      continue;
    } else if (current == 'D') {
      // Continuing in string...
      if (strncmp(text, "\\\"", 2) == 0) {
        // Quoted end quote...
	*style++ = current;
	*style++ = current;
	text ++;
	length --;
	col += 2;
	continue;
      } else if (*text == '\"') {
        // End quote...
	*style++ = current;
	col ++;
	current = 'A';
	continue;
      }
    }
    
    // Copy style info...
    if (current == 'A' && (*text == '{' || *text == '}')) *style++ = 'G';
    else *style++ = current;
    col ++;
    
    last = isalnum((*text)&255) || *text == '_' || *text == '.';
    
    if (*text == '\n') {
      // Reset column and possibly reset the style
      col = 0;
      if (current == 'B' || current == 'E') current = 'A';
    }
  }
}


//
// 'style_init()' - Initialize the style buffer...
//

void
style_init(void) {
  char *style = new char[textbuf->length() + 1];
  char *text = textbuf->text();
  
  memset(style, 'A', textbuf->length());
  style[textbuf->length()] = '\0';
  
  if (!stylebuf) stylebuf = new fltk3::TextBuffer(textbuf->length());
  
  style_parse(text, style, textbuf->length());
  
  stylebuf->text(style);
  delete[] style;
  free(text);
}


//
// 'style_unfinished_cb()' - Update unfinished styles.
//

void
style_unfinished_cb(int, void*) {
}


//
// 'style_update()' - Update the style buffer...
//

void
style_update(int        pos,		// I - Position of update
             int        nInserted,	// I - Number of inserted chars
	     int        nDeleted,	// I - Number of deleted chars
             int        /*nRestyled*/,	// I - Number of restyled chars
	     const char * /*deletedText*/,// I - Text that was deleted
             void       *cbArg) {	// I - Callback data
  int	start,				// Start of text
  end;				// End of text
  char	last,				// Last style on line
  *style,				// Style data
  *text;				// Text data
  
  
  // If this is just a selection change, just unselect the style buffer...
  if (nInserted == 0 && nDeleted == 0) {
    stylebuf->unselect();
    return;
  }
  
  // Track changes in the text buffer...
  if (nInserted > 0) {
    // Insert characters into the style buffer...
    style = new char[nInserted + 1];
    memset(style, 'A', nInserted);
    style[nInserted] = '\0';
    
    stylebuf->replace(pos, pos + nDeleted, style);
    delete[] style;
  } else {
    // Just delete characters in the style buffer...
    stylebuf->remove(pos, pos + nDeleted);
  }
  
  // Select the area that was just updated to avoid unnecessary
  // callbacks...
  stylebuf->select(pos, pos + nInserted - nDeleted);
  
  // Re-parse the changed region; we do this by parsing from the
  // beginning of the previous line of the changed region to the end of
  // the line of the changed region...  Then we check the last
  // style character and keep updating if we have a multi-line
  // comment character...
  start = textbuf->line_start(pos);
  //  if (start > 0) start = textbuf->line_start(start - 1);
  end   = textbuf->line_end(pos + nInserted);
  text  = textbuf->text_range(start, end);
  style = stylebuf->text_range(start, end);
  if (start==end)
    last = 0;
  else
    last  = style[end - start - 1];
  
  //  printf("start = %d, end = %d, text = \"%s\", style = \"%s\", last='%c'...\n",
  //         start, end, text, style, last);
  
  style_parse(text, style, end - start);
  
  //  printf("new style = \"%s\", new last='%c'...\n",
  //         style, style[end - start - 1]);
  
  stylebuf->replace(start, end, style);
  ((fltk3::TextEditor *)cbArg)->redisplay_range(start, end);
  
  if (start==end || last != style[end - start - 1]) {
    //    printf("Recalculate the rest of the buffer style\n");
    // Either the user deleted some text, or the last character
    // on the line changed styles, so reparse the
    // remainder of the buffer...
    free(text);
    free(style);
    
    end   = textbuf->length();
    text  = textbuf->text_range(start, end);
    style = stylebuf->text_range(start, end);
    
    style_parse(text, style, end - start);
    
    stylebuf->replace(start, end, style);
    ((fltk3::TextEditor *)cbArg)->redisplay_range(start, end);
  }
  
  free(text);
  free(style);
}


// Editor window functions and class...
void save_cb();
void saveas_cb();
void find2_cb(fltk3::Widget*, void*);
void replall_cb(fltk3::Widget*, void*);
void replace2_cb(fltk3::Widget*, void*);
void replcan_cb(fltk3::Widget*, void*);

class EditorWindow : public fltk3::DoubleWindow {
public:
  EditorWindow(int w, int h, const char* t);
  ~EditorWindow();
  
  fltk3::Window          *replace_dlg;
  fltk3::Input           *replace_find;
  fltk3::Input           *replace_with;
  fltk3::Button          *replace_all;
  fltk3::ReturnButton   *replace_next;
  fltk3::Button          *replace_cancel;
  
  fltk3::TextEditor     *editor;
  char               search[256];
};

EditorWindow::EditorWindow(int w, int h, const char* t) : fltk3::DoubleWindow(w, h, t) {
  replace_dlg = new fltk3::Window(300, 105, "Replace");
  replace_find = new fltk3::Input(80, 10, 210, 25, "Find:");
  replace_find->align(fltk3::ALIGN_LEFT);
  
  replace_with = new fltk3::Input(80, 40, 210, 25, "Replace:");
  replace_with->align(fltk3::ALIGN_LEFT);
  
  replace_all = new fltk3::Button(10, 70, 90, 25, "Replace All");
  replace_all->callback((fltk3::Callback *)replall_cb, this);
  
  replace_next = new fltk3::ReturnButton(105, 70, 120, 25, "Replace Next");
  replace_next->callback((fltk3::Callback *)replace2_cb, this);
  
  replace_cancel = new fltk3::Button(230, 70, 60, 25, "Cancel");
  replace_cancel->callback((fltk3::Callback *)replcan_cb, this);
  replace_dlg->end();
  replace_dlg->set_non_modal();
  editor = 0;
  *search = (char)0;
}

EditorWindow::~EditorWindow() {
  delete replace_dlg;
}

int check_save(void) {
  if (!changed) return 1;
  
  int r = fltk3::choice("The current file has not been saved.\n"
                        "Would you like to save it now?",
                        "Cancel", "Save", "Don't Save");
  
  if (r == 1) {
    save_cb(); // Save the file...
    return !changed;
  }
  
  return (r == 2) ? 1 : 0;
}

int loading = 0;
void load_file(const char *newfile, int ipos) {
  loading = 1;
  int insert = (ipos != -1);
  changed = insert;
  if (!insert) strcpy(filename, "");
  int r;
  if (!insert) r = textbuf->loadfile(newfile);
  else r = textbuf->insertfile(newfile, ipos);
  changed = changed || textbuf->input_file_was_transcoded;
  if (r)
    fltk3::alert("Error reading from file \'%s\':\n%s.", newfile, strerror(errno));
  else
    if (!insert) strcpy(filename, newfile);
  loading = 0;
  textbuf->call_modify_callbacks();
}

void save_file(const char *newfile) {
  if (textbuf->savefile(newfile))
    fltk3::alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
  else
    strcpy(filename, newfile);
  changed = 0;
  textbuf->call_modify_callbacks();
}

void copy_cb(fltk3::Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  fltk3::TextEditor::kf_copy((unsigned)0, e->editor);
}

void cut_cb(fltk3::Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  fltk3::TextEditor::kf_cut(0, e->editor);
}

void delete_cb(fltk3::Widget*, void*) {
  textbuf->remove_selection();
}

void find_cb(fltk3::Widget* w, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  const char *val;
  
  val = fltk3::input("Search String:", e->search);
  if (val != NULL) {
    // User entered a string - go find it!
    strcpy(e->search, val);
    find2_cb(w, v);
  }
}

void find2_cb(fltk3::Widget* w, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  if (e->search[0] == '\0') {
    // Search string is blank; get a new one...
    find_cb(w, v);
    return;
  }
  
  int pos = e->editor->insert_position();
  int found = textbuf->search_forward(pos, e->search, &pos);
  if (found) {
    // Found a match; select and update the position...
    textbuf->select(pos, pos+strlen(e->search));
    e->editor->insert_position(pos+strlen(e->search));
    e->editor->show_insert_position();
  }
  else fltk3::alert("No occurrences of \'%s\' found!", e->search);
}

void set_title(fltk3::Window* w) {
  if (filename[0] == '\0') strcpy(title, "Untitled");
  else {
    char *slash;
    slash = strrchr(filename, '/');
#ifdef WIN32
    if (slash == NULL) slash = strrchr(filename, '\\');
#endif
    if (slash != NULL) strcpy(title, slash + 1);
    else strcpy(title, filename);
  }
  
  if (changed) strcat(title, " (modified)");
  
  w->label(title);
}

void changed_cb(int, int nInserted, int nDeleted,int, const char*, void* v) {
  if ((nInserted || nDeleted) && !loading) changed = 1;
  EditorWindow *w = (EditorWindow *)v;
  set_title(w);
  if (loading) w->editor->show_insert_position();
}

void new_cb(fltk3::Widget*, void*) {
  if (!check_save()) return;
  
  filename[0] = '\0';
  textbuf->select(0, textbuf->length());
  textbuf->remove_selection();
  changed = 0;
  textbuf->call_modify_callbacks();
}

void open_cb(fltk3::Widget*, void*) {
  if (!check_save()) return;
  fltk3::NativeFileChooser fnfc;
  fnfc.title("Open file");
  fnfc.type(fltk3::NativeFileChooser::BROWSE_FILE);
  if ( fnfc.show() ) return;
  load_file(fnfc.filename(), -1);
  
}

void insert_cb(fltk3::Widget*, void *v) {
  fltk3::NativeFileChooser fnfc;
  fnfc.title("Insert file");
  fnfc.type(fltk3::NativeFileChooser::BROWSE_FILE);
  if ( fnfc.show() ) return;
  EditorWindow *w = (EditorWindow *)v;
  load_file(fnfc.filename(), w->editor->insert_position());
}

void paste_cb(fltk3::Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  fltk3::TextEditor::kf_paste(0, e->editor);
}

int num_windows = 0;

void close_cb(fltk3::Widget*, void* v) {
  EditorWindow* w = (EditorWindow*)v;
  
  if (num_windows == 1) {
    if (!check_save())
      return;
  }
  
  w->hide();
  w->editor->buffer(0);
  textbuf->remove_modify_callback(style_update, w->editor);
  textbuf->remove_modify_callback(changed_cb, w);
  fltk3::delete_widget(w);
  
  num_windows--;
  if (!num_windows) exit(0);
}

void quit_cb(fltk3::Widget*, void*) {
  if (changed && !check_save())
    return;
  
  exit(0);
}

void replace_cb(fltk3::Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  e->replace_dlg->show();
}

void replace2_cb(fltk3::Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  const char *find = e->replace_find->value();
  const char *replace = e->replace_with->value();
  
  if (find[0] == '\0') {
    // Search string is blank; get a new one...
    e->replace_dlg->show();
    return;
  }
  
  e->replace_dlg->hide();
  
  int pos = e->editor->insert_position();
  int found = textbuf->search_forward(pos, find, &pos);
  
  if (found) {
    // Found a match; update the position and replace text...
    textbuf->select(pos, pos+strlen(find));
    textbuf->remove_selection();
    textbuf->insert(pos, replace);
    textbuf->select(pos, pos+strlen(replace));
    e->editor->insert_position(pos+strlen(replace));
    e->editor->show_insert_position();
  }
  else fltk3::alert("No occurrences of \'%s\' found!", find);
}

void replall_cb(fltk3::Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  const char *find = e->replace_find->value();
  const char *replace = e->replace_with->value();
  
  find = e->replace_find->value();
  if (find[0] == '\0') {
    // Search string is blank; get a new one...
    e->replace_dlg->show();
    return;
  }
  
  e->replace_dlg->hide();
  
  e->editor->insert_position(0);
  int times = 0;
  
  // Loop through the whole string
  for (int found = 1; found;) {
    int pos = e->editor->insert_position();
    found = textbuf->search_forward(pos, find, &pos);
    
    if (found) {
      // Found a match; update the position and replace text...
      textbuf->select(pos, pos+strlen(find));
      textbuf->remove_selection();
      textbuf->insert(pos, replace);
      e->editor->insert_position(pos+strlen(replace));
      e->editor->show_insert_position();
      times++;
    }
  }
  
  if (times) fltk3::message("Replaced %d occurrences.", times);
  else fltk3::alert("No occurrences of \'%s\' found!", find);
}

void replcan_cb(fltk3::Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  e->replace_dlg->hide();
}

void save_cb() {
  if (filename[0] == '\0') {
    // No filename - get one!
    saveas_cb();
    return;
  }
  else save_file(filename);
}

void saveas_cb() {
  fltk3::NativeFileChooser fnfc;
  fnfc.title("Save File As?");
  fnfc.type(fltk3::NativeFileChooser::BROWSE_SAVE_FILE);
  if ( fnfc.show() ) return;
  save_file(fnfc.filename());
}

fltk3::Window* new_view();

void view_cb(fltk3::Widget*, void*) {
  fltk3::Window* w = new_view();
  w->show();
}

fltk3::MenuItem menuitems[] = {
  { "&File",              0, 0, 0, fltk3::SUBMENU },
  { "&New File",        0, (fltk3::Callback *)new_cb },
  { "&Open File...",    fltk3::COMMAND + 'o', (fltk3::Callback *)open_cb },
  { "&Insert File...",  fltk3::COMMAND + 'i', (fltk3::Callback *)insert_cb, 0, fltk3::MENU_DIVIDER },
  { "&Save File",       fltk3::COMMAND + 's', (fltk3::Callback *)save_cb },
  { "Save File &As...", fltk3::COMMAND + fltk3::SHIFT + 's', (fltk3::Callback *)saveas_cb, 0, fltk3::MENU_DIVIDER },
  { "New &View",        fltk3::ALT
#ifdef __APPLE__
    + fltk3::COMMAND
#endif
    + 'v', (fltk3::Callback *)view_cb, 0 },
  { "&Close View",      fltk3::COMMAND + 'w', (fltk3::Callback *)close_cb, 0, fltk3::MENU_DIVIDER },
  { "E&xit",            fltk3::COMMAND + 'q', (fltk3::Callback *)quit_cb, 0 },
  { 0 },
  
  { "&Edit", 0, 0, 0, fltk3::SUBMENU },
  { "Cu&t",             fltk3::COMMAND + 'x', (fltk3::Callback *)cut_cb },
  { "&Copy",            fltk3::COMMAND + 'c', (fltk3::Callback *)copy_cb },
  { "&Paste",           fltk3::COMMAND + 'v', (fltk3::Callback *)paste_cb },
  { "&Delete",          0, (fltk3::Callback *)delete_cb },
  { 0 },
  
  { "&Search", 0, 0, 0, fltk3::SUBMENU },
  { "&Find...",         fltk3::COMMAND + 'f', (fltk3::Callback *)find_cb },
  { "F&ind Again",      fltk3::COMMAND + 'g', find2_cb },
  { "&Replace...",      fltk3::COMMAND + 'r', replace_cb },
  { "Re&place Again",   fltk3::COMMAND + 't', replace2_cb },
  { 0 },
  
  { 0 }
};

fltk3::Window* new_view() {
  EditorWindow* w = new EditorWindow(660, 400, title);
  w->begin();
  fltk3::MenuBar* m = new fltk3::MenuBar(0, 0, 660, 30);
  m->copy(menuitems, w);
  w->editor = new fltk3::TextEditor(0, 30, 660, 370);
  w->editor->textfont(fltk3::COURIER);
  w->editor->textsize(TS);
  //w->editor->wrap_mode(fltk3::Text_Editor::WRAP_AT_BOUNDS, 250);
  w->editor->buffer(textbuf);
  w->editor->highlight_data(stylebuf, styletable,
                            sizeof(styletable) / sizeof(styletable[0]),
                            'A', style_unfinished_cb, 0);
  textbuf->text();
  style_init();
  w->end();
  w->resizable(w->editor);
  w->callback((fltk3::Callback *)close_cb, w);
  
  textbuf->add_modify_callback(style_update, w->editor);
  textbuf->add_modify_callback(changed_cb, w);
  textbuf->call_modify_callbacks();
  num_windows++;
  return w;
}

int main(int argc, char **argv) {
  textbuf = new fltk3::TextBuffer;
  //textbuf->transcoding_warning_action = NULL;
  style_init();
  
  fltk3::Window* window = new_view();
  
  window->show(1, argv);
  
  if (argc > 1) load_file(argv[1], -1);
  
  return fltk3::run();
}

//
// End of "$Id$".
//
