//
// "$Id$"
//
// Widget type code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

// Each object created by Fluid is a subclass of FluidType. The majority
// of these are going to describe fltk::Widgets, so you will see the word
// "widget" used a lot instead of FluidType. But there are also functions
// and lines of code and anything else that can go into the browser.
//
// A hierarchial list of all FluidTypes is managed. The Widget Browser
// is the display in the main window of this list. Most of this code
// is concerned with drawing and updating the widget browser, with
// keeping the list up to date and rearranging it, and keeping track
// of which objects are selected.
//
// The "Type Browser" is also a list of FluidType, but is used for the
// popup menu of new objects to create. In this case these are
// "factory instances", not "real" ones.  Factory instances exist only
// so the "make" method can be called on them.  They are not in the
// linked list and are not written to files or copied or otherwise
// examined.

#include <fltk/run.h>
#include <fltk/MultiBrowser.h>
#include <fltk/Item.h>
#include <fltk/draw.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/filename.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
# define strcasecmp(a,b) stricmp(a,b)
# define strncasecmp(a,b,c) strnicmp(a,b,c)
#endif
#include <stdio.h>

#include "FluidType.h"
#include "Fluid_Image.h"

////////////////////////////////////////////////////////////////
// Code to access lists of enuerations:

const Enumeration* from_value(void* data, const Enumeration* table)
{
  for (;table->menu_entry; table++)
    if (table->compiled == data) return table;
  return 0;
}

const Enumeration* from_value(int data, const Enumeration* table)
{
  for (;table->menu_entry; table++)
    if (int((long)(table->compiled)) == data) return table;
  return 0;
}

const Enumeration* from_text(const char* text, const Enumeration* table)
{
  // For back compatability we strip leading fltk:: from symbols:
  if (text[0]=='F' && text[1]=='L' && text[2]=='_') text += 3;
  for (;table->menu_entry; table++) {
    if (table->symbol && !strcmp(table->symbol, text)) return table;
    // also for back compatability we match the menu entry and subclass:
    if (!strcasecmp(table->menu_entry, text)) return table;
    if (table->subclass && !strcmp(table->subclass, text)) return table;
  }
  return 0;
}

const char* to_text(void* data, const Enumeration* table)
{
  for (;table->menu_entry; table++) {
    if (table->compiled == data) {
      if (table->symbol && !table->subclass) return table->symbol;
      return table->menu_entry;
    }
  }
  return 0;
}

int number_from_text(const char* text, const Enumeration* table)
{
  if (table) {
    const Enumeration* t = from_text(text, table);
    if (t) return int((long)(t->compiled));
  }
  return strtol(text, 0, 0);
}

const char* number_to_text(int number, const Enumeration* table)
{
  if (table) {
    const char* t = to_text((void*)number, table);
    if (t) return t;
  }
  static char buffer[20];
  sprintf(buffer, "%d", number);
  return buffer;
}

////////////////////////////////////////////////////////////////
// Code to translate a table of Enumeration entries into fltk::Menu items:

class Enumeration_List : public fltk::List {
  virtual int children(const fltk::Menu*, const int* indexes, int level);
  virtual fltk::Widget* child(const fltk::Menu*, const int* indexes, int level);
  public: 
    virtual ~Enumeration_List() {}
};

static Enumeration_List enumeration_list;

void set_menu(fltk::Menu* menu, const Enumeration* list) {
  menu->list(&enumeration_list);
  menu->user_data((void*)list);
}

int Enumeration_List::children(const fltk::Menu* menu, const int*, int level)
{
  if (level) return -1;
  const Enumeration* e = (const Enumeration*)(menu->user_data());
  int n = 0;
  while (e->menu_entry) {n++; e++;}
  return n;
}

