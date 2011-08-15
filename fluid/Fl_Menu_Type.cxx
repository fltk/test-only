//
// "$Id$"
//
// Menu item code for the Fast Light Tool Kit (FLTK).
//
// Menu items are kludged by making a phony fltk3::Box widget so the normal
// widget panel can be used to control them.
//
// This file also contains code to make fltk3::MenuButton, fltk3::MenuBar,
// etc widgets.
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
#include "Fl_Widget_Type.h"
#include "alignment_panel.h"
#include <fltk3/message.h>
#include <fltk3/Menu_.h>
#include <fltk3/Button.h>
#include <fltk3/ValueInput.h>
#include <fltk3/TextDisplay.h>
#include <fltk3/Wrapper.h>
#include "../src/flstring.h"
#include <stdio.h>
#include <stdlib.h>

fltk3::MenuItem menu_item_type_menu[] = {
  {"Normal",0,0,(void*)0},
  {"Toggle",0,0,(void*)fltk3::MENU_TOGGLE},
  {"Radio",0,0,(void*)fltk3::MENU_RADIO},
  {0}};

extern int reading_file;
extern int force_parent;
extern int i18n_type;
extern const char* i18n_include;
extern const char* i18n_function;
extern const char* i18n_file;
extern const char* i18n_set;

static char submenuflag;

void Fl_Input_Choice_Type::build_menu() {
  fltk3::InputChoice* w = (fltk3::InputChoice*)o;
  // count how many fltk3::MenuItem structures needed:
  int n = 0;
  Fl_Type* q;
  for (q = next; q && q->level > level; q = q->next) {
    if (q->is_parent()) n++; // space for null at end of submenu
    n++;
  }
  if (!n) {
    if (menusize) delete[] (fltk3::MenuItem*)(w->menu());
    w->menu(0);
    menusize = 0;
  } else {
    n++; // space for null at end of menu
    if (menusize<n) {
      if (menusize) delete[] (fltk3::MenuItem*)(w->menu());
      menusize = n+10;
      w->menu(new fltk3::MenuItem[menusize]);
    }
    // fill them all in:
    fltk3::MenuItem* m = (fltk3::MenuItem*)(w->menu());
    int lvl = level+1;
    for (q = next; q && q->level > level; q = q->next) {
      Fl_Menu_Item_Type* i = (Fl_Menu_Item_Type*)q;
      if (i->o->image()) i->o->image()->label(m);
      else {
        m->label(i->o->label() ? i->o->label() : "(nolabel)");
        m->labeltype(i->o->labeltype());
      }
      m->shortcut(((fltk3::Button*)(i->o))->shortcut());
      m->callback(0,(void*)i);
      m->flags = i->flags();
      m->labelfont(i->o->labelfont());
      m->labelsize(i->o->labelsize());
      m->labelcolor(i->o->labelcolor());
      if (q->is_parent()) {lvl++; m->flags |= fltk3::SUBMENU;}
      m++;
      int l1 =
	(q->next && q->next->is_menu_item()) ? q->next->level : level;
      while (lvl > l1) {m->label(0); m++; lvl--;}
      lvl = l1;
    }
  }
  o->redraw();
}


Fl_Type *Fl_Menu_Item_Type::make() {
  // Find the current menu item:
  Fl_Type* q = Fl_Type::current;
  Fl_Type* p = q;
  if (p) {
    if ( (force_parent && q->is_menu_item()) || !q->is_parent()) p = p->parent;
  }
  force_parent = 0;
  if (!p || !(p->is_menu_button() || (p->is_menu_item() && p->is_parent()))) {
    fltk3::message("Please select a menu to add to");
    return 0;
  }
  if (!o) {
    o = new fltk3::Button(0,0,100,20); // create template widget
    o->labelsize(Fl_Widget_Type::default_size);
  }

  Fl_Menu_Item_Type* t = submenuflag ? new Fl_Submenu_Type() : new Fl_Menu_Item_Type();
  t->o = new fltk3::Button(0,0,100,20);
  t->factory = this;
  t->add(p);
  if (!reading_file) t->label(submenuflag ? "submenu" : "item");
  return t;
}

Fl_Type *Fl_Submenu_Type::make() {
  submenuflag = 1;
  Fl_Type* t = Fl_Menu_Item_Type::make();
  submenuflag = 0;
  return t;
}

