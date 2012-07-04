//
// "$Id$"
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

/*
 
 Fl_Type
 +- Fl_Tool_Type
 |  +- Fl_Workspace_Type
 |  +- Fl_Target_Dependency_Type
 |  +- Fl_Target_Type
 |  |  +- Fl_App_Target
 |  |  +- Fl_Lib_Target
 |  +- Fl_File_Type
 |  +- Fl_Folder_Type (is_folder, is_category)
 :  
 
 */

#ifndef FLUID_TYPE_H
#define FLUID_TYPE_H

#include <fltk3/Widget.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Menu.h>
#include <fltk3/Plugin.h>
#include "Fluid_Image.h"
#include <fltk3/run.h>
#include <fltk3/draw.h>

void set_modflag(int mf);

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

typedef int (Fl_Type::*RTTI_Query)() const;

class Fl_Tool_Type : public Fl_Type {  
  unsigned int pBuildEnv, pListEnv;
  int pNUUID, pnUUID;
  char **pUUIDName;
  char **pUUID;
  unsigned pFlags;
  
  void set_UUID(const char *name, const char *uuid);
  void set_UUID(int i, const char *uuid);
  int find_UUID(const char *name);
  static const char *verify_UUID(const char *uuid);
public:
  Fl_Tool_Type();
  ~Fl_Tool_Type();
  const char *get_UUID(const char *name);
  const char *get_UUID_Xcode(const char *name);
  void write_properties();
  char read_property(const char *);
  virtual int is_tool() const { return 1; }
  virtual int dnd_available();
  virtual int dnd_paste();
  
  void build_env(unsigned int e) { pBuildEnv = e; }
  unsigned int build_env() { return pBuildEnv; }
  int builds_in(unsigned int env) { return ((pBuildEnv&env)!=0); }
  
  void list_env(unsigned int e) { pListEnv = e; }
  unsigned int list_env() { return pListEnv; }
  int lists_in(unsigned int env) { return (((pBuildEnv|pListEnv)&env)!=0); }
  
  unsigned flags() { return pFlags; }
  void set_flags(unsigned v) { pFlags |= v; }
  void clear_flags(unsigned v) { pFlags &= ~v; }
};

class Fl_Option_Type : public Fl_Tool_Type {
  char *pValue;
  int pValueType;
public:
  Fl_Option_Type() :
    Fl_Tool_Type(),
    pValue(strdup("")),
    pValueType(0) {
  }
  ~Fl_Option_Type() {
    if (pValue) free(pValue);
  }
  const char *type_name() { return "tool_option"; }
  Fl_Type *make();
  virtual int is_option() const { return 1; }
  virtual int pixmapID() { return 60; }
  virtual void open();
  
  void write_properties();
  char read_property(const char *);

  const char *value() { return pValue; }
  void value(const char *);
  int value_type() { return pValueType; }
  void value_type(int v) { pValueType = v; }
};
extern Fl_Option_Type Fl_Option_type;


class Fl_Workspace_Type : public Fl_Tool_Type {
public:
  Fl_Workspace_Type() :
  Fl_Tool_Type() {
  }
  ~Fl_Workspace_Type() {
  }
  const char *type_name() { return "workspace"; }
  Fl_Type *make();
  virtual int is_parent() const { return 1; }
  virtual int is_workspace() const { return 1; }
  virtual int pixmapID() { return 58; }
  virtual void open();
};
extern Fl_Workspace_Type Fl_Workspace_type;

class Fl_Target_Dependency_Type : public Fl_Tool_Type {
public:
  Fl_Target_Dependency_Type() :
  Fl_Tool_Type() {
  }
  ~Fl_Target_Dependency_Type() {
  }
  const char *type_name() { return "target_dependency"; }
  Fl_Type *make();
  virtual int is_target_dependency() const { return 1; }
  virtual int pixmapID() { return 59; }
  virtual void open();
  static Fl_Target_Dependency_Type *first_dependency(Fl_Type *base);
  Fl_Target_Dependency_Type *next_dependency(Fl_Type *base);
};
extern Fl_Target_Dependency_Type Fl_Target_Dependency_type;

