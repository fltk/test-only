// Maarten de Boer's toggle tree demo program rewritten to use the
// fltk 2.0 browser.  This unfortunately required a bunch of changes.

#include <fltk/Fl_Browser.h>
#include <fltk/Fl_Window.h>
#include <fltk/Fl_Button.h>
#include <fltk/Fl_Scroll.h>
#include <fltk/Fl.h>
#include <fltk/Fl_Pixmap.h>
#include <fltk/Fl_Item.h>
#include <fltk/Fl_Item_Group.h>
#include <fltk/Fl_Check_Button.h>

#include "folder_small.xpm"
#include "file_small.xpm"

#include <stdio.h>
#include <stdlib.h>

Fl_Pixmap* folderSmall;
Fl_Pixmap* fileSmall;

void
cb_test(Fl_Widget* browser, void*) {
  Fl_Widget* w = ((Fl_Browser*)browser)->item();
  printf("Callback, browser->item() = '%s'",
	 w && w->label() ? w->label() : "null");
  if (Fl::event_clicks()) printf(", Double Click");
  printf("\n");
}

void
cb_remove(Fl_Widget*, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*) ptr;
  if (tree->type() & Fl_Browser::MULTI) {
    Fl_Widget* w = tree->goto_top();
    while (w) {
      if (w->selected()) {
	Fl_Group* g = w->parent();
	g->remove(w);
	delete w;
	g->relayout();
	w = tree->goto_top();
      } else {
	w = tree->next();
      }
    }
  } else {
    Fl_Widget* w = tree->goto_focus();
    if (w) {
      Fl_Group* g = w->parent();
      g->remove(w);
      delete w;
      g->relayout();
    }
  }
}

void
cb_multi(Fl_Widget* w, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*) ptr;
  tree->type(w->value() ? Fl_Browser::MULTI : Fl_Browser::NORMAL);
  tree->relayout();
}

static Fl_Group* current_group(Fl_Browser* tree) {
  Fl_Widget* w = tree->goto_focus();
  if (!w) return tree;
  if (w->is_group() && w->flags()&FL_VALUE) return (Fl_Group*)w;
  return w->parent() ? w->parent() : tree;
}

static Fl_Group* add_folder(Fl_Group* parent,
		       const char* name, int open, Fl_Image* image) {
  parent->begin();
  Fl_Item_Group* o = new Fl_Item_Group(name);
  o->image(image);
  if (open) o->set_flag(FL_VALUE);
  return o;
}

static Fl_Widget* add_paper(Fl_Group* parent,
			   const char* name, int, Fl_Image* image) {
  parent->begin();
  Fl_Item* o = new Fl_Item(name);
  o->image(image);
  return o;
}

void
cb_add_folder(Fl_Widget*, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*) ptr;
  add_folder(current_group(tree), "Added folder", 1, folderSmall);
  tree->relayout();
}

void
cb_add_paper(Fl_Widget*, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*) ptr;
  add_paper(current_group(tree), "New paper", 0, fileSmall);
  tree->relayout();
}

void cb_when_changed(Fl_Widget* b, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|FL_WHEN_CHANGED);
  else
    tree->when(tree->when()&~FL_WHEN_CHANGED);
}

void cb_when_release(Fl_Widget* b, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|FL_WHEN_RELEASE);
  else
    tree->when(tree->when()&~FL_WHEN_RELEASE);
}

void cb_when_not_changed(Fl_Widget* b, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|FL_WHEN_NOT_CHANGED);
  else
    tree->when(tree->when()&~FL_WHEN_NOT_CHANGED);
}

void cb_when_enter_key(Fl_Widget* b, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|FL_WHEN_ENTER_KEY);
  else
    tree->when(tree->when()&~FL_WHEN_ENTER_KEY);
}

#define USE_STRING_LIST 0
#if USE_STRING_LIST
#include <fltk/Fl_String_List.h>

const char* const strings[] = {
  "foo", "bar", "number 2", "number 3", "another item", "blah", "zoo"
};
#endif

