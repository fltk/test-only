
#include <FL/Fl.H>
#include <FL/Fl_Item.H>
#include <FL/Fl_Item_Group.H>
#include <FL/Fl_Divider.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Tooltip.H>
#include <FL/fl_draw.H>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FL/Fl_Hor_Slider.H>

#define WIDTH 600
#define HEIGHT 24 //30 // use 25 for better Windoze look

void callback(Fl_Widget* w, void*) {
  printf("Callback for %s\n", w->label());
}

void build_hierarchy() {
  Fl_Item_Group* g = new Fl_Item_Group("submenu&1");
  new Fl_Item("Item &1");
  new Fl_Item("Item &2");
  new Fl_Item("Item &3");
  new Fl_Item("Item &4");
  g->end();
  g = new Fl_Item_Group("submenu&2");
  (new Fl_Item("Item &1"))->label_size(10);
  (new Fl_Item("Item &2"))->label_size(14);
  (new Fl_Item("Item &3"))->label_size(18);
  (new Fl_Item("Item &4"))->label_size(22);
  g->deactivate();
  Fl_Group* g1 = new Fl_Item_Group("&nested menu");
  new Fl_Item("Item &1");
  new Fl_Item("Item &2");
  Fl_Group* g2 = new Fl_Item_Group("deeper");
  (new Fl_Item("Very low level items"))->deactivate();
  (new Fl_Item("Are here on this menu"))->deactivate();
  new Fl_Item("In this test");
  new Fl_Item("Program");
  g2->end();
  g1->end();
  g->end();
}

void quit_cb(Fl_Widget*, void*) {exit(0);}

int main(int argc, char **argv) {
  Fl_Window window(WIDTH,400);
  Fl_Menu_Bar menubar(0,0,WIDTH,HEIGHT);
  menubar.callback(callback);
  menubar.begin();

  Fl_Item_Group file("&File");
  Fl_Item* o = new Fl_Item("Quit");
  o->shortcut(FL_ALT+'q');
  o->callback(quit_cb);
  new Fl_Divider();
  (new Fl_Hor_Slider(0,0,100,30))->value(.3);
  build_hierarchy();
  file.end();
  Fl_Item_Group edit("&Edit");
  (new Fl_Item("Undo"))->shortcut(FL_ALT+'z');
  (new Fl_Item("Cut"))->shortcut(FL_ALT+'x');
  (new Fl_Item("Copy"))->shortcut(FL_ALT+'c');
  (new Fl_Item("Paste"))->shortcut(FL_ALT+'v');
  build_hierarchy();
  edit.end();
  edit.deactivate();
  Fl_Item_Group options("&Options");
  o = new Fl_Item("Red"); o->type(FL_RADIO_ITEM); o->set_flag(FL_MENU_STAYS_UP);
  o = new Fl_Item("Green"); o->type(FL_RADIO_ITEM); o->set_flag(FL_MENU_STAYS_UP);
  o = new Fl_Item("Blue"); o->type(FL_RADIO_ITEM); o->set_flag(FL_MENU_STAYS_UP);
  o = new Fl_Item("Aqua"); o->type(FL_RADIO_ITEM); o->set_flag(FL_MENU_STAYS_UP);
  o = new Fl_Item("Toggle 1"); o->type(FL_TOGGLE_ITEM); o->set_flag(FL_MENU_STAYS_UP);
  o = new Fl_Item("Toggle 2"); o->type(FL_TOGGLE_ITEM); o->set_flag(FL_MENU_STAYS_UP);
  o = new Fl_Item("Toggle 3"); o->type(FL_TOGGLE_ITEM); o->set_flag(FL_MENU_STAYS_UP);
  o = new Fl_Item("Toggle 4"); o->type(FL_TOGGLE_ITEM); o->set_flag(FL_MENU_STAYS_UP);
  options.end();
  Fl_Item item("&Item"); item.type(FL_TOGGLE_ITEM);
  menubar.end();

  Fl_Box box(0,HEIGHT,WIDTH,400-HEIGHT, "Press right button\nfor a pop-up menu");
  box.color(FL_WHITE);
  box.box(FL_FLAT_BOX);

  Fl_Menu_Button mb(0,25,WIDTH,400-HEIGHT, "popup");
  mb.callback(callback);
  mb.begin();
  build_hierarchy();
  mb.end();
  mb.type(Fl_Menu_Item::POPUP3);

  Fl_Menu_Button mb1(100,100,120,25,"&menubutton");
  mb1.callback(callback);
  mb1.begin();
  build_hierarchy();
  mb1.end();
  mb1.tooltip("This is a menu button");

  Fl_Choice ch(300,100,90,25,"&choice:");
  ch.callback(callback);
  ch.begin();
  build_hierarchy();
  ch.end();
  ch.tooltip("This is a choice");

  window.resizable(box);
  window.size_range(300,20);
  window.end();
  window.show(argc, argv);
  return Fl::run();
}