Fl_Menu_Item_Type Fl_Menu_Item_type;
Fl_Submenu_Type Fl_Submenu_type;

////////////////////////////////////////////////////////////////
// Writing the C code:

// test functions in Fl_Widget_Type.C:
int is_name(const char *c);
const char *array_name(Fl_Widget_Type *o);
int isdeclare(const char *c);

// Search backwards to find the parent menu button and return it's name.
// Also put in i the index into the button's menu item array belonging
// to this menu item.
const char* Fl_Menu_Item_Type::menu_name(int& i) {
  i = 0;
  Fl_Type* t = prev;
  while (t && t->is_menu_item()) {
    // be sure to count the {0} that ends a submenu:
    if (t->level > t->next->level) i += (t->level - t->next->level);
    // detect empty submenu:
    else if (t->level == t->next->level && t->is_parent()) i++;
    t = t->prev;
    i++;
  }
  return unique_id(t, "menu", t->name(), t->label());
}

#include "Fluid_Image.h"

void Fl_Menu_Item_Type::write_static() {
  if (callback() && is_name(callback()) && !user_defined(callback()))
    write_declare("extern void %s(fltk3::Menu_*, %s);", callback(),
                  user_data_type() ? user_data_type() : "void*");
  for (int n=0; n < NUM_EXTRA_CODE; n++) {
    if (extra_code(n) && isdeclare(extra_code(n)))
      write_declare("%s", extra_code(n));
  }
  if (callback() && !is_name(callback())) {
    // see if 'o' or 'v' used, to prevent unused argument warnings:
    int use_o = 0;
    int use_v = 0;
    const char *d;
    for (d = callback(); *d;) {
      if (*d == 'o' && !is_id(d[1])) use_o = 1;
      if (*d == 'v' && !is_id(d[1])) use_v = 1;
      do d++; while (is_id(*d));
      while (*d && !is_id(*d)) d++;
    }
    const char* cn = callback_name();
    const char* k = class_name(1);
    if (k) {
      write_c("\nvoid %s::%s_i(fltk3::Menu_*", k, cn);
    } else {
      write_c("\nstatic void %s(fltk3::Menu_*", cn);
    }
    if (use_o) write_c(" o");
    const char* ut = user_data_type() ? user_data_type() : "void*";
    write_c(", %s", ut);
    if (use_v) write_c(" v");
    write_c(") {\n  %s", callback());
    if (*(d-1) != ';') {
      const char *p = strrchr(callback(), '\n');
      if (p) p ++;
      else p = callback();
      // Only add trailing semicolon if the last line is not a preprocessor
      // statement...
      if (*p != '#' && *p) write_c(";");
    }
    write_c("\n}\n");
    if (k) {
      write_c("void %s::%s(fltk3::Menu_* o, %s v) {\n", k, cn, ut);
      write_c("  ((%s*)(o", k);
      Fl_Type* t = parent; while (t->is_menu_item()) t = t->parent;
      Fl_Type *q = 0;
      for (t = t->parent; t && t->is_widget() && !is_class(); q = t, t = t->parent) 
        write_c("->parent()");
      if (!q || strcmp(q->type_name(), "widget_class"))
        write_c("->user_data()");
      write_c("))->%s_i(o,v);\n}\n", cn);
    }
  }
  if (image) {
    if (image->written != write_number) {
      image->write_static();
      image->written = write_number;
    }
  }
  if (next && next->is_menu_item()) return;
  // okay, when we hit last item in the menu we have to write the
  // entire array out:
  const char* k = class_name(1);
  if (k) {
    int i; 
    if (i18n_type) write_c("\nunsigned char %s::%s_i18n_done = 0;", k, menu_name(i));
    write_c("\nfltk3::MenuItem %s::%s[] = {\n", k, menu_name(i));
  } else {
    int i; 
    if (i18n_type) write_c("\nunsigned char %s_i18n_done = 0;", menu_name(i));
    write_c("\nfltk3::MenuItem %s[] = {\n", menu_name(i));
  }
  Fl_Type* t = prev; while (t && t->is_menu_item()) t = t->prev;
  for (Fl_Type* q = t->next; q && q->is_menu_item(); q = q->next) {
    ((Fl_Menu_Item_Type*)q)->write_item();
    int thislevel = q->level; if (q->is_parent()) thislevel++;
    int nextlevel =
      (q->next && q->next->is_menu_item()) ? q->next->level : t->level+1;
    while (thislevel > nextlevel) {write_c(" {0,0,0,0,0,0,0,0,0},\n"); thislevel--;}
  }
  write_c(" {0,0,0,0,0,0,0,0,0}\n};\n");

  if (k) {
    // Write menu item variables...
    t = prev; while (t && t->is_menu_item()) t = t->prev;
    for (Fl_Type* q = t->next; q && q->is_menu_item(); q = q->next) {
      Fl_Menu_Item_Type *m = (Fl_Menu_Item_Type*)q;
      const char *c = array_name(m);
      if (c) {
        if (c==m->name()) {
          // assign a menu item address directly to a variable
          int i; 
          const char* n = ((Fl_Menu_Item_Type *)q)->menu_name(i);
          write_c("fltk3::MenuItem* %s::%s = %s::%s + %d;\n", k, c, k, n, i);
        } else {
          // if the name is an array, only define the array. 
          // The actual assignment is in write_code1()
          write_c("fltk3::MenuItem* %s::%s;\n", k, c);
        }
      }
    }
  }
}