class Fl_Target_Type : public Fl_Tool_Type {
  char *pTargetPath;
  char *pMakefilePath;
public:
  Fl_Target_Type() :
  Fl_Tool_Type(),
  pTargetPath(0),
  pMakefilePath(0)
  {
    pTargetPath = strdup("");
    pMakefilePath = strdup("");
  }
  ~Fl_Target_Type() {
    if (pTargetPath) free(pTargetPath);
    if (pMakefilePath) free(pMakefilePath);
  }
  const char *type_name() { return "target"; }
  const char *caps_name();
  const char *lowercase_name();
  Fl_Type *make();
  virtual int is_parent() const { return 1; }
  virtual int is_target() const { return 1; }  
  static Fl_Target_Type *find(const char *name, char end=0);
  static Fl_Target_Type *first_target(Fl_Type *base);
  Fl_Target_Type *next_target(Fl_Type *base);
  void target_path(const char *path);
  const char *target_path() { return pTargetPath; }
  void makefile_path(const char *path);
  const char *makefile_path() { return pMakefilePath; }
  void write_properties();
  char read_property(const char *);
};
extern Fl_Target_Type Fl_Target_type;

class Fl_App_Target_Type : public Fl_Target_Type {
public:
  Fl_App_Target_Type() :
  Fl_Target_Type() {
  }
  ~Fl_App_Target_Type() {
  }
  const char *type_name() { return "app_target"; }
  Fl_Type *make();
  virtual int is_app_target() const { return 1; }
  virtual int pixmapID() { return 52; }
  virtual void open();
};
extern Fl_App_Target_Type Fl_App_Target_type;

class Fl_Lib_Target_Type : public Fl_Target_Type {
public:
  Fl_Lib_Target_Type() :
  Fl_Target_Type() {
  }
  ~Fl_Lib_Target_Type() {
  }
  const char *type_name() { return "lib_target"; }
  Fl_Type *make();
  virtual int is_lib_target() const { return 1; }
  virtual int pixmapID() { return 57; } // FIXME: new icon
  virtual void open();
};
extern Fl_Lib_Target_Type Fl_Lib_Target_type;

class Fl_File_Type : public Fl_Tool_Type {
  char *pFilename;
  FileType pFileType;
  FileLocation pFileLocation;
public:
  Fl_File_Type() :
    Fl_Tool_Type(),
    pFilename(0),
    pFileType(FL_FILE_UNKNOWN),
    pFileLocation(FL_LOCATION_WORKSPACE) {
  }
  ~Fl_File_Type() {
    if (pFilename) free(pFilename);
  }
  const char *type_name() { return "file_ref"; }
  Fl_Type *make();
  virtual int is_file() const { return 1; }
  virtual int pixmapID() { return 53; } // FIXME: draw icon
  void filename(const char *new_name);
  const char *filename() { return pFilename; }
  const char *filename_name();
  const char *filename_relative(const char *fnbase, const char *tgtbase);
  virtual void open();
  virtual void write_properties();
  virtual char read_property(const char *);
  static Fl_File_Type *first_file(Fl_Type *base);
  Fl_File_Type *next_file(Fl_Type *base);
  void set_default_type();
  FileType filetype() { return pFileType; }
  void filetype(unsigned int ft) { pFileType = (FileType)ft; }
  FileLocation location() { return pFileLocation; }
  void location(unsigned int fl) { pFileLocation = (FileLocation)fl; }
  char file_is_objc_code();
  char file_is_objc_header();
  char file_is_c_code();
  char file_is_c_header();
  char file_is_cplusplus_code();
  char file_is_cplusplus_header();
  char file_is_framework();
  char file_is_library();
  char file_is_fluid_ui();
  char file_is_code();
  char file_is_header();
  char file_is_other();
};
extern Fl_File_Type Fl_File_type;

class Fl_Folder_Type : public Fl_Tool_Type {
public:
  Fl_Folder_Type() :
  Fl_Tool_Type() {
  }
  ~Fl_Folder_Type() {
  }
  const char *type_name() { return "folder"; }
  Fl_Type *make();
  virtual int is_parent() const { return 1; }
  virtual int is_folder() const { 
    return parent && (parent->is_folder() || parent->is_target()); 
  }
  virtual int is_category() const { 
    return parent==0 || parent->is_workspace() || parent->is_category(); 
  }
  virtual int pixmapID() { return 54; }
  virtual void open();
};
extern Fl_Folder_Type Fl_Folder_type;

