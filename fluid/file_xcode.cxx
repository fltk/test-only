//
// "$Id: file_xcode.cxx 8870 2011-07-26 21:19:35Z matt $"
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


int write_fltk_ide_xcode4() {
  // for now, we use a template project file. Eventually, the entire file will be generated
  //    template is ide/Xcode4/FLTK.xcodeproj/project.pbxproj.tmpl
  // destination is ide/Xcode4/FLTK.xcodeproj/project.pbxproj

  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Target_Type::find("Fluid");
  if (!tgt) {
    printf("FLUID target not found\n");
    return -1;
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
        if (strncmp(hash, "#FluidBuildFileReferences;", 26)==0) {
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->is_cplusplus_code()) {
              char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode("Xcode4_PBXBuildFile"));
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
              fprintf(out, "\t\t%s /* %s in %s */ = {isa = PBXBuildFile; fileRef = %s /* %s */; };\n", 
                      PBXBuildFile, 
                      f->filename_name(), 
                      "Sources", 
                      PBXFileRef, 
                      f->filename_name());
            }
          }
          hash += 26;
        } else if (strncmp(hash, "#FluidFileReferences;", 21)==0) {
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->is_cplusplus_code()) {
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
              fprintf(out, "\t\t%s /* %s */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.cpp.cpp; name = %s; path = ../../%s; sourceTree = SOURCE_ROOT; };\n", 
                      PBXFileRef,
                      f->filename_name(), 
                      f->filename_name(), 
                      f->filename());
            } else if (f->is_cplusplus_header()) {
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
              fprintf(out, "\t\t%s /* %s */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; name = %s; path = ../../%s; sourceTree = SOURCE_ROOT; };\n", 
                      PBXFileRef,
                      f->filename_name(), 
                      f->filename_name(), 
                      f->filename());
            }
          }
          hash += 21;
        } else if (strncmp(hash, "#FluidHeadersGroup;", 19)==0) {
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->is_cplusplus_header()) {
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
              fprintf(out, "\t\t\t\t%s /* %s */,\n", 
                      PBXFileRef, 
                      f->filename_name());
            }
          }
          hash += 19;
        } else if (strncmp(hash, "#FluidSourcesGroup;", 19)==0) {
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->is_cplusplus_code()) {
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
              fprintf(out, "\t\t\t\t%s /* %s */,\n", 
                      PBXFileRef, 
                      f->filename_name());
            }
          }
          hash += 19;
        } else if (strncmp(hash, "#FluidSourcesBuildPhase;", 23)==0) {
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->is_cplusplus_code()) {
              char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode("Xcode4_PBXBuildFile"));
              fprintf(out, "\t\t\t\t%s /* %s in %s */,\n", 
                      PBXBuildFile, 
                      f->filename_name(), 
                      "Sources");
            }
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
  
  return 0;
}


//
// End of "$Id: file_xcode.cxx 8870 2011-07-26 21:19:35Z matt $".
//
