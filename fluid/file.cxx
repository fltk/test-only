//
// "$Id$"
//
// Fluid file routines for the Fast Light Tool Kit (FLTK).
//
// You may find the basic read_* and write_* routines to
// be useful for other programs.  I have used them many times.
// They are somewhat similar to tcl, using matching { and }
// to quote strings.
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

#include <stdio.h>
#include <stdlib.h>
#include "../src/flstring.h"
#include <stdarg.h>
#include "alignment_panel.h"
#include <fltk3/FileChooser.h>

////////////////////////////////////////////////////////////////
// BASIC FILE WRITING:

static FILE *fout;

double read_version;
double write_version;

int open_write(const char *s) {
  if (!s) {fout = stdout; return 1;}
  FILE *f = fltk3::fopen(s,"w");
  if (!f) return 0;
  fout = f;
  return 1;
}

int close_write() {
  if (fout != stdout) {
    int x = fclose(fout);
    fout = stdout;
    return x >= 0;
  }
  return 1;
}

static int needspace;
int is_id(char); // in code.C

// write a string, quoting characters if necessary:
void write_word(const char *w) {
  if (needspace) putc(' ', fout);
  needspace = 1;
  if (!w || !*w) {fprintf(fout,"{}"); return;}
  const char *p;
  // see if it is a single word:
  for (p = w; is_id(*p); p++) ;
  if (!*p) {fprintf(fout,"%s",w); return;}
  // see if there are matching braces:
  int n = 0;
  for (p = w; *p; p++) {
    if (*p == '{') n++;
    else if (*p == '}') {n--; if (n<0) break;}
  }
  int mismatched = (n != 0);
  // write out brace-quoted string:
  putc('{', fout);
  for (; *w; w++) {
    switch (*w) {
    case '{':
    case '}':
      if (!mismatched) break;
    case '\\':
    case '#':
      putc('\\',fout);
      break;
    }
    putc(*w,fout);
  }
  putc('}', fout);
}

// write an arbitrary formatted word, or a comment, etc:
void write_string(const char *format, ...) {
  va_list args;
  va_start(args, format);
  if (needspace) fputc(' ',fout);
  vfprintf(fout, format, args);
  va_end(args);
  needspace = !isspace(format[strlen(format)-1] & 255);
}

// start a new line and indent it for a given nesting level:
void write_indent(int n) {
  fputc('\n',fout);
  while (n--) {fputc(' ',fout); fputc(' ',fout);}
  needspace = 0;
}

// write a '{' at the given indenting level:
void write_open(int) {
  if (needspace) fputc(' ',fout);
  fputc('{',fout);
  needspace = 0;
}

// write a '}' at the given indenting level:
void write_close(int n) {
  if (needspace) write_indent(n);
  fputc('}',fout);
  needspace = 1;
}

////////////////////////////////////////////////////////////////
// BASIC FILE READING:

static FILE *fin;
static int lineno;
static const char *fname;

int open_read(const char *s) {
  lineno = 1;
  if (!s) {fin = stdin; fname = "stdin"; return 1;}
  FILE *f = fltk3::fopen(s,"r");
  if (!f) return 0;
  fin = f;
  fname = s;
  return 1;
}

int close_read() {
  if (fin != stdin) {
    int x = fclose(fin);
    fin = 0;
    return x >= 0;
  }
  return 1;
}

#include <fltk3/message.h>

void read_error(const char *format, ...) {
  va_list args;
  va_start(args, format);
  if (!fin) {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    fltk3::message("%s", buffer);
  } else {
    fprintf(stderr, "%s:%d: ", fname, lineno);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
  }
  va_end(args);
}

static int hexdigit(int x) {
  if (isdigit(x)) return x-'0';
  if (isupper(x)) return x-'A'+10;
  if (islower(x)) return x-'a'+10;
  return 20;
}


static int read_quoted() {	// read whatever character is after a \ .
  int c,d,x;
  switch(c = fgetc(fin)) {
  case '\n': lineno++; return -1;
  case 'a' : return('\a');
  case 'b' : return('\b');
  case 'f' : return('\f');
  case 'n' : return('\n');
  case 'r' : return('\r');
  case 't' : return('\t');
  case 'v' : return('\v');
  case 'x' :	/* read hex */
    for (c=x=0; x<3; x++) {
      int ch = fgetc(fin);
      d = hexdigit(ch);
      if (d > 15) {ungetc(ch,fin); break;}
      c = (c<<4)+d;
    }
    break;
  default:		/* read octal */
    if (c<'0' || c>'7') break;
    c -= '0';
    for (x=0; x<2; x++) {
      int ch = fgetc(fin);
      d = hexdigit(ch);
      if (d>7) {ungetc(ch,fin); break;}
      c = (c<<3)+d;
    }
    break;
  }
  return(c);
}

// return a word read from the file, or NULL at the EOF:
// This will skip all comments (# to end of line), and evaluate
// all \xxx sequences and use \ at the end of line to remove the newline.
// A word is any one of:
//	a continuous string of non-space chars except { and } and #
//	everything between matching {...} (unless wantbrace != 0)
//	the characters '{' and '}'

static char *buffer;
static int buflen;
static void expand_buffer(int length) {
  if (length >= buflen) {
    if (!buflen) {
      buflen = length+1;
      buffer = (char*)malloc(buflen);
    } else {
      buflen = 2*buflen;
      if (length >= buflen) buflen = length+1;
      buffer = (char *)realloc((void *)buffer,buflen);
    }
  }
}

const char *read_word(int wantbrace) {
  int x;

  // skip all the whitespace before it:
  for (;;) {
    x = getc(fin);
    if (x < 0 && feof(fin)) {	// eof
      return 0;
    } else if (x == '#') {	// comment
      do x = getc(fin); while (x >= 0 && x != '\n');
      lineno++;
      continue;
    } else if (x == '\n') {
      lineno++;
    } else if (!isspace(x & 255)) {
      break;
    }
  }

  expand_buffer(100);

  if (x == '{' && !wantbrace) {

    // read in whatever is between braces
    int length = 0;
    int nesting = 0;
    for (;;) {
      x = getc(fin);
      if (x<0) {read_error("Missing '}'"); break;}
      else if (x == '#') { // embedded comment
	do x = getc(fin); while (x >= 0 && x != '\n');
	lineno++;
	continue;
      } else if (x == '\n') lineno++;
      else if (x == '\\') {x = read_quoted(); if (x<0) continue;}
      else if (x == '{') nesting++;
      else if (x == '}') {if (!nesting--) break;}
      buffer[length++] = x;
      expand_buffer(length);
    }
    buffer[length] = 0;
    return buffer;

  } else if (x == '{' || x == '}') {
    // all the punctuation is a word:
    buffer[0] = x;
    buffer[1] = 0;
    return buffer;

  } else {

    // read in an unquoted word:
    int length = 0;
    for (;;) {
      if (x == '\\') {x = read_quoted(); if (x<0) continue;}
      else if (x<0 || isspace(x & 255) || x=='{' || x=='}' || x=='#') break;
      buffer[length++] = x;
      expand_buffer(length);
      x = getc(fin);
    }
    ungetc(x, fin);
    buffer[length] = 0;
    return buffer;

  }
}

////////////////////////////////////////////////////////////////

#include <fltk3/run.h>
#include "Fl_Widget_Type.h"

// global int variables:
extern int i18n_type;
extern const char* i18n_include;
extern const char* i18n_function;
extern const char* i18n_file;
extern const char* i18n_set;


extern int header_file_set;
extern int code_file_set;
extern const char* header_file_name;
extern const char* code_file_name;

int write_file(const char *filename, int selected_only) {
  if (!open_write(filename)) return 0;
  write_string("# data file for the Fltk User Interface Designer (fluid)\n"
	       "version %.4f",FL_VERSION);
  if(!include_H_from_C)
    write_string("\ndo_not_include_H_from_C");
  if(use_FL_COMMAND)
    write_string("\nuse_FL_COMMAND");
  if (i18n_type) {
    write_string("\ni18n_type %d", i18n_type);
    write_string("\ni18n_include %s", i18n_include);
    switch (i18n_type) {
    case 1 : /* GNU gettext */
	write_string("\ni18n_function %s", i18n_function);
        break;
    case 2 : /* POSIX catgets */
        if (i18n_file[0]) write_string("\ni18n_file %s", i18n_file);
	write_string("\ni18n_set %s", i18n_set);
        break;
    }
  }
  if (!selected_only) {
    write_string("\nheader_name"); write_word(header_file_name);
    write_string("\ncode_name"); write_word(code_file_name);
  }
  for (Fl_Type *p = Fl_Type::first; p;) {
    if (!selected_only || p->selected) {
      p->write();
      write_string("\n");
      int q = p->level;
      for (p = p->next; p && p->level > q; p = p->next);
    } else {
      p = p->next;
    }
  }
  return close_write();
}

////////////////////////////////////////////////////////////////
// read all the objects out of the input file:

void read_fdesign();

extern Fl_Type *Fl_Type_make(const char *tn);

static void read_children(Fl_Type *p, int paste) {
  Fl_Type::current = p;
  for (;;) {
    const char *c = read_word();
  REUSE_C:
    if (!c) {
      if (p && !paste) read_error("Missing '}'");
      break;
    }

    if (!strcmp(c,"}")) {
      if (!p) read_error("Unexpected '}'");
      break;
    }

    // this is the first word in a .fd file:
    if (!strcmp(c,"Magic:")) {
      read_fdesign();
      return;
    }

    if (!strcmp(c,"version")) {
      c = read_word();
      read_version = write_version = strtod(c,0);
      if (read_version<=0 || read_version>FL_VERSION)
        read_error("unknown version '%s'",c);
      continue;
    }

    // back compatibility with Vincent Penne's original class code:
    if (!p && !strcmp(c,"define_in_struct")) {
      Fl_Type *t = Fl_Type_make("class");
      t->name(read_word());
      Fl_Type::current = p = t;
      paste = 1; // stops "missing }" error
      continue;
    }

    if (!strcmp(c,"do_not_include_H_from_C")) {
      include_H_from_C=0;
      goto CONTINUE;
    }
    if (!strcmp(c,"use_FL_COMMAND")) {
      use_FL_COMMAND=1;
      goto CONTINUE;
    }
    if (!strcmp(c,"i18n_type")) {
      i18n_type = atoi(read_word());
      goto CONTINUE;
    }
    if (!strcmp(c,"i18n_function")) {
      i18n_function = strdup(read_word());
      goto CONTINUE;
    }
    if (!strcmp(c,"i18n_file")) {
      i18n_file = strdup(read_word());
      goto CONTINUE;
    }
    if (!strcmp(c,"i18n_set")) {
      i18n_set = strdup(read_word());
      goto CONTINUE;
    }
    if (!strcmp(c,"i18n_include")) {
      i18n_include = strdup(read_word());
      goto CONTINUE;
    }
    if (!strcmp(c,"i18n_type"))
    {
      i18n_type = atoi(read_word());
      goto CONTINUE;
    }
    if (!strcmp(c,"i18n_type"))
    {
      i18n_type = atoi(read_word());
      goto CONTINUE;
    }
    if (!strcmp(c,"header_name")) {
      if (!header_file_set) header_file_name = strdup(read_word());
      else read_word();
      goto CONTINUE;
    }

    if (!strcmp(c,"code_name")) {
      if (!code_file_set) code_file_name = strdup(read_word());
      else read_word();
      goto CONTINUE;
    }

    if (!strcmp(c, "snap") || !strcmp(c, "gridx") || !strcmp(c, "gridy")) {
      // grid settings are now global
      read_word();
      goto CONTINUE;
    }

    {Fl_Type *t = Fl_Type_make(c);
    if (!t) {
      read_error("Unknown word \"%s\"", c);
      continue;
    }
    t->name(read_word());

    c = read_word(1);
    if (strcmp(c,"{") && t->is_class()) {   // <prefix> <name>
      ((Fl_Class_Type*)t)->prefix(t->name());
      t->name(c);
      c = read_word(1);
    }

    if (strcmp(c,"{")) {
      read_error("Missing property list for %s\n",t->title());
      goto REUSE_C;
    }

    t->open_ = 0;
    for (;;) {
      const char *cc = read_word();
      if (!cc || !strcmp(cc,"}")) break;
      if (t->read_property(cc)==0) {
        read_error("Unknown property \"%s\" for \"%s\".\n",cc, t->title());
        goto CONTINUE;
      }
    }

    if (!t->is_parent()) continue;
    c = read_word(1);
    if (strcmp(c,"{")) {
      read_error("Missing child list for %s\n",t->title());
      goto REUSE_C;
    }
    read_children(t, 0);}
    Fl_Type::current = p;
  CONTINUE:;
  }
}

