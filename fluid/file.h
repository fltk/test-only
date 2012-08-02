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

#ifndef FLUID_FILE_H
#define FLUID_FILE_H

#  ifdef __GNUC__
#    define __fl_attr(x) __attribute__ (x)
#  else
#    define __fl_attr(x)
#  endif // __GNUC__


namespace fltk3 {
  class Widget;
};


extern double read_version;
extern double write_version;

int open_write(const char *s);
int close_write();
void write_word(const char *w);
void write_string(const char *,...) __fl_attr((__format__ (__printf__, 1, 2)));
void write_indent(int n);
void write_open(int);
void write_close(int n);
void write_public(int state);

int open_read(const char *s);
int close_read();
void read_error(const char *format, ...);
const char *read_word(int wantbrace=0);

int write_file(const char *filename, int selected_only=0);
int read_file(const char *filename, int merge);


#endif

//
// End of "$Id$".
//
