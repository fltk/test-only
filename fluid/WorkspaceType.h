//
// "$Id$"
//
// Workspace type header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Matthias Melcher and others.
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

#ifndef FLUID3_WORKSPACE_TYPE_H
#define FLUID3_WORKSPACE_TYPE_H

#include "Fl_Type.h"


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


#endif

//
// End of "$Id$".
//