int Fl_Menu_Item_Type::flags() {
  int i = o->type();
  if (((fltk3::Button*)o)->value()) i |= fltk3::MENU_VALUE;
  if (!o->active()) i |= fltk3::MENU_INACTIVE;
  if (!o->visible()) i |= fltk3::MENU_INVISIBLE;
  if (is_parent()) {
    if (user_data() == NULL) i |= fltk3::SUBMENU;
    else i |= fltk3::SUBMENU_POINTER;
  }
  if (hotspot()) i |= fltk3::MENU_DIVIDER;
  return i;
}

void Fl_Menu_Item_Type::write_item() {
  static const char * const labeltypes[] = {
    "fltk3::NORMAL_LABEL",
    "fltk3::NO_LABEL",
    "fltk3::SHADOW_LABEL",
    "fltk3::ENGRAVED_LABEL",
    "fltk3::EMBOSSED_LABEL",
    "fltk3::MULTI_LABEL",
    "fltk3::ICON_LABEL",
    "fltk3::IMAGE_LABEL"
  };

  write_c(" {");
  if (image) write_c("0");
  else if (label()) write_cstring(label()); // we will call i18n when the widget is instantiated for the first time
  else write_c("\"\"");
  if (((fltk3::Button*)o)->shortcut()) {
		int s = ((fltk3::Button*)o)->shortcut();
		if (use_FL_COMMAND && (s & (fltk3::CTRL|fltk3::META))) {
			write_c(", fltk3::COMMAND|0x%x, ", s & ~(fltk3::CTRL|fltk3::META));
		} else {
			write_c(", 0x%x, ", s);
		}
  } else
    write_c(", 0, ");
  if (callback()) {
    const char* k = is_name(callback()) ? 0 : class_name(1);
    if (k) {
      write_c(" (fltk3::Callback*)%s::%s,", k, callback_name());
    } else {
      write_c(" (fltk3::Callback*)%s,", callback_name());
    }
  } else
    write_c(" 0,");
  if (user_data())
    write_c(" (void*)(%s),", user_data());
  else
    write_c(" 0,");
  write_c(" %d, %s, %d, %d, %d", flags(),
	  labeltypes[o->labeltype()], o->labelfont(), o->labelsize(), o->labelcolor());
  write_c("},\n");
}

