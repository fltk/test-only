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


static const char *Xcode4_BuildFileInSources = "Xcode4_BuildFileInSources";
static const char *Xcode4_BuildFileInHeaders = "Xcode4_BuildFileInHeaders";
static const char *Xcode4_BuildFileInFrameworks = "Xcode4_BuildFileInFrameworks";
static const char *Xcode4_FileRef = "Xcode4_FileRef";
static const char *Xcode4_FluidBuildRule = "Xcode4_FluidBuildRule";
static const char *Xcode4_ContainerItemProxy = "Xcode4_ContainerItemProxy";
static const char *Xcode4_Target = "Xcode4_Target";
static const char *Xcode4_Root = "Xcode4_Root";
static const char *Xcode4_TargetDependency = "Xcode4_TargetDependency";
static const char *Xcode4_DebugBuildConfiguration = "Xcode4_DebugBuildConfiguration";
static const char *Xcode4_ReleaseBuildConfiguration = "Xcode4_ReleaseBuildConfiguration";
static const char *Xcode4_BuildConfigurationList = "Xcode4_BuildConfigurationList";
static const char *Xcode4_ResourcesBuildPhase = "Xcode4_ResourcesBuildPhase";
static const char *Xcode4_HeadersBuildPhase = "Xcode4_HeadersBuildPhase";
static const char *Xcode4_SourcesBuildPhase = "Xcode4_SourcesBuildPhase";
static const char *Xcode4_FrameworksBuildPhase = "Xcode4_FrameworksBuildPhase";
static const char *Xcode4_ProductFileReference = "Xcode4_ProductFileReference";
static const char *Xcode4_Group = "Xcode4_Group";
static const char *Xcode4_CopyFilesBuildPhase = "Xcode4_CopyFilesBuildPhase";
static const char *Xcode4_BuildFileInCopyFiles = "Xcode4_BuildFileInCopyFiles";
static const char *Xcode4_ProductsGroup = "Xcode4_ProductsGroup";

static Fl_Workspace_Type *workspace;

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
    case FL_FILE_FRAMEWORK:    strcat(buf, "wrapper.framework"); break;
    case FL_FILE_FLUID_UI:     strcat(buf, "text"); break;
    default:                   strcat(buf, "text"); break;
  }
  return buf;
}


static const char *xcode4_location(unsigned int fl) {
  switch (fl) {
    case FL_LOCATION_WORKSPACE: return "SOURCE_ROOT";
    case FL_LOCATION_IDE:       return "DEVELOPER_DIR";
    case FL_LOCATION_SDK:       return "SDKROOT";
    case FL_LOCATION_ABSOLUTE:  return "<absolute>";
  }
  return "";
}


// ------------ file structure -------------------------------------------------


#if 0
{
  archiveVersion = 1;
  classes = {
  };
  objectVersion = 44;
  objects = {    
    /* Begin PBXBuildFile section */
    /* Begin PBXBuildRule section */
    /* Begin PBXContainerItemProxy section */
    /* Begin PBXCopyFilesBuildPhase section */
    /* Begin PBXFileReference section */
    /* Begin PBXFrameworksBuildPhase section */
    /* Begin PBXGroup section */
    /* Begin PBXHeadersBuildPhase section */
    /* Begin PBXNativeTarget section */
    /* Begin PBXProject section */
    /* Begin PBXResourcesBuildPhase section */
    /* Begin PBXSourcesBuildPhase section */
    /* Begin PBXTargetDependency section */
    /* Begin XCBuildConfiguration section */
    /* Begin XCConfigurationList section */
  };
  rootObject = 4BF1A7FFEACF5F31B4127482 /* Project object */;
}

#endif


// ------------ file writers ---------------------------------------------------


/* Begin PBXBuildFile section */