fltk::Widget* Enumeration_List::child(const fltk::Menu* menu, const int* indexes, int)
{
  const Enumeration* e = (const Enumeration*)(menu->user_data());
  int n = *indexes;
  while (n && e->menu_entry) {n--; e++;}
  if (!e->menu_entry) return 0;
  static fltk::Widget* widget;
  if (!widget) {
    fltk::Group::current(0);
    widget = new fltk::Item();
  }
  widget->user_data((void*)e);
  widget->label(e->menu_entry);
  widget->w(0);
  widget->h(0);
  return widget;
}

////////////////////////////////////////////////////////////////

FluidType *FluidType::first;
FluidType *FluidType::current;

class Widget_List : public fltk::List {
  virtual int children(const fltk::Menu*, const int* indexes, int level);
  virtual fltk::Widget* child(const fltk::Menu*, const int* indexes, int level);
  virtual void flags_changed(const fltk::Menu*, fltk::Widget*);
  public:
    virtual ~Widget_List() {}
};

static Widget_List widgetlist;

extern fltk::Browser *widget_browser;

extern void deselect();
static void Widget_Browser_callback(fltk::Widget * w,void *) {
    if (fltk::event()==fltk::PUSH )  {
	if ( ( (fltk::Browser*) w)->item()==0) 
	    deselect();
	    widget_browser->redraw();
    }
    else if (fltk::event()==fltk::WHEN_ENTER_KEY || fltk::event_clicks()) { // double_click open the widget editor
	if (FluidType::current) FluidType::current->open();
    }
}

fltk::Widget *make_widget_browser(int x,int y,int w,int h) {
  widget_browser = new fltk::MultiBrowser(x,y,w,h);
  widget_browser->list(&widgetlist);
  widget_browser->callback(Widget_Browser_callback);
  widget_browser->when(fltk::WHEN_ENTER_KEY|fltk::WHEN_CHANGED);
  widget_browser->indented(1);
  return widget_browser;
}

int Widget_List::children(const fltk::Menu*, const int* indexes, int level) {
  FluidType* item = FluidType::first;
  if (!item) return 0;
  for (int l = 0; l < level; l++) {
    for (int i = indexes[l]; item && i; --i) item = item->next_brother;
    if (!item || !item->is_parent()) return -1;
    item = item->first_child;
  }
  int n; for (n = 0; item; item = item->next_brother) n++;
  return n;
}

static const char * get_item_fullname(FluidType* item) {
    static char buffer[PATH_MAX];
    if (strcmp(item->type_name(),"namespace") ==0) 
	sprintf(buffer, "%s %s", "namespace", item->title());
    else if (strcmp(item->type_name(),"class") ==0) 
	sprintf(buffer, "%s %s", "class", item->title());
    else 
	strncpy(buffer, item->title(), sizeof(buffer));
    return buffer;
}

fltk::Widget* Widget_List::child(const fltk::Menu*, const int* indexes, int level) {
  FluidType* item = FluidType::first;
  if (!item) return 0;
  for (int l = 0;; l++) {
    for (int i = indexes[l]; item && i; --i) item = item->next_brother;
    if (!item) return 0;
    if (l >= level) break;
    item = item->first_child;
  }
  static fltk::Widget* widget;
  if (!widget) {
    fltk::Group::current(0);
    widget = new fltk::Item();
  }
  widget->user_data(item);
  if (item->selected) widget->set_selected();
  else widget->clear_selected();
  // force the hierarchy to be open/closed:
  if (item->is_parent() && item->open_) widget->set_flag(fltk::VALUE);
  else widget->clear_flag(fltk::VALUE);

  widget->label(get_item_fullname(item));
  widget->w(0);
  widget->h(0);

  return widget;
}

void Widget_List::flags_changed(const fltk::Menu*, fltk::Widget* w) {
  FluidType* item = (FluidType*)(w->user_data());
  item->open_ = (w->flags()&fltk::VALUE) != 0;
  item->new_selected = w->selected();
  if (item->new_selected != item->selected) selection_changed(item);
}

void select(FluidType* it, int value) {
  it->new_selected = value != 0;
  if (it->new_selected != it->selected) {
    selection_changed(it);
    widget_browser->goto_focus();
    widget_browser->redraw();
  }
}

