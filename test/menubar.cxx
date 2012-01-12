//
// "$Id$"
//
// Menubar test program for the Fast Light Tool Kit (FLTK).
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

#include <fltk3/run.h>
#include <fltk3/Box.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/SysMenuBar.h>
#include <fltk3/ToggleButton.h>
#include <fltk3/MenuButton.h>
#include <fltk3/Choice.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/fltk3/flstring.h"
#include <fltk3/draw.h>

void window_cb(fltk3::Widget* w, void*) {
  puts("window callback called");
  ((fltk3::DoubleWindow *)w)->hide();
}

void test_cb(fltk3::Widget* w, void*) {
  fltk3::Menu_* mw = (fltk3::Menu_*)w;
  const fltk3::MenuItem* m = mw->mvalue();
  if (!m)
    printf("NULL\n");
  else if (m->shortcut())
    printf("%s - %s\n", m->label(), fltk3::shortcut_label(m->shortcut()));
  else
    printf("%s\n", m->label());
}

void quit_cb(fltk3::Widget*, void*) {exit(0);}

fltk3::MenuItem hugemenu[100];

fltk3::MenuItem menutable[] = {
  {"foo",0,0,0,fltk3::MENU_INACTIVE},
  {"&File",0,0,0,fltk3::SUBMENU},
    {"&Open",	fltk3::ALT+'o', 0, 0, fltk3::MENU_INACTIVE},
    {"&Close",	0,	0},
    {"&Quit",	fltk3::ALT+'q', quit_cb, 0, fltk3::MENU_DIVIDER},
    {"shortcut",'a'},
    {"shortcut",fltk3::SHIFT+'a'},
    {"shortcut",fltk3::CTRL+'a'},
    {"shortcut",fltk3::CTRL+fltk3::SHIFT+'a'},
    {"shortcut",fltk3::ALT+'a'},
    {"shortcut",fltk3::ALT+fltk3::SHIFT+'a'},
    {"shortcut",fltk3::ALT+fltk3::CTRL+'a'},
    {"shortcut",fltk3::ALT+fltk3::SHIFT+fltk3::CTRL+'a', 0,0, fltk3::MENU_DIVIDER},
    {"shortcut",'\r'/*fltk3::Enter*/},
    {"shortcut",fltk3::CTRL+fltk3::EnterKey, 0,0, fltk3::MENU_DIVIDER},
    {"shortcut",fltk3::FKey+1},
    {"shortcut",fltk3::SHIFT+fltk3::FKey+1},
    {"shortcut",fltk3::CTRL+fltk3::FKey+1},
    {"shortcut",fltk3::SHIFT+fltk3::CTRL+fltk3::FKey+1},
    {"shortcut",fltk3::ALT+fltk3::FKey+1},
    {"shortcut",fltk3::ALT+fltk3::SHIFT+fltk3::FKey+1},
    {"shortcut",fltk3::ALT+fltk3::CTRL+fltk3::FKey+1},
    {"shortcut",fltk3::ALT+fltk3::SHIFT+fltk3::CTRL+fltk3::FKey+1, 0,0, fltk3::MENU_DIVIDER},
    {"&Submenus", fltk3::ALT+'S',	0, (void*)"Submenu1", fltk3::SUBMENU},
      {"A very long menu item"},
      {"&submenu",fltk3::CTRL+'S',	0, (void*)"submenu2", fltk3::SUBMENU},
	{"item 1"},
	{"item 2"},
	{"item 3"},
	{"item 4"},
	{0},
      {"after submenu"},
      {0},
    {0},
  {"&Edit",fltk3::FKey+2,0,0,fltk3::SUBMENU},
    {"Undo",	fltk3::ALT+'z',	0},
    {"Redo",	fltk3::ALT+'r',	0, 0, fltk3::MENU_DIVIDER},
    {"Cut",	fltk3::ALT+'x',	0},
    {"Copy",	fltk3::ALT+'c',	0},
    {"Paste",	fltk3::ALT+'v',	0},
    {"Inactive",fltk3::ALT+'d',	0, 0, fltk3::MENU_INACTIVE},
    {"Clear",	0,	0, 0, fltk3::MENU_DIVIDER},
    {"Invisible",fltk3::ALT+'e',	0, 0, fltk3::MENU_INVISIBLE},
    {"Preferences",0,	0},
    {"Size",	0,	0},
    {0},
  {"&Checkbox",fltk3::FKey+3,0,0,fltk3::SUBMENU},
    {"&Alpha",	fltk3::FKey+2,	0, (void *)1, fltk3::MENU_TOGGLE},
    {"&Beta",	0,	0, (void *)2, fltk3::MENU_TOGGLE},
    {"&Gamma",	0,	0, (void *)3, fltk3::MENU_TOGGLE},
    {"&Delta",	0,	0, (void *)4, fltk3::MENU_TOGGLE|fltk3::MENU_VALUE},
    {"&Epsilon",0,	0, (void *)5, fltk3::MENU_TOGGLE},
    {"&Pi",	0,	0, (void *)6, fltk3::MENU_TOGGLE},
    {"&Mu",	0,	0, (void *)7, fltk3::MENU_TOGGLE|fltk3::MENU_DIVIDER},
    {"Red",	0,	0, (void *)1, fltk3::MENU_TOGGLE, 0, 0, 0, 1},
    {"Black",	0,	0, (void *)1, fltk3::MENU_TOGGLE|fltk3::MENU_DIVIDER},
    {"00",	0,	0, (void *)1, fltk3::MENU_TOGGLE},
    {"000",	0,	0, (void *)1, fltk3::MENU_TOGGLE},
    {0},
  {"&Radio",0,0,0,fltk3::SUBMENU},
    {"&Alpha",	0,	0, (void *)1, fltk3::MENU_RADIO},
    {"&Beta",	0,	0, (void *)2, fltk3::MENU_RADIO},
    {"&Gamma",	0,	0, (void *)3, fltk3::MENU_RADIO},
    {"&Delta",	0,	0, (void *)4, fltk3::MENU_RADIO|fltk3::MENU_VALUE},
    {"&Epsilon",0,	0, (void *)5, fltk3::MENU_RADIO},
    {"&Pi",	0,	0, (void *)6, fltk3::MENU_RADIO},
    {"&Mu",	0,	0, (void *)7, fltk3::MENU_RADIO|fltk3::MENU_DIVIDER},
    {"Red",	0,	0, (void *)1, fltk3::MENU_RADIO},
    {"Black",	0,	0, (void *)1, fltk3::MENU_RADIO|fltk3::MENU_DIVIDER},
    {"00",	0,	0, (void *)1, fltk3::MENU_RADIO},
    {"000",	0,	0, (void *)1, fltk3::MENU_RADIO},
    {0},
  {"&Font",0,0,0,fltk3::SUBMENU /*, 0, fltk3::BOLD, 20*/},
    {"Normal",	0, 0, 0, 0, 0, 0, 14},
    {"Bold",	0, 0, 0, 0, 0, fltk3::BOLD, 14},
    {"Italic",	0, 0, 0, 0, 0, fltk3::ITALIC, 14},
    {"BoldItalic",0,0,0, 0, 0, fltk3::BOLD+fltk3::ITALIC, 14},
    {"Small",	0, 0, 0, 0, 0, fltk3::BOLD+fltk3::ITALIC, 10},
    {"Emboss",	0, 0, 0, 0, fltk3::EMBOSSED_LABEL},
    {"Engrave",	0, 0, 0, 0, fltk3::ENGRAVED_LABEL},
    {"Shadow",	0, 0, 0, 0, fltk3::SHADOW_LABEL},
    {"@->",	0, 0, 0, 0, fltk3::SYMBOL_LABEL},
    {0},
  {"&International",0,0,0,fltk3::SUBMENU},
    {"Sharp Ess",0x0000df},
    {"A Umlaut",0x0000c4},
    {"a Umlaut",0x0000e4},
    {"Euro currency",fltk3::COMMAND+0x0020ac},
    {"the &\xc3\xbc Umlaut"},  // &uuml;
    {"the capital &\xc3\x9c"}, // &Uuml;
    {"convert \xc2\xa5 to &\xc2\xa3"}, // Yen to GBP
    {"convert \xc2\xa5 to &\xe2\x82\xac"}, // Yen to Euro
    {"Hangul character Sios &\xe3\x85\x85"},
    {"Hangul character Cieuc", 0x003148},
    {0},
  {"E&mpty",0,0,0,fltk3::SUBMENU},
    {0},
  {"&Inactive", 0,	0, 0, fltk3::MENU_INACTIVE|fltk3::SUBMENU},
    {"A very long menu item"},
    {"A very long menu item"},
    {0},
  {"Invisible",0,	0, 0, fltk3::MENU_INVISIBLE|fltk3::SUBMENU},
    {"A very long menu item"},
    {"A very long menu item"},
    {0},
  {"&Huge", 0, 0, (void*)hugemenu, fltk3::SUBMENU_POINTER},
  {"button",fltk3::FKey+4, 0, 0, fltk3::MENU_TOGGLE},
  {0}
};

