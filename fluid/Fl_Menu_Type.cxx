//
// "$Id: Fl_Menu_Type.cxx,v 1.47 2002/01/23 08:46:00 spitzak Exp $"
//
// Menu item code for the Fast Light Tool Kit (FLTK).
//
// Vastly simplified from the 1.0 version, since 2.0 uses real child
// widgets to represent menu items.
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#include <fltk/Fl.h>
#include "Fl_Type.h"
#include <fltk/fl_message.h>
#include <fltk/Fl_Menu_.h>
#include <fltk/Fl_Item.h>
#include <fltk/Fl_Item_Group.h>
#include <fltk/Fl_Divider.h>
#include <fltk/Fl_Button.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const Enumeration menu_item_type_menu[] = {
  {"Normal", "NORMAL", (void*)0},
  {"Toggle", "TOGGLE", (void*)Fl_Item::TOGGLE},
  {"Radio",  "RADIO",  (void*)Fl_Item::RADIO},
  {0}};

class Fl_Menu_Item_Type : public Fl_Widget_Type {
public:
  const Enumeration* subtypes() const {return menu_item_type_menu;}
  const char* type_name() const {return "Fl_Item";}
  int is_menu_item() const {return 1;}
  int is_button() const {return 1;} // this gets shortcut to work
  Fl_Widget *widget(int x,int y,int w,int h);
  Fl_Widget_Type *_make() {return new Fl_Menu_Item_Type();}
};

class Fl_Menu_Divider_Type : public Fl_Widget_Type {
public:
  const Enumeration* subtypes() const {return 0;}
  const char* type_name() const {return "Fl_Divider";}
  int is_menu_item() const {return 1;}
  Fl_Widget *widget(int x,int y,int w,int h) { return new Fl_Divider; }
  Fl_Widget_Type *_make() {return new Fl_Menu_Divider_Type();}
};

class Fl_Submenu_Type : public Fl_Group_Type {
public:
  const Enumeration* subtypes() const {return 0;}
  const char* type_name() const {return "Fl_Item_Group";}
  int is_menu_item() const {return 1;}
  Fl_Widget *widget(int x,int y,int w,int h);
  Fl_Widget_Type *_make() {return new Fl_Submenu_Type();}
};

extern int reading_file;

Fl_Widget *Fl_Menu_Item_Type::widget(int,int,int,int) {
  return new Fl_Item(reading_file ? 0 : "item");
}

Fl_Widget *Fl_Submenu_Type::widget(int,int,int,int) {
  Fl_Item_Group *g = new Fl_Item_Group(reading_file ? 0 : "submenu");
  Fl_Group::current(0);
  return g;
}

Fl_Menu_Item_Type Fl_Menu_Item_type;
Fl_Menu_Divider_Type Fl_Menu_Divider_type;
Fl_Submenu_Type Fl_Submenu_type;

////////////////////////////////////////////////////////////////

// This is the base class for widgets that contain a menu (ie
// subclasses of Fl_Menu_).

class Fl_Menu_Type : public Fl_Group_Type {
public:
  int is_menu_button() const {return 1;}
  Fl_Menu_Type() : Fl_Group_Type() {}
  ~Fl_Menu_Type() {}
  Fl_Type* click_test(int x, int y);
};

extern FL_API int fl_dont_execute; // in Fl_Menu.cxx

Fl_Type* Fl_Menu_Type::click_test(int, int) {
  if (selected) return 0; // let user move the widget
  Fl_Menu_* w = (Fl_Menu_*)o;
  if (!w->size()) return 0;
  Fl_Widget* save = w->item();
  w->item(0);
  fl_dont_execute = 1;
  Fl::pushed(w);
  w->handle(FL_PUSH);
  fl_dont_execute = 0;
  const Fl_Widget* m = w->item();
  if (m) return (Fl_Type*)(m->user_data());
  w->item(save);
  return this;
}

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Menu_Button.h>
static const Enumeration button_type_menu[] = {
  {"normal", 0,		(void*)0},
  {"popup1", "POPUP1",	(void*)Fl_Menu_Button::POPUP1},
  {"popup2", "POPUP2",	(void*)Fl_Menu_Button::POPUP2},
  {"popup3", "POPUP3",	(void*)Fl_Menu_Button::POPUP3},
  {"popup12","POPUP12", (void*)Fl_Menu_Button::POPUP12},
  {"popup23","POPUP23", (void*)Fl_Menu_Button::POPUP23},
  {"popup13","POPUP13", (void*)Fl_Menu_Button::POPUP13},
  {"popup123","POPUP123",(void*)Fl_Menu_Button::POPUP123},
  {0}};

class Fl_Menu_Button_Type : public Fl_Menu_Type {
  const Enumeration *subtypes() const {return button_type_menu;}
public:
  virtual const char *type_name() const {return "Fl_Menu_Button";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Menu_Button(x,y,w,h,"menu");}
  Fl_Widget_Type *_make() {return new Fl_Menu_Button_Type();}
};

Fl_Menu_Button_Type Fl_Menu_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Choice.h>

