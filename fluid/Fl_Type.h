//
// "$Id: Fl_Type.h,v 1.25 2000/09/05 17:36:20 spitzak Exp $"
//
// Widget type header file for the Fast Light Tool Kit (FLTK).
//
// Each object described by Fluid is one of these objects.  They
// are all stored in a double-linked list.
//
// There is also a single "factory" instance of each type of this.
// The method "make()" is called on this factory to create a new
// instance of this object.  It could also have a "copy()" function,
// but it was easier to implement this by using the file read/write
// that is needed to save the setup anyways.
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

#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu.H>
#include "Fluid_Plugins.h"
#include "Fluid_Image.h"

class FLUID_API Fl_Type {

  friend class Widget_Browser;
  friend Fl_Widget *make_type_browser(int,int,int,int,const char *l=0);
  friend class Fl_Window_Type;
  virtual void setlabel(const char *); // virtual part of label(char*)

protected:

  Fl_Type();

  const char *name_;
  const char *label_;
  const char *tooltip_;
  const char *callback_;
  const char *user_data_;
  const char *user_data_type_;

public:	// things that should not be public:

  Fl_Type* parent;
  Fl_Type* first_child;
  Fl_Type* next_brother;
  Fl_Type* previous_brother;

  Fl_Type* walk(const Fl_Type* top) const;
  Fl_Type* walk() const;

  static Fl_Type *first;

  char new_selected; // browser highlight
  char selected; // copied here by selection_changed()
  char open_;	// open/close state of this parent in browser

  Fl_Type *factory;
  const char *callback_name();

public:

  virtual ~Fl_Type();
  virtual Fl_Type *make() = 0;

  void add(Fl_Type *parent); // add as new child
  void insert(Fl_Type *n); // insert into list before n
  void remove(); // remove from list
  void move_before(Fl_Type*); // move before a sibling

  virtual const char *title(); // string for browser
  virtual const char *type_name() = 0; // type for code output

  const char *name() const {return name_;}
  void name(const char *);
  const char *label() const {return label_;}
  void label(const char *);
  const char *tooltip() const {return tooltip_;}
  void tooltip(const char *);
  const char *callback() const {return callback_;}
  void callback(const char *);
  const char *user_data() const {return user_data_;}
  void user_data(const char *);
  const char *user_data_type() const {return user_data_type_;}
  void user_data_type(const char *);

  virtual Fl_Type* click_test(int,int);
  virtual void add_child(Fl_Type*, Fl_Type* beforethis);
  virtual void move_child(Fl_Type*, Fl_Type* beforethis);
  virtual void remove_child(Fl_Type*);

  static Fl_Type *current;  // most recently picked object
  virtual void open();	// what happens when you double-click

  // read and write data to a saved file:
  void write();
  virtual void write_properties();
  virtual void read_property(const char *);
  virtual int read_fdesign(const char*, const char*);

  // write code, these are called in order:
  virtual void write_static(); // write static stuff to .c file
  virtual void write_code();   // write .h and .c file

  // fake rtti:
  virtual int is_parent() const;
  virtual int is_widget() const;
  virtual int is_button() const;
  virtual int is_light_button() const;
  virtual int is_valuator() const;
  virtual int is_menu_item() const;
  virtual int is_menu_button() const;
  virtual int is_adjuster() const;
  virtual int is_counter() const;
  virtual int is_slider() const;
  virtual int is_scrollbar() const;
  virtual int is_choice() const;
  virtual int is_group() const;
  virtual int is_window() const;
  virtual int is_code_block() const;
  virtual int is_decl_block() const;
  virtual int is_class() const;
  virtual int is_browser() const;
  virtual int is_input() const;
  virtual int is_value_input() const;
  virtual int is_value_output() const;
  virtual int is_value_slider() const;

  const char* class_name(const int need_nest) const;
};

////////////////////////////////////////////////////////////////

#define NUM_EXTRA_CODE 4

class FLUID_API Fl_Widget_Type : public Fl_Type {
  virtual Fl_Widget *widget(int,int,int,int) = 0;
  virtual Fl_Widget_Type *_make() = 0; // virtual constructor
  virtual void setlabel(const char *);

  const char *extra_code_[NUM_EXTRA_CODE];
  const char *subclass_;
  uchar hotspot_;

protected:

  void write_static();
  void write_code();
  void write_code1();
  void write_widget_code();
  void write_extra_code();
  void write_block_close();

public:

  Fl_Widget *o;
  int public_;
  char set_xy;
  
  Fluid_Image* image;
  void setimage(Fluid_Image*);

  Fl_Widget_Type();
  Fl_Type *make();
  void open();