void Fl_Menu_Item_Type::write_code1() {
  int i; const char* mname = menu_name(i);
  if (!prev->is_menu_item()) {
    // for first menu item, declare the array
    if (class_name(1)) {
      if (i18n_type) write_h("  static unsigned char %s_i18n_done;\n", mname);
      write_h("  static fltk3::MenuItem %s[];\n", mname);
    } else {
      if (i18n_type) write_h("extern unsigned char %s_i18n_done;\n", mname);
      write_h("extern fltk3::MenuItem %s[];\n", mname);
    }
  }

  const char *c = array_name(this);
  if (c) {
    if (class_name(1)) {
      write_public(public_);
      write_h("  static fltk3::MenuItem *%s;\n", c);
    } else {
      if (c==name())
        write_h("#define %s (%s+%d)\n", c, mname, i);
      else
        write_h("extern fltk3::MenuItem *%s;\n", c);
    }
  }

  if (callback()) {
    if (!is_name(callback()) && class_name(1)) {
      const char* cn = callback_name();
      const char* ut = user_data_type() ? user_data_type() : "void*";
      write_public(0);
      write_h("  void %s_i(fltk3::Menu_*, %s);\n", cn, ut);
      write_h("  static void %s(fltk3::Menu_*, %s);\n", cn, ut);
    }
  }

  int init = 0;
  // if the name is an array variable, assign the value here
  if (name() && strchr(name(), '[')) {
    write_c("%s%s = &%s[%d];\n", indent(), name(), mname, i);
  }
  if (image) {
    if (!init) {
      init = 1;
      write_c("%s{ fltk3::MenuItem* o = &%s[%d];\n", indent(), mname, i);
    }
    image->write_code("o");
  }
  for (int n=0; n < NUM_EXTRA_CODE; n++)
    if (extra_code(n) && !isdeclare(extra_code(n))) {
      if (!init) {
	init = 1;
	write_c("%s{ fltk3::MenuItem* o = &%s[%d];\n", indent(), mname, i);
      }
      write_c("%s  %s\n", indent(), extra_code(n));
    }
  if (init) write_c("%s}\n",indent());
}

void Fl_Menu_Item_Type::write_code2() {}

////////////////////////////////////////////////////////////////
// This is the base class for widgets that contain a menu (ie
// subclasses of fltk3::Menu_.
// This is a parent widget and menu items can be added as
// children.  An actual array of Fl_Menu_Items is kept parallel
// with the child objects and updated as they change.

void Fl_Menu_Type::build_menu() {
  fltk3::Menu_* w = (fltk3::Menu_*)o;
  // count how many fltk3::MenuItem structures needed:
  int n = 0;
  Fl_Type* q;
  for (q = next; q && q->level > level; q = q->next) {
    if (q->is_parent()) n++; // space for null at end of submenu
    n++;
  }
  if (!n) {
    if (menusize) delete[] (fltk3::MenuItem*)(w->menu());
    w->menu(0);
    menusize = 0;
  } else {
    n++; // space for null at end of menu
    if (menusize<n) {
      if (menusize) delete[] (fltk3::MenuItem*)(w->menu());
      menusize = n+10;
      w->menu(new fltk3::MenuItem[menusize]);
    }
    // fill them all in:
    fltk3::MenuItem* m = (fltk3::MenuItem*)(w->menu());
    int lvl = level+1;
    for (q = next; q && q->level > level; q = q->next) {
      Fl_Menu_Item_Type* i = (Fl_Menu_Item_Type*)q;
      if (i->o->image()) i->o->image()->label(m);
      else {
        m->label(i->o->label() ? i->o->label() : "(nolabel)");
        m->labeltype(i->o->labeltype());
      }
      m->shortcut(((fltk3::Button*)(i->o))->shortcut());
      m->callback(0,(void*)i);
      m->flags = i->flags();
      m->labelfont(i->o->labelfont());
      m->labelsize(i->o->labelsize());
      m->labelcolor(i->o->labelcolor());
      if (q->is_parent()) {lvl++; m->flags |= fltk3::SUBMENU;}
      m++;
      int l1 =
	(q->next && q->next->is_menu_item()) ? q->next->level : level;
      while (lvl > l1) {m->label(0); m++; lvl--;}
      lvl = l1;
    }
  }
  o->redraw();
}

Fl_Type* Fl_Menu_Type::click_test(int, int) {
  if (selected) return 0; // let user move the widget
  fltk3::Menu_* w = (fltk3::Menu_*)o;
  if (!menusize) return 0;
  const fltk3::MenuItem* save = w->mvalue();
  w->value((fltk3::MenuItem*)0);
  fltk3::pushed(w);
  w->handle(fltk3::PUSH);
  const fltk3::MenuItem* m = w->mvalue();
  if (m) {
    // restore the settings of toggles & radio items:
    if (m->flags & (fltk3::MENU_RADIO | fltk3::MENU_TOGGLE)) build_menu();
    return (Fl_Type*)(m->user_data());
  }
  w->value(save);
  return this;
}

