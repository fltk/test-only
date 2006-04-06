//
// "$Id$"
//
// browser.cxx
// Maarten de Boer's toggle tree demo program rewritten to use the
// fltk 2.0 browser.  This unfortunately required a bunch of changes.
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
#include "folder_small2.xpm"
#include "folder_small3.xpm"
#include "file_small.xpm"
#include "file_small2.xpm"
#include "book.xpm"
#include "porsche.xpm"

#include <stdio.h>
#include <stdlib.h>

fltk::xpmImage* folderSmall, *folderSmall2,*folderSmall3;
fltk::xpmImage* fileSmall, *fileSmall2,*bookImg;
fltk::xpmImage* customImage;

void cb_test(fltk::Widget* browser, void*) {
  fltk::Widget* w = ((fltk::Browser*)browser)->item();
  printf("Callback, browser->item() = '%s'",
	 w && w->label() ? w->label() : "null");
  if (fltk::event_clicks()) printf(", Double Click");
  printf("\n");
}

void cb_remove(fltk::Widget*, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*) ptr;
  if (tree->type() & fltk::Browser::MULTI) {
    fltk::Widget* w = tree->goto_top();
    while (w) {
      if (w->selected()) { // test for parent being open
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

void cb_multi(fltk::Button* w, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*) ptr;
  tree->type(w->value() ? fltk::Browser::MULTI : fltk::Browser::NORMAL);
  tree->relayout();
}

static fltk::Group* current_group(fltk::Browser* tree) {
  fltk::Widget* w = tree->goto_focus();
  if (!w) return tree;
  if (w->is_group() && w->flags()&fltk::OPENED) return (fltk::Group*)w;
  return w->parent() ? w->parent() : tree;
}

void cb_add_folder(fltk::Widget*, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*) ptr;
  tree->add_group("Added folder", current_group(tree));
  tree->relayout();
}

void cb_add_paper(fltk::Widget*, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*) ptr;
  tree->add_leaf("New paper\t@rt2.col\t3.col", current_group(tree));
  tree->relayout();
}

void cb_when_changed(fltk::Button* b, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|fltk::WHEN_CHANGED);
  else
    tree->when(tree->when()&~fltk::WHEN_CHANGED);
}

void cb_when_release(fltk::Button* b, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|fltk::WHEN_RELEASE);
  else
    tree->when(tree->when()&~fltk::WHEN_RELEASE);
}

void cb_when_not_changed(fltk::Button* b, void* ptr) {
  fltk::Browser* tree = (fltk::Browser*)ptr;
  if (b->value())
    tree->when(tree->when()|fltk::WHEN_NOT_CHANGED);
  else
    tree->when(tree->when()&~fltk::WHEN_NOT_CHANGED);
}

