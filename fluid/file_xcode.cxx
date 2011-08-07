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

/*
 
 typedef enum {
 FL_FILE_DEFAULT = 0x8000,
 FL_FILE_UNKNOWN = 0,
 FL_FILE_C_SOURCE, FL_FILE_C_HEADER,
 FL_FILE_CPP_SOURCE, FL_FILE_CPP_HEADER,
 FL_FILE_OBJC_SOURCE, FL_FILE_OBJC_HEADER,
 FL_FILE_TEXT, FL_FILE_TEXT_SCRIPT
 } FileType;

lastKnownFileType  or  explicitFileType
 sourcecode.c.c
 sourcecode.c.h
 sourcecode.cpp.cpp
 sourcecode.cpp.objcpp
 sourcecode.cpp.h
 sourcecode.fluid (=text)
 text
 text.script.sh
 
 wrapper.framework
 wrapper.application
 "compiled.mach-o.dylib"
 
 */

// ------------ file conversion ------------------------------------------------

extern const char *filename;


static const char *xcode4_type(unsigned int ft) {
  static char buf[64];
  if (ft & FL_FILE_EXPLICIT) {
    strcpy(buf, "explicitFileType = ");
  } else {
    strcpy(buf, "lastKnownFileType = ");
  }
  switch (ft & 0x7fff) {
    case FL_FILE_C_SOURCE:     strcat(buf, "sourcecode.c.c"); break;
    case FL_FILE_C_HEADER:     strcat(buf, "sourcecode.c.h"); break;
    case FL_FILE_CPP_SOURCE:   strcat(buf, "sourcecode.cpp.cpp"); break;
    case FL_FILE_CPP_HEADER:   strcat(buf, "sourcecode.cpp.h"); break;
    case FL_FILE_OBJC_SOURCE:  strcat(buf, "sourcecode.cpp.objcpp"); break;
    case FL_FILE_OBJC_HEADER:  strcat(buf, "sourcecode.cpp.h"); break;
    case FL_FILE_TEXT:         strcat(buf, "text"); break;
    case FL_FILE_TEXT_SCRIPT:  strcat(buf, "text.script.sh"); break;
    default:                strcat(buf, "text"); break;
  }
  return buf;
}


static int writeFileReferences(FILE *out, Fl_Target_Type *tgt) {

  char ProductReference[32]; strcpy(ProductReference, tgt->get_UUID_Xcode("Xcode4_ProductReference"));

  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    // FIXME: write a file type converter!
    if (f->lists_in(FL_ENV_XC4)) {
      char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
      fprintf(out, "\t\t%s /* %s */ = {isa = PBXFileReference; fileEncoding = 4; %s; name = %s; path = ../../%s; sourceTree = SOURCE_ROOT; };\n", 
              PBXFileRef,
              f->filename_name(), 
              xcode4_type(f->filetype()),
              f->filename_name(), 
              f->filename());
    }
  }
  if (tgt->is_lib_target()) {
    fprintf(out, "\t\t%s /* %s.framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = %s.framework; sourceTree = BUILT_PRODUCTS_DIR; };\n", ProductReference, tgt->name(), tgt->name());
  } else if (tgt->is_app_target()) {
    // TODO: B2F3E15BD31ADBA58ECD50C4 /* Fluid.app */ = {isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = Fluid.app; sourceTree = BUILT_PRODUCTS_DIR; };
  }
  // we also need a file reference for the end produkt, for example:
  // B2F3E15BD31ADBA58ECD50C4 /* Fluid.app */ = {isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = Fluid.app; sourceTree = BUILT_PRODUCTS_DIR; };
  // or:
  // FEB0F8FE6383384180570D94 /* fltk.framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = fltk.framework; sourceTree = BUILT_PRODUCTS_DIR; };
  // but be careful: these are referenced very often in the BuildFile section for CopyFiles phases, and in the group "products"
  return 0;
}


