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
#include <fltk3/filename.h>


extern const char *filename;
extern const char *DOS_path(const char *filename);


// ------------ VisualC 6 ------------------------------------------------------


/*
 Write a target entry for a VC6 workspace file.
 */
static int write_dsw_entry(FILE *out, Fl_Target_Type *tgt, const char *name) {
  fprintf(out, "Project: \"%s\"=\".\\%s.dsp\" - Package Owner=<4>\r\n\r\n", name, name);
  fprintf(out, "Package=<5>\r\n{{{\r\n}}}\r\n\r\n");
  fprintf(out, "Package=<4>\r\n{{{\r\n");
  
  Fl_Target_Dependency_Type *tgt_dep = Fl_Target_Dependency_Type::first_dependency(tgt);
  for ( ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
    if (tgt_dep->builds_in(FL_ENV_VC6)) {
      fprintf(out, "    Begin Project Dependency\r\n");
      fprintf(out, "    Project_Dep_Name %s\r\n", tgt_dep->name());
      fprintf(out, "    End Project Dependency\r\n");}
  }
  fprintf(out, "}}}\r\n\r\n");
  fprintf(out, "###############################################################################\r\n");
  fprintf(out, "\r\n");
  return 0;
}


/*
 Write a workspace fiel in VC6 format.
 */
