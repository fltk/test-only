//
// "$Id: Fl_Widget_Type.cxx,v 1.77 2001/07/23 09:50:04 spitzak Exp $"
//
// Widget type code for the Fast Light Tool Kit (FLTK).
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
#include <fltk/Fl_Group.h>
#include "Fluid_Plugins.h"
#include "Fl_Type.h"
#include <fltk/fl_message.h>
#include <fltk/Fl_Slider.h>
#include <fltk/Fl_Window.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "coding_style.h"

// Make an Fl_Widget_Type subclass instance.
// It figures out the automatic size and parent of the new widget,
// creates the Fl_Widget (by calling the virtual function _make),
// adds it to the Fl_Widget hierarchy, creates a new Fl_Type
// instance, sets the widget pointers, and makes all the display
// update correctly...

extern int reading_file;
int force_parent;
extern int gridx;
extern int gridy;

int Fl_Widget_Type::is_widget() const {return 1;}

const char* subclassname(Fl_Type* l) {
  if (l->is_widget()) {
    Fl_Widget_Type* p = (Fl_Widget_Type*)l;
    const char* c = p->subclass();
    if (c) return c;
    if (p->o->type() == FL_WINDOW+1) return "Fl_Double_Window";
  }
  return l->type_name();
}

Fl_Type *Fl_Widget_Type::make() {
  // Find the current widget, or widget to copy:
  Fl_Type *qq = Fl_Type::current;
  while (qq && !qq->is_widget()) qq = qq->parent;
  if (!qq) {
    fl_message("Please select a widget");
    return 0;
  }
  Fl_Widget_Type* q = (Fl_Widget_Type*)qq;
  // find the parent widget:
  Fl_Widget_Type* p = q;
  if ((force_parent || !p->is_group()) && p->parent->is_widget())
    p = (Fl_Widget_Type*)(p->parent);
  force_parent = 0;

  // Figure out a border between widget and window:
  int B = p->o->w()/2; if (p->o->h()/2 < B) B = p->o->h()/2; if (B>25) B = 25;

  // Figure out a position and size for the widget
  int X,Y,W,H;
  if (is_group()) {	// fill the parent with the widget
    X = B;
    W = p->o->w()-B;
    Y = B;
    H = p->o->h()-B;
  } else if (q != p) {	// copy position and size of current widget
    W = q->o->w();
    H = q->o->h();
    X = q->o->x()+W;
    Y = q->o->y();
    if (X+W > p->o->w()) {
      X = q->o->x();
      Y = q->o->y()+H;
      if (Y+H > p->o->h()) Y = B;
    }
  } else {	// just make it small and square...
    X = B;
    Y = B;
    W = H = B;
  }

  // satisfy the grid requirements (otherwise it edits really strangely):
  if (gridx>1) {
    X = (X/gridx)*gridx;
    W = ((W-1)/gridx+1)*gridx;
  }
  if (gridy>1) {
    Y = (Y/gridy)*gridy;
    H = ((H-1)/gridy+1)*gridy;
  }

  // Construct the Fl_Type:
  Fl_Style::start("style1");
  Fl_Widget_Type *t = _make();
  if (!o) o = widget(0,0,100,100); // create template widget
  t->factory = this;
  // Construct the Fl_Widget:
  t->o = widget(X,Y,W,H);
  Fl_Style::start("fluid_style");
  if (reading_file) t->o->label(0);
  else if (t->o->label()) t->label(t->o->label()); // allow editing
  t->o->user_data((void*)t);
  // Put it in the parent:
  //  ((Fl_Group *)(p->o))->add(t->o); (done by Fl_Type::add())
  // add to browser:
  t->add(p);
  t->redraw();
  return t;
}

#include "Fluid_Image.h"

void Fl_Widget_Type::setimage(Fluid_Image *i) {
  if (i == image) return;
  if (image) image->decrement();
  if (i) {i->increment(); i->label(o);} else o->image(0);
  image = i;
  redraw();
}

void Fl_Widget_Type::setlabel(const char *n) {
  o->label(n);
  redraw();
}

Fl_Widget_Type::Fl_Widget_Type() {
  for (int n=0; n<NUM_EXTRA_CODE; n++) {extra_code_[n] = 0; subclass_ = 0;}
  hotspot_ = 0;
  image = 0;
  o = 0;
  public_ = 1;
  set_xy = 0;
}

Fl_Widget_Type::~Fl_Widget_Type() {
  if (o) {
    o->hide();
    if (o->parent()) ((Fl_Group*)o->parent())->remove(*o);
    delete o;
  }
}

void Fl_Widget_Type::extra_code(int m,const char *n) {
  storestring(n,extra_code_[m]);
}

extern void redraw_browser();
void Fl_Widget_Type::subclass(const char *n) {
  if (storestring(n,subclass_)) redraw_browser();
}

void Fl_Widget_Type::redraw() {
  Fl_Type *t = this;
  if (is_menu_item()) {
    // find the menu button that parents this menu:
    do t = t->parent; while (t && t->is_menu_item());
  } else {
    while (t->parent && t->parent->is_widget()) t = t->parent;
  }
  if (t) ((Fl_Widget_Type*)t)->o->redraw();
}

// sorts all selected children by position:
void sort(Fl_Type *parent) {
  for (Fl_Type* f = parent->first_child; f;) {
    sort(f);
    Fl_Type* next = f->next_brother;
    if (f->selected && (f->is_widget() && !f->is_menu_item())) {
      Fl_Widget* fw = ((Fl_Widget_Type*)f)->o;
      Fl_Type *g; // we will insert before this
      for (g = parent->first_child; g != f; g = g->next_brother) {
	if (!g->selected) continue;
	Fl_Widget* gw = ((Fl_Widget_Type*)g)->o;
	if (gw->y() > fw->y()) break;
	if (gw->y() == fw->y() && gw->x() > fw->x()) break;
      }
      if (g != f) f->move_before(g);
    }
    f = next;
  }
}

////////////////////////////////////////////////////////////////
// The control panels!

#include "widget_panel.h"
#include <fltk/Fl_Color_Chooser.h>

// All the callbacks use the argument to indicate whether to load or store.
// This avoids the need for pointers to all the widgets, and keeps the
// code localized in the callbacks.
// A value of LOAD means to load.  The hope is that this will not collide
// with any actual useful values for the argument.  I also use this to
// initialized parts of the widget that are nyi by fluid.

Fl_Widget_Type *current_widget; // one of the selected ones
static int numselected; // number selected
static int haderror;
static Fl_Window *the_panel;

void name_cb(Fl_Input* o, void *v) {
  if (v == LOAD) {
    if (numselected != 1) {
      static char buf[16];
      sprintf(buf,"(%d widgets)",numselected);
      o->window()->label(buf);
      o->hide();
    } else {
      o->static_value(current_widget->name());
      o->show();
      o->window()->label(current_widget->title());
    }
  } else {
    if (numselected == 1) {
      current_widget->name(o->value());
      // I don't update window title, as it probably is being closed
      // and wm2 (a window manager) barfs if you retitle and then
      // hide a window:
      // o->window()->label(current_widget->title());
    }
  }
  Fl_Color tc = FL_BLACK;
  if (current_widget->name() && *(current_widget->name())) tc = FL_RED;
  if (o->label_color() != tc)
    { o->label_color(tc); o->damage_label(); }
}

#define for_all_selected_widgets() \
for (Fl_Type *o = Fl_Type::first; o; o = o->walk()) \
  if (o->selected && o->is_widget())

void name_public_cb(Fl_Light_Button* i, void* v) {
  if (v == LOAD) {
    i->value(current_widget->public_);
  } else {
    for_all_selected_widgets() {
      modflag = 1;
      ((Fl_Widget_Type*)o)->public_ = i->value();
    } 	
  }
  if (!i->value()) i->label_color(FL_RED);
  else i->label_color(FL_BLACK);
  i->redraw();
}    

static char* oldlabel;
static unsigned oldlabellen;