static int writeHeadersBuildPhase(FILE *out, Fl_Target_Type *tgt) {
  
  char HeadersBuildPhase[32]; strcpy(HeadersBuildPhase, tgt->get_UUID_Xcode("Xcode4_HeadersBuildPhase"));
  
  fprintf(out, "\t\t%s /* Headers */ = {\n", HeadersBuildPhase);
  fprintf(out, "\t\t\tisa = PBXHeadersBuildPhase;\n");
  fprintf(out, "\t\t\tbuildActionMask = 2147483647;\n");
  fprintf(out, "\t\t\tfiles = (\n");
  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    if (f->builds_in(FL_ENV_XC4) && f->file_is_header()) {
      char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode("Xcode4_PBXBuildFile"));
      fprintf(out, "\t\t\t\t%s /* %s in %s */,\n", 
              PBXBuildFile, 
              f->filename_name(), 
              "Sources");
    }
  }
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\trunOnlyForDeploymentPostprocessing = 0;\n");
  fprintf(out, "\t\t};\n");
  return 0;
}


static int writeSourcesBuildPhase(FILE *out, Fl_Target_Type *tgt) {
  
  char SourcesBuildPhase[32]; strcpy(SourcesBuildPhase, tgt->get_UUID_Xcode("Xcode4_SourcesBuildPhase"));
  
  fprintf(out, "\t\t%s /* Sources */ = {\n", SourcesBuildPhase);
  fprintf(out, "\t\t\tisa = PBXSourcesBuildPhase;\n");
  fprintf(out, "\t\t\tbuildActionMask = 2147483647;\n");
  fprintf(out, "\t\t\tfiles = (\n");
  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    if (f->builds_in(FL_ENV_XC4) && f->file_is_code()) {
      char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode("Xcode4_PBXBuildFile"));
      fprintf(out, "\t\t\t\t%s /* %s in %s */,\n", 
              PBXBuildFile, 
              f->filename_name(), 
              "Sources");
    }
  }
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\trunOnlyForDeploymentPostprocessing = 0;\n");
  fprintf(out, "\t\t};\n");
  return 0;
}