class Fl_Function_Type : public Fl_Type {
  const char* return_type;
  char public_, cdecl_, constructor, havewidgets;
public:
  Fl_Function_Type() : 
    Fl_Type(), 
    return_type(0L), public_(0), cdecl_(0), constructor(0), havewidgets(0)
  { }
  ~Fl_Function_Type() {
    if (return_type) free((void*)return_type);
  }
  Fl_Type *make();
  void write_code1();
  void write_code2();
  void open();
  int ismain() {return name_ == 0;}
  virtual const char *type_name() {return "Function";}
  virtual const char *alt_type_name() { return type_name(); }
  virtual const char *title() {
    return name() ? name() : "main()";
  }
  int is_parent() const {return 1;}
  int is_code_block() const {return 1;}
  virtual int is_public() const;
  int pixmapID() { return 7; }
  void write_properties();
  char read_property(const char *);
  int has_signature(const char *, const char*) const;
};

class Fl_Code_Type : public Fl_Type {
public:
  Fl_Type *make();
  void write_code1();
  void write_code2();
  void open();
  virtual const char *type_name() {return "code";}
  virtual const char *alt_type_name() { return type_name(); }
  int is_code_block() const {return 0;}
  int pixmapID() { return 8; }
  virtual int is_public() const;
};

class Fl_CodeBlock_Type : public Fl_Type {
  const char* after;
public:
  Fl_CodeBlock_Type() : Fl_Type(), after(0L) { }
  ~Fl_CodeBlock_Type() {
    if (after) free((void*)after);
  }
  Fl_Type *make();
  void write_code1();
  void write_code2();
  void open();
  virtual const char *type_name() {return "codeblock";}
  virtual const char *alt_type_name() { return type_name(); }
  int is_code_block() const {return 1;}
  int is_parent() const {return 1;}
  virtual int is_public() const;
  int pixmapID() { return 9; }
  void write_properties();
  char read_property(const char *);
};

class Fl_Decl_Type : public Fl_Type {
protected:
  char public_;
  char static_;
public:
  Fl_Type *make();
  void write_code1();
  void write_code2();
  void open();
  virtual const char *type_name() {return "decl";}
  virtual const char *alt_type_name() { return type_name(); }
  void write_properties();
  char read_property(const char *);
  virtual int is_public() const;
  int pixmapID() { return 10; }
};

class Fl_Data_Type : public Fl_Decl_Type {
  const char *filename_;
public:
  Fl_Data_Type() : Fl_Decl_Type(), filename_(0L) { }
  ~Fl_Data_Type() {
    if (filename_) free((void*)filename_);
  }
  Fl_Type *make();
  void write_code1();
  void write_code2();
  void open();
  virtual const char *type_name() {return "data";}
  virtual const char *alt_type_name() { return type_name(); }
  void write_properties();
  char read_property(const char *);
  int pixmapID() { return 49; }
};

class Fl_DeclBlock_Type : public Fl_Type {
  const char* after;
  char public_;
public:
  Fl_DeclBlock_Type() : Fl_Type(), after(0L) { }
  ~Fl_DeclBlock_Type() {
    if (after) free((void*)after);
  }
  Fl_Type *make();
  void write_code1();
  void write_code2();
  void open();
  virtual const char *type_name() {return "declblock";}
  virtual const char *alt_type_name() { return type_name(); }
  void write_properties();
  char read_property(const char *);
  int is_parent() const {return 1;}
  int is_decl_block() const {return 1;}
  virtual int is_public() const;
  int pixmapID() { return 11; }
};

class Fl_Comment_Type : public Fl_Type {
  char in_c_, in_h_, style_;
  char title_buf[64];
public:
  Fl_Type *make();
  void write_code1();
  void write_code2();
  void open();
  virtual const char *type_name() {return "comment";}
  virtual const char *alt_type_name() { return type_name(); }
  virtual const char *title(); // string for browser
  void write_properties();
  char read_property(const char *);
  virtual int is_public() const { return 1; }
  virtual int is_comment() const { return 1; }
  int pixmapID() { return 46; }
};

class Fl_Class_Type : public Fl_Type {
  const char* subclass_of;
  char public_;
public:
  Fl_Class_Type() : Fl_Type(), subclass_of(0L) { }
  ~Fl_Class_Type() {
    if (subclass_of) free((void*)subclass_of);
  }
  
