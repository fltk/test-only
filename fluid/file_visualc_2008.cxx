//
// "$Id$"
//
// Fluid VisualC 2008 output for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2012 by Matthias Melcher and others.
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
#include "WorkspaceType.h"
#include <fltk3/filename.h>


extern const char *filename;
extern const char *DOS_path(const char *filename);


static const char *VC2008_Workspace = "VC2008_Workspace";
static const char *VC2008_Project = "VC2008_Project";


// ------------ VisualC 2008 ---------------------------------------------------


/*
 Write a VisualC 2008 Solution file.
 
 We write build configurations for Win32 Debug and Win32 Release.
 There are currently no Cairo setting (I will solve that differently)
 */
static int write_sln_file(FILE *out, Fl_Workspace_Type *workspace) {
  Fl_Target_Type *tgt;
  // write header
  fprintf(out, "Microsoft Visual Studio Solution File, Format Version 10.00\r\n");
  fprintf(out, "# Visual C++ Express 2008\r\n");
  
  // write all projects and their dependencies
  for (tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_VC2008)) {
      fprintf(out, "Project(\"{%s}\") = \"%s\", \"%s.vcproj\", \"{%s}\"\r\n",
              workspace->get_UUID(VC2008_Workspace), 
              tgt->name(), 
              tgt->name(),
              tgt->get_UUID(VC2008_Project));
      fprintf(out, "\tProjectSection(ProjectDependencies) = postProject\r\n");
      // write all the dependencies
      Fl_Target_Dependency_Type *tgt_dep = Fl_Target_Dependency_Type::first_dependency(tgt);
      for ( ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
        Fl_Target_Type *dep = Fl_Target_Type::find(tgt_dep->name());
        if (dep && tgt_dep->builds_in(FL_ENV_VC2008)) {
          fprintf(out, "\t\t{%s} = {%s}\r\n",
                  dep->get_UUID(VC2008_Project),
                  dep->get_UUID(VC2008_Project));
        }
      }
      fprintf(out, "\tEndProjectSection\r\n");
      fprintf(out, "EndProject\r\n");
    }
  }
  
  // begin the global section
  fprintf(out, "Global\r\n");
  
  // write a list of all configurations
  fprintf(out, "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\r\n");
  fprintf(out, "\t\tDebug|Win32 = Debug|Win32\r\n");
  fprintf(out, "\t\tRelease|Win32 = Release|Win32\r\n");
  fprintf(out, "\tEndGlobalSection\r\n");
  
  // now write a list of configurations for each target
  fprintf(out, "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\r\n");
  for (tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_VC2008)) {
      fprintf(out, "\t\t{%s}.Debug|Win32.ActiveCfg = Debug|Win32\r\n", tgt->get_UUID(VC2008_Project));
      fprintf(out, "\t\t{%s}.Debug|Win32.Build.0 = Debug|Win32\r\n", tgt->get_UUID(VC2008_Project));
      fprintf(out, "\t\t{%s}.Release|Win32.ActiveCfg = Release|Win32\r\n", tgt->get_UUID(VC2008_Project));
      fprintf(out, "\t\t{%s}.Release|Win32.Build.0 = Release|Win32\r\n", tgt->get_UUID(VC2008_Project));
    }
  }
  
  fprintf(out, "\tEndGlobalSection\r\n");
  fprintf(out, "\tGlobalSection(SolutionProperties) = preSolution\r\n");
  fprintf(out, "\t\tHideSolutionNode = FALSE\r\n");
  fprintf(out, "\tEndGlobalSection\r\n");
  fprintf(out, "EndGlobal\r\n");
  
  return 0;
}


/*
 Write the build rules for .cxx files.
 
 Since .fl files need a build rule for corresponding .cxx files, we can set 
 a specific filename as an optional argument.
 */
