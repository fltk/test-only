//
// "$Id$"
//
// C function type code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/run.h>
#include "FluidType.h"
#include "Fluid_Image.h"
#include <fltk/ask.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "coding_style.h"

////////////////////////////////////////////////////////////////
// quick check of any C code for legality, returns an error message

static char buffer[128]; // for error messages

const char *strip_default_args(const char *name) {
  if(!strchr(name, '='))
    return name;
  static char *buffer = NULL;
  static size_t bufsize = 0;
  if(!bufsize || bufsize < strlen(name) + 1) {
    if(buffer) delete[] buffer;
    size_t allocsize = strlen(name) * 2;	// the *2 is just arbitrary pad
    buffer = new char[allocsize]; 
    if(buffer) bufsize = allocsize;
  }
  if(buffer) {
    memset(buffer, 0, bufsize);
    char *pbuff = buffer;
    const char *pname = name;
    int skipping = 0;
    int inargs = 0;
    while(*pname) {
      if(!inargs) {
	if(*pname == '(')
	  inargs = 1;
      }
      else if(!skipping) {
	if(*pname == '=') {
	  skipping = 1;
	  ++pname;
	  continue;
	}
      } else if(*pname == ',' || *pname == ')') {
	skipping = 0;
      }
      else if (skipping) {
	++pname;
	continue;
      }
      *pbuff++ = *pname++;
    }
    *pbuff = '\0';
  }
  return buffer;
}

// check a quoted string ending in either " or ' or >:
const char *_q_check(const char * & c, int type) {
  for (;;) switch (*c++) {
  case '\0':
    sprintf(buffer,"missing %c",type);
    return buffer;
  case '\\':
    if (*c) c++;
    break;
  default:
    if (*(c-1) == type) return 0;
  }
}

// check normal code, match braces and parenthesis:
const char *_c_check(const char * & c, int type) {
  const char *d;
  for (;;) switch (*c++) {
  case 0:
    if (!type) return 0;
    sprintf(buffer, "missing %c", type);
    return buffer;
  case '/':
    // Skip comments as needed...
    if (*c == '/') {
      while (*c != '\n' && *c) c++;
    } else if (*c == '*') {
      c++;
      while ((*c != '*' || c[1] != '/') && *c) c++;
      if (*c == '*') c+=2;
      else {
        return "missing '*/'";
      }
    }
    break;
  case '#':
    // treat cpp directives as a comment:
    while (*c != '\n' && *c) c++;
    break;
  case '{':
    if (type==')') goto UNEXPECTED;
    d = _c_check(c,'}');
    if (d) return d;
    break;
  case '(':
    d = _c_check(c,')');
    if (d) return d;
    break;
  case '\"':
    d = _q_check(c,'\"');
    if (d) return d;
    break;
  case '\'':
    d = _q_check(c,'\'');
    if (d) return d;
    break;
  case '}':
  case ')':
  UNEXPECTED:
    if (type == *(c-1)) return 0;
    sprintf(buffer, "unexpected %c", *(c-1));
    return buffer;
  }
}

const char *c_check(const char *c, int type) {
  return _c_check(c,type);
}

////////////////////////////////////////////////////////////////

class FunctionType : public FluidType {
  const char* return_type;
  const char* attributes;
  bool public_, cdecl_, constructor, havewidgets;
public:
  FluidType *make();
  void write_code();
  void open();
  int ismain() {return name_ == 0;}
  virtual const char *type_name() const {return "Function";}
  virtual const char *title() {
    return name() ? name() : "main()";
  }
  int is_parent() const {return 1;}
  int is_code_block() const {return 1;}
  void write_properties();
  void read_property(const char *);
};

FluidType *FunctionType::make() {
  FluidType *p = FluidType::current;
  while (p && !p->is_decl_block()) p = p->parent;
  FunctionType *o = new FunctionType();
  o->name("make_window()");
  o->attributes = 0;
  o->return_type = 0;
  o->add(p);
  o->factory = this;
  o->public_ = true;
  o->cdecl_ = false;
  return o;
}