// This section contains a list of every file that is required to build the 
// Workspace.
static int writeBuildFileReferences(FILE *out, Fl_Target_Type *tgt) {
  
  Fl_File_Type *file;
  Fl_Target_Dependency_Type *dep;
  
  // --- in Sources
  for (file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->file_is_code() || file->file_is_fluid_ui()) {
      char BuildFileInSource[32]; strcpy(BuildFileInSource, file->get_UUID_Xcode(Xcode4_BuildFileInSources));
      char FileRef[32]; strcpy(FileRef, file->get_UUID_Xcode(Xcode4_FileRef));
      fprintf(out, "\t\t%s /* %s in Sources */ = {isa = PBXBuildFile; fileRef = %s /* %s */; };\n", 
              BuildFileInSource, 
              file->filename_name(), 
              FileRef, 
              file->filename_name());
    }
  }

  // --- in Headers
  for (file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->file_is_header()) {
      char BuildFileInHeaders[32]; strcpy(BuildFileInHeaders, file->get_UUID_Xcode(Xcode4_BuildFileInHeaders));
      char FileRef[32]; strcpy(FileRef, file->get_UUID_Xcode(Xcode4_FileRef));
      fprintf(out, "\t\t%s /* %s in Headers */ = {isa = PBXBuildFile; fileRef = %s /* %s */; };\n", 
              BuildFileInHeaders,
              file->filename_name(), 
              FileRef, 
              file->filename_name());
    }
  }

  // --- in Frameworks
  for (file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->file_is_framework()) {
      char BuildFileInFrameworks[32]; strcpy(BuildFileInFrameworks, file->get_UUID_Xcode(Xcode4_BuildFileInFrameworks));
      char FileRef[32]; strcpy(FileRef, file->get_UUID_Xcode(Xcode4_FileRef));
      fprintf(out, "\t\t%s /* %s in Frameworks */ = {isa = PBXBuildFile; fileRef = %s /* %s */; };\n", 
              BuildFileInFrameworks,
              file->filename_name(), 
              FileRef, 
              file->filename_name());
    }
  }
  for (dep = Fl_Target_Dependency_Type::first_dependency(tgt); dep; dep = dep->next_dependency(tgt)) {
    Fl_Target_Type *file = Fl_Target_Type::find(dep->name());
    if (file && file->is_lib_target()) {
      char BuildFileInFrameworks[32]; strcpy(BuildFileInFrameworks, dep->get_UUID_Xcode(Xcode4_BuildFileInFrameworks));
      char FileRef[32]; strcpy(FileRef, file->get_UUID_Xcode(Xcode4_ProductFileReference));
      fprintf(out, "\t\t%s /* %s.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = %s /* %s.framework */; };\n", 
              BuildFileInFrameworks,
              file->name(), 
              FileRef, 
              file->name());
    } else {
      printf("Unresoved target dependency from \"%s\" to \"%s\"\n", tgt->name(), dep->name());
    }
  }
  
  // --- in Resources
  // TODO: not yet implemented
  
  // --- in CopyFiles
  for (dep = Fl_Target_Dependency_Type::first_dependency(tgt); dep; dep = dep->next_dependency(tgt)) {
    Fl_Target_Type *file = Fl_Target_Type::find(dep->name());
    if (file && file->is_lib_target()) {
      char BuildFileInCopyFiles[32]; strcpy(BuildFileInCopyFiles, dep->get_UUID_Xcode(Xcode4_BuildFileInCopyFiles));
      char FileRef[32]; strcpy(FileRef, file->get_UUID_Xcode(Xcode4_ProductFileReference));
      fprintf(out, "\t\t%s /* %s.framework in CopyFiles */ = {isa = PBXBuildFile; fileRef = %s /* %s.framework */; };\n", 
              BuildFileInCopyFiles,
              file->name(), 
              FileRef, 
              file->name());
    } else {
      printf("Unresoved target dependency from \"%s\" to \"%s\"\n", tgt->name(), dep->name());
    }
  }
  
  return 0;
}

static int writeBuildFileSection(FILE *out) {
  fprintf(out, "\n/* Begin PBXBuildFile section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeBuildFileReferences(out, tgt);
  }
  fprintf(out, "/* End PBXBuildFile section */\n");
  return 0;
}


/* Begin PBXBuildRule section */