static int write_source_file(FILE *out, Fl_File_Type *file, const char *filename=0) {
  if (!filename) filename = file->filename();
  fprintf(out, "\t\t<File\r\n");
  fprintf(out, "\t\t\tRelativePath=\"..\\..\\%s\"\r\n", DOS_path(filename));
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
  fprintf(out, "\t\t\t\t\tAdditionalIncludeDirectories=\"\"\r\n");
  fprintf(out, "\t\t\t\t\tPreprocessorDefinitions=\"\"\r\n");
  fprintf(out, "\t\t\t\t/>\r\n");
  fprintf(out, "\t\t\t</FileConfiguration>\r\n");
  fprintf(out, "\t\t</File>\r\n");
  return 0;
}


/*
 Write the .fl build rule and the corresponding .cxx build rule.
 */
static int write_fluid_source_file(FILE *out, Fl_File_Type *file) {
  char cxxName[2048], flPath[2048];
  strcpy(cxxName, file->filename());
  fltk3::filename_setext(cxxName, 2048, ".cxx");
  strcpy(flPath, file->filename());
  *((char*)fltk3::filename_name(flPath)) = 0; // keep only the path
  
  write_source_file(out, file, cxxName);
  
  fprintf(out, "\t\t<File\r\n");
  fprintf(out, "\t\t\tRelativePath=\"..\\..\\%s\"\r\n", DOS_path(file->filename()));
  fprintf(out, "\t\t\t>\r\n");
  fprintf(out, "\t\t\t<FileConfiguration\r\n");
  fprintf(out, "\t\t\t\tName=\"Debug|Win32\"\r\n");
  fprintf(out, "\t\t\t\t>\r\n");
  fprintf(out, "\t\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\t\tName=\"VCCustomBuildTool\"\r\n");
  fprintf(out, "\t\t\t\t\tDescription=\"Create .cxx and .h file with fluidd\"\r\n");
  fprintf(out, "\t\t\t\t\tCommandLine=\"cfluid /D $(InputName).fl&#x0D;&#x0A;\"\r\n");
  fprintf(out, "\t\t\t\t\tOutputs=\"..\\..\\%s\\$(InputName).cxx\"\r\n", DOS_path(flPath));
  fprintf(out, "\t\t\t\t/>\r\n");
  fprintf(out, "\t\t\t</FileConfiguration>\r\n");
  fprintf(out, "\t\t\t<FileConfiguration\r\n");
  fprintf(out, "\t\t\t\tName=\"Release|Win32\"\r\n");
  fprintf(out, "\t\t\t\t>\r\n");
  fprintf(out, "\t\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\t\tName=\"VCCustomBuildTool\"\r\n");
  fprintf(out, "\t\t\t\t\tDescription=\"Create .cxx and .h file with fluid\"\r\n");
  fprintf(out, "\t\t\t\t\tCommandLine=\"cfluid $(InputName).fl&#x0D;&#x0A;\"\r\n");
  fprintf(out, "\t\t\t\t\tOutputs=\"..\\..\\%s\\$(InputName).cxx\"\r\n", DOS_path(flPath));
  fprintf(out, "\t\t\t\t/>\r\n");
  fprintf(out, "\t\t\t</FileConfiguration>\r\n");
  fprintf(out, "\t\t</File>\r\n");
  
  return 0;
}


/*
 Write all the source file dependencies for a VC2008 project file.
 
 This also generates the build rules for .fl files.
 */
static int write_source_files(FILE *out, Fl_Target_Type *tgt) {
  for (Fl_File_Type *file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->builds_in(FL_ENV_VC2008)) {
      if (file->file_is_fluid_ui()) {
        write_fluid_source_file(out, file);
      } else if (file->file_is_code()) {
        write_source_file(out, file);
      }
    }
  }
  return 0;
}


/*
 Create the list of linked libraries.
 
 If is_debug is set, all internal target links will link to the 
 debug version of that library
 */
static int write_additional_dependencies(FILE *out, Fl_Target_Type *tgt, char is_debug) {
  for (Fl_Type *t = tgt->next; t && (t->level>tgt->level); t = t->next) {
    if (t->is_tool() && ((Fl_Tool_Type*)t)->builds_in(FL_ENV_VC2008)) {
      if (t->is_file() && ((Fl_File_Type*)t)->file_is_library()) {
        fprintf(out, "%s ", t->name());
      } else if (t->is_target_dependency() && tgt->is_app_target()) {
        Fl_Target_Type *dep = Fl_Target_Type::find(t->name());
        if (dep && dep->is_lib_target()) {
          fprintf(out, "%s%s.lib ", t->name(), (is_debug?"d":""));
        }
      }
    }
  }
  return 0;
}