void FunctionType::write_properties() {
  FluidType::write_properties();
  if (!public_) write_string("private");
  if (cdecl_) write_string("C");
  if (return_type) {
    write_string("return_type");
    write_word(return_type);
  }
  if (attributes) {
    write_string("attributes");
    write_word(attributes);
  }
}

void FunctionType::read_property(const char *c) {
  if (!strcmp(c,"private")) {
    public_ = 0;
  } else if (!strcmp(c,"C")) {
    cdecl_ = true;
  } else if (!strcmp(c,"return_type")) {
    storestring(read_word(),return_type);
  } else if (!strcmp(c,"attributes")) {
    storestring(read_word(),attributes);
  } else {
    FluidType::read_property(c);
  }
}

#include "function_panel.h"
#include <fltk/ask.h>

static void ok_callback(fltk::Widget* w, void*) {
  w->window()->set_value();
  w->window()->hide();
}

static void cancel_callback(fltk::Widget* w, void*) {
  w->window()->hide();
}

void FunctionType::open() {
  if (!function_panel) {
    make_function_panel();
    f_panel_ok->callback(ok_callback);
    f_panel_cancel->callback(cancel_callback);
  }
  f_return_type_input->static_value(return_type);
  f_attributes_input->static_value(attributes);
  f_name_input->static_value(name());
  f_public_button->value(public_);
  f_c_button->value(cdecl_);
  const char* message = 0;
  for (;;) {
    if (message) fltk::alert(message);
    if (!function_panel->exec()) break;
    const char* c = f_name_input->value();
    while (isspace(*c)) c++;
    message = c_check(c); if (message) continue;
    const char *d = c;
    for (; *d != '('; d++) if (isspace(*d) || !*d) break;
    if (*c && *d != '(') {
      message = "Name must contain argument list in parenthesis."; continue;
    }
    c = f_return_type_input->value();
    message = c_check(c); if (message) continue;
    name(f_name_input->value());
    storestring(c, return_type);
    storestring(f_attributes_input->value(), attributes);
    public_ = f_public_button->value();
    cdecl_ = f_c_button->value();
    break;
  }
  delete function_panel;
  function_panel = NULL;
}

FunctionType Functiontype;

WidgetType* last_group;

