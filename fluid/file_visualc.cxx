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

// ------------ VisualC 6 ------------------------------------------------------

static int write_dsw_entry(FILE *out, Fl_Target_Type *tgt, const char *name) {
  fprintf(out, "Project: \"%s\"=\".\%s.dsp\" - Package Owner=<4>\r\n\r\n", tgt->name(), tgt->name());
  fprintf(out, "Package=<5>\r\n{{{\r\n}}}\r\n\r\n");
  fprintf(out, "Package=<4>\r\n{{{\r\n");
  
  Fl_Target_Dependency_Type *tgt_dep = Fl_Target_Dependency_Type::first_dependency(tgt);
  for ( ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
    fprintf(out, "    Begin Project Dependency\r\n");
    fprintf(out, "    Project_Dep_Name %s\r\n", tgt_dep->name());
    fprintf(out, "    End Project Dependency\r\n");
  }
  fprintf(out, "}}}\r\n\r\n");
  fprintf(out, "###############################################################################\r\n");
  fprintf(out, "\r\n");
  return 0;
}


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


static int write_dsp_file(FILE *out, Fl_Target_Type *tgt) {
  // FIXME: we are not generating code to build the dll's yet!  
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
  fprintf(out, "# PROP BASE Output_Dir \"Release/%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Intermediate_Dir \"Release/%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Target_Dir \"\"\r\n");
  fprintf(out, "# PROP Use_MFC 0\r\n");
  fprintf(out, "# PROP Use_Debug_Libraries 0\r\n");
  fprintf(out, "# PROP Output_Dir \"Release/%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Intermediate_Dir \"Release/%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Ignore_Export_Lib 0\r\n");
  fprintf(out, "# PROP Target_Dir \"\"\r\n");
  fprintf(out, "# ADD BASE CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /YX /FD /c\r\n");
// TODO: add library dependencies here!
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

  fprintf(out, "# ADD LINK32 ");
  for (Fl_Type *t = tgt->next; t && (t->level>tgt->level); t = t->next) {
    if (t->is_tool() && ((Fl_Tool_Type*)t)->builds_in(FL_ENV_VC6)) {
      if (t->is_file() && ((Fl_File_Type*)t)->file_is_library()) {
        fprintf(out, "%s ", t->name());
      } else if (t->is_target_dependency()) {
        Fl_Target_Type *dep = Fl_Target_Type::find(t->name());
        if (dep && dep->is_lib_target()) {
          fprintf(out, "%s.lib ", t->name());
        }
      }
    }
  }
  fprintf(out, "comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:\"libcd\" /out:\"../../test/%s.exe\" /pdbtype:sept /libpath:\"..\\..\\lib\"\r\n", tgt->name());
  
  fprintf(out, "# SUBTRACT LINK32 /pdb:none /incremental:yes\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\r\n", tgt->name());
  fprintf(out, "\r\n");
  fprintf(out, "# PROP BASE Use_MFC 0\r\n");
  fprintf(out, "# PROP BASE Use_Debug_Libraries 1\r\n");
  fprintf(out, "# PROP BASE Output_Dir \"Debug/%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Intermediate_Dir \"Debug/%s\"\r\n", tgt->name());
  fprintf(out, "# PROP BASE Target_Dir \"\"\r\n");
  fprintf(out, "# PROP Use_MFC 0\r\n");
  fprintf(out, "# PROP Use_Debug_Libraries 1\r\n");
  fprintf(out, "# PROP Output_Dir \"Debug/%s\"\r\n", tgt->name());
  fprintf(out, "# PROP Intermediate_Dir \"Debug/%s\"\r\n", tgt->name());
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
  
  fprintf(out, "# ADD LINK32 ");
  for (Fl_Type *t = tgt->next; t && (t->level>tgt->level); t = t->next) {
    if (t->is_tool() && ((Fl_Tool_Type*)t)->builds_in(FL_ENV_VC6)) {
      if (t->is_file() && ((Fl_File_Type*)t)->file_is_library()) {
        fprintf(out, "%s ", t->name());
      } else if (t->is_target_dependency()) {
        Fl_Target_Type *dep = Fl_Target_Type::find(t->name());
        if (dep && dep->is_lib_target()) {
          fprintf(out, "%sd.lib ", t->name());
        }
      }
    }
  }
  // FIXME: output path must not be "test"
  fprintf(out, "comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:\"libcd\" /out:\"../../test/%sd.exe\" /pdbtype:sept /libpath:\"..\\..\\lib\"\r\n", tgt->name());
  
  fprintf(out, "# SUBTRACT LINK32 /pdb:none /incremental:no\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "!ENDIF \r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Begin Target\r\n");
  fprintf(out, "\r\n");
  fprintf(out, "# Name \"%s - Win32 Release\"\r\n", tgt->name());
  fprintf(out, "# Name \"%s - Win32 Debug\"\r\n", tgt->name());
  
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
  
  fprintf(out, "# End Target\r\n");
  fprintf(out, "# End Project\r\n");
  
  return 0;
}


int write_fltk_ide_visualc6() {
  
  Fl_Workspace_Type *workspace = (Fl_Workspace_Type*)Fl_Type::first;
  
  workspace = (Fl_Workspace_Type*)Fl_Type::first;
  
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
      sprintf(buf, "%side/VisualC6/%s.dsp", base_dir, tgt->name());
      out = fopen(buf, "wb");
      write_dsp_file(out, tgt);
      fclose(out);
    }
  }
  
  //+++
  // TODO: write static and dynamic library project files
  // The code below fixes Fluid and fltk.lib for now.
  
  // for now, we use a template file in FLTK/ide/templates/VisualC6.tmpl .
  // When done, everything will likely be integrated into the executable to make one compact package.
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  strcpy(tgt_base, base_dir);
  strcpy(buf, base_dir);
  strcat(buf, "ide/templates/VisualC6.tmpl");
  out = stdout;
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
        } else if (strncmp(hash, "#SourceFiles(", 13)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+13, ')'); // keep tgt local
          if (!tgt) {
            printf("ERROR writing VisualC6 file: target not found!");
            return -1;
          }
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->file_is_code() && f->builds_in(FL_ENV_VC6)) {
              fprintf(out, "# Begin Source File\r\n");
              fprintf(out, "\r\n");
              fprintf(out, "SOURCE=..\\..\\%s\r\n", DOS_path(f->filename()));
              fprintf(out, "# End Source File\r\n");
            }
          }
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

