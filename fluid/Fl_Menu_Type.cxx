//
// "$Id: Fl_Menu_Type.cxx,v 1.50 2002/12/10 01:46:15 easysw Exp $"
//
// Menu item code for the Fast Light Tool Kit (FLTK).
//
// Vastly simplified from the 1.0 version, since 2.0 uses real child
// widgets to represent menu items.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <fltk/events.h>
#include "FluidType.h"
#include <fltk/ask.h>
#include <fltk/Menu.h>
#include <fltk/Item.h>
#include <fltk/ItemGroup.h>
#include <fltk/Divider.h>
#include <fltk/Button.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const Enumeration item_type_menu[] = {
  {"Normal", "NORMAL", (void*)0},
  {"Toggle", "TOGGLE", (void*)fltk::Item::TOGGLE},
  {"Radio",  "RADIO",  (void*)fltk::Item::RADIO},
  {0}};

class ItemType : public WidgetType {
public:
  const Enumeration* subtypes() const {return item_type_menu;}
  const char* type_name() const {return "fltk::Item";}
  int is_menu_item() const {return 1;}
  int is_button() const {return 1;} // this gets shortcut to work
  fltk::Widget *widget(int x,int y,int w,int h);
  WidgetType *_make() {return new ItemType();}
};

class Dividertype : public WidgetType {
public:
  const Enumeration* subtypes() const {return 0;}
  const char* type_name() const {return "fltk::Divider";}
  int is_menu_item() const {return 1;}
  fltk::Widget *widget(int x,int y,int w,int h) { return new fltk::Divider; }
  WidgetType *_make() {return new Dividertype();}
};

class SubmenuType : public GroupType {
public:
  const Enumeration* subtypes() const {return 0;}
  const char* type_name() const {return "fltk::ItemGroup";}
  int is_menu_item() const {return 1;}
  fltk::Widget *widget(int x,int y,int w,int h);
  WidgetType *_make() {return new SubmenuType();}
};

extern int reading_file;

fltk::Widget *ItemType::widget(int,int,int,int) {
  return new fltk::Item(reading_file ? 0 : "item");
}

fltk::Widget *SubmenuType::widget(int,int,int,int) {
  fltk::ItemGroup *g = new fltk::ItemGroup(reading_file ? 0 : "submenu");
  fltk::Group::current(0);
  return g;
}

ItemType Itemtype;
Dividertype Dividertype;
SubmenuType Submenutype;

////////////////////////////////////////////////////////////////

// This is the base class for widgets that contain a menu (ie
// subclasses of fltk::Menu).

class MenuType : public GroupType {
public:
  int is_menu_button() const {return 1;}
  MenuType() : GroupType() {}
  ~MenuType() {}
  FluidType* click_test(int x, int y);
};

extern FL_API bool fl_dont_execute; // in Menu.cxx

FluidType* MenuType::click_test(int, int) {
  if (selected) return 0; // let user move the widget
  fltk::Menu* w = (fltk::Menu*)o;
  if (!w->size()) return 0;
  fltk::Widget* save = w->item();
  w->item(0);
  fl_dont_execute = true;
  fltk::pushed(w);
  w->handle(fltk::PUSH);
  fl_dont_execute = false;
  const fltk::Widget* m = w->item();
  if (m) return (FluidType*)(m->user_data());
  w->item(save);
  return this;
}

////////////////////////////////////////////////////////////////

#include <fltk/PopupMenu.h>
static const Enumeration button_type_menu[] = {
  {"normal", 0,		(void*)0},
  {"popup1", "POPUP1",	(void*)fltk::PopupMenu::POPUP1},
  {"popup2", "POPUP2",	(void*)fltk::PopupMenu::POPUP2},
  {"popup3", "POPUP3",	(void*)fltk::PopupMenu::POPUP3},
  {"popup12","POPUP12", (void*)fltk::PopupMenu::POPUP12},
  {"popup23","POPUP23", (void*)fltk::PopupMenu::POPUP23},
  {"popup13","POPUP13", (void*)fltk::PopupMenu::POPUP13},
  {"popup123","POPUP123",(void*)fltk::PopupMenu::POPUP123},
  {0}};

class PopupMenuType : public MenuType {
  const Enumeration *subtypes() const {return button_type_menu;}
public:
  virtual const char *type_name() const {return "fltk::PopupMenu";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::PopupMenu(x,y,w,h,"menu");}
  WidgetType *_make() {return new PopupMenuType();}
};

PopupMenuType PopupMenutype;

////////////////////////////////////////////////////////////////

#include <fltk/Choice.h>

