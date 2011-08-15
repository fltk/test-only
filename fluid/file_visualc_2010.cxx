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


static const char *VC2010_Workspace = "VC2010_Workspace";
static const char *VC2010_Project = "VC2010_Project";

extern const char *filename;

// ------------ file conversion ------------------------------------------------

const char *DOS_path(const char *filename) {
  static char buf[2048];
  char *c;
  strcpy(buf, filename);
  for (c=buf;;c++) {
    switch (*c) {
      case '/': *c = '\\'; break;
      case 0: return buf;
    }
  }
  return buf;
}

// ------------ VisualC 2010 ---------------------------------------------------


static int write_sln_configuration(FILE *out, Fl_Workspace_Type *workspace, Fl_Target_Type *tgt) {
  fprintf(out, "\t\t{%s}.Debug|Win32.ActiveCfg = Debug|Win32\r\n", tgt->get_UUID(VC2010_Project));
  fprintf(out, "\t\t{%s}.Debug|Win32.Build.0 = Debug|Win32\r\n", tgt->get_UUID(VC2010_Project));
  fprintf(out, "\t\t{%s}.Release|Win32.ActiveCfg = Release|Win32\r\n", tgt->get_UUID(VC2010_Project));
  fprintf(out, "\t\t{%s}.Release|Win32.Build.0 = Release|Win32\r\n", tgt->get_UUID(VC2010_Project));
  return 0;
}


static int write_sln_project(FILE *out, Fl_Workspace_Type *workspace, Fl_Target_Type *tgt) {
  fprintf(out, "Project(\"{%s}\") = \"%s\", \"%s.vcxproj\", \"{%s}\"\r\n", workspace->get_UUID(VC2010_Workspace), tgt->name(), tgt->name(), tgt->get_UUID(VC2010_Project));
  // dependencies can be found in the project files
  fprintf(out, "EndProject\r\n");
  return 0;
}


/*
 Write a VisualC 2010 Solution file.
 */