static int write_configuration(FILE *out, Fl_Target_Type *tgt, char is_debug) {
  // some definitions we will need later
  const char *cfg, *pre, *pre2, *debug_d;
  int opt, lib, cfg_type;
  char is_lib, cfg_path[256];
  
  if (is_debug) {
    cfg = "Debug";  // this is the name of the configuration as well as the path for all temporary files
    pre = "_DEBUG"; // this is a preprocessor definition for compile time configuration
    opt = 0;        // this is the level of optimization
    lib = 3;        // this is the link library (not sure what it actually does...)
    sprintf(cfg_path, "%s%s", tgt->name(), "_debug");
	debug_d = "d";
  } else {
    cfg = "Release";
    pre = "NDEBUG";
    opt = 4;
    lib = 2;
    sprintf(cfg_path, "%s%s", tgt->name(), "_release");
	debug_d = "";
  }
  
  if (tgt->is_lib_target()) {
    is_lib = 1;           // are we building a (static) library?
    cfg_type = 4;         // type of configuration
    pre2 = "FL_LIBRARY;"; // additional preprocessor definition
  } else {
    cfg_type = 1;
    is_lib = 0;
    pre2 = "";
  }
  
  fprintf(out, "\t\t<Configuration\r\n");
  fprintf(out, "\t\t\tName=\"%s|Win32\"\r\n", cfg);
  fprintf(out, "\t\t\tOutputDirectory=\"..\\..\\%s\"\r\n", DOS_path(tgt->target_path()));
  fprintf(out, "\t\t\tIntermediateDirectory=\".\\%s\"\r\n", cfg_path);
  fprintf(out, "\t\t\tConfigurationType=\"%d\"\r\n", cfg_type);
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
  fprintf(out, "\t\t\t\tName=\"VCMIDLTool\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"%s\"\r\n", pre);
  if (!is_lib) { // Not sure why any of these are missing for libraries, but the are...
    fprintf(out, "\t\t\t\tMkTypLibCompatible=\"true\"\r\n");
    fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
    fprintf(out, "\t\t\t\tTargetEnvironment=\"1\"\r\n");
    fprintf(out, "\t\t\t\tTypeLibraryName=\".\\%s\\%s.tlb\"\r\n", cfg_path, tgt->name());
    fprintf(out, "\t\t\t\tHeaderFileName=\"\"\r\n");
  }
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCCLCompilerTool\"\r\n");
  fprintf(out, "\t\t\t\tOptimization=\"%d\"\r\n", opt);
  if (!is_debug) {
    fprintf(out, "\t\t\t\tInlineFunctionExpansion=\"2\"\r\n");
    fprintf(out, "\t\t\t\tFavorSizeOrSpeed=\"2\"\r\n");
  }
  fprintf(out, "\t\t\t\tAdditionalIncludeDirectories=\".;..\\..\\include;..\\..\\include\\fltk3zlib;..\\..\\include\\fltk3png;..\\..\\include\\fltk3jpeg\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"_CRT_SECURE_NO_DEPRECATE;WIN32;%s;%s_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN\"\r\n", pre, pre2);
  fprintf(out, "\t\t\t\tRuntimeLibrary=\"%d\"\r\n", lib);
  fprintf(out, "\t\t\t\tUsePrecompiledHeader=\"0\"\r\n");
  fprintf(out, "\t\t\t\tPrecompiledHeaderFile=\".\\%s\\%s.pch\"\r\n", cfg_path, tgt->name());
  fprintf(out, "\t\t\t\tAssemblerListingLocation=\".\\%s\\\"\r\n", cfg_path);
  fprintf(out, "\t\t\t\tObjectFile=\".\\%s\\\"\r\n", cfg_path);
  fprintf(out, "\t\t\t\tProgramDataBaseFileName=\".\\%s\\\"\r\n", cfg_path);
  if (is_lib) {
    fprintf(out, "\t\t\t\tBrowseInformation=\"1\"\r\n");
  }
  fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
  if (is_debug) {
    fprintf(out, "\t\t\t\tDebugInformationFormat=\"3\"\r\n");
  }
  fprintf(out, "\t\t\t\tCompileAs=\"0\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCManagedResourceCompilerTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCResourceCompilerTool\"\r\n");
  fprintf(out, "\t\t\t\tPreprocessorDefinitions=\"%s\"\r\n", pre);
  fprintf(out, "\t\t\t\tCulture=\"1033\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPreLinkEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  
  if (is_lib) {
    fprintf(out, "\t\t\t<Tool\r\n");
    fprintf(out, "\t\t\t\tName=\"VCLibrarianTool\"\r\n");
    fprintf(out, "\t\t\t\tOutputFile=\"..\\..\\%s\\%s%s.lib\"\r\n", DOS_path(tgt->target_path()), tgt->name(), debug_d);
    fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
    fprintf(out, "\t\t\t/>\r\n");
  } else {
    fprintf(out, "\t\t\t<Tool\r\n");
    fprintf(out, "\t\t\t\tName=\"VCLinkerTool\"\r\n");
    
    fprintf(out, "\t\t\t\tAdditionalDependencies=\"");
    write_additional_dependencies(out, tgt, is_debug);
    fprintf(out, "comctl32.lib\"\r\n");
    
    fprintf(out, "\t\t\t\tOutputFile=\"..\\..\\%s\\%s%s.exe\"\r\n", DOS_path(tgt->target_path()), tgt->name(), debug_d);
    fprintf(out, "\t\t\t\tLinkIncremental=\"1\"\r\n");
    fprintf(out, "\t\t\t\tSuppressStartupBanner=\"true\"\r\n");
    fprintf(out, "\t\t\t\tAdditionalLibraryDirectories=\"..\\..\\lib\"\r\n");
    fprintf(out, "\t\t\t\tIgnoreDefaultLibraryNames=\"libcd\"\r\n");
    if (is_debug) {
      fprintf(out, "\t\t\t\tGenerateDebugInformation=\"true\"\r\n");
    }
    fprintf(out, "\t\t\t\tProgramDatabaseFile=\".\\%s\\%s.pdb\"\r\n", cfg_path, tgt->name());
    fprintf(out, "\t\t\t\tSubSystem=\"2\"\r\n");
    fprintf(out, "\t\t\t\tRandomizedBaseAddress=\"1\"\r\n");
    fprintf(out, "\t\t\t\tDataExecutionPrevention=\"0\"\r\n");
    fprintf(out, "\t\t\t\tTargetMachine=\"1\"\r\n");
    fprintf(out, "\t\t\t/>\r\n");
  }
  
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCALinkTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  if (!is_lib) {
    fprintf(out, "\t\t\t<Tool\r\n");
    fprintf(out, "\t\t\t\tName=\"VCManifestTool\"\r\n");
    fprintf(out, "\t\t\t/>\r\n");
  }
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCXDCMakeTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCBscMakeTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCFxCopTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  if (!is_lib) {
    fprintf(out, "\t\t\t<Tool\r\n");
    fprintf(out, "\t\t\t\tName=\"VCAppVerifierTool\"\r\n");
    fprintf(out, "\t\t\t/>\r\n");
  }
  fprintf(out, "\t\t\t<Tool\r\n");
  fprintf(out, "\t\t\t\tName=\"VCPostBuildEventTool\"\r\n");
  fprintf(out, "\t\t\t/>\r\n");
  fprintf(out, "\t\t</Configuration>\r\n");

  return 0;
}