void Fl_Menu_Type::write_code2() {
  if (next && next->is_menu_item()) {
    if (i18n_type) {
      // take care of i18n now!
      Fl_Menu_Item_Type *mi = (Fl_Menu_Item_Type*)next;
      int i, nItem = 0, nLabel = 0;
      const char *mName = mi->menu_name(i);
      for (Fl_Type* q = next; q && q->is_menu_item(); q = q->next) {
        if (((Fl_Menu_Item_Type*)q)->label()) nLabel++;
	int thislevel = q->level; if (q->is_parent()) thislevel++;
	int nextlevel =
	    (q->next && q->next->is_menu_item()) ? q->next->level : next->level+1;
	nItem += 1 + ((thislevel > nextlevel) ? (thislevel-nextlevel) : 0);
      }
      if (nLabel) {
        write_c("%sif (!%s_i18n_done) {\n", indent(), mName);
        write_c("%s  int i=0;\n", indent());
        write_c("%s  for ( ; i<%d; i++)\n", indent(), nItem);
        write_c("%s    if (%s[i].label())\n", indent(), mName);
        switch (i18n_type) {
          case 1:
            write_c("%s      %s[i].label(%s(%s[i].label()));\n",
                    indent(), mName, i18n_function, mName);
            break;
          case 2:
            write_c("%s      %s[i].label(catgets(%s,%s,i+%d,%s[i].label()));\n",
                    indent(), mName, i18n_file[0] ? i18n_file : "_catalog", 
                    i18n_set, mi->msgnum(), mName);
            break;
        }
        write_c("%s  %s_i18n_done = 1;\n", indent(), mName);
        write_c("%s}\n", indent());
      }
    }
    write_c("%s%s->menu(%s);\n", indent(), name() ? name() : "o",
	    unique_id(this, "menu", name(), label()));
  }
  Fl_Widget_Type::write_code2();
}

void Fl_Menu_Type::copy_properties() {
  Fl_Widget_Type::copy_properties();
  fltk3::Menu_ *s = (fltk3::Menu_*)o, *d = (fltk3::Menu_*)live_widget;
  d->menu(s->menu());
  d->down_box(s->down_box());
  d->textcolor(s->textcolor());
  d->textfont(s->textfont());
  d->textsize(s->textsize());
}

////////////////////////////////////////////////////////////////

#include <fltk3/MenuButton.h>
fltk3::MenuItem button_type_menu[] = {
  {"normal",0,0,(void*)0},
  {"popup1",0,0,(void*)fltk3::MenuButton::POPUP1},
  {"popup2",0,0,(void*)fltk3::MenuButton::POPUP2},
  {"popup3",0,0,(void*)fltk3::MenuButton::POPUP3},
  {"popup12",0,0,(void*)fltk3::MenuButton::POPUP12},
  {"popup23",0,0,(void*)fltk3::MenuButton::POPUP23},
  {"popup13",0,0,(void*)fltk3::MenuButton::POPUP13},
  {"popup123",0,0,(void*)fltk3::MenuButton::POPUP123},
  {0}};

Fl_Menu_Button_Type Fl_Menu_Button_type;

////////////////////////////////////////////////////////////////

fltk3::MenuItem dummymenu[] = {{"CHOICE"},{0}};

Fl_Choice_Type Fl_Choice_type;

Fl_Input_Choice_Type Fl_Input_Choice_type;

void Fl_Input_Choice_Type::copy_properties() {
  Fl_Widget_Type::copy_properties();
  fltk3::InputChoice *s = (fltk3::InputChoice*)o, *d = (fltk3::InputChoice*)live_widget;
  d->menu(s->menu());
  d->down_box(s->down_box());
  d->textcolor(s->textcolor());
  d->textfont(s->textfont());
  d->textsize(s->textsize());
}

Fl_Type* Fl_Input_Choice_Type::click_test(int, int) {
  if (selected) return 0; // let user move the widget
  fltk3::Menu_* w = ((fltk3::InputChoice*)o)->menubutton();
  if (!menusize) return 0;
  const fltk3::MenuItem* save = w->mvalue();
  w->value((fltk3::MenuItem*)0);
  fltk3::pushed(w);
  w->handle(fltk3::PUSH);
  const fltk3::MenuItem* m = w->mvalue();
  if (m) {
    // restore the settings of toggles & radio items:
    if (m->flags & (fltk3::MENU_RADIO | fltk3::MENU_TOGGLE)) build_menu();
    return (Fl_Type*)(m->user_data());
  }
  w->value(save);
  return this;
}

////////////////////////////////////////////////////////////////

Fl_Menu_Bar_Type Fl_Menu_Bar_type;