class Fl_Choice_Type : public Fl_Menu_Type {
public:
  int is_choice() const {return 1;}
  virtual const char *type_name() const {return "Fl_Choice";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    Fl_Choice *o = new Fl_Choice(x,y,w,h,"choice:");
    return o;
  }
  Fl_Widget_Type *_make() {return new Fl_Choice_Type();}
};

Fl_Choice_Type Fl_Choice_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Menu_Bar.h>

class Fl_Menu_Bar_Type : public Fl_Menu_Type {
public:
  virtual const char *type_name() const {return "Fl_Menu_Bar";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Menu_Bar(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Menu_Bar_Type();}
};

Fl_Menu_Bar_Type Fl_Menu_Bar_type;

#include <fltk/Fl_Input_Browser.h>
static const Enumeration input_browser_type_menu[] = {
  {"Normal",		0,	(void*)Fl_Input_Browser::NORMAL},
  {"Non-Editable","NONEDITABLE",(void*)Fl_Input_Browser::NONEDITABLE},
  {"Indented",	"INDENTED",	(void*)Fl_Input_Browser::INDENTED},
  {"Non-Editable Indented","NONEDITABLE_INDENTED",(void*)Fl_Input_Browser::NONEDITABLE_INDENTED},
  {0}};
class Fl_Input_Browser_Type : public Fl_Menu_Type {
  int is_input_browser() const {return 1;}
  const Enumeration *subtypes() const {return input_browser_type_menu;}
public:
  virtual const char *type_name() const {return "Fl_Input_Browser";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Input_Browser(x,y,w,h);
  }
  Fl_Widget_Type *_make() {return new Fl_Input_Browser_Type();}
};
Fl_Input_Browser_Type Fl_Input_Browser_type;

#include <fltk/Fl_Browser.h>
static const Enumeration browser_type_menu[] = {
  {"Single",	0,	(void*)Fl_Browser::NORMAL},
  {"Multi",	0,	(void*)Fl_Browser::MULTI, "Fl_Multi_Browser"},
  {0}};
class Fl_Browser_Type : public Fl_Menu_Type {
  int is_browser() const {return 1;}
  const Enumeration *subtypes() const {return browser_type_menu;}
public:
  virtual const char *type_name() const {return "Fl_Browser";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Browser(x,y,w,h);
  }
  Fl_Widget_Type *_make() {return new Fl_Browser_Type();}
};
Fl_Browser_Type Fl_Browser_type;

////////////////////////////////////////////////////////////////
// Shortcut entry item in panel:

#include <fltk/Fl_Output.h>
#include "Shortcut_Button.h"
#include <fltk/fl_draw.h>

void Shortcut_Button::draw() {
  box()->draw(0, 0, w(), h(),
		   value() ? selection_color() : color());
  fl_font(text_font(), text_size());
  fl_color(value() ? selection_text_color() : text_color());
  fl_draw(Fl::key_name(svalue), 6, 0, w(), h(), FL_ALIGN_LEFT);
}

int Shortcut_Button::handle(int e) {
  when(0); type(TOGGLE);
  if (e == FL_KEYBOARD) {
    if (!value()) return 0;
    int v = Fl::event_text()[0];
    if (v > 32 && v < 0x7f || v > 0xa0 && v <= 0xff) {
      v = v | Fl::event_state()&(FL_WIN|FL_ALT|FL_CTRL);
    } else {
      v = Fl::event_state()&(FL_WIN|FL_ALT|FL_CTRL|FL_SHIFT) | Fl::event_key();
      if (v == FL_BackSpace && svalue) v = 0;
    }
    if (v != svalue) {svalue = v; do_callback(); redraw();}
    return 1;
  } else if (e == FL_UNFOCUS) {
    int c = changed(); value(0); if (c) set_changed();
    return 1;
  } else if (e == FL_FOCUS) {
    return value();
  } else {
    int r = Fl_Button::handle(e);
    if (e == FL_RELEASE && value() && Fl::focus() != this) take_focus();
    return r;
  }
}
  
#define NOT_DEFAULT(W, what) (W->o->what() != ((Fl_Widget_Type*)(W->factory))->o->what())

void shortcut_in_cb(Shortcut_Button* i, void* v) {
  if (v == LOAD) {
    if (!current_widget->is_button() &&
	!((Fl_Widget_Type*)(current_widget->factory))->o->shortcut()) {i->hide(); return;}
    i->show();
    i->svalue = ((current_widget->o))->shortcut();
    i->redraw();
  } else {
    for (Fl_Type *o = Fl_Type::first; o; o = o->walk())
      if (o->selected && o->is_widget()) {
	Fl_Widget* b = ((Fl_Widget_Type*)o)->o;
	b->shortcut(i->svalue);
	if (o->is_menu_item()) ((Fl_Widget_Type*)o)->redraw();
      }
  }
  Fl_Color tc = FL_BLACK;
  if (NOT_DEFAULT(current_widget, shortcut)) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->redraw_label(); }
}

//
// End of "$Id: Fl_Menu_Type.cxx,v 1.47 2002/01/23 08:46:00 spitzak Exp $".
//
