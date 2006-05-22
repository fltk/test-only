// "$Id$"
//
// Base type of object constructed and manipualated by the
// Fast Light toolkit Interface Designer (fluid)
//
// Each object described by Fluid is one of these objects.  They
// are all stored in a double-linked list.
//
// There is also a single "factory" instance of each type of this.
// The method "make()" is called on this factory to create a new
// instance of this object.  It could also have a "copy()" function,
// but it was easier to implement this by using the file read/write
// that is needed to save the setup anyways.
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

#ifndef fltk_fluid_type_h
#define fltk_fluid_type_h

#include <fltk/Widget.h>
#include <fltk/Color.h>
#include <fltk/Preferences.h>
#include "Fluid_Plugins.h"
#include "Fluid_Image.h"

class FLUID_API FluidType {

  friend class Widget_Browser;
  friend fltk::Widget *make_type_browser(int,int,int,int,const char *l=0);
  friend class WindowType;
  virtual void setlabel(const char *); // virtual part of label(char*)

public:
  virtual void prefix(const char* p) {}
  virtual const char*  prefix() const {return 0;}
  virtual int pixmapID() {return -1;}

protected:

  FluidType();

  const char *name_;
  const char *label_;
  const char *tooltip_;
  const char *callback_;
  const char *user_data_;
  const char *user_data_type_;
  fltk::Widget *live_widget;

public:	// things that should not be public:

  FluidType* parent;
  FluidType* first_child;
  FluidType* next_brother;
  FluidType* previous_brother;

  FluidType* walk(const FluidType* top) const;
  FluidType* walk() const;

  static FluidType *first;
  
  FluidType *factory;
  const char *callback_name();

  bool open_;	// open/close state of this parent in browser

  static int selected_count() {return selected_count_;}
  bool selected() const {return selected_;}
  void selected(bool s) {
      if (s==selected_) return;
      if (s ) ++selected_count_; else --selected_count_; 
      selected_=s;
  }

  bool new_selected; // browser highlight
private:
  bool selected_; // copied here by selection_changed()
  static int selected_count_;

public:

  virtual ~FluidType();
  virtual FluidType *make() = 0;

  void add(FluidType *parent); // add as new child
  void insert(FluidType *n); // insert into list before n
  void remove(); // remove from list
  void move_before(FluidType*); // move before a sibling

  virtual const char *title(); // string for browser
  virtual const char *type_name() const = 0; // type for code output

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

  virtual FluidType* click_test(int,int);
  virtual void add_child(FluidType*, FluidType* beforethis);
  virtual void move_child(FluidType*, FluidType* beforethis);
  virtual void remove_child(FluidType*);

  static FluidType *current;  // most recently picked object
  virtual void open();	// what happens when you double-click

  // read and write data to a saved file:
  void write();
  virtual void write_properties();
  virtual void read_property(const char *);
  virtual int read_fdesign(const char*, const char*);

  // write code, these are called in order:
  virtual void write_static(); // write static stuff to .c file
  virtual void write_code();   // write .h and .c file

  // live mode
  virtual fltk::Widget *enter_live_mode(int top=0); // build widgets needed for live mode
  virtual void leave_live_mode(); // free allocated resources
  virtual void copy_properties(); // copy properties from this type into a potetial live object

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
  virtual int is_comment() const;
  virtual int is_class() const;
  virtual int is_browser() const;
  virtual int is_input() const;
  virtual int is_value_input() const;
  virtual int is_value_output() const;
  virtual int is_value_slider() const;

  const char* member_of(bool need_nest = false) const;
  // codeview
  int code_line, header_line;
  int code_line_end, header_line_end;

};

////////////////////////////////////////////////////////////////

// This structure is used to define tables of enumerations:

struct Enumeration {
  const char* menu_entry;	// user-friendly string, null for end of table
  const char* symbol;		// symbol for c++ code and for .fl file
  void* compiled;		// symbol compiled for use by fluid
  const char* subclass;		// For type() of widgets, use this subclass
};

// Use this call to make a menu/Choice from a table. Warning this
// will overwrite the user_data() of the fltk::Menu:
namespace fltk {class Menu;}
void set_menu(fltk::Menu*, const Enumeration*);

// Converters from/to strings and values:
const Enumeration* from_value(void* data, const Enumeration* table);
const Enumeration* from_value(int data, const Enumeration* table);
const Enumeration* from_text(const char* text, const Enumeration* table);
const char* to_text(void* data, const Enumeration* table);
int number_from_text(const char* text, const Enumeration* table);
const char* number_to_text(int number, const Enumeration* table);
const int FLUID_MAX_IMG=4;
////////////////////////////////////////////////////////////////

class FLUID_API WidgetType : public FluidType {
  virtual fltk::Widget *widget(int,int,int,int);
  virtual WidgetType *_make(); // virtual constructor
  virtual void setlabel(const char *);

  const char *extra_code_;
  const char *user_class_;
  bool hotspot_;

protected:

  void write_static();
  void write_code();
  void write_code1();
  void write_widget_code();
  void write_extra_code();
  void write_block_close();

public:

  bool set_xy;
  fltk::Widget *o;
  bool public_;
  
  Fluid_Image* image[FLUID_MAX_IMG]; // updated image number to four seems enough
			 // for more han 4 combinations use a MultiImage
			 // and set the default image to it
  void set_image(Fluid_Image* i,int num);

  WidgetType();
  FluidType *make();
  void open();

  const char *extra_code() const {return extra_code_;}
  void extra_code(const char *);
  const char *user_class() const {return user_class_;}
  void user_class(const char *);
  bool hotspot() const {return hotspot_;}
  void hotspot(bool v) {hotspot_ = v;}
  bool resizable() const;
  void resizable(bool v);

