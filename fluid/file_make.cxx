//
// "$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $"
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
#include <stdarg.h>
#include "Fl_Type.h"
#include "../fltk3/filename.h"

// ------------ file conversion ------------------------------------------------

extern char *filename;

int write_fltk_makefiles() {
  // for now, we use a template file in FLTK/ide/templates/Makefile.tmpl .
  // When done, everything will likely be integrated into the executable to make one compact package.
  
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "ide/templates/Makefile.tmpl");
  FILE *out = stdout;
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
        if (strncmp(hash, "#WriteFile(",11)==0) {
          char fnbuf[2048];
          char *sep = strchr(hash, ')');
          *sep = 0;
          strcpy(fnbuf, filename);
          strcpy((char*)fltk3::filename_name(fnbuf), hash+11);
          *sep = ')';
          out = fopen(fnbuf, "wb");
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#CloseFile", 10)==0) {
          if (out!=stdout) fclose(out);
          out = stdout;
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#CppFiles(", 10)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+10, ')'); // keep tgt local
          if (!tgt) {
            printf("ERROR writing Makefile: target not found!");
            return -1;
          }
          char first = 1;
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->is_cplusplus_code() && f->builds_in(ENV_MAKE)) {
              if (first) { fprintf(out, "CPPFILES = "); first=0; }
              fprintf(out, " \\\n\t%s", f->filename_name());
            }
          }
          if (!first) fprintf(out, "\n");
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#ObjCppFiles(", 13)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+13, ')'); // keep tgt local
          if (!tgt) {
            printf("ERROR writing Makefile: target not found!");
            return -1;
          }
          char first = 1;
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->is_objc_code() && f->builds_in(ENV_MAKE)) {
              if (first) { fprintf(out, "OBJCPPFILES = "); first=0; }
              fprintf(out, " \\\n\t%s", f->filename_name());
            }
          }
          if (!first) fprintf(out, "\n");
          hash = strchr(hash, ';')+1;
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
  if (out!=stdout) fclose(out);
  
  return 0;
}



//
// End of "$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $".
//