void FunctionType::write_code() {
  const char* rtype = return_type;
  constructor=0;
  havewidgets = 0;
  FluidType *child;
  char attr[256];
  if (attributes) {
    strncpy(attr, attributes, 255);
    strcat(attr, " ");
  } else
    attr[0] = 0;
  for (child = first_child; child; child = child->next_brother)
    if (child->is_widget()) {
      havewidgets = 1;
      last_group = (WidgetType*)child;
    }
  write_c("\n");
  if (ismain()) {
    write_c("#include <fltk/run.h>\n\n");
    write_c("int main%s(int argc, char **argv)%s\n",
	    gno_space_parens ? "" : " ", get_opening_brace(1));
    // write_c("int main(int argc, char **argv) {\n");
    if (havewidgets)
      rtype = last_group->subclass();
    else 
      rtype = "void";
  } else {
    const char* star = "";
    // from matt: let the user type "static " at the start of type
    // in order to declare a static method;
    int is_static = 0;
    int is_virtual = 0;
    if (rtype) {
      if (!strcmp(rtype,"static")) {is_static = 1; rtype = 0;}
      else if (!strncmp(rtype, "static ",7)) {is_static = 1; rtype += 7;}
      if (!strcmp(rtype, "virtual")) {is_virtual = 1; rtype = 0;}
      else if (!strncmp(rtype, "virtual ",8)) {is_virtual = 1; rtype += 8;}
    }
    if (!rtype) {
      if (havewidgets) {
	rtype = last_group->subclass();
	star = "*";
      } else rtype = "void";
    }

    const char* k = member_of(0);
    if (k) {
      write_public(public_);
      if (name()[0] == '~')
	constructor = 1;
      else {
	size_t n = strlen(k);
	if (!strncmp(name(), k, n) && name()[n] == '(') constructor = 1;
      }
	  write_h(get_indent_string(1));
      if (is_static) write_h("static ");
      if (is_virtual) write_h("virtual ");
      if (!constructor) {
        write_h("%s%s%s ", attr, rtype, star);
	write_c("%s%s ", rtype, star);
      }

      // if this is a subclass, only write_h() the part before the ':'
      char s[1024], *sptr = s;
      char *nptr = (char *)name();

      while (*nptr) {
        if (*nptr == ':') {
	  if (nptr[1] != ':') break;
	  // Copy extra ":" for "class::member"...
          *sptr++ = *nptr++;
        }	  
        *sptr++ = *nptr++;
      }
      *sptr = '\0';

      if(constructor)	// already wrote this for constructors.
	write_h("%s", attr);
      write_h("%s;\n", s);
      write_c("%s::%s%s", k, strip_default_args(name()), get_opening_brace(1));
    } else {
      if (public_) {
	if (cdecl_)
	  write_h("extern \"C\" { %s%s%s %s; }\n", attr, rtype, star, name());
	else
	  write_h("%s%s%s %s;\n", attr, rtype, star, name());
      }
      else write_c("static ");
      write_c("%s%s %s%s", rtype, star, name(), get_opening_brace(1));
    }
  }
  indentation += 2;
  if(havewidgets) 
    write_c("%s%s* w;\n", indent(), last_group->subclass());

  for (FluidType* q = first_child; q; q = q->next_brother) q->write_code();

  if (ismain()) {
    if (havewidgets) write_c("%sw->show(argc, argv);\n", get_indent_string(1));
    write_c("%sreturn %s%sfltk::run()%s;\n", get_indent_string(1),
			gno_space_parens ? "" : " ",
			galways_return_parens ? "(" : "", galways_return_parens ? ")" : "");
  } else if (havewidgets && !constructor && !return_type)
    write_c("%sreturn %s%sw%s;\n", get_indent_string(1), 
			gno_space_parens ? "" : " ",
			galways_return_parens ? "(" : "", galways_return_parens ? ")" : "");
  write_c("}\n");
  indentation -= 2;
  if (indentation < 0) indentation = 0;
}

////////////////////////////////////////////////////////////////

class CodeType : public FluidType {
public:
  FluidType *make();
  void write_code();
  void write_static();
  void open();
  virtual const char *type_name() const {return "code";}
  int is_code_block() const {return 0;}
};

FluidType *CodeType::make() {
  FluidType *p = FluidType::current;
  while (p && !p->is_code_block()) p = p->parent;
  if (!p) {
    fltk::message("Please select a function");
    return 0;
  }
  CodeType *o = new CodeType();
  o->name("printf(\"Hello, World!\\n\");");
  o->add(p);
  o->factory = this;
  return o;
}

void CodeType::open() {
  if (!code_panel) {
    make_code_panel();
    code_panel_ok->callback(ok_callback);
    code_panel_cancel->callback(cancel_callback);
  }
  code_input->static_value(name());
  const char* message = 0;
  for (;;) { // repeat as long as there are errors
    if (message) fltk::alert(message);
    if (!code_panel->exec()) break;
    const char*c = code_input->value();
    message = c_check(c); if (message) continue;
    name(c);
    break;
  }
  code_panel->hide();
  delete code_panel;
  code_panel = NULL;
}

CodeType Codetype;

void CodeType::write_code() {
  const char* c = name();
  if (!c) return;
  //write_c("%s%s\n", indent(), c);
  write_code_block((char *)c);
  for (FluidType* q = first_child; q; q = q->next_brother) q->write_code();
}

void CodeType::write_static() {
  const char* c = name();
  if (!c) return;
  //write_c("%s%s\n", indent(), c);
  write_includes_from_code((char *)c);
}

////////////////////////////////////////////////////////////////

class CodeBlockType : public FluidType {
  const char* after;
public:
  FluidType *make();
  void write_code();
  void open();
  virtual const char *type_name() const {return "codeblock";}
  int is_code_block() const {return 1;}
  int is_parent() const {return 1;}
  void write_properties();
  void read_property(const char *);
};

