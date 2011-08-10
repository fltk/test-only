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
#include "workspace_panel.h"
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