int main(int argc,char** argv) {

  Fl_Window win(240, 304, "Browser Example");

  Fl_Browser tree(10, 10, 220, 180);
  tree.indented(1);
  tree.callback(cb_test);

  Fl_Button remove_button(5, 200, 70, 22, "Remove");
  remove_button.callback((Fl_Callback*)cb_remove, (void *)&tree);

  Fl_Button add_paper_button(5, 224, 70, 22, "Add Paper");
  add_paper_button.callback((Fl_Callback*)cb_add_paper, (void *)&tree);

  Fl_Button add_folder_button(5, 248, 70, 22, "Add Folder");
  add_folder_button.callback((Fl_Callback*)cb_add_folder, (void *)&tree);

  Fl_Check_Button multi_button(80, 200, 160, 20, "Fl_Browser::MULTI");
  multi_button.callback((Fl_Callback*)cb_multi, (void *)&tree);

  Fl_Check_Button when_changed_button(80, 220, 160, 20, "FL_WHEN_CHANGED");
  when_changed_button.callback(cb_when_changed, (void *)&tree);

  Fl_Check_Button when_not_changed_button(80, 240, 160, 20, "FL_WHEN_NOT_CHANGED");
  when_not_changed_button.callback(cb_when_not_changed, (void *)&tree);

  Fl_Check_Button when_release_button(80, 260, 160, 20, "FL_WHEN_RELEASE");
  when_release_button.callback(cb_when_release, (void *)&tree);
  when_release_button.set_value();

  Fl_Check_Button when_enter_key_button(80, 280, 160, 20, "FL_WHEN_ENTER_KEY");
  when_enter_key_button.callback(cb_when_enter_key, (void *)&tree);

  win.resizable(tree);
  win.end();

  folderSmall = new Fl_Pixmap(folder_small);
  fileSmall = new Fl_Pixmap(file_small);

#if USE_STRING_LIST
  //tree.list(new Fl_String_List("alpha\0beta\0ceta\0delta\0red\0green\0blue\0"));
  tree.list(new Fl_String_List(strings, sizeof(strings)/sizeof(*strings)));
  //tree.list(new Fl_String_List(strings));
#else
  //int w[3] = {150, 200, 0};
  //tree.column_widths(w);

  // Add some nodes with icons -- some open, some closed.
  // Fl_ToggleNode::Fl_ToggleNode( LABEL , CAN_OPEN (default=1) , ICON )
  Fl_Group* g;

  g = add_folder(&tree, "aaa\t123", 1, folderSmall);

  add_folder(g, "bbb TWO\t456", 1, folderSmall);

  g = add_folder(&tree, "bbb", 0, folderSmall);

  add_paper(g, "ccc\t789", 1, folderSmall);
  add_paper(g, "ddd\t012", 0, fileSmall);

  g = add_folder(&tree, "eee", 1, folderSmall);
  add_paper(g, "fff", 0, fileSmall);

  g = add_folder(g, "ggg", 1, folderSmall);
  add_paper(g, "hhh", 1, fileSmall);
  add_paper(g, "iii", 1, fileSmall);

  g = add_folder(&tree, "jjj", 1, folderSmall);
  add_paper(g, "kkk", 0, fileSmall);

  add_paper(&tree, "lll", 0, fileSmall);
  g = add_folder(&tree, "mmm", 0, folderSmall);
  add_paper(g, "nnn", 1, folderSmall);
  add_paper(g, "ooo", 0, fileSmall);

  g = add_folder(g, "ppp", 1, folderSmall);
  add_paper(g, "qqq", 0, fileSmall);
  g = g->parent();

  g = add_folder(g, "rrr", 1, folderSmall);
  g = add_folder(g, "sss", 1, folderSmall);
  g = add_folder(g, "ttt", 1, folderSmall);

  g = add_folder(&tree, "uuu", 1, folderSmall);
  add_paper(g, "vvv", 0, fileSmall);

  add_paper(&tree, "www", 0, fileSmall);
  add_paper(&tree, "xxx", 0, fileSmall);
  add_paper(&tree, "yyy", 0, fileSmall);
  add_paper(&tree, "zzz", 0, fileSmall);

#if 0
  // Examples of removing items (successfully, and unsuccessfully)
  // by label name:
  if (&tree.remove("xxx"))
    printf("Successfully deleted \"xxx\"\n");
  else
    printf("Could not delete \"xxx\"\n");

  if (&tree.remove("nonexistant"))
    printf("Successfully deleted \"nonexistant\"\n");
  else
    printf("Could not delete \"nonexistant\"\n");
#endif
#endif

  //Fl::visual(FL_RGB);
  win.show(argc,argv);

  Fl::run();
  return 0;
}