// Create new custom build rules. We currently support .fl files with Fluid.
static int writeFluidBuildRule(FILE *out, Fl_Target_Type *tgt) {
  
  char FluidBuildRule[32]; strcpy(FluidBuildRule, tgt->get_UUID_Xcode(Xcode4_FluidBuildRule));
  
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

static int writeBuildRuleSection(FILE *out) {
  fprintf(out, "\n/* Begin PBXBuildRule section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeFluidBuildRule(out, tgt);
  }
  fprintf(out, "/* End PBXBuildRule section */\n");
  return 0;
}


/* Begin PBXContainerItemProxy section */

static int writeContainerItemProxy(FILE *out, Fl_Target_Type *tgt) {
  Fl_Target_Dependency_Type *tgt_dep = Fl_Target_Dependency_Type::first_dependency(tgt);
  for ( ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
    Fl_Target_Type *dep = Fl_Target_Type::find(tgt_dep->name());
    if (dep) {
      Fl_Workspace_Type *wsp = (Fl_Workspace_Type*)Fl_Type::first;
      
      char Root[32]; strcpy(Root, wsp->get_UUID_Xcode(Xcode4_Root));
      char ContainerItemProxy[32]; strcpy(ContainerItemProxy, tgt_dep->get_UUID_Xcode(Xcode4_ContainerItemProxy));
      char DepTarget[32]; strcpy(DepTarget, dep->get_UUID_Xcode(Xcode4_Target));
      
      fprintf(out, "\t\t%s /* PBXContainerItemProxy */ = {\n", ContainerItemProxy);
      fprintf(out, "\t\t\tisa = PBXContainerItemProxy;\n");
      fprintf(out, "\t\t\tcontainerPortal = %s /* Project object */;\n", Root);
      fprintf(out, "\t\t\tproxyType = 1;\n");
      fprintf(out, "\t\t\tremoteGlobalIDString = %s;\n", DepTarget);
      fprintf(out, "\t\t\tremoteInfo = %s;\n", dep->name());
      fprintf(out, "\t\t};\n");
      
    } else {
      printf("Unresoved target dependency to \"%s\"\n", tgt_dep->name());
    }
  }
  return 0;
}

static int writeContainerItemProxySection(FILE *out) {
  fprintf(out, "\n/* Begin PBXContainerItemProxy section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeContainerItemProxy(out, tgt);
  }
  fprintf(out, "/* End PBXContainerItemProxy section */\n");
  return 0;
}


/* Begin PBXCopyFilesBuildPhase section */

// This section lists all kinds or resourced that will be copied into the 
// application bundles. Usually, we want to copy the fltk frameworks into the
// bundle so that users do not need to install FLTK to use and application.
static int writeCopyFilesBuildPhase(FILE *out, Fl_Target_Type *tgt) {
  
  // get the required keys
  char CopyFilesBuildPhase[32]; strcpy(CopyFilesBuildPhase, tgt->get_UUID_Xcode(Xcode4_CopyFilesBuildPhase));
  
  // write a list of all source files
  fprintf(out, "\t\t%s /* CopyFiles */ = {\n", CopyFilesBuildPhase);
  fprintf(out, "\t\t\tisa = PBXCopyFilesBuildPhase;\n");
  fprintf(out, "\t\t\tbuildActionMask = 2147483647;\n");
  fprintf(out, "\t\t\tdstPath = \"\";\n");
  fprintf(out, "\t\t\tdstSubfolderSpec = 10;\n");
  fprintf(out, "\t\t\tfiles = (\n");
  
  Fl_Target_Dependency_Type *tgt_dep = Fl_Target_Dependency_Type::first_dependency(tgt);
  for ( ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
    Fl_Target_Type *dep = Fl_Target_Type::find(tgt_dep->name());
    if (dep && dep->is_lib_target()) {
      char PBXBuildFile[32]; strcpy(PBXBuildFile, tgt_dep->get_UUID_Xcode(Xcode4_BuildFileInCopyFiles));
      fprintf(out, "\t\t\t\t%s /* %s in CopyFiles */,\n", 
              PBXBuildFile, 
              dep->name());
    } else {
      printf("Unresoved target dependency to \"%s\"\n", tgt_dep->name());
    }
  }
  
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\trunOnlyForDeploymentPostprocessing = 0;\n");
  fprintf(out, "\t\t};\n");

  return 0;
}

static int writeCopyFilesBuildPhaseSection(FILE *out) {
  fprintf(out, "\n/* Begin PBXCopyFilesBuildPhase section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_app_target())
      writeCopyFilesBuildPhase(out, tgt);
  }
  fprintf(out, "/* End PBXCopyFilesBuildPhasey section */\n");
  return 0;
}


/* Begin PBXFileReference section */

// This section describes every file that is referenced or generated.
static int writeFileReferences(FILE *out, Fl_Target_Type *tgt) {
  
  char ProductReference[32]; strcpy(ProductReference, tgt->get_UUID_Xcode(Xcode4_ProductFileReference));
  
  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    if (f->lists_in(FL_ENV_XC4)) {
      char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode(Xcode4_FileRef));
      fprintf(out, "\t\t%s /* %s */ = {isa = PBXFileReference; fileEncoding = 4; %s; name = %s; path = %s%s; sourceTree = %s; };\n", 
              PBXFileRef,
              f->filename_name(), 
              xcode4_type(f->filetype()),
              f->filename_name(), 
              f->location()==FL_LOCATION_WORKSPACE ? "../../" : "",
              f->filename(), // FIXME: calculate the correct path!
              xcode4_location(f->location()));
    }
  }
  if (tgt->is_lib_target()) {
    fprintf(out, "\t\t%s /* %s.framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = %s.framework; sourceTree = BUILT_PRODUCTS_DIR; };\n", ProductReference, tgt->name(), tgt->name());
  } else if (tgt->is_app_target()) {
    fprintf(out, "\t\t%s /* %s.app */ = {isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = %s.app; sourceTree = BUILT_PRODUCTS_DIR; };\n", ProductReference, tgt->name(), tgt->name());
  }
  // we also need a file reference for the end produkt, for example:
  // B2F3E15BD31ADBA58ECD50C4 /* Fluid.app */ = {isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = Fluid.app; sourceTree = BUILT_PRODUCTS_DIR; };
  // or:
  // FEB0F8FE6383384180570D94 /* fltk.framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = fltk.framework; sourceTree = BUILT_PRODUCTS_DIR; };
  // but be careful: these are referenced very often in the BuildFile section for CopyFiles phases, and in the group "products"
  
  // C9E17DB813EF55140066CA61 /* Cocoa.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = Cocoa.framework; path = SDKs/MacOSX10.7.sdk/System/Library/Frameworks/Cocoa.framework; sourceTree = DEVELOPER_DIR; };
  // C9E17DB913EF55140066CA61 /* Cocoa.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = C9E17DB813EF55140066CA61 /* Cocoa.framework */; };
  return 0;
}

static int writeFileReferenceSection(FILE *out) {
  fprintf(out, "\n/* Begin PBXFileReference section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeFileReferences(out, tgt);
  }
  fprintf(out, "/* End PBXFileReference section */\n");
  return 0;
}


/* Begin PBXFrameworksBuildPhase section */

// This section describes the Frameworks that a Target links to. It does not 
// copy a framework into a target!
static int writeFrameworksBuildPhase(FILE *out, Fl_Target_Type *tgt) {
  
  char FrameworksBuildPhase[32]; strcpy(FrameworksBuildPhase, tgt->get_UUID_Xcode(Xcode4_FrameworksBuildPhase));
  
  fprintf(out, "\t\t%s /* Frameworks */ = {\n", FrameworksBuildPhase);
  fprintf(out, "\t\t\tisa = PBXFrameworksBuildPhase;\n");
  fprintf(out, "\t\t\tbuildActionMask = 2147483647;\n");
  fprintf(out, "\t\t\tfiles = (\n");
  
  // write all system dependencies
  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    if (f->file_is_framework()) {
      char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode(Xcode4_BuildFileInFrameworks));
      fprintf(out, "\t\t\t\t%s /* %s in %s */,\n", 
              PBXBuildFile, 
              f->filename_name(), 
              "Frameworks");
    }
  }
  
  // write all workspace dependencies
  Fl_Target_Dependency_Type *tgt_dep = Fl_Target_Dependency_Type::first_dependency(tgt);
  for ( ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
    Fl_Target_Type *dep = Fl_Target_Type::find(tgt_dep->name());
    if (dep && dep->is_lib_target()) {
      char PBXBuildFile[32]; strcpy(PBXBuildFile, tgt_dep->get_UUID_Xcode(Xcode4_BuildFileInFrameworks));
      fprintf(out, "\t\t\t\t%s /* %s in %s */,\n", 
              PBXBuildFile, 
              dep->name(), 
              "Frameworks");        
    } else {
      printf("Unresoved target dependency to \"%s\"\n", tgt_dep->name());
    }
  }
  
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\trunOnlyForDeploymentPostprocessing = 0;\n");
  fprintf(out, "\t\t};\n");
  return 0;
}

