//
// "$Id: Fl_Menu_Type.cxx,v 1.29 2000/02/14 11:32:39 bill Exp $"
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

#include <FL/Fl.H>
#include "Fl_Type.h"
#include <FL/fl_message.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Item.H>
#include <FL/Fl_Item_Group.H>
#include <FL/Fl_Divider.H>
#include <FL/Fl_Button.H>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Fl_Menu_Item menu_item_type_menu[] = {
  {"Normal",0,0,(void*)0},
  {"Toggle",0,0,(void*)FL_TOGGLE_ITEM},
  {"Radio",0,0,(void*)FL_RADIO_ITEM},
  {0}};

extern int reading_file;
extern int force_parent;

Fl_Widget *Fl_Menu_Item_Type::widget(int,int,int,int) {
  return new Fl_Item(reading_file ? 0 : "item");
}

Fl_Widget *Fl_Submenu_Type::widget(int,int,int,int) {
  Fl_Item_Group *g = new Fl_Item_Group(reading_file ? 0 : "submenu");
  Fl_Group::current(0);
  return g;
}

Fl_Menu_Item_Type Fl_Menu_Item_type;
Fl_Submenu_Type Fl_Submenu_type;

////////////////////////////////////////////////////////////////

extern int fl_dont_execute; // in Fl_Menu.cxx

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

#include <FL/Fl_Menu_Button.H>
Fl_Menu_Item button_type_menu[] = {
  {"normal",0,0,(void*)0},
  {"popup1",0,0,(void*)Fl_Menu_Button::POPUP1},
  {"popup2",0,0,(void*)Fl_Menu_Button::POPUP2},
  {"popup3",0,0,(void*)Fl_Menu_Button::POPUP3},
  {"popup12",0,0,(void*)Fl_Menu_Button::POPUP12},
  {"popup23",0,0,(void*)Fl_Menu_Button::POPUP23},
  {"popup13",0,0,(void*)Fl_Menu_Button::POPUP13},
  {"popup123",0,0,(void*)Fl_Menu_Button::POPUP123},
  {0}};

Fl_Menu_Button_Type Fl_Menu_Button_type;
Fl_Choice_Type Fl_Choice_type;
Fl_Menu_Bar_Type Fl_Menu_Bar_type;

////////////////////////////////////////////////////////////////
// Shortcut entry item in panel:

#include <FL/Fl_Output.H>
#include "Shortcut_Button.h"
#include <FL/fl_draw.H>

void Shortcut_Button::draw() {
  FL_THIN_UP_BOX->draw(x(),y(),w(),h(),value() ? FL_BLUE_SELECTION_COLOR : FL_WHITE, flags());
  fl_font(FL_HELVETICA,14); fl_color(value() ? FL_WHITE : FL_BLACK);
  fl_draw(fl_shortcut_label(svalue),x()+6,y(),w(),h(),FL_ALIGN_LEFT);
}

int Shortcut_Button::handle(int e) {
  when(0); type(FL_TOGGLE_BUTTON);
  if (e == FL_KEYBOARD) {
    if (!value()) return 0;
    int v = Fl::event_text()[0];
    if (v > 32 && v < 0x7f || v > 0xa0 && v <= 0xff) {
      v = v | Fl::event_state()&(FL_META|FL_ALT|FL_CTRL);
    } else {
      v = Fl::event_state()&(FL_META|FL_ALT|FL_CTRL|FL_SHIFT) | Fl::event_key();
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
    for (Fl_Type *o = Fl_Type::first; o; o = o->next)
      if (o->selected && o->is_widget()) {
	Fl_Widget* b = ((Fl_Widget_Type*)o)->o;
	b->shortcut(i->svalue);
	if (o->is_menu_item()) ((Fl_Widget_Type*)o)->redraw();
      }
  }
  Fl_Color tc = FL_BLACK;
  if (NOT_DEFAULT(current_widget, shortcut)) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

//
// End of "$Id: Fl_Menu_Type.cxx,v 1.29 2000/02/14 11:32:39 bill Exp $".
//