void select_only(FluidType* i) {
  for (FluidType* item = FluidType::first; item; item = item->walk())
    select(item, item == i);
  if (!widget_browser || !i) return;
  int indexes[100];
  int L = 100;
  while (i) {
    FluidType* child = i->parent ? i->parent->first_child : FluidType::first;
    int n; for (n = 0; child != i; child=child->next_brother) n++;
    indexes[--L] = n;
    i = i->parent;
  }
  widget_browser->goto_index(indexes+L, 99-L);
  widget_browser->set_focus();
}

void deselect() {
  for (FluidType* item = FluidType::first; item; item = item->walk())
    select(item,0);
  FluidType::current = 0;
}

// Generate a descriptive text for this item, to put in browser & window
// titles. Warning: the buffer used is overwritten each time!
const char* FluidType::title() {
#define MAXLABEL 128
  static char buffer[MAXLABEL];
  const char* t1 = type_name();
  const char* type = 0;
  if (is_widget()) type = t1 = ((WidgetType*)this)->subclass();
  const char* name = this->name();
  bool quoted = false;
  if (!name || !*name) {
    name = label();
    if (!name || !*name) return t1;
    quoted = true;
  }
  // copy but stop at any newline or when the buffer fills up:
  char* e = buffer+MAXLABEL-1; if (quoted) e--;
  char* p = buffer;
  if (type) {
    while (p < e && *type) *p++ = *type++;
    if (p >= e-4) return name;
    *p++ = ' ';
  }
  if (quoted) *p++ = '"';
  while (p < e && (*name&~31)) *p++ = *name++;
  if (*name) {
    if (p > e-3) p = e-3;
    strcpy(p, quoted ? "...\"" : "...");
  } else {
    if (quoted) *p++ = '"';
    *p++ = 0;
  }
  return buffer;
}

// Call this when the descriptive text changes:
void redraw_browser() {
  widget_browser->redraw();
}

FluidType::FluidType() {
  memset(this, 0, sizeof(FluidType));
  pixID_ = -1;
}

// Calling walk(N) will return every FluidType under N by scanning
// the tree. Start with N->first_child. If N is null this will
// walk the entire tree, start with FluidType::first.

FluidType* FluidType::walk(const FluidType* topmost) const {
  if (first_child) return first_child;
  const FluidType* p = this;
  while (!p->next_brother) {
    p = p->parent;
    if (p == topmost) return 0;
  }
  return p->next_brother;
}

// walk() is the same as walk(0), which walks the entire tree:

FluidType* FluidType::walk() const {
  if (first_child) return first_child;
  const FluidType* p = this;
  while (!p->next_brother) {
    p = p->parent;
    if (!p) return 0;
  }
  return p->next_brother;
}

// turn a click at x,y on this into the actual picked object:
FluidType* FluidType::click_test(int,int) {return 0;}
void FluidType::add_child(FluidType*, FluidType*) {}
void FluidType::move_child(FluidType*, FluidType*) {}
void FluidType::remove_child(FluidType*) {}

// add as a new child of p:
void FluidType::add(FluidType *p) {
  if (p && parent == p) return;
  // FIXME UNDO:   undo_checkpoint();
  parent = p;
  // calculate level
  FluidType *end = this;
  while (end->next_brother) end = end->next_brother;
  
  FluidType * q = p ? p->first_child : FluidType::first;
  if (q) {
    // find the last child:
    while (q->next_brother) q = q->next_brother;
    this->previous_brother = q;
    q->next_brother = this;
  } else {
    // no other children
    this->previous_brother = 0;
    if (p)
      p->first_child = this;
    else
      FluidType::first = this;
  }
  if (p) p->add_child(this,0);
  open_ = 1;
  modflag = 1;
  widget_browser->relayout();
}

