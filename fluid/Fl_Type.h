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

#ifndef FLUID3_TYPE_H
#define FLUID3_TYPE_H

#include <fltk3/Widget.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Menu.h>
#include <fltk3/Plugin.h>
#include "Fluid_Image.h"
#include <fltk3/run.h>
#include <fltk3/draw.h>

#  ifdef __GNUC__
#    define __fl_attr(x) __attribute__ (x)
#  else
#    define __fl_attr(x)
#  endif // __GNUC__


// -- Types

class Fl_Type;

enum { 
  FL_ENV_NONE=0, 
      FL_ENV_MAKE = 0x0001, 
      FL_ENV_CMAKE = 0x0002, 
    FL_ENV_ALL_SHELL = (FL_ENV_MAKE | FL_ENV_CMAKE), 
      FL_ENV_VC6 = 0x0004, 
      FL_ENV_VC2008 = 0x0008, 
      FL_ENV_VC2010 = 0x0010,
    FL_ENV_ALL_VC = (FL_ENV_VC6 | FL_ENV_VC2008 | FL_ENV_VC2010), 
      FL_ENV_XC4 = 0x0040,
    FL_ENV_ALL_XC = (FL_ENV_XC4), 
  FL_ENV_ALL = (FL_ENV_ALL_SHELL | FL_ENV_ALL_VC | FL_ENV_ALL_XC)
}; 

typedef enum {
  FL_FILE_EXPLICIT = 0x8000,
  FL_FILE_UNKNOWN = 0,
  FL_FILE_C_SOURCE, FL_FILE_C_HEADER,
  FL_FILE_CPP_SOURCE, FL_FILE_CPP_HEADER,
  FL_FILE_OBJC_SOURCE, FL_FILE_OBJC_HEADER,
  FL_FILE_TEXT, FL_FILE_TEXT_SCRIPT,
  FL_FILE_FRAMEWORK, FL_FILE_FLUID_UI,
  FL_FILE_LIBRARY
} FileType;

typedef enum {
  FL_LOCATION_WORKSPACE = 0,
  FL_LOCATION_ABSOLUTE,
  FL_LOCATION_IDE,
  FL_LOCATION_SDK
} FileLocation;

typedef enum {
  FL_OPTION_OTHER = 0,
  FL_OPTION_COMPILER,
  FL_OPTION_LINKER
} OptionType;

typedef int (Fl_Type::*RTTI_Query)() const;


// -- Variables

extern int include_H_from_C;
extern int use_FL_COMMAND;
extern int modflag;


// -- Functions

void delete_all(int selected_only=0);
void selection_changed(Fl_Type* new_current);
void reveal_in_browser(Fl_Type*);
int has_toplevel_function(const char *rtype, const char *sig);

// file operations

int write_declare(const char *, ...) __fl_attr((__format__ (__printf__, 1, 2)));

// check legality of c code (sort of) and return error:
const char *c_check(const char *c, int type = 0);

// replace a string pointer with new value, strips leading/trailing blanks:
int storestring(const char *n, const char * & p, int nostrip=0);


// -- Classes

class Fl_Type {
  
  friend class Widget_Browser;
  friend fltk3::Widget *make_type_browser(int,int,int,int,const char *l=0);
  friend class Fl_Window_Type;
  virtual void setlabel(const char *); // virtual part of label(char*)
  
protected:
  
  Fl_Type();
  
  const char *name_;
  const char *label_;
  const char *callback_;
  const char *user_data_;
  const char *user_data_type_;
  const char *comment_;
  
public:	// things that should not be public:
  
  Fl_Type *parent; // parent, which is previous in list
  char new_selected; // browser highlight
  char selected; // copied here by selection_changed()
  char open_;	// state of triangle in browser
  char visible; // true if all parents are open
  char rtti;	// hack because I have no rtti, this is 0 for base class
  int level;	// number of parents over this
  static Fl_Type *first, *last; // linked list of all objects
  Fl_Type *next, *prev;	// linked list of all objects
  Fl_Type *first_child();
  Fl_Type *next_brother();
  
  Fl_Type *factory;
  const char *callback_name();
  
  int code_position, header_position;
  int code_position_end, header_position_end;
  
protected:
  int user_defined(const char* cbname) const;
  
public:
  
  virtual ~Fl_Type();
  virtual Fl_Type *make() = 0;
  
  void add(Fl_Type *parent); // add as new child
  void insert(Fl_Type *n); // insert into list before n
  Fl_Type* remove();	// remove from list
  void move_before(Fl_Type*); // move before a sibling
  
  virtual const char *title(); // string for browser
  virtual const char *type_name() = 0; // type for code output
  virtual const char *alt_type_name() { return type_name(); } // alternate type for FLTK1 file reading
  virtual const char *include_path() { return 0; }
  
  const char *name() const {return name_;}
  void name(const char *);
  const char *label() const {return label_;}
  void label(const char *);
  const char *callback() const {return callback_;}
  void callback(const char *);
  const char *user_data() const {return user_data_;}
  void user_data(const char *);
  const char *user_data_type() const {return user_data_type_;}
  void user_data_type(const char *);
  const char *comment() { return comment_; }
  void comment(const char *);
  
  virtual Fl_Type* click_test(int,int);
  virtual void add_child(Fl_Type*, Fl_Type* beforethis);
  virtual void move_child(Fl_Type*, Fl_Type* beforethis);
  virtual void remove_child(Fl_Type*);
  
  static Fl_Type *current;  // most recently picked object
  virtual void open();	// what happens when you double-click
  
  // read and write data to a saved file:
  void write();
  virtual void write_properties();
  virtual char read_property(const char *);
  
  // write code, these are called in order:
  virtual void write_static(); // write static stuff to .c file
  virtual void write_code1(); // code and .h before children
  virtual void write_code2(); // code and .h after children
  void write_comment_h(const char *ind=""); // write the commentary text into the header file
  void write_comment_c(const char *ind=""); // write the commentary text into the source file
  
  // live mode
  virtual fltk3::Widget *enter_live_mode(int top=0); // build wdgets needed for live mode
  virtual void leave_live_mode(); // free allocated resources
  virtual void copy_properties(); // copy properties from this type into a potetial live object
  
  // get message number for I18N
  int msgnum();
  
  // fake rtti:
  virtual int is_parent() const;
  virtual int is_widget() const;
  virtual int is_button() const;
  virtual int is_input() const;
  virtual int is_value_input() const;
  virtual int is_text_display() const;
  virtual int is_valuator() const;
  virtual int is_spinner() const;
  virtual int is_menu_item() const;
  virtual int is_menu_button() const;
  virtual int is_group() const;
  virtual int is_window() const;
  virtual int is_code_block() const;
  virtual int is_decl_block() const;
  virtual int is_comment() const;
  virtual int is_class() const;
  virtual int is_public() const;
  virtual int is_target() const { return 0; }
  virtual int is_lib_target() const { return 0; }
  virtual int is_app_target() const { return 0; }
  virtual int is_file() const { return 0; }
  virtual int is_fluid_file() const { return 0; }
  virtual int is_folder() const { return 0; }
  virtual int is_category() const { return 0; }
  virtual int is_tool() const { return 0; }
  virtual int is_workspace() const { return 0; }
  virtual int is_target_dependency() const { return 0; }
  virtual int is_option() const { return 0; }
  
  virtual int pixmapID() { return 0; }
  
  virtual int dnd_available() { return 0; }
  virtual int dnd_paste() { return 0; }
  
  const char* class_name(const int need_nest) const;
  const class Fl_Class_Type* is_in_class() const;
};


#endif

//
// End of "$Id$".
//
