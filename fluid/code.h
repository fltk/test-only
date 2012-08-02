//
// "$Id$"
//
// Code output routines for the Fast Light Tool Kit (FLTK).
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

#ifndef FLUID3_CODE_H
#define FLUID3_CODE_H

#  ifdef __GNUC__
#    define __fl_attr(x) __attribute__ (x)
#  else
#    define __fl_attr(x)
#  endif // __GNUC__


extern int indentation;
extern int write_number;
extern int write_sourceview;
extern int varused_test;
extern int varused;

int is_id(char c);
const char* unique_id(void* o, const char* type, const char* name, const char* label);
const char* indent();
int write_declare(const char *format, ...);
void write_cstring(const char *w, int length);
void write_cstring(const char *w);
void write_cdata(const char *s, int length);
void write_c(const char*, ...) __fl_attr((__format__ (__printf__, 1, 2)));
void write_h(const char*, ...) __fl_attr((__format__ (__printf__, 1, 2)));
int write_code(const char *s, const char *t);
int write_strings(const char *sfile);


#endif

//
// End of "$Id$".
//
