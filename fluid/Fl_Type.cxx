//
// "$Id$"
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

// Widget type code for the Fast Light Tool Kit (FLTK).
//
// Each object described by Fluid is one of these objects.  They
// are all stored in a double-linked list.
//
// They "type" of the object is covered by the virtual functions.
// There will probably be a lot of these virtual functions.
//
// The type browser is also a list of these objects, but they
// are "factory" instances, not "real" ones.  These objects exist
// only so the "make" method can be called on them.  They are
// not in the linked list and are not written to files or
// copied or otherwise examined.

#include "fluid.h"

#include "Fl_Type.h"
#include "Fl_Function_Type.h"
#include "Fl_Widget_Type.h"
#include "WidgetBrowser.h"
#include "WorkspaceType.h"
#include "workspace_panel.h"
#include "undo.h"
#include "code.h"
#include "file.h"

#include <fltk3/run.h>
#include <fltk3/ask.h>
#include <fltk3/Browser_.h>
#include <fltk3/MenuWindow.h>
#include <fltk3/draw.h>
#include <fltk3/filename.h>
#include <stdlib.h>
#include "../src/fltk3/flstring.h"
#include <stdio.h>


Fl_Type *Fl_Type::first;
Fl_Type *Fl_Type::last;
Fl_Type *in_this_only; // set if menu popped-up in window


static void fixvisible(Fl_Type *p) {
  Fl_Type *t = p;
  for (;;) {
    if (t->parent) t->visible = t->parent->visible && t->parent->open_;
    else t->visible = 1;
    t = t->next;
    if (!t || t->level <= p->level) break;
  }
}


void select_all_cb(fltk3::Widget *,void *) {
  Fl_Type *p = Fl_Type::current ? Fl_Type::current->parent : 0;
  if (in_this_only) {
    Fl_Type *t = p;
    for (; t && t != in_this_only; t = t->parent);
    if (t != in_this_only) p = in_this_only;
  }
  for (;;) {
    if (p) {
      int foundany = 0;
      for (Fl_Type *t = p->next; t && t->level>p->level; t = t->next) {
	if (!t->new_selected) {widget_browser->select(t,1,0); foundany = 1;}
      }
      if (foundany) break;
      p = p->parent;
    } else {
      for (Fl_Type *t = Fl_Type::first; t; t = t->next)
	widget_browser->select(t,1,0);
      break;
    }
  }
  selection_changed(p);
}


void select_none_cb(fltk3::Widget *,void *) {
  Fl_Type *p = Fl_Type::current ? Fl_Type::current->parent : 0;
  if (in_this_only) {
    Fl_Type *t = p;
    for (; t && t != in_this_only; t = t->parent);
    if (t != in_this_only) p = in_this_only;
  }
  for (;;) {
    if (p) {
      int foundany = 0;
      for (Fl_Type *t = p->next; t && t->level>p->level; t = t->next) {
	if (t->new_selected) {widget_browser->select(t,0,0); foundany = 1;}
      }
      if (foundany) break;
      p = p->parent;
    } else {
      for (Fl_Type *t = Fl_Type::first; t; t = t->next)
	widget_browser->select(t,0,0);
      break;
    }
  }
  selection_changed(p);
}


static void delete_children(Fl_Type *p) {
  Fl_Type *f;
  for (f = p; f && f->next && f->next->level > p->level; f = f->next);
  for (; f != p; ) {
    Fl_Type *g = f->prev;
    delete f;
    f = g;
  }
}


void delete_all(int selected_only) {
  for (Fl_Type *f = Fl_Type::first; f;) {
    if (f->selected || !selected_only) {
      delete_children(f);
      Fl_Type *g = f->next;
      delete f;
      f = g;
    } else f = f->next;
  }
  if(!selected_only) {
    include_H_from_C=1;
    use_FL_COMMAND=0;
  }
  
  selection_changed(0);
}


// move selected widgets in their parent's list:
void earlier_cb(fltk3::Widget*,void*) {
  Fl_Type *f;
  int mod = 0;
  for (f = Fl_Type::first; f; ) {
    Fl_Type* nxt = f->next;
    if (f->selected) {
      Fl_Type* g;
      for (g = f->prev; g && g->level > f->level; g = g->prev);
      if (g && g->level == f->level && !g->selected) {
        f->move_before(g);
        mod = 1;
      }
    }
    f = nxt;
  }
  if (mod) set_modflag(1);
}


void later_cb(fltk3::Widget*,void*) {
  Fl_Type *f;
  int mod = 0;
  for (f = Fl_Type::last; f; ) {
    Fl_Type* prv = f->prev;
    if (f->selected) {
      Fl_Type* g;
      for (g = f->next; g && g->level > f->level; g = g->next);
      if (g && g->level == f->level && !g->selected) {
        g->move_before(f);
        mod = 1;
      }
    }
    f = prv;
  }
  if (mod) set_modflag(1);
}


