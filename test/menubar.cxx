//
// "$Id: menubar.cxx,v 1.38 2002/12/10 02:01:06 easysw Exp $"
//
// Menubar test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/Fl.h>
#include <fltk/Fl_Output.h>
#include <fltk/Fl_Box.h>
#include <fltk/Fl_Window.h>
#include <fltk/Fl_Menu_Bar.h>
#include <fltk/Fl_Toggle_Button.h>
#include <fltk/Fl_Menu_Button.h>
#include <fltk/Fl_Choice.h>
#include <fltk/Fl_Tooltip.h>
#include <fltk/fl_draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Fl_Window *window;

Fl_Menu_* menus[4];

void test_cb(Fl_Widget* w, void*) {
  Fl_Menu_* mw = (Fl_Menu_*)w;
  Fl_Widget* m = mw->item();
  if (!m)
    printf("NULL\n");
  else if (m->shortcut())
    printf("%s - %s\n", m->label(), Fl::key_name(m->shortcut()));
  else
    printf("%s\n", m->label());
  if (!strcmp("item 77", m->label())) {
    if (mw->find("button")) mw->replace("button", "Spitzak");
    else mw->replace("Spitzak", "button");
    menus[0]->redraw();
  }
  m->do_callback();
}

void quit_cb(Fl_Widget*, void*) {exit(0);}

Fl_Menu_Item hugemenu[100];

Fl_Menu_Item menutable[] = {
  {"foo",0,0,0,FL_MENU_INACTIVE},
  {"&File",0,0,0,FL_SUBMENU},
    {"&Open",	FL_ALT+'o', 0, 0, FL_MENU_INACTIVE},
    {"&Close",	0,	0},
    {"&Quit",	FL_ALT+'q', quit_cb, 0, FL_MENU_DIVIDER},
    {"shortcut",'a'},
    {"shortcut",FL_SHIFT+'a'},
    {"shortcut",FL_CTRL+'a'},
    {"shortcut",FL_CTRL+FL_SHIFT+'a'},
    {"shortcut",FL_ALT+'a'},
    {"shortcut",FL_ALT+FL_SHIFT+'a'},
    {"shortcut",FL_ALT+FL_CTRL+'a'},
    {"shortcut",FL_ALT+FL_SHIFT+FL_CTRL+'a', 0,0, FL_MENU_DIVIDER},
  {"shortcut",'\r'/*FL_Enter*/},
    {"shortcut",FL_CTRL+FL_Enter, 0,0, FL_MENU_DIVIDER},
    {"shortcut",FL_F(1)},
    {"shortcut",FL_SHIFT+FL_F(1)},
    {"shortcut",FL_CTRL+FL_F(1)},
    {"shortcut",FL_SHIFT+FL_CTRL+FL_F(1)},
    {"shortcut",FL_ALT+FL_F(1)},
    {"shortcut",FL_ALT+FL_SHIFT+FL_F(1)},
    {"shortcut",FL_ALT+FL_CTRL+FL_F(1)},
    {"shortcut",FL_ALT+FL_SHIFT+FL_CTRL+FL_F(1), 0,0, FL_MENU_DIVIDER},
    {"&Submenus", FL_ALT+'S',	0, (void*)"Submenu1", FL_SUBMENU},
      {"A very long menu item"},
      {"&submenu",FL_CTRL+'S',	0, (void*)"submenu2", FL_SUBMENU},
	{"item 1"},
	{"item 2"},
	{"item 3"},
	{"item 4"},
	{0},
      {"after submenu"},
      {0},
    {0},
  {"&Edit",0,0,0,FL_SUBMENU},
    {"Undo",	FL_ALT+'z',	0},
    {"Redo",	FL_ALT+'r',	0, 0, FL_MENU_DIVIDER},
    {"Cut",	FL_ALT+'x',	0},
    {"Copy",	FL_ALT+'c',	0},
    {"Paste",	FL_ALT+'v',	0},
    {"Inactive",FL_ALT+'d',	0, 0, FL_MENU_INACTIVE},
    {"Clear",	0,	0, 0, FL_MENU_DIVIDER},
    {"Invisible",FL_ALT+'e',	0, 0, FL_MENU_INVISIBLE},
    {"Preferences",0,	0},
    {"Size",	0,	0},
    {0},
  {"&Checkbox",0,0,0,FL_SUBMENU},
    {"&Alpha",	0,	0, (void *)1, FL_MENU_TOGGLE|FL_MENU_VALUE},
    {"&Beta",	0,	0, (void *)2, FL_MENU_TOGGLE},
    {"&Gamma",	0,	0, (void *)3, FL_MENU_TOGGLE},
    {"&Delta",	0,	0, (void *)4, FL_MENU_TOGGLE|FL_MENU_VALUE},
    {"&Epsilon",0,	0, (void *)5, FL_MENU_TOGGLE},
    {"&Pi",	0,	0, (void *)6, FL_MENU_TOGGLE},
    {"&Mu",	0,	0, (void *)7, FL_MENU_TOGGLE|FL_MENU_DIVIDER},
    {"Red",	0,	0, (void *)1, FL_MENU_TOGGLE},
    {"Black",	0,	0, (void *)1, FL_MENU_TOGGLE|FL_MENU_DIVIDER},
    {"00",	0,	0, (void *)1, FL_MENU_TOGGLE},
    {"000",	0,	0, (void *)1, FL_MENU_TOGGLE},
    {0},
  {"&Radio",0,0,0,FL_SUBMENU},
    {"&Alpha",	0,	0, (void *)1, FL_MENU_RADIO},
    {"&Beta",	0,	0, (void *)2, FL_MENU_RADIO},
    {"&Gamma",	0,	0, (void *)3, FL_MENU_RADIO},
    {"&Delta",	0,	0, (void *)4, FL_MENU_RADIO|FL_MENU_VALUE},
    {"&Epsilon",0,	0, (void *)5, FL_MENU_RADIO},
    {"&Pi",	0,	0, (void *)6, FL_MENU_RADIO},
    {"&Mu",	0,	0, (void *)7, FL_MENU_RADIO|FL_MENU_DIVIDER},
    {"Red",	0,	0, (void *)1, FL_MENU_RADIO},
    {"Black",	0,	0, (void *)1, FL_MENU_RADIO|FL_MENU_DIVIDER},
    {"00",	0,	0, (void *)1, FL_MENU_RADIO},
    {"000",	0,	0, (void *)1, FL_MENU_RADIO},
    {0},
  {"&Font",0,0,0,FL_SUBMENU},
    {"Normal",	0, 0},
    {"Bold",	0, 0},
    {"Italic",	0, 0},
    {"BoldItalic",0,0},
    {"Small",	0, 0},
    {"Large",	0, 0},
    {"Emboss",	0, 0},
    {"Engrave",	0, 0},
    {"Shadow",	0, 0},
    {"@->",	0, 0},
    {0},
  {"E&mpty",0,0,0,FL_SUBMENU},
    {0},
  {"&Inactive", 0,	0, 0, FL_MENU_INACTIVE|FL_SUBMENU},
    {"A very long menu item"},
    {"A very long menu item"},
    {0},
  {"Invisible",0,	0, 0, FL_MENU_INVISIBLE|FL_SUBMENU},
    {"A very long menu item"},
    {"A very long menu item"},
    {0},
  {"&Huge", 0, 0, (void*)hugemenu, FL_SUBMENU_POINTER},
  {"button",0, 0, 0, FL_MENU_TOGGLE},
  {0}
};