void label_cb(Fl_Input* i, void *v) {
  if (v == LOAD) {
    i->static_value(current_widget->label());
    if (strlen(i->value()) >= oldlabellen) {
      oldlabellen = strlen(i->value())+128;
      oldlabel = (char*)realloc(oldlabel,oldlabellen);
    }
    strcpy(oldlabel,i->value());
  } else {
    for_all_selected_widgets() o->label(i->value());
  }
  Fl_Color tc = FL_BLACK;
  if (current_widget->label() && *(current_widget->label())) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

static char* oldtooltip;
static unsigned oldtooltiplen;

void tooltip_cb(Fl_Input* i, void *v) {
  if (v == LOAD) {
    if (current_widget->o->type() >= FL_WINDOW) { i->hide(); return; }
    i->show();
    i->static_value(current_widget->tooltip());
    if (strlen(i->value()) >= oldtooltiplen) {
      oldtooltiplen = strlen(i->value())+128;
      oldtooltip = (char*)realloc(oldtooltip,oldtooltiplen);
    }
    strcpy(oldtooltip,i->value());
  } else {
    for_all_selected_widgets() o->tooltip(i->value());
  }
  Fl_Color tc = FL_BLACK;
  if (current_widget->tooltip() && *(current_widget->tooltip())) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

void x_cb(Fl_Value_Input* i, void *v) {
  int x;
  if (v != LOAD) {
    x = int(i->value());
    if (x <= -1) x = -1;
    for_all_selected_widgets() {
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->position(x, q->o->y());
      q->redraw();      
    }
  } else {
	  x = current_widget ? current_widget->o->x() : 0;
  }
  i->value(x);
}

void y_cb(Fl_Value_Input* i, void *v) {
  int y;
  if (v != LOAD) {
    y = int(i->value());
    if (y <= -1) y = -1;
    for_all_selected_widgets() {
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->position(q->o->x(), y);
      q->redraw();
    }
  } else {
	  y = current_widget ? current_widget->o->y() : 0;    
  }
  i->value (y);
}

void width_cb(Fl_Value_Input* i, void *v) {
  int width;
  if (v != LOAD) {
    width = int(i->value());
    if (width <= 0) width = 0;
    for_all_selected_widgets() {
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->size(width, q->o->h());
      q->redraw();
    }
  } else {
	  width = current_widget ? current_widget->o->w() : 0;
  }
  i->value (width);
}

void height_cb(Fl_Value_Input* i, void *v) {
  int height;
  if (v != LOAD) {
    height = int(i->value());
    if (height <= 0) height = 0;
    for_all_selected_widgets() {
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->size(q->o->w(), height);
      q->redraw();
    }  
  } else {
	  height = current_widget ? current_widget->o->h() : 0;  
  }
  i->value (height);
}

void set_xy_cb(Fl_Light_Button* i, void *v) {
  if (v == LOAD) {
    if (current_widget->is_window())
      i->show();
    else i->hide();
    i->value(!current_widget->set_xy);
  } else {
    modflag = 1;
    current_widget->set_xy = !i->value();  
  }
  if (current_widget->set_xy) {
    widget_x->deactivate ();
    widget_y->deactivate ();  
  } else {
    widget_x->activate ();
    widget_y->activate ();  
  }
  if (i->value()) i->label_color(FL_RED);
  else i->label_color(FL_BLACK);  
  i->redraw();
}

////////////////////////////////////////////////////////////////

// turn number to string or string to number for saving to file:
// does not work for hierarchial menus!

const char *item_name(Fl_Menu_Item* m, int i) {
  if (m) {
    while (m->label()) {
      if (m->argument() == i) return m->label();
      m++;
    }
  }
  static char buffer[20];
  sprintf(buffer, "%d", i);
  return buffer;
}
void* item_pointer(Fl_Menu_Item* m, const char* i) {
  if (m && i) {
    if (i[0]=='F' && i[1]=='L' && i[2]=='_') i += 3;
    while (m->label()) {
      if (!strcmp(m->label(), i)) return m->user_data();
      m++;
    }
  }
  return 0;
}
int item_number(Fl_Menu_Item* m, const char* i) {
  void* v = item_pointer(m,i);
  if (v) return (long)v;
  return atoi(i);
}

Fl_Menu_Item boxmenu[] = {
{"NO_BOX",		0,0, (void *)FL_NO_BOX},
{"UP_BOX",		0,0, (void *)FL_UP_BOX},
{"DOWN_BOX",		0,0, (void *)FL_DOWN_BOX},
{"THIN_UP_BOX",		0,0, (void *)FL_THIN_UP_BOX},
{"THIN_DOWN_BOX",	0,0, (void *)FL_THIN_DOWN_BOX},
{"FLAT_BOX",		0,0, (void *)FL_FLAT_BOX},
{"BORDER_BOX",		0,0, (void *)FL_BORDER_BOX},
{"BORDER_FRAME",	0,0, (void *)FL_BORDER_FRAME},
{"ENGRAVED_BOX",	0,0, (void *)FL_ENGRAVED_BOX},
{"EMBOSSED_BOX",	0,0, (void *)FL_EMBOSSED_BOX},
{"HIGHLIGHT_UP_BOX",	0,0, (void *)FL_HIGHLIGHT_UP_BOX},
{"HIGHLIGHT_DOWN_BOX",	0,0, (void *)FL_HIGHLIGHT_DOWN_BOX},
{"ROUND_UP_BOX",	0,0, (void *)FL_ROUND_UP_BOX},
{"ROUND_DOWN_BOX",	0,0, (void *)FL_ROUND_DOWN_BOX},
{"DIAMOND_UP_BOX",	0,0, (void *)FL_DIAMOND_UP_BOX},
{"DIAMOND_DOWN_BOX",	0,0, (void *)FL_DIAMOND_DOWN_BOX},
{"SHADOW_BOX",		0,0, (void *)FL_SHADOW_BOX},
{"ROUNDED_BOX",		0,0, (void *)FL_ROUNDED_BOX},
{"RSHADOW_BOX",		0,0, (void *)FL_RSHADOW_BOX},
{"RFLAT_BOX",		0,0, (void *)FL_RFLAT_BOX},
{"OVAL_BOX",		0,0, (void *)FL_OVAL_BOX},
{"OSHADOW_BOX",		0,0, (void *)FL_OSHADOW_BOX},
{"OFLAT_BOX",		0,0, (void *)FL_OFLAT_BOX},
{0}};

const char *boxname(Fl_Boxtype i) {
  for (int j = 0; j < int(sizeof(boxmenu)/sizeof(*boxmenu)); j++)
    if (boxmenu[j].user_data() == (void*)i) return boxmenu[j].label();
  return 0;
}

Fl_Boxtype boxnumber(const char *i) {
  if (i[0]=='F' && i[1]=='L' && i[2]=='_') i += 3;
  for (int j = 0; j < int(sizeof(boxmenu)/sizeof(*boxmenu)); j++)
    if (boxmenu[j].label() && !strcmp(boxmenu[j].label(), i)) {
      return Fl_Boxtype(boxmenu[j].user_data());
    }
  return 0;
}

#define NOT_DEFAULT(W, what) (W->o->what() != ((Fl_Widget_Type*)(W->factory))->o->what())

void box_cb(Fl_Choice* i, void *v) {
  if (v == LOAD) {
    i->show();
    Fl_Boxtype n = current_widget->o->box();
    for (int j = 0; j < int(sizeof(boxmenu)/sizeof(*boxmenu)); j++)
      if ((void*)n == boxmenu[j].user_data()) {i->value(j); break;}
  } else {
    int m = i->value();
    Fl_Boxtype n = Fl_Boxtype(boxmenu[m].user_data());
    if (!n) return; // should not happen
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->box(n);
      q->redraw();
    }
  }
  Fl_Color tc = FL_BLACK;
  if (NOT_DEFAULT(current_widget, box)) tc = FL_RED;
  if (i->label_color() != tc) {
    i->label_color(tc);
    i->damage_label();
  }
}

void text_box_cb(Fl_Choice* i, void *v) {
  if (v == LOAD) {
    i->show();
    Fl_Boxtype n = current_widget->o->text_box();
    for (int j = 0; j < int(sizeof(boxmenu)/sizeof(*boxmenu)); j++)
      if ((void*)n == boxmenu[j].user_data()) {i->value(j); break;}
  } else {
    int m = i->value();
    Fl_Boxtype n = Fl_Boxtype(boxmenu[m].user_data());
    if (!n) return; // should not happen
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->text_box(n);
      q->redraw();
    }
  }
  Fl_Color tc = FL_BLACK;
  if (NOT_DEFAULT(current_widget, text_box)) tc = FL_RED;
  if (i->label_color() != tc) {
    i->label_color(tc);
    i->damage_label();
  }
}

////////////////////////////////////////////////////////////////

#define ZERO_ENTRY 1000

Fl_Menu_Item whenmenu[] = {
  {"never",0,0,(void*)ZERO_ENTRY},
  {"Release",0,0,(void*)FL_WHEN_RELEASE},
  {"Changed",0,0,(void*)FL_WHEN_CHANGED},
  {"Enter key",0,0,(void*)FL_WHEN_ENTER_KEY},
  //{"Release or Enter",0,0,(void*)(FL_WHEN_ENTER_KEY|FL_WHEN_RELEASE)},
  {0}};

static Fl_Menu_Item whensymbolmenu[] = {
  {"FL_WHEN_NEVER",0,0,(void*)(FL_WHEN_NEVER)},
  {"FL_WHEN_CHANGED",0,0,(void*)(FL_WHEN_CHANGED)},
  {"FL_WHEN_RELEASE",0,0,(void*)(FL_WHEN_RELEASE)},
  {"FL_WHEN_RELEASE_ALWAYS",0,0,(void*)(FL_WHEN_RELEASE_ALWAYS)},
  {"FL_WHEN_ENTER_KEY",0,0,(void*)(FL_WHEN_ENTER_KEY)},
  {"FL_WHEN_ENTER_KEY_ALWAYS",0,0,(void*)(FL_WHEN_ENTER_KEY_ALWAYS)},
  {0}};

void when_cb(Fl_Choice* i, void *v) {
  if (v == LOAD) {
    if (current_widget->is_menu_item()) {i->hide(); return;}
    i->show();
    int n = current_widget->o->when() & (~FL_WHEN_NOT_CHANGED);
    if (!n) n = ZERO_ENTRY;
    for (int j = 0; j < int(sizeof(whenmenu)/sizeof(*whenmenu)); j++)
      if (whenmenu[j].argument() == n) {i->value(j); break;}
  } else {
    int m = i->value();
    int n = int(whenmenu[m].argument());
    if (!n) return; // should not happen
    if (n == ZERO_ENTRY) n = 0;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->when(n|(q->o->when()&FL_WHEN_NOT_CHANGED));
    }
  }
  i->redraw();
}

void when_button_cb(Fl_Light_Button* i, void *v) {
  if (v == LOAD) {
    if (current_widget->is_menu_item()) {i->hide(); return;}
    i->show();
    i->value(current_widget->o->when()&FL_WHEN_NOT_CHANGED);
  } else {
    int n = i->value() ? FL_WHEN_NOT_CHANGED : 0;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->when(n|(q->o->when()&~FL_WHEN_NOT_CHANGED));
    }
  }
  if (i->value()) i->label_color(FL_RED);
  else i->label_color(FL_BLACK);
  i->redraw();
}