// add to a parent before another widget:
void FluidType::insert(FluidType *g) {
  parent = g->parent;
  previous_brother = g->previous_brother;
  if (previous_brother) previous_brother->next_brother = this;
  else if (parent) parent->first_child = this;
  else FluidType::first = this;
  next_brother = g;
  g->previous_brother = this;
  if (parent) parent->add_child(this, g);
  widget_browser->relayout();
}

// delete from parent:
void FluidType::remove() {
  if (previous_brother) previous_brother->next_brother = next_brother;
  else if (parent) parent->first_child = next_brother;
  else FluidType::first = next_brother;
  if (next_brother) next_brother->previous_brother = previous_brother;
  previous_brother = next_brother = 0;
  if (parent) parent->remove_child(this);
  parent = 0;
  widget_browser->relayout();
  selection_changed(0);
}

// update a string member:
int storestring(const char *n, const char * & p, int nostrip) {
  if (n == p) return 0;
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
    strncpy(q,n,length);
    q[length] = 0;
    p = q;
  }
  modflag = 1;
  return 1;
}

void FluidType::name(const char *n) {
  if (storestring(n,name_)) widget_browser->redraw();
}

void FluidType::label(const char *n) {
  if (storestring(n,label_,1)) {
    setlabel(label_);
    if (!name_) widget_browser->redraw();
  }
}

void FluidType::tooltip(const char *n) {
  storestring(n,tooltip_,1);
}

void FluidType::callback(const char *n) {
  storestring(n,callback_);
}

void FluidType::user_data(const char *n) {
  storestring(n,user_data_);
}

void FluidType::user_data_type(const char *n) {
  storestring(n,user_data_type_);
}

void FluidType::open() {
  printf("Open of '%s' is not yet implemented\n",type_name());
}

void FluidType::setlabel(const char *) {}

FluidType::~FluidType() {
  for (FluidType* f = first_child; f;) {
    FluidType* next = f->next_brother;
    delete f;
    f = next;
  }
  if (previous_brother) previous_brother->next_brother = next_brother;
  else if (parent) parent->first_child = next_brother;
  else first = next_brother;
  if (next_brother) next_brother->previous_brother = previous_brother;
  if (current == this) current = 0;
  modflag = 1;
  if (widget_browser) widget_browser->relayout();
}

int FluidType::is_parent() const {return 0;}
int FluidType::is_widget() const {return 0;}
int FluidType::is_valuator() const {return 0;}
int FluidType::is_button() const {return 0;}
int FluidType::is_light_button() const {return 0;}
int FluidType::is_menu_item() const {return 0;}
int FluidType::is_menu_button() const {return 0;}
int FluidType::is_group() const {return 0;}
int FluidType::is_window() const {return 0;}
int FluidType::is_code_block() const {return 0;}
int FluidType::is_decl_block() const {return 0;}
int FluidType::is_class() const {return 0;}
int FluidType::is_counter() const {return 0;}
int FluidType::is_adjuster() const {return 0;}
int FluidType::is_slider() const {return 0;}
int FluidType::is_scrollbar() const {return 0;}
int FluidType::is_choice() const {return 0;}
int FluidType::is_browser() const {return 0;}
int FluidType::is_input() const {return 0;}
int FluidType::is_value_input() const {return 0;}
int FluidType::is_value_output() const {return 0;}
int FluidType::is_value_slider() const {return 0;}

////////////////////////////////////////////////////////////////

FluidType *in_this_only; // set if menu popped-up in window

void select_all_cb(fltk::Widget *,void *) {
  FluidType *parent = FluidType::current ? FluidType::current->parent : 0;
  if (in_this_only) {
    // make sure we don't select outside the current window
    FluidType* p;
    for (p = parent; p && p != in_this_only; p = p->parent);
    if (!p) parent = in_this_only;
  }
  for (;;) {
    // select all children of parent:
    int changed = 0;
    for (FluidType *t = parent ? parent->first_child : FluidType::first;
	 t; t = t->walk(parent))
      if (!t->selected) {changed = 1; select(t,1);}
    if (changed) break;
    // if everything was selected, try a higher parent:
    if (!parent || parent == in_this_only) break;
    parent = parent->parent;
  }
}