FluidType *CodeBlockType::make() {
  FluidType *p = FluidType::current;
  while (p && !p->is_code_block()) p = p->parent;
  if (!p) {
    fltk::message("Please select a function");
    return 0;
  }
  CodeBlockType *o = new CodeBlockType();
  o->name("if (test())");
  o->after = 0;
  o->add(p);
  o->factory = this;
  return o;
}

void CodeBlockType::write_properties() {
  FluidType::write_properties();
  if (after) {
    write_string("after");
    write_word(after);
  }
}

void CodeBlockType::read_property(const char *c) {
  if (!strcmp(c,"after")) {
    storestring(read_word(),after);
  } else {
    FluidType::read_property(c);
  }
}

void CodeBlockType::open() {
  if (!codeblock_panel) {
    make_codeblock_panel();
    codeblock_panel_ok->callback(ok_callback);
    codeblock_panel_cancel->callback(cancel_callback);
  }
  code_before_input->static_value(name());
  code_after_input->static_value(after);
  const char* message = 0;
  for (;;) { // repeat as long as there are errors
    if (message) fltk::alert(message);
    if (!codeblock_panel->exec()) break;
    const char*c = code_before_input->value();
    message = c_check(c); if (message) continue;
    name(c);
    c = code_after_input->value();
    message = c_check(c); if (message) continue;
    storestring(c, after);
    break;
  }
  codeblock_panel->hide();
  delete codeblock_panel;
  codeblock_panel = NULL;
}

CodeBlockType CodeBlocktype;

void CodeBlockType::write_code() {
  const char* c = name();
  write_c("%s%s%s", indent(), c ? c : "", get_opening_brace(0));
  indentation += 2;
  for (FluidType* q = first_child; q; q = q->next_brother) q->write_code();
  indentation -= 2;
  if (after) write_c("%s} %s\n", indent(), after);
  else write_c("%s}\n", indent());
}

////////////////////////////////////////////////////////////////

class DeclType : public FluidType {
  bool public_;
public:
  FluidType *make();
  void write_code();
  void open();
  virtual const char *type_name() const {return "decl";}
  void write_properties();
  void read_property(const char *);
};

FluidType *DeclType::make() {
  FluidType *p = FluidType::current;
  while (p && !p->is_decl_block()) p = p->parent;
  DeclType *o = new DeclType();
  o->public_ = 0;
  o->name("int x;");
  o->add(p);
  o->factory = this;
  return o;
}

void DeclType::write_properties() {
  FluidType::write_properties();
  if (public_) write_string("public");
}

void DeclType::read_property(const char *c) {
  if (!strcmp(c,"public")) {
    public_ = 1;
  } else {
    FluidType::read_property(c);
  }
}

void DeclType::open() {
  if (!decl_panel) {
    make_decl_panel();
    decl_panel_ok->callback(ok_callback);
    decl_panel_cancel->callback(cancel_callback);
  }
  decl_input->static_value(name());
  decl_public_button->value(public_);
  const char* message = 0;
  for (;;) { // repeat as long as there are errors
    if (message) fltk::alert(message);
    if (!decl_panel->exec()) break;
    const char*c = decl_input->value();
    while (isspace(*c)) c++;
    message = c_check(c&&c[0]=='#' ? c+1 : c);
    if (message) continue;
    name(c);
    public_ = decl_public_button->value();
    break;
  }
  decl_panel->hide();
  delete decl_panel;
  decl_panel = NULL;
}

DeclType Decltype;

