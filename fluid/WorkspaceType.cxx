//
// "$Id$"
//
// Workspace type code for the Fast Light Tool Kit (FLTK).
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

#include "WorkspaceType.h"
#include "workspace_panel.h"
#include "fluid.h"
#include "panel.h"
#include "file.h"

#include <fltk3/filename.h>
#include <fltk3/ask.h>


//extern Fl_Panel *the_workspace_panel;
//extern char *get_temporary_return_buffer(int size);
//
//extern const char *filename;
//extern void set_filename(const char*);
//extern char project_is_workspace();


// ------------ Tool -----------------------------------------------------------

Fl_Tool_Type::Fl_Tool_Type()
: Fl_Type(),
  pBuildEnv(FL_ENV_ALL),
  pListEnv(FL_ENV_ALL),
  pNUUID(0), pnUUID(0), 
  pUUIDName(0L), pUUID(0),
  pFlags(0)
{
}

Fl_Tool_Type::~Fl_Tool_Type() {
  int i;
  for (i=0; i<pnUUID; i++) {
    free(pUUIDName[i]);
    free(pUUID[i]);
  }
  if (pNUUID) {
    free(pUUIDName);
    free(pUUID);
  }
}

int Fl_Tool_Type::find_UUID(const char *name) {
  int i;
  for (i=0; i<pnUUID; i++) {
    if (strcmp(pUUIDName[i], name)==0)
      return i;
  }
  return -1;
}

void Fl_Tool_Type::set_UUID(int i, const char *uuid) {
  if (pUUID[i]) 
    free(pUUID[i]);
  pUUID[i] = 0;
  pUUID[i] = strdup(verify_UUID(uuid));
}

void Fl_Tool_Type::set_UUID(const char *name, const char *uuid) {
  int i = find_UUID(name);
  if (i==-1) {
    if (pNUUID==pnUUID) {
      pNUUID += 4;
      pUUIDName = (char**)realloc(pUUIDName, pNUUID*sizeof(char*));
      pUUID = (char**)realloc(pUUID, pNUUID*sizeof(char*));
    }
    pUUIDName[pnUUID] = strdup(name);
    pUUID[pnUUID] = strdup(verify_UUID(uuid));
    pnUUID++;
  } else {
    set_UUID(i, uuid);
  }
}

/*
 Verify that a Uniqe ID is truly unique.
 */
const char *Fl_Tool_Type::verify_UUID(const char *uuid) {
  Fl_Type *t;
  for (t = first; t; t = t->next) {
    if (t->is_tool()) {
      Fl_Tool_Type *wt = (Fl_Tool_Type*)t;
      int j, n = wt->pnUUID;
      for (j=0; j<n; j++) {
        if (wt->pUUID[j] && strcmp(wt->pUUID[j], uuid)==0) {
          printf("Replacing duplicate UUIDin %s!\n", wt->name());
          return fltk3::Preferences::newUUID();
        }
      }
    }
  }
  return uuid;
}

const char *Fl_Tool_Type::get_UUID(const char *name) {
  int i = find_UUID(name);
  if (i==-1) {
    i = pnUUID;
    const char *uuid = fltk3::Preferences::newUUID();
    set_UUID(name, uuid);
  }
  return pUUID[i];
}

const char *Fl_Tool_Type::get_UUID_Xcode(const char *name) {
  char *buf = get_temporary_return_buffer(25);
  const char *uuid = get_UUID(name);
  // 937C4900-51AA-4C11-8DD3-7AB5 9944F03E
  unsigned int a, b, c, d, e, f;
  sscanf(uuid, "%08X-%04X-%04X-%04X-%04X%08X", &a, &b, &c, &d, &e, &f);
  sprintf(buf, "%08X%04X%04X%08X", a, b^c, d^e, f);
  return buf;
}

void Fl_Tool_Type::write_properties() {
  Fl_Type::write_properties();
  int i;
  for (i=0; i<pnUUID; i++) {
    char buf[80];
    strcpy(buf, "uuid_");
    strcat(buf, pUUIDName[i]); 
    write_indent(level+1);
    write_word(buf);
    write_word(pUUID[i]);
  }
  if (build_env()!=FL_ENV_ALL) {
    write_indent(level+1);
    write_string("build_env %d", build_env());
  }
  if (list_env()!=FL_ENV_ALL) {
    write_indent(level+1);
    write_string("list_env %d", list_env());
  }
}

