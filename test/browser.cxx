// Maarten de Boer's toggle tree demo program rewritten to use the
// fltk 2.0 browser.  This unfortunately required a bunch of changes.

#include <fltk/Browser.h>
#include <fltk/Window.h>
#include <fltk/Button.h>
#include <fltk/CheckButton.h>
#include <fltk/RadioButton.h>
#include <fltk/Input.h>
#include <fltk/ValueSlider.h>
#include <fltk/run.h>
#include <fltk/events.h>
#include <fltk/xpmImage.h>
#include <fltk/Item.h>
#include <fltk/ItemGroup.h>

#include "folder_small.xpm"
#include "file_small.xpm"

#include <stdio.h>
#include <stdlib.h>

fltk::xpmImage* folderSmall;
fltk::xpmImage* fileSmall;

void
cb_test(fltk::Widget* browser, void*) {
  fltk::Widget* w = ((fltk::Browser*)browser)->item();
  printf("Callback, browser->item() = '%s'",
	 w && w->label() ? w->label() : "null");
  if (fltk::event_clicks()) printf(", Double Click");
  printf("\n");
}

void
cb_remove(fltk::Widget*, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*) ptr;
  if (tree->type() & fltk::Browser::MULTI) {
    fltk::Widget* w = tree->goto_top();
    while (w) {
      if (w->value()) {
	fltk::Group* g = w->parent();
	g->remove(w);
	delete w;
	g->relayout();
	w = tree->goto_top();
      } else {
	w = tree->next();
      }
    }
  } else {
    fltk::Widget* w = tree->goto_focus();
    if (w) {
      fltk::Group* g = w->parent();
      g->remove(w);
      delete w;
      g->relayout();
    }
  }
}

void
cb_multi(fltk::Widget* w, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*) ptr;
  tree->type(w->value() ? fltk::Browser::MULTI : fltk::Browser::NORMAL);
  tree->relayout();
}

static fltk::Group* current_group(fltk::Browser* tree) {
  fltk::Widget* w = tree->goto_focus();
  if (!w) return tree;
  if (w->is_group() && w->flags()&fltk::VALUE) return (fltk::Group*)w;
  return w->parent() ? w->parent() : tree;
}

static fltk::Group* add_folder(fltk::Group* parent,
		       const char* name, int open, fltk::Image* image) {
  parent->begin();
  fltk::ItemGroup* o = new fltk::ItemGroup(name);
  o->image(image);
  if (open) o->set_flag(fltk::VALUE);
  return o;
}

static fltk::Widget* add_paper(fltk::Group* parent,
			   const char* name, int, fltk::Image* image) {
  parent->begin();
  fltk::Item* o = new fltk::Item(name);
  o->align(fltk::ALIGN_LEFT|fltk::ALIGN_INSIDE|fltk::ALIGN_CLIP);
  o->image(image);
  return o;
}

void
cb_add_folder(fltk::Widget*, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*) ptr;
  add_folder(current_group(tree), "Added folder", 1, folderSmall);
  tree->relayout();
}

void
cb_add_paper(fltk::Widget*, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*) ptr;
  add_paper(current_group(tree), "New paper\t2.col\t3.col", 0, fileSmall);
  tree->relayout();
}

void cb_when_changed(fltk::Widget* b, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|fltk::WHEN_CHANGED);
  else
    tree->when(tree->when()&~fltk::WHEN_CHANGED);
}

void cb_when_release(fltk::Widget* b, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|fltk::WHEN_RELEASE);
  else
    tree->when(tree->when()&~fltk::WHEN_RELEASE);
}

void cb_when_not_changed(fltk::Widget* b, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|fltk::WHEN_NOT_CHANGED);
  else
    tree->when(tree->when()&~fltk::WHEN_NOT_CHANGED);
}

void cb_when_enter_key(fltk::Widget* b, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|fltk::WHEN_ENTER_KEY);
  else
    tree->when(tree->when()&~fltk::WHEN_ENTER_KEY);
}

void button_cb(fltk::Widget* b, void *) {
  printf("Button %s pushed\n", b->label());
}