void DeclType::write_code() {
  const char* c = name();
  if (!c) return;
  // handle putting #include or extern or typedef into decl:
  if (!isalpha(*c) && *c != '~'
      || !strncmp(c,"extern",6) && isspace(c[6])
      || !strncmp(c,"class",5) && isspace(c[5])
      || !strncmp(c,"typedef",7) && isspace(c[7])
//    || !strncmp(c,"struct",6) && isspace(c[6])
      ) {
    if (public_)
      write_h("%s\n", c);
    else
      write_c("%s\n", c);
    return;
  }
  // lose all trailing semicolons so I can add one:
  const char* e = c+strlen(c);
  while (e>c && e[-1]==';') e--;
  if (member_of()) {
    write_public(public_);
    write_h("%s%.*s;\n", get_indent_string(1), e-c, c);
  } else {
    if (public_) {
      write_h("extern %.*s;\n", e-c, c);
      write_c("%.*s;\n", e-c, c);
    } else {
      write_c("static %.*s;\n", e-c, c);
    }
  }

  for (FluidType* q = first_child; q; q = q->next_brother) q->write_code();
}

////////////////////////////////////////////////////////////////

class DeclBlockType : public FluidType {
  const char* after;
public:
  FluidType *make();
  void write_code();
  void open();
  virtual const char *type_name() const {return "declblock";}
  void write_properties();
  void read_property(const char *);
  int is_parent() const {return 1;}
  int is_decl_block() const {return 1;}
};

FluidType *DeclBlockType::make() {
  FluidType *p = FluidType::current;
  while (p && !p->is_decl_block()) p = p->parent;
  DeclBlockType *o = new DeclBlockType();
  o->name("#if 1");
  o->after = strdup("#endif");
  o->add(p);
  o->factory = this;
  return o;
}

void DeclBlockType::write_properties() {
  FluidType::write_properties();
  write_string("after");
  write_word(after);
}

void DeclBlockType::read_property(const char *c) {
  if (!strcmp(c,"after")) {
    storestring(read_word(),after);
  } else {
    FluidType::read_property(c);
  }
}

void DeclBlockType::open() {
  if (!declblock_panel) {
    make_declblock_panel();
    declblock_panel_ok->callback(ok_callback);
    declblock_panel_cancel->callback(cancel_callback);
  }
  decl_before_input->static_value(name());
  decl_after_input->static_value(after);
  const char* message = 0;
  for (;;) { // repeat as long as there are errors
    if (message) fltk::alert(message);
    if (!declblock_panel->exec()) break;
    const char*c = decl_before_input->value();
    while (isspace(*c)) c++;
    message = c_check(c&&c[0]=='#' ? c+1 : c);
    if (message) continue;
    name(c);
    c = decl_after_input->value();
    while (isspace(*c)) c++;
    message = c_check(c&&c[0]=='#' ? c+1 : c);
    if (message) continue;
    storestring(c,after);
    break;
  }
  declblock_panel->hide();
  delete declblock_panel;
  declblock_panel = NULL;
}

DeclBlockType DeclBlocktype;

void DeclBlockType::write_code() {
  const char* c = name();
  if (c) write_c("%s\n", c);
  for (FluidType* q = first_child; q; q = q->next_brother) q->write_code();
  if (after) write_c("%s\n", after);
}

////////////////////////////////////////////////////////////////

class ClassType : public FluidType {
  const char* subclass_of;
  bool public_;
public:
  // state variables for output:
  char write_public_state; // true when public: has been printed
  ClassType* parent_class; // save class if nested
//
  FluidType *make();
  void write_code();
  void open();
  virtual const char *type_name() const {return "class";}
  int is_parent() const {return 1;}
  int is_decl_block() const {return 1;}
  int is_class() const {return 1;}
  void write_properties();
  void read_property(const char *);
   
  // fc: added for FL_API prefixing and others prefix capability
  // class prefix attribute access
  void prefix(const char* p);
  const char*  prefix() const {return class_prefix;}
private:
  const char* class_prefix;
};

// Return the class that this is a member of, or null if this is not
// a member of a class. If need_nest is true then a fully-qualified
// name (ie foo::bar::baz) of nested classes is returned, you need this
// if you actually want to print the class.
const char* FluidType::member_of(bool need_nest) const {
  FluidType* p = parent;
  while (p) {
    if (p->is_class()) {
      if (!need_nest) return p->name();
      // see if we are nested in another class, we must fully-qualify name:
      // this is lame but works...
      const char* q = p->member_of(true);
      if (!q) return p->name();
      static char buffer[256];
      if (q != buffer) strcpy(buffer, q);
      strcat(buffer, "::");
      strcat(buffer, p->name());
      return buffer;
    }
    p = p->parent;
  }
  return 0;
}