char Fl_Tool_Type::read_property(const char *name) {
  if (strncmp(name, "uuid_", 5)==0) {
    char buf[80];
    strcpy(buf, name+5);
    set_UUID(buf, read_word());
  } else if (!strcmp(name,"environments")) { // FIXME: delete this
    int v = atoi(read_word());
    build_env(v); list_env(v);
  } else if (!strcmp(name,"build_env")) {
    build_env(atoi(read_word()));
  } else if (!strcmp(name,"list_env")) {
    list_env(atoi(read_word()));
  } else {
    return Fl_Type::read_property(name);
  }
  return 1;
}

int Fl_Tool_Type::dnd_available() {
  // FIXME: we should maybe test if the objects dropped are actually valid file names
  if (filename && *filename && (is_target() || is_folder()))
    return 1;
  return 0;
}

int Fl_Tool_Type::dnd_paste() {
  if (filename && *filename && (is_target() || is_folder())) {
    if (fltk3::event_text() && *fltk3::event_text()) {
      char *basedir = strdup(filename); // global name of workspace file!
      char *fn = (char*)fltk3::filename_name(basedir);
      if (fn) *fn = 0;
      char *files = strdup(fltk3::event_text()), *s = files, *e = s;
      char done = 0;
      for (;!done;) {
        // find the end of the line
        for (;;) {
          if (*e==0) { done = 1; break; }
          if (*e=='\n') { *e++ = 0; break; }
          e++;
        }
        // add the file 's' to this item
        if (e!=s) {
          char buf[FLTK3_PATH_MAX];
          fltk3::filename_relative(buf, FLTK3_PATH_MAX, s, basedir);
          Fl_File_Type *o = new Fl_File_Type();
          o->filename(buf);
          o->add(this);
          o->factory = this;
        }
        s = e;
      }
      free(files);
      free(basedir);
    }
  }
  return 1;
}

// ------------ Workspace ------------------------------------------------------

Fl_Workspace_Type Fl_Workspace_type;

Fl_Type *Fl_Workspace_Type::make() {
  // A workspace must be the first entry
  if (Fl_Type::first) {
    fltk3::message("A Workspace can only be added to an empty project.");
    return 0;
  }
  // add the workspace to the file
  Fl_Workspace_Type *o = new Fl_Workspace_Type();
  o->name("myWorkspace");
  o->add(0L);
  o->factory = this;
  return o;
}

void Fl_Workspace_Type::open() {
  if (!the_workspace_panel) the_workspace_panel = make_workspace_panel();
  the_workspace_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_workspace_panel->show();
}


// ------------ Workspace Option -----------------------------------------------

Fl_Option_Type Fl_Option_type;

Fl_Type *Fl_Option_Type::make() {
  // A workspace must be the first entry
  if (!project_is_workspace()) {
    fltk3::message("This element can only be added into a workspace.");
    return 0;
  }
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_folder() && !p->is_target() && !p->is_workspace())
    p = p->parent;  
  if (!p) {
    fltk3::message("This element can only be added to a Target, Folder, or Workspace!");
    return 0;
  }
  // add the workspace to the file
  Fl_Option_Type *o = new Fl_Option_Type();
  o->name("myOption");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_Option_Type::open() {
  if (!the_workspace_panel) the_workspace_panel = make_workspace_panel();
  the_workspace_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_workspace_panel->show();
}

void Fl_Option_Type::value(const char *v) {
  if (pValue) {
    free(pValue);
    pValue = 0L;
  }
  if (v) {
    pValue = strdup(v);
  } else {
    pValue = strdup("");
  }
}

char Fl_Option_Type::read_property(const char *c) {
  if (!strcmp(c,"value")) {
    value(read_word());
  } else if (!strcmp(c,"valuetype")) {
    value_type(atoi(read_word()));
  } else {
    return Fl_Tool_Type::read_property(c);
  }
  return 1;
}

void Fl_Option_Type::write_properties() {
  Fl_Tool_Type::write_properties();
  if (value() && *value()) {
    write_indent(level+1);
    write_string("value");
    write_word(value());
  }
  if (value_type()!=FL_OPTION_OTHER) {
    write_indent(level+1);
    write_string("valuetype %d", value_type());
  }
}


