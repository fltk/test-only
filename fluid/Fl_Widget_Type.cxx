//
// "$Id$"
//
// Widget type code for the Fast Light Tool Kit (FLTK).
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

#include "Fl_Widget_Type.h"
#include "Fl_Function_Type.h"
#include "Fl_Group_Type.h"
#include "Fl_Menu_Type.h"
#include "Fl_Window_Type.h"
#include "WorkspaceType.h"
#include "panel.h"
#include "code.h"
#include "widget_panel.h"
#include "file.h"

#include "Fluid_Image.h"
#include <fltk3/run.h>
#include <fltk3/utf8.h>
#include <fltk3/Group.h>
#include <fltk3/Box.h>
#include <fltk3/Table.h>
#include <fltk3/Input.h>
#include "alignment_panel.h"
#include <fltk3/message.h>
#include <fltk3/Slider.h>
#include <fltk3/Spinner.h>
#include <fltk3/Window.h>
#include "../src/fltk3/flstring.h"
#include <stdio.h>
#include <stdlib.h>

// Make an Fl_Widget_Type subclass instance.
// It figures out the automatic size and parent of the new widget,
// creates the fltk3::Widget (by calling the virtual function _make),
// adds it to the fltk3::Widget hierarchy, creates a new Fl_Type
// instance, sets the widget pointers, and makes all the display
// update correctly...

extern int reading_file;
extern int gridx;
extern int gridy;
extern int i18n_type;
extern const char* i18n_include;
extern const char* i18n_function;
extern const char* i18n_file;
extern const char* i18n_set;
extern fltk3::MenuItem alignmenu[];
extern fltk3::MenuItem whensymbolmenu[];
extern fltk3::NoBox* const ZERO_BOX;
extern int force_parent;
extern void redraw_browser();
extern const char* subclassname(Fl_Type*);
int is_name(const char *c);
const char *array_name(Fl_Widget_Type *o);
int isdeclare(const char *c);
const char *boxname(fltk3::Box *b);
const char *item_name(fltk3::MenuItem* m, int i);
fltk3::Box* boxptr(const char *i);
int item_number(fltk3::MenuItem* m, const char* i);

int pasteoffset;


int Fl_Widget_Type::default_size = fltk3::NORMAL_SIZE;

Fl_Type *Fl_Type::current = 0L;


int Fl_Widget_Type::is_widget() const 
{
  return 1;
}


int Fl_Widget_Type::is_public() const 
{
  return public_;
}


// Return the ideal widget size...
void
Fl_Widget_Type::ideal_size(int &w, int &h) {
  h = o->labelsize();
  o->measure_label(w, h);

  w += fltk3::box_dw(o->box());
  h += fltk3::box_dh(o->box());

  if (w < 15) w = 15;
  if (h < 15) h = 15;
}


// Return the ideal widget spacing...
void
Fl_Widget_Type::ideal_spacing(int &x, int &y) {
  if (o->labelsize() < 10)
    x = y = 0;
  else if (o->labelsize() < 14)
    x = y = 5;
  else
    x = y = 10;
}