  // state variables for output:
  char write_public_state; // true when public: has been printed
  Fl_Class_Type* parent_class; // save class if nested
//
  Fl_Type *make();
  void write_code1();
  void write_code2();
  void open();
  virtual const char *type_name() {return "class";}
  virtual const char *alt_type_name() { return type_name(); }
  virtual const char *include_path() { return "fltk3"; }
  int is_parent() const {return 1;}
  int is_decl_block() const {return 1;}
  int is_class() const {return 1;}
  virtual int is_public() const;
  int pixmapID() { return 12; }
  void write_properties();
  char read_property(const char *);

  // class prefix attribute access
  void prefix(const char* p);
  const char*  prefix() const {return class_prefix;}
  int has_function(const char*, const char*) const;
private:
  const char* class_prefix;
};

#define NUM_EXTRA_CODE 4

class Fl_Widget_Type : public Fl_Type {
  virtual fltk3::Widget *widget(int,int,int,int) = 0;
  virtual Fl_Widget_Type *_make() = 0; // virtual constructor
  virtual void setlabel(const char *);

  const char *extra_code_[NUM_EXTRA_CODE];
  const char *subclass_;
  const char *tooltip_;
  const char *image_name_;
  const char *inactive_name_;
  uchar hotspot_;

protected:

  void write_static();
  void write_code1();
  void write_widget_code();
  void write_extra_code();
  void write_block_close();
  void write_code2();
  void write_color(const char*, fltk3::Color);
  fltk3::Widget *live_widget;

public:
  static int default_size;

  const char *xclass; // junk string, used for shortcut
  fltk3::Widget *o;
  int public_;

  Fluid_Image *image;
  void setimage(Fluid_Image *);
  Fluid_Image *inactive;
  void setinactive(Fluid_Image *);

  Fl_Widget_Type();
  Fl_Type *make();
  void open();
  virtual const char *include_path() { return "fltk3"; }

  const char *extra_code(int n) const {return extra_code_[n];}
  void extra_code(int n,const char *);
  const char *subclass() const {return subclass_;}
  void subclass(const char *);
  const char *tooltip() const {return tooltip_;}
  void tooltip(const char *);
  const char *image_name() const {return image_name_;}
  void image_name(const char *);
  const char *inactive_name() const {return inactive_name_;}
  void inactive_name(const char *);
  uchar hotspot() const {return hotspot_;}
  void hotspot(uchar v) {hotspot_ = v;}
  uchar resizable() const;
  void resizable(uchar v);

  virtual int textstuff(int what, fltk3::Font &, int &, fltk3::Color &);
  virtual fltk3::MenuItem *subtypes();

  virtual int is_widget() const;
  virtual int is_public() const;

  virtual void write_properties();
  virtual char read_property(const char *);

  virtual fltk3::Widget *enter_live_mode(int top=0);
  virtual void leave_live_mode();
  virtual void copy_properties();

  virtual void ideal_size(int &w, int &h);
  virtual void ideal_spacing(int &x, int &y);

  ~Fl_Widget_Type();
  void redraw();
};

#include <fltk3/TabGroup.h>
#include <fltk3/PackedGroup.h>
#include <fltk3/WizardGroup.h>

class igroup : public fltk3::Group {
public:
  void resize(int,int,int,int);
  void full_resize(int X, int Y, int W, int H) { Group::resize(X, Y, W, H); }
  igroup(int X,int Y,int W,int H) : fltk3::Group(X,Y,W,H) {Group::current(0);}
};

class itabs : public fltk3::TabGroup {
public:
  void resize(int,int,int,int);
  void full_resize(int X, int Y, int W, int H) { Group::resize(X, Y, W, H); }
  itabs(int X,int Y,int W,int H) : fltk3::TabGroup(X,Y,W,H) {}
};

class iwizard : public fltk3::WizardGroup {
public:
  void resize(int,int,int,int);
  void full_resize(int X, int Y, int W, int H) { Group::resize(X, Y, W, H); }
  iwizard(int X,int Y,int W,int H) : fltk3::WizardGroup(X,Y,W,H) {}
};