extern void deselect();

int read_file(const char *filename, int merge) {
  Fl_Type *o;
  read_version = 0.0;
  if (!open_read(filename)) return 0;
  if (merge) deselect(); else delete_all();
  read_children(Fl_Type::current, merge);
  Fl_Type::current = 0;
  // Force menu items to be rebuilt...
  for (o = Fl_Type::first; o; o = o->next)
    if (o->is_menu_button()) o->add_child(0,0);
  for (o = Fl_Type::first; o; o = o->next)
    if (o->selected) {Fl_Type::current = o; break;}
  selection_changed(Fl_Type::current);
  return close_read();
}

////////////////////////////////////////////////////////////////
// Read Forms and XForms fdesign files:

int read_fdesign_line(const char*& name, const char*& value) {

  int length = 0;
  int x;
  // find a colon:
  for (;;) {
    x = getc(fin);
    if (x < 0 && feof(fin)) return 0;
    if (x == '\n') {length = 0; continue;} // no colon this line...
    if (!isspace(x & 255)) {
      buffer[length++] = x;
      expand_buffer(length);
    }
    if (x == ':') break;
  }
  int valueoffset = length;
  buffer[length-1] = 0;

  // skip to start of value:
  for (;;) {
    x = getc(fin);
    if ((x < 0 && feof(fin)) || x == '\n' || !isspace(x & 255)) break;
  }

  // read the value:
  for (;;) {
    if (x == '\\') {x = read_quoted(); if (x<0) continue;}
    else if (x == '\n') break;
    buffer[length++] = x;
    expand_buffer(length);
    x = getc(fin);
  }
  buffer[length] = 0;
  name = buffer;
  value = buffer+valueoffset;
  return 1;
}

int fdesign_flip;
int fdesign_magic;
#include <fltk3/Group.h>

static const char *class_matcher[] = {
"fltk3::CHECKBUTTON", "fltk3::CheckButton",
"fltk3::ROUNDBUTTON", "fltk3::RoundButton",
"fltk3::ROUND3DBUTTON", "fltk3::RoundButton",
"fltk3::LIGHTBUTTON", "fltk3::LightButton",
"fltk3::FRAME", "fltk3::Box",
"fltk3::LABELFRAME", "fltk3::Box",
"fltk3::TEXT", "fltk3::Box",
"fltk3::VALSLIDER", "fltk3::ValueSlider",
"fltk3::MENU", "fltk3::MenuButton",
"3", "fltk3::BITMAP",
"1", "fltk3::BOX",
"71","fltk3::BROWSER",
"11","fltk3::BUTTON",
"4", "fltk3::CHART",
"42","fltk3::CHOICE",
"61","fltk3::CLOCK",
"25","fltk3::COUNTER",
"22","fltk3::DIAL",
"101","fltk3::FREE",
"31","fltk3::INPUT",
"12","fltk3::LightButton",
"41","fltk3::MENU",
"23","fltk3::POSITIONER",
"13","fltk3::RoundButton",
"21","fltk3::SLIDER",
"2", "fltk3::BOX", // was fltk3::TEXT
"62","fltk3::TIMER",
"24","fltk3::ValueSlider",
0};

void read_fdesign() {
  fdesign_magic = atoi(read_word());
  fdesign_flip = (fdesign_magic < 13000);
  Fl_Widget_Type *window = 0;
  Fl_Widget_Type *group = 0;
  Fl_Widget_Type *widget = 0;
  if (!Fl_Type::current) {
    Fl_Type *t = Fl_Type_make("Function");
    t->name("create_the_forms()");
    Fl_Type::current = t;
  }
  for (;;) {
    const char *name;
    const char *value;
    if (!read_fdesign_line(name, value)) break;

    if (!strcmp(name,"Name")) {

      window = (Fl_Widget_Type*)Fl_Type_make("fltk3::Window");
      window->name(value);
      window->label(value);
      Fl_Type::current = widget = window;

    } else if (!strcmp(name,"class")) {

      if (!strcmp(value,"fltk3::BEGIN_GROUP")) {
	group = widget = (Fl_Widget_Type*)Fl_Type_make("fltk3::Group");
	Fl_Type::current = group;
      } else if (!strcmp(value,"fltk3::END_GROUP")) {
	if (group) {
	  fltk3::Group* g = (fltk3::Group*)(group->o);
	  //g->begin(); FIXME: forms library no longer supported
	  //g->forms_end();
	  fltk3::Group::current(0);
	}
	group = widget = 0;
	Fl_Type::current = window;
      } else {
	for (int i = 0; class_matcher[i]; i += 2)
	  if (!strcmp(value,class_matcher[i])) {
	    value = class_matcher[i+1]; break;}
	widget = (Fl_Widget_Type*)Fl_Type_make(value);
	if (!widget) {
	  printf("class %s not found, using fltk3::Button\n", value);
	  widget = (Fl_Widget_Type*)Fl_Type_make("fltk3::Button");
	}
      }

    } else if (widget) {
      if (!widget->read_fdesign(name, value))
	printf("Ignoring \"%s: %s\"\n", name, value);
    }
  }
}


// ------------ file conversion ------------------------------------------------

extern char *filename;