static int writeFrameworksBuildPhaseSection(FILE *out) {
  fprintf(out, "\n/* Begin PBXFrameworksBuildPhase section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeFrameworksBuildPhase(out, tgt);
  }
  fprintf(out, "/* End PBXFrameworksBuildPhase section */\n");
  return 0;
}


/* Begin PBXGroup section */

// This section generates the hierarchy of files and folders in the left panel
// in the Xcode interface.
//  - t can be a target or a group.
static int writeGroup(FILE *out, Fl_Tool_Type *t) {
  char Group[32]; strcpy(Group, t->get_UUID_Xcode(Xcode4_Group));
  int tlev1 = t->level+1;
  Fl_Tool_Type *t1;

  for (t1 = (Fl_Tool_Type*)t->next; t1 && t1->level>=tlev1; t1 = (Fl_Tool_Type*)t1->next) {
    if (t1->level==tlev1 && (t1->is_folder() || t1->is_target() || t1->is_category())) {
      Fl_Folder_Type *folder = (Fl_Folder_Type*)t1;
      writeGroup(out, folder);
    }
  }

  fprintf(out, "\t\t%s /* %s */ = {\n", Group, t->name());
  fprintf(out, "\t\t\tisa = PBXGroup;\n");
  fprintf(out, "\t\t\tchildren = (\n");
  for (t1 = (Fl_Tool_Type*)t->next; t1 && t1->level>=tlev1; t1 = (Fl_Tool_Type*)t1->next) {
    if (t1->level==tlev1) {
      if (t1->is_folder() || t1->is_target() || t1->is_category()) {
        Fl_Tool_Type *folder = (Fl_Tool_Type*)t1;
        char Group[32]; strcpy(Group, folder->get_UUID_Xcode(Xcode4_Group));
        fprintf(out, "\t\t\t\t%s /* %s */,\n", Group, folder->name());
      } if (t1->lists_in(FL_ENV_XC4) && t1->is_file()) {
        char PBXFileRef[32]; strcpy(PBXFileRef, t1->get_UUID_Xcode(Xcode4_FileRef));
        Fl_File_Type *f = (Fl_File_Type*)t1;
        fprintf(out, "\t\t\t\t%s /* %s */,\n", PBXFileRef, f->filename_name());
      }
    }
  }
  if (t==workspace) {
    char ProductsGroup[32]; strcpy(ProductsGroup, t->get_UUID_Xcode(Xcode4_ProductsGroup));
    fprintf(out, "\t\t\t\t%s /* %s */,\n", ProductsGroup, "Products");
  }
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\tname = %s;\n", t->name());
  fprintf(out, "\t\t\tsourceTree = \"<group>\";\n");
  fprintf(out, "\t\t};\n");

  return 0;
}

static int writeProductsGroup(FILE *out, Fl_Target_Type *tgt) {
  char ProductReference[32]; strcpy(ProductReference, tgt->get_UUID_Xcode(Xcode4_ProductFileReference));
  fprintf(out, "\t\t\t\t%s /* %s.app or .framework */,\n", ProductReference, tgt->name());
  return 0;
}

static int writeProductsGroup(FILE *out) {
  char ProductsGroup[32]; strcpy(ProductsGroup, workspace->get_UUID_Xcode(Xcode4_ProductsGroup));
  
  fprintf(out, "\t\t%s /* Products */ = {\n", ProductsGroup);
  fprintf(out, "\t\t\tisa = PBXGroup;\n");
  fprintf(out, "\t\t\tchildren = (\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeProductsGroup(out, tgt);
  }
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\tname = Products;\n");
  fprintf(out, "\t\t\tsourceTree = \"<group>\";\n");
  fprintf(out, "\t\t};\n");
  return 0;
}

static int writeGroupSection(FILE *out) {
  fprintf(out, "\n/* Begin PBXGroup section */\n");
  writeProductsGroup(out);
  writeGroup(out, workspace);
  fprintf(out, "/* End PBXGroup section */\n");
  return 0;
}


/* Begin PBXHeadersBuildPhase section */