  const char *extra_code(int n) const {return extra_code_[n];}
  void extra_code(int n,const char *);
  const char *subclass() const {return subclass_;}
  void subclass(const char *);
  uchar hotspot() const {return hotspot_;}
  void hotspot(uchar v) {hotspot_ = v;}
  uchar resizable() const;
  void resizable(uchar v);

  virtual Fl_Menu_Item *subtypes();

  virtual int is_widget() const;

  virtual void write_properties();
  virtual void read_property(const char *);
  virtual int read_fdesign(const char*, const char*);

  ~Fl_Widget_Type();
  void redraw();
};

#define LOAD ((void*)9831)

FLUID_API extern Fl_Widget_Type *current_widget; // one of the selected ones

////////////////////////////////////////////////////////////////

FLUID_API extern Fl_Menu_Item window_type_menu[];

class FLUID_API Fl_Window_Type : public Fl_Widget_Type {
  Fl_Menu_Item* subtypes() {return window_type_menu;}

  friend class Overlay_Window;
  int mx,my;		// mouse position during dragging
  int x1,y1;		// initial position of selection box
  int bx,by,br,bt;	// bounding box of selection
  int dx,dy;
  int drag;		// which parts of bbox are being moved
  int numselected;	// number of children selected
  enum {LEFT=1,RIGHT=2,BOTTOM=4,TOP=8,DRAG=16,BOX=32};
  void draw_overlay();
  void newdx();
  void newposition(Fl_Widget_Type *,int &x,int &y,int &w,int &h);
  int handle(int);
  virtual void setlabel(const char *);
  void write_code();
  Fl_Widget_Type *_make() {return 0;} // we don't call this
  Fl_Widget *widget(int,int,int,int) {return 0;}
  int recalc;		// set by fix_overlay()
  void moveallchildren();
  void move_children(Fl_Type*, int);

public:

  uchar modal, non_modal, border;

  Fl_Type *make();
  virtual const char *type_name() {return "Fl_Window";}

  void open();

  void fix_overlay();	// update the bounding box, etc

  virtual void write_properties();
  virtual void read_property(const char *);
  virtual int read_fdesign(const char*, const char*);

  void add_child(Fl_Type*, Fl_Type*);
  void move_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);

  int is_parent() const {return 1;}
  int is_group() const {return 1;}
  int is_window() const {return 1;}
};

////////////////////////////////////////////////////////////////

class FLUID_API Fl_Group_Type : public Fl_Widget_Type {
public:
  virtual const char *type_name();
  Fl_Widget *widget(int x,int y,int w,int h);
  Fl_Widget_Type* _make();
  Fl_Type *make();
  void write_code();
  void add_child(Fl_Type*, Fl_Type*);
  void move_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);
  int is_parent() const;
  int is_group() const;
};

////////////////////////////////////////////////////////////////
// This header file also declares all the global functions in fluid:

// object list operations:
Fl_Widget *make_widget_browser(int x,int y,int w,int h);
extern int modflag;
void delete_all(int selected_only=0);
void selection_changed(Fl_Type* new_current);

// file operations:
FLUID_API void write_word(const char *);
FLUID_API void write_string(const char *,...);
FLUID_API int write_file(const char *, int selected_only = 0);
FLUID_API int write_code(const char *cfile, const char *hfile);

FLUID_API int write_declare(const char *, ...);
FLUID_API int is_id(char);
FLUID_API const char* unique_id(void* o, const char*, const char*, const char*);
FLUID_API void write_c(const char*, ...);
FLUID_API void write_h(const char*, ...);
FLUID_API void write_cstring(const char *);
FLUID_API void write_cstring(const char *,int length);
FLUID_API void write_carray(const char *);
FLUID_API void write_carray(const char *,int length);
FLUID_API void write_indent(int n);
FLUID_API void write_open(int);
FLUID_API void write_close(int n);
FLUID_API extern int write_number;
FLUID_API void write_public(int state); // writes pubic:/private: as needed
FLUID_API extern int indentation;
FLUID_API extern const char* indent();

FLUID_API int read_file(const char *, int merge);
FLUID_API const char *read_word(int wantbrace = 0);
FLUID_API void read_error(const char *format, ...);

// check legality of c code (sort of) and return error:
FLUID_API const char *c_check(const char *c, int type = 0);

// replace a string pointer with new value, strips leading/trailing blanks:
FLUID_API int storestring(const char *n, const char * & p, int nostrip=0);

FLUID_API extern int include_H_from_C;

//
// End of "$Id: Fl_Type.h,v 1.25 2000/09/05 17:36:20 spitzak Exp $".
//