void write_fltk_makefiles() {
  /* This function will eventually create a complet FLTK makefile system. */
  /* We are currently limited to creating the Fluid Makefile. */
  if (!filename) {
    printf("Workspace must be saved first\n");
    return;
  }
  
  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Type::first;
  while (tgt) {
    if (tgt->is_target() && strcmp(tgt->name(), "Fluid")==0)
      break;
    tgt = tgt->next;
  }
  if (!tgt) {
    printf("FLUID target not found\n");
    return;
  }
  
  /* Create a new Makefile */
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "fluid/Makefile");
  FILE *out = fopen(buf, "wb");
  if (!out) {
    printf("Can't open FLUID Makefile\n");
    return;
  }
  
  fprintf(out, "#\n");
  fprintf(out, "# \"$Id$\"\n");
  fprintf(out, "#\n");
  fprintf(out, "# FLUID makefile for the Fast Light Tool Kit (FLTK).\n");
  fprintf(out, "#\n");
  fprintf(out, "# Copyright 1998-2011 by Bill Spitzak and others.\n");
  fprintf(out, "#\n");
  fprintf(out, "# This library is free software; you can redistribute it and/or\n");
  fprintf(out, "# modify it under the terms of the GNU Library General Public\n");
  fprintf(out, "# License as published by the Free Software Foundation; either\n");
  fprintf(out, "# version 2 of the License, or (at your option) any later version.\n");
  fprintf(out, "#\n");
  fprintf(out, "# This library is distributed in the hope that it will be useful,\n");
  fprintf(out, "# but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(out, "# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
  fprintf(out, "# Library General Public License for more details.\n");
  fprintf(out, "#\n");
  fprintf(out, "# You should have received a copy of the GNU Library General Public\n");
  fprintf(out, "# License along with this library; if not, write to the Free Software\n");
  fprintf(out, "# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307\n");
  fprintf(out, "# USA.\n");
  fprintf(out, "#\n");
  fprintf(out, "# Please report all bugs and problems on the following page:\n");
  fprintf(out, "#\n");
  fprintf(out, "#      http://www.fltk.org/str.php\n");
  fprintf(out, "#\n");
  fprintf(out, "\n");
  
  /* loop through the target and write out all C++ files */
  fprintf(out, "CPPFILES = ");
  Fl_Type *src = tgt->next;
  while (src && src->level>tgt->level) {
    if (src->is_file()) {
      Fl_File_Type *f = (Fl_File_Type*)src;
      const char *fn = f->filename();
      if (fn) {
        const char *ext = fltk3::filename_ext(fn);
        if (ext && (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0)) {
          fprintf(out, "\\\n\t%s", fltk3::filename_name(fn));
        }
      }
    }
    src = src->next;
  }
  fprintf(out, "\n\n");
  
  fprintf(out, "################################################################\n");
  fprintf(out, "\n");
  fprintf(out, "OBJECTS = $(CPPFILES:.cxx=.o)\n");
  fprintf(out, "\n");
  fprintf(out, "include ../makeinclude\n");
  fprintf(out, "\n");
  fprintf(out, "all:\t$(FLUID) fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "fluid$(EXEEXT):\t\t$(OBJECTS) $(LIBNAME) $(FLLIBNAME) \\\n");
  fprintf(out, "\t\t\t$(IMGLIBNAME)\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LINKFLTKFORMS) $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\t$(OSX_ONLY) $(INSTALL_BIN) fluid fluid.app/Contents/MacOS\n");
  fprintf(out, "\n");
  fprintf(out, "fluid-shared$(EXEEXT):\t$(OBJECTS) ../src/$(DSONAME) ../src/$(FLDSONAME) \\\n");
  fprintf(out, "\t\t\t../src/$(IMGDSONAME)\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LINKSHARED) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "clean:\n");
  fprintf(out, "\t-$(RM) *.o core.* *~ *.bck *.bck\n");
  fprintf(out, "\t-$(RM) core fluid$(EXEEXT) fluid-shared$(EXEEXT)\n");
  fprintf(out, "\t-$(RM) fluid.app/Contents/MacOS/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "depend:\t$(CPPFILES)\n");
  fprintf(out, "\tmakedepend -Y -I.. -f makedepend $(CPPFILES)\n");
  fprintf(out, "\n");
  fprintf(out, "# Automatically generated dependencies...\n");
  fprintf(out, "include makedepend\n");
  fprintf(out, "\n");
  fprintf(out, "install: all\n");
  fprintf(out, "\techo \"Installing FLUID in $(DESTDIR)$(bindir)...\"\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)$(bindir)\n");
  fprintf(out, "\t$(INSTALL_BIN) $(FLUID) $(DESTDIR)$(bindir)/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "install-linux:\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/usr/share/applications\n");
  fprintf(out, "\t$(INSTALL_DATA) fluid.desktop  $(DESTDIR)/usr/share/applications\n");
  fprintf(out, "\tfor size in 16 32 48 64 128; do \\\n");
  fprintf(out, "\t\tif test ! -d $(DESTDIR)/usr/share/icons/hicolor/$${size}x$${size}/apps; then \\\n");
  fprintf(out, "\t\t\t$(INSTALL_DIR) $(DESTDIR)/usr/share/icons/hicolor/$${size}x$${size}/apps; \\\n");
  fprintf(out, "\t\tfi; \\\n");
  fprintf(out, "\t\t$(INSTALL_DATA) icons/fluid-$$size.png $(DESTDIR)/usr/share/icons/hicolor/$${size}x$${size}/apps/fluid.png; \\\n");
  fprintf(out, "\tdone\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/usr/share/mimelnk/application\n");
  fprintf(out, "\t$(INSTALL_DATA) x-fluid.desktop  $(DESTDIR)/usr/share/mimelnk/application\n");
  fprintf(out, "\n");
  fprintf(out, "install-osx:\n");
  fprintf(out, "\techo Installing Fluid in $(DESTDIR)/Applications...\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/Applications/fluid.app\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/Applications/fluid.app/Contents\n");
  fprintf(out, "\t$(INSTALL_DATA) fluid.app/Contents/Info.plist $(DESTDIR)/Applications/fluid.app/Contents/Info.plist\n");
  fprintf(out, "\t$(INSTALL_DATA) fluid.app/Contents/PkgInfo $(DESTDIR)/Applications/fluid.app/Contents/PkgInfo\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/Applications/fluid.app/Contents/MacOS\n");
  fprintf(out, "\t$(RM) $(DESTDIR)/Applications/fluid.app/Contents/MacOS/fluid\n");
  fprintf(out, "\t$(LN) $(bindir)/fluid $(DESTDIR)/Applications/fluid.app/Contents/MacOS/fluid\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/Applications/fluid.app/Contents/Resources\n");
  fprintf(out, "\t$(INSTALL_DATA) fluid.app/Contents/Resources/fluid.icns $(DESTDIR)/Applications/fluid.app/Contents/Resources\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall:\n");
  fprintf(out, "\t$(RM) $(DESTDIR)$(bindir)/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall-linux:\n");
  fprintf(out, "\t$(RM) $(DESTDIR)/usr/share/applications/fluid.desktop\n");
  fprintf(out, "\t$(RM) $(DESTDIR)/usr/share/icons/hicolor/*/fluid.png\n");
  fprintf(out, "\t$(RM) $(DESTDIR)/usr/share/mimelnk/application/x-fluid.desktop\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall-osx:\n");
  fprintf(out, "\t$(RM) -r $(DESTDIR)/Applications/fluid.app\n");
  fprintf(out, "\n");
  fprintf(out, "\n");
  fprintf(out, "#\n");
  fprintf(out, "# Note: The rebuild target can only be used if you have the original .fl\n");
  fprintf(out, "#       files.  This is normally only used by the FLTK maintainers...\n");
  fprintf(out, "#\n");
  fprintf(out, "\n");
  fprintf(out, "rebuild:\n");
  fprintf(out, "\t./fluid -c about_panel.fl\n");
  fprintf(out, "\t./fluid -c alignment_panel.fl\n");
  fprintf(out, "\t./fluid -c function_panel.fl\n");
  fprintf(out, "\t./fluid -c print_panel.fl\n");
  fprintf(out, "\t./fluid -c template_panel.fl\n");
  fprintf(out, "\t./fluid -c widget_panel.fl\n");
  fprintf(out, "\n");
  fprintf(out, "#\n");
  fprintf(out, "# End of \"$Id$\".\n");
  fprintf(out, "#\n");

  fclose(out);
}

void write_fltk_ide_xcode4() {
  // for now, we use a template project file. Eventually, the entire file will be generated
  //    template is ide/Xcode4/FLTK.xcodeproj/project.pbxproj.tmpl
  // destination is ide/Xcode4/FLTK.xcodeproj/project.pbxproj

  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Type::first;
  while (tgt) {
    if (tgt->is_target() && strcmp(tgt->name(), "Fluid")==0)
      break;
    tgt = tgt->next;
  }
  if (!tgt) {
    printf("FLUID target not found\n");
    return;
  }
  
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "ide/Xcode4/FLTK.xcodeproj/project.pbxproj");
  FILE *out = fopen(buf, "wb");
  strcat(buf, ".tmpl");
  FILE *in = fopen(buf, "rb");
  
  for (;;) {
    if (fgets(buf, 2047, in)==0) // FIXME: handle error!
      break;
    char *hash = buf-1;
    char copyLine = 1;
    for (;;) {
      hash = strchr(hash+1, '#');
      if (!hash) break;
      if (hash && hash[1]=='#') { // double hash escapes the control character
        int n = strlen(hash);
        memmove(hash, hash+1, n);
        continue;
      } else { // single hash is a command
        copyLine = 0;
        //char *semi = strchr(hash, ';');
        //if (!semi) continue; // syntax error
        if (strncmp(hash, "#FluidBuildFileReferences;", 26)==0) {
          Fl_Type *src = tgt->next;
          while (src && src->level>tgt->level) {
            if (src->is_file()) {
              Fl_File_Type *f = (Fl_File_Type*)src;
              const char *fn = f->filename();
              if (fn) {
                const char *ext = fltk3::filename_ext(fn);
                if (ext && (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0)) {
                  char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode("Xcode4_PBXBuildFile"));
                  char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
                  // 836: DCB5F32CFF3DCFF6F2DA89E2 /* CodeEditor.cxx in Sources */ = {isa = PBXBuildFile; fileRef = CC0C80DA4DD31B6B2DB91096 /* CodeEditor.cxx */; };
                  fprintf(out, "\t\t%s /* %s in %s */ = {isa = PBXBuildFile; fileRef = %s /* %s */; };\n", 
                          PBXBuildFile, 
                          fltk3::filename_name(fn), 
                          "Sources", 
                          PBXFileRef, 
                          fltk3::filename_name(fn));
                }
              }
            }
            src = src->next;
          }
          hash += 26;
        } else if (strncmp(hash, "#FluidFileReferences;", 21)==0) {
          Fl_Type *src = tgt->next;
          while (src && src->level>tgt->level) {
            if (src->is_file()) {
              Fl_File_Type *f = (Fl_File_Type*)src;
              const char *fn = f->filename();
              if (fn) {
                const char *ext = fltk3::filename_ext(fn);
                char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
                // 4818: CC0C80DA4DD31B6B2DB91096 /* CodeEditor.cxx */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.cpp.cpp; name = CodeEditor.cxx; path = ../../fluid/CodeEditor.cxx; sourceTree = SOURCE_ROOT; };
                if (ext && (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0) ) {
                  fprintf(out, "\t\t%s /* %s */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.cpp.cpp; name = %s; path = ../../%s; sourceTree = SOURCE_ROOT; };\n", 
                          PBXFileRef,
                          fltk3::filename_name(fn), 
                          fltk3::filename_name(fn), 
                          fn);
                } else if (ext && (strcmp(ext, ".h")==0)) {
                    fprintf(out, "\t\t%s /* %s */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; name = %s; path = ../../%s; sourceTree = SOURCE_ROOT; };\n", 
                            PBXFileRef,
                            fltk3::filename_name(fn), 
                            fltk3::filename_name(fn), 
                            fn);
                }
              }
            }
            src = src->next;
          }
          hash += 21;
        } else if (strncmp(hash, "#FluidHeadersGroup;", 19)==0) {
          Fl_Type *src = tgt->next;
          while (src && src->level>tgt->level) {
            if (src->is_file()) {
              Fl_File_Type *f = (Fl_File_Type*)src;
              const char *fn = f->filename();
              if (fn) {
                const char *ext = fltk3::filename_ext(fn);
                if (ext && strcmp(ext, ".h")==0) {
                  char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
                  fprintf(out, "\t\t\t\t%s /* %s */,\n", 
                          PBXFileRef, 
                          fltk3::filename_name(fn));
                }
              }
            }
            src = src->next;
          }
          hash += 19;
        } else if (strncmp(hash, "#FluidSourcesGroup;", 19)==0) {
          Fl_Type *src = tgt->next;
          while (src && src->level>tgt->level) {
            if (src->is_file()) {
              Fl_File_Type *f = (Fl_File_Type*)src;
              const char *fn = f->filename();
              if (fn) {
                const char *ext = fltk3::filename_ext(fn);
                if (ext && (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0)) {
                  char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
                  // 7109: CC0C80DA4DD31B6B2DB91096 /* CodeEditor.cxx */,
                  fprintf(out, "\t\t\t\t%s /* %s */,\n", 
                          PBXFileRef, 
                          fltk3::filename_name(fn));
                }
              }
            }
            src = src->next;
          }
          hash += 19;
        } else if (strncmp(hash, "#FluidSourcesBuildPhase;", 23)==0) {
          Fl_Type *src = tgt->next;
          while (src && src->level>tgt->level) {
            if (src->is_file()) {
              Fl_File_Type *f = (Fl_File_Type*)src;
              const char *fn = f->filename();
              if (fn) {
                const char *ext = fltk3::filename_ext(fn);
                if (ext && (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0)) {
                  char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode("Xcode4_PBXBuildFile"));
                  // 10787: DCB5F32CFF3DCFF6F2DA89E2 /* CodeEditor.cxx in Sources */,
                  fprintf(out, "\t\t\t\t%s /* %s in %s */,\n", 
                          PBXBuildFile, 
                          fltk3::filename_name(fn), 
                          "Sources");
                }
              }
            }
            src = src->next;
          }
          hash += 23;
        } else {
          printf("Unknown command in template: <<%s>>\n", hash);
          copyLine = 1;
          hash++;
        }
      }
    }
    if (copyLine) fputs(buf, out);
  }
  
  fclose(in);
  fclose(out);
}

void write_fltk_ide_visualc6() {
  if (!filename) {
    printf("Workspace must be saved first\n");
    return;
  }
  
  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Type::first;
  while (tgt) {
    if (tgt->is_target() && strcmp(tgt->name(), "Fluid")==0)
      break;
    tgt = tgt->next;
  }
  if (!tgt) {
    printf("FLUID target not found\n");
    return;
  }
  
  /* Create a new Fluid.dsp */
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "ide/VisualC6/Fluid.dsp");
  FILE *out = fopen(buf, "wb");
  if (!out) {
    printf("Can't open FLUID VisualC 6 project file\n");
    return;
  }
  
  fprintf(out, "# Microsoft Developer Studio Project File - Name=\"Fluid\" - Package Owner=<4>\r\n");
  fprintf(out, "# Microsoft Developer Studio Generated Build File, Format Version 6.00\r\n");
  fprintf(out, "# ** DO NOT EDIT **\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# TARGTYPE \"Win32 (x86) Application\" 0x0101\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "CFG=Fluid - Win32 Debug\r\n");
  fprintf(out, "!MESSAGE This is not a valid makefile. To build this project using NMAKE,\r\n");
  fprintf(out, "!MESSAGE use the Export Makefile command and run\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE NMAKE /f \"Fluid.mak\".\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE You can specify a configuration when running NMAKE\r\n");
  fprintf(out, "!MESSAGE by defining the macro CFG on the command line. For example:\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE NMAKE /f \"Fluid.mak\" CFG=\"Fluid - Win32 Debug\"\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE Possible choices for configuration are:\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE \"Fluid - Win32 Release\" (based on \"Win32 (x86) Application\")\r\n");
  fprintf(out, "!MESSAGE \"Fluid - Win32 Debug\" (based on \"Win32 (x86) Application\")\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Begin Project\r\n");
  fprintf(out, "# PROP AllowPerConfigDependencies 0\r\n");
  fprintf(out, "# PROP Scc_ProjName \"\"\r\n");
  fprintf(out, "# PROP Scc_LocalPath \"\"\r\n");
  fprintf(out, "CPP=cl.exe\r\n");
  fprintf(out, "MTL=midl.exe\r\n");
  fprintf(out, "RSC=rc.exe\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "!IF  \"$(CFG)\" == \"Fluid - Win32 Release\"\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# PROP BASE Use_MFC 0\r\n");
  fprintf(out, "# PROP BASE Use_Debug_Libraries 0\r\n");
  fprintf(out, "# PROP BASE Output_Dir \"Release/Fluid\"\r\n");
  fprintf(out, "# PROP BASE Intermediate_Dir \"Release/Fluid\"\r\n");
  fprintf(out, "# PROP BASE Target_Dir \"\"\r\n");
  fprintf(out, "# PROP Use_MFC 0\r\n");
  fprintf(out, "# PROP Use_Debug_Libraries 0\r\n");
  fprintf(out, "# PROP Output_Dir \"Release/Fluid\"\r\n");
  fprintf(out, "# PROP Intermediate_Dir \"Release/Fluid\"\r\n");
  fprintf(out, "# PROP Ignore_Export_Lib 0\r\n");
  fprintf(out, "# PROP Target_Dir \"\"\r\n");
  fprintf(out, "# ADD BASE CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /YX /FD /c\r\n");
  fprintf(out, "# ADD CPP /nologo /MD /GX /Os /Ob2 /I \".\" /I \"../..\" /I \"../../zlib\" /I \"../../png\" /I \"../../jpeg\" /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"_CRT_SECURE_NO_DEPRECATE\" /D \"_CRT_NONSTDC_NO_DEPRECATE\" /D \"WIN32_LEAN_AND_MEAN\" /D \"VC_EXTRA_LEAN\" /D \"WIN32_EXTRA_LEAN\" /YX /FD /c\r\n");
  fprintf(out, "# ADD BASE MTL /nologo /D \"NDEBUG\" /mktyplib203 /o \"NUL\" /win32\r\n");
  fprintf(out, "# ADD MTL /nologo /D \"NDEBUG\" /mktyplib203 /o \"NUL\" /win32\r\n");
  fprintf(out, "# ADD BASE RSC /l 0x409 /d \"NDEBUG\"\r\n");
  fprintf(out, "# ADD RSC /l 0x409 /d \"NDEBUG\"\r\n");
  fprintf(out, "BSC32=bscmake.exe\r\n");
  fprintf(out, "# ADD BASE BSC32 /nologo\r\n");
  fprintf(out, "# ADD BSC32 /nologo\r\n");
  fprintf(out, "LINK32=link.exe\r\n");
  fprintf(out, "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386\r\n");
  fprintf(out, "# ADD LINK32 fltk.lib fltk_forms.lib fltk_images.lib fltk_jpeg.lib fltk_png.lib fltk_zlib.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:\"libcd\" /out:\"../../Fluid/Fluid.exe\" /pdbtype:sept /libpath:\"..\\..\\lib\"\r\n");
  fprintf(out, "# SUBTRACT LINK32 /pdb:none /incremental:yes\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "!ELSEIF  \"$(CFG)\" == \"Fluid - Win32 Debug\"\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# PROP BASE Use_MFC 0\r\n");
  fprintf(out, "# PROP BASE Use_Debug_Libraries 1\r\n");
  fprintf(out, "# PROP BASE Output_Dir \"Debug/Fluid\"\r\n");
  fprintf(out, "# PROP BASE Intermediate_Dir \"Debug/Fluid\"\r\n");
  fprintf(out, "# PROP BASE Target_Dir \"\"\r\n");
  fprintf(out, "# PROP Use_MFC 0\r\n");
  fprintf(out, "# PROP Use_Debug_Libraries 1\r\n");
  fprintf(out, "# PROP Output_Dir \"Debug/Fluid\"\r\n");
  fprintf(out, "# PROP Intermediate_Dir \"Debug/Fluid\"\r\n");
  fprintf(out, "# PROP Ignore_Export_Lib 0\r\n");
  fprintf(out, "# PROP Target_Dir \"\"\r\n");
  fprintf(out, "# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /YX /FD /c\r\n");
  fprintf(out, "# ADD CPP /nologo /MDd /Gm /GX /ZI /Od /I \".\" /I \"../..\" /I \"../../zlib\" /I \"../../png\" /I \"../../jpeg\" /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"_CRT_SECURE_NO_DEPRECATE\" /D \"_CRT_NONSTDC_NO_DEPRECATE\" /D \"WIN32_LEAN_AND_MEAN\" /D \"VC_EXTRA_LEAN\" /D \"WIN32_EXTRA_LEAN\" /YX /FD /c\r\n");
  fprintf(out, "# ADD BASE MTL /nologo /D \"_DEBUG\" /mktyplib203 /o \"NUL\" /win32\r\n");
  fprintf(out, "# ADD MTL /nologo /D \"_DEBUG\" /mktyplib203 /o \"NUL\" /win32\r\n");
  fprintf(out, "# ADD BASE RSC /l 0x409 /d \"_DEBUG\"\r\n");
  fprintf(out, "# ADD RSC /l 0x409 /d \"_DEBUG\"\r\n");
  fprintf(out, "BSC32=bscmake.exe\r\n");
  fprintf(out, "# ADD BASE BSC32 /nologo\r\n");
  fprintf(out, "# ADD BSC32 /nologo\r\n");
  fprintf(out, "LINK32=link.exe\r\n");
  fprintf(out, "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept\r\n");
  fprintf(out, "# ADD LINK32 fltkd.lib fltk_formsd.lib fltk_imagesd.lib fltk_jpegd.lib fltk_pngd.lib fltk_zlibd.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:\"libcd\" /out:\"../../Fluid/Fluidd.exe\" /pdbtype:sept /libpath:\"..\\..\\lib\"\r\n");
  fprintf(out, "# SUBTRACT LINK32 /pdb:none /incremental:no\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "!ENDIF \r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Begin Target\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Name \"Fluid - Win32 Release\"\r\n");
  fprintf(out, "# Name \"Fluid - Win32 Debug\"\r\n");
  
  /* loop through the target and write out all C++ files */
  Fl_Type *src = tgt->next;
  while (src && src->level>tgt->level) {
    if (src->is_file()) {
      Fl_File_Type *f = (Fl_File_Type*)src;
      const char *fn = f->filename();
      if (fn) {
        const char *ext = fltk3::filename_ext(fn);
        if (ext && (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0)) {
          char buf[2048], *c;
          strcpy(buf, fn);
          for (c=buf;;) {
            c = strchr(c, '/');
            if (!c) break;
            *c = '\\';
            c++;
          }
          fprintf(out, "# Begin Source File\r\n");
          fprintf(out, "\r\n");
          fprintf(out, "SOURCE=..\\..\\%s\r\n", buf);
          fprintf(out, "# End Source File\r\n");
        }
      }
    }
    src = src->next;
  }
  
  fprintf(out, "# End Target\r\n");
  fprintf(out, "# End Project\r\n");
  
  fclose(out);
}

void write_fltk_ide_visualc2008() {
  if (!filename) {
    printf("Workspace must be saved first\n");
    return;
  }
  
  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Type::first;
  while (tgt) {
    if (tgt->is_target() && strcmp(tgt->name(), "Fluid")==0)
      break;
    tgt = tgt->next;
  }
  if (!tgt) {
    printf("FLUID target not found\n");
    return;
  }
  
  /* Create a new Fluid.dsp */
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "ide/VisualC2008/fluid.vcproj");
  FILE *out = fopen(buf, "wb");
  if (!out) {
    printf("Can't open FLUID VisualC 2008 project file\n");
    return;
  }
  
  fprintf(out, "<?xml version=\"1.0\" encoding=\"Windows-1252\"?>\r\n");
  fprintf(out, "<VisualStudioProject\r\n");
  fprintf(out, "\tProjectType=\"Visual C++\"\r\n");
  fprintf(out, "\tVersion=\"9,00\"\r\n");
  fprintf(out, "\tName=\"fluid\"\r\n");
  fprintf(out, "\tProjectGUID=\"{8AED3078-8CD8-40C9-A8FF-46080024F1EB}\"\r\n");
  fprintf(out, "\tRootNamespace=\"fluid\"\r\n");
  fprintf(out, "\tTargetFrameworkVersion=\"131072\"\r\n");
  fprintf(out, "\t>\r\n");
  fprintf(out, "\t<Platforms>\r\n");
  fprintf(out, "\t\t<Platform\r\n");
  fprintf(out, "\t\t\tName=\"Win32\"\r\n");
  fprintf(out, "\t\t/>\r\n");
  fprintf(out, "\t</Platforms>\r\n");
  fprintf(out, "\t<ToolFiles>\r\n");
  fprintf(out, "\t</ToolFiles>\r\n");
  fprintf(out, "\t<Configurations>\r\n");
  fprintf(out, "\t\t<Configuration\r\n");
  fprintf(out, "\t\t\tName=\"Debug|Win32\"\r\n");
  fprintf(out, "\t\t\tOutputDirectory=\".\\fluid__0\"\r\n");
  fprintf(out, "\t\t\tIntermediateDirectory=\".\\fluid__0\"\r\n");
  fprintf(out, "\t\t\tConfigurationType=\"1\"\r\n");
  fprintf(out, "\t\t\tInheritedPropertySheets=\"$(VCInstallDir)VCProjectDefaults\\UpgradeFromVC71.vsprops\"\r\n");
  fprintf(out, "\t\t\tUseOfMFC=\"0\"\r\n");
  fprintf(out, "\t\t\tATLMinimizesCRunTimeLibraryUsage=\"false\"\r\n");
  fprintf(out, "\t\t\t>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPreBuildEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCCustomBuildTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCXMLDataGeneratorTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCWebServiceProxyGeneratorTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCMIDLTool\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"_DEBUG\"\r\n");
  fprintf(out, "\t\t\t\tMkTypLibCompatible=\"true\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tTargetEnvironment=\"1\"\r\n");
  fprintf(out, "\t\t\t\tTypeLibraryName=\".\\fluid__0/fluid.tlb\"\r\n");
  fprintf(out, "\t\t\t\tHeaderFileName=\"\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCCLCompilerTool\"\r\n");
  fprintf(out, "\t\t\t\tOptimization=\"0\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalIncludeDirectories=\".;..\\..\\zlib;..\\..\\png;..\\..\\jpeg;../..\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"_CRT_SECURE_NO_DEPRECATE;WIN32;_DEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN\"\r\n");
  fprintf(out, "\t\t\t\tRuntimeLibrary=\"3\"\r\n");
  fprintf(out, "\t\t\t\tUsePrecompiledHeader=\"0\"\r\n");
  fprintf(out, "\t\t\t\tPrecompiledHeaderFile=\".\\fluid__0/fluid.pch\"\r\n");
  fprintf(out, "\t\t\t\tAssemblerListingLocation=\".\\fluid__0/\"\r\n");
  fprintf(out, "\t\t\t\tObjectFile=\".\\fluid__0/\"\r\n");
  fprintf(out, "\t\t\t\tProgramDataBaseFileName=\".\\fluid__0/\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tDebugInformationFormat=\"3\"\r\n");
  fprintf(out, "\t\t\t\tCompileAs=\"0\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCManagedResourceCompilerTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCResourceCompilerTool\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"_DEBUG\"\r\n");
  fprintf(out, "\t\t\t\tCulture=\"1033\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPreLinkEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCLinkerTool\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalDependencies=\"comctl32.lib\"\r\n");
  fprintf(out, "\t\t\t\tOutputFile=\"../../fluid/fluidd.exe\"\r\n");
  fprintf(out, "\t\t\t\tLinkIncremental=\"1\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalLibraryDirectories=\"..\\..\\lib\"\r\n");
  fprintf(out, "\t\t\t\tIgnoreDefaultLibraryNames=\"libcmtd\"\r\n");
  fprintf(out, "\t\t\t\tGenerateDebugInformation=\"true\"\r\n");
  fprintf(out, "\t\t\t\tProgramDatabaseFile=\".\\fluid__0/fluidd.pdb\"\r\n");
  fprintf(out, "\t\t\t\tSubSystem=\"2\"\r\n");
  fprintf(out, "\t\t\t\tRandomizedBaseAddress=\"1\"\r\n");
  fprintf(out, "\t\t\t\tDataExecutionPrevention=\"0\"\r\n");
  fprintf(out, "\t\t\t\tTargetMachine=\"1\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCALinkTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCManifestTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCXDCMakeTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCBscMakeTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCFxCopTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCAppVerifierTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPostBuildEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t</Configuration>\r\n");
  fprintf(out, "\t\t<Configuration\r\n");
  fprintf(out, "\t\t\tName=\"Release|Win32\"\r\n");
  fprintf(out, "\t\t\tOutputDirectory=\".\\fluid___\"\r\n");
  fprintf(out, "\t\t\tIntermediateDirectory=\".\\fluid___\"\r\n");
  fprintf(out, "\t\t\tConfigurationType=\"1\"\r\n");
  fprintf(out, "\t\t\tInheritedPropertySheets=\"$(VCInstallDir)VCProjectDefaults\\UpgradeFromVC71.vsprops\"\r\n");
  fprintf(out, "\t\t\tUseOfMFC=\"0\"\r\n");
  fprintf(out, "\t\t\tATLMinimizesCRunTimeLibraryUsage=\"false\"\r\n");
  fprintf(out, "\t\t\t>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPreBuildEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCCustomBuildTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCXMLDataGeneratorTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCWebServiceProxyGeneratorTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCMIDLTool\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"NDEBUG\"\r\n");
  fprintf(out, "\t\t\t\tMkTypLibCompatible=\"true\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tTargetEnvironment=\"1\"\r\n");
  fprintf(out, "\t\t\t\tTypeLibraryName=\".\\fluid___/fluid.tlb\"\r\n");
  fprintf(out, "\t\t\t\tHeaderFileName=\"\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCCLCompilerTool\"\r\n");
  fprintf(out, "\t\t\t\tOptimization=\"4\"\r\n");
  fprintf(out, "\t\t\t\tInlineFunctionExpansion=\"2\"\r\n");
  fprintf(out, "\t\t\t\tFavorSizeOrSpeed=\"0\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalIncludeDirectories=\".;..\\..\\zlib;..\\..\\png;..\\..\\jpeg;../..\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"_CRT_SECURE_NO_DEPRECATE;WIN32;NDEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN\"\r\n");
  fprintf(out, "\t\t\t\tRuntimeLibrary=\"2\"\r\n");
  fprintf(out, "\t\t\t\tUsePrecompiledHeader=\"0\"\r\n");
  fprintf(out, "\t\t\t\tPrecompiledHeaderFile=\".\\fluid___/fluid.pch\"\r\n");
  fprintf(out, "\t\t\t\tAssemblerListingLocation=\".\\fluid___/\"\r\n");
  fprintf(out, "\t\t\t\tObjectFile=\".\\fluid___/\"\r\n");
  fprintf(out, "\t\t\t\tProgramDataBaseFileName=\".\\fluid___/\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tCompileAs=\"0\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCManagedResourceCompilerTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCResourceCompilerTool\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"NDEBUG\"\r\n");
  fprintf(out, "\t\t\t\tCulture=\"1033\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPreLinkEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCLinkerTool\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalDependencies=\"comctl32.lib\"\r\n");
  fprintf(out, "\t\t\t\tOutputFile=\"../../fluid/fluid.exe\"\r\n");
  fprintf(out, "\t\t\t\tLinkIncremental=\"1\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalLibraryDirectories=\"..\\..\\lib\"\r\n");
  fprintf(out, "\t\t\t\tIgnoreDefaultLibraryNames=\"libcmt\"\r\n");
  fprintf(out, "\t\t\t\tProgramDatabaseFile=\".\\fluid___/fluid.pdb\"\r\n");
  fprintf(out, "\t\t\t\tSubSystem=\"2\"\r\n");
  fprintf(out, "\t\t\t\tRandomizedBaseAddress=\"1\"\r\n");
  fprintf(out, "\t\t\t\tDataExecutionPrevention=\"0\"\r\n");
  fprintf(out, "\t\t\t\tTargetMachine=\"1\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCALinkTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCManifestTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCXDCMakeTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCBscMakeTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCFxCopTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCAppVerifierTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPostBuildEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t</Configuration>\r\n");
  fprintf(out, "\t\t<Configuration\r\n");
  fprintf(out, "\t\t\tName=\"Debug Cairo|Win32\"\r\n");
  fprintf(out, "\t\t\tOutputDirectory=\"$(ConfigurationName)\"\r\n");
  fprintf(out, "\t\t\tIntermediateDirectory=\"$(ConfigurationName)\"\r\n");
  fprintf(out, "\t\t\tConfigurationType=\"1\"\r\n");
  fprintf(out, "\t\t\tInheritedPropertySheets=\"$(VCInstallDir)VCProjectDefaults\\UpgradeFromVC71.vsprops\"\r\n");
  fprintf(out, "\t\t\tUseOfMFC=\"0\"\r\n");
  fprintf(out, "\t\t\tATLMinimizesCRunTimeLibraryUsage=\"false\"\r\n");
  fprintf(out, "\t\t\t>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPreBuildEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCCustomBuildTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCXMLDataGeneratorTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCWebServiceProxyGeneratorTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCMIDLTool\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"_DEBUG\"\r\n");
  fprintf(out, "\t\t\t\tMkTypLibCompatible=\"true\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tTargetEnvironment=\"1\"\r\n");
  fprintf(out, "\t\t\t\tTypeLibraryName=\".\\fluid__0/fluid.tlb\"\r\n");
  fprintf(out, "\t\t\t\tHeaderFileName=\"\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCCLCompilerTool\"\r\n");
  fprintf(out, "\t\t\t\tOptimization=\"0\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalIncludeDirectories=\".;..\\..\\zlib;..\\..\\png;..\\..\\jpeg;../..\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"FLTK_HAVE_CAIRO=1;_CRT_SECURE_NO_DEPRECATE;WIN32;_DEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN\"\r\n");
  fprintf(out, "\t\t\t\tRuntimeLibrary=\"3\"\r\n");
  fprintf(out, "\t\t\t\tUsePrecompiledHeader=\"0\"\r\n");
  fprintf(out, "\t\t\t\tPrecompiledHeaderFile=\".\\fluid__0/fluid.pch\"\r\n");
  fprintf(out, "\t\t\t\tAssemblerListingLocation=\".\\fluid__0/\"\r\n");
  fprintf(out, "\t\t\t\tObjectFile=\".\\fluid__0/\"\r\n");
  fprintf(out, "\t\t\t\tProgramDataBaseFileName=\".\\fluid__0/\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tDebugInformationFormat=\"3\"\r\n");
  fprintf(out, "\t\t\t\tCompileAs=\"0\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCManagedResourceCompilerTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCResourceCompilerTool\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"_DEBUG\"\r\n");
  fprintf(out, "\t\t\t\tCulture=\"1033\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPreLinkEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCLinkerTool\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalDependencies=\"cairo.lib comctl32.lib\"\r\n");
  fprintf(out, "\t\t\t\tOutputFile=\"../fluid/fluidd.exe\"\r\n");
  fprintf(out, "\t\t\t\tLinkIncremental=\"1\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalLibraryDirectories=\"..\\..\\lib\"\r\n");
  fprintf(out, "\t\t\t\tIgnoreDefaultLibraryNames=\"libcmtd\"\r\n");
  fprintf(out, "\t\t\t\tGenerateDebugInformation=\"true\"\r\n");
  fprintf(out, "\t\t\t\tProgramDatabaseFile=\".\\fluid__0/fluidd.pdb\"\r\n");
  fprintf(out, "\t\t\t\tSubSystem=\"2\"\r\n");
  fprintf(out, "\t\t\t\tRandomizedBaseAddress=\"1\"\r\n");
  fprintf(out, "\t\t\t\tDataExecutionPrevention=\"0\"\r\n");
  fprintf(out, "\t\t\t\tTargetMachine=\"1\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCALinkTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCManifestTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCXDCMakeTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCBscMakeTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCFxCopTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCAppVerifierTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPostBuildEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t</Configuration>\r\n");
  fprintf(out, "\t\t<Configuration\r\n");
  fprintf(out, "\t\t\tName=\"Release Cairo|Win32\"\r\n");
  fprintf(out, "\t\t\tOutputDirectory=\"$(ConfigurationName)\"\r\n");
  fprintf(out, "\t\t\tIntermediateDirectory=\"$(ConfigurationName)\"\r\n");
  fprintf(out, "\t\t\tConfigurationType=\"1\"\r\n");
  fprintf(out, "\t\t\tInheritedPropertySheets=\"$(VCInstallDir)VCProjectDefaults\\UpgradeFromVC71.vsprops\"\r\n");
  fprintf(out, "\t\t\tUseOfMFC=\"0\"\r\n");
  fprintf(out, "\t\t\tATLMinimizesCRunTimeLibraryUsage=\"false\"\r\n");
  fprintf(out, "\t\t\t>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPreBuildEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCCustomBuildTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCXMLDataGeneratorTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCWebServiceProxyGeneratorTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCMIDLTool\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"NDEBUG\"\r\n");
  fprintf(out, "\t\t\t\tMkTypLibCompatible=\"true\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tTargetEnvironment=\"1\"\r\n");
  fprintf(out, "\t\t\t\tTypeLibraryName=\".\\fluid___/fluid.tlb\"\r\n");
  fprintf(out, "\t\t\t\tHeaderFileName=\"\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCCLCompilerTool\"\r\n");
  fprintf(out, "\t\t\t\tOptimization=\"4\"\r\n");
  fprintf(out, "\t\t\t\tInlineFunctionExpansion=\"2\"\r\n");
  fprintf(out, "\t\t\t\tFavorSizeOrSpeed=\"0\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalIncludeDirectories=\".;..\\..\\zlib;..\\..\\png;..\\..\\jpeg;../..\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"FLTK_HAVE_CAIRO=1,_CRT_SECURE_NO_DEPRECATE;WIN32;NDEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN\"\r\n");
  fprintf(out, "\t\t\t\tRuntimeLibrary=\"2\"\r\n");
  fprintf(out, "\t\t\t\tUsePrecompiledHeader=\"0\"\r\n");
  fprintf(out, "\t\t\t\tPrecompiledHeaderFile=\".\\fluid___/fluid.pch\"\r\n");
  fprintf(out, "\t\t\t\tAssemblerListingLocation=\".\\fluid___/\"\r\n");
  fprintf(out, "\t\t\t\tObjectFile=\".\\fluid___/\"\r\n");
  fprintf(out, "\t\t\t\tProgramDataBaseFileName=\".\\fluid___/\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tCompileAs=\"0\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCManagedResourceCompilerTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCResourceCompilerTool\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"NDEBUG\"\r\n");
  fprintf(out, "\t\t\t\tCulture=\"1033\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPreLinkEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCLinkerTool\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalDependencies=\"comctl32.lib cairo.lib\"\r\n");
  fprintf(out, "\t\t\t\tOutputFile=\"../fluid/fluid.exe\"\r\n");
  fprintf(out, "\t\t\t\tLinkIncremental=\"1\"\r\n");
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  fprintf(out, "\t\t\t\tAdditionalLibraryDirectories=\"..\\..\\lib\"\r\n");
  fprintf(out, "\t\t\t\tIgnoreDefaultLibraryNames=\"libcmt\"\r\n");
  fprintf(out, "\t\t\t\tProgramDatabaseFile=\".\\fluid___/fluid.pdb\"\r\n");
  fprintf(out, "\t\t\t\tSubSystem=\"2\"\r\n");
  fprintf(out, "\t\t\t\tRandomizedBaseAddress=\"1\"\r\n");
  fprintf(out, "\t\t\t\tDataExecutionPrevention=\"0\"\r\n");
  fprintf(out, "\t\t\t\tTargetMachine=\"1\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCALinkTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCManifestTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCXDCMakeTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCBscMakeTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCFxCopTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCAppVerifierTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPostBuildEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t</Configuration>\r\n");
  fprintf(out, "\t</Configurations>\r\n");
  fprintf(out, "\t<References>\r\n");
  fprintf(out, "\t</References>\r\n");
  fprintf(out, "\t<Files>\r\n");
  
  /* loop through the target and write out all C++ files */
  Fl_Type *src = tgt->next;
  while (src && src->level>tgt->level) {
    if (src->is_file()) {
      Fl_File_Type *f = (Fl_File_Type*)src;
      const char *fn = f->filename();
      if (fn) {
        const char *ext = fltk3::filename_ext(fn);
        if (ext && (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0)) {
          char buf[2048], *c;
          strcpy(buf, fn);
          for (c=buf;;) {
            c = strchr(c, '/');
            if (!c) break;
            *c = '\\';
            c++;
          }          
          fprintf(out, "\t\t<File\r\n");
          fprintf(out, "\t\t\tRelativePath=\"..\\..\\%s\"\r\n", buf);
          fprintf(out, "\t\t\t>\r\n");
          fprintf(out, "\t\t\t<FileConfiguration\r\n");
          fprintf(out, "\t\t\t\tName=\"Debug|Win32\"\r\n");
          fprintf(out, "\t\t\t\t>\r\n");
          fprintf(out, "\t\t\t\t<Tool\r\n");
          fprintf(out, "\t\t\t\t\tName=\"VCCLCompilerTool\"\r\n");
          fprintf(out, "\t\t\t\t\tOptimization=\"0\"\r\n");
          fprintf(out, "\t\t\t\t\tAdditionalIncludeDirectories=\"\"\r\n");
          fprintf(out, "\t\t\t\t\tPreprocessorDefinitions=\"\"\r\n");
          fprintf(out, "\t\t\t\t/>\r\n");
          fprintf(out, "\t\t\t</FileConfiguration>\r\n");
          fprintf(out, "\t\t\t<FileConfiguration\r\n");
          fprintf(out, "\t\t\t\tName=\"Release|Win32\"\r\n");
          fprintf(out, "\t\t\t\t>\r\n");
          fprintf(out, "\t\t\t\t<Tool\r\n");
          fprintf(out, "\t\t\t\t\tName=\"VCCLCompilerTool\"\r\n");
          fprintf(out, "\t\t\t\t\tFavorSizeOrSpeed=\"0\"\r\n");
          fprintf(out, "\t\t\t\t\tAdditionalIncludeDirectories=\"\"\r\n");
          fprintf(out, "\t\t\t\t\tPreprocessorDefinitions=\"\"\r\n");
          fprintf(out, "\t\t\t\t/>\r\n");
          fprintf(out, "\t\t\t</FileConfiguration>\r\n");
          fprintf(out, "\t\t\t<FileConfiguration\r\n");
          fprintf(out, "\t\t\t\tName=\"Debug Cairo|Win32\"\r\n");
          fprintf(out, "\t\t\t\t>\r\n");
          fprintf(out, "\t\t\t\t<Tool\r\n");
          fprintf(out, "\t\t\t\t\tName=\"VCCLCompilerTool\"\r\n");
          fprintf(out, "\t\t\t\t\tOptimization=\"0\"\r\n");
          fprintf(out, "\t\t\t\t\tAdditionalIncludeDirectories=\"\"\r\n");
          fprintf(out, "\t\t\t\t\tPreprocessorDefinitions=\"\"\r\n");
          fprintf(out, "\t\t\t\t/>\r\n");
          fprintf(out, "\t\t\t</FileConfiguration>\r\n");
          fprintf(out, "\t\t\t<FileConfiguration\r\n");
          fprintf(out, "\t\t\t\tName=\"Release Cairo|Win32\"\r\n");
          fprintf(out, "\t\t\t\t>\r\n");
          fprintf(out, "\t\t\t\t<Tool\r\n");
          fprintf(out, "\t\t\t\t\tName=\"VCCLCompilerTool\"\r\n");
          fprintf(out, "\t\t\t\t\tFavorSizeOrSpeed=\"0\"\r\n");
          fprintf(out, "\t\t\t\t\tAdditionalIncludeDirectories=\"\"\r\n");
          fprintf(out, "\t\t\t\t\tPreprocessorDefinitions=\"\"\r\n");
          fprintf(out, "\t\t\t\t/>\r\n");
          fprintf(out, "\t\t\t</FileConfiguration>\r\n");
          fprintf(out, "\t\t</File>\r\n");          
        }
      }
    }
    src = src->next;
  }
  
  fprintf(out, "\t</Files>\r\n");
  fprintf(out, "\t<Globals>\r\n");
  fprintf(out, "\t</Globals>\r\n");
  fprintf(out, "</VisualStudioProject>\r\n");
  
  fclose(out);
}