class Fl_Group_Type : public Fl_Widget_Type {
public:
  virtual const char *type_name() {return "fltk3::Group";}
  virtual const char *alt_type_name() {return "Fl_Group";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    igroup *g = new igroup(X,Y,W,H); fltk3::Group::current(0); return g;}
  Fl_Widget_Type *_make() {return new Fl_Group_Type();}
  Fl_Type *make();
  void write_code1();
  void write_code2();
  void add_child(Fl_Type*, Fl_Type*);
  void move_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);
  int is_parent() const {return 1;}
  int is_group() const {return 1;}
  int pixmapID() { return 6; }

  virtual fltk3::Widget *enter_live_mode(int top=0);
  virtual void leave_live_mode();
  virtual void copy_properties();
};

extern const char pack_type_name[];
extern fltk3::MenuItem pack_type_menu[];

class Fl_Pack_Type : public Fl_Group_Type {
  fltk3::MenuItem *subtypes() {return pack_type_menu;}
public:
  virtual const char *type_name() {return pack_type_name;}
  virtual const char *alt_type_name() {return "Fl_Pack";}
  Fl_Widget_Type *_make() {return new Fl_Pack_Type();}
  int pixmapID() { return 22; }
  void copy_properties();
};

extern const char table_type_name[];

class Fl_Table_Type : public Fl_Group_Type {
public:
  virtual const char *type_name() {return table_type_name;}
  virtual const char *alt_type_name() {return "Fl_Table";}
  Fl_Widget_Type *_make() {return new Fl_Table_Type();}
  fltk3::Widget *widget(int X,int Y,int W,int H);
  int pixmapID() { return 51; }
  virtual fltk3::Widget *enter_live_mode(int top=0);
  void add_child(Fl_Type*, Fl_Type*);
  void move_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);
};

extern const char tabs_type_name[];

class Fl_Tabs_Type : public Fl_Group_Type {
public:
  virtual void ideal_spacing(int &x, int &y) {
     x = 10;
     fltk3::font(o->labelfont(), o->labelsize());
     y = fltk3::height() + o->labelsize() - 6;
  }
  virtual const char *type_name() {return tabs_type_name;}
  virtual const char *alt_type_name() {return "Fl_Tabs";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    itabs *g = new itabs(X,Y,W,H); fltk3::Group::current(0); return g;}
  Fl_Widget_Type *_make() {return new Fl_Tabs_Type();}
  Fl_Type* click_test(int,int);
  void add_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);
  int pixmapID() { return 13; }
  fltk3::Widget *enter_live_mode(int top=0);
};

extern const char scroll_type_name[];
extern fltk3::MenuItem scroll_type_menu[];

class Fl_Scroll_Type : public Fl_Group_Type {
  fltk3::MenuItem *subtypes() {return scroll_type_menu;}
public:
  virtual const char *type_name() {return scroll_type_name;}
  virtual const char *alt_type_name() {return "Fl_Scroll";}
  Fl_Widget_Type *_make() {return new Fl_Scroll_Type();}
  int pixmapID() { return 19; }
  fltk3::Widget *enter_live_mode(int top=0);
  void copy_properties();
};

extern const char tile_type_name[];

class Fl_Tile_Type : public Fl_Group_Type {
public:
  virtual const char *type_name() {return tile_type_name;}
  virtual const char *alt_type_name() {return "Fl_Tile";}
  Fl_Widget_Type *_make() {return new Fl_Tile_Type();}
  int pixmapID() { return 20; }
  void copy_properties();
};

extern const char wizard_type_name[];

class Fl_Wizard_Type : public Fl_Group_Type {
public:
  virtual const char *type_name() {return wizard_type_name;}
  virtual const char *alt_type_name() {return "Fl_Wizard";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    iwizard *g = new iwizard(X,Y,W,H); fltk3::Group::current(0); return g;}
  Fl_Widget_Type *_make() {return new Fl_Wizard_Type();}
  int pixmapID() { return 21; }
};


extern fltk3::MenuItem window_type_menu[];


class Fl_Window_Type : public Fl_Widget_Type {

  friend class Overlay_Window;

protected:

  enum {LEFT=1,RIGHT=2,BOTTOM=4,TOP=8,DRAG=16,BOX=32};

  int pCurrentMouseX, pCurrentMouseY;		// mouse position during dragging
  int pInitialMouseX, pInitialMouseY; // initial position of selection box
  fltk3::Rectangle pSelectionBox; // bounding box of selection before snapping
  int pDeltaX, pDeltaY;
  int pDragMode;		// which parts of bbox are being moved
  int pNumSelected;	// number of children selected
  int pRecalculateSelectionBox;		// set by fix_overlay()

public:
  