// ------------ Target Dependency ----------------------------------------------

Fl_Target_Dependency_Type Fl_Target_Dependency_type;

Fl_Type *Fl_Target_Dependency_Type::make() {
  // A workspace must be the first entry
  if (!project_is_workspace()) {
    fltk3::message("A Target Dependency can only be added to another Target in a Workspace.");
    return 0;
  }
  // add the dependency to the target
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_folder() && !p->is_target())
    p = p->parent;  
  if (!p) {
    fltk3::message("A Target Dependency reference can only be added to a Target or Folder!");
    return 0;
  }
  Fl_Target_Dependency_Type *o = new Fl_Target_Dependency_Type();
  o->name("otherTarget");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_Target_Dependency_Type::open() {
  if (!the_workspace_panel) the_workspace_panel = make_workspace_panel();
  the_workspace_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_workspace_panel->show();
}


// ------------ Target ---------------------------------------------------------

Fl_Target_Type Fl_Target_type;

Fl_Type *Fl_Target_Type::make() {
  // no generic targets!
  return 0L;
}

// Note: we do not deal with UTF8 chracters here!
const char *Fl_Target_Type::caps_name() {
  const char *s = name();
  char *buf = get_temporary_return_buffer(strlen(s)+1);
  char *d = buf;
  while (*s) {
    *d++ = toupper(*s++);
  }
  *d++ = 0;
  return buf;
}

// Note: we do not deal with UTF8 chracters here!
const char *Fl_Target_Type::lowercase_name() {
  const char *s = name();
  char *buf = get_temporary_return_buffer(strlen(s)+1);
  char *d = buf;
  while (*s) {
    *d++ = tolower(*s++);
  }
  *d++ = 0;
  return buf;
}

Fl_Target_Type *Fl_Target_Type::find(const char *name, char end) {
  // find a partial string, if 'end' is set to a character
  char buf[FLTK3_PATH_MAX];
  strcpy(buf, name);
  if (end) {
    char *sep = strchr(buf, end);
    if (sep) *sep = 0;
  }
  // now find the target by name (stored in 'buf')
  Fl_Type *tgt = first;
  while (tgt) {
    if (tgt->is_target() && strcmp(tgt->name(), buf)==0)
      return (Fl_Target_Type*)tgt;
    tgt = tgt->next;
  }
  return 0;
}

Fl_Target_Type *Fl_Target_Type::first_target(Fl_Type *base) {
  Fl_Type *src = base->next;
  while (src && src->level>base->level) {
    if (src->is_target()) 
      return (Fl_Target_Type*)src;
    src = src->next;
  }
  return 0;
}

Fl_Target_Type *Fl_Target_Type::next_target(Fl_Type *base) {
  Fl_Type *src = this->next;
  while (src && src->level>base->level) {
    if (src->is_target()) 
      return (Fl_Target_Type*)src;
    src = src->next;
  }
  return 0;
}

void Fl_Target_Type::target_path(const char *path) {
  if (pTargetPath) {
    free(pTargetPath);
    pTargetPath = 0L;
  }
  if (path) {
    pTargetPath = strdup(path);
  } else {
    pTargetPath = strdup("");
  }
}

void Fl_Target_Type::makefile_path(const char *path) {
  if (pMakefilePath) {
    free(pMakefilePath);
    pMakefilePath = 0L;
  }
  if (path) {
    pMakefilePath = strdup(path);
  } else {
    pMakefilePath = strdup("");
  }
}

char Fl_Target_Type::read_property(const char *c) {
  if (!strcmp(c,"target_path")) {
    target_path(read_word());
  } else if (!strcmp(c,"makefile_path")) {
    makefile_path(read_word());
  } else {
    return Fl_Tool_Type::read_property(c);
  }
  return 1;
}

void Fl_Target_Type::write_properties() {
  Fl_Tool_Type::write_properties();
  if (target_path() && *target_path()) {
    write_indent(level+1);
    write_string("target_path");
    write_word(target_path());
  }
  if (makefile_path() && *makefile_path()) {
    write_indent(level+1);
    write_string("makefile_path");
    write_word(makefile_path());
  }
}


// ------------ Application Target ---------------------------------------------

Fl_App_Target_Type Fl_App_Target_type;