void write_fltk_ide_visualc2010() {
  if (!filename) {
    printf("Workspace must be saved first\n");
    return;
  }
  
  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Type::first;
  while (tgt) {
    if (tgt->is_target() && strcmp(tgt->name(), "Fluid")==0)
      break;
    tgt = tgt->next;
  }
  if (!tgt) {
    printf("FLUID target not found\n");
    return;
  }
  
  /* Create a new Fluid.dsp */
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "ide/VisualC2010/fluid.vcxproj");
  FILE *out = fopen(buf, "wb");
  if (!out) {
    printf("Can't open FLUID VisualC 2010 project file\n");
    return;
  }
  
  fprintf(out, "\357\273\277<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
  fprintf(out, "<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n");
  fprintf(out, "  <ItemGroup Label=\"ProjectConfigurations\">\r\n");
  fprintf(out, "    <ProjectConfiguration Include=\"Debug Cairo|Win32\">\r\n");
  fprintf(out, "      <Configuration>Debug Cairo</Configuration>\r\n");
  fprintf(out, "      <Platform>Win32</Platform>\r\n");
  fprintf(out, "    </ProjectConfiguration>\r\n");
  fprintf(out, "    <ProjectConfiguration Include=\"Debug|Win32\">\r\n");
  fprintf(out, "      <Configuration>Debug</Configuration>\r\n");
  fprintf(out, "      <Platform>Win32</Platform>\r\n");
  fprintf(out, "    </ProjectConfiguration>\r\n");
  fprintf(out, "    <ProjectConfiguration Include=\"Release Cairo|Win32\">\r\n");
  fprintf(out, "      <Configuration>Release Cairo</Configuration>\r\n");
  fprintf(out, "      <Platform>Win32</Platform>\r\n");
  fprintf(out, "    </ProjectConfiguration>\r\n");
  fprintf(out, "    <ProjectConfiguration Include=\"Release|Win32\">\r\n");
  fprintf(out, "      <Configuration>Release</Configuration>\r\n");
  fprintf(out, "      <Platform>Win32</Platform>\r\n");
  fprintf(out, "    </ProjectConfiguration>\r\n");
  fprintf(out, "  </ItemGroup>\r\n");
  fprintf(out, "  <PropertyGroup Label=\"Globals\">\r\n");
  fprintf(out, "    <ProjectGuid>{8AED3078-8CD8-40C9-A8FF-46080024F1EB}</ProjectGuid>\r\n");
  fprintf(out, "    <RootNamespace>fluid</RootNamespace>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  fprintf(out, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\r\n");
  fprintf(out, "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release Cairo|Win32'\" Label=\"Configuration\">\r\n");
  fprintf(out, "    <ConfigurationType>Application</ConfigurationType>\r\n");
  fprintf(out, "    <UseOfMfc>false</UseOfMfc>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  fprintf(out, "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug Cairo|Win32'\" Label=\"Configuration\">\r\n");
  fprintf(out, "    <ConfigurationType>Application</ConfigurationType>\r\n");
  fprintf(out, "    <UseOfMfc>false</UseOfMfc>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  fprintf(out, "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">\r\n");
  fprintf(out, "    <ConfigurationType>Application</ConfigurationType>\r\n");
  fprintf(out, "    <UseOfMfc>false</UseOfMfc>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  fprintf(out, "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">\r\n");
  fprintf(out, "    <ConfigurationType>Application</ConfigurationType>\r\n");
  fprintf(out, "    <UseOfMfc>false</UseOfMfc>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  fprintf(out, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\r\n");
  fprintf(out, "  <ImportGroup Label=\"ExtensionSettings\">\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  fprintf(out, "  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Release Cairo|Win32'\" Label=\"PropertySheets\">\r\n");
  fprintf(out, "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\r\n");
  fprintf(out, "    <Import Project=\"$(VCTargetsPath)Microsoft.CPP.UpgradeFromVC71.props\" />\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  fprintf(out, "  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug Cairo|Win32'\" Label=\"PropertySheets\">\r\n");
  fprintf(out, "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\r\n");
  fprintf(out, "    <Import Project=\"$(VCTargetsPath)Microsoft.CPP.UpgradeFromVC71.props\" />\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  fprintf(out, "  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"PropertySheets\">\r\n");
  fprintf(out, "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\r\n");
  fprintf(out, "    <Import Project=\"$(VCTargetsPath)Microsoft.CPP.UpgradeFromVC71.props\" />\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  fprintf(out, "  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"PropertySheets\">\r\n");
  fprintf(out, "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\r\n");
  fprintf(out, "    <Import Project=\"$(VCTargetsPath)Microsoft.CPP.UpgradeFromVC71.props\" />\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  fprintf(out, "  <PropertyGroup Label=\"UserMacros\" />\r\n");
  fprintf(out, "  <PropertyGroup>\r\n");
  fprintf(out, "    <_ProjectFileVersion>10.0.30319.1</_ProjectFileVersion>\r\n");
  fprintf(out, "    <OutDir Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">..\\..\\fluid\\</OutDir>\r\n");
  fprintf(out, "    <IntDir Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">.\\$(ProjectName)_debug\\</IntDir>\r\n");
  fprintf(out, "    <LinkIncremental Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</LinkIncremental>\r\n");
  fprintf(out, "    <OutDir Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">..\\..\\fluid\\</OutDir>\r\n");
  fprintf(out, "    <IntDir Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">.\\$(ProjectName)_release\\</IntDir>\r\n");
  fprintf(out, "    <LinkIncremental Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">false</LinkIncremental>\r\n");
  fprintf(out, "    <OutDir Condition=\"'$(Configuration)|$(Platform)'=='Debug Cairo|Win32'\">..\\..\\fluid\\</OutDir>\r\n");
  fprintf(out, "    <IntDir Condition=\"'$(Configuration)|$(Platform)'=='Debug Cairo|Win32'\">$(Configuration)\\</IntDir>\r\n");
  fprintf(out, "    <LinkIncremental Condition=\"'$(Configuration)|$(Platform)'=='Debug Cairo|Win32'\">false</LinkIncremental>\r\n");
  fprintf(out, "    <OutDir Condition=\"'$(Configuration)|$(Platform)'=='Release Cairo|Win32'\">..\\..\\fluid\\</OutDir>\r\n");
  fprintf(out, "    <IntDir Condition=\"'$(Configuration)|$(Platform)'=='Release Cairo|Win32'\">$(Configuration)\\</IntDir>\r\n");
  fprintf(out, "    <LinkIncremental Condition=\"'$(Configuration)|$(Platform)'=='Release Cairo|Win32'\">false</LinkIncremental>\r\n");
  fprintf(out, "    <TargetName Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">$(ProjectName)d</TargetName>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  fprintf(out, "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">\r\n");
  fprintf(out, "    <Midl>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>_DEBUG;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <MkTypLibCompatible>true</MkTypLibCompatible>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <TargetEnvironment>Win32</TargetEnvironment>\r\n");
  fprintf(out, "      <TypeLibraryName>.\\fluid__0/fluid.tlb</TypeLibraryName>\r\n");
  fprintf(out, "      <HeaderFileName>\r\n");
  fprintf(out, "      </HeaderFileName>\r\n");
  fprintf(out, "    </Midl>\r\n");
  fprintf(out, "    <ClCompile>\r\n");
  fprintf(out, "      <Optimization>Disabled</Optimization>\r\n");
  fprintf(out, "      <AdditionalIncludeDirectories>.;..\\..\\zlib;..\\..\\png;..\\..\\jpeg;../..;%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>_CRT_SECURE_NO_DEPRECATE;WIN32;_DEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>\r\n");
  fprintf(out, "      <PrecompiledHeader>\r\n");
  fprintf(out, "      </PrecompiledHeader>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>\r\n");
  fprintf(out, "      <CompileAs>Default</CompileAs>\r\n");
  fprintf(out, "    </ClCompile>\r\n");
  fprintf(out, "    <ResourceCompile>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>_DEBUG;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <Culture>0x0409</Culture>\r\n");
  fprintf(out, "    </ResourceCompile>\r\n");
  fprintf(out, "    <Link>\r\n");
  fprintf(out, "      <AdditionalDependencies>comctl32.lib;%%(AdditionalDependencies)</AdditionalDependencies>\r\n");
  fprintf(out, "      <OutputFile>../../fluid/fluidd.exe</OutputFile>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <AdditionalLibraryDirectories>..\\..\\lib;%%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\r\n");
  fprintf(out, "      <IgnoreSpecificDefaultLibraries>libcmtd;%%(IgnoreSpecificDefaultLibraries)</IgnoreSpecificDefaultLibraries>\r\n");
  fprintf(out, "      <GenerateDebugInformation>true</GenerateDebugInformation>\r\n");
  fprintf(out, "      <ProgramDatabaseFile>$(IntDir)$(TargetName).pdb</ProgramDatabaseFile>\r\n");
  fprintf(out, "      <SubSystem>Windows</SubSystem>\r\n");
  fprintf(out, "      <RandomizedBaseAddress>false</RandomizedBaseAddress>\r\n");
  fprintf(out, "      <DataExecutionPrevention>\r\n");
  fprintf(out, "      </DataExecutionPrevention>\r\n");
  fprintf(out, "      <TargetMachine>MachineX86</TargetMachine>\r\n");
  fprintf(out, "    </Link>\r\n");
  fprintf(out, "  </ItemDefinitionGroup>\r\n");
  fprintf(out, "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">\r\n");
  fprintf(out, "    <Midl>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>NDEBUG;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <MkTypLibCompatible>true</MkTypLibCompatible>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <TargetEnvironment>Win32</TargetEnvironment>\r\n");
  fprintf(out, "      <TypeLibraryName>.\\fluid___/fluid.tlb</TypeLibraryName>\r\n");
  fprintf(out, "      <HeaderFileName>\r\n");
  fprintf(out, "      </HeaderFileName>\r\n");
  fprintf(out, "    </Midl>\r\n");
  fprintf(out, "    <ClCompile>\r\n");
  fprintf(out, "      <Optimization>Disabled</Optimization>\r\n");
  fprintf(out, "      <InlineFunctionExpansion>AnySuitable</InlineFunctionExpansion>\r\n");
  fprintf(out, "      <FavorSizeOrSpeed>Neither</FavorSizeOrSpeed>\r\n");
  fprintf(out, "      <AdditionalIncludeDirectories>.;..\\..\\zlib;..\\..\\png;..\\..\\jpeg;../..;%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>_CRT_SECURE_NO_DEPRECATE;WIN32;NDEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>\r\n");
  fprintf(out, "      <PrecompiledHeader>\r\n");
  fprintf(out, "      </PrecompiledHeader>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <CompileAs>Default</CompileAs>\r\n");
  fprintf(out, "      <AssemblerListingLocation>$(IntDir)</AssemblerListingLocation>\r\n");
  fprintf(out, "      <ObjectFileName>$(IntDir)</ObjectFileName>\r\n");
  fprintf(out, "      <ProgramDataBaseFileName>$(IntDir)vc$(PlatformToolsetVersion).pdb</ProgramDataBaseFileName>\r\n");
  fprintf(out, "    </ClCompile>\r\n");
  fprintf(out, "    <ResourceCompile>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>NDEBUG;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <Culture>0x0409</Culture>\r\n");
  fprintf(out, "    </ResourceCompile>\r\n");
  fprintf(out, "    <Link>\r\n");
  fprintf(out, "      <AdditionalDependencies>comctl32.lib;%%(AdditionalDependencies)</AdditionalDependencies>\r\n");
  fprintf(out, "      <OutputFile>../../fluid/fluid.exe</OutputFile>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <AdditionalLibraryDirectories>..\\..\\lib;%%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\r\n");
  fprintf(out, "      <IgnoreSpecificDefaultLibraries>libcmt;%%(IgnoreSpecificDefaultLibraries)</IgnoreSpecificDefaultLibraries>\r\n");
  fprintf(out, "      <ProgramDatabaseFile>$(IntDir)$(TargetName).pdb</ProgramDatabaseFile>\r\n");
  fprintf(out, "      <SubSystem>Windows</SubSystem>\r\n");
  fprintf(out, "      <RandomizedBaseAddress>false</RandomizedBaseAddress>\r\n");
  fprintf(out, "      <DataExecutionPrevention>\r\n");
  fprintf(out, "      </DataExecutionPrevention>\r\n");
  fprintf(out, "      <TargetMachine>MachineX86</TargetMachine>\r\n");
  fprintf(out, "    </Link>\r\n");
  fprintf(out, "  </ItemDefinitionGroup>\r\n");
  fprintf(out, "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug Cairo|Win32'\">\r\n");
  fprintf(out, "    <Midl>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>_DEBUG;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <MkTypLibCompatible>true</MkTypLibCompatible>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <TargetEnvironment>Win32</TargetEnvironment>\r\n");
  fprintf(out, "      <TypeLibraryName>.\\fluid__0/fluid.tlb</TypeLibraryName>\r\n");
  fprintf(out, "      <HeaderFileName>\r\n");
  fprintf(out, "      </HeaderFileName>\r\n");
  fprintf(out, "    </Midl>\r\n");
  fprintf(out, "    <ClCompile>\r\n");
  fprintf(out, "      <Optimization>Disabled</Optimization>\r\n");
  fprintf(out, "      <AdditionalIncludeDirectories>.;..\\..\\zlib;..\\..\\png;..\\..\\jpeg;../..;%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>FLTK_HAVE_CAIRO=1;_CRT_SECURE_NO_DEPRECATE;WIN32;_DEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>\r\n");
  fprintf(out, "      <PrecompiledHeader>\r\n");
  fprintf(out, "      </PrecompiledHeader>\r\n");
  fprintf(out, "      <PrecompiledHeaderOutputFile>.\\fluid__0/fluid.pch</PrecompiledHeaderOutputFile>\r\n");
  fprintf(out, "      <AssemblerListingLocation>.\\fluid__0/</AssemblerListingLocation>\r\n");
  fprintf(out, "      <ObjectFileName>.\\fluid__0/</ObjectFileName>\r\n");
  fprintf(out, "      <ProgramDataBaseFileName>.\\fluid__0/</ProgramDataBaseFileName>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>\r\n");
  fprintf(out, "      <CompileAs>Default</CompileAs>\r\n");
  fprintf(out, "    </ClCompile>\r\n");
  fprintf(out, "    <ResourceCompile>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>_DEBUG;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <Culture>0x0409</Culture>\r\n");
  fprintf(out, "    </ResourceCompile>\r\n");
  fprintf(out, "    <Link>\r\n");
  fprintf(out, "      <AdditionalDependencies>cairo.lib;comctl32.lib;%%(AdditionalDependencies)</AdditionalDependencies>\r\n");
  fprintf(out, "      <OutputFile>../fluid/fluidd.exe</OutputFile>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <AdditionalLibraryDirectories>..\\..\\lib;%%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\r\n");
  fprintf(out, "      <IgnoreSpecificDefaultLibraries>libcmtd;%%(IgnoreSpecificDefaultLibraries)</IgnoreSpecificDefaultLibraries>\r\n");
  fprintf(out, "      <GenerateDebugInformation>true</GenerateDebugInformation>\r\n");
  fprintf(out, "      <ProgramDatabaseFile>$(IntDir)$(TargetName).pdb</ProgramDatabaseFile>\r\n");
  fprintf(out, "      <SubSystem>Windows</SubSystem>\r\n");
  fprintf(out, "      <RandomizedBaseAddress>false</RandomizedBaseAddress>\r\n");
  fprintf(out, "      <DataExecutionPrevention>\r\n");
  fprintf(out, "      </DataExecutionPrevention>\r\n");
  fprintf(out, "      <TargetMachine>MachineX86</TargetMachine>\r\n");
  fprintf(out, "    </Link>\r\n");
  fprintf(out, "  </ItemDefinitionGroup>\r\n");
  fprintf(out, "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release Cairo|Win32'\">\r\n");
  fprintf(out, "    <Midl>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>NDEBUG;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <MkTypLibCompatible>true</MkTypLibCompatible>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <TargetEnvironment>Win32</TargetEnvironment>\r\n");
  fprintf(out, "      <TypeLibraryName>.\\fluid___/fluid.tlb</TypeLibraryName>\r\n");
  fprintf(out, "      <HeaderFileName>\r\n");
  fprintf(out, "      </HeaderFileName>\r\n");
  fprintf(out, "    </Midl>\r\n");
  fprintf(out, "    <ClCompile>\r\n");
  fprintf(out, "      <Optimization>Disabled</Optimization>\r\n");
  fprintf(out, "      <InlineFunctionExpansion>AnySuitable</InlineFunctionExpansion>\r\n");
  fprintf(out, "      <FavorSizeOrSpeed>Neither</FavorSizeOrSpeed>\r\n");
  fprintf(out, "      <AdditionalIncludeDirectories>.;..\\..\\zlib;..\\..\\png;..\\..\\jpeg;../..;%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>FLTK_HAVE_CAIRO=1;_CRT_SECURE_NO_DEPRECATE;WIN32;NDEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>\r\n");
  fprintf(out, "      <PrecompiledHeader>\r\n");
  fprintf(out, "      </PrecompiledHeader>\r\n");
  fprintf(out, "      <PrecompiledHeaderOutputFile>.\\fluid___/fluid.pch</PrecompiledHeaderOutputFile>\r\n");
  fprintf(out, "      <AssemblerListingLocation>.\\fluid___/</AssemblerListingLocation>\r\n");
  fprintf(out, "      <ObjectFileName>.\\fluid___/</ObjectFileName>\r\n");
  fprintf(out, "      <ProgramDataBaseFileName>.\\fluid___/</ProgramDataBaseFileName>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <CompileAs>Default</CompileAs>\r\n");
  fprintf(out, "    </ClCompile>\r\n");
  fprintf(out, "    <ResourceCompile>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>NDEBUG;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <Culture>0x0409</Culture>\r\n");
  fprintf(out, "    </ResourceCompile>\r\n");
  fprintf(out, "    <Link>\r\n");
  fprintf(out, "      <AdditionalDependencies>comctl32.lib;cairo.lib;%%(AdditionalDependencies)</AdditionalDependencies>\r\n");
  fprintf(out, "      <OutputFile>../fluid/fluid.exe</OutputFile>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <AdditionalLibraryDirectories>..\\..\\lib;%%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\r\n");
  fprintf(out, "      <IgnoreSpecificDefaultLibraries>libcmt;%%(IgnoreSpecificDefaultLibraries)</IgnoreSpecificDefaultLibraries>\r\n");
  fprintf(out, "      <ProgramDatabaseFile>$(IntDir)$(TargetName).pdb</ProgramDatabaseFile>\r\n");
  fprintf(out, "      <SubSystem>Windows</SubSystem>\r\n");
  fprintf(out, "      <RandomizedBaseAddress>false</RandomizedBaseAddress>\r\n");
  fprintf(out, "      <DataExecutionPrevention>\r\n");
  fprintf(out, "      </DataExecutionPrevention>\r\n");
  fprintf(out, "      <TargetMachine>MachineX86</TargetMachine>\r\n");
  fprintf(out, "    </Link>\r\n");
  fprintf(out, "  </ItemDefinitionGroup>\r\n");
  fprintf(out, "  <ItemGroup>\r\n");
    
  /* loop through the target and write out all C++ files */
  Fl_Type *src = tgt->next;
  while (src && src->level>tgt->level) {
    if (src->is_file()) {
      Fl_File_Type *f = (Fl_File_Type*)src;
      const char *fn = f->filename();
      if (fn) {
        const char *ext = fltk3::filename_ext(fn);
        if (ext && (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0)) {
          char buf[2048], *c;
          strcpy(buf, fn);
          for (c=buf;;) {
            c = strchr(c, '/');
            if (!c) break;
            *c = '\\';
            c++;
          }          
          fprintf(out, "    <ClCompile Include=\"..\\..\\%s\">\r\n", buf);
          fprintf(out, "      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug Cairo|Win32'\">Disabled</Optimization>\r\n");
          fprintf(out, "      <AdditionalIncludeDirectories Condition=\"'$(Configuration)|$(Platform)'=='Debug Cairo|Win32'\">%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
          fprintf(out, "      <PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug Cairo|Win32'\">%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
          fprintf(out, "      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Disabled</Optimization>\r\n");
          fprintf(out, "      <AdditionalIncludeDirectories Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
          fprintf(out, "      <PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
          fprintf(out, "      <FavorSizeOrSpeed Condition=\"'$(Configuration)|$(Platform)'=='Release Cairo|Win32'\">Neither</FavorSizeOrSpeed>\r\n");
          fprintf(out, "      <AdditionalIncludeDirectories Condition=\"'$(Configuration)|$(Platform)'=='Release Cairo|Win32'\">%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
          fprintf(out, "      <PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release Cairo|Win32'\">%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
          fprintf(out, "      <FavorSizeOrSpeed Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">Neither</FavorSizeOrSpeed>\r\n");
          fprintf(out, "      <AdditionalIncludeDirectories Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
          fprintf(out, "      <PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
          fprintf(out, "    </ClCompile>\r\n");
        }
      }
    }
    src = src->next;
  }

  fprintf(out, "  </ItemGroup>\r\n");
  fprintf(out, "  <ItemGroup>\r\n");
  fprintf(out, "    <ProjectReference Include=\"fltk.lib.vcxproj\">\r\n");
  fprintf(out, "      <Project>{e070aafc-9d03-41a3-bc7d-30887ea0d50f}</Project>\r\n");
  fprintf(out, "      <ReferenceOutputAssembly>false</ReferenceOutputAssembly>\r\n");
  fprintf(out, "    </ProjectReference>\r\n");
  fprintf(out, "    <ProjectReference Include=\"fltkimages.vcxproj\">\r\n");
  fprintf(out, "      <Project>{6e8e1663-b88d-4454-adf2-279666a93306}</Project>\r\n");
  fprintf(out, "      <ReferenceOutputAssembly>false</ReferenceOutputAssembly>\r\n");
  fprintf(out, "    </ProjectReference>\r\n");
  fprintf(out, "    <ProjectReference Include=\"jpeg.vcxproj\">\r\n");
  fprintf(out, "      <Project>{08b82852-90b3-4767-a5d2-f0a4fccb2377}</Project>\r\n");
  fprintf(out, "      <ReferenceOutputAssembly>false</ReferenceOutputAssembly>\r\n");
  fprintf(out, "    </ProjectReference>\r\n");
  fprintf(out, "    <ProjectReference Include=\"libpng.vcxproj\">\r\n");
  fprintf(out, "      <Project>{d640a221-f95a-40ff-ac0e-0e8b615c7681}</Project>\r\n");
  fprintf(out, "      <ReferenceOutputAssembly>false</ReferenceOutputAssembly>\r\n");
  fprintf(out, "    </ProjectReference>\r\n");
  fprintf(out, "    <ProjectReference Include=\"zlib.vcxproj\">\r\n");
  fprintf(out, "      <Project>{e1d9ce3f-400d-40e8-ad0d-61c29b1847ff}</Project>\r\n");
  fprintf(out, "      <ReferenceOutputAssembly>false</ReferenceOutputAssembly>\r\n");
  fprintf(out, "    </ProjectReference>\r\n");
  fprintf(out, "  </ItemGroup>\r\n");
  fprintf(out, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\r\n");
  fprintf(out, "  <ImportGroup Label=\"ExtensionTargets\">\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  fprintf(out, "</Project>");  
  
  fclose(out);
}

void write_makefiles_cb(fltk3::Widget*, void*) {
  write_fltk_makefiles();
  write_fltk_ide_xcode4();
  write_fltk_ide_visualc6();
  write_fltk_ide_visualc2008();
  write_fltk_ide_visualc2010();
}


// ------------ file conversion ------------------------------------------------

void convert_file(const char *src_name, int v) {
  char dst_name[2048];
  char line[2048];
  strcpy(dst_name, src_name);
  strcat(dst_name, ".3");
  FILE *f_in = fopen(src_name, "rb");
  FILE *f_out = fopen(dst_name, "wb");
  for (;;) {
    fgets(line, 2047, f_in);
    // replace words according to database
    fputs(line, f_out);
  }
  fclose(f_out);
  fclose(f_in);
}

void convert_1_to_3_cb(fltk3::Widget *, void *) {
  const char *base = fltk3::file_chooser("Convert a file or directory from FLTK 1 to 3", "*", 0L);
  if (!base) 
    return;
  convert_file(base, 1);
}

void convert_2_to_3_cb(fltk3::Widget *, void *) {
}

//
// End of "$Id$".
//