// update a string member:
int storestring(const char *n, const char * & p, int nostrip) {
  if (n == p) return 0;
  undo_checkpoint();
  int length = 0;
  if (n) { // see if blank, strip leading & trailing blanks
    if (!nostrip) while (isspace(*n)) n++;
    const char *e = n + strlen(n);
    if (!nostrip) while (e > n && isspace(*(e-1))) e--;
    length = e-n;
    if (!length) n = 0;
  }    
  if (n == p) return 0;
  if (n && p && !strncmp(n,p,length) && !p[length]) return 0;
  if (p) free((void *)p);
  if (!n || !*n) {
    p = 0;
  } else {
    char *q = (char *)malloc(length+1);
    strlcpy(q,n,length+1);
    p = q;
  }
  set_modflag(1);
  return 1;
}


/**
 * Make sure that the given item is visible in the browser by opening
 * all parent groups and moving the item into the visible space.
 */
void reveal_in_browser(Fl_Type *t) {
  Fl_Type *p = t->parent;
  if (p) {
    for (;;) {
      if (!p->open_)
        p->open_ = 1;
      if (!p->parent) break;
      p = p->parent;
    }
    fixvisible(p);
  }
  widget_browser->display(t);
  redraw_browser();
}


/**
 * Return 1 if the list contains a function with the given signature at the top level
 */
int has_toplevel_function(const char *rtype, const char *sig) {
  Fl_Type *child;
  for (child = Fl_Type::first; child; child = child->next) {
    if (!child->is_in_class() && strcmp(child->type_name(), "Function")==0) {
      const Fl_Function_Type *fn = (const Fl_Function_Type*)child;
      if (fn->has_signature(rtype, sig))
        return 1;
    }
  }
  return 0;
}



/*
 Implementation of class Fl_Type
 ===============================================================================
 */


Fl_Type::Fl_Type() {
  factory = 0;
  parent = 0;
  next = prev = 0;
  selected = new_selected = 0;
  visible = 0;
  name_ = 0;
  label_ = 0;
  user_data_ = 0;
  user_data_type_ = 0;
  callback_ = 0;
  comment_ = 0;
  rtti = 0;
  level = 0;
  code_position = header_position = -1;
  code_position_end = header_position_end = -1;
}


// Generate a descriptive text for this item, to put in browser & window titles
const char* Fl_Type::title() {
  const char* c = name(); 
  if (c) 
    return c;
  return type_name();
}


Fl_Type *Fl_Type::first_child() {
  if (next && next->level==this->level+1) {
    return next;
  } else {
    return 0L;
  }
}


Fl_Type *Fl_Type::next_brother() {
  Fl_Type *n = next;
  for (;;) {
    if (!n || n->level>=this->level) return 0L;
    if (n->level==this->level) return n;
    n = n->next;
  }
}


// turn a click at x,y on this into the actual picked object:
Fl_Type* Fl_Type::click_test(int,int) 
{
  return 0;
}


void Fl_Type::add_child(Fl_Type*, Fl_Type*) 
{
}


void Fl_Type::move_child(Fl_Type*, Fl_Type*) 
{
}


void Fl_Type::remove_child(Fl_Type*) 
{
}


// add a list of widgets as a new child of p:
void Fl_Type::add(Fl_Type *p) {
  if (p && parent == p) return;
  undo_checkpoint();
  parent = p;
  Fl_Type *end = this;
  while (end->next) end = end->next;
  Fl_Type *q;
  int newlevel;
  if (p) {
    for (q = p->next; q && q->level > p->level; q = q->next);
    newlevel = p->level+1;
  } else {
    q = 0;
    newlevel = 0;
  }
  for (Fl_Type *t = this->next; t; t = t->next) t->level += (newlevel-level);
  level = newlevel;
  if (q) {
    prev = q->prev;
    prev->next = this;
    q->prev = end;
    end->next = q;
  } else if (first) {
    prev = last;
    prev->next = this;
    end->next = 0;
    last = end;
  } else {
    first = this;
    last = end;
    prev = end->next = 0;
  }
  if (p) p->add_child(this,0);
  open_ = 1;
  fixvisible(this);
  set_modflag(1);
  widget_browser->redraw();
}


// add to a parent before another widget:
void Fl_Type::insert(Fl_Type *g) {
  Fl_Type *end = this;
  while (end->next) end = end->next;
  parent = g->parent;
  int newlevel = g->level;
  visible = g->visible;
  for (Fl_Type *t = this->next; t; t = t->next) t->level += newlevel-level;
  level = newlevel;
  prev = g->prev;
  if (prev) prev->next = this; else first = this;
  end->next = g;
  g->prev = end;
  fixvisible(this);
  if (parent) parent->add_child(this, g);
  widget_browser->redraw();
}


