// Maarten de Boer's toggle tree demo program rewritten to use the
// fltk 2.0 browser.  This unfortunately required a bunch of changes.

#include <FL/Fl_Browser.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Item.H>
#include <FL/Fl_Item_Group.H>
#include <FL/Fl_Check_Button.H>

#include "folder_small.xpm"
#include "file_small.xpm"

#include <stdio.h>
#include <stdlib.h>

Fl_Pixmap* folderSmall;
Fl_Pixmap* fileSmall;

void
cb_test(Fl_Widget* t, void*) {
  Fl_Widget* w = ((Fl_Browser*)t)->item();
  printf("callback for '%s'\n", w && w->label() ? w->label() : "null");
}

void
cb_remove(Fl_Widget*, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*) ptr;
  if (tree->multi()) {
    Fl_Widget* w = tree->goto_top();
    while (w) {
      if (w->value()) {
	Fl_Group* g = w->parent();
	g->remove(w);
	delete w;
	g->relayout();
	w = tree->goto_top();
      } else {
	w = tree->forward();
      }
    }
  } else {
    Fl_Widget* w = tree->item();
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
  tree->type(w->value() ? FL_MULTI_BROWSER : FL_NORMAL_BROWSER);
  tree->relayout();
}

void
cb_colors(Fl_Widget* w, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*) ptr;
  tree->color(w->value() ? Fl_Color(FL_LIGHT2) : tree->window_color());
  tree->redraw();
}

static Fl_Group* current_group(Fl_Browser* tree) {
  Fl_Widget* w = tree->goto_mark(Fl_Browser::FOCUS);
  if (!w) return tree;
  if (w->is_group() && w->flags()&FL_OPEN) return (Fl_Group*)w;
  return w->parent();
}

static Fl_Group* add_folder(Fl_Group* parent,
		       const char* name, int open, Fl_Image* image) {
  Fl_Item_Group* o = new Fl_Item_Group(name);
  o->image(image);
  if (open) o->set_flag(FL_OPEN);
  parent->add(o);
  parent->relayout();
  return o;
}

static Fl_Widget* add_paper(Fl_Group* parent,
			   const char* name, int, Fl_Image* image) {
  Fl_Item* o = new Fl_Item(name);
  o->image(image);
  parent->add(o);
  parent->relayout();
  return o;
}

void
cb_add_folder(Fl_Widget*, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*) ptr;
  add_folder(current_group(tree), "Added folder", 1, folderSmall);
}

void
cb_add_paper(Fl_Widget*, void* ptr) {
  Fl_Browser* tree = (Fl_Browser*) ptr;
  add_paper(current_group(tree), "New paper", 0, fileSmall);
}

void cb_sort(Fl_Widget*, void* ) {
}

void cb_sort_reverse(Fl_Widget*, void*) {
}

void cb_sort_random(Fl_Widget*, void*) {
}

#include <FL/Fl_String_List.H>

int main(int argc,char** argv) {
  Fl_Window win(240, 304, "Browser Example");
  Fl_Button remove_button(10, 200, 100, 22, "Remove");
  Fl_Button add_paper_button(10, 224, 100, 22, "Add Paper");
  Fl_Button add_folder_button(10, 248, 100, 22, "Add Folder");
  Fl_Button sort_button(10, 272, 100, 22, "Sort");
  Fl_Check_Button multi_button(130, 200, 100, 22, "MultiBrowser");
  Fl_Button sort_button_rev(130, 224, 100, 22, "Reverse Sort");
  Fl_Button sort_button_rnd(130, 248, 100, 22, "Randomize");
  Fl_Check_Button colors_button(130, 272, 100, 22, "Colors");
  Fl_Browser tree(10, 10, 220, 180);
  tree.indented(1);
  win.resizable(tree);
  win.end();

  // Add callbacks to the widgets

  remove_button.callback((Fl_Callback*)cb_remove, (void *)&tree);
  add_paper_button.callback((Fl_Callback*)cb_add_paper, (void *)&tree);
  add_folder_button.callback((Fl_Callback*)cb_add_folder, (void *)&tree);
  sort_button.callback((Fl_Callback*)cb_sort, (void *)&tree);
  sort_button_rnd.callback((Fl_Callback*)cb_sort_random, (void *)&tree);
  sort_button_rev.callback((Fl_Callback*)cb_sort_reverse, (void *)&tree);
  multi_button.callback((Fl_Callback*)cb_multi, (void *)&tree);
  colors_button.callback((Fl_Callback*)cb_colors, (void *)&tree);
  colors_button.set();
  tree.callback(cb_test);

#if 0
  tree.list(new Fl_String_List("alpha\0beta\0ceta\0delta\0red\0green\0blue\0"));
#else
  //int w[3] = {150, 200, 0};
  //tree.column_widths(w);

  folderSmall = new Fl_Pixmap(folder_small);
  fileSmall = new Fl_Pixmap(file_small);

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

  win.show(argc,argv);

  Fl::run();
}