static int write_dsw_file(FILE *out, Fl_Workspace_Type *workspace) {
  // file header
  fprintf(out, "Microsoft Developer Studio Workspace File, Format Version 6.00\r\n");
  fprintf(out, "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "###############################################################################\r\n");
  fprintf(out, "\r\n");
  // target entries
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_VC6)) {
      write_dsw_entry(out, tgt, tgt->name());
      if (tgt->is_lib_target()) {
        char buf[80];
        strcpy(buf, tgt->name());
        strcat(buf, "dll");
        write_dsw_entry(out, tgt, buf);
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
static int write_add_link32(FILE *out, Fl_Target_Type *tgt, char is_debug) {
  for (Fl_Type *t = tgt->next; t && (t->level>tgt->level); t = t->next) {
    if (t->is_tool() && ((Fl_Tool_Type*)t)->builds_in(FL_ENV_VC6)) {
      if (t->is_file() && ((Fl_File_Type*)t)->file_is_library()) {
        fprintf(out, "%s ", t->name());
      } else if (t->is_target_dependency() && tgt->is_app_target()) {
        Fl_Target_Type *dep = Fl_Target_Type::find(t->name());
        if (dep && dep->is_lib_target()) {
          if (is_debug)
            fprintf(out, "%sd.lib ", t->name());
          else
            fprintf(out, "%s.lib ", t->name());
        }
      }
    }
  }
  return 0;
}


/*
 Write all the source file dependencies for a VC6 project file.
 
 This also generates the build rules for .fl files.
 */
static int write_source_files(FILE *out, Fl_Target_Type *tgt) {
  for (Fl_File_Type *file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->builds_in(FL_ENV_VC6)) {
      if (file->file_is_fluid_ui()) {
        char cxxName[2048], flPath[2048];
        strcpy(cxxName, file->filename());
        fltk3::filename_setext(cxxName, 2048, ".cxx");
        strcpy(flPath, file->filename());
        *((char*)fltk3::filename_name(flPath)) = 0; // keep only the path
        fprintf(out, "# Begin Source File\r\n");
        fprintf(out, "\r\n");
        fprintf(out, "SOURCE=..\\..\\%s\r\n", DOS_path(cxxName));
        fprintf(out, "# End Source File\r\n");
        fprintf(out, "# Begin Source File\r\n");
        fprintf(out, "\r\n");
        fprintf(out, "SOURCE=..\\..\\%s\r\n", DOS_path(file->filename()));
        fprintf(out, "\r\n");
        fprintf(out, "!IF  \"$(CFG)\" == \"%s - Win32 Release\"\r\n", tgt->name());
        fprintf(out, "\r\n");
        fprintf(out, "# Begin Custom Build - Create .cxx and .h file with fluid\r\n");
        fprintf(out, "InputPath=..\\..\\%s\r\n", DOS_path(file->filename()));
        fprintf(out, "\r\n");
        fprintf(out, "\"..\\..\\%s\" : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\r\n", DOS_path(cxxName));
        fprintf(out, "\tpushd ..\\..\\%s \r\n", DOS_path(flPath));
        fprintf(out, "\t..\\fluid\\fluid -c %s\r\n", file->filename_name());
        fprintf(out, "\tpopd \r\n");
        fprintf(out, "\t\r\n");
        fprintf(out, "# End Custom Build\r\n");
        fprintf(out, "\r\n");
        fprintf(out, "!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\r\n", tgt->name());
        fprintf(out, "\r\n");
        fprintf(out, "# Begin Custom Build - Create .cxx and .h file with fluidd\r\n");
        fprintf(out, "InputPath=..\\..\\%s\r\n", DOS_path(file->filename()));
        fprintf(out, "\r\n");
        fprintf(out, "\"..\\..\\%s\" : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\r\n", DOS_path(cxxName));
        fprintf(out, "\tpushd ..\\..\\%s \r\n", DOS_path(flPath));
        fprintf(out, "\t..\\fluid\\fluidd -c %s \r\n", file->filename_name());
        fprintf(out, "\tpopd \r\n");
        fprintf(out, "\t\r\n");
        fprintf(out, "# End Custom Build\r\n");
        fprintf(out, "\r\n");
        fprintf(out, "!ENDIF \r\n");
        fprintf(out, "\r\n");
        fprintf(out, "# End Source File\r\n");
      } else if (file->file_is_code()) {
        fprintf(out, "# Begin Source File\r\n");
        fprintf(out, "\r\n");
        fprintf(out, "SOURCE=..\\..\\%s\r\n", DOS_path(file->filename()));
        fprintf(out, "# End Source File\r\n");
      }
    }
  }
  return 0;
}


/*
 Write an static library project description for VisualC 6.
 
 This function outputs a .dsp file for VC6. This function sets the specific flags
 for generating a static library.
 */
static int write_dsp_file_for_lib(FILE *out, Fl_Target_Type *tgt) {

  fprintf(out, "# Microsoft Developer Studio Project File - Name=\"%s\" - Package Owner=<4>\r\n", tgt->name());
  fprintf(out, "# Microsoft Developer Studio Generated Build File, Format Version 6.00\r\n");
  fprintf(out, "# ** DO NOT EDIT **\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# TARGTYPE \"Win32 (x86) Static Library\" 0x0104\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "CFG=%s - Win32 Debug\r\n", tgt->name());
  fprintf(out, "!MESSAGE This is not a valid makefile. To build this project using NMAKE,\r\n");
  fprintf(out, "!MESSAGE use the Export Makefile command and run\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE NMAKE /f \"%s.mak\".\r\n", tgt->name());
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE You can specify a configuration when running NMAKE\r\n");
  fprintf(out, "!MESSAGE by defining the macro CFG on the command line. For example:\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE NMAKE /f \"%s.mak\" CFG=\"%s - Win32 Debug\"\r\n", tgt->name(), tgt->name());
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE Possible choices for configuration are:\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE \"%s - Win32 Release\" (based on \"Win32 (x86) Static Library\")\r\n", tgt->name());
  fprintf(out, "!MESSAGE \"%s - Win32 Debug\" (based on \"Win32 (x86) Static Library\")\r\n", tgt->name());
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Begin Project\r\n");
  fprintf(out, "# PROP AllowPerConfigDependencies 0\r\n");
  fprintf(out, "# PROP Scc_ProjName \"\"\r\n");
  fprintf(out, "# PROP Scc_LocalPath \"\"\r\n");
  fprintf(out, "CPP=cl.exe\r\n");
  fprintf(out, "RSC=rc.exe\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "!IF  \"$(CFG)\" == \"%s - Win32 Release\"\r\n", tgt->name());
  fprintf(out, "\r\n");
  fprintf(out, "# PROP BASE Use_MFC 0\r\n");
  fprintf(out, "# PROP BASE Use_Debug_Libraries 0\r\n");
  fprintf(out, "# PROP BASE Output_Dir \"Release\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Intermediate_Dir \"Release\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Target_Dir \"\"\r\n");
  fprintf(out, "# PROP Use_MFC 0\r\n");
  fprintf(out, "# PROP Use_Debug_Libraries 0\r\n");
  fprintf(out, "# PROP Output_Dir \"Release\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Intermediate_Dir \"Release\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Target_Dir \"\"\r\n");
  fprintf(out, "# ADD BASE CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"FL_LIBRARY\" /D \"NDEBUG\" /D \"_WINDOWS\" /YX /FD /c\r\n");
  fprintf(out, "# ADD CPP /nologo /MD /GX /Ot /Op /Ob2 /I \".\" /I \"..\\..\\include\" /I \"..\\..\\include\\fltk3zlib\" /I \"..\\..\\include\\fltk3png\" /I \"..\\..\\include\\fltk3jpeg\" /D \"WIN32\" /D \"FL_LIBRARY\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"_CRT_SECURE_NO_DEPRECATE\" /D \"_CRT_NONSTDC_NO_DEPRECATE\" /D \"WIN32_LEAN_AND_MEAN\" /D \"VC_EXTRA_LEAN\" /D \"WIN32_EXTRA_LEAN\" /YX /FD /c\r\n");
  fprintf(out, "# SUBTRACT CPP /Os\r\n");
  fprintf(out, "# ADD BASE RSC /l 0x409\r\n");
  fprintf(out, "# ADD RSC /l 0x409\r\n");
  fprintf(out, "BSC32=bscmake.exe\r\n");
  fprintf(out, "# ADD BASE BSC32 /nologo\r\n");
  fprintf(out, "# ADD BSC32 /nologo\r\n");
  fprintf(out, "LIB32=link.exe -lib\r\n");
  fprintf(out, "# ADD BASE LIB32 /nologo\r\n");
  fprintf(out, "# ADD LIB32 /nologo /out:\"..\\..\\%s\\%s.lib\"\r\n", DOS_path(tgt->target_path()), tgt->name());
  fprintf(out, "\r\n");
  fprintf(out, "!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\r\n", tgt->name());
  fprintf(out, "\r\n");
  fprintf(out, "# PROP BASE Use_MFC 0\r\n");
  fprintf(out, "# PROP BASE Use_Debug_Libraries 1\r\n");
  fprintf(out, "# PROP BASE Output_Dir \"Debug\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Intermediate_Dir \"Debug\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Target_Dir \"\"\r\n");
  fprintf(out, "# PROP Use_MFC 0\r\n");
  fprintf(out, "# PROP Use_Debug_Libraries 1\r\n");
  fprintf(out, "# PROP Output_Dir \"Debug\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Intermediate_Dir \"Debug\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Target_Dir \"\"\r\n");
  fprintf(out, "# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D \"WIN32\" /D \"FL_LIBRARY\" /D \"_DEBUG\" /D \"_WINDOWS\" /YX /FD /c\r\n");
  fprintf(out, "# ADD CPP /nologo /MDd /GX /Z7 /Od /I \".\" /I \"..\\..\\include\" /I \"..\\..\\include\\fltk3zlib\" /I \"..\\..\\include\\fltk3png\" /I \"..\\..\\include\\fltk3jpeg\" /D \"WIN32\" /D \"FL_LIBRARY\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"_CRT_SECURE_NO_DEPRECATE\" /D \"_CRT_NONSTDC_NO_DEPRECATE\" /D \"WIN32_LEAN_AND_MEAN\" /D \"VC_EXTRA_LEAN\" /D \"WIN32_EXTRA_LEAN\" /FR /YX /FD /c\r\n");
  fprintf(out, "# ADD BASE RSC /l 0x409\r\n");
  fprintf(out, "# ADD RSC /l 0x409\r\n");
  fprintf(out, "BSC32=bscmake.exe\r\n");
  fprintf(out, "# ADD BASE BSC32 /nologo\r\n");
  fprintf(out, "# ADD BSC32 /nologo\r\n");
  fprintf(out, "LIB32=link.exe -lib\r\n");
  fprintf(out, "# ADD BASE LIB32 /nologo\r\n");
  fprintf(out, "# ADD LIB32 /nologo /out:\"..\\..\\%s\\%sd.lib\"\r\n", DOS_path(tgt->target_path()), tgt->name());
  fprintf(out, "\r\n");
  fprintf(out, "!ENDIF \r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Begin Target\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Name \"%s - Win32 Release\"\r\n", tgt->name());
  fprintf(out, "# Name \"%s - Win32 Debug\"\r\n", tgt->name());
  
  write_source_files(out, tgt);
  
  fprintf(out, "# End Target\r\n");
  fprintf(out, "# End Project\r\n");
   
  return 0;
}


/*
 Write an dll project description for VisualC 6.
 
 This function outputs a .dsp file for VC6. This function sets the specific flags
 for generating a dll. This is pretty much untested and looks wrong in some details.
 I would greatly appreciate if a VC6 user could test these files, as I no longer
 own VC6.
 */
static int write_dsp_file_for_dll(FILE *out, Fl_Target_Type *tgt) {

  fprintf(out, "# Microsoft Developer Studio Project File - Name=\"%sdll\" - Package Owner=<4>\r\n", tgt->name());
  fprintf(out, "# Microsoft Developer Studio Generated Build File, Format Version 6.00\r\n");
  fprintf(out, "# ** DO NOT EDIT **\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# TARGTYPE \"Win32 (x86) Dynamic-Link Library\" 0x0102\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "CFG=%sdll - Win32 Debug\r\n", tgt->name());
  fprintf(out, "!MESSAGE This is not a valid makefile. To build this project using NMAKE,\r\n");
  fprintf(out, "!MESSAGE use the Export Makefile command and run\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE NMAKE /f \"%sdll.mak\".\r\n", tgt->name());
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE You can specify a configuration when running NMAKE\r\n");
  fprintf(out, "!MESSAGE by defining the macro CFG on the command line. For example:\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE NMAKE /f \"%sdll.mak\" CFG=\"%sdll - Win32 Debug\"\r\n", tgt->name(), tgt->name());
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE Possible choices for configuration are:\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE \"%sdll - Win32 Release\" (based on \"Win32 (x86) Dynamic-Link Library\")\r\n", tgt->name());
  fprintf(out, "!MESSAGE \"%sdll - Win32 Debug\" (based on \"Win32 (x86) Dynamic-Link Library\")\r\n", tgt->name());
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
  fprintf(out, "!IF  \"$(CFG)\" == \"%sdll - Win32 Release\"\r\n", tgt->name());
  fprintf(out, "\r\n");
  fprintf(out, "# PROP BASE Use_MFC 0\r\n");
  fprintf(out, "# PROP BASE Use_Debug_Libraries 0\r\n");
  fprintf(out, "# PROP BASE Output_Dir \"Release\\%sdll\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Intermediate_Dir \"Release\\%sdll\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Target_Dir \"\"\r\n");
  fprintf(out, "# PROP Use_MFC 0\r\n");
  fprintf(out, "# PROP Use_Debug_Libraries 0\r\n");
  fprintf(out, "# PROP Output_Dir \"Release\\%sdll\"\r\n", tgt->name());
  fprintf(out, "# PROP Intermediate_Dir \"Release\\%sdll\"\r\n", tgt->name());
  fprintf(out, "# PROP Ignore_Export_Lib 0\r\n");
  fprintf(out, "# PROP Target_Dir \"\"\r\n");
  fprintf(out, "# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /YX /FD /c\r\n");
  fprintf(out, "# ADD CPP /nologo /MD /W3 /GX /Os /Ob2 /I \"..\\..\\include\\fltk3zlib\" /I \"..\\..\\include\\fltk3png\" /I \"..\\..\\include\\fltk3jpeg\" /I \".\" /D \"FL_DLL\" /D \"FL_LIBRARY\" /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"_CRT_SECURE_NO_DEPRECATE\" /D \"_CRT_NONSTDC_NO_DEPRECATE\" /D \"WIN32_LEAN_AND_MEAN\" /D \"VC_EXTRA_LEAN\" /D \"WIN32_EXTRA_LEAN\" /YX /c\r\n");
  fprintf(out, "# ADD BASE MTL /nologo /D \"NDEBUG\" /mktyplib203 /o /win32 \"NUL\"\r\n");
  fprintf(out, "# ADD MTL /nologo /D \"NDEBUG\" /mktyplib203 /o /win32 \"NUL\"\r\n");
  fprintf(out, "# ADD BASE RSC /l 0x409 /d \"NDEBUG\"\r\n");
  fprintf(out, "# ADD RSC /l 0x409 /d \"NDEBUG\"\r\n");
  fprintf(out, "BSC32=bscmake.exe\r\n");
  fprintf(out, "# ADD BASE BSC32 /nologo\r\n");
  fprintf(out, "# ADD BSC32 /nologo\r\n");
  fprintf(out, "LINK32=link.exe\r\n");
  fprintf(out, "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386\r\n");
  
  fprintf(out, "# ADD LINK32 ");
  write_add_link32(out, tgt, 0);
  fprintf(out, "comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:\"%sdll.pdb\" /machine:I386\r\n", tgt->name());
  
  fprintf(out, "# SUBTRACT LINK32 /pdb:none\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "!ELSEIF  \"$(CFG)\" == \"%sdll - Win32 Debug\"\r\n", tgt->name());
  fprintf(out, "\r\n");
  fprintf(out, "# PROP BASE Use_MFC 0\r\n");
  fprintf(out, "# PROP BASE Use_Debug_Libraries 1\r\n");
  fprintf(out, "# PROP BASE Output_Dir \"Debug\\%sdll\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Intermediate_Dir \"Debug\\%sdll\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Target_Dir \"\"\r\n");
  fprintf(out, "# PROP Use_MFC 0\r\n");
  fprintf(out, "# PROP Use_Debug_Libraries 1\r\n");
  fprintf(out, "# PROP Output_Dir \"Debug\\%sdll\"\r\n", tgt->name());
  fprintf(out, "# PROP Intermediate_Dir \"Debug\\%sdll\"\r\n", tgt->name());
  fprintf(out, "# PROP Ignore_Export_Lib 0\r\n");
  fprintf(out, "# PROP Target_Dir \"\"\r\n");
  fprintf(out, "# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /YX /FD /c\r\n");
  fprintf(out, "# ADD CPP /nologo /MDd /GX /ZI /Od /I \".\" /I \"..\\..\\include\" /I \"..\\..\\include\\fltk3zlib\" /I \"..\\..\\include\\fltk3png\" /I \"..\\..\\include\\fltk3jpeg\" /D \"FL_DLL\" /D \"FL_LIBRARY\" /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"_CRT_SECURE_NO_DEPRECATE\" /D \"_CRT_NONSTDC_NO_DEPRECATE\" /D \"WIN32_LEAN_AND_MEAN\" /D \"VC_EXTRA_LEAN\" /D \"WIN32_EXTRA_LEAN\" /YX /c\r\n");
  fprintf(out, "# ADD BASE MTL /nologo /D \"_DEBUG\" /mktyplib203 /o /win32 \"NUL\"\r\n");
  fprintf(out, "# ADD MTL /nologo /D \"_DEBUG\" /mktyplib203 /o /win32 \"NUL\"\r\n");
  fprintf(out, "# ADD BASE RSC /l 0x409 /d \"_DEBUG\"\r\n");
  fprintf(out, "# ADD RSC /l 0x409 /d \"_DEBUG\"\r\n");
  fprintf(out, "BSC32=bscmake.exe\r\n");
  fprintf(out, "# ADD BASE BSC32 /nologo\r\n");
  fprintf(out, "# ADD BSC32 /nologo\r\n");
  fprintf(out, "LINK32=link.exe\r\n");
  fprintf(out, "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept\r\n");
  
  fprintf(out, "# ADD LINK32 ");
  write_add_link32(out, tgt, 1);
  fprintf(out, "comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:\"%sdlld.pdb\" /debug /machine:I386 /out:\"Debug\\%sdll\\%sdlld.dll\" /pdbtype:sept\r\n", tgt->name(), tgt->name(), tgt->name());
  
  fprintf(out, "# SUBTRACT LINK32 /pdb:none /incremental:no\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "!ENDIF \r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Begin Target\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Name \"%sdll - Win32 Release\"\r\n", tgt->name());
  fprintf(out, "# Name \"%sdll - Win32 Debug\"\r\n", tgt->name());
  
  write_source_files(out, tgt);
  
  fprintf(out, "# End Target\r\n");
  fprintf(out, "# End Project\r\n");
 
  return 0;
}


/*
 Write an application project description for VisualC 6.
 
 This function outputs a .dsp file for VC6. The files should also work with
 nmake from the command line, which is very useful once Fluid can build and 
 launch applications.
 */
static int write_dsp_file_for_app(FILE *out, Fl_Target_Type *tgt) {
  fprintf(out, "# Microsoft Developer Studio Project File - Name=\"%s\" - Package Owner=<4>\r\n", tgt->name());
  fprintf(out, "# Microsoft Developer Studio Generated Build File, Format Version 6.00\r\n");
  fprintf(out, "# ** DO NOT EDIT **\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# TARGTYPE \"Win32 (x86) Application\" 0x0101\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "CFG=%s - Win32 Debug\r\n", tgt->name());
  fprintf(out, "!MESSAGE This is not a valid makefile. To build this project using NMAKE,\r\n");
  fprintf(out, "!MESSAGE use the Export Makefile command and run\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE NMAKE /f \"%s.mak\".\r\n", tgt->name());
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE You can specify a configuration when running NMAKE\r\n");
  fprintf(out, "!MESSAGE by defining the macro CFG on the command line. For example:\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE NMAKE /f \"%s.mak\" CFG=\"%s - Win32 Debug\"\r\n", tgt->name(), tgt->name());
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE Possible choices for configuration are:\r\n");
  fprintf(out, "!MESSAGE \r\n");
  fprintf(out, "!MESSAGE \"%s - Win32 Release\" (based on \"Win32 (x86) Application\")\r\n", tgt->name());
  fprintf(out, "!MESSAGE \"%s - Win32 Debug\" (based on \"Win32 (x86) Application\")\r\n", tgt->name());
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
  fprintf(out, "!IF  \"$(CFG)\" == \"%s - Win32 Release\"\r\n", tgt->name());
  fprintf(out, "\r\n");
  fprintf(out, "# PROP BASE Use_MFC 0\r\n");
  fprintf(out, "# PROP BASE Use_Debug_Libraries 0\r\n");
  fprintf(out, "# PROP BASE Output_Dir \"Release\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Intermediate_Dir \"Release\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Target_Dir \"\"\r\n");
  fprintf(out, "# PROP Use_MFC 0\r\n");
  fprintf(out, "# PROP Use_Debug_Libraries 0\r\n");
  fprintf(out, "# PROP Output_Dir \"Release\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Intermediate_Dir \"Release\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Ignore_Export_Lib 0\r\n");
  fprintf(out, "# PROP Target_Dir \"\"\r\n");
  fprintf(out, "# ADD BASE CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /YX /FD /c\r\n");
  fprintf(out, "# ADD CPP /nologo /MD /GX /Os /Ob2 /I \".\" /I \"..\\..\\include\" /I \"..\\..\\include\\fltk3zlib\" /I \"..\\..\\include\\fltk3png\" /I \"..\\..\\include\\fltk3jpeg\" /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"_CRT_SECURE_NO_DEPRECATE\" /D \"_CRT_NONSTDC_NO_DEPRECATE\" /D \"WIN32_LEAN_AND_MEAN\" /D \"VC_EXTRA_LEAN\" /D \"WIN32_EXTRA_LEAN\" /YX /FD /c\r\n");
  fprintf(out, "# ADD BASE MTL /nologo /D \"NDEBUG\" /mktyplib203 /o \"NUL\" /win32\r\n");
  fprintf(out, "# ADD MTL /nologo /D \"NDEBUG\" /mktyplib203 /o \"NUL\" /win32\r\n");
  fprintf(out, "# ADD BASE RSC /l 0x409 /d \"NDEBUG\"\r\n");
  fprintf(out, "# ADD RSC /l 0x409 /d \"NDEBUG\"\r\n");
  fprintf(out, "BSC32=bscmake.exe\r\n");
  fprintf(out, "# ADD BASE BSC32 /nologo\r\n");
  fprintf(out, "# ADD BSC32 /nologo\r\n");
  fprintf(out, "LINK32=link.exe\r\n");
  fprintf(out, "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386\r\n");

  fprintf(out, "# ADD LINK32 ");
  write_add_link32(out, tgt, 0);
  fprintf(out, "comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:\"libcd\" /out:\"../../%s/%s.exe\" /pdbtype:sept /libpath:\"..\\..\\lib\"\r\n", DOS_path(tgt->target_path()), tgt->name());
  
  fprintf(out, "# SUBTRACT LINK32 /pdb:none /incremental:yes\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\r\n", tgt->name());
  fprintf(out, "\r\n");
  fprintf(out, "# PROP BASE Use_MFC 0\r\n");
  fprintf(out, "# PROP BASE Use_Debug_Libraries 1\r\n");
  fprintf(out, "# PROP BASE Output_Dir \"Debug\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Intermediate_Dir \"Debug\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Target_Dir \"\"\r\n");
  fprintf(out, "# PROP Use_MFC 0\r\n");
  fprintf(out, "# PROP Use_Debug_Libraries 1\r\n");
  fprintf(out, "# PROP Output_Dir \"Debug\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Intermediate_Dir \"Debug\\%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Ignore_Export_Lib 0\r\n");
  fprintf(out, "# PROP Target_Dir \"\"\r\n");
  fprintf(out, "# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /YX /FD /c\r\n");
  fprintf(out, "# ADD CPP /nologo /MDd /Gm /GX /ZI /Od /I \".\" /I \"..\\..\\include\" /I \"..\\..\\include\\fltk3zlib\" /I \"..\\..\\include\\fltk3png\" /I \"..\\..\\include\\fltk3jpeg\" /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"_CRT_SECURE_NO_DEPRECATE\" /D \"_CRT_NONSTDC_NO_DEPRECATE\" /D \"WIN32_LEAN_AND_MEAN\" /D \"VC_EXTRA_LEAN\" /D \"WIN32_EXTRA_LEAN\" /YX /FD /c\r\n");
  fprintf(out, "# ADD BASE MTL /nologo /D \"_DEBUG\" /mktyplib203 /o \"NUL\" /win32\r\n");
  fprintf(out, "# ADD MTL /nologo /D \"_DEBUG\" /mktyplib203 /o \"NUL\" /win32\r\n");
  fprintf(out, "# ADD BASE RSC /l 0x409 /d \"_DEBUG\"\r\n");
  fprintf(out, "# ADD RSC /l 0x409 /d \"_DEBUG\"\r\n");
  fprintf(out, "BSC32=bscmake.exe\r\n");
  fprintf(out, "# ADD BASE BSC32 /nologo\r\n");
  fprintf(out, "# ADD BSC32 /nologo\r\n");
  fprintf(out, "LINK32=link.exe\r\n");
  fprintf(out, "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept\r\n");
  
  fprintf(out, "# ADD LINK32 ");
  write_add_link32(out, tgt, 1);
  fprintf(out, "comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:\"libcd\" /out:\"../../%s/%sd.exe\" /pdbtype:sept /libpath:\"..\\..\\lib\"\r\n", DOS_path(tgt->target_path()), tgt->name());
  
  fprintf(out, "# SUBTRACT LINK32 /pdb:none /incremental:no\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "!ENDIF \r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Begin Target\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Name \"%s - Win32 Release\"\r\n", tgt->name());
  fprintf(out, "# Name \"%s - Win32 Debug\"\r\n", tgt->name());
  
  write_source_files(out, tgt);
// TODO: include header file if they are so configured
// TODO: create visual folders in the IDE if possible (surely is!)
  
  fprintf(out, "# End Target\r\n");
  fprintf(out, "# End Project\r\n");
  
  return 0;
}


/*
 Write all VisualC 6 IDE files.
 
 This module write all the files needed to create the VisualC 6 IDE. It generates
 an fltk.dsw file and all dsp files depending on it. All parameters are taken from
 the Fl_..Type hierarchy in Fluid. 
 
 FIXME: !!! Currently, this function is limited to writing the FLTK build system itself. !!!
 This module was tested with fltk.flw. Eventually, we will be writing universal
 new IDE setups as a convinience to FLTK/ FLUID users.
 
 */
int write_fltk_ide_visualc6() {
  
  Fl_Workspace_Type *workspace = (Fl_Workspace_Type*)Fl_Type::first;
  
  char buf[2048], base_dir[2048], tgt_base[2048];
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  strcpy(tgt_base, base_dir);
  strcpy(buf, base_dir);

  // write workspace file (.dsw)
  strcat(buf, "ide/VisualC6/fltk.dsw");
// FIXME: use workspace->name();  
  FILE *out = fopen(buf, "wb");
  write_dsw_file(out, workspace);
  fclose(out);
  
  // write project files (.dsp)
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_VC6)) {
      if (tgt->is_app_target()) {
        sprintf(buf, "%side/VisualC6/%s.dsp", base_dir, tgt->name());
        out = fopen(buf, "wb");
        write_dsp_file_for_app(out, tgt);
        fclose(out);
      } else if (tgt->is_lib_target()) {
        sprintf(buf, "%side/VisualC6/%s.dsp", base_dir, tgt->name());
        out = fopen(buf, "wb");
        write_dsp_file_for_lib(out, tgt);
        fclose(out);
        sprintf(buf, "%side/VisualC6/%sdll.dsp", base_dir, tgt->name());
        out = fopen(buf, "wb");
        write_dsp_file_for_dll(out, tgt);
        fclose(out);
      } else {
        printf("Internale error writing unknown VC6 target!\n");
      }
    }
  }
  return 0;
}


//
// End of "$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $".
//