  uchar pIsModal, pIsNonModal;
  int pMinW, pMinH, pMaxW, pMaxH;
  
protected:
  
  fltk3::MenuItem* subtypes() {return window_type_menu;}
  void draw_overlay();
  void handle_drag_event();
  void newposition(Fl_Widget_Type *,int &x,int &y,int &w,int &h);
  int handle(int);
  virtual void setlabel(const char *);
  void write_code1();
  void write_code2();
  Fl_Widget_Type *_make() {return 0;} // we don't call this
  fltk3::Widget *widget(int,int,int,int) {return 0;}
  void move_all_children();
  int pixmapID() { return 1; }

public:

  Fl_Window_Type() 
  : pCurrentMouseX(0), pCurrentMouseY(0),
    pInitialMouseX(0), pInitialMouseY(0),
    pSelectionBox(0, 0, 0, 0),
    pDeltaX(0), pDeltaY(0),
    pDragMode(0),
    pNumSelected(0),
    pRecalculateSelectionBox(0),
    pIsModal(0), pIsNonModal(0),
    pMinW(0), pMinH(0), pMaxW(0), pMaxH(0)
  { }

  Fl_Type *make();
  virtual const char *type_name() {return "fltk3::Window";}
  virtual const char *alt_type_name() {return "Fl_Window";}

  void open();

  void fix_overlay();			// Update the bounding box, etc
  uchar *read_image(int &ww, int &hh);	// Read an image of the window

  virtual void write_properties();
  virtual char read_property(const char *);

  void add_child(Fl_Type*, Fl_Type*);
  void move_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);

  int is_parent() const {return 1;}
  int is_group() const {return 1;}
  int is_window() const {return 1;}

  fltk3::Widget *enter_live_mode(int top=0);
  void leave_live_mode();
  void copy_properties();
};


class Fl_Widget_Class_Type : private Fl_Window_Type {
public:
  Fl_Widget_Class_Type() {
    write_public_state = 0;
    wc_relative = 0;
  }
  // state variables for output:
  char write_public_state; // true when public: has been printed
  char wc_relative; // if true, reposition all child widgets in an fltk3::Group

  virtual void write_properties();
  virtual char read_property(const char *);

  void write_code1();
  void write_code2();
  Fl_Type *make();
  virtual const char *type_name() {return "widget_class";}
  virtual const char *alt_type_name() {return "widget_class";}
  int pixmapID() { return 48; }
  int is_parent() const {return 1;}
  int is_code_block() const {return 1;}
  int is_decl_block() const {return 1;}
  int is_class() const {return 1;}
};


extern fltk3::MenuItem menu_item_type_menu[];

class Fl_Menu_Item_Type : public Fl_Widget_Type {
public:
  fltk3::MenuItem* subtypes() {return menu_item_type_menu;}
  const char* type_name() {return "MenuItem";}
  const char* alt_type_name() {return "Fl_Menu_Item";}
  Fl_Type* make();
  int is_menu_item() const {return 1;}
  int is_button() const {return 1;} // this gets shortcut to work
  fltk3::Widget* widget(int,int,int,int) {return 0;}
  Fl_Widget_Type* _make() {return 0;}
  const char* menu_name(int& i);
  int flags();
  void write_static();
  void write_item();
  void write_code1();
  void write_code2();
  int pixmapID() { return 16; }
};

class Fl_Submenu_Type : public Fl_Menu_Item_Type {
public:
  fltk3::MenuItem* subtypes() {return 0;}
  const char* type_name() {return "Submenu";}
  const char* alt_type_name() {return "Submenu";}
  int is_parent() const {return 1;}
  int is_button() const {return 0;} // disable shortcut
  Fl_Type* make();
  // changes to submenu must propagate up so build_menu is called
  // on the parent Fl_Menu_Type:
  void add_child(Fl_Type*a, Fl_Type*b) {parent->add_child(a,b);}
  void move_child(Fl_Type*a, Fl_Type*b) {parent->move_child(a,b);}
  void remove_child(Fl_Type*a) {parent->remove_child(a);}
  int pixmapID() { return 18; }
};