Fl_Type *Fl_Widget_Type::make() {
  // Find the current widget, or widget to copy:
  Fl_Type *qq = Fl_Type::current;
  while (qq && (!qq->is_widget() || qq->is_menu_item())) qq = qq->parent;
  if (!qq) {
    fltk3::message("Please select a widget");
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

  int ULX,ULY; // parent's origin in window
  //if (!p->is_window()) { // if it is a group, add corner
  //  ULX = p->o->x(); ULY = p->o->y();
  //} else {
    ULX = ULY = 0;
  //}

  // Figure out a position and size for the widget
  int X,Y,W,H;
  if (is_group()) {	// fill the parent with the widget
    X = ULX+B;
    W = p->o->w()-B;
    Y = ULY+B;
    H = p->o->h()-B;
  } else if (q != p) {	// copy position and size of current widget
    W = q->o->w();
    H = q->o->h();
    X = q->o->x()+W;
    Y = q->o->y();
    if (X+W > ULX+p->o->w()) {
      X = q->o->x();
      Y = q->o->y()+H;
      if (Y+H > ULY+p->o->h()) Y = ULY+B;
    }
  } else {	// just make it small and square...
    X = ULX+B;
    Y = ULY+B;
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
  Fl_Widget_Type *t = _make();
  if (!o) o = widget(0,0,100,100); // create template widget
  t->factory = this;
  // Construct the fltk3::Widget:
  t->o = widget(X,Y,W,H);
  if (reading_file) t->o->label(0);
  else if (t->o->label()) t->label(t->o->label()); // allow editing
  t->o->user_data((void*)t);
  // Put it in the parent:
  //  ((fltk3::Group *)(p->o))->add(t->o); (done by Fl_Type::add())
  // add to browser:
  t->add(p);
  t->redraw();
  return t;
}


void Fl_Widget_Type::setimage(Fluid_Image *i) {
  if (i == image || is_window()) return;
  if (image) image->decrement();
  if (i) i->increment();
  image = i;
  if (i) i->image(o);
  else o->image(0);
  redraw();
}


void Fl_Widget_Type::setinactive(Fluid_Image *i) {
  if (i == inactive || is_window()) return;
  if (inactive) inactive->decrement();
  if (i) i->increment();
  inactive = i;
  if (i) i->deimage(o);
  else o->deimage(0);
  redraw();
}


void Fl_Widget_Type::setlabel(const char *n) {
  o->label(n);
  redraw();
}


Fl_Widget_Type::Fl_Widget_Type() {
  for (int n=0; n<NUM_EXTRA_CODE; n++) {extra_code_[n] = 0; }
  subclass_ = 0;
  hotspot_ = 0;
  tooltip_ = 0;
  image_name_ = 0;
  inactive_name_ = 0;
  image = 0;
  inactive = 0;
  xclass = 0;
  o = 0;
  public_ = 1;
}


Fl_Widget_Type::~Fl_Widget_Type() {
  if (o) {
    o->hide();
    if (o->parent()) ((fltk3::Group*)o->parent())->remove(*o);
    delete o;
  }
  if (subclass_) free((void*)subclass_);
  if (tooltip_) free((void*)tooltip_);
  if (image_name_) free((void*)image_name_);
  if (inactive_name_) free((void*)inactive_name_);
  for (int n=0; n<NUM_EXTRA_CODE; n++) {
    if (extra_code_[n]) free((void*) extra_code_[n]);
  }
}


void Fl_Widget_Type::extra_code(int m,const char *n) {
  storestring(n,extra_code_[m]);
}


void Fl_Widget_Type::subclass(const char *n) {
  if (storestring(n,subclass_) && visible)
    redraw_browser();
}


void Fl_Widget_Type::tooltip(const char *n) {
  storestring(n,tooltip_);
  o->tooltip(n);
}


void Fl_Widget_Type::image_name(const char *n) {
  setimage(Fluid_Image::find(n));
  storestring(n,image_name_);
}


void Fl_Widget_Type::inactive_name(const char *n) {
  setinactive(Fluid_Image::find(n));
  storestring(n,inactive_name_);
}


void Fl_Widget_Type::redraw() {
  Fl_Type *t = this;
  if (is_menu_item()) {
    // find the menu button that parents this menu:
    do t = t->parent; while (t && t->is_menu_item());
    // kludge to cause build_menu to be called again:
    t->add_child(0,0);
  } else {
    while (t->parent && t->parent->is_widget()) t = t->parent;
    ((Fl_Widget_Type*)t)->o->redraw();
  }
}


uchar Fl_Widget_Type::resizable() const {
  if (is_window()) return ((fltk3::Window*)o)->resizable() != 0;
  fltk3::Group* p = (fltk3::Group*)o->parent();
  if (p) return p->resizable() == o;
  else return 0;
}


void Fl_Widget_Type::resizable(uchar v) {
  if (v) {
    if (resizable()) return;
    if (is_window()) ((fltk3::Window*)o)->resizable(o);
    else {
      fltk3::Group* p = (fltk3::Group*)o->parent();
      if (p) p->resizable(o);
    }
  } else {
    if (!resizable()) return;
    if (is_window()) {
      ((fltk3::Window*)o)->resizable(0);
    } else {
      fltk3::Group* p = (fltk3::Group*)o->parent();
      if (p) p->resizable(0);
    }
  }
}


// default widget returns 0 to indicate not-implemented:
int Fl_Widget_Type::textstuff(int, fltk3::Font&, int&, fltk3::Color&) 
{
  return 0;
}


fltk3::MenuItem *Fl_Widget_Type::subtypes() 
{
  return 0;
}


// This is called when user double-clicks an item, open or update the panel:
void Fl_Widget_Type::open() {
  if (!the_widget_panel) the_widget_panel = make_widget_panel();
  the_widget_panel->load(&Fl_Type::is_widget);
  if (Fl_Panel::numselected) the_widget_panel->show();
}


void Fl_Widget_Type::write_static() {
  const char* t = subclassname(this);
  if (!subclass() || (is_class() && !strncmp(t, "fltk3::", 7))) {
    write_declare("#include <%s/%s.h>", include_path(), t+7);
  }
  for (int n=0; n < NUM_EXTRA_CODE; n++) {
    if (extra_code(n) && isdeclare(extra_code(n)))
      write_declare("%s", extra_code(n));
  }
  if (callback() && is_name(callback())) {
    int write_extern_declaration = 1;
    const Fl_Class_Type *cc = is_in_class();
    char buf[1024]; snprintf(buf, 1023, "%s(*)",  callback());
    if (cc) {
      if (cc->has_function("static void", buf))
        write_extern_declaration = 0;
    } else {
      if (has_toplevel_function(0L, buf))
        write_extern_declaration = 0;
    }
    if (write_extern_declaration && !strstr(callback(), "::"))
      write_declare("extern void %s(%s*, %s);", callback(), t,
		    user_data_type() ? user_data_type() : "void*");
  }
  const char* k = class_name(1);
  const char* c = array_name(this);
  if (c && !k && !is_class()) {
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
      write_c("\nvoid %s::%s_i(%s*", k, cn, t);
    } else {
      write_c("\nstatic void %s(%s*", cn, t);
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
      write_c("void %s::%s(%s* o, %s v) {\n", k, cn, t, ut);
      write_c("  ((%s*)(o", k);
      Fl_Type *q = 0;
      for (Fl_Type* p = parent; p && p->is_widget(); q = p, p = p->parent)
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
  if (inactive) {
    if (inactive->written != write_number) {
      inactive->write_static();
      inactive->written = write_number;
    }
  }
}


const char *Fl_Type::callback_name() {
  if (is_name(callback())) return callback();
  return unique_id(this, "cb", name(), label());
}


void Fl_Widget_Type::write_code1() {
  const char* t = subclassname(this);
  const char *c = array_name(this);
  if (c) {
    if (class_name(1)) {
      write_public(public_);
      write_h("  %s *%s;\n", t, c);
    }
  }
  if (class_name(1) && callback() && !is_name(callback())) {
    const char* cn = callback_name();
    const char* ut = user_data_type() ? user_data_type() : "void*";
    write_public(0);
    write_h("  inline void %s_i(%s*, %s);\n", cn, t, ut);
    write_h("  static void %s(%s*, %s);\n", cn, t, ut);
  }
  // figure out if local variable will be used (prevent compiler warnings):
  int wused = !name() && is_window();
  const char *ptr;

  varused = wused;

  if (!name() && !varused) {
    varused |= is_parent();

    if (!varused) {
      varused_test = 1;
      write_widget_code();
      varused_test = 0;
    }
  }

  if (!varused) {
    for (int n=0; n < NUM_EXTRA_CODE; n++)
      if (extra_code(n) && !isdeclare(extra_code(n)))
      {
        int instring = 0;
	int inname = 0;
        for (ptr = extra_code(n); *ptr; ptr ++) {
	  if (instring) {
	    if (*ptr == '\\') ptr++;
	    else if (*ptr == '\"') instring = 0;
	  } else if (inname && !isalnum(*ptr & 255)) inname = 0;
          else if (*ptr == '\"') instring = 1;
	  else if (isalnum(*ptr & 255) || *ptr == '_') {
	    size_t len = strspn(ptr, "0123456789_"
	                             "abcdefghijklmnopqrstuvwxyz"
				     "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

            if (!strncmp(ptr, "o", len)) {
	      varused = 1;
	      break;
	    } else ptr += len - 1;
          }
	}
      }
  }

  write_c("%s{ ", indent());
  if (varused) write_c("%s* o = ", t);
  if (name()) write_c("%s = ", name());
  if (is_window()) {
    // Handle special case where user is faking a fltk3::Group type as a window,
    // there is no 2-argument constructor in that case:
    if (!strstr(t, "Window"))
      write_c("new %s(0, 0, %d, %d", t, o->w(), o->h());
    else
      write_c("new %s(%d, %d", t, o->w(), o->h());
  } else {
    write_c("new %s(%d, %d, %d, %d", t, o->x(), o->y(), o->w(), o->h());
  }
  if (label() && *label()) {
    write_c(", ");
    switch (i18n_type) {
    case 0 : /* None */
        write_cstring(label());
        break;
    case 1 : /* GNU gettext */
        write_c("%s(", i18n_function);
        write_cstring(label());
	write_c(")");
        break;
    case 2 : /* POSIX catgets */
        write_c("catgets(%s,%s,%d,", i18n_file[0] ? i18n_file : "_catalog",
	        i18n_set, msgnum());
        write_cstring(label());
	write_c(")");
        break;
    }
  }
  write_c(");\n");

  indentation += 2;

  if (wused) write_c("%sw = o;\n", indent());

  write_widget_code();
}


void Fl_Widget_Type::write_color(const char* field, fltk3::Color color) {
  const char* color_name = 0;
  switch (color) {
  case fltk3::FOREGROUND_COLOR:	color_name = "fltk3::FOREGROUND_COLOR";	break;
  case fltk3::BACKGROUND2_COLOR:	color_name = "fltk3::BACKGROUND2_COLOR";	break;
  case fltk3::INACTIVE_COLOR:	color_name = "fltk3::INACTIVE_COLOR";	break;
  case fltk3::SELECTION_COLOR:	color_name = "fltk3::SELECTION_COLOR";	break;
  case fltk3::GRAY0:		color_name = "fltk3::GRAY0";		break;
  case fltk3::DARK3:		color_name = "fltk3::DARK3";		break;
  case fltk3::DARK2:		color_name = "fltk3::DARK2";		break;
  case fltk3::DARK1:		color_name = "fltk3::DARK1";		break;
  case fltk3::BACKGROUND_COLOR:	color_name = "fltk3::BACKGROUND_COLOR";	break;
  case fltk3::LIGHT1:		color_name = "fltk3::LIGHT1";		break;
  case fltk3::LIGHT2:		color_name = "fltk3::LIGHT2";		break;
  case fltk3::LIGHT3:		color_name = "fltk3::LIGHT3";		break;
  case fltk3::BLACK:		color_name = "fltk3::BLACK";		break;
  case fltk3::RED:			color_name = "fltk3::RED";			break;
  case fltk3::GREEN:		color_name = "fltk3::GREEN";		break;
  case fltk3::YELLOW:		color_name = "fltk3::YELLOW";		break;
  case fltk3::BLUE:			color_name = "fltk3::BLUE";			break;
  case fltk3::MAGENTA:		color_name = "fltk3::MAGENTA";		break;
  case fltk3::CYAN:			color_name = "fltk3::CYAN";			break;
  case fltk3::DARK_RED:		color_name = "fltk3::DARK_RED";		break;
  case fltk3::DARK_GREEN:		color_name = "fltk3::DARK_GREEN";		break;
  case fltk3::DARK_YELLOW:		color_name = "fltk3::DARK_YELLOW";		break;
  case fltk3::DARK_BLUE:		color_name = "fltk3::DARK_BLUE";		break;
  case fltk3::DARK_MAGENTA:		color_name = "fltk3::DARK_MAGENTA";		break;
  case fltk3::DARK_CYAN:		color_name = "fltk3::DARK_CYAN";		break;
  case fltk3::WHITE:		color_name = "fltk3::WHITE";		break;
  }
  const char *var = is_class() ? "this" : name() ? name() : "o";
  if (color_name) {
    write_c("%s%s->%s(%s);\n", indent(), var, field, color_name);
  } else {
    write_c("%s%s->%s((fltk3::Color)%d);\n", indent(), var, field, color);
  }
}


// this is split from write_code1() for Fl_Window_Type:
void Fl_Widget_Type::write_widget_code() {
  fltk3::Widget* tplate = ((Fl_Widget_Type*)factory)->o;
  const char *var = is_class() ? "this" : name() ? name() : "o";

  if (tooltip() && *tooltip()) {
    write_c("%s%s->tooltip(",indent(), var);
    switch (i18n_type) {
    case 0 : /* None */
        write_cstring(tooltip());
        break;
    case 1 : /* GNU gettext */
        write_c("%s(", i18n_function);
        write_cstring(tooltip());
	write_c(")");
        break;
    case 2 : /* POSIX catgets */
        write_c("catgets(%s,%s,%d,", i18n_file[0] ? i18n_file : "_catalog",
	        i18n_set, msgnum() + 1);
        write_cstring(tooltip());
	write_c(")");
        break;
    }
    write_c(");\n");
  }

  if (is_spinner() && ((fltk3::Spinner*)o)->type() != ((fltk3::Spinner*)tplate)->type())
    write_c("%s%s->type(%d);\n", indent(), var, ((fltk3::Spinner*)o)->type());
  else if (o->type() != tplate->type() && !is_window())
    write_c("%s%s->type(%d);\n", indent(), var, o->type());
  if (o->box() != tplate->box() || subclass())
    write_c("%s%s->box(fltk3::%s);\n", indent(), var, boxname(o->box()));

  // write shortcut command if needed
  unsigned int shortcut = 0;
  if (is_button()) shortcut = ((fltk3::Button*)o)->shortcut();
  else if (is_input()) shortcut = ((fltk3::Input_*)o)->shortcut();
  else if (is_value_input()) shortcut = ((fltk3::ValueInput*)o)->shortcut();
  else if (is_text_display()) shortcut = ((fltk3::TextDisplay*)o)->shortcut();
  if (shortcut) {
    if (use_FL_COMMAND && (shortcut & (fltk3::CTRL|fltk3::META))) {
      write_c("%s%s->shortcut(fltk3::COMMAND|0x%x);\n", indent(), var, shortcut & ~(fltk3::CTRL|fltk3::META));
    } else {
      write_c("%s%s->shortcut(0x%x);\n", indent(), var, shortcut);
    }
  }

  if (is_button()) {
    fltk3::Button* b = (fltk3::Button*)o;
    if (b->down_box()) write_c("%s%s->down_box(fltk3::%s);\n", indent(), var,
			       boxname(b->down_box()));
    if (b->value()) write_c("%s%s->value(1);\n", indent(), var);
  } else if (!strcmp(type_name(), "fltk3::InputChoice")) {
    fltk3::InputChoice* b = (fltk3::InputChoice*)o;
    if (b->down_box()) write_c("%s%s->down_box(fltk3::%s);\n", indent(), var,
			       boxname(b->down_box()));
  } else if (is_menu_button()) {
    fltk3::Menu_* b = (fltk3::Menu_*)o;
    if (b->down_box()) write_c("%s%s->down_box(fltk3::%s);\n", indent(), var,
			       boxname(b->down_box()));
  }
  if (o->color() != tplate->color() || subclass())
    write_color("color", o->color());
  if (o->selection_color() != tplate->selection_color() || subclass())
    write_color("selection_color", o->selection_color());
  if (image) image->write_code(var);
  if (inactive) inactive->write_code(var, 1);
  if (o->labeltype() != tplate->labeltype() || subclass())
    write_c("%s%s->labeltype(fltk3::%s);\n", indent(), var,
	    item_name(labeltypemenu, o->labeltype()));
  if (o->labelfont() != tplate->labelfont() || subclass())
    write_c("%s%s->labelfont(%d);\n", indent(), var, o->labelfont());
  if (o->labelsize() != tplate->labelsize() || subclass())
    write_c("%s%s->labelsize(%d);\n", indent(), var, o->labelsize());
  if (o->labelcolor() != tplate->labelcolor() || subclass())
    write_color("labelcolor", o->labelcolor());
  if (is_valuator()) {
    fltk3::Valuator* v = (fltk3::Valuator*)o;
    fltk3::Valuator* f = (fltk3::Valuator*)(tplate);
    if (v->minimum()!=f->minimum())
      write_c("%s%s->minimum(%g);\n", indent(), var, v->minimum());
    if (v->maximum()!=f->maximum())
      write_c("%s%s->maximum(%g);\n", indent(), var, v->maximum());
    if (v->step()!=f->step())
      write_c("%s%s->step(%g);\n", indent(), var, v->step());
    if (v->value()) {
      if (is_valuator()==3) { // fltk3::Scrollbar::value(double) is nott available
        write_c("%s%s->fltk3::Slider::value(%g);\n", indent(), var, v->value());
      } else {
        write_c("%s%s->value(%g);\n", indent(), var, v->value());
      }
    }
    if (is_valuator()>=2) {
      double x = ((fltk3::Slider*)v)->slider_size();
      double y = ((fltk3::Slider*)f)->slider_size();
      if (x != y) write_c("%s%s->slider_size(%g);\n", indent(), var, x);
    }
  }
  if (is_spinner()) {
    fltk3::Spinner* v = (fltk3::Spinner*)o;
    fltk3::Spinner* f = (fltk3::Spinner*)(tplate);
    if (v->minimum()!=f->minimum())
      write_c("%s%s->minimum(%g);\n", indent(), var, v->minimum());
    if (v->maximum()!=f->maximum())
      write_c("%s%s->maximum(%g);\n", indent(), var, v->maximum());
    if (v->step()!=f->step())
      write_c("%s%s->step(%g);\n", indent(), var, v->step());
    if (v->value()!=1.0f)
      write_c("%s%s->value(%g);\n", indent(), var, v->value());
  }

  {fltk3::Font ff; int fs; fltk3::Color fc; if (textstuff(4,ff,fs,fc)) {
    fltk3::Font f; int s; fltk3::Color c; textstuff(0,f,s,c);
    if (f != ff) write_c("%s%s->textfont(%d);\n", indent(), var, f);
    if (s != fs) write_c("%s%s->textsize(%d);\n", indent(), var, s);
    if (c != fc) write_color("textcolor", c);
  }}
  const char* ud = user_data();
  if (class_name(1) && !parent->is_widget()) ud = "this";
  if (callback()) {
    write_c("%s%s->callback((fltk3::Callback*)%s", indent(), var, callback_name());
    if (ud)
      write_c(", (void*)(%s));\n", ud);
    else
      write_c(");\n");
  } else if (ud) {
    write_c("%s%s->user_data((void*)(%s));\n", indent(), var, ud);
  }
  if (o->align() != tplate->align() || subclass()) {
    int i = o->align();
    write_c("%s%s->align(fltk3::Align(%s", indent(), var,
	    item_name(alignmenu, i & ~fltk3::ALIGN_INSIDE));
    if (i & fltk3::ALIGN_INSIDE) write_c("|fltk3::ALIGN_INSIDE");
    write_c("));\n");
  }
  // avoid the unsupported combination of flegs when user sets 
  // "when" to "fltk3::WHEN_NEVER", but keeps the "no change" set. 
  // FIXME: This could be reflected in the GUI by graying out the button.
  fltk3::When ww = o->when();
  if (ww==fltk3::WHEN_NOT_CHANGED)
    ww = fltk3::WHEN_NEVER;
  if (ww != tplate->when() || subclass())
    write_c("%s%s->when(%s);\n", indent(), var,
            item_name(whensymbolmenu, ww));
  if (!o->visible() && o->parent())
    write_c("%s%s->hide();\n", indent(), var);
  if (!o->active())
    write_c("%s%s->deactivate();\n", indent(), var);
  if (!is_group() && resizable())
    write_c("%sfltk3::Group::current()->resizable(%s);\n", indent(), var);
  if (hotspot()) {
    if (is_class())
      write_c("%shotspot(%s);\n", indent(), var);
    else if (is_window())
      write_c("%s%s->hotspot(%s);\n", indent(), var, var);
    else
      write_c("%s%s->window()->hotspot(%s);\n", indent(), var, var);
  }
}


void Fl_Widget_Type::write_extra_code() {
  for (int n=0; n < NUM_EXTRA_CODE; n++)
    if (extra_code(n) && !isdeclare(extra_code(n)))
      write_c("%s%s\n", indent(), extra_code(n));
}


void Fl_Widget_Type::write_block_close() {
  indentation -= 2;
  write_c("%s} // %s* %s\n", indent(), subclassname(this),
          name() ? name() : "o");
}


void Fl_Widget_Type::write_code2() {
  write_extra_code();
  write_block_close();
}


void Fl_Widget_Type::write_properties() {
  Fl_Type::write_properties();
  write_indent(level+1);
  switch (public_) {
    case 0: write_string("private"); break;
    case 1: break;
    case 2: write_string("protected"); break;
  }
  if (tooltip() && *tooltip()) {
    write_string("tooltip");
    write_word(tooltip());
  }
  if (image_name() && *image_name()) {
    write_string("image");
    write_word(image_name());
  }
  if (inactive_name() && *inactive_name()) {
    write_string("deimage");
    write_word(inactive_name());
  }
  write_string("xywh {%d %d %d %d}", o->x(), o->y(), o->w(), o->h());
  fltk3::Widget* tplate = ((Fl_Widget_Type*)factory)->o;
  if (is_spinner() && ((fltk3::Spinner*)o)->type() != ((fltk3::Spinner*)tplate)->type()) {
    write_string("type");
    write_word(item_name(subtypes(), ((fltk3::Spinner*)o)->type()));
  } else if (o->type() != tplate->type() || is_window()) {
    write_string("type");
    write_word(item_name(subtypes(), o->type()));
  }
  if (o->box() != tplate->box()) {
    write_string("box"); write_word(boxname(o->box()));}
  if (is_input()) {
    fltk3::Input_* b = (fltk3::Input_*)o;
    if (b->shortcut()) write_string("shortcut 0x%x", b->shortcut());
  }
  if (is_value_input()) {
    fltk3::ValueInput* b = (fltk3::ValueInput*)o;
    if (b->shortcut()) write_string("shortcut 0x%x", b->shortcut());
  }
  if (is_text_display()) {
    fltk3::TextDisplay* b = (fltk3::TextDisplay*)o;
    if (b->shortcut()) write_string("shortcut 0x%x", b->shortcut());
  }
  if (is_button()) {
    fltk3::Button* b = (fltk3::Button*)o;
    if (b->down_box()) {
      write_string("down_box"); write_word(boxname(b->down_box()));}
    if (b->shortcut()) write_string("shortcut 0x%x", b->shortcut());
    if (b->value()) write_string("value 1");
  } else if (!strcmp(type_name(), "fltk3::InputChoice")) {
    fltk3::InputChoice* b = (fltk3::InputChoice*)o;
    if (b->down_box()) {
      write_string("down_box"); write_word(boxname(b->down_box()));}
  } else if (is_menu_button()) {
    fltk3::Menu_* b = (fltk3::Menu_*)o;
    if (b->down_box()) {
      write_string("down_box"); write_word(boxname(b->down_box()));}
  }
  if (o->color()!=tplate->color())
    write_string("color %d", o->color());
  if (o->selection_color()!=tplate->selection_color())
    write_string("selection_color %d", o->selection_color());
  if (o->labeltype()!=tplate->labeltype()) {
    write_string("labeltype");
    write_word(item_name(labeltypemenu, o->labeltype()));
  }
  if (o->labelfont()!=tplate->labelfont())
    write_string("labelfont %d", o->labelfont());
  if (o->labelsize()!=tplate->labelsize())
    write_string("labelsize %d", o->labelsize());
  if (o->labelcolor()!=tplate->labelcolor())
    write_string("labelcolor %d", o->labelcolor());
  if (o->align()!=tplate->align())
    write_string("align %d", o->align());
  if (o->when() != tplate->when())
    write_string("when %d", o->when());
  if (is_valuator()) {
    fltk3::Valuator* v = (fltk3::Valuator*)o;
    fltk3::Valuator* f = (fltk3::Valuator*)(tplate);
    if (v->minimum()!=f->minimum()) write_string("minimum %g",v->minimum());
    if (v->maximum()!=f->maximum()) write_string("maximum %g",v->maximum());
    if (v->step()!=f->step()) write_string("step %g",v->step());
    if (v->value()!=0.0) write_string("value %g",v->value());
    if (is_valuator()>=2) {
      double x = ((fltk3::Slider*)v)->slider_size();
      double y = ((fltk3::Slider*)f)->slider_size();
      if (x != y) write_string("slider_size %g", x);
    }
  }
  if (is_spinner()) {
    fltk3::Spinner* v = (fltk3::Spinner*)o;
    fltk3::Spinner* f = (fltk3::Spinner*)(tplate);
    if (v->minimum()!=f->minimum()) write_string("minimum %g",v->minimum());
    if (v->maximum()!=f->maximum()) write_string("maximum %g",v->maximum());
    if (v->step()!=f->step()) write_string("step %g",v->step());
    if (v->value()!=1.0) write_string("value %g",v->value());
  }
  {fltk3::Font ff; int fs; fltk3::Color fc; if (textstuff(4,ff,fs,fc)) {
    fltk3::Font f; int s; fltk3::Color c; textstuff(0,f,s,c);
    if (f != ff) write_string("textfont %d", f);
    if (s != fs) write_string("textsize %d", s);
    if (c != fc) write_string("textcolor %d", c);
  }}
  if (!o->visible()) write_string("hide");
  if (!o->active()) write_string("deactivate");
  if (resizable()) write_string("resizable");
  if (hotspot()) write_string(is_menu_item() ? "divider" : "hotspot");
  for (int n=0; n < NUM_EXTRA_CODE; n++) if (extra_code(n)) {
    write_indent(level+1);
    write_string("code%d",n);
    write_word(extra_code(n));
  }
  if (subclass()) {
    write_indent(level+1);
    write_string("class");
    write_word(subclass());
  }
}


char Fl_Widget_Type::read_property(const char *c) {
  int x,y,w,h; fltk3::Font f; int s; fltk3::Color cc; fltk3::Box *bx;
  if (!strcmp(c,"private")) {
    public_ = 0;
  } else if (!strcmp(c,"protected")) {
    public_ = 2;
  } else if (!strcmp(c,"xywh")) {
    if (sscanf(read_word(),"%d %d %d %d",&x,&y,&w,&h) == 4) {
      x += pasteoffset;
      y += pasteoffset;
      // FIXME temporary change!
      //if (read_version>=2.0 && o->parent() && o->parent()!=o->window()) {
      //  x += o->parent()->x();
      //  y += o->parent()->y();
      //}
      o->resize(x,y,w,h);
    }
  } else if (!strcmp(c,"tooltip")) {
    tooltip(read_word());
  } else if (!strcmp(c,"image")) {
    image_name(read_word());
  } else if (!strcmp(c,"deimage")) {
    inactive_name(read_word());
  } else if (!strcmp(c,"type")) {
    if (is_spinner()) 
      ((fltk3::Spinner*)o)->type(item_number(subtypes(), read_word()));
    else
      o->type(item_number(subtypes(), read_word()));
  } else if (!strcmp(c,"box")) {
    const char* value = read_word();
    if ((bx = boxptr(value))) {
      if (bx == ZERO_BOX) bx = fltk3::NO_BOX;
      o->box(bx);
    }
  } else if (is_button() && !strcmp(c,"down_box")) {
    const char* value = read_word();
    if ((bx = boxptr(value))) {
      if (bx == ZERO_BOX) bx = 0;
      ((fltk3::Button*)o)->down_box(bx);
    }
  } else if (!strcmp(type_name(), "fltk3::InputChoice") && !strcmp(c,"down_box")) {
    const char* value = read_word();
    if ((bx = boxptr(value))) {
      if (bx == ZERO_BOX) bx = 0;
      ((fltk3::InputChoice*)o)->down_box(bx);
    }
  } else if (is_menu_button() && !strcmp(c,"down_box")) {
    const char* value = read_word();
    if ((bx = boxptr(value))) {
      if (bx == ZERO_BOX) bx = 0;
      ((fltk3::Menu_*)o)->down_box(bx);
    }
  } else if (is_button() && !strcmp(c,"value")) {
    const char* value = read_word();
    ((fltk3::Button*)o)->value(atoi(value));
  } else if (!strcmp(c,"color")) {
    const char *cw = read_word();
    if (cw[0]=='0' && cw[1]=='x') {
      sscanf(cw,"0x%x",&x);
      o->color(x);
    } else {
      int n = sscanf(cw,"%d %d",&x,&y);
      if (n == 2) { // back compatibility...
        if (x != 47) o->color(x);
        o->selection_color(y);
      } else {
        o->color(x);
      }
    }
  } else if (!strcmp(c,"selection_color")) {
    if (sscanf(read_word(),"%d",&x)) o->selection_color(x);
  } else if (!strcmp(c,"labeltype")) {
    c = read_word();
    if (!strcmp(c,"image")) {
      Fluid_Image *i = Fluid_Image::find(label());
      if (!i) read_error("Image file '%s' not found", label());
      else setimage(i);
      image_name(label());
      label("");
    } else {
      o->labeltype((fltk3::Labeltype)item_number(labeltypemenu,c));
    }
  } else if (!strcmp(c,"labelfont")) {
    if (sscanf(read_word(),"%d",&x) == 1) o->labelfont(x);
  } else if (!strcmp(c,"labelsize")) {
    if (sscanf(read_word(),"%d",&x) == 1) o->labelsize(x);
  } else if (!strcmp(c,"labelcolor")) {
    if (sscanf(read_word(),"%d",&x) == 1) o->labelcolor(x);
  } else if (!strcmp(c,"align")) {
    if (sscanf(read_word(),"%d",&x) == 1) o->align(x);
  } else if (!strcmp(c,"when")) {
    if (sscanf(read_word(),"%d",&x) == 1) o->when(x);
  } else if (!strcmp(c,"minimum")) {
    if (is_valuator()) ((fltk3::Valuator*)o)->minimum(strtod(read_word(),0));
    if (is_spinner()) ((fltk3::Spinner*)o)->minimum(strtod(read_word(),0));
  } else if (!strcmp(c,"maximum")) {
    if (is_valuator()) ((fltk3::Valuator*)o)->maximum(strtod(read_word(),0));
    if (is_spinner()) ((fltk3::Spinner*)o)->maximum(strtod(read_word(),0));
  } else if (!strcmp(c,"step")) {
    if (is_valuator()) ((fltk3::Valuator*)o)->step(strtod(read_word(),0));
    if (is_spinner()) ((fltk3::Spinner*)o)->step(strtod(read_word(),0));
  } else if (!strcmp(c,"value")) {
    if (is_valuator()) ((fltk3::Valuator*)o)->value(strtod(read_word(),0));
    if (is_spinner()) ((fltk3::Spinner*)o)->value(strtod(read_word(),0));
  } else if ((!strcmp(c,"slider_size")||!strcmp(c,"size"))&&is_valuator()==2) {
    ((fltk3::Slider*)o)->slider_size(strtod(read_word(),0));
  } else if (!strcmp(c,"textfont")) {
    if (sscanf(read_word(),"%d",&x) == 1) {f=(fltk3::Font)x; textstuff(1,f,s,cc);}
  } else if (!strcmp(c,"textsize")) {
    if (sscanf(read_word(),"%d",&x) == 1) {s=x; textstuff(2,f,s,cc);}
  } else if (!strcmp(c,"textcolor")) {
    if (sscanf(read_word(),"%d",&x) == 1) {cc=(fltk3::Color)x;textstuff(3,f,s,cc);}
  } else if (!strcmp(c,"hide")) {
    o->hide();
  } else if (!strcmp(c,"deactivate")) {
    o->deactivate();
  } else if (!strcmp(c,"resizable")) {
    resizable(1);
  } else if (!strcmp(c,"hotspot") || !strcmp(c, "divider")) {
    hotspot(1);
  } else if (!strcmp(c,"class")) {
    subclass(read_word());
  } else if (!strcmp(c,"shortcut")) {
    unsigned int shortcut = strtol(read_word(),0,0);
    if (is_button()) ((fltk3::Button*)o)->shortcut(shortcut);
    else if (is_input()) ((fltk3::Input_*)o)->shortcut(shortcut);
    else if (is_value_input()) ((fltk3::ValueInput*)o)->shortcut(shortcut);
    else if (is_text_display()) ((fltk3::TextDisplay*)o)->shortcut(shortcut);
  } else {
    if (!strncmp(c,"code",4)) {
      int n = atoi(c+4);
      if (n >= 0 && n <= NUM_EXTRA_CODE) {
        extra_code(n,read_word());
        return 1;
      }
    } else if (!strcmp(c,"extra_code")) {
      extra_code(0,read_word());
      return 1;
    }
    return Fl_Type::read_property(c);
  }
  return 1;
}


fltk3::Widget *Fl_Widget_Type::enter_live_mode(int) {
  live_widget = widget(o->x(), o->y(), o->w(), o->h());
  if (live_widget)
    copy_properties();
  return live_widget;
}


void Fl_Widget_Type::leave_live_mode() {
}


/**
 * copy all properties from the edit widget to the live widget
 */
void Fl_Widget_Type::copy_properties() {
  if (!live_widget) 
    return;

  // copy all attributes common to all widget types
  fltk3::Widget *w = live_widget;
  w->label(o->label());
  w->tooltip(tooltip());
  w->type(o->type());
  w->box(o->box());
  w->color(o->color());
  w->selection_color(o->selection_color());
  w->labeltype(o->labeltype());
  w->labelfont(o->labelfont());
  w->labelsize(o->labelsize());
  w->labelcolor(o->labelcolor());
  w->align(o->align());

  // copy all attributes specific to widgets derived from fltk3::Button
  if (is_button()) {
    fltk3::Button* d = (fltk3::Button*)live_widget, *s = (fltk3::Button*)o;
    d->down_box(s->down_box());
    d->shortcut(s->shortcut());
    d->value(s->value());
  }

  // copy all attributes specific to widgets derived from fltk3::Input_
  if (is_input()) {
    fltk3::Input_* d = (fltk3::Input_*)live_widget, *s = (fltk3::Input_*)o;
    d->shortcut(s->shortcut());
  }

  // copy all attributes specific to widgets derived from fltk3::ValueInput
  if (is_value_input()) {
    fltk3::ValueInput* d = (fltk3::ValueInput*)live_widget, *s = (fltk3::ValueInput*)o;
    d->shortcut(s->shortcut());
  }

  // copy all attributes specific to widgets derived from fltk3::TextDisplay
  if (is_text_display()) {
    fltk3::TextDisplay* d = (fltk3::TextDisplay*)live_widget, *s = (fltk3::TextDisplay*)o;
    d->shortcut(s->shortcut());
  }

  // copy all attributes specific to fltk3::Valuator and derived classes
  if (is_valuator()) {
    fltk3::Valuator* d = (fltk3::Valuator*)live_widget, *s = (fltk3::Valuator*)o;
    d->minimum(s->minimum());
    d->maximum(s->maximum());
    d->step(s->step());
    d->value(s->value());
    if (is_valuator()>=2) {
      fltk3::Slider *d = (fltk3::Slider*)live_widget, *s = (fltk3::Slider*)o;
      d->slider_size(s->slider_size());
    }
  }

  // copy all attributes specific to fltk3::Spinner and derived classes
  if (is_spinner()) {
    fltk3::Spinner* d = (fltk3::Spinner*)live_widget, *s = (fltk3::Spinner*)o;
    d->minimum(s->minimum());
    d->maximum(s->maximum());
    d->step(s->step());
    d->value(s->value());
  }
 
/* TODO: implement this
  {fltk3::Font ff; int fs; fltk3::Color fc; if (textstuff(4,ff,fs,fc)) {
    fltk3::Font f; int s; fltk3::Color c; textstuff(0,f,s,c);
    if (f != ff) write_string("textfont %d", f);
    if (s != fs) write_string("textsize %d", s);
    if (c != fc) write_string("textcolor %d", c);
  }}*/

  if (!o->visible()) 
    w->hide();
  if (!o->active()) 
    w->deactivate();
  if (resizable() && w->parent()) 
    w->parent()->resizable(o);
}


//
// End of "$Id$".
//