Fl_Type *Fl_App_Target_Type::make() {
  // a target can only go into a workspace file
  if (!project_is_workspace()) {
    fltk3::message("A Target can only be added to a Workspace.");
    return 0;
  }
  // find out where we can add this target
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_category() && !p->is_workspace())
    p = p->parent;  
  Fl_App_Target_Type *o = new Fl_App_Target_Type();
  o->name("myProgram");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_App_Target_Type::open() {
  if (!the_workspace_panel) the_workspace_panel = make_workspace_panel();
  the_workspace_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_workspace_panel->show();
}

// ------------ Library Target -------------------------------------------------

Fl_Lib_Target_Type Fl_Lib_Target_type;

Fl_Type *Fl_Lib_Target_Type::make() {
  // a target con only go into a workspace file
  // TODO: we can offer to create a workspace if this is currently a GUI file
  if (!project_is_workspace()) {
    fltk3::message("A Target can only be added to a Workspace.");
    return 0;
  }
  // find out where we can add this target
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_category() && !p->is_workspace())
    p = p->parent;  
  Fl_Lib_Target_Type *o = new Fl_Lib_Target_Type();
  o->name("myLibrary");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_Lib_Target_Type::open() {
  if (!the_workspace_panel) the_workspace_panel = make_workspace_panel();
  the_workspace_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_workspace_panel->show();
}

// ------------ Generic File ---------------------------------------------------

Fl_File_Type Fl_File_type;

Fl_Type *Fl_File_Type::make() {
  // make sure that this is a workspace file
  if (Fl_Type::first && !project_is_workspace()) {
    fltk3::message("File references can only be used in Workspace files.");
    return 0;
  }
  // files can be children of folders and children of targets
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_target() && !p->is_folder()) 
    p = p->parent;
  if (!p) {
    fltk3::message("A File reference can only be added to a Target or Folder!");
    return 0;
  }
  Fl_File_Type *o = new Fl_File_Type();
  o->name("readme.txt");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_File_Type::set_default_type() {
  const char *fn = filename();
  if (fn) {
    const char *ext = fltk3::filename_ext(fn);
    filetype(FL_FILE_TEXT);
    if (ext) {
      if (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0) {
        filetype(FL_FILE_CPP_SOURCE);
      } else if (strcmp(ext, ".c")==0) {
        filetype(FL_FILE_C_SOURCE);
      } else if (strcmp(ext, ".H")==0) {
        filetype(FL_FILE_CPP_HEADER);
      } else if (strcmp(ext, ".h")==0) {
        filetype(FL_FILE_C_HEADER);
      } else if (strcmp(ext, ".mm")==0) {
        filetype(FL_FILE_OBJC_SOURCE);
      } else if (strcmp(ext, ".framework")==0) {
        filetype(FL_FILE_FRAMEWORK);
      } else if (strcmp(ext, ".fl")==0) {
        filetype(FL_FILE_FLUID_UI);
      } else if (strcmp(ext, ".lib")==0) {
        filetype(FL_FILE_LIBRARY);
      }
    }
  }
}

void Fl_File_Type::filename(const char *fn) {
  if (pFilename) {
    free(pFilename);
    pFilename = 0;
    name(0);
  }
  if (fn) {
    pFilename = strdup(fn);
    name(fltk3::filename_name(fn));
    set_default_type();
  }
}

char Fl_File_Type::read_property(const char *c) {
  if (!strcmp(c,"filename_and_path")) {
    filename(read_word());
  } else if (!strcmp(c,"filetype")) {
    filetype(atoi(read_word()));
  } else if (!strcmp(c,"location")) {
    location(atoi(read_word()));
  } else {
    return Fl_Tool_Type::read_property(c);
  }
  return 1;
}

void Fl_File_Type::write_properties() {
  Fl_Tool_Type::write_properties();
  if (filename() && *filename()) {
    write_indent(level+1);
    write_string("filename_and_path");
    write_word(filename());
  }
  if (filetype()&FL_FILE_EXPLICIT) {
    write_indent(level+1);
    write_string("filetype %d", filetype());
  }
  if (location()!=FL_LOCATION_WORKSPACE) {
    write_indent(level+1);
    write_string("location %d", location());
  }
}

Fl_File_Type *Fl_File_Type::first_file(Fl_Type *base) {
  Fl_Type *src = base->next;
  while (src && src->level>base->level) {
    if (src->is_file()) 
      return (Fl_File_Type*)src;
    src = src->next;
  }
  return 0;
}