uchar Fl_Widget_Type::resizable() const {
  if (is_window()) return ((Fl_Window*)o)->resizable() != 0;
  Fl_Group* p = o->parent();
  if (p) return p->resizable() == o;
  else return 0;
}

void Fl_Widget_Type::resizable(uchar v) {
  if (v) {
    if (resizable()) return;
    if (is_window()) {
      ((Fl_Window*)o)->resizable(o);
      ((Fl_Window*)o)->init_sizes();
    } else {
      Fl_Group* p = (Fl_Group*)o->parent();
      if (p) {p->resizable(o); p->init_sizes();}
    }
  } else {
    if (!resizable()) return;
    if (is_window()) {
      ((Fl_Window*)o)->resizable(0);
    } else {
      Fl_Group* p = (Fl_Group*)o->parent();
      if (p) p->resizable(0);
    }
  }
}

void resizable_cb(Fl_Light_Button* i,void* v) {
  if (v == LOAD) {
    if (current_widget->is_menu_item()) {i->hide(); return;}
    if (numselected > 1) {i->hide(); return;}
    i->show();
    i->value(current_widget->resizable());
  } else {
    current_widget->resizable(i->value());
    modflag = 1;
  }
  if (current_widget->resizable()) i->label_color(FL_RED);
  else i->label_color(FL_BLACK);
  i->redraw();
}

void hotspot_cb(Fl_Light_Button* i,void* v) {
  if (v == LOAD) {
    if (numselected > 1 || current_widget->is_menu_item()) {i->hide(); return;}
    i->show();
    i->value(current_widget->hotspot());
  } else {
    modflag = 1;
    current_widget->hotspot(i->value());
    if (i->value()) {
      // turn off hotspot in all siblings
      for (Fl_Type *p = current_widget->parent; p; p = p->parent) {
	if (!p->is_widget()) continue;
	for (Fl_Type* o = p->first_child; o; o = o->walk())
	  if (o != current_widget && o->is_widget())
	    ((Fl_Widget_Type*)o)->hotspot(0);
      }
    }
  }
  if (current_widget->hotspot()) i->label_color(FL_RED);
  else i->label_color(FL_BLACK);  
  i->redraw();
}

void visible_cb(Fl_Light_Button* i, void* v) {
  if (v == LOAD) {
    i->value(current_widget->o->visible());
  } else {
    int n = i->value();
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      n ? q->o->show() : q->o->hide();
      q->redraw();
    }
  }
  if (!i->value()) i->label_color(FL_RED);
  else i->label_color(FL_BLACK);
  i->redraw();
}

void active_cb(Fl_Light_Button* i, void* v) {
  if (v == LOAD) {
    i->value(current_widget->o->active());
  } else {
    int n = i->value();
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      n ? q->o->activate() : q->o->deactivate();
      q->redraw();
    }
  }
  if (!i->value()) i->label_color(FL_RED);
  else i->label_color(FL_BLACK);  
  i->redraw();
}

////////////////////////////////////////////////////////////////

Fl_Menu_Item fontmenu[] = {
{"Helvetica"},
{"Helvetica bold"},
{"Helvetica italic"},
{"Helvetica bold italic"},
{"Courier"},
{"Courier bold"},
{"Courier italic"},
{"Courier bold italic"},
{"Times"},
{"Times bold"},
{"Times italic"},
{"Times bold italic"},
{"Symbol"},
{"Terminal"},
{"Terminal Bold"},
{"Zapf Dingbats"},
{0}};

void labelfont_cb(Fl_Choice* i, void *v) {
  if (v == LOAD) {
    int n = current_widget->o->label_font() - fl_fonts;
    if (n > 15 || n < 0) n = 0;
    i->value(n);
  } else {
    int n = i->value();
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->label_font(fl_fonts + n);
      q->redraw();
    }
  }
  Fl_Color tc = FL_BLACK;
  if (NOT_DEFAULT(current_widget, label_font)) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

void labelsize_cb(Fl_Value_Input* i, void *v) {
  int n;
  if (v == LOAD) {
    n = current_widget->o->label_size();
  } else {
    n = int(i->value());
    if (n <= 0) n = 1;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->label_size(n);
      q->redraw();
    }
  }
  i->value(n);
  Fl_Color tc = FL_BLACK;
  if (NOT_DEFAULT(current_widget, label_size)) tc = FL_RED;
  if (i->text_color() != tc)
    { i->text_color(tc); i->redraw();}
}

void image_cb(Fl_Button *a, void *v) {
  if (v != LOAD) {
    Fluid_Image *i = ui_find_image(current_widget->image);
    if (i == current_widget->image) return; // user hit "Cancel"
    for_all_selected_widgets() {
      Fl_Widget_Type* p = (Fl_Widget_Type*)o;
      p->setimage(i);
    }
    image_inlined_cb(include_image_button, LOAD); // update the button state
  }
  const char* s = current_widget->image ? current_widget->image->name() : 0;
  if (s != a->label()) {a->label(s); a->redraw();}
  Fl_Color tc = FL_BLACK;
  if (current_widget->image) tc = FL_RED;
  if (image_label->label_color() != tc)
    { image_label->label_color(tc); image_label->redraw();}
}

Fl_Menu_Item labeltypemenu[] = {
  {"NORMAL_LABEL",0,0,(void*)FL_NORMAL_LABEL},
  {"SYMBOL_LABEL",0,0,(void*)FL_SYMBOL_LABEL},
  {"SHADOW_LABEL",0,0,(void*)FL_SHADOW_LABEL},
  {"ENGRAVED_LABEL",0,0,(void*)FL_ENGRAVED_LABEL},
  {"EMBOSSED_LABEL",0,0,(void*)FL_EMBOSSED_LABEL},
  {"NO_LABEL",0,0,(void*)(FL_NO_LABEL)},
{0}};

const char *labeltypename(Fl_Labeltype i) {
  for (int j = 0; j < int(sizeof(labeltypemenu)/sizeof(*labeltypemenu)); j++)
    if (labeltypemenu[j].user_data() == (void*)i) return labeltypemenu[j].label();
  return 0;
}

void labeltype_cb(Fl_Choice* i, void *v) {
  if (v == LOAD) {
    Fl_Labeltype n = current_widget->o->label_type();
    i->when(FL_WHEN_RELEASE);
    for (int j = 0; j < int(sizeof(labeltypemenu)/sizeof(*labeltypemenu)); j++)
      if (labeltypemenu[j].user_data() == (void*)n) {i->value(j); break;}
  } else {
    int m = i->value();
    Fl_Labeltype n = Fl_Labeltype(labeltypemenu[m].user_data());
    if (!n) return; // should not happen
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* p = (Fl_Widget_Type*)o;
      p->o->label_type(n);
      p->redraw();
    }
  }
  Fl_Color tc = FL_BLACK;
  if (NOT_DEFAULT(current_widget, label_type)) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

////////////////////////////////////////////////////////////////