fltk3::MenuItem pulldown[] = {
  {"Red",	fltk3::ALT+'r'},
  {"Green",	fltk3::ALT+'g'},
  {"Blue",	fltk3::ALT+'b'},
  {"Strange",	fltk3::ALT+'s', 0, 0, fltk3::MENU_INACTIVE},
  {"&Charm",	fltk3::ALT+'c'},
  {"Truth",	fltk3::ALT+'t'},
  {"Beauty",	fltk3::ALT+'b'},
  {0}
};

#ifdef __APPLE__
fltk3::MenuItem menu_location[] = {
  {"Window menu bar",	0, 0, 0, fltk3::MENU_VALUE},
  {"System menu bar",	},
  {0}
};

fltk3::SysMenuBar* smenubar;

void menu_location_cb(fltk3::Widget* w, void* data) 
{
  fltk3::MenuBar *menubar = (fltk3::MenuBar*)data;
  if (((fltk3::Choice*)w)->value() == 1) { // switch to system menu bar
    menubar->hide();
    const fltk3::MenuItem *menu = menubar->menu();
    smenubar = new  fltk3::SysMenuBar(0,0,0,30); 
    smenubar->menu(menu);
    smenubar->callback(test_cb);
  }
  else { // switch to window menu bar
    delete smenubar;
    menubar->show();
  }
}
#endif // __APPLE__