// Return message number for I18N...
int Fl_Type::msgnum() {
  int		count;
  Fl_Type	*p;

  for (count = 0, p = this; p;) {
    if (p->label()) count ++;
    if (p != this && p->is_widget() && ((Fl_Widget_Type *)p)->tooltip()) count ++;

    if (p->prev) p = p->prev;
    else p = p->parent;
  }

  return count;
}


// delete from parent:
Fl_Type *Fl_Type::remove() {
  Fl_Type *end = this;
  for (;;) {
    if (!end->next || end->next->level <= level) break;
    end = end->next;
  }
  if (prev) prev->next = end->next;
  else first = end->next;
  if (end->next) end->next->prev = prev;
  else last = prev;
  Fl_Type *r = end->next;
  prev = end->next = 0;
  if (parent) parent->remove_child(this);
  parent = 0;
  widget_browser->redraw();
  selection_changed(0);
  return r;
}


void Fl_Type::name(const char *n) {
  int nostrip = is_comment();
  if (storestring(n,name_,nostrip)) {
    if (visible) widget_browser->redraw();
  }
}


void Fl_Type::label(const char *n) {
  if (storestring(n,label_,1)) {
    setlabel(label_);
    if (visible && !name_) widget_browser->redraw();
  }
}


void Fl_Type::callback(const char *n) {
  storestring(n,callback_);
}


void Fl_Type::user_data(const char *n) {
  storestring(n,user_data_);
}


void Fl_Type::user_data_type(const char *n) {
  storestring(n,user_data_type_);
}


void Fl_Type::comment(const char *n) {
  storestring(n, comment_, 1);
}


void Fl_Type::open() {
  printf("Open of '%s' is not yet implemented\n",type_name());
}


void Fl_Type::setlabel(const char *) 
{
}


Fl_Type::~Fl_Type() {
  // warning: destructor only works for widgets that have been add()ed.
  if (widget_browser) widget_browser->deleting(this);
  if (prev) prev->next = next; else first = next;
  if (next) next->prev = prev; else last = prev;
  if (current == this) current = 0;
  if (parent) parent->remove_child(this);
  if (name_) free((void*)name_);
  if (label_) free((void*)label_);
  if (callback_) free((void*)callback_);
  if (user_data_) free((void*)user_data_);
  if (user_data_type_) free((void*)user_data_type_);
  if (comment_) free((void*)comment_);
  if (Fl_Panel::selected_type()==this) {
    Fl_Panel::select_type(0);
  }
}


int Fl_Type::is_parent() const {return 0;}
int Fl_Type::is_widget() const {return 0;}
int Fl_Type::is_valuator() const {return 0;}
int Fl_Type::is_spinner() const {return 0;}
int Fl_Type::is_button() const {return 0;}
int Fl_Type::is_input() const {return 0;}
int Fl_Type::is_value_input() const {return 0;}
int Fl_Type::is_text_display() const {return 0;}
int Fl_Type::is_menu_item() const {return 0;}
int Fl_Type::is_menu_button() const {return 0;}
int Fl_Type::is_group() const {return 0;}
int Fl_Type::is_window() const {return 0;}
int Fl_Type::is_code_block() const {return 0;}
int Fl_Type::is_decl_block() const {return 0;}
int Fl_Type::is_comment() const {return 0;}
int Fl_Type::is_class() const {return 0;}
int Fl_Type::is_public() const {return 1;}


// move f (and it's children) into list before g:
// returns pointer to whatever is after f & children
void Fl_Type::move_before(Fl_Type* g) {
  if (level != g->level) printf("move_before levels don't match! %d %d\n",
				level, g->level);
  Fl_Type* n;
  for (n = next; n && n->level > level; n = n->next);
  if (n == g) return;
  Fl_Type *l = n ? n->prev : Fl_Type::last;
  prev->next = n;
  if (n) n->prev = prev; else Fl_Type::last = prev;
  prev = g->prev;
  l->next = g;
  if (prev) prev->next = this; else Fl_Type::first = this;
  g->prev = l;
  if (parent && is_widget()) parent->move_child(this,g);
  widget_browser->inserting(g, this);
  widget_browser->display(this);
  widget_browser->redraw();
}


// write a widget and all it's children:
void Fl_Type::write() {
    write_indent(level);
    write_word(type_name());
    
    if (is_class()) {
      const char * p = 	((Fl_Class_Type*)this)->prefix();
      if (p &&	strlen(p))
        write_word(p);
    }

    write_word(name());
    write_open(level);
    write_properties();
    write_close(level);
    if (!is_parent()) return;
    // now do children:
    write_open(level);
    Fl_Type *child;
    for (child = next; child && child->level > level; child = child->next)
	if (child->level == level+1) child->write();
    write_close(level);
}