static int write_sln_file(FILE *out, Fl_Workspace_Type *workspace) {
  
  fprintf(out, "Microsoft Visual Studio Solution File, Format Version 11.00\r\n");
  fprintf(out, "# Visual C++ Express 2010\r\n");
  
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_VC2010)) {
      write_sln_project(out, workspace, tgt);
    }
  }
  
  fprintf(out, "Global\r\n");
  
  fprintf(out, "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\r\n");
  fprintf(out, "\t\tDebug|Win32 = Debug|Win32\r\n");
  fprintf(out, "\t\tRelease|Win32 = Release|Win32\r\n");
  fprintf(out, "\tEndGlobalSection\r\n");
  
  fprintf(out, "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\r\n");
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_VC2010)) {
      write_sln_configuration(out, workspace, tgt);
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
 Create the list of linked libraries.
 */
static int write_additional_dependencies(FILE *out, Fl_Target_Type *tgt) {
  for (Fl_Type *t = tgt->next; t && (t->level>tgt->level); t = t->next) {
    if (t->is_tool() && ((Fl_Tool_Type*)t)->builds_in(FL_ENV_VC2008)) {
      if (t->is_file() && ((Fl_File_Type*)t)->file_is_library()) {
        fprintf(out, "%s;", t->name());
      } else if (t->is_target_dependency() && tgt->is_app_target()) {
        Fl_Target_Type *dep = Fl_Target_Type::find(t->name());
        if (dep && dep->is_lib_target()) {
          fprintf(out, "%s.lib;", t->name());
        }
      }
    }
  }
  return 0;
}


static int write_configuration(FILE *out, Fl_Target_Type *tgt, char is_debug) {
  // some definitions we will need later
  const char *cfg, *pre, *pre2;
  int opt, lib, cfg_type;
  char is_lib, cfg_path[256];
  
  if (is_debug) {
    cfg = "Debug";  // this is the name of the configuration as well as the path for all temporary files
    pre = "_DEBUG"; // this is a preprocessor definition for compile time configuration
    opt = 0;        // this is the level of optimization
    lib = 3;        // this is the link library (not sure what it actually does...)
    sprintf(cfg_path, "%s%s", tgt->name(), "_debug");
  } else {
    cfg = "Release";
    pre = "NDEBUG";
    opt = 4;
    lib = 2;
    sprintf(cfg_path, "%s%s", tgt->name(), "_release");
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
  
  fprintf(out, "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='%s|Win32'\">\r\n", cfg);
  if (!is_lib) {
    fprintf(out, "    <Midl>\r\n");
    fprintf(out, "      <PreprocessorDefinitions>%s;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n", pre);
    fprintf(out, "      <MkTypLibCompatible>true</MkTypLibCompatible>\r\n");
    fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
    fprintf(out, "      <TargetEnvironment>Win32</TargetEnvironment>\r\n");
    fprintf(out, "      <TypeLibraryName>.\\%s\\%s.tlb</TypeLibraryName>\r\n", cfg_path, tgt->name());
    fprintf(out, "      <HeaderFileName>\r\n");
    fprintf(out, "      </HeaderFileName>\r\n");
    fprintf(out, "    </Midl>\r\n");
  }
  fprintf(out, "    <ClCompile>\r\n");
  fprintf(out, "      <Optimization>Disabled</Optimization>\r\n");
  if (!is_debug) {
    fprintf(out, "      <InlineFunctionExpansion>AnySuitable</InlineFunctionExpansion>\r\n");
    fprintf(out, "      <FavorSizeOrSpeed>Size</FavorSizeOrSpeed>\r\n");
  }
  fprintf(out, "      <AdditionalIncludeDirectories>.;..\\..\\zlib;..\\..\\png;..\\..\\jpeg;..\\..;%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>_CRT_SECURE_NO_DEPRECATE;WIN32;%s;%s_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n", pre, pre2);
  if (is_debug) {
    fprintf(out, "      <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>\r\n");
  } else {
    fprintf(out, "      <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>\r\n");
  }
  fprintf(out, "      <PrecompiledHeader>\r\n");
  fprintf(out, "      </PrecompiledHeader>\r\n");
  if (is_lib) fprintf(out, "<BrowseInformation>true</BrowseInformation>\r\n");
  if (!is_debug) {
    fprintf(out, "      <AssemblerListingLocation>$(IntDir)</AssemblerListingLocation>\r\n");
    fprintf(out, "      <ObjectFileName>$(IntDir)</ObjectFileName>\r\n");
    fprintf(out, "      <ProgramDataBaseFileName>$(IntDir)vc$(PlatformToolsetVersion).pdb</ProgramDataBaseFileName>\r\n");
  }
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  if (is_debug) {
    fprintf(out, "      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>\r\n");
  }
  fprintf(out, "      <CompileAs>Default</CompileAs>\r\n");
  fprintf(out, "    </ClCompile>\r\n");
  
  fprintf(out, "    <ResourceCompile>\r\n");
  if (!is_lib) fprintf(out, "      <PreprocessorDefinitions>%s;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n", pre);
  fprintf(out, "      <Culture>0x0409</Culture>\r\n");
  fprintf(out, "    </ResourceCompile>\r\n");

  if (is_lib) {
    fprintf(out, "    <Lib>\r\n");
    fprintf(out, "    <OutputFile>..\\..\\%s\\$(ProjectName).lib</OutputFile>\r\n", DOS_path(tgt->target_path()));
    fprintf(out, "    <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
    fprintf(out, "    </Lib>\r\n");
  } else {
    fprintf(out, "    <Link>\r\n");
    fprintf(out, "      <AdditionalDependencies>");
    write_additional_dependencies(out, tgt);
    fprintf(out, "comctl32.lib;%%(AdditionalDependencies)</AdditionalDependencies>\r\n");
    fprintf(out, "      <OutputFile>..\\..\\%s\\%s.exe</OutputFile>\r\n", DOS_path(tgt->target_path()), tgt->name());
    fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
    fprintf(out, "      <AdditionalLibraryDirectories>..\\..\\lib;%%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\r\n");
    fprintf(out, "      <IgnoreSpecificDefaultLibraries>libcd;%%(IgnoreSpecificDefaultLibraries)</IgnoreSpecificDefaultLibraries>\r\n");
    fprintf(out, "      <GenerateDebugInformation>true</GenerateDebugInformation>\r\n");
    fprintf(out, "      <SubSystem>Windows</SubSystem>\r\n");
    fprintf(out, "      <RandomizedBaseAddress>false</RandomizedBaseAddress>\r\n");
    fprintf(out, "      <DataExecutionPrevention>\r\n");
    fprintf(out, "      </DataExecutionPrevention>\r\n");
    fprintf(out, "      <TargetMachine>MachineX86</TargetMachine>\r\n");
    if (is_debug) {
      fprintf(out, "      <ProgramDatabaseFile>$(IntDir)$(TargetName).pdb</ProgramDatabaseFile>\r\n");
    }
    fprintf(out, "    </Link>\r\n");
  }
  
  fprintf(out, "  </ItemDefinitionGroup>\r\n");
  
  return 0;
}


static int write_source_file(FILE *out, Fl_File_Type *file, const char *filename=0) {
  if (!filename) filename = file->filename();
  fprintf(out, "    <ClCompile Include=\"..\\..\\%s\">\r\n", DOS_path(filename));
  fprintf(out, "      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Disabled</Optimization>\r\n");
  fprintf(out, "      <AdditionalIncludeDirectories Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
  fprintf(out, "      <PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <AdditionalIncludeDirectories Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
  fprintf(out, "      <PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "    </ClCompile>\r\n");
  return 0;
}


static int write_fluid_source_file(FILE *out, Fl_File_Type *file) {
  char cxxName[2048];
  strcpy(cxxName, file->filename());
  fltk3::filename_setext(cxxName, 2048, ".cxx");  
  return write_source_file(out, file, cxxName);
}
 

static int write_fluid_custom_build(FILE *out, Fl_File_Type *file) {
  char flPath[2048];
  strcpy(flPath, file->filename());
  *((char*)fltk3::filename_name(flPath)) = 0; // keep only the path  

  //fprintf(out, "  <ItemGroup>\r\n");
  fprintf(out, "    <CustomBuild Include=\"..\\..\\%s\">\r\n", DOS_path(file->filename()));
  fprintf(out, "      <Message Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Create .cxx and .h file with fluid</Message>\r\n");
  fprintf(out, "      <Command Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">cfluid %%(Filename).fl\r\n");
  fprintf(out, "      </Command>\r\n");
  fprintf(out, "      <Outputs Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">..\\..\\%s\\%%(Filename).cxx;%%(Outputs)</Outputs>\r\n", DOS_path(flPath));
  fprintf(out, "      <Message Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">Create .cxx and .h file with fluid</Message>\r\n");
  fprintf(out, "      <Command Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">cfluid %%(Filename).fl\r\n");
  fprintf(out, "      </Command>\r\n");
  fprintf(out, "      <Outputs Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">..\\..\\%s\\%%(Filename).cxx;%%(Outputs)</Outputs>\r\n", DOS_path(flPath));
  fprintf(out, "    </CustomBuild>\r\n");
  //fprintf(out, "  </ItemGroup>\r\n");
  
  return 0;
}


/*
 Write a VisualC 2010 Project file.
 */
static int write_vcxproj_file(FILE *out, Fl_Target_Type *tgt) {
  const char *type;
  char is_lib;
  
  if (tgt->is_lib_target()) {
    is_lib = 1;
    type = "StaticLibrary";
  } else {
    is_lib = 0;
    type = "Application";
  }
  
  fprintf(out, "\357\273\277<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
  
  fprintf(out, "<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n");
  
  fprintf(out, "  <ItemGroup Label=\"ProjectConfigurations\">\r\n");
  fprintf(out, "    <ProjectConfiguration Include=\"Debug|Win32\">\r\n");
  fprintf(out, "      <Configuration>Debug</Configuration>\r\n");
  fprintf(out, "      <Platform>Win32</Platform>\r\n");
  fprintf(out, "    </ProjectConfiguration>\r\n");
  fprintf(out, "    <ProjectConfiguration Include=\"Release|Win32\">\r\n");
  fprintf(out, "      <Configuration>Release</Configuration>\r\n");
  fprintf(out, "      <Platform>Win32</Platform>\r\n");
  fprintf(out, "    </ProjectConfiguration>\r\n");
  fprintf(out, "  </ItemGroup>\r\n");
  
  fprintf(out, "  <PropertyGroup Label=\"Globals\">\r\n");
  if (is_lib) fprintf(out, "    <ProjectName>%s</ProjectName>\r\n", tgt->name());
  fprintf(out, "    <ProjectGuid>{%s}</ProjectGuid>\r\n", tgt->get_UUID(VC2010_Project));
  if (is_lib) fprintf(out, "    <RootNamespace>%s</RootNamespace>\r\n", tgt->name());
  fprintf(out, "  </PropertyGroup>\r\n");
  //  
  fprintf(out, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\r\n");
  
  fprintf(out, "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">\r\n");
  fprintf(out, "    <ConfigurationType>%s</ConfigurationType>\r\n", type);
  fprintf(out, "    <UseOfMfc>false</UseOfMfc>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  
  fprintf(out, "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">\r\n");
  fprintf(out, "    <ConfigurationType>%s</ConfigurationType>\r\n", type);
  fprintf(out, "    <UseOfMfc>false</UseOfMfc>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  
  fprintf(out, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\r\n");
  fprintf(out, "  <ImportGroup Label=\"ExtensionSettings\">\r\n");
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
  fprintf(out, "    <OutDir Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">..\\..\\%s\\</OutDir>\r\n", DOS_path(tgt->target_path()));
  fprintf(out, "    <IntDir Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">.\\$(ProjectName)_debug\\</IntDir>\r\n");
  fprintf(out, "    <LinkIncremental Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</LinkIncremental>\r\n");
  fprintf(out, "    <OutDir Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">..\\..\\%s\\</OutDir>\r\n", DOS_path(tgt->target_path()));
  fprintf(out, "    <IntDir Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">.\\$(ProjectName)_release\\</IntDir>\r\n");
  fprintf(out, "    <LinkIncremental Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">false</LinkIncremental>\r\n");
  fprintf(out, "    <TargetName Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">$(ProjectName)d</TargetName>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  
  write_configuration(out, tgt, 1);
  write_configuration(out, tgt, 0);
  
  char has_custom_build = 0;
  
  fprintf(out, "  <ItemGroup>\r\n");
  for (Fl_File_Type *file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->builds_in(FL_ENV_VC2010)) {
      if (file->file_is_fluid_ui()) {
        write_fluid_source_file(out, file);
        has_custom_build = 1;
      } else if (file->file_is_code()) {
        write_source_file(out, file);
      }
    }
  }
  fprintf(out, "  </ItemGroup>\r\n");
  
  if (has_custom_build) {
    fprintf(out, "  <ItemGroup>\r\n");
    for (Fl_File_Type *file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
      if (file->builds_in(FL_ENV_VC2010)) {
        if (file->file_is_fluid_ui()) {
          write_fluid_custom_build(out, file);
          has_custom_build = 1;
        }
      }
    }
    fprintf(out, "  </ItemGroup>\r\n");
  }
  
  // make sure that a dependency exits. If not, don't write this block at all!
  Fl_Target_Dependency_Type *tgt_dep = Fl_Target_Dependency_Type::first_dependency(tgt);
  for ( ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
    if (tgt_dep->builds_in(FL_ENV_VC2010)) break;
  }
  if (tgt_dep) {
    fprintf(out, "  <ItemGroup>\r\n");
    for (/* already found the first one */ ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
      Fl_Target_Type *dep = Fl_Target_Type::find(tgt_dep->name());
      if (dep && tgt_dep->builds_in(FL_ENV_VC2010)) {
        fprintf(out, "    <ProjectReference Include=\"%s.vcxproj\">\r\n", dep->name());
        fprintf(out, "      <Project>{%s}</Project>\r\n", dep->get_UUID(VC2010_Project));
        fprintf(out, "      <ReferenceOutputAssembly>false</ReferenceOutputAssembly>\r\n");
        fprintf(out, "    </ProjectReference>\r\n");
      }
    }
    fprintf(out, "  </ItemGroup>\r\n");
  }
  
  fprintf(out, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\r\n");
  fprintf(out, "  <ImportGroup Label=\"ExtensionTargets\">\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  
  fprintf(out, "</Project>");
  
  return 0;
}



int write_fltk_ide_visualc2010() {

  Fl_Workspace_Type *workspace = (Fl_Workspace_Type*)Fl_Type::first;
  
  workspace = (Fl_Workspace_Type*)Fl_Type::first;
  
  char buf[2048], base_dir[2048], tgt_base[2048];
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  strcpy(tgt_base, base_dir);
  strcpy(buf, base_dir);
  
  // write workspace file (.sln)
  strcat(buf, "ide/VisualC2010/fltk.sln");
  // FIXME: use workspace->name();  
  FILE *out = fopen(buf, "wb");
  write_sln_file(out, workspace);
  fclose(out);
  
  // TODO: write fluid build rule into a file
  // cfluid.cmd 
  
  // write project files (.vcproj)
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_VC2010)) {
      //if (strcmp(tgt->name(), "adjuster")==0 || strcmp(tgt->name(), "fltk")==0 || strcmp(tgt->name(), "arc")==0) {
        sprintf(buf, "%side/VisualC2010/%s.vcxproj", base_dir, tgt->name());
        out = fopen(buf, "wb");
        write_vcxproj_file(out, tgt);
        fclose(out);
      //}
    }
  }
  
  return 0;  
}

//
// End of "$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $".
//