static int write_vcproj_file(FILE *out, Fl_Target_Type *tgt) {
  
  // intro
  fprintf(out, "<?xml version=\"1.0\" encoding=\"Windows-1252\"?>\r\n");
  fprintf(out, "<VisualStudioProject\r\n");
  fprintf(out, "\tProjectType=\"Visual C++\"\r\n");
  fprintf(out, "\tVersion=\"9,00\"\r\n");
  fprintf(out, "\tName=\"%s\"\r\n", tgt->name());
  fprintf(out, "\tProjectGUID=\"{%s}\"\r\n", tgt->get_UUID(VC2008_Project));
  fprintf(out, "\tRootNamespace=\"%s\"\r\n", tgt->name()); // FIXME: when is this required?
  fprintf(out, "\tTargetFrameworkVersion=\"131072\"\r\n");
  fprintf(out, "\t>\r\n");
  
  // platforms
  fprintf(out, "\t<Platforms>\r\n");
  fprintf(out, "\t\t<Platform\r\n");
  fprintf(out, "\t\t\tName=\"Win32\"\r\n");
  fprintf(out, "\t\t/>\r\n");
  fprintf(out, "\t</Platforms>\r\n");
  
  // tool files
  fprintf(out, "\t<ToolFiles>\r\n");
  fprintf(out, "\t</ToolFiles>\r\n");
  
  // configurations
  fprintf(out, "\t<Configurations>\r\n");  
  write_configuration(out, tgt, 1);
  write_configuration(out, tgt, 0);
  fprintf(out, "\t</Configurations>\r\n");  

  // references
  fprintf(out, "\t<References>\r\n");
  fprintf(out, "\t</References>\r\n");

  // filter
  // TODO: include header file if they are so configured
  // TODO: create visual folders in the IDE if possible (surely is!)
  // This is done using a <Filter> section: 
  /*
   <               <Filter
   <                       Name="Headers"
   <                       Filter="*.h*"
   <                       >
   <                       <File
   <                               RelativePath="..\..\fltk3\Adjuster.h"
   <                               >
   <                       </File>
   */
  
  // files
  fprintf(out, "\t<Files>\r\n");
  write_source_files(out, tgt);
  fprintf(out, "\t</Files>\r\n");

  // gobals
  fprintf(out, "\t<Globals>\r\n");
  fprintf(out, "\t</Globals>\r\n");

  // end
  fprintf(out, "</VisualStudioProject>\r\n");
  
  return 0;
}