void Fl_Type::write_properties() {
  // repeat this for each attribute:
  if (label()) {
    write_indent(level+1);
    write_word("label");
    write_word(label());
  }
  if (user_data()) {
    write_indent(level+1);
    write_word("user_data");
    write_word(user_data());
  }
  if (user_data_type()) {
    write_word("user_data_type");
    write_word(user_data_type());
  }
  if (callback()) {
    write_indent(level+1);
    write_word("callback");
    write_word(callback());
  }
  if (comment()) {
    write_indent(level+1);
    write_word("comment");
    write_word(comment());
  }
  if (is_parent() && open_) write_word("open");
  if (selected) write_word("selected");
}


char Fl_Type::read_property(const char *c) {
  if (!strcmp(c,"label"))
    label(read_word());
  else if (!strcmp(c,"user_data"))
    user_data(read_word());
  else if (!strcmp(c,"user_data_type"))
    user_data_type(read_word());
  else if (!strcmp(c,"callback"))
    callback(read_word());
  else if (!strcmp(c,"comment"))
    comment(read_word());
  else if (!strcmp(c,"open"))
    open_ = 1;
  else if (!strcmp(c,"selected"))
    select(this,1);
  else {
    read_error("Unknown property \"%s\"", c);
    return 0;
  }
  return 1;
}


/**
 * Write a comment into the header file.
 */
void Fl_Type::write_comment_h(const char *pre)
{
  if (comment()) {
    write_h("%s/**\n", pre);
    const char *s = comment();
    write_h("%s   ", pre);
    while(*s) {
      if (*s=='\n') {
        if (s[1]) {
          write_h("\n%s   ", pre);
        }
      } else {
        write_h("%c", *s);
      }
      s++;
    }
    write_h("\n%s*/\n", pre);
  }
}


/**
 * Write a comment into the source file.
 */
void Fl_Type::write_comment_c(const char *pre)
{
  if (comment()) {
    write_c("%s/**\n", pre);
    const char *s = comment();
    write_c("%s   ", pre);
    while(*s) {
      if (*s=='\n') {
        if (s[1]) {
          write_c("\n%s   ", pre);
        }
      } else {
        write_c("%c", *s);
      }
      s++;
    }
    write_c("\n%s*/\n", pre);
  }
}


/**
 * Build widgets and dataset needed in live mode.
 * \return a widget pointer that the live mode initiator can 'show()'
 * \see leave_live_mode()
 */
fltk3::Widget *Fl_Type::enter_live_mode(int) {
  return 0L;
}


/**
 * Release all resources created when enetring live mode.
 * \see enter_live_mode()
 */
void Fl_Type::leave_live_mode() {
}


/**
 * Copy all needed properties for this tye into the live object.
 */
void Fl_Type::copy_properties() {
}


/**
 * Check whether callback name is declared anywhere else by the user
 *
 * \b Warning: this just checks that the name is declared somewhere,
 * but it should probably also check that the name corresponds to a
 * plain function or a member function within the same class and that
 * the parameter types match.
 */
int Fl_Type::user_defined(const char* cbname) const {
  for (Fl_Type* p = Fl_Type::first; p ; p = p->next)
    if (strcmp(p->type_name(), "Function") == 0 && p->name() != 0)
      if (strncmp(p->name(), cbname, strlen(cbname)) == 0)
        if (p->name()[strlen(cbname)] == '(')
          return 1;
  return 0;
}


void Fl_Type::write_static() 
{
}


void Fl_Type::write_code1() 
{
  write_h("// Header for %s\n", title());
  write_c("// Code for %s\n", title());
}


void Fl_Type::write_code2() 
{
}

const char* Fl_Type::class_name(const int need_nest) const {
  Fl_Type* p = parent;
  while (p) {
    if (p->is_class()) {
      // see if we are nested in another class, we must fully-qualify name:
      // this is lame but works...
      const char* q = 0;
      if(need_nest) q=p->class_name(need_nest);
      if (q) {
	static char s[256];
	if (q != s) strlcpy(s, q, sizeof(s));
	strlcat(s, "::", sizeof(s));
	strlcat(s, p->name(), sizeof(s));
	return s;
      }
      return p->name();
    }
    p = p->parent;
  }
  return 0;
}

/**
 * If this Type resides inside a class, this function returns the class type, or null.
 */
const Fl_Class_Type *Fl_Type::is_in_class() const {
  Fl_Type* p = parent;
  while (p) {
    if (p->is_class()) {
      return (Fl_Class_Type*)p;
    }
    p = p->parent;
  }
  return 0;
}



//
// End of "$Id$".
//
