//
// "$Id: Fl_Group_Type.cxx,v 1.13 2001/02/21 23:26:55 robertk Exp $"
//
// Fl_Group object code for the Fast Light Tool Kit (FLTK).
//
// Object describing an Fl_Group and links to Fl_Window_Type.C and
// the Fl_Tabs widget, with special stuff to select tab items and
// insure that only one is visible.
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
#include <FL/Fl_Group.H>
#include <FL/fl_message.H>
#include "Fl_Type.h"

class igroup : public Fl_Group {
public:
  igroup(int x,int y,int w,int h) : Fl_Group(x,y,w,h) {
    Fl_Group::current(0);
    resizable(0);
  }
};

Fl_Widget_Type* Fl_Group_Type::_make() {return new Fl_Group_Type();}

Fl_Group_Type::~Fl_Group_Type() {
  for (Fl_Type* q = first_child; q; q = q->next_brother) {
	  remove_child(q);
	  if(q->is_widget() && (((Fl_Widget_Type *)q)->o->parent() == o)) {
		  ((Fl_Widget_Type *)q)->o->parent(0);
		  // Fl_Group destructor will delete all its children!!
		  ((Fl_Widget_Type *)q)->o = NULL;	
	  }
  }
}

Fl_Widget *Fl_Group_Type::widget(int x,int y,int w,int h) {
  igroup *g = new igroup(x,y,w,h);
  Fl_Group::current(0);
  return g;
}

const char* Fl_Group_Type::type_name() {return "Fl_Group";}

int Fl_Group_Type::is_parent() const {return 1;}
int Fl_Group_Type::is_group() const {return 1;}

Fl_Group_Type Fl_Group_type;	// the "factory"

Fl_Type *Fl_Group_Type::make() {
  return Fl_Widget_Type::make();
}

// Enlarge the group to surround all it's children.  This is done to
// all groups whenever the user moves any widgets.
void fix_group_size(Fl_Type *t) {
  if (!t || !t->is_group() || t->is_menu_button()) return;
  Fl_Group* g = (Fl_Group*)((Fl_Group_Type*)t)->o;
  //if (g->resizable()) return;
  int X = 0;
  int Y = 0;
  int R = g->w();
  int B = g->h();
  for (Fl_Type *nn = t->first_child; nn; nn = nn->next_brother) {
    if (nn->is_widget()) {
      Fl_Widget* o = ((Fl_Widget_Type*)nn)->o;
      int x = o->x();  if (x < X) X = x;
      int y = o->y();  if (y < Y) Y = y;
      int r = x+o->w();if (r > R) R = r;
      int b = y+o->h();if (b > B) B = b;
    }
  }
  g->resize(g->x()+X,g->y()+Y,R-X,B-Y);
  if (X || Y) {
    for (Fl_Type *nn = t->first_child; nn; nn = nn->next_brother) {
      if (nn->is_widget()) {
	Fl_Widget* o = ((Fl_Widget_Type*)nn)->o;
	o->x(o->x()-X);
	o->y(o->y()-Y);
      }
    }
  }
  g->init_sizes();
}

extern int force_parent;

void group_cb(Fl_Widget *, void *) {
  // Find the current widget:
  Fl_Type *qq = Fl_Type::current;
  while (qq && (!qq->is_widget() || qq->is_menu_item())) qq = qq->parent;
  if (!qq || !qq->parent || !qq->parent->is_widget()) {
    fl_message("Please select widgets to group");
    return;
  }
  Fl_Widget_Type* q = (Fl_Widget_Type*)qq;
  force_parent = 1;
  Fl_Group_Type *n = (Fl_Group_Type*)(Fl_Group_type.make());
  n->move_before(q);
  n->o->resize(0,0,0,0);
  for (Fl_Type *t = q->parent->first_child; t;) {
    Fl_Type* next = t->next_brother;
    if (t->selected && t != n) {
      t->remove();
      t->add(n);
    }
    t = next;
  }
  fix_group_size(n);
}

void ungroup_cb(Fl_Widget *, void *) {
  // Find the group:
  Fl_Type *q = Fl_Type::current;
  while (q && (!q->is_widget() || q->is_menu_item())) q = q->parent;
  if (q) q = q->parent;
  if (!q || !q->parent->is_widget()) {
    fl_message("Please select widgets in a group");
    return;
  }
  for (Fl_Type* n = q->first_child; n;) {
    Fl_Type* next = n->next_brother;
    if (n->selected) {
      n->remove();
      n->insert(q);
    }
    n = next;
  }
  if (!q->first_child) delete q;
}

////////////////////////////////////////////////////////////////

#include <stdio.h>

void Fl_Group_Type::write_code() {
  write_code1();
  for (Fl_Type* q = first_child; q; q = q->next_brother) q->write_code();
  write_extra_code();
  write_c("%so->end();\n", indent());
  if (resizable()) write_c("%sFl_Group::current()->resizable(o);\n", indent());
  write_block_close();
}

////////////////////////////////////////////////////////////////

#include <FL/Fl_Pack.H>

static Fl_Menu_Item pack_type_menu[] = {
  {"HORIZONTAL", 0, 0, (void*)Fl_Pack::HORIZONTAL},
  {"VERTICAL", 0, 0, (void*)Fl_Pack::VERTICAL},
  {0}};