// This section lists Headers that will be part of Frameworks. Other Target 
// types do not need to include any Headers.
static int writeHeadersBuildPhase(FILE *out, Fl_Target_Type *tgt) {
  
  char HeadersBuildPhase[32]; strcpy(HeadersBuildPhase, tgt->get_UUID_Xcode(Xcode4_HeadersBuildPhase));
  
  fprintf(out, "\t\t%s /* Headers */ = {\n", HeadersBuildPhase);
  fprintf(out, "\t\t\tisa = PBXHeadersBuildPhase;\n");
  fprintf(out, "\t\t\tbuildActionMask = 2147483647;\n");
  fprintf(out, "\t\t\tfiles = (\n");
  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    if (f->builds_in(FL_ENV_XC4) && f->file_is_header()) {
      char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode(Xcode4_BuildFileInHeaders));
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

static int writeHeadersBuildPhaseSection(FILE *out) {
  fprintf(out, "\n/* Begin PBXHeadersBuildPhase section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target())
      writeHeadersBuildPhase(out, tgt);
  }
  fprintf(out, "/* End PBXHeadersBuildPhase section */\n");
  return 0;
}


/* Begin PBXNativeTarget section */

// This section describes every Target in full detail.
static int writeNativeTarget(FILE *out, Fl_Target_Type *tgt) {
  // currently we still have a bunch of fixed UUIDs in here!
  
  char Target[32]; strcpy(Target, tgt->get_UUID_Xcode(Xcode4_Target));
  char buildConfigurationList[32]; strcpy(buildConfigurationList, tgt->get_UUID_Xcode(Xcode4_BuildConfigurationList));
  char ResourcesBuildPhase[32]; strcpy(ResourcesBuildPhase, tgt->get_UUID_Xcode(Xcode4_ResourcesBuildPhase));
  char SourcesBuildPhase[32]; strcpy(SourcesBuildPhase, tgt->get_UUID_Xcode(Xcode4_SourcesBuildPhase));
  char FrameworksBuildPhase[32]; strcpy(FrameworksBuildPhase, tgt->get_UUID_Xcode(Xcode4_FrameworksBuildPhase));
  char FluidBuildRule[32]; strcpy(FluidBuildRule, tgt->get_UUID_Xcode(Xcode4_FluidBuildRule));
  char ProductReference[32]; strcpy(ProductReference, tgt->get_UUID_Xcode(Xcode4_ProductFileReference));
  
  fprintf(out, "\t\t%s /* %s */ = {\n",Target ,tgt->name());
  fprintf(out, "\t\t\tisa = PBXNativeTarget;\n");
  fprintf(out, "\t\t\tbuildConfigurationList = %s /* Build configuration list for PBXNativeTarget \"%s\" */;\n", buildConfigurationList, tgt->name());
  fprintf(out, "\t\t\tbuildPhases = (\n");
  fprintf(out, "\t\t\t\t%s /* Resources */,\n", ResourcesBuildPhase);
  if (tgt->is_lib_target()) {
    char HeadersBuildPhase[32]; strcpy(HeadersBuildPhase, tgt->get_UUID_Xcode(Xcode4_HeadersBuildPhase));
    fprintf(out, "\t\t\t\t%s /* Headers */,\n", HeadersBuildPhase);
  }
  fprintf(out, "\t\t\t\t%s /* Sources */,\n", SourcesBuildPhase);
  fprintf(out, "\t\t\t\t%s /* Frameworks */,\n", FrameworksBuildPhase);
  if (tgt->is_app_target()) {
    char CopyFilesBuildPhase[32]; strcpy(CopyFilesBuildPhase, tgt->get_UUID_Xcode(Xcode4_CopyFilesBuildPhase));
    fprintf(out, "\t\t\t\t%s /* CopyFiles */,\n", CopyFilesBuildPhase);
  }
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\tbuildRules = (\n");
  fprintf(out, "\t\t\t\t%s /* PBXBuildRule */,\n", FluidBuildRule);
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\tdependencies = (\n");

  Fl_Target_Dependency_Type *tgt_dep = Fl_Target_Dependency_Type::first_dependency(tgt);
  for ( ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
    char TargetDependency[32]; strcpy(TargetDependency, tgt_dep->get_UUID_Xcode(Xcode4_TargetDependency));
    fprintf(out, "\t\t\t\t%s /* %s */,\n", TargetDependency, tgt_dep->name());
  }
  
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\tname = %s;\n", tgt->name());
  fprintf(out, "\t\t\tproductName = %s;\n", tgt->name());
  if (tgt->is_lib_target()) {
    fprintf(out, "\t\t\tproductReference = %s /* %s.framework */;\n", ProductReference, tgt->name()); // FIXME: .framework or .app
    fprintf(out, "\t\t\tproductType = \"com.apple.product-type.framework\";\n");
  } else if (tgt->is_app_target()) {
    fprintf(out, "\t\t\tproductReference = %s /* %s.app */;\n", ProductReference, tgt->name()); // FIXME: .framework or .app
    fprintf(out, "\t\t\tproductType = \"com.apple.product-type.application\";\n");
  }
  fprintf(out, "\t\t};\n");
  
  return 0;
}

static int writeNativeTargetSection(FILE *out) {
  fprintf(out, "\n/* Begin PBXNativeTarget section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeNativeTarget(out, tgt);
  }
  fprintf(out, "/* End PBXNativeTarget section */\n");
  return 0;
}


/* Begin PBXProject section */

// This section contains a few basic setting and a list of all targets.
static int writeProjectTarget(FILE *out, Fl_Target_Type *tgt) {
  char Target[32]; strcpy(Target, tgt->get_UUID_Xcode(Xcode4_Target));
  fprintf(out, "\t\t\t\t%s /* %s */,\n", Target, tgt->name());
  return 0;
}

static int writeProjectSection(FILE *out) {
  
  char Root[32]; strcpy(Root, workspace->get_UUID_Xcode(Xcode4_Root));
  char Group[32]; strcpy(Group, workspace->get_UUID_Xcode(Xcode4_Group));
  char ProductsGroup[32]; strcpy(ProductsGroup, workspace->get_UUID_Xcode(Xcode4_ProductsGroup));
  char BuildConfigurationList[32]; strcpy(BuildConfigurationList, workspace->get_UUID_Xcode(Xcode4_BuildConfigurationList));

  fprintf(out, "\n/* Begin PBXProject section */\n");
  fprintf(out, "\t\t%s /* Project object */ = {\n", Root);
  fprintf(out, "\t\t\tisa = PBXProject;\n");
  fprintf(out, "\t\t\tbuildConfigurationList = %s /* Build configuration list for PBXProject \"%s\" */;\n", BuildConfigurationList, workspace->name());
  fprintf(out, "\t\t\tcompatibilityVersion = \"Xcode 3.0\";\n");
  fprintf(out, "\t\t\tdevelopmentRegion = English;\n");
  fprintf(out, "\t\t\thasScannedForEncodings = 0;\n");
  fprintf(out, "\t\t\tknownRegions = (\n");
  fprintf(out, "\t\t\t\tEnglish,\n");
  fprintf(out, "\t\t\t\tJapanese,\n");
  fprintf(out, "\t\t\t\tFrench,\n");
  fprintf(out, "\t\t\t\tGerman,\n");
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\tmainGroup = %s;\n", Group);
  fprintf(out, "\t\t\tproductRefGroup = %s /* Products */;\n", ProductsGroup);
  fprintf(out, "\t\t\tprojectDirPath = \"\";\n");
  fprintf(out, "\t\t\tprojectRoot = \"\";\n");
  fprintf(out, "\t\t\ttargets = (\n");
  
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeProjectTarget(out, tgt);
  }
  
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t};\n");
  fprintf(out, "/* End PBXProject section */\n");
  
  return 0;
}


/* Begin PBXResourcesBuildPhase section */

// This section writes a list of references to Buil Files of type source.
// Resources are all files that are not in Sources or Headers (i.e. icons, etc.)
static int writeResourcesBuildPhase(FILE *out, Fl_Target_Type *tgt) {
  char key[32]; strcpy(key, tgt->get_UUID_Xcode(Xcode4_ResourcesBuildPhase));
  fprintf(out, "\t\t%s /* Resources */ = {\n", key);
  fprintf(out, "\t\t\tisa = PBXResourcesBuildPhase;\n");
  fprintf(out, "\t\t\tbuildActionMask = 2147483647;\n");
  fprintf(out, "\t\t\tfiles = (\n");
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\trunOnlyForDeploymentPostprocessing = 0;\n");
  fprintf(out, "\t\t};\n");
  return 0;
}

static int writeResourcesBuildPhaseSection(FILE *out) {
  // FIXME:
  fprintf(out, "\n/* Begin PBXResourcesBuildPhase section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeResourcesBuildPhase(out, tgt);
  }
  fprintf(out, "/* End PBXResourcesBuildPhase section */\n");
  return 0;
}


/* Begin PBXSourcesBuildPhase section */

// This section writes a list of references to Build Files of type source.
// This includes .fl files, but not the files that are generated via Fluid.
static int writeSourcesBuildPhase(FILE *out, Fl_Target_Type *tgt) {
  
  // get the required keys
  char SourcesBuildPhase[32]; strcpy(SourcesBuildPhase, tgt->get_UUID_Xcode(Xcode4_SourcesBuildPhase));
  
  // write a list of all source files
  fprintf(out, "\t\t%s /* Sources */ = {\n", SourcesBuildPhase);
  fprintf(out, "\t\t\tisa = PBXSourcesBuildPhase;\n");
  fprintf(out, "\t\t\tbuildActionMask = 2147483647;\n");
  fprintf(out, "\t\t\tfiles = (\n");
  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    if (f->builds_in(FL_ENV_XC4) && (f->file_is_code() || f->file_is_fluid_ui())) {
      char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode(Xcode4_BuildFileInSources));
      fprintf(out, "\t\t\t\t%s /* %s in Sources */,\n", 
              PBXBuildFile, 
              f->filename_name());
    }
  }
  fprintf(out, "\t\t\t);\n");
  fprintf(out, "\t\t\trunOnlyForDeploymentPostprocessing = 0;\n");
  fprintf(out, "\t\t};\n");
  return 0;
}