class ChoiceType : public MenuType {
public:
  int is_choice() const {return 1;}
  virtual const char *type_name() const {return "fltk::Choice";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    fltk::Choice *o = new fltk::Choice(x,y,w,h,"choice:");
    return o;
  }
  WidgetType *_make() {return new ChoiceType();}
};

ChoiceType Choicetype;

////////////////////////////////////////////////////////////////

#include <fltk/MenuBar.h>

class MenuBarType : public MenuType {
public:
  virtual const char *type_name() const {return "fltk::MenuBar";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::MenuBar(x,y,w,h);}
  WidgetType *_make() {return new MenuBarType();}
};

MenuBarType MenuBartype;

#include <fltk/InputBrowser.h>
static const Enumeration input_browser_type_menu[] = {
  {"Normal",		0,	(void*)fltk::InputBrowser::NORMAL},
  {"Non-Editable","NONEDITABLE",(void*)fltk::InputBrowser::NONEDITABLE},
  {"Indented",	"INDENTED",	(void*)fltk::InputBrowser::INDENTED},
  {"Non-Editable Indented","NONEDITABLE_INDENTED",(void*)fltk::InputBrowser::NONEDITABLE_INDENTED},
  {0}};
class InputBrowserType : public MenuType {
  int is_input_browser() const {return 1;}
  const Enumeration *subtypes() const {return input_browser_type_menu;}
public:
  virtual const char *type_name() const {return "fltk::InputBrowser";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::InputBrowser(x,y,w,h);
  }
  WidgetType *_make() {return new InputBrowserType();}
};
InputBrowserType InputBrowsertype;

#include <fltk/Browser.h>
static const Enumeration browser_type_menu[] = {
  {"Single",	0,	(void*)fltk::Browser::NORMAL},
  {"Multi",	0,	(void*)fltk::Browser::MULTI, "fltk::Multi_Browser"},
  {0}};
class BrowserType : public MenuType {
  int is_browser() const {return 1;}
  const Enumeration *subtypes() const {return browser_type_menu;}
public:
  virtual const char *type_name() const {return "fltk::Browser";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::Browser(x,y,w,h);
  }
  WidgetType *_make() {return new BrowserType();}
};
BrowserType Browsertype;

////////////////////////////////////////////////////////////////
// Shortcut entry item in panel:

#include <fltk/Output.h>
#include "Shortcut_Button.h"
#include <fltk/draw.h>

void Shortcut_Button::draw() {
  label(fltk::key_name(svalue));
  fltk::Button::draw();
}

int Shortcut_Button::handle(int e) {
  when(0); type(TOGGLE);
  if (e == fltk::KEY) {
    if (!value()) return 0;
    int v = fltk::event_text()[0];
    if (v > 32 && v < 0x7f || v > 0xa0 && v <= 0xff) {
      v = v | fltk::event_state()&(fltk::COMMAND|fltk::ALT|fltk::CTRL);
    } else {
      v = fltk::event_state()&(fltk::COMMAND|fltk::ALT|fltk::CTRL|fltk::SHIFT) | fltk::event_key();
      if (v == fltk::BackSpaceKey && svalue) v = 0;
    }
    if (v != svalue) {svalue = v; do_callback(); redraw();}
    return 1;
  } else if (e == fltk::UNFOCUS) {
    int c = changed(); value(0); if (c) set_changed();
    return 1;
  } else if (e == fltk::FOCUS) {
    return value();
  } else {
    int r = fltk::Button::handle(e);
    if (e == fltk::RELEASE && value() && fltk::focus() != this) take_focus();
    return r;
  }
}
  
#define NOT_DEFAULT(W, what) (W->o->what() != ((WidgetType*)(W->factory))->o->what())

void shortcut_in_cb(Shortcut_Button* i, void* v) {
  if (v == LOAD) {
    if (!current_widget->is_button() &&
	!((WidgetType*)(current_widget->factory))->o->shortcut()) {i->hide(); return;}
    i->show();
    i->svalue = ((current_widget->o))->shortcut();
    i->redraw();
  } else {
    for (FluidType *o = FluidType::first; o; o = o->walk())
      if (o->selected && o->is_widget()) {
	fltk::Widget* b = ((WidgetType*)o)->o;
	b->shortcut(i->svalue);
	if (o->is_menu_item()) ((WidgetType*)o)->redraw();
      }
  }
  fltk::Color tc = fltk::BLACK;
  if (NOT_DEFAULT(current_widget, shortcut)) tc = fltk::RED;
  if (i->labelcolor() != tc)
    { i->labelcolor(tc); i->redraw_label(); }
}

//
// End of "$Id: Fl_Menu_Type.cxx,v 1.50 2002/12/10 01:46:15 easysw Exp $".
//