/*
 Write all VisualC 2008 IDE files.
 
 This module write all the files needed to create the VisualC 2008 IDE. It 
 generates an fltk.sln file and all vcproj files depending on it. All 
 parameters are taken from the Fl_..Type hierarchy in Fluid. 
 
 FIXME: !!! Currently, this function is limited to writing the FLTK build 
 system itself. This module was tested with fltk.sln. Eventually, we will be 
 writing universal new IDE setups as a convinience to FLTK/FLUID users.
 */
int write_fltk_ide_visualc2008() {
  
  Fl_Workspace_Type *workspace = (Fl_Workspace_Type*)Fl_Type::first;
  
  char buf[2048], base_dir[2048], tgt_base[2048];
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  strcpy(tgt_base, base_dir);
  strcpy(buf, base_dir);
  
  // write workspace file (.sln)
  strcat(buf, "ide/VisualC2008/fltk.sln");
  // FIXME: use workspace->name();  
  FILE *out = fopen(buf, "wb");
  write_sln_file(out, workspace);
  fclose(out);
  
  // write project files (.vcproj)
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_VC2008)) {
      if (tgt->is_app_target()) {
        sprintf(buf, "%side/VisualC2008/%s.vcproj", base_dir, tgt->name());
        out = fopen(buf, "wb");
        write_vcproj_file(out, tgt);
        fclose(out);
      } else if (tgt->is_lib_target()) {
        sprintf(buf, "%side/VisualC2008/%s.vcproj", base_dir, tgt->name());
        out = fopen(buf, "wb");
        write_vcproj_file(out, tgt);
        fclose(out);
        // TODO: no support for dll's yet
      } else {
        printf("Internale error writing unknown VC2008 target!\n");
      }
    }
  }
  
  return 0;
}


//
// End of "$Id$".
//