static int writeBuildConfigurations(FILE *out, const char *debugKey, const char *releaseKey, const char *productName) {
  // Write the Debug Build Configuration
  fprintf(out, "\t\t%s /* Debug */ = {\n", debugKey);
  fprintf(out, "\t\t\tisa = XCBuildConfiguration;\n");
  fprintf(out, "\t\t\tbuildSettings = {\n");
  fprintf(out, "\t\t\t\tALWAYS_SEARCH_USER_PATHS = NO;\n");
  fprintf(out, "\t\t\t\tCOPY_PHASE_STRIP = NO;\n");
  fprintf(out, "\t\t\t\tDEPLOYMENT_LOCATION = NO;\n");
  fprintf(out, "\t\t\t\tDEPLOYMENT_POSTPROCESSING = NO;\n");
  fprintf(out, "\t\t\t\tDYLIB_COMPATIBILITY_VERSION = 1;\n");
  fprintf(out, "\t\t\t\tDYLIB_CURRENT_VERSION = 1;\n");
  fprintf(out, "\t\t\t\tFRAMEWORK_VERSION = A;\n");
  fprintf(out, "\t\t\t\tGCC_DYNAMIC_NO_PIC = NO;\n");
  fprintf(out, "\t\t\t\tGCC_MODEL_TUNING = G5;\n");
  fprintf(out, "\t\t\t\tGCC_OPTIMIZATION_LEVEL = 0;\n");
  fprintf(out, "\t\t\t\tGCC_PRECOMPILE_PREFIX_HEADER = YES;\n");
  fprintf(out, "\t\t\t\tGCC_PREFIX_HEADER = fltk.pch;\n");
  fprintf(out, "\t\t\t\tGCC_PREPROCESSOR_DEFINITIONS = FL_LIBRARY;\n");
  fprintf(out, "\t\t\t\tGCC_WARN_ABOUT_DEPRECATED_FUNCTIONS = NO;\n");
  fprintf(out, "\t\t\t\tHEADER_SEARCH_PATHS = (\n");
  fprintf(out, "\t\t\t\t\t../../ide/XCode4/,\n");
  fprintf(out, "\t\t\t\t\t../../,\n");
  fprintf(out, "\t\t\t\t\t../../png,\n");
  fprintf(out, "\t\t\t\t\t../../jpeg,\n");
  fprintf(out, "\t\t\t\t);\n");
  fprintf(out, "\t\t\t\tINFOPLIST_FILE = \"plists/fltk-Info.plist\";\n");
  fprintf(out, "\t\t\t\tINSTALL_PATH = \"@executable_path/../Frameworks\";\n");
  fprintf(out, "\t\t\t\tOTHER_LDFLAGS = \"\";\n");
  fprintf(out, "\t\t\t\tPRODUCT_NAME = \"%s\";\n", productName);
  fprintf(out, "\t\t\t\tSDKROOT = macosx;\n");
  fprintf(out, "\t\t\t\tWARNING_CFLAGS = (\n");
  fprintf(out, "\t\t\t\t\t\"-Wno-format-security\",\n");
  fprintf(out, "\t\t\t\t\t\"-Wall\",\n");
  fprintf(out, "\t\t\t\t);\n");
  fprintf(out, "\t\t\t};\n");
  fprintf(out, "\t\t\tname = Debug;\n");
  fprintf(out, "\t\t};\n");
  
  // Write the Release Build Configuration
  fprintf(out, "\t\t%s /* Release */ = {\n", releaseKey);
  fprintf(out, "\t\t\tisa = XCBuildConfiguration;\n");
  fprintf(out, "\t\t\tbuildSettings = {\n");
  fprintf(out, "\t\t\t\tALWAYS_SEARCH_USER_PATHS = NO;\n");
  fprintf(out, "\t\t\t\tCOPY_PHASE_STRIP = YES;\n");
  fprintf(out, "\t\t\t\tDEBUG_INFORMATION_FORMAT = \"dwarf-with-dsym\";\n");
  fprintf(out, "\t\t\t\tDEPLOYMENT_LOCATION = NO;\n");
  fprintf(out, "\t\t\t\tDEPLOYMENT_POSTPROCESSING = NO;\n");
  fprintf(out, "\t\t\t\tDYLIB_COMPATIBILITY_VERSION = 1;\n");
  fprintf(out, "\t\t\t\tDYLIB_CURRENT_VERSION = 1;\n");
  fprintf(out, "\t\t\t\tFRAMEWORK_VERSION = A;\n");
  fprintf(out, "\t\t\t\tGCC_MODEL_TUNING = G5;\n");
  fprintf(out, "\t\t\t\tGCC_PRECOMPILE_PREFIX_HEADER = YES;\n");
  fprintf(out, "\t\t\t\tGCC_PREFIX_HEADER = fltk.pch;\n");
  fprintf(out, "\t\t\t\tGCC_PREPROCESSOR_DEFINITIONS = FL_LIBRARY;\n");
  fprintf(out, "\t\t\t\tGCC_WARN_ABOUT_DEPRECATED_FUNCTIONS = NO;\n");
  fprintf(out, "\t\t\t\tHEADER_SEARCH_PATHS = (\n");
  fprintf(out, "\t\t\t\t\t../../ide/XCode4/,\n");
  fprintf(out, "\t\t\t\t\t../../,\n");
  fprintf(out, "\t\t\t\t\t../../png,\n");
  fprintf(out, "\t\t\t\t\t../../jpeg,\n");
  fprintf(out, "\t\t\t\t);\n");
  fprintf(out, "\t\t\t\tINFOPLIST_FILE = \"plists/fltk-Info.plist\";\n");
  fprintf(out, "\t\t\t\tINSTALL_PATH = \"@executable_path/../Frameworks\";\n");
  fprintf(out, "\t\t\t\tOTHER_LDFLAGS = \"\";\n");
  fprintf(out, "\t\t\t\tPRODUCT_NAME = \"%s\";\n", productName);
  fprintf(out, "\t\t\t\tSDKROOT = macosx;\n");
  fprintf(out, "\t\t\t\tWARNING_CFLAGS = (\n");
  fprintf(out, "\t\t\t\t\t\"-Wno-format-security\",\n");
  fprintf(out, "\t\t\t\t\t\"-Wall\",\n");
  fprintf(out, "\t\t\t\t);\n");
  fprintf(out, "\t\t\t\tZERO_LINK = NO;\n");
  fprintf(out, "\t\t\t};\n");
  fprintf(out, "\t\t\tname = Release;\n");
  fprintf(out, "\t\t};\n");

  return 0;
}