  virtual const Enumeration* subtypes() const;

  virtual int is_widget() const;

  virtual void write_properties();
  virtual void read_property(const char *);
  virtual int read_fdesign(const char*, const char*);

  virtual ~WidgetType();
  void redraw();

  const char* subclass() const;
  const char* array_name() const;

  virtual const char *type_name() const;

  // live mode functionalities
  fltk::Widget *enter_live_mode(int top);
  void leave_live_mode();
  void copy_properties();
  
  virtual int textstuff(int w, fltk::Font* f, float& s, fltk::Color c) {
	fltk::Widget *myo = (fltk::Widget *)(w==4 ? ((WidgetType*)factory)->o : o);
	switch (w) {
	case 4:
	case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
	case 1: myo->textfont(f); break;
	case 2: myo->textsize((float)s); break;
	case 3: myo->textcolor(c); break;
	}
	return 1;
    }

  static float default_size;
  int pixmapID() { return 5;}
  
};

#define LOAD ((void*)9831)

FLUID_API extern WidgetType *current_widget; // one of the selected ones

////////////////////////////////////////////////////////////////

class FLUID_API GroupType : public WidgetType {
public:
  const Enumeration* subtypes() const;
  virtual ~GroupType();
  virtual const char *type_name() const;
  fltk::Widget *widget(int x,int y,int w,int h);
  WidgetType* _make();
  FluidType *make();
  void write_code();
  void add_child(FluidType*, FluidType*);
  void move_child(FluidType*, FluidType*);
  void remove_child(FluidType*);
  int is_parent() const;
  int is_group() const;

  // live mode functionalities
  fltk::Widget *enter_live_mode(int top);
  void leave_live_mode();
  void copy_properties();
  int pixmapID() { return 6; }
};

////////////////////////////////////////////////////////////////

//class FLUID_API WindowType : public WidgetType {
class FLUID_API WindowType : public GroupType {
  const Enumeration* subtypes() const;

  friend class Overlay_Window;
  int mx,my;		// mouse position during dragging
  int x1,y1;		// initial position of selection box
  int bx,by,br,bt;	// bounding box of selection
  int dx,dy;
  int recalc;		// set by fix_overlay()
public:
  int drag;		// which parts of bbox are being moved
  int numselected;	// number of children selected

  enum {LEFT=1,RIGHT=2,BOTTOM=4,TOP=8,DRAG=16,BOX=32};
  void draw_overlay();
  void newdx();
  void newposition(WidgetType *,int &x,int &y,int &w,int &h);
  int handle(int);
  virtual void setlabel(const char *);
  void write_code();
  WidgetType *_make() {return 0;} // we don't call this
  fltk::Widget *widget(int,int,int,int) {return 0;}
  void moveallchildren();
  void move_children(FluidType*, int);
  WidgetType* clicked_widget();

public:
  WindowType();

  bool modal, non_modal, border;

  FluidType *make();
  virtual const char *type_name() const;

  void open();

  void fix_overlay();	// update the bounding box, etc

  virtual void write_properties();
  virtual void read_property(const char *);
  virtual int read_fdesign(const char*, const char*);

  void add_child(FluidType*, FluidType*);
  void move_child(FluidType*, FluidType*);
  void remove_child(FluidType*);

  int is_parent() const {return 1;}
  int is_group() const {return 1;}
  int is_window() const {return 1;}

  // live mode functionalities
  fltk::Widget *enter_live_mode(int top);
  void leave_live_mode();
  void copy_properties();
  
  int sr_min_w, sr_min_h, sr_max_w, sr_max_h; // size_range related

  int pixmapID() { return 1; }
};

class FLUID_API WidgetClassType : private WindowType {
public:
  WidgetClassType() {write_public_state = false;  wc_relative = false;}
  // state variables for output:
  bool write_public_state; // true when public: has been printed
  bool wc_relative; // if true, reposition all child widgets in an Fl_Group

  virtual void write_properties();
  virtual void read_property(const char *);

  void write_code();
  void write_code1();
  FluidType *make();
  virtual const char *type_name() const  {return "fltk::WidgetClass";}
  int is_parent() const {return 1;}
  int is_decl_block() const {return 1;}
  int is_class() const {return 1;}
  int pixmapID() { return 48; }
};


////////////////////////////////////////////////////////////////
// This header file also declares all the global functions in fluid:

// object list operations:
fltk::Widget *make_widget_browser(int x,int y,int w,int h);
void delete_all(int selected_only=0);
void selection_changed(FluidType* new_current);

// file operations:
FLUID_API void write_word(const char *);
FLUID_API void write_string(const char *,...);
FLUID_API int write_file(const char *, int selected_only = 0);
FLUID_API int write_code(const char *cfile, const char *hfile);

FLUID_API int write_declare(const char *, ...);
FLUID_API int is_id(char);
FLUID_API const char* unique_id(void* o, const char*, const char*, const char*);
FLUID_API void write_craw(const char* string);
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

extern double read_version;

// check legality of c code (sort of) and return error:
FLUID_API const char *c_check(const char *c, int type = 0);

// replace a string pointer with new value, strips leading/trailing blanks:
FLUID_API int storestring(const char *n, const char * & p, int nostrip=0);

FLUID_API extern bool include_H_from_C;
FLUID_API void select(FluidType* it, int value);
FLUID_API void select_only(FluidType *);
FLUID_API void refresh_browser_views();
FLUID_API void initialize_tab_colors();

extern int modflag;

#endif
//
// End of "$Id$".
//
