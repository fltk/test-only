//
// "$Id$"
//
// Function type header file for the Fast Light Tool Kit (FLTK).
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

#ifndef FLUID3_FUNCTION_TYPE_H
#define FLUID3_FUNCTION_TYPE_H

#include "Fl_Type.h"


extern class Fl_Class_Type *current_class;


class Fl_Function_Type : public Fl_Type 
{
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


#endif

//
// End of "$Id$".
//
