// editor.cxx (example6)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fltk/run.h>
#include <fltk/Group.h>
#include <fltk/Window.h>
#include <fltk/ask.h>
#include <fltk/file_chooser.h>
#include <fltk/MenuBar.h>
#include <fltk/Input.h>
#include <fltk/Button.h>
#include <fltk/ReturnButton.h>
#include <fltk/TextBuffer.h>
#include <fltk/TextEditor.h>
#include <fltk/Input.h>
#include <fltk/events.h> // for shift flags

using namespace fltk;

int                changed = 0;
char               filename[256] = "";
char               title[256];
TextBuffer	   *textbuf;

void save_cb();
void saveas_cb();
void find2_cb(Widget*, void*);
void replall_cb(Widget*, void*);
void replace2_cb(Widget*, void*);
void replcan_cb(Widget*, void*);

class EditorWindow : public Window {
  public:
    EditorWindow(int w, int h, TextBuffer*, const char* t);
    ~EditorWindow();

    TextEditor     *editor;
};

int check_save(void) {
  if (!changed) return 1;

  int r = choice("The current file has not been saved.\n"
                    "Would you like to save it now?",
                    "Cancel", "No", "*Yes");

  if (r == 2) {
    save_cb(); // Save the file...
    return !changed;
  };

   return (r == 1) ? 1 : 0;
}

int loading = 0;
void load_file(char *newfile, int ipos) {
  loading = 1;
  int insert = (ipos != -1);
  changed = insert;
  if (!insert) strcpy(filename, "");
  int r;
  if (!insert) r = textbuf->loadfile(newfile);
  else r = textbuf->insertfile(newfile, ipos);
  if (r)
    alert("Error reading from file \'%s\':\n%s.", newfile, strerror(errno));
  else
    if (!insert) strcpy(filename, newfile);
  loading = 0;
  textbuf->call_modify_callbacks();
}

void save_file(char *newfile) {
  if (textbuf->savefile(newfile))
    alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
  else
    strcpy(filename, newfile);
  changed = 0;
  textbuf->call_modify_callbacks();
}

void copy_cb(Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  TextEditor::kf_copy(0, e->editor);
}

void cut_cb(Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  TextEditor::kf_cut(0, e->editor);
}

void delete_cb(Widget*, void*) {
  textbuf->remove_selection();
}

char               search[256];

void find_cb(Widget* w, void* v) {
  const char *val;
  val = input("Search String:", search);
  if (val != NULL) {
    // User entered a string - go find it!
    strcpy(search, val);
    find2_cb(w, v);
  }
}

void find2_cb(Widget* w, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  if (search[0] == '\0') {
    // Search string is blank; get a new one...
    find_cb(w, v);
    return;
  }

  int pos = e->editor->insert_position();
  int found = textbuf->search_forward(pos, search, &pos);
  if (found) {
    // Found a match; select and update the position...
    textbuf->select(pos, pos+strlen(search));
    e->editor->insert_position(pos+strlen(search));
    e->editor->show_insert_position();
  }
  else alert("No occurrences of \'%s\' found!", search);
}