#include <fltk3/Menu_.h>
class Fl_Menu_Type : public Fl_Widget_Type {
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
    fltk3::Menu_ *myo = (fltk3::Menu_*)(w==4 ? ((Fl_Widget_Type*)this->factory)->o : this->o);
    switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
    }
    return 1;
  }
public:
  int is_menu_button() const {return 1;}
  int is_parent() const {return 1;}
  int menusize;
  virtual void build_menu();
  Fl_Menu_Type() : Fl_Widget_Type() {menusize = 0;}
  ~Fl_Menu_Type() {
    if (menusize) delete[] (fltk3::MenuItem*)(((fltk3::Menu_*)o)->menu());
  }
  void add_child(Fl_Type*, Fl_Type*) {build_menu();}
  void move_child(Fl_Type*, Fl_Type*) {build_menu();}
  void remove_child(Fl_Type*) {build_menu();}
  Fl_Type* click_test(int x, int y);
  void write_code2();
  void copy_properties();
};

extern fltk3::MenuItem button_type_menu[];

#include <fltk3/MenuButton.h>
class Fl_Menu_Button_Type : public Fl_Menu_Type {
  fltk3::MenuItem *subtypes() {return button_type_menu;}
public:
  virtual void ideal_size(int &w, int &h) {
    Fl_Widget_Type::ideal_size(w, h);
    w += 2 * ((o->labelsize() - 3) & ~1) + o->labelsize() - 4;
    h = (h / 5) * 5;
    if (h < 15) h = 15;
    if (w < (15 + h)) w = 15 + h;
  }
  virtual const char *type_name() {return "fltk3::MenuButton";}
  virtual const char *alt_type_name() {return "Fl_Menu_Button";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    return new fltk3::MenuButton(X,Y,W,H,"menu");}
  Fl_Widget_Type *_make() {return new Fl_Menu_Button_Type();}
  int pixmapID() { return 26; }
};

extern fltk3::MenuItem dummymenu[];

#include <fltk3/Choice.h>
class Fl_Choice_Type : public Fl_Menu_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    Fl_Widget_Type::ideal_size(w, h);
    int w1 = o->h() - fltk3::box_dh(o->box());
    if (w1 > 20) w1 = 20;
    w1 = (w1 - 4) / 3;
    if (w1 < 1) w1 = 1;
    w += 2 * w1 + o->labelsize() - 4;
    h = (h / 5) * 5;
    if (h < 15) h = 15;
    if (w < (15 + h)) w = 15 + h;
  }
  virtual const char *type_name() {return "fltk3::Choice";}
  virtual const char *alt_type_name() {return "Fl_Choice";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    fltk3::Choice *myo = new fltk3::Choice(X,Y,W,H,"choice:");
    myo->menu(dummymenu);
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_Choice_Type();}
  int pixmapID() { return 15; }
};

#include <fltk3/InputChoice.h>
class Fl_Input_Choice_Type : public Fl_Menu_Type {
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
    fltk3::InputChoice *myo = (fltk3::InputChoice*)(w==4 ? ((Fl_Widget_Type*)this->factory)->o : this->o);
    switch (w) {
    case 4:
    case 0: f = (fltk3::Font)myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
    }
    return 1;
  }
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::InputChoice *myo = (fltk3::InputChoice *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h = fltk3::height() + myo->textsize() - 6;
    w = o->w() - 20 - fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + 20 + fltk3::box_dw(o->box());
    if (h < 15) h = 15;
    if (w < (15 + h)) w = 15 + h;
  }
  virtual const char *type_name() {return "fltk3::InputChoice";}
  virtual const char *alt_type_name() {return "Fl_Input_Choice";}
  virtual Fl_Type* click_test(int,int);
  fltk3::Widget *widget(int X,int Y,int W,int H) {
    fltk3::InputChoice *myo = new fltk3::InputChoice(X,Y,W,H,"input choice:");
    myo->menu(dummymenu);
    myo->value("input");
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_Input_Choice_Type();}
  virtual void build_menu();
  int pixmapID() { return 15; }
  void copy_properties();
};