static int writeSourcesBuildPhaseSection(FILE *out) {
  fprintf(out, "\n/* Begin PBXSourcesBuildPhase section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeSourcesBuildPhase(out, tgt);
  }
  fprintf(out, "/* End PBXSourcesBuildPhase section */\n");
  return 0;
}


/* Begin PBXTargetDependency section */

static int writeTargetDependency(FILE *out, Fl_Target_Type *tgt) {
  Fl_Target_Dependency_Type *tgt_dep = Fl_Target_Dependency_Type::first_dependency(tgt);
  for ( ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
    Fl_Target_Type *dep = Fl_Target_Type::find(tgt_dep->name());
    if (dep) {
      Fl_Workspace_Type *wsp = (Fl_Workspace_Type*)Fl_Type::first;
      
      char Root[32]; strcpy(Root, wsp->get_UUID_Xcode(Xcode4_Root));
      char ContainerItemProxy[32]; strcpy(ContainerItemProxy, wsp->get_UUID_Xcode(Xcode4_ContainerItemProxy));
      char TargetDependency[32]; strcpy(TargetDependency, tgt_dep->get_UUID_Xcode(Xcode4_TargetDependency));
      char DepTarget[32]; strcpy(DepTarget, dep->get_UUID_Xcode(Xcode4_Target));
      
      fprintf(out, "\t\t%s /* PBXTargetDependency */ = {\n", TargetDependency);
      fprintf(out, "\t\t\tisa = PBXTargetDependency;\n");
      fprintf(out, "\t\t\ttarget = %s /* %s */;\n", DepTarget, dep->name());
      fprintf(out, "\t\t\ttargetProxy = %s /* PBXContainerItemProxy */;\n", ContainerItemProxy);
      fprintf(out, "\t\t};\n");
      
    } else {
      printf("Unresoved target dependency to \"%s\"\n", tgt_dep->name());
    }
  }
  return 0;
}

static int writeTargetDependencySection(FILE *out) {
  fprintf(out, "\n/* Begin PBXTargetDependency section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeTargetDependency(out, tgt);
  }
  fprintf(out, "/* End PBXTargetDependency section */\n");
  return 0;
}


/* Begin XCBuildConfiguration section */

