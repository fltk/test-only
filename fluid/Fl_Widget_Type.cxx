//
// "$Id: Fl_Widget_Type.cxx,v 1.87 2002/05/06 06:31:26 spitzak Exp $"
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
#include <ctype.h>

#include "coding_style.h"

int Fl_Widget_Type::is_widget() const {return 1;}

const char* Fl_Widget_Type::subclass() const {
  // first return any text class name the user put in:
  if (user_class_) return user_class_;
  // then return any class based on the "type()", this is any
  // entry in the type table where the code starts with "Fl":
  const Enumeration* table = subtypes();
  if (table) {
    const Enumeration* e = from_value(o->type(), table);
    if (e && e->subclass) return e->subclass;
  }
  return type_name();
}

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

#include <fltk/Fl_Style_Set.h>
extern Fl_Style_Set* fluid_style_set;
extern Fl_Style_Set* style_set;

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
  style_set->make_current();
  Fl_Widget_Type *t = _make();
  if (!o) o = widget(0,0,100,100); // create template widget
  t->factory = this;
  // Construct the Fl_Widget:
  t->o = widget(X,Y,W,H);
  fluid_style_set->make_current();
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
  extra_code_ = 0;
  user_class_ = 0;
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

void Fl_Widget_Type::extra_code(const char *n) {
  storestring(n,extra_code_);
}

extern void redraw_browser();
void Fl_Widget_Type::user_class(const char *n) {
  if (storestring(n,user_class_)) redraw_browser();
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
	if (gw->y() >= fw->y()+fw->h()) break;
	if (gw->y()+gw->h() > fw->y() && gw->x() >= fw->x()+fw->w()) break;
	if (gw->y() >= fw->y() && gw->x() > fw->x()) break;
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
  Fl_Color c = FL_BLACK;
  if (current_widget->name() && *(current_widget->name())) c = FL_RED;
  if (o->label_color() != c)
    { o->label_color(c); o->redraw_label(); }
}

#define for_all_selected_widgets() \
for (Fl_Type *o = Fl_Type::first; o; o = o->walk()) \
  if (o->selected && o->is_widget())

void name_public_cb(Fl_Check_Button* i, void* v) {
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
  Fl_Color c = FL_BLACK;
  if (current_widget->label() && *(current_widget->label())) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
}

static char* oldtooltip;
static unsigned oldtooltiplen;

void tooltip_cb(Fl_Wordwrap_Input* i, void *v) {
  if (v == LOAD) {
    if (current_widget->o->is_window()) { i->hide(); return; }
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
  Fl_Color c = FL_BLACK;
  if (current_widget->tooltip() && *(current_widget->tooltip())) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
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

void set_xy_cb(Fl_Check_Button* i, void *v) {
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

static const Enumeration boxmenu[] = {
{"None",	"NO_BOX",	(void *)FL_NO_BOX},
{"Up",		"UP_BOX",	(void *)FL_UP_BOX},
{"Down",	"DOWN_BOX",	(void *)FL_DOWN_BOX},
{"Thin Up",	"THIN_UP_BOX",	(void *)FL_THIN_UP_BOX},
{"Thin Down",	"THIN_DOWN_BOX",(void *)FL_THIN_DOWN_BOX},
{"Flat",	"FLAT_BOX",	(void *)FL_FLAT_BOX},
{"Border",	"BORDER_BOX",	(void *)FL_BORDER_BOX},
{"Frame",	"BORDER_FRAME",	(void *)FL_BORDER_FRAME},
{"Engraved",	"ENGRAVED_BOX",	(void *)FL_ENGRAVED_BOX},
{"Embossed",	"EMBOSSED_BOX",	(void *)FL_EMBOSSED_BOX},
{"Highlight",	"HIGHLIGHT_UP_BOX",(void *)FL_HIGHLIGHT_UP_BOX},
{"Highlight Down","HIGHLIGHT_DOWN_BOX",(void *)FL_HIGHLIGHT_DOWN_BOX},
{"Round",	"ROUND_UP_BOX",	(void *)FL_ROUND_UP_BOX},
{"Round Down",	"ROUND_DOWN_BOX",(void *)FL_ROUND_DOWN_BOX},
{"Diamond",	"DIAMOND_UP_BOX",(void *)FL_DIAMOND_UP_BOX},
{"Diamond Down","DIAMOND_DOWN_BOX",(void *)FL_DIAMOND_DOWN_BOX},
{"Shadow",	"SHADOW_BOX",	(void *)FL_SHADOW_BOX},
{"Rounded",	"ROUNDED_BOX",	(void *)FL_ROUNDED_BOX},
{"Rounded Shadow","RSHADOW_BOX",(void *)FL_RSHADOW_BOX},
{"Rounded Flat","RFLAT_BOX",	(void *)FL_RFLAT_BOX},
{"Oval",	"OVAL_BOX",	(void *)FL_OVAL_BOX},
{"Oval Shadow",	"OSHADOW_BOX",	(void *)FL_OSHADOW_BOX},
{"Oval Flat",	"OFLAT_BOX",	(void *)FL_OFLAT_BOX},
{0}};

#define NOT_DEFAULT(W, what) (W->o->what() != ((Fl_Widget_Type*)(W->factory))->o->what())

void box_cb(Fl_Choice* i, void *v) {
  if (v == LOAD) {
    set_menu(i, boxmenu);
    const Enumeration* e = from_value((void*)(current_widget->o->box()), boxmenu);
    i->value(e ? e-boxmenu : 0);
  } else {
    int m = i->value();
    Fl_Boxtype n = Fl_Boxtype(boxmenu[m].compiled);
    if (!n) return; // should not happen
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->box(n);
      q->redraw();
    }
  }
  Fl_Color c = FL_BLACK;
  if (NOT_DEFAULT(current_widget, box)) c = FL_RED;
  if (i->label_color() != c) {
    i->label_color(c);
    i->redraw_label();
  }
}

void button_box_cb(Fl_Choice* i, void *v) {
  if (v == LOAD) {
    set_menu(i, boxmenu);
    const Enumeration* e = from_value((void*)(current_widget->o->button_box()), boxmenu);
    i->value(e ? e-boxmenu : 0);
  } else {
    int m = i->value();
    Fl_Boxtype n = Fl_Boxtype(boxmenu[m].compiled);
    if (!n) return; // should not happen
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->button_box(n);
      q->redraw();
    }
  }
  Fl_Color c = FL_BLACK;
  if (NOT_DEFAULT(current_widget, button_box)) c = FL_RED;
  if (i->label_color() != c) {
    i->label_color(c);
    i->redraw_label();
  }
}

////////////////////////////////////////////////////////////////

static const Enumeration whenmenu[] = {
  {"never",	"NEVER",	(void*)FL_WHEN_NEVER},
  {"Changed",	"CHANGED",	(void*)FL_WHEN_CHANGED},
  {"Release",	"RELEASE",	(void*)FL_WHEN_RELEASE},
  {"Release always","RELEASE_ALWAYS",(void*)FL_WHEN_RELEASE_ALWAYS},
  {"Enter key",	"ENTER_KEY",	(void*)FL_WHEN_ENTER_KEY},
  {"Enter key always","ENTER_KEY_ALWAYS",(void*)FL_WHEN_ENTER_KEY_ALWAYS},
  {0}
};

void when_cb(Fl_Choice* i, void *v) {
  if (v == LOAD) {
    if (current_widget->is_menu_item()) {i->hide(); return;}
    set_menu(i, whenmenu);
    i->show();
    const Enumeration* e =
      from_value(current_widget->o->when(), whenmenu);
    i->value(e ? e-whenmenu : 0);
  } else {
    int m = i->value();
    int n = int(whenmenu[m].compiled);
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->when(n);
    }
  }
  i->redraw();
}