#define WIDTH 700

fltk3::Menu_* menus[4];

int main(int argc, char **argv) {
  //fltk3::set_color(fltk3::Color(15),0,0,128);
  for (int i=0; i<99; i++) {
    char buf[100];
    sprintf(buf,"item %d",i);
    hugemenu[i].text = strdup(buf);
  }
  fltk3::DoubleWindow window(WIDTH,400);
  window.callback(window_cb);
  fltk3::MenuBar menubar(0,0,WIDTH,30); menubar.menu(menutable);
  menubar.callback(test_cb);
  menus[0] = &menubar;
  fltk3::MenuButton mb1(100,100,120,25,"&menubutton"); mb1.menu(pulldown);
  mb1.tooltip("this is a menu button");
  mb1.callback(test_cb);
  menus[1] = &mb1;
  fltk3::Choice ch(300,100,80,25,"&choice:"); ch.menu(pulldown);
  ch.tooltip("this is a choice menu");
  ch.callback(test_cb);
  menus[2] = &ch;
  fltk3::MenuButton mb(0,0,WIDTH,400,"&popup");
  mb.type(fltk3::MenuButton::POPUP3);
  mb.box(fltk3::NO_BOX);
  mb.menu(menutable);
  mb.remove(1); // delete the "File" submenu
  mb.callback(test_cb);
  menus[3] = &mb;
  fltk3::Box b(200,200,200,100,"Press right button\nfor a pop-up menu");
  window.resizable(&mb);
  window.size_range(300,400,0,400);
#ifdef __APPLE__
  fltk3::Choice ch2(500,100,150,25,"Use:"); 
  ch2.menu(menu_location);
  ch2.callback(menu_location_cb, &menubar);
#endif
  window.end();
  window.show(argc, argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