// This section is referenced by the Build Configuration Lists. It directly
// writes compiler and linker flags and other settings.
//  - we currently support just one standard settting for Debug and Release
static int writeBuildConfigurations(FILE *out, Fl_Target_Type *tgt) {
  
  // get the required keys
  char debugKey[32]; strcpy(debugKey, tgt->get_UUID_Xcode(Xcode4_DebugBuildConfiguration));
  char releaseKey[32]; strcpy(releaseKey, tgt->get_UUID_Xcode(Xcode4_ReleaseBuildConfiguration));

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
  fprintf(out, "\t\t\t\tINFOPLIST_FILE = \"plists/%s-Info.plist\";\n", tgt->name());
  fprintf(out, "\t\t\t\tINSTALL_PATH = \"@executable_path/../Frameworks\";\n");
  fprintf(out, "\t\t\t\tOTHER_LDFLAGS = \"\";\n");
  fprintf(out, "\t\t\t\tPRODUCT_NAME = \"%s\";\n", tgt->name());
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
  fprintf(out, "\t\t\t\tINFOPLIST_FILE = \"plists/%s-Info.plist\";\n", tgt->name());
  fprintf(out, "\t\t\t\tINSTALL_PATH = \"@executable_path/../Frameworks\";\n");
  fprintf(out, "\t\t\t\tOTHER_LDFLAGS = \"\";\n");
  fprintf(out, "\t\t\t\tPRODUCT_NAME = \"%s\";\n", tgt->name());
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

static int writeBuildConfigurations(FILE *out, Fl_Workspace_Type *wsp) {
  
  // get the required keys
  char debugKey[32]; strcpy(debugKey, wsp->get_UUID_Xcode(Xcode4_DebugBuildConfiguration));
  char releaseKey[32]; strcpy(releaseKey, wsp->get_UUID_Xcode(Xcode4_ReleaseBuildConfiguration));
  
  fprintf(out, "\t\t%s /* Debug */ = {\n", debugKey);
  fprintf(out, "\t\t\tisa = XCBuildConfiguration;\n");
  fprintf(out, "\t\t\tbuildSettings = {\n");
  fprintf(out, "\t\t\t\tARCHS = \"$(NATIVE_ARCH_ACTUAL)\";\n");
  fprintf(out, "\t\t\t\tGCC_C_LANGUAGE_STANDARD = gnu99;\n");
  fprintf(out, "\t\t\t\tGCC_OPTIMIZATION_LEVEL = 0;\n");
  fprintf(out, "\t\t\t\tGCC_WARN_ABOUT_RETURN_TYPE = YES;\n");
  fprintf(out, "\t\t\t\tGCC_WARN_UNUSED_VARIABLE = YES;\n");
  fprintf(out, "\t\t\t\tONLY_ACTIVE_ARCH = YES;\n");
  fprintf(out, "\t\t\t\tPREBINDING = NO;\n");
  fprintf(out, "\t\t\t\tSDKROOT = \"$(DEVELOPER_SDK_DIR)/MacOSX10.5.sdk\";\n");
  fprintf(out, "\t\t\t};\n");
  fprintf(out, "\t\t\tname = Debug;\n");
  fprintf(out, "\t\t};\n");
  
  fprintf(out, "\t\t%s /* Release */ = {\n", releaseKey);
  fprintf(out, "\t\t\tisa = XCBuildConfiguration;\n");
  fprintf(out, "\t\t\tbuildSettings = {\n");
  fprintf(out, "\t\t\t\tARCHS = \"$(ARCHS_STANDARD_32_64_BIT_PRE_XCODE_3_1)\";\n");
  fprintf(out, "\t\t\t\tARCHS_STANDARD_32_64_BIT_PRE_XCODE_3_1 = \"x86_64 i386 ppc\";\n");
  fprintf(out, "\t\t\t\tGCC_C_LANGUAGE_STANDARD = gnu99;\n");
  fprintf(out, "\t\t\t\tGCC_WARN_ABOUT_RETURN_TYPE = YES;\n");
  fprintf(out, "\t\t\t\tGCC_WARN_UNUSED_VARIABLE = YES;\n");
  fprintf(out, "\t\t\t\tPREBINDING = NO;\n");
  fprintf(out, "\t\t\t\tSDKROOT = \"$(DEVELOPER_SDK_DIR)/MacOSX10.5.sdk\";\n");
  fprintf(out, "\t\t\t};\n");
  fprintf(out, "\t\t\tname = Release;\n");
  fprintf(out, "\t\t};\n");
  
  return  0;
}

static int writeBuildConfigurationSection(FILE *out) {
  fprintf(out, "\n/* Begin XCBuildConfiguration section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeBuildConfigurations(out, tgt);
  }
  writeBuildConfigurations(out, workspace);
  fprintf(out, "/* End XCBuildConfiguration section */\n");
  return 0;
}


/* Begin XCConfigurationList section */

// Write the section that is references from the Native Target and lists
// all possible build configurations.
//  - we currently limit this to the default Debug and Release build.
static int writeBuildConfigurationList(FILE *out, Fl_Target_Type *tgt) {
  
  // get the required keys
  char listKey[32]; strcpy(listKey, tgt->get_UUID_Xcode(Xcode4_BuildConfigurationList));
  char debugKey[32]; strcpy(debugKey, tgt->get_UUID_Xcode(Xcode4_DebugBuildConfiguration));
  char releaseKey[32]; strcpy(releaseKey, tgt->get_UUID_Xcode(Xcode4_ReleaseBuildConfiguration));
  
  // write the section for this target
  fprintf(out, "\t\t%s /* Build configuration list for PBXNativeTarget \"%s\" */ = {\n", listKey, tgt->name());
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

static int writeBuildConfigurationList(FILE *out, Fl_Workspace_Type *wsp) {
  
  // get the required keys
  char listKey[32]; strcpy(listKey, wsp->get_UUID_Xcode(Xcode4_BuildConfigurationList));
  char debugKey[32]; strcpy(debugKey, wsp->get_UUID_Xcode(Xcode4_DebugBuildConfiguration));
  char releaseKey[32]; strcpy(releaseKey, wsp->get_UUID_Xcode(Xcode4_ReleaseBuildConfiguration));
  
  // write the section for this target
  fprintf(out, "\t\t%s /* Build configuration list for PBXNativeTarget \"%s\" */ = {\n", listKey, wsp->name());
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

static int writeConfigurationListSection(FILE *out) {
  fprintf(out, "\n/* Begin XCConfigurationList section */\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    writeBuildConfigurationList(out, tgt);
  }
  writeBuildConfigurationList(out, workspace);
  fprintf(out, "/* End XCConfigurationList section */\n");
  return 0;
}


// ------------ file writers dispatch-------------------------------------------


int write_fltk_ide_xcode4() {
  
  workspace = (Fl_Workspace_Type*)Fl_Type::first;
  
#if 1
  
  char buf[2048], base_dir[2048], tgt_base[2048];
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  strcpy(tgt_base, base_dir);
  strcpy(buf, base_dir);
  strcat(buf, "ide/Xcode4/FLTK.xcodeproj/project.pbxproj");
  FILE *out = fopen(buf, "wb");
  
  fprintf(out, "\n");
  
  fprintf(out, "// !$*UTF8*$!\n");
  fprintf(out, "{\n");
  fprintf(out, "\tarchiveVersion = 1;\n");
  fprintf(out, "\tclasses = {\n");
  fprintf(out, "\t};\n");
  fprintf(out, "\tobjectVersion = 44;\n");
  fprintf(out, "\tobjects = {\n");

  writeBuildFileSection(out);
  writeBuildRuleSection(out);
  writeContainerItemProxySection(out);
  writeCopyFilesBuildPhaseSection(out);
  writeFileReferenceSection(out);
  writeFrameworksBuildPhaseSection(out);
  writeGroupSection(out);
  writeHeadersBuildPhaseSection(out);
  writeNativeTargetSection(out);
  writeProjectSection(out);
  writeResourcesBuildPhaseSection(out);
  writeSourcesBuildPhaseSection(out);
  writeTargetDependencySection(out);
  writeBuildConfigurationSection(out);
  writeConfigurationListSection(out);
  
  char Root[32]; strcpy(Root, workspace->get_UUID_Xcode(Xcode4_Root));
  
  fprintf(out, "\t};\n");
  fprintf(out, "\trootObject = %s /* Project object */;\n", Root);
  fprintf(out, "}\n");

  fclose(out);
  
#else
  
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
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+21, ')'); // keep tgt local
          writeBuildFileReferences(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#ContainerItemProxy(", 20)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+20, ')');
          writeContainerItemProxy(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#TargetDependency(", 18)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+18, ')');
          writeTargetDependency(out, tgt);
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
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode(Xcode4_FileRef));
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
            if (f->lists_in(FL_ENV_XC4) && (f->file_is_code() || f->file_is_fluid_ui()) {
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode(Xcode4_FileRef));
              fprintf(out, "\t\t\t\t%s /* %s */,\n", 
                      PBXFileRef, 
                      f->filename_name());
            }
          }
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#ResourcesGroup(", 16)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+16, ')');
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->lists_in(FL_ENV_XC4) && f->file_is_framework()) {
              char PBXFileRef[32]; strcpy(PBXFileRef, f->get_UUID_Xcode(Xcode4_FileRef));
              fprintf(out, "\t\t\t\t%s /* %s */,\n", 
                      PBXFileRef, 
                      f->filename_name());
            }
          }
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#TargetGroup(", 13)==0) {
          Fl_Tool_Type *tgt = Fl_Target_Type::find(hash+13, ')');
          writeGroup(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#ProductsGroup(", 15)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+15, ')');
          writeProductsGroup(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#TargetGroupRef(", 16)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+16, ')');
          char Group[32]; strcpy(Group, tgt->get_UUID_Xcode(Xcode4_Group));
          fprintf(out, "\t\t\t\t%s /* %s */,\n", Group, tgt->name());
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#HeadersBuildPhase(", 19)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+19, ')');
          writeHeadersBuildPhase(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#CopyFilesBuildPhase(", 21)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+21, ')');
          writeCopyFilesBuildPhase(out, tgt);
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
              char PBXBuildFile[32]; strcpy(PBXBuildFile, f->get_UUID_Xcode(Xcode4_BuildFileInSources));
              fprintf(out, "\t\t\t\t%s /* %s in %s */,\n", 
                      PBXBuildFile, 
                      f->filename_name(), 
                      "Sources");
            }
          }
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#BuildConfigurations(", 21)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+21, ')');
          writeBuildConfigurations(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#BuildConfigurationList(", 24)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+24, ')');
          writeBuildConfigurationList(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#ResourcesBuildPhase(", 21)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+21, ')');
          writeResourcesBuildPhase(out, tgt);
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#FrameworksBuildPhase(", 22)==0) {
          Fl_Target_Type *tgt = Fl_Target_Type::find(hash+22, ')');
          writeFrameworksBuildPhase(out, tgt);
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
  
#endif
  
  return 0;
}


//
// End of "$Id: file_xcode.cxx 8870 2011-07-26 21:19:35Z matt $".
//