void cb_when_enter_key(fltk::Button* b, void* ptr) {
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
int widths[]   = {100, 70, 70, 0};

fltk::Browser *browser=0;
fltk::CheckButton *bm = 0;

// callback for changing dynamically the look of the tree browser 
void cb_change_look(fltk::Widget*, void* ptr) {
  static bool flip = true;
  fltk::Browser* tree = (fltk::Browser*) ptr;
  bm->set();
  bm->do_callback();

  tree->set_symbol(fltk::Browser::GROUP	   // tell if you want to setup Group nodes
      ,flip ? folderSmall : folderSmall2   // node default (closed) image 
      ,flip ? folderSmall3  : folderSmall  // belowmouse image (optional)
      ,flip ? folderSmall : folderSmall3   // group node open image (optional)
      );
  tree->set_symbol(fltk::Browser::LEAF, // tell you want to setup Leaf nodes
      flip ? fileSmall : fileSmall2,    // node default (closed) image 
      flip ? fileSmall2 : fileSmall);   // belowmouse image (optional)
  flip = !flip;
  tree->relayout();
}

// callback for deactivate/activate the belowmouse img change
void below_mouse_cb(fltk::Button *w, long arg) {
    static const fltk::Symbol *last1 = 0;
    static const fltk::Symbol *last2=0;

    if (w->value()) {
	browser->set_symbol(fltk::Browser::GROUP, browser->get_symbol(fltk::Browser::GROUP),last1,
	    browser->get_symbol(fltk::Browser::GROUP,fltk::OPENED));
	browser->set_symbol(fltk::Browser::LEAF, browser->get_symbol(fltk::Browser::LEAF),last2);
    } else {
	last1 = browser->get_symbol(fltk::Browser::GROUP, fltk::BELOWMOUSE);
	last2 = browser->get_symbol(fltk::Browser::LEAF, fltk::BELOWMOUSE);
	browser->set_symbol(fltk::Browser::GROUP, 
	    browser->get_symbol(fltk::Browser::GROUP), 0, 
	    browser->get_symbol(fltk::Browser::GROUP,fltk::OPENED));
	browser->set_symbol(fltk::Browser::LEAF, browser->get_symbol(fltk::Browser::LEAF),0);
    }
    browser->relayout();
}

void change_resize(fltk::Button *w, long arg) {
  if (w->value()) 
    widths[1] = -1;
  else
    widths[1] = 70; 
  browser->column_widths(widths);
}

int main(int argc,char** argv) {

  fltk::Window win(280, 330, "Browser Example");
  win.begin();

  fltk::Browser tree(10, 10, 260, 180);
  tree.indented(1);
  tree.callback(cb_test);

  browser = &tree;
  tree.column_widths(widths);
  tree.column_labels(labels);
  
  fltk::Button remove_button(5, 200, 80, 22, "Remove");
  remove_button.callback((fltk::Callback*)cb_remove, (void *)&tree);

  fltk::Button add_paper_button(5, 224, 80, 22, "Add Paper");
  add_paper_button.callback((fltk::Callback*)cb_add_paper, (void *)&tree);

  fltk::Button add_folder_button(5, 248, 80, 22, "Add Folder");
  add_folder_button.callback((fltk::Callback*)cb_add_folder, (void *)&tree);

  fltk::Button change_look_button(5, 272, 80, 22, "Change Look!");
  change_look_button.callback((fltk::Callback*)cb_change_look , (void *)&tree);

  fltk::CheckButton multi_button(88, 200, 160, 20, "fltk::Browser::MULTI");
  multi_button.callback((fltk::Callback*)cb_multi, (void *)&tree);

  fltk::CheckButton when_changed_button(88, 220, 160, 20, "fltk::WHEN_CHANGED");
  when_changed_button.callback((fltk::Callback*)cb_when_changed, (void *)&tree);

  fltk::CheckButton when_not_changed_button(88, 240, 160, 20, "fltk::WHEN_NOT_CHANGED");
  when_not_changed_button.callback((fltk::Callback*)cb_when_not_changed, (void *)&tree);

  fltk::CheckButton when_release_button(88, 260, 160, 20, "fltk::WHEN_RELEASE");
  when_release_button.callback((fltk::Callback*)cb_when_release, (void *)&tree);
  when_release_button.set_flag(fltk::VALUE);

  fltk::CheckButton when_enter_key_button(88, 280, 160, 20, "fltk::WHEN_ENTER_KEY");
  when_enter_key_button.callback((fltk::Callback*)cb_when_enter_key, (void *)&tree);

  fltk::CheckButton resize(88, 310, 160, 20, "Make 2. column flexible");
  resize.callback((fltk::Callback*)change_resize);

  bm = new fltk::CheckButton (5, 310, 82, 20, "below mouse");
  bm->set();
  bm->callback((fltk::Callback*)below_mouse_cb);

  win.resizable(tree);
  win.end();

  folderSmall = new fltk::xpmImage(folder_small);
  folderSmall2= new fltk::xpmImage(folder_small2);
  folderSmall3= new fltk::xpmImage(folder_small3);

  fileSmall = new fltk::xpmImage(file_small);
  fileSmall2 = new fltk::xpmImage(file_small2);
  bookImg = new fltk::xpmImage(book);
  customImage = new fltk::xpmImage(porsche_xpm);

#if USE_STRING_LIST
  //tree.list(new fltk::String_List("alpha\0beta\0ceta\0delta\0red\0green\0blue\0"));
  tree.list(new fltk::String_List(strings, sizeof(strings)/sizeof(*strings)));
  //tree.list(new fltk::String_List(strings));
#else

  // defining default images for nodes
  cb_change_look(0, &tree);

  // Add some nodes with icons -- some open, some closed.
  fltk::Group* g;
  g = tree.add_group ("aaa\t2.col\t3.col", &tree);
  tree.add_group ("bbb TWO\t2.col\t3.col", g);

  g = tree.add_group ("bbb", &tree);
  tree.add_leaf("ccc\t789", g); 
  tree.add_leaf("ddd\t012", g); 

  
  g = tree.add_group("eee", &tree);
  tree.add_leaf("fff", &tree, customImage); // let's have fun with a custom image
					    // while demonstrating different height nodes
  g = tree.add_group("ggg", g);
  tree.add_leaf("hhh", g); // g decl is not even necessary for next children
  tree.add_leaf("iii", g); // I let it for keeping API orthogonality

  g = tree.add_group("jjj",&tree);
  tree.add_leaf("kkk",g);

  tree.add_leaf("lll");

  g = tree.add_group("mmm", &tree, 0); // let this node closed
  tree.add_leaf("nnn",g);
  tree.add_leaf("ooo",g);

  g = tree.add_group("ppp", g);
  tree.add_leaf("qqq",g);

  g = tree.add_group("rrr", g); // more imbricated groups 
  g = tree.add_group("sss", g);
  g = tree.add_group("ttt", g);

  g = tree.add_group("uuu", &tree);
  tree.add_leaf("vvv", g);
  tree.add_leaf("www", g);

  tree.add_leaf("yyy", g);
  tree.add_leaf("zzz", g);

  // add some widgets:
  fltk::Button * b = new fltk::Button(0,0,100,23,"button");
  b->callback(button_cb);
#if 1
  // fabien: creating the CheckButton below will still mess up the drawing ... 
  //   have to be fixed, but not in the browser i think.
  b = new fltk::CheckButton(0,0,100,23,"CheckButton");

  printf("b->type = %d, group = %d, is_group = %d\n",
	 b->type(), fltk::Widget::GROUP_TYPE, b->is_group());
  b->callback(button_cb);
#endif
  new fltk::Input(0,0,200,23,"Input:");
  new fltk::ValueSlider(0,0,200,23,"Input1:");
  new fltk::ValueSlider(0,0,200,23,"Input2:");
  new fltk::ValueSlider(0,0,200,23,"Input3:");
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

  win.show(argc,argv);

  fltk::run();
  return 0;
}

// End of "$Id$"