// ------------ VisualC 2008 ---------------------------------------------------

int write_fltk_ide_visualc2008() {
  // for now, we use a template file in FLTK/ide/templates/VisualC2008.tmpl .
  // When done, everything will likely be integrated into the executable to make one compact package.
  char buf[2048], base_dir[2048], tgt_base[2048];
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  strcpy(tgt_base, base_dir);
  strcpy(buf, base_dir);
  strcat(buf, "ide/templates/VisualC2008.tmpl");
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
        } else if (strncmp(hash, "#SourceFiles(", 13)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+13, ')'); // keep tgt local
          if (!tgt) {
            printf("ERROR writing VisualC 2008 file: target not found!");
            return -1;
          }
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->file_is_code() && f->builds_in(FL_ENV_VC2008)) {
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
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#HeaderFiles(", 13)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+13, ')'); // keep tgt local
          if (!tgt) {
            printf("ERROR writing VisualC 2008 file: target not found!");
            return -1;
          }
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->file_is_header() && f->lists_in(FL_ENV_VC2008)) {
              fprintf(out, "\t\t\t<File\r\n");
              fprintf(out, "\t\t\t\tRelativePath=\"..\\..\\%s\"\r\n", DOS_path(f->filename()));
              fprintf(out, "\t\t\t\t>\r\n");
              // OK, we have cheated here quite a bit. The stuff missing here (as opposed to "#SourceFiles"
              // depends on "builds_in", and not on the fact of being a header file. But for now it does
              // what we expect...
              fprintf(out, "\t\t\t</File>\r\n");                        
            }
          }
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

// ------------ VisualC 2010 ---------------------------------------------------

int write_fltk_ide_visualc2010() {
  // for now, we use a template file in FLTK/ide/templates/VisualC2010.tmpl .
  // When done, everything will likely be integrated into the executable to make one compact package.
  char buf[2048], base_dir[2048], tgt_base[2048];
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  strcpy(tgt_base, base_dir);
  strcpy(buf, base_dir);
  strcat(buf, "ide/templates/VisualC2010.tmpl");
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
        } else if (strncmp(hash, "#SourceFiles(", 13)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+13, ')'); // keep tgt local
          if (!tgt) {
            printf("ERROR writing VisualC 2010 file: target not found!");
            return -1;
          }
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->file_is_code() && f->builds_in(FL_ENV_VC2010)) {
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
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#SourceFilesFilters(", 20)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+20, ')'); // keep tgt local
          if (!tgt) {
            printf("ERROR writing VisualC 2010 file: target not found!");
            return -1;
          }
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->file_is_code() && f->lists_in(FL_ENV_VC2010)) {
              fprintf(out, "    <ClInclude Include=\"..\\..\\%s\" />\r\n", DOS_path(f->filename()));
            }
          }
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#HeaderFiles(", 13)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+13, ')'); // keep tgt local
          if (!tgt) {
            printf("ERROR writing VisualC 2010 file: target not found!");
            return -1;
          }
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->file_is_header() && f->lists_in(FL_ENV_VC2010)) {
              fprintf(out, "    <ClInclude Include=\"..\\..\\%s\" />\r\n", DOS_path(f->filename()));
            }
          }
          hash = strchr(hash, ';')+1;
        } else if (strncmp(hash, "#HeaderFilesFilters(", 20)==0) {
          Fl_Type *tgt = Fl_Target_Type::find(hash+20, ')'); // keep tgt local
          if (!tgt) {
            printf("ERROR writing VisualC 2010 file: target not found!");
            return -1;
          }
          Fl_File_Type *f;
          for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
            if (f->file_is_header() && f->lists_in(FL_ENV_VC2010)) {
              fprintf(out, "    <ClInclude Include=\"..\\..\\%s\">\r\n", DOS_path(f->filename()));
              fprintf(out, "      <Filter>Headers</Filter>\r\n");
              fprintf(out, "    </ClInclude>\r\n");
            }
          }
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