Fl_Menu_Item pulldown[] = {
  {"Red",	FL_ALT+'r'},
  {"Green",	FL_ALT+'g'},
  {"Blue",	FL_ALT+'b'},
  {"Strange",	FL_ALT+'s'},
  {"&Charm",	FL_ALT+'c'},
  {"Truth",	FL_ALT+'t'},
  {"Beauty",	FL_ALT+'b'},
  {0}
};

#define WIDTH 600
#define HEIGHT 24 //30 // use 25 for better Windoze look

int main(int argc, char **argv) {
  for (int i=0; i<99; i++) {
    char buf[100];
    sprintf(buf,"item %d",i);
    hugemenu[i].text = strdup(buf);
  }
  
  Fl_Window window(WIDTH,400);
  window.color(FL_WHITE);
  window.tooltip("Press right button\nfor a pop-up menu");
  Fl_Menu_Bar menubar(0,0,WIDTH,HEIGHT); menubar.menu(menutable);
  menubar.find("&Font/Normal")->label_font(FL_HELVETICA);
  menubar.find("&Font/Bold")->label_font(FL_HELVETICA_BOLD);
  menubar.find("&Font/Italic")->label_font(FL_HELVETICA_ITALIC);
  menubar.find("&Font/BoldItalic")->label_font(FL_HELVETICA_BOLD_ITALIC);
  menubar.find("&Font/Small")->label_size(10);
  menubar.find("&Font/Large")->label_size(24);
  menubar.find("&Font/Emboss")->label_type(FL_EMBOSSED_LABEL);
  menubar.find("&Font/Engrave")->label_type(FL_ENGRAVED_LABEL);
  menubar.find("&Font/Shadow")->label_type(FL_SHADOW_LABEL);
  menubar.find("&Font/@->")->label_type(FL_SYMBOL_LABEL);
  menubar.find("&Checkbox/Red")->label_color(FL_RED); // label text red
  menubar.find("&Checkbox/Red")->selection_text_color(FL_RED); // label text red when selected
  menubar.find("&Checkbox/Red")->text_color(FL_RED); // check mark red
  menubar.find("&Checkbox/Black")->label_color(FL_BLACK);
  menubar.find("&Checkbox/Black")->selection_text_color(FL_BLACK);
  menubar.find("&Checkbox/Black")->text_color(FL_BLACK);
  menubar.find("&Radio/Red")->label_color(FL_RED);
  menubar.find("&Radio/Red")->selection_text_color(FL_RED);
  menubar.find("&Radio/Red")->text_color(FL_RED);
  menubar.find("&Radio/Black")->label_color(FL_BLACK);
  menubar.find("&Radio/Black")->selection_text_color(FL_BLACK);
  menubar.find("&Radio/Black")->text_color(FL_BLACK);
  menubar.find("&Huge/item 69")->deactivate();
  menubar.callback(test_cb);
  menubar.tooltip("This is a menu bar");
  menus[0] = &menubar;
  Fl_Menu_Button mb1(100,100,120,25,"&menubutton"); mb1.menu(pulldown);
  mb1.callback(test_cb);
  mb1.tooltip("This is a menu button");
  menus[1] = &mb1;
  Fl_Choice ch(300,100,90,25,"&choice:"); ch.menu(pulldown);
  ch.callback(test_cb);
  ch.tooltip("This is a choice");
  menus[2] = &ch;
  Fl_Menu_Button mb(0,25,WIDTH,400-HEIGHT/*,"&popup"*/);
  mb.type(Fl_Menu_Button::POPUP3);
  mb.menu(menutable);
  mb.callback(test_cb);
  menus[3] = &mb;
  window.resizable(&mb);
  window.size_range(300,20);
  window.end();
  window.show(argc, argv);
  return Fl::run();
}

//
// End of "$Id: menubar.cxx,v 1.38 2002/12/10 02:01:06 easysw Exp $".
//
