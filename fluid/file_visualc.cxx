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

int write_fltk_ide_visualc6() {
  if (!filename) {
    printf("Workspace must be saved first\n");
    return -1;
  }
  
  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Target_Type::find("Fluid");
  if (!tgt) {
    printf("FLUID target not found\n");
    return -1;
  }
  
  /* Create a new Fluid.dsp */
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "ide/VisualC6/Fluid.dsp");
  FILE *out = fopen(buf, "wb");
  if (!out) {
    printf("Can't open FLUID VisualC 6 project file\n");
    return -1;
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
  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    if (f->is_cplusplus_code()) {
      fprintf(out, "# Begin Source File\r\n");
      fprintf(out, "\r\n");
      fprintf(out, "SOURCE=..\\..\\%s\r\n", DOS_path(f->filename()));
      fprintf(out, "# End Source File\r\n");
    }
  }  
  
  fprintf(out, "# End Target\r\n");
  fprintf(out, "# End Project\r\n");
  
  fclose(out);
  
  return 0;
}

int write_fltk_ide_visualc2008() {
  if (!filename) {
    printf("Workspace must be saved first\n");
    return -1;
  }
  
  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Target_Type::find("Fluid");
  if (!tgt) {
    printf("FLUID target not found\n");
    return -1;
  }
  
  /* Create a new Fluid.dsp */
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "ide/VisualC2008/fluid.vcproj");
  FILE *out = fopen(buf, "wb");
  if (!out) {
    printf("Can't open FLUID VisualC 2008 project file\n");
    return -1;
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
  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    if (f->is_cplusplus_code()) {
      fprintf(out, "\t\t<File\r\n");
      fprintf(out, "\t\t\tRelativePath=\"..\\..\\%s\"\r\n", DOS_path(f->filename()));
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
  
  fprintf(out, "\t</Files>\r\n");
  fprintf(out, "\t<Globals>\r\n");
  fprintf(out, "\t</Globals>\r\n");
  fprintf(out, "</VisualStudioProject>\r\n");
  
  fclose(out);

  return 0;
}

int write_fltk_ide_visualc2010() {
  if (!filename) {
    printf("Workspace must be saved first\n");
    return -1;
  }
  
  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Target_Type::find("Fluid");
  if (!tgt) {
    printf("FLUID target not found\n");
    return -1;
  }
  
  /* Create a new Fluid.dsp */
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "ide/VisualC2010/fluid.vcxproj");
  FILE *out = fopen(buf, "wb");
  if (!out) {
    printf("Can't open FLUID VisualC 2010 project file\n");
    return -1;
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
  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    if (f->is_cplusplus_code()) {
      fprintf(out, "    <ClCompile Include=\"..\\..\\%s\">\r\n", DOS_path(f->filename()));
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
  
  return 0;
}


//
// End of "$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $".
//