void ClassType::prefix(const char*p) {
  free((void*) class_prefix);
  class_prefix=strdup(p ? p : "" );
}

FluidType *ClassType::make() {
  FluidType *p = FluidType::current;
  while (p && !p->is_decl_block()) p = p->parent;
  ClassType *o = new ClassType();
  o->name("UserInterface");
  o->class_prefix = 0;
  o->subclass_of = 0;
  o->public_ = true;
  o->add(p);
  o->factory = this;
  return o;
}

void ClassType::write_properties() {
  FluidType::write_properties();
  if (subclass_of) {
    write_string(":");
    write_word(subclass_of);
  }
  if (!public_) write_string("private");
}

void ClassType::read_property(const char *c) {
  if (!strcmp(c,"private")) {
    public_ = false;
  } else if (!strcmp(c,":")) {
    storestring(read_word(), subclass_of);
  } else {
    FluidType::read_property(c);
  }
}

void ClassType::open() {
  if (!class_panel) {
    make_class_panel();
    c_panel_ok->callback(ok_callback);
    c_panel_cancel->callback(cancel_callback);
  }
  char fullname[1024]="";
  if (prefix() && strlen(prefix())) 
    sprintf(fullname,"%s %s",prefix(),name());
  else 
    strcpy(fullname, name());
  c_name_input->static_value(fullname);

  c_subclass_input->static_value(subclass_of);
  c_public_button->value(public_);
  const char* message = 0;
 char *na=0,*pr=0,*p=0; // name and prefix substrings

  for (;;) { // repeat as long as there are errors
    if (message) fltk::alert(message);
    if (!class_panel->exec()) break;
    const char*c = c_name_input->value();
    char *s = strdup(c);
    size_t len = strlen(s);
    if (!*s) goto OOPS;
    p = (char*) (s+len-1);
    while (p>=s && isspace(*p)) *(p--)='\0';
    if (p<s) goto OOPS;
    while (p>=s && is_id(*p)) p--;
    if ( (p<s && !is_id(*(p+1))) || !*(p+1) ) {
      OOPS: message = "class name must be C++ identifier";
      free((void*)s);
      continue;
    }
    na=p+1; // now we have the name
    if(p>s) *p--='\0';
    while (p>=s && isspace(*p)) *(p--)='\0';
    while (p>=s && is_id(*p))   p--;
    if (p<s)                    p++;
    if (is_id(*p) && p<na)      pr=p; // prefix detected
    c = c_subclass_input->value();
    message = c_check(c); 
    if (message) { free((void*)s);continue;}
    name(na);
    prefix(pr);
    free((void*)s);
    storestring(c, subclass_of);
    if (public_ != c_public_button->value()) {
      public_ = c_public_button->value();
    }
    break;
  }
  class_panel->hide();
  delete class_panel;
  class_panel = NULL;
}

ClassType Classtype;

static ClassType *current_class;
extern int varused_test;
void write_public(int state) {
  if (!current_class || varused_test) return;
  if (current_class->write_public_state == state) return;
  current_class->write_public_state = state;
  write_h(state ? "public:\n" : "private:\n");
}

void ClassType::write_code() {
  parent_class = current_class;
  current_class = this;
  write_public_state = 0;
  if (prefix() && strlen(prefix()))
      write_h("\nclass %s %s ", prefix(), name());
  else
      write_h("\nclass %s ", name());
  if (subclass_of) write_h(": %s ", subclass_of);
  write_h("%s", get_opening_brace(1));
  for (FluidType* q = first_child; q; q = q->next_brother) q->write_code();
  write_h("};\n");
  current_class = parent_class;
}

//
// End of "$Id$".
//