void delete_all(int selected_only) {
  for (FluidType *f = FluidType::first; f;) {
    if (f->selected || !selected_only) {
      FluidType* next = f->next_brother;
      delete f;
      f = next;
    } else {
      f = f->walk();
    }
  }
  if(!selected_only) {
    include_H_from_C = 1;
    images_dir = ""; //"./";
  }
  selection_changed(0);
}

// move f (and it's children) into list before g:
// returns pointer to whatever is after f & children
void FluidType::move_before(FluidType* g) {
  remove();
  insert(g);
}

// move selected widgets in their parent's list:
void earlier_cb(fltk::Widget*,void*) {
  FluidType *parent = FluidType::current ? FluidType::current->parent : 0;
  for (FluidType* f = parent ? parent->first_child : FluidType::first; f; ) {
    FluidType* next = f->next_brother;
    if (f->selected) {
      FluidType* g = f->previous_brother;
      if (g && !g->selected) f->move_before(g);
    }
    f = next;
  }
}

void later_cb(fltk::Widget*,void*) {
  FluidType *parent = FluidType::current ? FluidType::current->parent : 0;
  FluidType *f;
  for (f = parent ? parent->first_child : FluidType::first;f && f->next_brother;)
    f = f->next_brother;
  for (;f;) {
    FluidType* prev = f->previous_brother;
    if (f->selected) {
      FluidType* g = f->next_brother;
      if (g && !g->selected) g->move_before(f);
    }
    f = prev;
  }
}

////////////////////////////////////////////////////////////////

// write a widget and all it's children:
void FluidType::write() {
  int level = 0;
  for (FluidType* p = parent; p; p = p->parent) level++;
  write_indent(level);
  write_word(type_name());
  if (is_class()) {
    const char * p = prefix();
    if (p && *p) write_word(p);
  }

  write_word(name());
  write_open(level);
  write_properties();
  write_close(level);
  if (!is_parent()) return;
  // now do children:
  write_open(level);
  FluidType *child;
  for (child = first_child; child; child = child->next_brother) child->write();
  write_close(level);
}

void FluidType::write_properties() {
  int level = 0;
  for (FluidType* p = parent; p; p = p->parent) level++;
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
    if (user_data_type()) {
      write_word("user_data_type");
      write_word(user_data_type());
    }
  }
  if (callback()) {
    write_indent(level+1);
    write_word("callback");
    write_word(callback());
  }
  if (is_parent() && open_) write_word("open");
  if (selected) write_word("selected");
  if (tooltip()) {
    write_indent(level+1);
    write_word("tooltip");
    write_word(tooltip());
  }
}

void FluidType::read_property(const char *c) {
  if (!strcmp(c,"label"))
    label(read_word());
  else if (!strcmp(c,"tooltip"))
    tooltip(read_word());
  else if (!strcmp(c,"user_data"))
    user_data(read_word());
  else if (!strcmp(c,"user_data_type"))
    user_data_type(read_word());
  else if (!strcmp(c,"callback"))
    callback(read_word());
  else if (!strcmp(c,"open"))
    open_ = 1;
  else if (!strcmp(c,"selected"))
    select(this,1);
  else
    read_error("Unknown property \"%s\"", c);
}

int FluidType::read_fdesign(const char*, const char*) {return 0;}

/**
 * Build widgets and dataset needed in live mode.
 * \return a widget pointer that the live mode initiator can 'show()'
 * \see leave_live_mode()
 */
fltk::Widget *FluidType::enter_live_mode(int top) {
  return 0L;
}

/**
 * Release all resources created when enetring live mode.
 * \see enter_live_mode()
 */
void FluidType::leave_live_mode() {
}

/**
 * Copy all needed properties for this tye into the live object.
 */
void FluidType::copy_properties() {
}

//
// End of "$Id$".
//