#include <fltk3/Window.h>
#include <fltk3/MenuBar.h>
class Fl_Menu_Bar_Type : public Fl_Menu_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    w = o->window()->w();
    h = ((o->labelsize() + fltk3::box_dh(o->box()) + 4) / 5) * 5;
    if (h < 15) h = 15;
  }
  virtual const char *type_name() {return "fltk3::MenuBar";}
  virtual const char *alt_type_name() {return "Fl_Menu_Bar";}
  fltk3::Widget *widget(int X,int Y,int W,int H) {return new fltk3::MenuBar(X,Y,W,H);}
  Fl_Widget_Type *_make() {return new Fl_Menu_Bar_Type();}
  int pixmapID() { return 17; }
};
// object list operations:
fltk3::Widget *make_widget_browser(int X,int Y,int W,int H);
extern int modflag;
void delete_all(int selected_only=0);
void selection_changed(Fl_Type* new_current);
void reveal_in_browser(Fl_Type*);
int has_toplevel_function(const char *rtype, const char *sig);

// file operations:
#  ifdef __GNUC__
#    define __fl_attr(x) __attribute__ (x)
#  else
#    define __fl_attr(x)
#  endif // __GNUC__

void write_word(const char *);
void write_string(const char *,...) __fl_attr((__format__ (__printf__, 1, 2)));
int write_file(const char *, int selected_only = 0);
int write_code(const char *cfile, const char *hfile);
int write_strings(const char *sfile);

int write_declare(const char *, ...) __fl_attr((__format__ (__printf__, 1, 2)));
int is_id(char);
const char* unique_id(void* o, const char*, const char*, const char*);
void write_c(const char*, ...) __fl_attr((__format__ (__printf__, 1, 2)));
void write_h(const char*, ...) __fl_attr((__format__ (__printf__, 1, 2)));
void write_cstring(const char *);
void write_cstring(const char *,int length);
void write_cdata(const char *,int length);
void write_indent(int n);
void write_open(int);
void write_close(int n);
extern int write_number;
extern int write_sourceview;
void write_public(int state); // writes pubic:/private: as needed
extern int indentation;
extern const char* indent();

int read_file(const char *, int merge);
const char *read_word(int wantbrace = 0);
void read_error(const char *format, ...);

// check legality of c code (sort of) and return error:
const char *c_check(const char *c, int type = 0);

// replace a string pointer with new value, strips leading/trailing blanks:
int storestring(const char *n, const char * & p, int nostrip=0);

extern int include_H_from_C;
extern int use_FL_COMMAND;

/*
 * This class is needed for additional command line plugins.
 */
class Fl_Commandline_Plugin : public fltk3::Plugin {
public:
  Fl_Commandline_Plugin(const char *name)
  : fltk3::Plugin(klass(), name) { }
  virtual const char *klass() { return "commandline"; }
  // return a unique name for this plugin
  virtual const char *name() = 0;
  // return a help text for all supported commands
  virtual const char *help() = 0;
  // handle a command and return the number of args used, or 0
  virtual int arg(int argc, char **argv, int &i) = 0;
  // optional test the plugin
  virtual int test(const char *a1=0L, const char *a2=0L, const char *a3=0L) { 
    return 0;
  }
  // show a GUI panel to edit some data
  virtual void show_panel() { }
};

/*
 * This is a base class for all panels that allow editing multiple items.
 * Currently, this is the widget panel (the_panel) and the file panel
 */
class Fl_Panel : public fltk3::DoubleWindow {
  static Fl_Type *pSelectedType;
public:
  Fl_Panel(int x, int y, int w, int h, const char *name=0L);
  ~Fl_Panel();
  void load(RTTI_Query query);
  
  static void *const LOAD;
  static int numselected;
  static void select_type(Fl_Type *t) { 
    pSelectedType = t; 
  }
  static Fl_Type *selected_type() {
    return pSelectedType;
  }
  static Fl_Widget_Type *selected_widget() { 
    return (pSelectedType && pSelectedType->is_widget())?(Fl_Widget_Type*)pSelectedType:0L; 
  }
  static Fl_File_Type *selected_file() { 
    return (pSelectedType && pSelectedType->is_file())?(Fl_File_Type*)pSelectedType:0L; 
  }
  static Fl_Tool_Type *selected_tool() { 
    return (pSelectedType && pSelectedType->is_tool())?(Fl_File_Type*)pSelectedType:0L; 
  }
  static void propagate_load(fltk3::Group* g, void* v=LOAD);
};


#endif

//
// End of "$Id$".
//