void when_button_cb(Fl_Check_Button*, void *) {} // delete this!

uchar Fl_Widget_Type::resizable() const {
  if (is_group()) if (!((Fl_Group*)o)->resizable()) return false;
  Fl_Group* group = o->parent();
  if (group && group->resizable() != o) return false;
  return true;
}

void Fl_Widget_Type::resizable(uchar value) {
  if (value) {
    Fl_Widget* child = o;
    Fl_Group* group = is_group() ? (Fl_Group*)o : o->parent();
    while (group) {
      if (group->resizable() != child) {
	group->resizable(child);
	group->init_sizes();
      }
      child = group;
      group = group->parent();
    }
  } else {
    Fl_Group* group = is_group() ? (Fl_Group*)o : o->parent();
    while (group) {
      group->resizable(0);
      group = group->parent();
    }
  }
}

void resizable_cb(Fl_Check_Button* i,void* v) {
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

void hotspot_cb(Fl_Check_Button* i,void* v) {
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

void visible_cb(Fl_Check_Button* i, void* v) {
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

void active_cb(Fl_Check_Button* i, void* v) {
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

static const Enumeration fontmenu[] = {
  {"Helvetica",		"HELVETICA",		(void*)FL_HELVETICA},
  {"Helvetica bold",	"HELVETICA_BOLD",	(void*)FL_HELVETICA_BOLD},
  {"Helvetica italic",	"HELVETICA_ITALIC",	(void*)FL_HELVETICA_ITALIC},
  {"Helvetica bold italic","HELVETICA_BOLD_ITALIC",(void*)FL_HELVETICA_BOLD_ITALIC},
  {"Courier",		"COURIER",		(void*)FL_COURIER},
  {"Courier bold",	"COURIER_BOLD",		(void*)FL_COURIER_BOLD},
  {"Courier italic",	"COURIER_ITALIC",	(void*)FL_COURIER_ITALIC},
  {"Courier bold italic","COURIER_BOLD_ITALIC",	(void*)FL_COURIER_BOLD_ITALIC},
  {"Times",		"TIMES",		(void*)FL_TIMES},
  {"Times bold",	"TIMES_BOLD",		(void*)FL_TIMES_BOLD},
  {"Times italic",	"TIMES_ITALIC",		(void*)FL_TIMES_ITALIC},
  {"Times bold italic",	"TIMES_BOLD_ITALIC",	(void*)FL_TIMES_BOLD_ITALIC},
  {"Symbol",		"SYMBOL",		(void*)FL_SYMBOL},
  {"Terminal",		"SCREEN",		(void*)FL_SCREEN},
  {"Terminal Bold",	"SCREEN_BOLD",		(void*)FL_SCREEN_BOLD},
  {"Zapf Dingbats",	"ZAPF_DINGBATS",	(void*)FL_ZAPF_DINGBATS},
  {0}
};

void label_font_cb(Fl_Choice* i, void *v) {
  if (v == LOAD) {
    set_menu(i, fontmenu);
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
  Fl_Color c = FL_BLACK;
  if (NOT_DEFAULT(current_widget, label_font)) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
}

void label_size_cb(Fl_Value_Input* i, void *v) {
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
  Fl_Color c = FL_BLACK;
  if (NOT_DEFAULT(current_widget, label_size)) c = FL_RED;
  if (i->text_color() != c)
    { i->text_color(c); i->redraw();}
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
  Fl_Color c = FL_BLACK;
  if (current_widget->image) c = FL_RED;
  if (image_label->label_color() != c)
    { image_label->label_color(c); image_label->redraw();}
}

static const Enumeration labeltypemenu[] = {
  {"Normal",	"NORMAL_LABEL",	(void*)FL_NORMAL_LABEL},
  {"Symbol",	"SYMBOL_LABEL",	(void*)FL_SYMBOL_LABEL},
  {"Shadow",	"SHADOW_LABEL",	(void*)FL_SHADOW_LABEL},
  {"Engraved",	"ENGRAVED_LABEL",(void*)FL_ENGRAVED_LABEL},
  {"Embossed",	"EMBOSSED_LABEL",(void*)FL_EMBOSSED_LABEL},
  {"None",	"NO_LABEL",	(void*)(FL_NO_LABEL)},
{0}};

void label_type_cb(Fl_Choice* i, void *v) {
  if (v == LOAD) {
    set_menu(i, labeltypemenu);
    const Enumeration* e = from_value((void*)(current_widget->o->label_type()), labeltypemenu);
    i->value(e ? e-labeltypemenu : 0);
  } else {
    int m = i->value();
    Fl_Labeltype n = Fl_Labeltype(labeltypemenu[m].compiled);
    if (!n) return; // should not happen
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* p = (Fl_Widget_Type*)o;
      p->o->label_type(n);
      p->redraw();
    }
  }
  Fl_Color c = FL_BLACK;
  if (NOT_DEFAULT(current_widget, label_type)) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
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
  i->button_color(c);
  i->selection_color(c);
  i->label_color(NOT_DEFAULT(current_widget, color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void selection_color_cb(Fl_Light_Button* i, void *v) {
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
  i->button_color(c);
  i->selection_color(c);
  i->label_color(NOT_DEFAULT(current_widget, selection_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void button_color_cb(Fl_Light_Button* i, void *v) {
  Fl_Color c = current_widget->o->button_color();
  if (v == LOAD) {
    i->show();
  } else {
    if (!fl_color_chooser(i->label(), c)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->button_color(c); q->redraw();
    }
  }
  i->button_color(c);
  i->selection_color(c);
  i->label_color(NOT_DEFAULT(current_widget, button_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void label_color_cb(Fl_Light_Button* i, void *v) {
  Fl_Color c = current_widget->o->label_color();
  if (v != LOAD) {
    if (!fl_color_chooser(i->label(), c)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->label_color(c); q->redraw();
    }
  } else {
    i->show();
  }
  i->button_color(c);
  i->selection_color(c);
  i->label_color(NOT_DEFAULT(current_widget, label_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void text_font_cb(Fl_Choice* i, void* v) {
  Fl_Font n;
  if (v == LOAD) {
    set_menu(i, fontmenu);
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
  Fl_Color c = FL_BLACK;
  if (NOT_DEFAULT(current_widget, text_font)) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
}

void text_size_cb(Fl_Value_Input* i, void* v) {
  int s;
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
  Fl_Color c = FL_BLACK;
  if (NOT_DEFAULT(current_widget, text_size)) c = FL_RED;
  if (i->text_color() != c)
    { i->text_color(c); i->redraw(); }
}

void text_color_cb(Fl_Light_Button* i, void* v) {
  Fl_Color c;
  if (v == LOAD) {
    if (current_widget->is_menu_item()) i->label("Shortcut Color");
    else if (current_widget->is_slider()) i->label("Glyph Color");
    else i->label("Text Color");
    c = current_widget->o->text_color();
    i->show();
  } else {
    c = i->selection_color();
    if (!fl_color_chooser(i->label(), c)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->text_color(c);
      q->redraw();
    }
  }
  i->button_color(c);
  i->selection_color(c);
  i->label_color(NOT_DEFAULT(current_widget, text_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void selected_text_color_cb(Fl_Light_Button* i, void* v) {
  Fl_Color c;
  if (v == LOAD) {
    i->label("Selected Text Color");
    c = current_widget->o->selection_text_color();
    i->show();
  } else {
    c = i->selection_color();
    if (!fl_color_chooser(i->label(), c)) return;
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      q->o->selection_text_color(c);
      q->redraw();
    }
  }
  i->button_color(c);
  i->selection_color(c);
  i->label_color(NOT_DEFAULT(current_widget, selection_text_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void highlight_color_cb(Fl_Light_Button* i, void *v) {
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
  i->button_color(c);
  i->selection_color(c);
  i->label_color(NOT_DEFAULT(current_widget, highlight_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

void highlight_label_color_cb(Fl_Light_Button* i, void *v) {
  Fl_Color c = FL_BLACK;
  if (v == LOAD) {
    i->label("Highlight Label Color");
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
  i->button_color(c);
  i->selection_color(c);
  i->label_color(NOT_DEFAULT(current_widget, highlight_label_color) ? FL_RED : FL_BLACK);
  i->redraw();
}

static const Enumeration alignmenu[] = {
  {"x", "FL_ALIGN_CENTER",	(void*)(FL_ALIGN_CENTER)},
  {"x", "FL_ALIGN_TOP",		(void*)(FL_ALIGN_TOP)},
  {"x", "FL_ALIGN_BOTTOM",	(void*)(FL_ALIGN_BOTTOM)},
  {"x", "FL_ALIGN_LEFT",	(void*)(FL_ALIGN_LEFT)},
  {"x", "FL_ALIGN_RIGHT",	(void*)(FL_ALIGN_RIGHT)},
  {"x", "FL_ALIGN_INSIDE",	(void*)(FL_ALIGN_INSIDE)},
  {"x", "FL_ALIGN_CLIP",	(void*)(FL_ALIGN_CLIP)},
  {"x", "FL_ALIGN_WRAP",	(void*)(FL_ALIGN_WRAP)},
  {"x", "FL_ALIGN_TOP | FL_ALIGN_LEFT",(void*)(FL_ALIGN_TOP | FL_ALIGN_LEFT)},
  {"x", "FL_ALIGN_TOP | FL_ALIGN_RIGHT",(void*)(FL_ALIGN_TOP | FL_ALIGN_RIGHT)},
  {"x", "FL_ALIGN_BOTTOM | FL_ALIGN_LEFT",(void*)(FL_ALIGN_BOTTOM | FL_ALIGN_LEFT)},
  {"x", "FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT",(void*)(FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT)},
  {"x", "FL_ALIGN_LEFT | FL_ALIGN_TOP",(void*)(FL_ALIGN_LEFT | FL_ALIGN_TOP)},
  {"x", "FL_ALIGN_RIGHT | FL_ALIGN_TOP",(void*)(FL_ALIGN_RIGHT | FL_ALIGN_TOP)},
  {"x", "FL_ALIGN_LEFT | FL_ALIGN_BOTTOM",(void*)(FL_ALIGN_LEFT | FL_ALIGN_BOTTOM)},
  {"x", "FL_ALIGN_RIGHT | FL_ALIGN_BOTTOM",(void*)(FL_ALIGN_RIGHT | FL_ALIGN_BOTTOM)},

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

void image_inlined_cb(Fl_Check_Button* i, void *v) {
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

void callback_cb(Fl_Multiline_Input* i, void *v) {
  if (v == LOAD) {
    i->static_value(current_widget->callback());
  } else {
    const char *c = i->value();
    const char *d = c_check(c);
    if (d) {fl_message("Error in callback: %s",d); haderror = 1; return;}
    for_all_selected_widgets() o->callback(c);
  }
  Fl_Color c = FL_BLACK;
  if (current_widget->callback()) c = FL_RED;
  if (i->label_color() != c) {
    i->label_color(c);
    i->redraw_label();
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
  Fl_Color c = FL_BLACK;
  if (current_widget->user_data()) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
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
  Fl_Color c = FL_BLACK;
  if (strcmp(i->value(), "void*")) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
}

void extra_code_input_cb(Fl_Multiline_Input* i, void* v) {
  if (v == LOAD) {
    i->static_value(current_widget->extra_code());
  } else {
    const char *c = i->value();
    const char *d = c_check(c&&c[0]=='#' ? c+1 : c);
    if (d) {fl_message("Error in %s: %s",i->label(),d); haderror = 1; return;}
    for_all_selected_widgets() {
      Fl_Widget_Type *t = (Fl_Widget_Type*)o;
      t->extra_code(c);
    }
  }
  Fl_Color c = FL_BLACK;
  if (i->value() && *i->value()) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
}

void user_class_cb(Fl_Input* i, void* v) {
  if (v == LOAD) {
    const char* s = 0;
    for_all_selected_widgets() {
      Fl_Widget_Type *t = (Fl_Widget_Type*)o;
      const char* c = t->subclass();
      if (!s || !*s) s = c;
      else if (strcmp(s,c)) {s = 0; break;}
    }
    i->static_value(s);
  } else {
    const char *c = i->value();
    while (isspace(*c)) c++;
    for_all_selected_widgets() {
      Fl_Widget_Type *t = (Fl_Widget_Type*)o;
      t->user_class(0);
      if (*c && strcmp(c, t->subclass())) t->user_class(c);
    }
  }
  Fl_Color c = FL_BLACK;
  if (current_widget->user_class()) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
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
  Fl_Color c = FL_BLACK;
  if (i->value() != 0.0) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
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
  Fl_Color c = FL_BLACK;
  if (i->value() != 1.0) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
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
  Fl_Color c = FL_BLACK;
  if (i->value() != 0.0) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
}

void line_cb(Fl_Value_Input* i, void* v) {
  if (v == LOAD) {
    if (!current_widget->is_valuator()) {i->hide(); return;}
    i->show();
    i->value(((Fl_Valuator*)(current_widget->o))->linesize());
  } else {
    int n = int(i->value());
    if (n > 0) for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      if (q->is_valuator()) {
	((Fl_Valuator*)(q->o))->linesize(n);
	q->redraw();
      }
    }
  }
  Fl_Color c = FL_BLACK;
  if (i->value() != 1.0) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
}

void page_cb(Fl_Value_Input* i, void* v) {
  if (v == LOAD) {
    if (!current_widget->is_valuator()) {i->hide(); return;}
    i->show();
    i->value(((Fl_Valuator*)(current_widget->o))->pagesize());
  } else {
    int n = int(i->value());
    if (n > 0) for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      if (q->is_valuator()) {
	((Fl_Valuator*)(q->o))->pagesize(n);
	q->redraw();
      }
    }
  }
  Fl_Color c = FL_BLACK;
  if (i->value() != 10.0) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
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
  Fl_Color c = FL_BLACK;
  if (i->value() != 0.0) c = FL_RED;
  if (i->label_color() != c)
    { i->label_color(c); i->redraw_label(); }
}

////////////////////////////////////////////////////////////////

// subtypes:

const Enumeration *Fl_Widget_Type::subtypes() const {return 0;}

void subtype_cb(Fl_Choice* i, void* v) {
  if (v == LOAD) {
    const Enumeration* table = current_widget->subtypes();
    if (!table) {i->hide(); return;}
    i->show();
    set_menu(i, table);
    const Enumeration* e = from_value(current_widget->o->type(), table);
    i->value(e ? e-table : 0);
    i->redraw();
  } else {
    const Enumeration* table = current_widget->subtypes();
    int n = int(table[i->value()].compiled);
    for_all_selected_widgets() {
      modflag = 1;
      Fl_Widget_Type* q = (Fl_Widget_Type*)o;
      if (q->subtypes()==table) {
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
void overlay_cb(Fl_Check_Button*o,void *v) {
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
#if 0
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
#endif
  }
  // update the selected flags to new set:
  for (Fl_Type *o = Fl_Type::first; o; o = o->walk())
    o->selected = o->new_selected;
  if (p && p->new_selected) Fl_Type::current = p;
  redraw_overlays();
  // load the panel with the new settings:
  load_panel();
}

////////////////////////////////////////////////////////////////
// Writing the C code:

// test to see if user named a function, or typed in code:
int is_name(const char *c) {
  for (; *c; c++) if (ispunct(*c) && *c!='_' && *c!=':') return 0;
  return 1;
}

// Test to see if name() is an array entry.  If so, and this is the
// highest number, return name[num+1].  Return null if not the highest
// number or a field or function.  Return name() if not an array entry.
const char* Fl_Widget_Type::array_name() const {
  const char *c = name();
  if (!c) return 0;
  const char *d;
  for (d = c; *d != '['; d++) {
    if (!*d) return c;
    if (ispunct(*d) && *d!='_') return 0;
  }
  char* p;
  long num = strtol(d+1, &p, 0); if (*p != ']') return 0;
  int sawthis = 0;
  Fl_Type* parent = this->parent;
  while (parent && !parent->is_class()) parent = parent->parent;
  for (Fl_Type* t = parent?parent->first_child : Fl_Type::first;
       t; t = t->walk(parent)) {
    if (t == (Fl_Type*)this) {sawthis=1; continue;}
    const char *e = t->name();
    if (!e) continue;
    if (strncmp(c,e,d-c)) continue;
    int n1 = strtol(e+(d-c)+1,0,0);
    if (n1 > num || n1==num && sawthis) return 0;
  }
  static char buffer[128];
  strncpy(buffer,c,d-c+1);
  sprintf(buffer+(d-c+1), "%ld]", num+1);
  return buffer;
}

void Fl_Widget_Type::write_static() {
  const char* subclass = this->subclass();
  if (!user_class()) ::write_declare("#include <fltk/%s.h>", subclass);
  if (extra_code()) write_includes_from_code((char*)extra_code());
  if (callback()) {
    if (is_name(callback()))
      ::write_declare("extern void %s(%s*, %s);", callback(), subclass,
		      user_data_type() ? user_data_type() : "void*");
    else
      write_includes_from_code((char*)callback());
  }
  const char* array_name = this->array_name();
  const char* member_of = this->member_of(true);
  if (array_name && !member_of) {
    write_c("\n");
    if (!public_) write_c("static ");
    else write_h("extern %s* %s;\n", subclass, array_name);
    write_c("%s* %s;\n", subclass, array_name);
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
    if (member_of) {
      write_c("\ninline void %s::%s_i(%s*", member_of, cn, subclass);
    } else {
      write_c("\nstatic void %s(%s*", cn, subclass);
    }
    if (use_o) write_c(" o");
    const char* ut = user_data_type() ? user_data_type() : "void*";
    write_c(", %s", ut);
    if (use_v) write_c(" v");
    write_c(")%s", get_opening_brace(1));
    indentation += 2;
    write_code_block((char *)callback());
    indentation -= 2;
    if (*(d-1) != ';') {
      const char *p = strrchr(callback(), '\n');
      if (p) p ++;
      else p = callback();
      // Only add trailing semicolon if the last line is not a preprocessor
      // statement...
      if (*p != '#' && *p) write_c(";");
    }
    write_c("}\n");
    if (member_of) {
      write_c("void %s::%s(%s* o, %s v)%s", member_of, cn, subclass,
	      ut, get_opening_brace(1));
      write_c("%s((%s*)(o->", get_indent_string(1), member_of);
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
extern Fl_Widget_Type* last_group;
  
void Fl_Widget_Type::write_code1() {
  const char* subclass = this->subclass();
  const char* array_name = this->array_name();
  bool member_of = this->member_of() != 0;
  if (array_name) {
    if (member_of) {
      write_public(public_);
      write_h("%s%s *%s;\n", indent(), subclass, array_name);
    }
  }
  if (member_of && callback() && !is_name(callback())) {
    const char* cn = callback_name();
    const char* ut = user_data_type() ? user_data_type() : "void*";
    write_public(0);
    write_h("%sinline void %s_i(%s*, %s);\n", indent(), cn, subclass, ut);
    write_h("%sstatic void %s(%s*, %s);\n", indent(), cn, subclass, ut);
  }
  // figure out if local variable will be used (prevent compiler warnings):
  if (is_parent() || extra_code())
    varused = 1;
  else {
    varused_test = 1; varused = 0;
    write_widget_code();
    varused_test = 0;
  }
  write_c(indent());
  if (varused) write_c("%s%s* o = ", get_opening_brace(0), subclass);
  if (name()) write_c("%s = ", name());
  if (is_window()) {
    if (set_xy && strcmp(subclass,"Fl_Group")!=0)
      write_c("new %s(%d, %d", subclass, o->w(), o->h());
    else
      write_c("new %s(%d, %d, %d, %d", subclass, o->x(), o->y(), o->w(), o->h());
  } else if (is_menu_item()) {
    write_c("new %s(", subclass);
  } else {
    write_c("new %s(%d, %d, %d, %d", subclass, o->x(), o->y(), o->w(), o->h());
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

static void write_color(const char* function, Fl_Color c) {
  if (c > 255) write_c("%so->%s((Fl_Color)0x%x);\n", indent(), function, c);
  else write_c("%so->%s((Fl_Color)%i);\n", indent(), function, c);
}

// this is split from write_code1() for Fl_Window_Type:
void Fl_Widget_Type::write_widget_code() {
  Fl_Widget* tplate = ((Fl_Widget_Type*)factory)->o;
  if (o->type() != tplate->type()) {
    const Enumeration* e = subtypes();
    if (e) e = from_value(o->type(), e);
    if (e && e->symbol)
      write_c("%so->type(%s::%s);\n", indent(), subclass(), e->symbol);
    else
      write_c("%so->type(%d);\n", indent(), o->type());
  }

  if (image) image->write_code();

  if (o->box() != tplate->box())
    write_c("%so->box(FL_%s);\n",indent(),to_text((void*)(o->box()),boxmenu));
  if (o->button_box() != tplate->button_box())
    write_c("%so->button_box(FL_%s);\n", indent(), to_text((void*)(o->button_box()),boxmenu));
  if (o->label_font() != tplate->label_font())
    write_c("%so->label_font(fl_fonts+%d);\n", indent(), o->label_font()-fl_fonts);
  if (o->text_font() != tplate->text_font())
    write_c("%so->text_font(fl_fonts+%d);\n", indent(), o->text_font()-fl_fonts);
  if (o->label_type() != tplate->label_type())
    write_c("%so->label_type(FL_%s);\n", indent(),
	    to_text((void*)(o->label_type()),labeltypemenu));
  if (o->color() != tplate->color())
    write_color("color", o->color());
  if (o->label_color() != tplate->label_color())
    write_color("label_color", o->label_color());
  if (o->selection_color() != tplate->selection_color())
    write_color("selection_color", o->selection_color());
  if (o->selection_text_color() != tplate->selection_text_color())
    write_color("selection_text_color", o->selection_text_color());
  if (o->button_color() != tplate->button_color())
    write_color("button_color", o->button_color());
  if (o->highlight_color() != tplate->highlight_color())
    write_color("highlight_color", o->highlight_color());
  if (o->highlight_label_color() != tplate->highlight_label_color())
    write_color("highlight_label_color", o->highlight_label_color());
  if (o->text_color() != tplate->text_color())
    write_color("text_color", o->text_color());
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
    if (v->linesize()!=f->linesize())
      write_c("%so->linesize(%g);\n", indent(), v->linesize());
    if (v->pagesize()!=f->pagesize())
      write_c("%so->pagesize(%g);\n", indent(), v->pagesize());
    if (v->value())
      write_c("%so->value(%g);\n", indent(), v->value());
    if (is_valuator()==2) {
      double x = ((Fl_Slider*)v)->slider_size();
      double y = ((Fl_Slider*)f)->slider_size();
      if (x != y) write_c("%so->slider_size(%g);\n", indent(), x);
    }
  }

  const char* ud = user_data();
  if (member_of() && !parent->is_widget()) ud = "this";
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
           number_to_text(i & ~FL_ALIGN_INSIDE, alignmenu));
    if (i & FL_ALIGN_INSIDE) write_c("|FL_ALIGN_INSIDE");
    write_c(");\n");
  }
  if (o->when() != tplate->when())
    write_c("%so->when(FL_WHEN_%s);\n", indent(), number_to_text(o->when(), whenmenu));
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
  if (extra_code()) write_code_block((char*)extra_code());
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

static void save_color(const char* name, Fl_Color color) {
  if (color > 255) write_string("%s 0x%x", name, color);
  else write_string("%s %u", name, color);
}

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
    write_word(number_to_text(o->type(), subtypes()));
  }
  if ((!set_xy) && (is_window())) {
    write_string("set_xy");  
  }
  if (o->align() != tplate->align())
    write_string("align %d", o->align());
  if (o->when() != tplate->when())
    write_string("when %s", number_to_text(o->when(), whenmenu));
  if (!o->visible()) write_string("hide");
  if (!o->active()) write_string("deactivate");
  if (resizable()) write_string("resizable");
  if (hotspot()) write_string("hotspot");

  if (o->box() != tplate->box()) {
    write_string("box"); write_word(to_text((void*)(o->box()),boxmenu));}
  if (o->button_box() != tplate->button_box()) {
    write_string("button_box"); write_word(to_text((void*)(o->button_box()),boxmenu));}
  if (o->label_font() != tplate->label_font())
    write_string("label_font %d", o->label_font()-fl_fonts);
  if (o->text_font() != tplate->text_font())
    write_string("text_font %d", o->text_font()-fl_fonts);
  if (o->label_type() != tplate->label_type()) {
    write_string("label_type");
    write_word(to_text((void*)(o->label_type()),labeltypemenu));
  }
  if (image) {
    write_string("image");
    if (!image->inlined) write_string("not_inlined");
    write_word(image->name());
  }
  if (o->color() != tplate->color())
    save_color("color", o->color());
  if (o->label_color() != tplate->label_color())
    save_color("label_color", o->label_color());
  if (o->selection_color() != tplate->selection_color())
    save_color("selection_color", o->selection_color());
  if (o->selection_text_color() != tplate->selection_text_color())
    save_color("selected_text_color", o->selection_text_color());
  if (o->button_color() != tplate->button_color())
    save_color("button_color", o->button_color());
  if (o->highlight_color() != tplate->highlight_color())
    save_color("highlight_color", o->highlight_color());
  if (o->highlight_label_color() != tplate->highlight_label_color())
    save_color("highlight_label_color", o->highlight_label_color());
  if (o->text_color() != tplate->text_color())
    save_color("text_color", o->text_color());
  if (o->label_size() != tplate->label_size())
    write_string("label_size %u", o->label_size());
  if (o->text_size() != tplate->text_size())
    write_string("text_size %u", o->text_size());

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
    if (v->linesize()!=f->linesize()) write_string("linesize %d",v->linesize());
    if (v->pagesize()!=f->pagesize()) write_string("pagesize %d",v->pagesize());
    if (v->value()!=0.0) write_string("value %g",v->value());
    if (is_valuator()==2) {
      double x = ((Fl_Slider*)v)->slider_size();
      double y = ((Fl_Slider*)f)->slider_size();
      if (x != y) write_string("slider_size %g", x);
    }
  }

  if (extra_code()) {
    write_indent(indent);
    write_string("extra_code");
    write_word(extra_code());
  }
  if (user_class()) {
    write_indent(indent);
    write_string("class");
    write_word(user_class());
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
    o->type(number_from_text(read_word(), subtypes()));
  } else if (!strcmp(c,"set_xy")) {
    set_xy = 0;
  } else if (!strcmp(c,"align")) {
    o->align((int)strtoul(read_word(),0,0));
  } else if (!strcmp(c,"when")) {
    o->when(number_from_text(read_word(), whenmenu));
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
    user_class(read_word());
  } else if (is_button() && !strcmp(c,"shortcut")) {
    ((Fl_Button*)o)->shortcut(strtol(read_word(),0,0));
  } else if (is_button() && !strcmp(c,"value")) {
    const char* value = read_word();
    ((Fl_Button*)o)->value(atoi(value));

  } else if (!strcmp(c,"box") || !strcmp(c,"text_box") || !strcmp(c, "window_box")) {
    const char* value = read_word();
    const Enumeration* e = from_text(value, boxmenu);
    if (e) o->box(Fl_Boxtype(e->compiled));
    else read_error("Boxtype '%s' not found", value);
  } else if (!strcmp(c,"button_box") || !strcmp(c,"glyph_box")) {
    const char* value = read_word();
    const Enumeration* e = from_text(value, boxmenu);
    if (e) o->button_box(Fl_Boxtype(e->compiled));
    else read_error("Boxtype '%s' not found", value);
  } else if (!strcmp(c, "down_box")) { // ignore this fltk 1.0 item
    read_word();
  } else if (!strcmp(c,"labelfont") || !strcmp(c,"label_font")) {
    if (sscanf(read_word(),"%d",&x) == 1) o->label_font(fl_fonts+x);
  } else if (!strcmp(c,"textfont") || !strcmp(c,"text_font")) {
    if (sscanf(read_word(),"%d",&x) == 1) o->text_font(fl_fonts+x);
  } else if (!strcmp(c,"labeltype") || !strcmp(c,"label_type")) {
    c = read_word();
    // back compatability with 1.0 and Vincent's original graphical patch
    if (!strcmp(c,"image") || !strcmp(c, "image_file")) { 
      Fluid_Image *i = Fluid_Image::find(label());
      if (!i) read_error("Image file '%s' not found", label());
      setimage(i); label(0);
      if (!strcmp(c,"image_file")) {
	c = read_word();
	if (i && c[0]=='d') i->inlined = 0;
	// if (c[1]=='t') do something here to make it Fl_Tiled_Image
      }
    } else {
      const Enumeration* e = from_text(c, labeltypemenu);
      if (e) o->label_type((Fl_Labeltype)(e->compiled));
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
  } else if (!strcmp(c,"color") || !strcmp(c,"text_background")
	     || !strcmp(c,"off_color") || !strcmp(c,"window_color")) {
    char* p;
    o->color((Fl_Color)strtoul(read_word(),&p,0));
    // back compatability with very old fluid versions that wrote 2 numbers:
    if (*p) o->selection_color((Fl_Color)strtoul(p,0,0));
  } else if (!strcmp(c,"labelcolor") || !strcmp(c,"label_color")) {
    o->label_color((Fl_Color)strtoul(read_word(),0,0));
  } else if (!strcmp(c,"selection_color")) {
    o->selection_color((Fl_Color)strtoul(read_word(),0,0));
  } else if (!strcmp(c,"selected_text_color") || !strcmp(c,"selected_textcolor")) {
    o->selection_text_color((Fl_Color)strtoul(read_word(),0,0));
  } else if (!strcmp(c,"button_color") || !strcmp(c,"off_color")) {
    o->button_color((Fl_Color)strtoul(read_word(),0,0));
  } else if (!strcmp(c,"highlight_color")) {
    o->highlight_color((Fl_Color)strtoul(read_word(),0,0));
  } else if (!strcmp(c,"highlight_label_color")) {
    o->highlight_label_color((Fl_Color)strtoul(read_word(),0,0));
  } else if (!strcmp(c,"textcolor") || !strcmp(c,"text_color")) {
    o->text_color((Fl_Color)strtoul(read_word(),0,0));
  } else if (!strcmp(c,"labelsize") || !strcmp(c,"label_size")) {
    o->label_size((int)strtoul(read_word(),0,0));
  } else if (!strcmp(c,"textsize") || !strcmp(c,"text_size")) {
    o->text_size((int)strtoul(read_word(),0,0));

  } else if (!strcmp(c,"minimum") && is_valuator()) {
    ((Fl_Valuator*)o)->minimum(strtod(read_word(),0));
  } else if (!strcmp(c,"maximum") && is_valuator()) {
    ((Fl_Valuator*)o)->maximum(strtod(read_word(),0));
  } else if (!strcmp(c,"step") && is_valuator()) {
    ((Fl_Valuator*)o)->step(strtod(read_word(),0));
  } else if (!strcmp(c,"linesize") && is_valuator()) {
    ((Fl_Valuator*)o)->linesize(strtod(read_word(),0));
  } else if (!strcmp(c,"pagesize") && is_valuator()) {
    ((Fl_Valuator*)o)->pagesize(strtod(read_word(),0));
  } else if (!strcmp(c,"value") && is_valuator()) {
    ((Fl_Valuator*)o)->value(strtod(read_word(),0));
  } else if ((!strcmp(c,"slider_size")||!strcmp(c,"size"))&&is_valuator()==2) {
    double v = strtod(read_word(),0);
    if (v < 1.0)
      ((Fl_Slider*)o)->slider_size(int(v*o->w()));
    else
      ((Fl_Slider*)o)->slider_size(int(v));

  } else if (!strcmp(c, "extra_code")) {
    extra_code(read_word());
  } else if (!strncmp(c,"code",4)) {
    // for back-compatabilty all old code0, code1, ... fields are concatenated
    const char* newtext = read_word();
    const char* oldtext = extra_code();
    if (!oldtext) {extra_code(newtext); return;}
    int n = strlen(oldtext)+1+strlen(newtext)+1;
    char* buffer = new char[n];
    sprintf(buffer, "%s\n%s", oldtext, newtext);
    extra_code(buffer);
    delete[] buffer;
  } else {
    Fl_Type::read_property(c);
  }
}

static const Enumeration boxmenu1[] = {
  // these extra ones are for looking up fdesign saved strings:
  {"x", "NO_FRAME",		(void *)FL_NO_BOX},
  {"x", "ROUNDED3D_UPBOX",	(void *)FL_ROUND_UP_BOX},
  {"x", "ROUNDED3D_DOWNBOX",	(void *)FL_ROUND_DOWN_BOX},
  {"x", "OVAL3D_UPBOX",		(void *)FL_ROUND_UP_BOX},
  {"x", "OVAL3D_DOWNBOX",	(void *)FL_ROUND_DOWN_BOX},
  {"x", "0",			(void *)FL_NO_BOX},
  {"x", "1",			(void *)FL_UP_BOX},
  {"x", "2",			(void *)FL_DOWN_BOX},
  {"x", "3",			(void *)FL_FLAT_BOX},
  {"x", "4",			(void *)FL_BORDER_BOX},
  {"x", "5",			(void *)FL_SHADOW_BOX},
  {"x", "6",			(void *)FL_ENGRAVED_BOX},
  {"x", "7",			(void *)FL_ROUNDED_BOX},
  {"x", "8",			(void *)FL_RFLAT_BOX},
  {"x", "9",			(void *)FL_RSHADOW_BOX},
  {"x", "10",			(void *)FL_UP_BOX},
  {"x", "11",			(void *)FL_DOWN_BOX},
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
      extra_code(buf);
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
      int v1 = strtol(value,0,0); if (v1 <= 0 && strcmp(value,"0")) return 0;
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
    const Enumeration* e = from_text(value, boxmenu);
    if (!e) {e = from_text(value, boxmenu1); if (!e) return 0;}
    Fl_Boxtype x = (Fl_Boxtype)(e->compiled);
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
// End of "$Id: Fl_Widget_Type.cxx,v 1.87 2002/05/06 06:31:26 spitzak Exp $".
//