static int writeBuildConfigurationList(FILE *out, const char *listKey, const char *debugKey, const char *releaseKey, const char *productName) {
  fprintf(out, "\t\t%s /* Build configuration list for PBXNativeTarget \"%s\" */ = {\n", listKey, productName);
  fprintf(out, "\t\t\tisa = XCConfigurationList;\n");
  fprintf(out, "\t\t\tbuildConfigurations = (\n");
  fprintf(out, "\t\t\t\t%s /* Debug */,\n", debugKey);
  fprintf(out, "\t\t\t\t%s /* Release */,\n", releaseKey);
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\tdefaultConfigurationIsVisible = 0;\n");
  fprintf(out, "\t\t\tdefaultConfigurationName = Debug;\n");
  fprintf(out, "\t\t};\n");
  return 0;
}


static int writeResourcesBuildPhase(FILE *out, const char *key, const char *productName) {
  fprintf(out, "\t\t%s /* Resources */ = {\n", key);
  fprintf(out, "\t\t\tisa = PBXResourcesBuildPhase;\n");
  fprintf(out, "\t\t\tbuildActionMask = 2147483647;\n");
  fprintf(out, "\t\t\tfiles = (\n");
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\trunOnlyForDeploymentPostprocessing = 0;\n");
  fprintf(out, "\t\t};\n");
  return 0;
}