void color_cb(Fl_Light_Button* i, void *v) {
  Fl_Color c = current_widget->o->color();
  if (v == LOAD) {
//     if (current_widget->is_slider()) i->label("Back Color");
//     else if (current_widget->is_counter()) i->label("Button Color");
//     else i->label("Color");
    i->show();
  } else {
    if (!fl_color_chooser(i->label(), c)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->color(c); q->redraw();
    }
  }
  i->selection_color(c);
  i->text_background(c);
  i->label_color(NOT_DEFAULT(current_widget, color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void color2_cb(Fl_Light_Button* i, void *v) {
  Fl_Color c = current_widget->o->selection_color();
  if (v == LOAD) {
    if (current_widget->is_light_button() ||
	current_widget->is_menu_item()) i->label("On Color");
    else if (current_widget->is_button()) i->label("Down Color");
    else i->label("Selection Color");
    i->show();
  } else {
    if (!fl_color_chooser(i->label(), c)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->selection_color(c); q->redraw();
    }
  }
  i->selection_color(c);
  i->text_background(c);
  i->label_color(NOT_DEFAULT(current_widget, selection_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void color3_cb(Fl_Light_Button* i, void *v) {
  Fl_Color c = current_widget->o->text_background();
  if (v == LOAD) {
//     if (current_widget->is_slider()) i->label("Button Color");
//     else i->label("Color 3");
    i->show();
  } else {
    if (!fl_color_chooser(i->label(), c)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->text_background(c); q->redraw();
    }
  }
  i->selection_color(c);
  i->text_background(c);
  i->label_color(NOT_DEFAULT(current_widget,text_background) ? FL_RED : FL_BLACK);
  i->redraw();
}

void labelcolor_cb(Fl_Light_Button* i, void *v) {
  Fl_Color c = current_widget->o->label_color();
  if (v != LOAD) {
    if (!fl_color_chooser(i->label(), c)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->label_color(c); q->redraw();
    }
  } else {
    if (current_widget->is_slider()) i->label("Label / Glyph Color");
    else i->label("Label Color");
    i->show();
  }
  i->selection_color(c);
  i->text_background(c);
  i->label_color(NOT_DEFAULT(current_widget, label_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void textfont_cb(Fl_Choice* i, void* v) {
  Fl_Font n; Fl_Color tc;
  if (v == LOAD) {
    if (current_widget->is_menu_item()) i->label("Shortcut Font:");
    else i->label("Text Font:");
    i->show();
    int m = current_widget->o->text_font()-fl_fonts;
    if (m < 0 || m > 15) m = 0;
    i->value(m);
  } else {
    n = i->value()+fl_fonts;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->text_font(n);
      q->redraw();
    }
  }
  tc = FL_BLACK;
  if (NOT_DEFAULT(current_widget, text_font)) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

void textsize_cb(Fl_Value_Input* i, void* v) {
  int s; Fl_Color tc;
  if (v == LOAD) {
    s = current_widget->o->text_size();
    i->show();
  } else {
    s = int(i->value());
    if (s <= 0) s = 1;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->text_size(s);
      q->redraw();
    }
  }
  i->value(s);
  tc = FL_BLACK;
  if (NOT_DEFAULT(current_widget, text_size)) tc = FL_RED;
  if (i->text_color() != tc)
    { i->text_color(tc); i->redraw(); }
}

void textcolor_cb(Fl_Light_Button* i, void* v) {
  Fl_Color tc;
  if (v == LOAD) {
    if (current_widget->is_menu_item()) i->label("Shortcut Color");
    else i->label("Text Color");
    tc = current_widget->o->text_color();
    i->show();
  } else {
    tc = i->selection_color();
    if (!fl_color_chooser(i->label(), tc)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->text_color(tc);
      q->redraw();
    }
  }
  i->selection_color(tc);
  i->text_background(tc);
  i->label_color(NOT_DEFAULT(current_widget, text_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void selected_textcolor_cb(Fl_Light_Button* i, void* v) {
  Fl_Color tc;
  if (v == LOAD) {
    if (current_widget->is_slider()) i->label("Selected Glyph Color");
    else i->label("Selected Text Color");
    tc = current_widget->o->selection_text_color();
    i->show();
  } else {
    tc = i->selection_color();
    if (!fl_color_chooser(i->label(), tc)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->selection_text_color(tc);
      q->redraw();
    }
  }
  i->selection_color(tc);
  i->text_background(tc);
  i->label_color(NOT_DEFAULT(current_widget, selection_text_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void highlightcolor_cb(Fl_Light_Button* i, void *v) {
  Fl_Color c = FL_BLACK;
  if (v == LOAD) {
    c = current_widget->o->highlight_color();
    i->show();
  } else {
    c = i->selection_color();
    if (!fl_color_chooser(i->label(), c)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->highlight_color(c); q->redraw();
    }
  }
  i->selection_color(c);
  i->text_background(c);
  i->label_color(NOT_DEFAULT(current_widget, highlight_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void highlight_label_color_cb(Fl_Light_Button* i, void *v) {
  Fl_Color c = FL_BLACK;
  if (v == LOAD) {
    if (current_widget->is_slider()) i->label("Highlight Glyph Color");
    else i->label("Highlight Label Color");
    c = current_widget->o->highlight_label_color();
    i->show();
  } else {
    c = i->selection_color();
    if (!fl_color_chooser(i->label(), c)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->highlight_label_color(c); q->redraw();
    }
  }
  i->selection_color(c);
  i->text_background(c);
  i->label_color(NOT_DEFAULT(current_widget, highlight_label_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

static Fl_Menu_Item alignmenu[] = {
  {"FL_ALIGN_CENTER",0,0,(void*)(FL_ALIGN_CENTER)},
  {"FL_ALIGN_TOP",0,0,(void*)(FL_ALIGN_TOP)},
  {"FL_ALIGN_BOTTOM",0,0,(void*)(FL_ALIGN_BOTTOM)},
  {"FL_ALIGN_LEFT",0,0,(void*)(FL_ALIGN_LEFT)},
  {"FL_ALIGN_RIGHT",0,0,(void*)(FL_ALIGN_RIGHT)},
  {"FL_ALIGN_INSIDE",0,0,(void*)(FL_ALIGN_INSIDE)},
  {"FL_ALIGN_CLIP",0,0,(void*)(FL_ALIGN_CLIP)},
  {"FL_ALIGN_WRAP",0,0,(void*)(FL_ALIGN_WRAP)},
  {"FL_ALIGN_TOP | FL_ALIGN_LEFT",0,0,(void*)(FL_ALIGN_TOP | FL_ALIGN_LEFT)},
  {"FL_ALIGN_TOP | FL_ALIGN_RIGHT",0,0,(void*)(FL_ALIGN_TOP | FL_ALIGN_RIGHT)},
  {"FL_ALIGN_BOTTOM | FL_ALIGN_LEFT",0,0,(void*)(FL_ALIGN_BOTTOM | FL_ALIGN_LEFT)},
  {"FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT",0,0,(void*)(FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT)},
  {"FL_ALIGN_LEFT | FL_ALIGN_TOP",0,0,(void*)(FL_ALIGN_LEFT | FL_ALIGN_TOP)},
  {"FL_ALIGN_RIGHT | FL_ALIGN_TOP",0,0,(void*)(FL_ALIGN_RIGHT | FL_ALIGN_TOP)},
  {"FL_ALIGN_LEFT | FL_ALIGN_BOTTOM",0,0,(void*)(FL_ALIGN_LEFT | FL_ALIGN_BOTTOM)},
  {"FL_ALIGN_RIGHT | FL_ALIGN_BOTTOM",0,0,(void*)(FL_ALIGN_RIGHT | FL_ALIGN_BOTTOM)},

  {"FL_ALIGN_TILED",0,0,(void*)(FL_ALIGN_TILED)},
{0}};

void align_cb(Fl_Button* i, void *v) {
  Fl_Flags b = Fl_Flags(i->argument());
  if (v == LOAD) {
    i->value(current_widget->o->flags() & b);
    Fl_Flags tplate = ((Fl_Widget_Type*)(current_widget->factory))->o->flags();
    Fl_Color c = FL_BLACK;
    Fl_Color d = FL_RED;
    if (tplate & b) {c = FL_RED; d = FL_BLACK;}
    if (i->label_color() != c) {i->label_color(c); i->redraw();}
    i->selection_text_color(d);
    i->selection_color(i->color());
  } else {
    for_all_selected_widgets() {
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      int x = q->o->flags() & FL_ALIGN_MASK;
      int y;
      if (i->value()) {
	y = x | b;
      } else {
	y = x & ~b;
      }
      if (x != y) {q->o->align(y); q->redraw(); modflag = 1;}
    }
  }
}

void image_inlined_cb(Fl_Light_Button* i, void *v) {
  if (v==LOAD) {
    if(current_widget->image) {
      i->value(current_widget->image->inlined);
      i->activate();
    } else {
      i->value(0);
      i->deactivate();
    }
  } else {
    for_all_selected_widgets() {
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      if(q->image) q->image->inlined = i->value();
    }
  }
}

////////////////////////////////////////////////////////////////

void callback_cb(Fl_Input* i, void *v) {
  if (v == LOAD) {
    i->static_value(current_widget->callback());
  } else {
    const char *c = i->value();
    const char *d = c_check(c);
    if (d) {fl_message("Error in callback: %s",d); haderror = 1; return;}
    for_all_selected_widgets() o->callback(c);
  }
  Fl_Color tc = FL_BLACK;
  if (current_widget->callback()) tc = FL_RED;
  if (callback_label->label_color() != tc) {
    callback_label->label_color(tc);
    callback_label->damage_label();
  }
}

void user_data_cb(Fl_Input *i, void *v) {
  if (v == LOAD) {
    i->static_value(current_widget->user_data());
  } else {
    const char *c = i->value();
    const char *d = c_check(c);
    if (d) {fl_message("Error in user_data: %s",d); haderror = 1; return;}
    for_all_selected_widgets() o->user_data(c);
  }
  Fl_Color tc = FL_BLACK;
  if (current_widget->user_data()) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

void user_data_type_cb(Fl_Input *i, void *v) {
  static const char *dflt = "void*";
  if (v == LOAD) {
    const char *c = current_widget->user_data_type();
    if (!c) c = dflt;
    i->static_value(c);
  } else {
    const char *c = i->value();
    const char *d = c_check(c);
    if (!*c) i->value(dflt);
    else if (!strcmp(c,dflt)) c = 0;
    if (!d) {
      if (c && *c && c[strlen(c)-1] != '*' && strcmp(c,"long"))
	d = "must be pointer or long";
    }
    if (d) {fl_message("Error in type: %s",d); haderror = 1; return;}
    for_all_selected_widgets() o->user_data_type(c);
  }
  Fl_Color tc = FL_BLACK;
  if (strcmp(i->value(), "void*")) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

// "v_attributes" let user type in random code for attribute settings:

void v_input_cb(Fl_Input* i, void* v) {
  int n = int(long(i->user_data()));
  if (v == LOAD) {
    i->static_value(current_widget->extra_code(n));
  } else {
    const char *c = i->value();
    const char *d = c_check(c&&c[0]=='#' ? c+1 : c);
    if (d) {fl_message("Error in %s: %s",i->label(),d); haderror = 1; return;}
    for_all_selected_widgets() {
      Fl_Widget_Type *t = (Fl_Widget_Type*)o;
      t->extra_code(n,c);
    }
  }
  Fl_Color tc = FL_BLACK;
  if (i->value() && *i->value()) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

void subclass_cb(Fl_Input* i, void* v) {
  if (v == LOAD) {
    i->static_value(current_widget->subclass());
  } else {
    const char *c = i->value();
    for_all_selected_widgets() {
      Fl_Widget_Type *t = (Fl_Widget_Type*)o;
      t->subclass(c);
    }
  }
  Fl_Color tc = FL_BLACK;
  if (current_widget->subclass() && *(current_widget->subclass())) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

////////////////////////////////////////////////////////////////
// Kludges to the panel for subclasses:

void slider_size_cb(Fl_Value_Input* i, void* v) {
  if (v == LOAD) {
    if (current_widget->is_valuator()!=2) {i->hide(); return;}
    i->show();
    i->value(((Fl_Slider*)(current_widget->o))->slider_size());
  } else {
    int n = int(i->value());
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      if (q->is_valuator()==2) {
	((Fl_Slider*)(q->o))->slider_size(n);
	q->redraw();
      }
    }
  }
}

void min_cb(Fl_Value_Input* i, void* v) {
  if (v == LOAD) {
    if (!current_widget->is_valuator()) {i->hide(); return;}
    i->show();
    i->value(((Fl_Valuator*)(current_widget->o))->minimum());
  } else {
    double n = i->value();
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      if (q->is_valuator()) {
	((Fl_Valuator*)(q->o))->minimum(n);
	q->redraw();
      }
    }
  }
  Fl_Color tc = FL_BLACK;
  if (i->value() != 0.0) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

void max_cb(Fl_Value_Input* i, void* v) {
  if (v == LOAD) {
    if (!current_widget->is_valuator()) {i->hide(); return;}
    i->show();
    i->value(((Fl_Valuator*)(current_widget->o))->maximum());
  } else {
    double n = i->value();
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      if (q->is_valuator()) {
	((Fl_Valuator*)(q->o))->maximum(n);
	q->redraw();
      }
    }
  }
  Fl_Color tc = FL_BLACK;
  if (i->value() != 1.0) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

void step_cb(Fl_Value_Input* i, void* v) {
  if (v == LOAD) {
    if (!current_widget->is_valuator()) {i->hide(); return;}
    i->show();
    i->value(((Fl_Valuator*)(current_widget->o))->step());
  } else {
    double n = i->value();
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      if (q->is_valuator()) {
	((Fl_Valuator*)(q->o))->step(n);
	q->redraw();
      }
    }
  }
  Fl_Color tc = FL_BLACK;
  if (i->value() != 0.0) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

void value_cb(Fl_Value_Input* i, void* v) {
  if (v == LOAD) {
    if (current_widget->is_valuator()) {
      i->show();
      i->value(((Fl_Valuator*)(current_widget->o))->value());
    } else if (current_widget->is_button()) {
      i->show();
      i->value(((Fl_Button*)(current_widget->o))->value());
    } else 
      i->hide();
  } else {
    double n = i->value();
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      if (q->is_valuator()) {
	((Fl_Valuator*)(q->o))->value(n);
      } else if (q->is_button()) {
	((Fl_Button*)(q->o))->value(n != 0);
	if (q->is_menu_item()) q->redraw();
      }
    }
  }
  Fl_Color tc = FL_BLACK;
  if (i->value() != 0.0) tc = FL_RED;
  if (i->label_color() != tc)
    { i->label_color(tc); i->damage_label(); }
}

////////////////////////////////////////////////////////////////

// subtypes:

Fl_Menu_Item *Fl_Widget_Type::subtypes() {return 0;}

void subtype_cb(Fl_Choice* i, void* v) {
  if (v == LOAD) {
    Fl_Menu_Item* m = current_widget->subtypes();
    if (!m) {i->hide(); return;}
    i->menu(m);
    int j;
    for (j = 0;; j++) {
      if (!m[j].text) {j = 0; break;}
      if (m[j].argument() == current_widget->o->type()) break;
    }
    i->value(j);
    i->show();
    i->redraw();
  } else {
    int n = int(i->mvalue()->argument());
    Fl_Menu_Item* m = current_widget->subtypes();
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      if (q->subtypes()==m) {
	q->o->type(n);
	q->redraw();
      }
    }
  }
}

////////////////////////////////////////////////////////////////

// Groups in the panel send LOAD to all widgets, send any other
// callback to only the changed widgets, and recursively call
// themselves:

void propagate_group(Fl_Group* g, void* v) {
  if (v == LOAD) {
    for (int i=g->children(); i--;) {
      Fl_Widget* o = g->child(i);
      o->do_callback(o,LOAD);
    }
  } else {
    for (int i=g->children(); i--;) {
      Fl_Widget* o = g->child(i);
      if (o->changed() || o->callback()==(Fl_Callback*)propagate_group ||
          o->callback()==(Fl_Callback*)propagate_tabs)
      {
	o->do_callback();
	if (haderror) return;
	o->clear_changed();
      }
    }
  }    
}

// Fluid insists on giving the correct type to arguments so we need
// a different call for the tabs:
void propagate_tabs(Fl_Tabs* g, void* v) {
  propagate_group(g, v);
}

inline Fluid_Plugin** next_panel(Fluid_Plugin** pp, Fluid_Plugin* &p)
{
  while(pp < plugins+nbplugins && !(*pp)->make_panel) pp++;
  if(pp < plugins+nbplugins) p = *pp;
  return pp;
}

void set_cb(Fl_Button*, void*) {
  haderror = 0;
  propagate_group(the_panel, 0);
  Fluid_Plugin *p, **pp;
  for(pp = next_panel(plugins, p); pp-plugins<nbplugins; pp = next_panel(pp+1, p))
    if(p->panel_is_orphan) propagate_group(p->panel, 0);
}

void ok_cb(Fl_Return_Button* o, void* v) {
  set_cb(o,v);
  if (!haderror)
    the_panel->hide();
}

static void load_panel();

void default_cb(Fl_Button*, void* v) {
  if (v == LOAD) return;
  const Fl_Style* default_style =
    ((Fl_Widget_Type*)(current_widget->factory))->o->style();
  Fl_Style* s1 = (Fl_Style*)(current_widget->o->style());
  if (s1 == default_style) return;
  // Delete the style:
  delete s1;
  // this just assigns the pointer:
  current_widget->o->style((Fl_Style*)default_style);
  load_panel();
  current_widget->redraw();
}

void revert_cb(Fl_Button*, void*) {
  if (!current_widget) return;
  // We have to revert all dynamically changing fields:
  // but for now only the first label works...
  if (numselected == 1) current_widget->label(oldlabel);
  propagate_group(the_panel, LOAD);
  Fluid_Plugin *p, **pp;
  for(pp = next_panel(plugins, p); pp-plugins<nbplugins; pp = next_panel(pp+1, p))
    if(p->panel_is_orphan) propagate_group(p->panel, LOAD);
}

void cancel_cb(Fl_Button* o, void* v) {
  revert_cb(o, v);
  the_panel->hide();
}

void toggle_overlays(Fl_Widget *,void *); // in Fl_Window_Type.C
void overlay_cb(Fl_Light_Button*o,void *v) {
  toggle_overlays(o,v);
}

// update the panel according to current widget set:
static void load_panel() {
  if (!the_panel) return;

  // find all the Fl_Widget subclasses currently selected:
  numselected = 0;
  current_widget = 0;
  if (Fl_Type::current) {
    if (Fl_Type::current->is_widget())
      current_widget=(Fl_Widget_Type*)Fl_Type::current;
    for_all_selected_widgets() {
      numselected++;
      if (!current_widget) current_widget = (Fl_Widget_Type*)o;
    }
  }
  if (numselected) {
    Fluid_Plugin *p, **pp;
    for(pp = next_panel(plugins, p); pp-plugins<nbplugins; pp = next_panel(pp+1, p))
      p->please_show_panel = 0;
    propagate_group(the_panel, LOAD);
    for(pp = next_panel(plugins, p); pp-plugins<nbplugins; pp = next_panel(pp+1, p))
    {
      if(p->panel_is_orphan) {
	propagate_group(p->panel, LOAD);
	if(p->please_show_panel) {
	  p->panel_is_orphan = 0;
	  panel_tabs->add(*p->panel);
	  if(p->was_visible) panel_tabs->value(p->panel);
	  the_panel->redraw();
	  p->panel->redraw();
	}
      } else {
	if(!p->please_show_panel) {
	  if(panel_tabs->value() == p->panel) {
	    panel_tabs->value(panel_tabs->child(0));
	    p->was_visible = 1;
	  } else
	    p->was_visible = 0;
	  p->panel_is_orphan = 1;
	  panel_tabs->remove(*p->panel);
	  the_panel->redraw();
	}
      }
    }	
  }
}

// This is called when user double-clicks an item, open or update the panel:
void Fl_Widget_Type::open() {
  if (!the_panel) {
    the_panel = make_widget_panel();
    Fluid_Plugin *p, **pp;
    for(pp = next_panel(plugins, p); pp-plugins<nbplugins; pp = next_panel(pp+1, p))
    {
      p->make_panel();
      // All plugin panels are initially not mapped in the main pannel
      p->panel_is_orphan = 1; 
      p->panel->position(panel_tabs->child(0)->x(), panel_tabs->child(0)->y());
      p->panel->layout();
      p->panel->resizable(0);
      p->panel->size(panel_tabs->child(0)->w(), panel_tabs->child(0)->h());
      p->panel->label(p->name);
    }
  }

  load_panel();
  if (numselected) {
    the_panel->show();
  }
}

extern void redraw_overlays();

// Called when the select value on the passed object changes, or when
// objects are deleted (in which case the passed object is null).

void selection_changed(Fl_Type *p) {
  if (p && the_panel && the_panel->visible()) {
    // store all changes to the current selected objects:
    set_cb(0,0);
    if (haderror) {
      // If there is an error, try to put selected flags and Fl_Type::current
      // back to the previous values:
      Fl_Type *q = 0;
      for (Fl_Type *o = Fl_Type::first; o; o = o->walk()) {
	o->new_selected = o->selected;
	if (!q && o->selected) q = o;
      }
      if (!p || !p->selected) p = q;
      Fl_Type::current = p;
      redraw_browser();
      return;
    }
  }
  // update the selected flags to new set:
  for (Fl_Type *o = Fl_Type::first; o; o = o->walk())
    o->selected = o->new_selected;
  redraw_overlays();
  // load the panel with the new settings:
  load_panel();
}

////////////////////////////////////////////////////////////////
// Writing the C code:

#include <ctype.h>

// test to see if user named a function, or typed in code:
int is_name(const char *c) {
  for (; *c; c++) if (ispunct(*c) && *c!='_' && *c!=':') return 0;
  return 1;
}

// Test to see if name() is an array entry.  If so, and this is the
// highest number, return name[num+1].  Return null if not the highest
// number or a field or function.  Return name() if not an array entry.
const char *array_name(Fl_Widget_Type *o) {
  const char *c = o->name();
  if (!c) return 0;
  const char *d;
  for (d = c; *d != '['; d++) {
    if (!*d) return c;
    if (ispunct(*d) && *d!='_') return 0;
  }
  int num = atoi(d+1);
  int sawthis = 0;
  Fl_Type* parent = o->parent;
  while (parent && !parent->is_class()) parent = parent->parent;
  for (Fl_Type* t = parent?parent->first_child : Fl_Type::first;
       t; t = t->walk(parent)) {
    if (t == o) {sawthis=1; continue;}
    const char *e = t->name();
    if (!e) continue;
    if (strncmp(c,e,d-c)) continue;
    int n1 = atoi(e+(d-c)+1);
    if (n1 > num || n1==num && sawthis) return 0;
  }
  static char buffer[128];
  strncpy(buffer,c,d-c+1);
  sprintf(buffer+(d-c+1),"%d]",num+1);
  return buffer;
}

// Test to see if extra code is a declaration:
int isdeclare(const char *c) {
  while (isspace(*c)) c++;
  if (*c == '#') return 1;
  if (!strncmp(c,"extern",6)) return 1;
  if (!strncmp(c,"typedef",7)) return 1;
  return 0;
}

void Fl_Widget_Type::write_static() {
  const char* t = subclassname(this);
  if (!subclass()) ::write_declare("#include <fltk/%s.h>", t);
  for (int n=0; n < NUM_EXTRA_CODE; n++) {
    if (extra_code(n) && isdeclare(extra_code(n)))
      ::write_declare("%s", extra_code(n));
  }
  if (callback() && is_name(callback()))
    ::write_declare("extern void %s(%s*, %s);", callback(), t,
		    user_data_type() ? user_data_type() : "void*");
  const char* c = array_name(this);
  const char* k = class_name(1);
  if (c && !k) {
    write_c("\n");
    if (!public_) write_c("static ");
    else write_h("extern %s *%s;\n", t, c);
    if (strchr(c, '[') == NULL) write_c("%s *%s=(%s *)0;\n", t, c, t);
    else write_c("%s *%s={(%s *)0};\n", t, c, t);
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
    if (k) {
      write_c("\ninline void %s::%s_i(%s*", k, cn, t);
    } else {
      write_c("\nstatic void %s(%s*", cn, t);
    }
    if (use_o) write_c(" o");
    const char* ut = user_data_type() ? user_data_type() : "void*";
    write_c(", %s", ut);
    if (use_v) write_c(" v");
    write_c(")%s", get_opening_brace(1));
	indentation += 2;
	write_code_block((char *)callback());
	indentation -= 2;
    if (*(d-1) != ';') write_c(";");
    write_c("\n}\n");
    if (k) {
      write_c("void %s::%s(%s* o, %s v)%s", k, cn, t, ut, get_opening_brace(1));
      write_c("%s((%s*)(o->", get_indent_string(1), k);
      for (Fl_Type* p = parent; p->is_widget(); p = p->parent)
	     write_c("parent()->");
      write_c("user_data()))->%s_i(o,v);\n}\n", cn);
    }
  }
  if (image) {
    if (image->written != write_number) {
      image->write_static();
      image->written = write_number;
    }
  }
}

const char *Fl_Type::callback_name() {
  if (is_name(callback())) return callback();
  return unique_id(this, "cb", name(), label());
}

extern int varused_test, varused;
extern Fl_Type* last_group;
  
void Fl_Widget_Type::write_code1() {
  const char* t = subclassname(this);
  const char *c = array_name(this);
  if (c) {
    if (class_name(1)) {
      write_public(public_);
      write_h("%s%s *%s;\n", indent(), t, c);
    }
  }
  if (class_name(1) && callback() && !is_name(callback())) {
    const char* cn = callback_name();
    const char* ut = user_data_type() ? user_data_type() : "void*";
    write_public(0);
    write_h("%sinline void %s_i(%s*, %s);\n", indent(), cn, t, ut);
    write_h("%sstatic void %s(%s*, %s);\n", indent(), cn, t, ut);
  }
  // figure out if local variable will be used (prevent compiler warnings):
  if (is_parent())
    varused = 1;
  else {
    varused_test = 1; varused = 0;
    write_widget_code();
    varused_test = 0;
    for (int n=0; n < NUM_EXTRA_CODE; n++)
      if (extra_code(n) && !isdeclare(extra_code(n))) varused = 1;
  }
  write_c(indent());
  if (varused) write_c("%s%s%s* o = ", get_opening_brace(0), 
					   indent(), t);
  if (name()) write_c("%s = ", name());
  if (is_window()) {
    if (set_xy && strcmp(t,"Fl_Group")!=0)
      write_c("new %s(%d, %d", t, o->w(), o->h());
    else
      write_c("new %s(%d, %d, %d, %d", t, o->x(), o->y(), o->w(), o->h());
  } else if (is_menu_item()) {
    write_c("new %s(", t);
  } else {
    write_c("new %s(%d, %d, %d, %d", t, o->x(), o->y(), o->w(), o->h());
  }
  if (label() && *label()) {
    if (!is_menu_item()) write_c(", ");
    write_cstring(label());
  }
  if (is_menu_button()) write_c("); o->begin();\n");
  else write_c(");\n");
  indentation += 2;
  if (this == last_group) write_c("%sw = o;\n",indent());
  if (varused) write_widget_code();
}

// this is split from write_code1() for Fl_Window_Type:
void Fl_Widget_Type::write_widget_code() {
  Fl_Widget* tplate = ((Fl_Widget_Type*)factory)->o;
  if (o->type() != tplate->type() && !is_window())
    write_c("%so->type(%d);\n", indent(), o->type());

  if (image) image->write_code();

  if (o->box() != tplate->box())
    write_c("%so->box(FL_%s);\n", indent(), boxname(o->box()));
  if (o->text_box() != tplate->text_box())
    write_c("%so->text_box(FL_%s);\n", indent(), boxname(o->text_box()));
//   if (o->glyph() != tplate->glyph())
//     write_c("%so->box(FL_%s);\n", indent(), boxname(o->glyph()));
  if (o->label_font() != tplate->label_font())
    write_c("%so->label_font(fl_fonts+%d);\n", indent(), o->label_font()-fl_fonts);
  if (o->text_font() != tplate->text_font())
    write_c("%so->text_font(fl_fonts+%d);\n", indent(), o->text_font()-fl_fonts);
  if (o->label_type() != tplate->label_type())
    write_c("%so->label_type(FL_%s);\n", indent(),
	    labeltypename(o->label_type()));
  if (o->color() != tplate->color())
    write_c("%so->color((Fl_Color)%i);\n", indent(), o->color());
  if (o->label_color() != tplate->label_color())
    write_c("%so->label_color((Fl_Color)%i);\n", indent(), o->label_color());
  if (o->selection_color() != tplate->selection_color())
    write_c("%so->selection_color((Fl_Color)%i);\n", indent(), o->selection_color());
  if (o->selection_text_color() != tplate->selection_text_color())
    write_c("%so->selection_text_color((Fl_Color)%i);\n", indent(), o->selection_text_color());
  if (o->text_background() != tplate->text_background())
    write_c("%so->text_background((Fl_Color)%i);\n", indent(), o->text_background());
  if (o->highlight_color() != tplate->highlight_color())
    write_c("%so->highlight_color((Fl_Color)%i);\n", indent(), o->highlight_color());
  if (o->highlight_label_color() != tplate->highlight_label_color())
    write_c("%so->highlight_label_color((Fl_Color)%i);\n", indent(), o->highlight_label_color());
  if (o->text_color() != tplate->text_color())
    write_c("%so->text_color((Fl_Color)%i);\n", indent(), o->text_color());
  if (o->label_size() != tplate->label_size())
    write_c("%so->label_size(%d);\n", indent(), o->label_size());
  if (o->text_size() != tplate->text_size())
    write_c("%so->text_size(%d);\n", indent(), o->text_size());

  if (is_button()) {
    Fl_Button* b = (Fl_Button*)o;
    if (b->value()) write_c("%so->value(1);\n", indent());
    if (b->shortcut())
      write_c("%so->shortcut(0x%x);\n", indent(), b->shortcut());
  }

  if (is_valuator()) {
    Fl_Valuator* v = (Fl_Valuator*)o;
    Fl_Valuator* f = (Fl_Valuator*)(tplate);
    if (v->minimum()!=f->minimum())
      write_c("%so->minimum(%g);\n", indent(), v->minimum());
    if (v->maximum()!=f->maximum())
      write_c("%so->maximum(%g);\n", indent(), v->maximum());
    if (v->step()!=f->step())
      write_c("%so->step(%g);\n", indent(), v->step());
    if (v->value())
      write_c("%so->value(%g);\n", indent(), v->value());
    if (is_valuator()==2) {
      double x = ((Fl_Slider*)v)->slider_size();
      double y = ((Fl_Slider*)f)->slider_size();
      if (x != y) write_c("%so->slider_size(%g);\n", indent(), x);
    }
  }

  const char* ud = user_data();
  if (class_name(1) && !parent->is_widget()) ud = "this";
  if (callback()) {
    write_c("%so->callback((Fl_Callback*)%s", indent(), callback_name());
    if (ud)
      write_c(", (void*)(%s));\n", ud);
    else
      write_c(");\n");
  } else if (ud) {
    write_c("%so->user_data((void*)(%s));\n", indent(), ud);
  }
  if ((o->flags()&FL_ALIGN_MASK) != (tplate->flags()&FL_ALIGN_MASK)) {
    Fl_Flags i = o->flags() & FL_ALIGN_MASK;
    write_c("%so->align(%s", indent(),
           item_name(alignmenu, i & ~FL_ALIGN_INSIDE));
    if (i & FL_ALIGN_INSIDE) write_c("|FL_ALIGN_INSIDE");
    write_c(");\n");
  }
  if (o->when() != tplate->when())
    write_c("%so->when(%s);\n", indent(),item_name(whensymbolmenu, o->when()));
  if (!o->visible() && o->parent())
    write_c("%so->hide();\n", indent());
  if (!o->active())
    write_c("%so->deactivate();\n", indent());
  if (!is_group() && resizable())
    write_c("%sFl_Group::current()->resizable(o);\n",indent());
  if (hotspot()) {
    write_c("%s((Fl_Window*)(o", indent());
    Fl_Widget* p = o->parent();
    while (p) { p = p->parent(); write_c("->parent()"); }
    write_c("))->hotspot(o);\n");
  }
  if (tooltip() && *tooltip()) {
    write_c("%so->tooltip(", indent());
    write_cstring(tooltip());
    write_c(");\n");
  }
}

void Fl_Widget_Type::write_extra_code() {
  for (int n=0; n < NUM_EXTRA_CODE; n++)
    if (extra_code(n) && !isdeclare(extra_code(n)))
      write_c("%s%s\n", indent(), extra_code(n));
}

void Fl_Widget_Type::write_block_close() {
  indentation -= 2;
  if (is_parent() || varused) write_c("%s}\n", indent());
}

void Fl_Widget_Type::write_code() {
  write_code1();
  for (Fl_Type* q = first_child; q; q = q->next_brother) q->write_code();
  write_extra_code();
  write_block_close();
}

////////////////////////////////////////////////////////////////

void Fl_Widget_Type::write_properties() {
  Fl_Type::write_properties();
  int indent = 1;
  for (Fl_Type* p = parent; p; p = p->parent) indent++;
  write_indent(indent);
  if (!public_) write_string("private");
  if (!is_menu_item())
    write_string("xywh {%d %d %d %d}", o->x(), o->y(), o->w(), o->h());
  Fl_Widget* tplate = ((Fl_Widget_Type*)factory)->o;
  if (o->type() != tplate->type()) {
    write_string("type");
    write_word(item_name(subtypes(), o->type()));
  }
  if ((!set_xy) && (is_window())) {
    write_string("set_xy");  
  }
  if ((o->flags()&FL_ALIGN_MASK)!=(tplate->flags()&FL_ALIGN_MASK))
    write_string("align %d", o->flags());
  if (o->when() != tplate->when())
    write_string("when %d", o->when());
  if (!o->visible()) write_string("hide");
  if (!o->active()) write_string("deactivate");
  if (resizable()) write_string("resizable");
  if (hotspot()) write_string("hotspot");

  if (o->box() != tplate->box()) {
    write_string("box"); write_word(boxname(o->box()));}
  if (o->text_box() != tplate->text_box()) {
    write_string("text_box"); write_word(boxname(o->text_box()));}
  // if (o->glyph() != tplate->glyph())...
  if (o->label_font() != tplate->label_font())
    write_string("labelfont %d", o->label_font()-fl_fonts);
  if (o->text_font() != tplate->text_font())
    write_string("textfont %d", o->text_font()-fl_fonts);
  if (o->label_type() != tplate->label_type()) {
    write_string("labeltype");
    write_word(labeltypename(o->label_type()));
  }
  if (image) {
    write_string("image");
    if (!image->inlined) write_string("not_inlined");
    write_word(image->name());
  }
  if (o->color() != tplate->color())
    write_string("color %u", o->color());
  if (o->label_color() != tplate->label_color())
    write_string("labelcolor %u", o->label_color());
  if (o->selection_color() != tplate->selection_color())
    write_string("selection_color %u", o->selection_color());
  if (o->selection_text_color() != tplate->selection_text_color())
    write_string("selected_textcolor %u", o->selection_text_color());
  if (o->text_background() != tplate->text_background())
    write_string("text_background %u", o->text_background());
  if (o->highlight_color() != tplate->highlight_color())
    write_string("highlight_color %u", o->highlight_color());
  if (o->highlight_label_color() != tplate->highlight_label_color())
    write_string("highlight_label_color %u", o->highlight_label_color());
  if (o->text_color() != tplate->text_color())
    write_string("textcolor %u", o->text_color());
  if (o->label_size() != tplate->label_size())
    write_string("labelsize %u", o->label_size());
  if (o->text_size() != tplate->text_size())
    write_string("textsize %u", o->text_size());

  if (is_button()) {
    Fl_Button* b = (Fl_Button*)o;
    if (b->shortcut()) write_string("shortcut 0x%x", b->shortcut());
    if (b->value()) write_string("value 1");
  }

  if (is_valuator()) {
    Fl_Valuator* v = (Fl_Valuator*)o;
    Fl_Valuator* f = (Fl_Valuator*)(tplate);
    if (v->minimum()!=f->minimum()) write_string("minimum %g",v->minimum());
    if (v->maximum()!=f->maximum()) write_string("maximum %g",v->maximum());
    if (v->step()!=f->step()) write_string("step %g",v->step());
    if (v->value()!=0.0) write_string("value %g",v->value());
    if (is_valuator()==2) {
      double x = ((Fl_Slider*)v)->slider_size();
      double y = ((Fl_Slider*)f)->slider_size();
      if (x != y) write_string("slider_size %g", x);
    }
  }

  for (int n=0; n < NUM_EXTRA_CODE; n++) if (extra_code(n)) {
    write_indent(indent);
    write_string("code%d",n);
    write_word(extra_code(n));
  }
  if (subclass()) {
    write_indent(indent);
    write_string("class");
    write_word(subclass());
  }
}

int pasteoffset;

void Fl_Widget_Type::read_property(const char *c) {
  Fl_Widget* o = this->o;
  int x,y,w,h;
  if (!strcmp(c,"private")) {
    public_ = 0;
  } else if (!strcmp(c,"xywh")) {
    if (sscanf(read_word(),"%d %d %d %d",&x,&y,&w,&h) == 4) {
      x += pasteoffset;
      y += pasteoffset;
      // adjust for older relative coordinates:
      if (read_version < 2.0001 && !is_menu_item() && o->parent()) {
	Fl_Group* p = o->parent();
	while (p->parent()) {x -= p->x(); y -= p->y(); p = p->parent();}
      }
      o->x(x); o->y(y); o->w(w); o->h(h);
      //o->layout();
    }
  } else if (!strcmp(c,"type")) {
    o->type(item_number(subtypes(), read_word()));
  } else if (!strcmp(c,"set_xy")) {
    set_xy = 0;
  } else if (!strcmp(c,"align")) {
    if (sscanf(read_word(),"%d",&x) == 1) {o->align(x);}
  } else if (!strcmp(c,"when")) {
    if (sscanf(read_word(),"%d",&x) == 1) o->when(x);
  } else if (!strcmp(c,"hide")) {
    o->hide();
  } else if (!strcmp(c,"deactivate")) {
    o->deactivate();
  } else if (!strcmp(c,"resizable")) {
    resizable(1);
  } else if (!strcmp(c,"hotspot")) {
    hotspot(1);
  //} else if (!strcmp(c, "divider")) { // do something here...
  } else if (!strcmp(c,"class")) {
    subclass(read_word());
  } else if (is_button() && !strcmp(c,"shortcut")) {
    ((Fl_Button*)o)->shortcut(strtol(read_word(),0,0));
  } else if (is_button() && !strcmp(c,"value")) {
    const char* value = read_word();
    ((Fl_Button*)o)->value(atoi(value));

  } else if (!strcmp(c,"box")) {
    const char* value = read_word();
    Fl_Boxtype b = boxnumber(value);
    if (b) o->box(b);
    else read_error("Boxtype '%s' not found", value);
  } else if (!strcmp(c,"text_box") || !strcmp(c,"glyph_box")
	     || !strcmp(c, "window_box")) {
    const char* value = read_word();
    Fl_Boxtype b = boxnumber(value);
    if (b) o->text_box(b);
    else read_error("Boxtype '%s' not found", value);
  } else if (!strcmp(c, "down_box")) { // ignore this fltk 1.0 item
    read_word();
  } else if (!strcmp(c,"labelfont")) {
    if (sscanf(read_word(),"%d",&x) == 1) o->label_font(fl_fonts+x);
  } else if (!strcmp(c,"textfont")) {
    if (sscanf(read_word(),"%d",&x) == 1) o->text_font(fl_fonts+x);
  } else if (!strcmp(c,"labeltype")) {
    c = read_word();
    // back compatability with 1.0 and Vincent's original graphical patch
    if (!strcmp(c,"image") || !strcmp(c, "image_file")) { 
      Fluid_Image *i = Fluid_Image::find(label());
      if (!i) read_error("Image file '%s' not found", label());
      setimage(i); label(0);
      if (!strcmp(c,"image_file")) {
	c = read_word();
	if (i && c[0]=='d') i->inlined = 0;
	if (c[1]=='t') o->set_flag(FL_ALIGN_TILED);
      }
    } else {
      o->label_type((Fl_Labeltype)item_pointer(labeltypemenu,c));
    }
  } else if (!strcmp(c, "image")) {
    int inlined = 1;
    c = read_word();
    if (!strcmp(c, "inlined")) c = read_word(); // for back compatability
    if (!strcmp(c, "not_inlined")) {
      inlined = 0;
      c = read_word();
    }
    Fluid_Image *i = Fluid_Image::find(c);
    if(!inlined) i->inlined = 0;
    if (!i) read_error("Image file '%s' not found", c);
    setimage(i);
  } else if (!strcmp(c,"color")) {
    int n = sscanf(read_word(),"%u %u",&x,&y);
    o->color(x);
    if (n == 2) o->selection_color(y); // back compatability...
  } else if (!strcmp(c,"labelcolor")) {
    if (sscanf(read_word(),"%u",&x) == 1) o->label_color(x);
  } else if (!strcmp(c,"selection_color")) {
    if (sscanf(read_word(),"%u",&x)) o->selection_color(x);
  } else if (!strcmp(c,"selected_textcolor")) {
    if (sscanf(read_word(),"%u",&x)) o->selection_text_color(x);
  } else if (!strcmp(c,"text_background") || !strcmp(c,"off_color") ||
	     !strcmp(c,"window_color")) {
    if (sscanf(read_word(),"%u",&x)) o->text_background(x);
  } else if (!strcmp(c,"highlight_color")) {
    if (sscanf(read_word(),"%u",&x)) o->highlight_color(x);
  } else if (!strcmp(c,"highlight_label_color")) {
    if (sscanf(read_word(),"%u",&x)) o->highlight_label_color(x);
  } else if (!strcmp(c,"textcolor")) {
    if (sscanf(read_word(),"%u",&x)) o->text_color(x);
  } else if (!strcmp(c,"labelsize")) {
    if (sscanf(read_word(),"%u",&x)) o->label_size(x);
  } else if (!strcmp(c,"textsize")) {
    if (sscanf(read_word(),"%u",&x)) o->text_size(x);

  } else if (!strcmp(c,"minimum") && is_valuator()) {
    ((Fl_Valuator*)o)->minimum(strtod(read_word(),0));
  } else if (!strcmp(c,"maximum") && is_valuator()) {
    ((Fl_Valuator*)o)->maximum(strtod(read_word(),0));
  } else if (!strcmp(c,"step") && is_valuator()) {
    ((Fl_Valuator*)o)->step(strtod(read_word(),0));
  } else if (!strcmp(c,"value") && is_valuator()) {
    ((Fl_Valuator*)o)->value(strtod(read_word(),0));
  } else if ((!strcmp(c,"slider_size")||!strcmp(c,"size"))&&is_valuator()==2) {
    double v = strtod(read_word(),0);
    if (v < 1.0)
      ((Fl_Slider*)o)->slider_size(v);
    else
      ((Fl_Slider*)o)->slider_size(int(v));

  } else {
    if (!strncmp(c,"code",4)) {
      int n = atoi(c+4);
      if (n >= 0 && n <= NUM_EXTRA_CODE) {
	extra_code(n,read_word());
	return;
      }
    }
    Fl_Type::read_property(c);
  }
}

Fl_Menu_Item boxmenu1[] = {
  // these extra ones are for looking up fdesign saved strings:
  {"NO_FRAME",		0,0,(void *)FL_NO_BOX},
  {"ROUNDED3D_UPBOX",	0,0,(void *)FL_ROUND_UP_BOX},
  {"ROUNDED3D_DOWNBOX",	0,0,(void *)FL_ROUND_DOWN_BOX},
  {"OVAL3D_UPBOX",	0,0,(void *)FL_ROUND_UP_BOX},
  {"OVAL3D_DOWNBOX",	0,0,(void *)FL_ROUND_DOWN_BOX},
  {"0",			0,0,(void *)FL_NO_BOX},
  {"1",			0,0,(void *)FL_UP_BOX},
  {"2",			0,0,(void *)FL_DOWN_BOX},
  {"3",			0,0,(void *)FL_FLAT_BOX},
  {"4",			0,0,(void *)FL_BORDER_BOX},
  {"5",			0,0,(void *)FL_SHADOW_BOX},
  {"6",			0,0,(void *)FL_ENGRAVED_BOX},
  {"7",			0,0,(void *)FL_ROUNDED_BOX},
  {"8",			0,0,(void *)FL_RFLAT_BOX},
  {"9",			0,0,(void *)FL_RSHADOW_BOX},
  {"10",		0,0,(void *)FL_UP_BOX},
  {"11",		0,0,(void *)FL_DOWN_BOX},
{0}};

extern int fdesign_flip;
int lookup_symbol(const char *, int &, int numberok = 0);

int Fl_Widget_Type::read_fdesign(const char* name, const char* value) {
  int v;
  if (!strcmp(name,"box")) {
    float x,y,w,h;
    if (sscanf(value,"%f %f %f %f",&x,&y,&w,&h) == 4) {
      if (fdesign_flip) {
	Fl_Type *p;
	for (p = parent; p && !p->is_window(); p = p->parent);
	if (p && p->is_widget()) y = ((Fl_Widget_Type*)p)->o->h()-(y+h);
      }	  
      x += pasteoffset;
      y += pasteoffset;
      o->resize(int(x),int(y),int(w),int(h));
      o->layout();
    }
  } else if (!strcmp(name,"label")) {
    label(value);
    if (value[0] == '@') o->label_type(FL_SYMBOL_LABEL);
  } else if (!strcmp(name,"name")) {
    this->name(value);
  } else if (!strcmp(name,"callback")) {
    callback(value); user_data_type("long");
  } else if (!strcmp(name,"argument")) {
    user_data(value);
  } else if (!strcmp(name,"shortcut")) {
    if (value[0]) {
      char buf[128]; sprintf(buf,"o->shortcut(\"%s\");",value);
      extra_code(0,buf);
    }
  } else if (!strcmp(name,"style")) {
    if (!strncmp(value,"FL_NORMAL",9)) return 1;
    if (!lookup_symbol(value,v,1)) return 0;
    o->label_font(fl_fonts + v); o->label_type((Fl_Labeltype)(v>>8));
  } else if (!strcmp(name,"size")) {
    if (!lookup_symbol(value,v,1)) return 0;
    o->label_size(v);
  } else if (!strcmp(name,"type")) {
    if (!strncmp(value,"NORMAL",6)) return 1;
    if (lookup_symbol(value,v,1)) {o->type(v); return 1;}
    if (!strcmp(value+strlen(value)-5,"FRAME")) goto TRY_BOXTYPE;
    if (!strcmp(value+strlen(value)-3,"BOX")) goto TRY_BOXTYPE;
    return 0;
  } else if (!strcmp(name,"lcol")) {
    if (!lookup_symbol(value,v,1)) return 0;
    o->label_color(v);
  } else if (!strcmp(name,"return")) {
    if (!lookup_symbol(value,v,0)) return 0;
    o->when(v|FL_WHEN_RELEASE);
  } else if (!strcmp(name,"alignment")) {
    if (!lookup_symbol(value,v)) {
      // convert old numeric values:
      int v1 = atoi(value); if (v1 <= 0 && strcmp(value,"0")) return 0;
      v = 0;
      if (v1 >= 5) {v = FL_ALIGN_INSIDE; v1 -= 5;}
      switch (v1) {
      case 0: v += FL_ALIGN_TOP; break;
      case 1: v += FL_ALIGN_BOTTOM; break;
      case 2: v += FL_ALIGN_LEFT; break;
      case 3: v += FL_ALIGN_RIGHT; break;
      case 4: v += FL_ALIGN_CENTER; break;
      default: return 0;
      }
    }
    o->align(v);
  } else if (!strcmp(name,"resizebox")) {
    resizable(1);
  } else if (!strcmp(name,"colors")) {
    char* p = (char*)value;
    while (*p != ' ') {if (!*p) return 0; p++;}
    *p = 0;
    int v1;
    if (!lookup_symbol(value,v,1) || !lookup_symbol(p+1,v1,1)) {
      *p=' '; return 0;}
    o->color(v); o->selection_color(v1);
  } else if (!strcmp(name,"resize")) {
    return !strcmp(value,"FL_RESIZE_ALL");
  } else if (!strcmp(name,"gravity")) {
    return !strcmp(value,"FL_NoGravity FL_NoGravity");
  } else if (!strcmp(name,"boxtype")) {
  TRY_BOXTYPE:
    Fl_Boxtype x = boxnumber(value);
    if (!x) {x = (Fl_Boxtype)item_pointer(boxmenu1, value); if (!x) return 0;}
    if (x == FL_NO_BOX) {
      if (o->box() != ((Fl_Widget_Type*)factory)->o->box()) return 1; // kludge for frame
    }
    o->box(x);
  } else {
    return 0;
  }
  return 1;
}

//
// End of "$Id: Fl_Widget_Type.cxx,v 1.77 2001/07/23 09:50:04 spitzak Exp $".
//