const char *labels[] = {"Column 1", "Column 2", "Column 3", 0};
int widths[]   = {100, 70, 50, 0};

fltk::Browser *browser;
void change_resize(fltk::Widget *w, long arg) {
  if (w->value()) 
    widths[1] = -1;
  else
    widths[1] = 70; 
  browser->column_widths(widths);
}

#define USE_STRING_LIST 0
#if USE_STRING_LIST
#include <fltk/fltk::String_List.h>

const char* const strings[] = {
  "foo", "bar", "number 2", "number 3", "another item", "blah", "zoo"
};
#endif

int main(int argc,char** argv) {

  fltk::Window win(240, 330, "Browser Example");
  win.begin();

  fltk::Browser tree(10, 10, 220, 180);
  tree.indented(1);
  tree.callback(cb_test);

  browser = &tree;
  tree.column_widths(widths);
  tree.column_labels(labels);

  fltk::Button remove_button(5, 200, 70, 22, "Remove");
  remove_button.callback((fltk::Callback*)cb_remove, (void *)&tree);

  fltk::Button add_paper_button(5, 224, 70, 22, "Add Paper");
  add_paper_button.callback((fltk::Callback*)cb_add_paper, (void *)&tree);

  fltk::Button add_folder_button(5, 248, 70, 22, "Add Folder");
  add_folder_button.callback((fltk::Callback*)cb_add_folder, (void *)&tree);

  fltk::CheckButton multi_button(80, 200, 160, 20, "fltk::Browser::MULTI");
  multi_button.callback((fltk::Callback*)cb_multi, (void *)&tree);

  fltk::CheckButton when_changed_button(80, 220, 160, 20, "fltk::WHEN_CHANGED");
  when_changed_button.callback(cb_when_changed, (void *)&tree);

  fltk::CheckButton when_not_changed_button(80, 240, 160, 20, "fltk::WHEN_NOT_CHANGED");
  when_not_changed_button.callback(cb_when_not_changed, (void *)&tree);

  fltk::CheckButton when_release_button(80, 260, 160, 20, "fltk::WHEN_RELEASE");
  when_release_button.callback(cb_when_release, (void *)&tree);
  when_release_button.set_value();

  fltk::CheckButton when_enter_key_button(80, 280, 160, 20, "fltk::WHEN_ENTER_KEY");
  when_enter_key_button.callback(cb_when_enter_key, (void *)&tree);

  fltk::CheckButton resize(80, 310, 160, 20, "Make 2. column flexible");
  resize.callback(change_resize, 3);

  win.resizable(tree);
  win.end();

  folderSmall = new fltk::xpmImage(folder_small);
  fileSmall = new fltk::xpmImage(file_small);

#if USE_STRING_LIST
  //tree.list(new fltk::String_List("alpha\0beta\0ceta\0delta\0red\0green\0blue\0"));
  tree.list(new fltk::String_List(strings, sizeof(strings)/sizeof(*strings)));
  //tree.list(new fltk::String_List(strings));
#else

  // Add some nodes with icons -- some open, some closed.
  // fltk::ToggleNode::fltk::ToggleNode( LABEL , CAN_OPEN (default=1) , ICON )
  fltk::Group* g;

  g = add_folder(&tree, "aaa\t2.col\t3.col", 1, folderSmall);

  add_folder(g, "bbb TWO\t2.col\t3.col", 1, folderSmall);

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

  // add some widgets:
  g->begin();
  fltk::Button * b = new fltk::Button(0,0,100,23,"button");
  b->callback(button_cb);
  b = new fltk::CheckButton(0,0,100,23,"CheckButton");
  printf("b->type = %d, group = %d, is_group = %d\n",
	 b->type(), fltk::Widget::GROUP_TYPE, b->is_group());
  b->callback(button_cb);
  new fltk::Input(0,0,200,23,"Input:");
  new fltk::ValueSlider(0,0,200,23,"Input:");
  new fltk::ValueSlider(0,0,200,23,"Input:");
  new fltk::ValueSlider(0,0,200,23,"Input:");
  tree.end();

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

  //fltk::visual(fltk::RGB);
  win.show(argc,argv);

  fltk::run();
  return 0;
}