static int writeNativeTarget(FILE *out, Fl_Target_Type *tgt) {
  // currently we still have a bunch of fixed UUIDs in here!

  char Target[32]; strcpy(Target, tgt->get_UUID_Xcode("Xcode4_Target"));
  char buildConfigurationList[32]; strcpy(buildConfigurationList, tgt->get_UUID_Xcode("Xcode4_BuildConfigurationList"));
  char ResourcesBuildPhase[32]; strcpy(ResourcesBuildPhase, tgt->get_UUID_Xcode("Xcode4_ResourcesBuildPhase"));
  char HeadersBuildPhase[32]; strcpy(HeadersBuildPhase, tgt->get_UUID_Xcode("Xcode4_HeadersBuildPhase"));
  char SourcesBuildPhase[32]; strcpy(SourcesBuildPhase, tgt->get_UUID_Xcode("Xcode4_SourcesBuildPhase"));
  char FluidBuildRule[32]; strcpy(FluidBuildRule, tgt->get_UUID_Xcode("Xcode4_FluidBuildRule"));
  char ProductReference[32]; strcpy(ProductReference, tgt->get_UUID_Xcode("Xcode4_ProductReference"));

  fprintf(out, "\t\t%s /* %s */ = {\n",Target ,tgt->name());     // FIXME: use generated key
  fprintf(out, "\t\t\tisa = PBXNativeTarget;\n");
  fprintf(out, "\t\t\tbuildConfigurationList = %s /* Build configuration list for PBXNativeTarget \"%s\" */;", buildConfigurationList, tgt->name());
  fprintf(out, "\t\t\tbuildPhases = (\n");
  fprintf(out, "\t\t\t\t%s /* Resources */,\n", ResourcesBuildPhase);
  fprintf(out, "\t\t\t\t%s /* Headers */,\n", HeadersBuildPhase);
  fprintf(out, "\t\t\t\t%s /* Sources */,\n", SourcesBuildPhase);
  fprintf(out, "\t\t\t\tD2A1AD2D93B0EED43F624520 /* Frameworks */,\n");         // FIXME: use generated key
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\tbuildRules = (\n");
  fprintf(out, "\t\t\t\t%s /* PBXBuildRule */,\n", FluidBuildRule);
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\tdependencies = (\n");
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\tname = %s;\n", tgt->name());
  fprintf(out, "\t\t\tproductName = %s;\n", tgt->name());
  fprintf(out, "\t\t\tproductReference = %s /* %s.framework */;\n", ProductReference, tgt->name()); // FIXME: .framework or .app
  fprintf(out, "\t\t\tproductType = \"com.apple.product-type.framework\";\n");
  fprintf(out, "\t\t};\n");
  
  return 0;
}


static int writeFluidBuildRule(FILE *out, Fl_Target_Type *tgt) {
  
  char FluidBuildRule[32]; strcpy(FluidBuildRule, tgt->get_UUID_Xcode("Xcode4_FluidBuildRule"));
  
  fprintf(out, "\t\t%s /* PBXBuildRule */ = {\n", FluidBuildRule);
  fprintf(out, "\t\t\tisa = PBXBuildRule;\n");
  fprintf(out, "\t\t\tcompilerSpec = com.apple.compilers.proxy.script;\n");
  fprintf(out, "\t\t\tfilePatterns = \"*.fl\";\n");
  fprintf(out, "\t\t\tfileType = pattern.proxy;\n");
  fprintf(out, "\t\t\tisEditable = 1;\n");
  fprintf(out, "\t\t\toutputFiles = (\n");
  fprintf(out, "\t\t\t\t\"${INPUT_FILE_DIR}/${INPUT_FILE_BASE}.cxx\",\n");
  fprintf(out, "\t\t\t\t\"${INPUT_FILE_DIR}/${INPUT_FILE_BASE}.h\",\n");
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\tscript = \"export DYLD_FRAMEWORK_PATH=${TARGET_BUILD_DIR} && cd ${INPUT_FILE_DIR} && ${TARGET_BUILD_DIR}/Fluid.app/Contents/MacOS/Fluid -c ${INPUT_FILE_NAME}\";\n");
  fprintf(out, "\t\t};\n");

  return 0;
}


static int writeProjectTarget(FILE *out, Fl_Target_Type *tgt) {
  char Target[32]; strcpy(Target, tgt->get_UUID_Xcode("Xcode4_Target"));
  fprintf(out, "\t\t\t\t%s /* %s */,\n", Target, tgt->name());
  return 0;
}

int write_fltk_ide_xcode4() {
  // for now, we use a template file in FLTK/ide/templates/VisualC2008.tmpl .
  // When done, everything will likely be integrated into the executable to make one compact package.
  char buf[2048], base_dir[2048], tgt_base[2048];
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  strcpy(tgt_base, base_dir);
  strcpy(buf, base_dir);
  strcat(buf, "ide/templates/Xcode4.tmpl");
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
          // mark the end of the filename (this will crash if the formatting is wrong!)
          char *sep = strchr(hash, ')');
          *sep = 0;
          // filename is relative, so add it to the base_dir
          char fnbuf[2048];
          strcpy(fnbuf, base_dir);
          strcat(fnbuf, hash+11);
          out = fopen(fnbuf, "wb");
          // set the filepath for this target. In this module, all filenames are relative to the Makefile
          strcpy(tgt_base, fnbuf);
          *((char*)fltk3::filename_name(tgt_base)) = 0; // keep only the path
                                                        // restore buffer and continue 
          *sep = ')';
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#CloseFile", 10)==0) {
          if (out!=stdout) fclose(out);
          out = stdout;
          // set the filepath for the default target. 
          strcpy(tgt_base, base_dir);
          hash = strchr(hash, ';')+1;
      } else if (strncmp(hash, "#BuildFileReferences(", 21)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+21, ')'); // keep tgt local
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->builds_in(FL_ENV_XC4)) {
              char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode("Xcode4_PBXBuildFile"));
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
              fprintf(out, "\t\t%s /* %s in %s */ = {isa = PBXBuildFile; fileRef = %s /* %s */; };\n", 
                      PBXBuildFile, 
                      f->filename_name(), 
                      "Sources",  // FIXME: use group name!
                      PBXFileRef, 
                      f->filename_name());
            }
          }
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#FileReferences(", 16)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+16, ')');
          writeFileReferences(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#HeadersGroup(", 14)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+14, ')');
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->lists_in(FL_ENV_XC4) && f->file_is_header()) {
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
              fprintf(out, "\t\t\t\t%s /* %s */,\n", 
                      PBXFileRef, 
                      f->filename_name());
            }
          }
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#SourcesGroup(", 14)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+14, ')');
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->lists_in(FL_ENV_XC4) && f->file_is_code()) {
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode("Xcode4_PBXFileRef"));
              fprintf(out, "\t\t\t\t%s /* %s */,\n", 
                      PBXFileRef, 
                      f->filename_name());
            }
          }
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#HeadersBuildPhase(", 19)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+19, ')');
          writeHeadersBuildPhase(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#SourcesBuildPhase(", 19)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+19, ')');
          writeSourcesBuildPhase(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#OldSourcesBuildPhase(", 22)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+22, ')');
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->builds_in(FL_ENV_XC4) && f->file_is_code()) {
              char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode("Xcode4_PBXBuildFile"));
              fprintf(out, "\t\t\t\t%s /* %s in %s */,\n", 
                      PBXBuildFile, 
                      f->filename_name(), 
                      "Sources");
            }
          }
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#BuildConfigurations(", 21)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+21, ')');
          char debugKey[32]; strcpy(debugKey, tgt->get_UUID_Xcode("Xcode4_DebugBuildConfiguration"));
          char releaseKey[32]; strcpy(releaseKey, tgt->get_UUID_Xcode("Xcode4_ReleaseBuildConfiguration"));
          writeBuildConfigurations(out, debugKey, releaseKey, tgt->name());
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#BuildConfigurationList(", 24)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+24, ')');
          char listKey[32]; strcpy(listKey, tgt->get_UUID_Xcode("Xcode4_BuildConfigurationList"));
          char debugKey[32]; strcpy(debugKey, tgt->get_UUID_Xcode("Xcode4_DebugBuildConfiguration"));
          char releaseKey[32]; strcpy(releaseKey, tgt->get_UUID_Xcode("Xcode4_ReleaseBuildConfiguration"));
          writeBuildConfigurationList(out, listKey, debugKey, releaseKey, tgt->name());
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#ResourcesBuildPhase(", 21)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+21, ')');
          char key[32]; strcpy(key, tgt->get_UUID_Xcode("Xcode4_ResourcesBuildPhase"));
          writeResourcesBuildPhase(out, key, tgt->name());
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#NativeTarget(", 14)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+14, ')');
          writeNativeTarget(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#FluidBuildRule(", 16)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+16, ')');
          writeFluidBuildRule(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#ProjectTarget(", 15)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+15, ')');
          writeProjectTarget(out, tgt);
          hash = strchr(hash, ';')+1;
        } else {
#if 0
          fltk.framework starts here: A57FDE871C99A52BEEDEE68C
          
          rootObject = 4BF1A7FFEACF5F31B4127482, contains fltk as a target
            fltk (NativeTarget) = A57FDE871C99A52BEEDEE68C
              -buildConfigurationList (ConfigurationList) = 3BD5FFB6FCC8F21A23C23DF4 /* Build configuration list for PBXNativeTarget "fltk" */;
                -Debug (BuildConfiguration) = 78446623B2E9921ED6B05986
                -Release (BuildConfiguration) = BFEB622BA8B40E851AF0E91F
              -Resources build phase (ResourcesBuildPhase) = 6715D162BEFF87372B2A31E0 (can reference files (BuildFile))
              -Headers build phase (HeadersBuildPhas) = C9EDD5C81274C6BA00ADB21C (can reference files (BuildFile))
              -CopyFilesBuildPhase = C9EDD42D1274B84100ADB21C /* CopyFiles */, (copying header files around that shouldn`t)
              -CopyFilesBuildPhase = C9EDD4DD1274BB4100ADB21C /* CopyFiles */, (copying header files around that shouldn`t)
              -SourcesBuildPhase = 4DA82C38AA0403E56A1E3545 /* Sources */,
              FrameworksBuildPhase = D2A1AD2D93B0EED43F624520
                Cocoa.framework = C96290C21274D0CF007D3CFE (BuildFile -> FileReference)
              -buildRules (BuildRule) = EFFAAB905A54B0BFE13CB56C (Fluid build rule, no more references)
              -productReference = FEB0F8FE6383384180570D94 (-> FileReference, is also referenced in "Products")
          
          Also still missing are dependencies, like this one: A8AB7DEC3970D5A693D2BC84 and dependency proxies (uuuaaaahh!)
#endif
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