class Fl_Pack_Type : public Fl_Group_Type {
  Fl_Menu_Item *subtypes() {return pack_type_menu;}
public:
  virtual const char *type_name() {return "Fl_Pack";}
  Fl_Widget_Type *_make() {return new Fl_Pack_Type();}
};

Fl_Pack_Type Fl_Pack_type;	// the "factory"

////////////////////////////////////////////////////////////////

#include <FL/Fl_Tabs.H>

class itabs : public Fl_Tabs {
public:
  itabs(int x,int y,int w,int h) : Fl_Tabs(x,y,w,h) {
    Fl_Group::current(0);
    resizable(0);
  }
};

class Fl_Tabs_Type : public Fl_Group_Type {
public:
  virtual const char *type_name() {return "Fl_Tabs";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    itabs *g = new itabs(x,y,w,h); Fl_Group::current(0); return g;}
  Fl_Widget_Type *_make() {return new Fl_Tabs_Type();}
  Fl_Type* click_test(int,int);
  void add_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);
};

Fl_Tabs_Type Fl_Tabs_type;	// the "factory"

// This is called when user clicks on a widget in the window.  See
// if it is a tab title, and adjust visibility and return new selection:
// If none, return o unchanged:

Fl_Type* Fl_Tabs_Type::click_test(int x, int y) {
  Fl_Tabs *t = (Fl_Tabs*)o;
  Fl_Widget *a = t->which(x,y);
  if (!a) return 0; // didn't click on tab
  // okay, run the tabs ui until they let go of mouse:
  t->handle(FL_PUSH);
  Fl::pushed(t);
  while (Fl::pushed()==t) Fl::wait();
  return (Fl_Type*)(t->value()->user_data());
}

// This is called when o is created.  If it is in the tab group make
// sure it is visible:

void Fl_Group_Type::add_child(Fl_Type* cc, Fl_Type* before) {
  Fl_Widget_Type* c = (Fl_Widget_Type*)cc;
  Fl_Widget* b = before ? ((Fl_Widget_Type*)before)->o : 0;
  ((Fl_Group*)o)->insert(*(c->o), b);
  o->redraw();
}

void Fl_Tabs_Type::add_child(Fl_Type* c, Fl_Type* before) {
  Fl_Group_Type::add_child(c, before);
}

// This is called when o is deleted.  If it is in the tab group make
// sure it is not visible:

void Fl_Group_Type::remove_child(Fl_Type* cc) {
  Fl_Widget_Type* c = (Fl_Widget_Type*)cc;
  ((Fl_Group*)o)->remove(c->o);
  o->redraw();
}

void Fl_Tabs_Type::remove_child(Fl_Type* cc) {
  Fl_Widget_Type* c = (Fl_Widget_Type*)cc;
  Fl_Tabs *t = (Fl_Tabs*)o;
  if (t->value() == c->o) t->value(0);
  Fl_Group_Type::remove_child(c);
}

// move, don't change selected value:

void Fl_Group_Type::move_child(Fl_Type* cc, Fl_Type* before) {
  Fl_Widget_Type* c = (Fl_Widget_Type*)cc;
  Fl_Widget* b = before ? ((Fl_Widget_Type*)before)->o : 0;
  ((Fl_Group*)o)->remove(c->o);
  ((Fl_Group*)o)->insert(*(c->o), b);
  o->redraw();
}

////////////////////////////////////////////////////////////////
// some other group subclasses that fluid does not treat specially:

#include <FL/Fl_Scroll.H>

static Fl_Menu_Item scroll_type_menu[] = {
  {"BOTH", 0, 0, 0/*(void*)Fl_Scroll::BOTH*/},
  {"HORIZONTAL", 0, 0, (void*)Fl_Scroll::HORIZONTAL},
  {"VERTICAL", 0, 0, (void*)Fl_Scroll::VERTICAL},
  {"HORIZONTAL_ALWAYS", 0, 0, (void*)Fl_Scroll::HORIZONTAL_ALWAYS},
  {"VERTICAL_ALWAYS", 0, 0, (void*)Fl_Scroll::VERTICAL_ALWAYS},
  {"BOTH_ALWAYS", 0, 0, (void*)Fl_Scroll::BOTH_ALWAYS},
  {0}};

class Fl_Scroll_Type : public Fl_Group_Type {
  Fl_Menu_Item *subtypes() {return scroll_type_menu;}
public:
  virtual const char *type_name() {return "Fl_Scroll";}
  Fl_Widget_Type *_make() {return new Fl_Scroll_Type();}
};

Fl_Scroll_Type Fl_Scroll_type;	// the "factory"

////////////////////////////////////////////////////////////////

class Fl_Tile_Type : public Fl_Group_Type {
public:
  virtual const char *type_name() {return "Fl_Tile";}
  Fl_Widget_Type *_make() {return new Fl_Tile_Type();}
};

Fl_Tile_Type Fl_Tile_type;	// the "factory"

//
// End of "$Id: Fl_Group_Type.cxx,v 1.13 2001/02/21 23:26:55 robertk Exp $".
//