Fl_File_Type *Fl_File_Type::next_file(Fl_Type *base) {
  Fl_Type *src = this->next;
  while (src && src->level>base->level) {
    if (src->is_file()) 
      return (Fl_File_Type*)src;
    src = src->next;
  }
  return 0;
}

Fl_Target_Dependency_Type *Fl_Target_Dependency_Type::first_dependency(Fl_Type *base) {
  Fl_Type *src = base->next;
  while (src && src->level>base->level) {
    if (src->is_target_dependency()) 
      return (Fl_Target_Dependency_Type*)src;
    src = src->next;
  }
  return 0;
}

Fl_Target_Dependency_Type *Fl_Target_Dependency_Type::next_dependency(Fl_Type *base) {
  Fl_Type *src = this->next;
  while (src && src->level>base->level) {
    if (src->is_target_dependency()) 
      return (Fl_Target_Dependency_Type*)src;
    src = src->next;
  }
  return 0;
}

char Fl_File_Type::file_is_cplusplus_code() {
  return (pFileType==FL_FILE_CPP_SOURCE);
}

char Fl_File_Type::file_is_cplusplus_header() {
  return (pFileType==FL_FILE_CPP_HEADER);
}

char Fl_File_Type::file_is_c_code() {
  return (pFileType==FL_FILE_C_SOURCE);
}

char Fl_File_Type::file_is_c_header() {
  return (pFileType==FL_FILE_C_HEADER);
}

char Fl_File_Type::file_is_objc_code() {
  return (pFileType==FL_FILE_OBJC_SOURCE);
}

char Fl_File_Type::file_is_objc_header() {
  return (pFileType==FL_FILE_OBJC_HEADER);
}

char Fl_File_Type::file_is_framework() {
  return (pFileType==FL_FILE_FRAMEWORK);
}

char Fl_File_Type::file_is_library() {
  return (pFileType==FL_FILE_LIBRARY);
}

char Fl_File_Type::file_is_fluid_ui() {
  return (pFileType==FL_FILE_FLUID_UI);
}

char Fl_File_Type::file_is_other() {
  return (pFileType==FL_FILE_TEXT);
}

char Fl_File_Type::file_is_code() {
  return (pFileType==FL_FILE_CPP_SOURCE)||(pFileType==FL_FILE_C_SOURCE)||(pFileType==FL_FILE_OBJC_SOURCE);
}

char Fl_File_Type::file_is_header() {
  return (pFileType==FL_FILE_CPP_HEADER)||(pFileType==FL_FILE_C_HEADER)||(pFileType==FL_FILE_OBJC_HEADER);
}

const char *Fl_File_Type::filename_name() {
  const char *fn = filename();
  if (fn) {
    return fltk3::filename_name(fn);
  }
  return 0;
}

const char *Fl_File_Type::filename_relative(const char *fnbase, const char *tgtbase) {
  char src_name[FLTK3_PATH_MAX];
  char *result = get_temporary_return_buffer(FLTK3_PATH_MAX);
  const char *fn = filename();
  if (fn) {
    strcpy(src_name, fnbase);
    strcat(src_name, fn);
    fltk3::filename_relative(result, FLTK3_PATH_MAX, src_name, tgtbase);
    return result;
  } else {
    return 0;
  }
}

void Fl_File_Type::open() {
  if (!the_workspace_panel) the_workspace_panel = make_workspace_panel();
  the_workspace_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_workspace_panel->show();
}

// ------------ Folder ---------------------------------------------------------

Fl_Folder_Type Fl_Folder_type;

Fl_Type *Fl_Folder_Type::make() {
  // make sure that this is a workspace file
  if (Fl_Type::first && !project_is_workspace()) {
    fltk3::message("Folders can only be used in Workspace files.");
    return 0;
  }
  // Folders can be inside folders of inside targets
  // Categories can be at the top level or inside categories
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_category() && !p->is_folder() && !p->is_target() && !p->is_workspace()) 
    p = p->parent;
  Fl_Folder_Type *o = new Fl_Folder_Type();
  o->name("Group");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_Folder_Type::open() {
  if (!the_workspace_panel) the_workspace_panel = make_workspace_panel();
  the_workspace_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_workspace_panel->show();
}


//
// End of "$Id$".
//