////////////////////////////////////////////////////////////////
// Shortcut entry item in panel:

#include <fltk3/Output.h>
#include "Shortcut_Button.h"
#include <fltk3/draw.h>

void Shortcut_Button::draw() {
  if (value()) draw_box(fltk3::DOWN_BOX, (fltk3::Color)9);
  else draw_box(fltk3::UP_BOX, fltk3::WHITE);
  fltk3::font(fltk3::HELVETICA,14); fltk3::color(fltk3::FOREGROUND_COLOR);
	if (use_FL_COMMAND && (svalue & (fltk3::CTRL|fltk3::META))) {
		char buf[1024];
		fl_snprintf(buf, 1023, "Command+%s", fltk3::shortcut_label(svalue&~(fltk3::CTRL|fltk3::META)));
		fltk3::draw(buf,x()+6,y(),w(),h(),fltk3::ALIGN_LEFT);
	} else {
		fltk3::draw(fltk3::shortcut_label(svalue),x()+6,y(),w(),h(),fltk3::ALIGN_LEFT);
	}
}

int Shortcut_Button::handle(int e) {
  when(0); type(fltk3::TOGGLE_BUTTON);
  if (e == fltk3::KEYBOARD) {
    if (!value()) return 0;
    int v = fltk3::event_text()[0];
    if ( (v > 32 && v < 0x7f) || (v > 0xa0 && v <= 0xff) ) {
      if (isupper(v)) {
        v = tolower(v);
        v |= fltk3::SHIFT;
      }
      v = v | (fltk3::event_state()&(fltk3::META|fltk3::ALT|fltk3::CTRL));
    } else {
      v = (fltk3::event_state()&(fltk3::META|fltk3::ALT|fltk3::CTRL|fltk3::SHIFT)) | fltk3::event_key();
      if (v == fltk3::BackSpaceKey && svalue) v = 0;
    }
    if (v != svalue) {svalue = v; set_changed(); redraw(); do_callback(); }
    return 1;
  } else if (e == fltk3::UNFOCUS) {
    int c = changed(); value(0); if (c) set_changed();
    return 1;
  } else if (e == fltk3::FOCUS) {
    return value();
  } else {
    int r = Button::handle(e);
    if (e == fltk3::RELEASE && value() && fltk3::focus() != this) take_focus();
    return r;
  }
}
  
void shortcut_in_cb(Shortcut_Button* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::current->is_button())
      i->svalue = ((fltk3::Button*)(Fl_Panel::current_widget()->o))->shortcut();
    else if (Fl_Panel::current->is_input())
      i->svalue = ((fltk3::Input_*)(Fl_Panel::current_widget()->o))->shortcut();
    else if (Fl_Panel::current->is_value_input())
      i->svalue = ((fltk3::ValueInput*)(Fl_Panel::current_widget()->o))->shortcut();
    else if (Fl_Panel::current->is_text_display())
      i->svalue = ((fltk3::TextDisplay*)(Fl_Panel::current_widget()->o))->shortcut();
    else {
      i->hide();
      return;
    }
    i->show();
    i->redraw();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next)
      if (o->selected && o->is_button()) {
	fltk3::Button* b = (fltk3::Button*)(((Fl_Widget_Type*)o)->o);
        if (b->shortcut()!=i->svalue) mod = 1;
	b->shortcut(i->svalue);
	if (o->is_menu_item()) ((Fl_Widget_Type*)o)->redraw();
      } else if (o->selected && o->is_input()) {
	fltk3::Input_* b = (fltk3::Input_*)(((Fl_Widget_Type*)o)->o);
        if (b->shortcut()!=i->svalue) mod = 1;
	b->shortcut(i->svalue);
      } else if (o->selected && o->is_value_input()) {
	fltk3::ValueInput* b = (fltk3::ValueInput*)(((Fl_Widget_Type*)o)->o);
        if (b->shortcut()!=i->svalue) mod = 1;
	b->shortcut(i->svalue);
      } else if (o->selected && o->is_text_display()) {
	fltk3::TextDisplay* b = (fltk3::TextDisplay*)(((Fl_Widget_Type*)o)->o);
        if (b->shortcut()!=i->svalue) mod = 1;
	b->shortcut(i->svalue);
      }
    if (mod) set_modflag(1);
  }
}

//
// End of "$Id$".
//