void set_title(Window* w) {
  if (filename[0] == '\0') strcpy(title, "Untitled");
  else {
    char *slash;
    slash = strrchr(filename, '/');
#ifdef _WIN32
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

void new_cb(Widget*, void*) {
  if (!check_save()) return;

  filename[0] = '\0';
  textbuf->select(0, textbuf->length());
  textbuf->remove_selection();
  changed = 0;
  textbuf->call_modify_callbacks();
}

void open_cb(Widget*, void*) {
  if (!check_save()) return;

  char *newfile = file_chooser("Open File?", "*", filename);
  if (newfile != NULL) load_file(newfile, -1);
}

void insert_cb(Widget*, void *v) {
  char *newfile = file_chooser("Insert File?", "*", filename);
  EditorWindow *w = (EditorWindow *)v;
  if (newfile != NULL) load_file(newfile, w->editor->insert_position());
}

void paste_cb(Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  TextEditor::kf_paste(0, e->editor);
}

int num_windows = 0;

void close_cb(Widget*, void* v) {
  Window* w = (Window*)v;
  if (num_windows <= 1) {
    if (!check_save()) return;
    exit(0);
  }
  --num_windows;
  delete w;
}

void quit_cb(Widget*, void*) {
  if (!check_save()) return;
  exit(0);
}

// Dialog box for search/replace:

Input           *replace_find;
Input           *replace_with;
Button          *replace_all;
ReturnButton    *replace_next;
Button          *replace_cancel;

Window* replace_dlg(EditorWindow* e) {
  static Window *replace_dlg = 0;
  if (!replace_dlg) {
    replace_dlg = new Window(300, 105, "Replace");
    replace_dlg->begin();
    replace_find = new Input(70, 10, 210, 25, "Find:");
    replace_find->clear_flag(ALIGN_MASK);
    replace_find->set_flag(ALIGN_LEFT);

    replace_with = new Input(70, 40, 210, 25, "Replace:");
    replace_with->clear_flag(ALIGN_MASK);
    replace_with->set_flag(ALIGN_LEFT);

    replace_all = new Button(10, 70, 90, 25, "Replace All");

    replace_next = new ReturnButton(105, 70, 120, 25, "Replace Next");

    replace_cancel = new Button(230, 70, 60, 25, "Cancel");
    replace_dlg->end();
  }
  replace_all->callback((Callback *)replall_cb, e);
  replace_next->callback((Callback *)replace2_cb, e);
  replace_cancel->callback((Callback *)replcan_cb, e);
  return replace_dlg;
}

void replace_cb(Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  replace_dlg(e)->show(e);
}

void replace2_cb(Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  const char *find = replace_find->value();
  const char *replace = replace_with->value();

  if (find[0] == '\0') {
    // Search string is blank; get a new one...
    replace_dlg(e)->show(e);
    return;
  }

  replace_dlg(e)->hide();

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
  else alert("No occurrences of \'%s\' found!", find);
}

void replall_cb(Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  const char *find = replace_find->value();
  const char *replace = replace_with->value();

  if (find[0] == '\0') {
    // Search string is blank; get a new one...
    replace_dlg(e)->show(e);
    return;
  }

  replace_dlg(e)->hide();

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

  if (times) message("Replaced %d occurrences.", times);
  else alert("No occurrences of \'%s\' found!", find);
}

void replcan_cb(Widget*, void* v) {
  EditorWindow* e = (EditorWindow*)v;
  replace_dlg(e)->hide();
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
  char *newfile;

  newfile = file_chooser("Save File As?", "*", filename);
  if (newfile != NULL) save_file(newfile);
}

void undo_cb(Widget*, void*) {
  alert("Undo not implemented!");
}

Window* new_view() {
  Window* w = new EditorWindow(512, 384+22, textbuf, title);
  w->callback((Callback *)close_cb, w);
  num_windows++;
  return w;
}

void view_cb(Widget*, void*) {
  new_view()->show();
}

EditorWindow::EditorWindow(int w, int h, TextBuffer* textbuffer, const char* t)
  : Window(w, h, t)
{
  *search = (char)0;
  begin();
   MenuBar* m = new MenuBar(0, 0, 512, 25);

    m->user_data(this); // make this be passed to all menu callbacks
    m->add("&File/&New File",		0,        (Callback*)new_cb);
    m->add("&File/&Open File...",	CTRL+'o', (Callback*)open_cb);
    m->add("&File/_&Insert File...",	CTRL+'i', (Callback*)insert_cb);
    m->add("&File/&Save File",		CTRL+'s', (Callback*)save_cb);
    m->add("&File/Save File &As...",	CTRL+SHIFT+'S', (Callback*)saveas_cb);
    m->add("&File/New &View",		ALT+'v',  (Callback*)view_cb);
    m->add("&File/&Close View",		CTRL+'w', (Callback*)close_cb);
    m->add("&File/E&xit",		CTRL+'q', (Callback*)quit_cb);

    m->add("&Edit/_&Undo",		CTRL+'z', (Callback*)undo_cb);
    m->add("&Edit/Cu&t",		CTRL+'x', (Callback*)cut_cb);
    m->add("&Edit/&Copy",		CTRL+'c', (Callback*)copy_cb);
    m->add("&Edit/&Paste",		CTRL+'v', (Callback*)paste_cb);
    m->add("&Edit/&Delete",		0,        (Callback*)delete_cb);

    m->add("&Search/&Find...",		CTRL+'f', (Callback*)find_cb);
    m->add("&Search/Find A&gain",	CTRL+'g', (Callback*)find2_cb);
    m->add("&Search/&Replace...",	CTRL+'r', (Callback*)replace_cb);
    m->add("&Search/Replace &Again",	CTRL+'t', (Callback*)replace2_cb);

   editor = new TextEditor(0, 25, 512, 354+5);
   editor->buffer(textbuf);
   textbuf->add_modify_callback(changed_cb, this);
   //textbuf->call_modify_callbacks();
   Input* i = new Input(0, 384, 512, 22);
   i->value("Click here to test focus navigation");
  end();
  resizable(editor);
}

EditorWindow::~EditorWindow() {
  editor->buffer()->remove_modify_callback(changed_cb, this);
}

int main(int argc, char **argv) {
  textbuf = new TextBuffer;
  if (argc > 1) load_file(argv[1], -1);

  Window* window = new_view();
  window->show(1, argv);
  return run();
}

//
// End of "$Id: editor.cxx,v 1.17 2004/03/05 08:17:01 spitzak Exp $".
//
