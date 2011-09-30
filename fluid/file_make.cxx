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
#include <ctype.h>
#include "Fl_Type.h"
#include <fltk3/filename.h>

// ------------ file conversion ------------------------------------------------

extern const char *filename;

static int write_file(FILE *out, Fl_File_Type *file, const char *src_path, const char *tgt_path, const char *ext=0L) {
  char src[2048], tgt[2048];
  strcpy(src, src_path); strcat(src, file->filename());
  fltk3::filename_relative(tgt, 2048, src, tgt_path);
  if (ext) fltk3::filename_setext(tgt, 2048, ext);
  fprintf(out, "%s", tgt);
  return 0;
}

char target_has_cplusplus_code(Fl_Target_Type *tgt) {
  for (Fl_File_Type *file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->builds_in(FL_ENV_MAKE) && file->file_is_cplusplus_code())
      return 1;
  }
  return 0;
}

char target_has_c_code(Fl_Target_Type *tgt) {
  for (Fl_File_Type *file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->builds_in(FL_ENV_MAKE) && file->file_is_c_code())
      return 1;
  }
  return 0;
}

char target_has_objc_code(Fl_Target_Type *tgt) {
  for (Fl_File_Type *file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->builds_in(FL_ENV_MAKE) && file->file_is_objc_code())
      return 1;
  }
  return 0;
}

char target_links_with(Fl_Target_Type *tgt, const char *name) {
  for (Fl_Target_Dependency_Type *dep = Fl_Target_Dependency_Type::first_dependency(tgt); dep; dep = dep->next_dependency(tgt)) {
    if (dep->builds_in(FL_ENV_MAKE) && strcmp(dep->name(), name)==0)
      return 1;
  }
  return 0;
}

static int write_header(FILE *out, const char *msg, const char *rem="#") {
  fprintf(out, "%s\n", rem);
  fprintf(out, "%s \"$Id$\"\n", rem);
  fprintf(out, "%s\n", rem);
  fprintf(out, "%s %s\n", rem, msg);
  fprintf(out, "%s\n", rem);
  fprintf(out, "%s Copyright 1998-2011 by Bill Spitzak and others.\n", rem);
  fprintf(out, "%s\n", rem);
  fprintf(out, "%s This library is free software; you can redistribute it and/or\n", rem);
  fprintf(out, "%s modify it under the terms of the GNU Library General Public\n", rem);
  fprintf(out, "%s License as published by the Free Software Foundation; either\n", rem);
  fprintf(out, "%s version 2 of the License, or (at your option) any later version.\n", rem);
  fprintf(out, "%s\n", rem);
  fprintf(out, "%s This library is distributed in the hope that it will be useful,\n", rem);
  fprintf(out, "%s but WITHOUT ANY WARRANTY; without even the implied warranty of\n", rem);
  fprintf(out, "%s MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n", rem);
  fprintf(out, "%s Library General Public License for more details.\n", rem);
  fprintf(out, "%s\n", rem);
  fprintf(out, "%s You should have received a copy of the GNU Library General Public\n", rem);
  fprintf(out, "%s License along with this library; if not, write to the Free Software\n", rem);
  fprintf(out, "%s Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307\n", rem);
  fprintf(out, "%s USA.\n", rem);
  fprintf(out, "%s\n", rem);
  fprintf(out, "%s Please report all bugs and problems on the following page:\n", rem);
  fprintf(out, "%s\n", rem);
  fprintf(out, "%s      http://www.fltk.org/str.php\n", rem);
  fprintf(out, "%s\n", rem);
  fprintf(out, "\n");
  return 0;
}

int write_footer(FILE *out, const char *rem="#") {
  fprintf(out, "\n");
  fprintf(out, "%s\n", rem);
  fprintf(out, "%s End of \"$Id$\".\n", rem);
  fprintf(out, "%s\n", rem);
  return 0;
}

static int write_makeinclude_in(FILE *out, Fl_Workspace_Type *workspace, const char *path, const char *base_dir, const char *tgt_base, Fl_Target_Type *first_target) {
  
  Fl_Target_Type *tgt;

  write_header(out, "Make include file for the Fast Light Tool Kit (FLTK).");
  fprintf(out, "prefix\t\t= @prefix@\n");
  fprintf(out, "exec_prefix\t= @exec_prefix@\n");
  fprintf(out, "bindir\t\t= @bindir@\n");
  fprintf(out, "datadir\t\t= @datadir@\n");
  fprintf(out, "datarootdir\t= @datarootdir@\n");
  fprintf(out, "includedir\t= @includedir@\n");
  fprintf(out, "libdir\t\t= @libdir@\n");
  fprintf(out, "mandir\t\t= @mandir@\n");
  fprintf(out, "srcdir\t\t= @srcdir@\n");
  fprintf(out, "docdir\t\t= $(datadir)/doc/fltk\n");
  fprintf(out, "VPATH\t\t= @srcdir@\n");
  fprintf(out, "USEMMFILES  = @USEMMFILES@\n");
  fprintf(out, "# programs we use...\n");
  fprintf(out, "HTMLDOC\t\t= @HTMLDOC@\n");
  fprintf(out, "DOXYDOC\t\t= @DOXYDOC@\n");
  fprintf(out, "INSTALL\t\t= @INSTALL@\n");
  fprintf(out, "LN\t\t= ln -s\n");
  fprintf(out, "NROFF\t\t= @NROFF@\n");
  fprintf(out, "RM\t\t= rm -f\n");
  fprintf(out, "RMDIR\t\t= rm -rf\n");
  fprintf(out, "SHELL\t\t= /bin/sh\n");
  fprintf(out, "STRIP\t\t= strip\n");
  fprintf(out, "\n");
  fprintf(out, "# compiler names:\n");
  fprintf(out, "CXX\t\t= @CXX@\n");
  fprintf(out, "CC\t\t= @CC@\n");
  fprintf(out, "MAKEDEPEND\t= @MAKEDEPEND@\n");
  fprintf(out, "\n");
  fprintf(out, "# (Windows) resource compiler\n");
  fprintf(out, "RC\t\t= @RC@\n");
  fprintf(out, "\n");
  fprintf(out, "# flags for C++ compiler:\n");
  fprintf(out, "ARCHFLAGS\t= @ARCHFLAGS@\n");
  fprintf(out, "OPTIM\t\t= @OPTIM@\n");
  fprintf(out, "CFLAGS\t\t= $(OPTIM) @LARGEFILE@ @PTHREAD_FLAGS@ @CPPFLAGS@ @CFLAGS@\n");
  fprintf(out, "CXXFLAGS\t= $(OPTIM) @LARGEFILE@ @PTHREAD_FLAGS@ @CPPFLAGS@ @CXXFLAGS@ $(FLTKFLAGS)\n");
  fprintf(out, "\n");
  fprintf(out, "# program to make the archive:\n");
  
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "%s_LIBNAME\t\t= @%s_LIBNAME@\n", tgt->caps_name(), tgt->caps_name());      
      fprintf(out, "%s_DSONAME\t\t= @%s_DSONAME@\n", tgt->caps_name(), tgt->caps_name());      
      fprintf(out, "%s_LIBBASENAME\t\t= @%s_LIBBASENAME@\n", tgt->caps_name(), tgt->caps_name());      
    }
  }
    
  fprintf(out, "LIBCOMMAND\t= @LIBCOMMAND@\n");
  fprintf(out, "LIBEXT\t\t= @LIBEXT@\n");
  fprintf(out, "RANLIB\t\t= @RANLIB@\n");
  fprintf(out, "DSOCOMMAND\t= @DSOCOMMAND@\n");
  fprintf(out, "\n");
  fprintf(out, "# libraries to link with:\n");
  fprintf(out, "AUDIOLIBS\t= @AUDIOLIBS@\n");
  fprintf(out, "DSOFLAGS\t= -L. @DSOFLAGS@\n");
  fprintf(out, "LDFLAGS\t\t= $(OPTIM) @LDFLAGS@\n");
  fprintf(out, "LDLIBS\t\t= @LIBS@\n");
  fprintf(out, "GLLIB\t\t= @GLLIB@\n");
  fprintf(out, "GLDLIBS\t\t= @GLLIB@ @LIBS@\n");
  
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "LINK_%s\t= @LINK_%s@\n", tgt->caps_name(), tgt->caps_name());      
    }
  }
  
  fprintf(out, "LINKSHARED\t= @DSOLINK@ @LINKSHARED@ $(IMAGELIBS)\n");
  fprintf(out, "IMAGELIBS\t= -L../lib @IMAGELIBS@\n");
  fprintf(out, "\n");
  fprintf(out, "# image libraries to build...\n");
  fprintf(out, "IMAGEDIRS\t= \n");
  fprintf(out, "# The extension to use for executables...\n");
  fprintf(out, "EXEEXT\t\t= @EXEEXT@\n");
  fprintf(out, "\n");
  fprintf(out, "# Do we build the OpenGL demos?\n");
  fprintf(out, "GLDEMOS\t\t= @GLDEMOS@\n");
  fprintf(out, "\n");
  fprintf(out, "# Do we build the threads demo?\n");
  fprintf(out, "THREADS\t\t= @THREADS@\n");
  fprintf(out, "\n");
  fprintf(out, "# Name of FLUID executable we install\n");
  fprintf(out, "FLUID\t\t= @FLUID@$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "# Possible steps for OS X build only\n");
  fprintf(out, "OSX_ONLY\t= @OSX_ONLY@\n");
  fprintf(out, "\n");
  fprintf(out, "# Man page extensions...\n");
  fprintf(out, "CAT1EXT\t\t= @CAT1EXT@\n");
  fprintf(out, "CAT3EXT\t\t= @CAT3EXT@\n");
  fprintf(out, "CAT6EXT\t\t= @CAT6EXT@\n");
  fprintf(out, "\n");
  fprintf(out, "#\n");
  fprintf(out, "# Installation programs...\n");
  fprintf(out, "#\n");
  fprintf(out, "\n");
  fprintf(out, "INSTALL_BIN\t= $(INSTALL) -m 755\n");
  fprintf(out, "INSTALL_DATA\t= $(INSTALL) -m 644\n");
  fprintf(out, "INSTALL_DIR\t= $(INSTALL) -d \n");
  fprintf(out, "INSTALL_LIB\t= $(INSTALL) -m 755\n");
  fprintf(out, "INSTALL_MAN\t= $(INSTALL) -m 644\n");
  fprintf(out, "INSTALL_SCRIPT\t= $(INSTALL) -m 755\n");
  fprintf(out, "\n");
  fprintf(out, "# Additional GUI install/uninstall targets...\n");
  fprintf(out, "INSTALL_DESKTOP\t= @INSTALL_DESKTOP@\n");
  fprintf(out, "UNINSTALL_DESKTOP = @UNINSTALL_DESKTOP@\n");
  fprintf(out, "\n");
  fprintf(out, "# Be quiet when building...\n");
  fprintf(out, ".SILENT:\n");
  fprintf(out, "\n");
  fprintf(out, "# Build commands and filename extensions...\n");
  fprintf(out, ".SUFFIXES:\t.0 .1 .3 .6 .c .cxx .mm .h .fl .man .o .z $(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, ".o$(EXEEXT):\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) $< $(LINK_FLTK3) $(LDLIBS) -o $@\n");
  fprintf(out, "\n");
  fprintf(out, ".c.o:\n");
  fprintf(out, "\techo Compiling $<...\n");
  fprintf(out, "\t$(CC) -I../include $(ARCHFLAGS) @PNGINC@ @JPEGINC@ @ZLIBINC@ $(CFLAGS) -c $< -o $@\n");
  fprintf(out, "\n");
  fprintf(out, ".cxx.o:\n");
  fprintf(out, "\techo Compiling $<...\n");
  fprintf(out, "\t$(CXX) -I../include $(ARCHFLAGS) @PNGINC@ @JPEGINC@ @ZLIBINC@ $(CXXFLAGS) -c $< -o $@\n");
  fprintf(out, "\t\n");
  fprintf(out, ".mm.o:\n");
  fprintf(out, "\techo Compiling $<...\n");
  fprintf(out, "\t$(CXX) -I../include $(ARCHFLAGS) $(CXXFLAGS) -x objective-c++ -c $< -o $@\n");
  fprintf(out, "\n");
  fprintf(out, "\n");
  fprintf(out, ".man.0 .man.1 .man.3 .man.6:\n");
  fprintf(out, "\techo Formatting $<...\n");
  fprintf(out, "\trm -f $@\n");
  fprintf(out, "\t$(NROFF) -man $< >$@\n");
  fprintf(out, "\n");
  fprintf(out, ".man.z:\n");
  fprintf(out, "\techo Formatting $<...\n");
  fprintf(out, "\trm -f $@ t.z\n");
  fprintf(out, "\t$(NROFF) -man $< >t\n");
  fprintf(out, "\tpack -f t\n");
  fprintf(out, "\tmv t.z $@\n");
  write_footer(out);
  return 0;
}

static int write_configure_in(FILE *out, Fl_Workspace_Type *workspace, const char *path, const char *base_dir, const char *tgt_base, Fl_Target_Type *first_target) {
  
  Fl_Target_Type *tgt;
  
  fprintf(out, "dnl -*- sh -*-\n");
  fprintf(out, "dnl the \"configure\" script is made from this by running GNU \"autoconf\"\n");
  write_header(out, "Configuration script for the Fast Light Tool Kit (FLTK).", "dnl");
  fprintf(out, "dnl We need at least autoconf 2.50...\n");
  fprintf(out, "AC_PREREQ(2.50)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Required file in package...\n");
  fprintf(out, "AC_INIT(src/fltk3/run.cxx)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl So --with-archflags option is used during \"checking size of long\"\n");
  fprintf(out, "if test `uname` = Darwin; then\n");
  fprintf(out, "  if test \"x$with_archflags\" != x ; then\n");
  fprintf(out, "    CFLAGS=\"$CFLAGS $with_archflags\"\n");
  fprintf(out, "  fi\n");
  fprintf(out, "  USEMMFILES=\"Yes\"\n");
  fprintf(out, "else\n");
  fprintf(out, "  USEMMFILES=\"No\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "AC_SUBST(USEMMFILES)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl FLTK library versions...\n");
  fprintf(out, "FL_MAJOR_VERSION=3\n");
  fprintf(out, "FL_MINOR_VERSION=0\n");
  fprintf(out, "FL_PATCH_VERSION=0\n");
  fprintf(out, "FL_API_VERSION=${FL_MAJOR_VERSION}.${FL_MINOR_VERSION}\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(FL_MAJOR_VERSION)\n");
  fprintf(out, "AC_SUBST(FL_MINOR_VERSION)\n");
  fprintf(out, "AC_SUBST(FL_PATCH_VERSION)\n");
  fprintf(out, "AC_SUBST(FL_API_VERSION)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Get the operating system and version number...\n");
  fprintf(out, "uname=`uname`\n");
  fprintf(out, "uversion=`uname -r | sed -e '1,$s/[[^0-9]]//g'`\n");
  fprintf(out, "if test \"x$uname\" = xIRIX64; then\n");
  fprintf(out, "    uname=\"IRIX\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "if test \"x$uname\" = x; then\n");
  fprintf(out, "    # Older MinGW doesn't provide any output when uname is run, even with \"-s\".\n");
  fprintf(out, "    # Current MinGW (as of Jan. 2011) however does, so this is not necessary anymore...\n");
  fprintf(out, "    uname=\"MINGW\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Do not automatically add \"-g\" to compiler options...\n");
  fprintf(out, "ARCHFLAGS=\"${ARCHFLAGS:=}\"\n");
  fprintf(out, "CFLAGS=\"${CFLAGS:=}\"\n");
  fprintf(out, "CPPFLAGS=\"${CPPFLAGS:=}\"\n");
  fprintf(out, "CXXFLAGS=\"${CXXFLAGS:=}\"\n");
  fprintf(out, "DSOFLAGS=\"${DSOFLAGS:=}\"\n");
  fprintf(out, "LDFLAGS=\"${LDFLAGS:=}\"\n");
  fprintf(out, "OPTIM=\"${OPTIM:=}\"\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(ARCHFLAGS)\n");
  fprintf(out, "AC_SUBST(OPTIM)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl in case of cross-compiling mingw32 under linux we must change uname for Win32 GUI\n");
  fprintf(out, "dnl run ./configure --host=i686-pc-ming32 try use X11 without change uname\n");
  fprintf(out, "if test \"x$cross_compiling\" != xno ; then\n");
  fprintf(out, "  case $host_alias in\n");
  fprintf(out, "    *mingw*)\n");
  fprintf(out, "    uname=MINGW\n");
  fprintf(out, "    ;;\n");
  fprintf(out, "  esac\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "dnl OS-specific pre-tests...\n");
  fprintf(out, "dnl uname_GUI equals $uname unless we target Cygwin in combination with X11.\n");
  fprintf(out, "uname_GUI=$uname\n");
  fprintf(out, "case $uname in\n");
  fprintf(out, "    CYGWIN*)\n");
  fprintf(out, "        # Handle Cygwin option *first*, before all other tests.\n");
  fprintf(out, "        AC_ARG_ENABLE(cygwin, [  --enable-cygwin         use the Cygwin libraries [(default=no)]])\n");
  fprintf(out, "        AC_ARG_ENABLE(x11,    [  --enable-x11            use Cygwin with X11 [(default=no)]])\n");
  fprintf(out, "        if test x$enable_cygwin != xyes; then\n");
  fprintf(out, "            # NOTE: We can't use ARCHFLAGS for this, since it does not work\n");
  fprintf(out, "            #       with some of the function tests - Cygwin uses a\n");
  fprintf(out, "            #       different C library...\n");
  fprintf(out, "            CFLAGS=\"$CFLAGS -mno-cygwin\"\n");
  fprintf(out, "            CPPFLAGS=\"$CPPFLAGS -mno-cygwin\"\n");
  fprintf(out, "            CXXFLAGS=\"$CXXFLAGS -mno-cygwin\"\n");
  fprintf(out, "            LDFLAGS=\"$LDFLAGS -mno-cygwin\"\n");
  fprintf(out, "            DSOFLAGS=\"$DSOFLAGS -mno-cygwin\"\n");
  fprintf(out, "        else\n");
  fprintf(out, "            # we target Cygwin in combination with X11\n");
  fprintf(out, "            if test x$enable_x11 = xyes; then\n");
  fprintf(out, "                uname_GUI=\"X11$uname\"\n");
  fprintf(out, "            fi\n");
  fprintf(out, "        fi\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "esac\n");
  fprintf(out, "\n");
  
  fprintf(out, "dnl Define the libraries and link options we will need.\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "LINK_%s=\"../%s/lib%s.a\"\n", tgt->caps_name(), tgt->target_path(), tgt->name());
    }
  }
  
  fprintf(out, "GLDEMOS=\"gldemos\"\n");
  fprintf(out, "\n");
  fprintf(out, "LIBEXT=\".a\"\n");
  
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "%s_LIBNAME=\"../%s/lib%s.a\"\n", tgt->caps_name(), tgt->target_path(), tgt->name());
    }
  }
  fprintf(out, "\n");
  
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "%s_LIBBASENAME=\"lib%s.a\"\n", tgt->caps_name(), tgt->name());
    }
  }
  fprintf(out, "\n");
  
  fprintf(out, "AC_SUBST(GLDEMOS)\n");
  fprintf(out, "AC_SUBST(LIBEXT)\n");
  
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "AC_SUBST(%s_LIBNAME)\n", tgt->caps_name());
      fprintf(out, "AC_SUBST(LINK_%s)\n", tgt->caps_name());
      fprintf(out, "AC_SUBST(%s_LIBBASENAME)\n", tgt->caps_name());
    }
  }  
  fprintf(out, "\n");
  
  fprintf(out, "dnl Handle compile-time options...\n");
  fprintf(out, "AC_ARG_ENABLE(debug, [  --enable-debug          turn on debugging [default=no]])\n");
  fprintf(out, "if test x$enable_debug = xyes; then\n");
  fprintf(out, "    DEBUGFLAG=\"-g \"\n");
  fprintf(out, "else\n");
  fprintf(out, "    DEBUGFLAG=\"\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "AC_ARG_ENABLE(cp936, [  --enable-cp936          turn on CP936 [default=no]])\n");
  fprintf(out, "if test x$enable_cp936 = xyes; then\n");
  fprintf(out, "    CFLAGS=\"$CFLAGS -DCP936\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "\n");
  fprintf(out, "AC_ARG_ENABLE(gl, [  --enable-gl             turn on OpenGL support [default=yes]])\n");
  fprintf(out, "\n");
  fprintf(out, "AC_ARG_ENABLE(shared, [  --enable-shared         turn on shared libraries [default=no]])\n");
  fprintf(out, "if test x$enable_shared = xyes; then\n");
  fprintf(out, "    PICFLAG=1\n");
  fprintf(out, "    SHAREDSUFFIX=\"\"\n");
  fprintf(out, "    FLUID=\"fluid-shared\"\n");
  fprintf(out, "\n");
  fprintf(out, "    case $uname in\n");
  fprintf(out, "        Darwin*)\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "            %s_DSONAME=\"lib%s.$FL_API_VERSION.dylib\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "            DSOCOMMAND=\"\\$(CXX) \\$(ARCHFLAGS) \\$(DSOFLAGS) -dynamiclib -lc -o\"\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "\n");
  fprintf(out, "        SunOS* | UNIX_S*)\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "            %s_DSONAME=\"lib%s.so.$FL_API_VERSION\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "            DSOCOMMAND=\"\\$(CXX) \\$(DSOFLAGS) -h \\$@ \\$(LDLIBS) -G $DEBUGFLAG -o\"\n");
  fprintf(out, "            if test \"x$libdir\" != \"x/usr/lib\"; then\n");
  fprintf(out, "                    DSOLINK=\"-R$libdir\"\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        HP-UX*)\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "            %s_DSONAME=\"lib%s.sl.$FL_API_VERSION\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "            DSOCOMMAND=\"ld \\$(DSOFLAGS) -b -z +h \\$@ $DEBUGFLAG -o\"\n");
  fprintf(out, "            if test \"x$libdir\" != \"x/usr/lib\"; then\n");
  fprintf(out, "                    DSOLINK=\"-Wl,-rpath,$libdir\"\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        IRIX*)\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "            %s_DSONAME=\"lib%s.so.$FL_API_VERSION\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "            DSOCOMMAND=\"\\$(CXX) \\$(DSOFLAGS) -Wl,-soname,\\$@,-set_version,sgi1.1 \\$(LDLIBS) -shared $DEBUGFLAG -o\"\n");
  fprintf(out, "            if test \"x$libdir\" != \"x/usr/lib\" -a \"x$libdir\" != \"x/usr/lib32\" -a \"x$libdir\" != \"x/usr/lib64\"; then\n");
  fprintf(out, "                    DSOLINK=\"-Wl,-rpath,$libdir\"\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        OSF1*)\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "            %s_DSONAME=\"lib%s.so.$FL_API_VERSION\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "            DSOCOMMAND=\"\\$(CXX) \\$(DSOFLAGS) -Wl,-soname,\\$@ \\$(LDLIBS) -shared $DEBUGFLAG -o\"\n");
  fprintf(out, "            if test \"x$libdir\" != \"x/usr/lib\" -a \"x$libdir\" != \"x/usr/lib32\"; then\n");
  fprintf(out, "                    DSOLINK=\"-Wl,-rpath,$libdir\"\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        Linux* | *BSD*)\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "            %s_DSONAME=\"lib%s.so.$FL_API_VERSION\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "            DSOCOMMAND=\"\\$(CXX) \\$(DSOFLAGS) -Wl,-soname,\\$@ \\$(LDLIBS) -shared -fPIC $DEBUGFLAG -o\"\n");
  fprintf(out, "            if test \"x$libdir\" != \"x/usr/lib\" -a \"x$libdir\" != \"x/usr/lib64\"; then\n");
  fprintf(out, "                    DSOLINK=\"-Wl,-rpath,$libdir\"\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        AIX*)\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "            %s_DSONAME=\"lib%s_s.a\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "            DSOCOMMAND=\"\\$(CXX) \\$(DSOFLAGS) -Wl,-bexpall,-bM:SRE,-bnoentry -o\"\n");
  fprintf(out, "            SHAREDSUFFIX=\"_s\"\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        CYGWIN* | MINGW*)\n");
  fprintf(out, "            PICFLAG=0\n");
  // FIXME: naming convention is messed up!
  fprintf(out, "            if test x$enable_cygwin != xyes; then\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "                %s_DSONAME=\"mgw%snox-$FL_API_VERSION.dll\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "            else\n");
  fprintf(out, "                if test x$enable_x11 = xyes; then\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "                    %s_DSONAME=\"cyg%s-$FL_API_VERSION.dll\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "                else\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "                    %s_DSONAME=\"cyg%snox-$FL_API_VERSION.dll\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "                fi\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            #-----------------------------------------------------------\n");
  fprintf(out, "            # -Wl,--enable-runtime-pseudo-reloc: See str 1585\n");
  fprintf(out, "            # appears to be necessary for older binutils versions < 2.16\n");
  fprintf(out, "            #-----------------------------------------------------------\n");
  fprintf(out, "            LDFLAGS=\"$LDFLAGS -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc\"\n");
  fprintf(out, "            DSOCOMMAND=\"\\$(CXX) \\$(DSOFLAGS) -shared \\\n");
  fprintf(out, "                    -Wl,--whole-archive -Wl,--export-all-symbols \\\n");
  fprintf(out, "                -Wl,--enable-runtime-pseudo-reloc -Wl,--enable-auto-import \\\n");
  fprintf(out, "                -Wl,--enable-auto-image-base -o \\$@\"\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        *)\n");
  fprintf(out, "            AC_MSG_WARN(Shared libraries may not be supported.  Trying -shared option with compiler.)\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "            %s_DSONAME=\"lib%s.so.$FL_API_VERSION\"\n", tgt->caps_name(), tgt->name());
    }
  }  
  fprintf(out, "            DSOCOMMAND=\"\\$(CXX) \\$(DSOFLAGS) -Wl,-soname,\\$@ \\$(LDLIBS) -shared $DEBUGFLAG -o\"\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "    esac\n");
  fprintf(out, "\n");
  fprintf(out, "    LINKSHARED=\"-L../src -lfltk_images$SHAREDSUFFIX -lfltk$SHAREDSUFFIX\"\n");
  fprintf(out, "else\n");
  fprintf(out, "    DSOCOMMAND=\"echo\"\n");
  fprintf(out, "    DSOLINK=\"\"\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "    %s_DSONAME=\"\"\n", tgt->caps_name());
    }
  }  
  fprintf(out, "    PICFLAG=0\n");
  fprintf(out, "    SHAREDSUFFIX=\"\"\n");
  fprintf(out, "    FLUID=\"fluid\"\n");
  fprintf(out, "    LINKSHARED=\"../lib/libfltk_images.a ../lib/libfltk.a\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(DSOCOMMAND)\n");
  fprintf(out, "AC_SUBST(DSOFLAGS)\n");
  fprintf(out, "AC_SUBST(DSOLINK)\n");
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->is_lib_target() && tgt->builds_in(FL_ENV_MAKE)) {
      fprintf(out, "AC_SUBST(%s_DSONAME)\n", tgt->caps_name());
    }
  }  
  fprintf(out, "AC_SUBST(SHAREDSUFFIX)\n");
  fprintf(out, "AC_SUBST(LINKSHARED)\n");
  fprintf(out, "AC_SUBST(FLUID)\n");
  fprintf(out, "\n");
  fprintf(out, "AC_ARG_ENABLE(threads, [  --enable-threads        enable multi-threading support (default=yes)])\n");
  fprintf(out, "\n");
  fprintf(out, "AC_ARG_WITH(optim, [  --with-optim=\"flags\"    use custom optimization flags])\n");
  fprintf(out, "\n");
  fprintf(out, "AC_ARG_WITH(archflags, [  --with-archflags=\"flags\"\n");
  fprintf(out, "                          use custom architecture flags \n");
  fprintf(out, "                          (possible Mac OS X values include -arch i386, -arch x86_64, -arch ppc)],\n");
  fprintf(out, "    ARCHFLAGS=\"$withval\")\n");
  fprintf(out, "case $uname in\n");
  fprintf(out, "    Darwin*)\n");
  fprintf(out, "# QD is not supported anymore since 1.3\n");
  fprintf(out, "        AC_DEFINE(__APPLE_QUARTZ__)\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "esac\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Find commands...\n");
  fprintf(out, "AC_PROG_CC\n");
  fprintf(out, "AC_PROG_CXX\n");
  fprintf(out, "AC_PROG_INSTALL\n");
  fprintf(out, "case $uname in\n");
  fprintf(out, "    OSF1*)\n");
  fprintf(out, "        INSTALL=\"`pwd`/install-sh -c\"\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "esac\n");
  fprintf(out, "if test \"$INSTALL\" = \"$ac_install_sh\"; then\n");
  fprintf(out, "    # Use full path to install-sh script...\n");
  fprintf(out, "    INSTALL=\"`pwd`/install-sh -c\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "AC_PATH_PROG(NROFF,nroff)\n");
  fprintf(out, "if test \"x$NROFF\" = \"x:\"; then\n");
  fprintf(out, "    # Try groff instead of nroff...\n");
  fprintf(out, "    AC_PATH_PROG(GROFF,groff)\n");
  fprintf(out, "    if test \"x$GROFF\" = \"x:\"; then\n");
  fprintf(out, "        NROFF=\"echo\"\n");
  fprintf(out, "    else\n");
  fprintf(out, "        NROFF=\"$GROFF -T ascii\"\n");
  fprintf(out, "    fi\n");
  fprintf(out, "fi\n");
  fprintf(out, "AC_PATH_PROG(HTMLDOC,htmldoc)\n");
  fprintf(out, "AC_PATH_PROG(DOXYDOC,doxygen)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl How do we make libraries?\n");
  fprintf(out, "AC_PROG_RANLIB\n");
  fprintf(out, "AC_PATH_TOOL(AR, ar)\n");
  fprintf(out, "\n");
  fprintf(out, "if test \"x$AR\" = \"x:\"; then\n");
  fprintf(out, "    AC_MSG_ERROR(Configure could not find the library archiver, aborting.)\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "if test \"x$RANLIB\" != \"x:\"; then\n");
  fprintf(out, "    LIBCOMMAND=\"$AR cr\"\n");
  fprintf(out, "else\n");
  fprintf(out, "    LIBCOMMAND=\"$AR crs\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(LIBCOMMAND)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl how to compile (Windows) resource files\n");
  fprintf(out, "dnl this will only be used to compile resources for Windows .exe files\n");
  fprintf(out, "AC_PATH_TOOL(RC,windres)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Architecture checks...\n");
  fprintf(out, "if test \"$uname\" != Darwin; then\n");
  fprintf(out, "  AC_C_BIGENDIAN\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "AC_CHECK_SIZEOF(short, 2)\n");
  fprintf(out, "AC_CHECK_SIZEOF(int, 4)\n");
  fprintf(out, "AC_CHECK_SIZEOF(long, 4)\n");
  fprintf(out, "if test $ac_cv_sizeof_short -eq 2; then\n");
  fprintf(out, "    AC_DEFINE(U16,unsigned short)\n");
  fprintf(out, "fi\n");
  fprintf(out, "if test $ac_cv_sizeof_int -eq 4; then\n");
  fprintf(out, "    AC_DEFINE(U32,unsigned)\n");
  fprintf(out, "else\n");
  fprintf(out, "    if test $ac_cv_sizeof_long -eq 4; then\n");
  fprintf(out, "        AC_DEFINE(U32,unsigned long)\n");
  fprintf(out, "    fi\n");
  fprintf(out, "fi\n");
  fprintf(out, "if test \"$uname\" != Darwin; then\n");
  fprintf(out, "        if test $ac_cv_sizeof_int -eq 8; then\n");
  fprintf(out, "                AC_DEFINE(U64,unsigned)\n");
  fprintf(out, "        else\n");
  fprintf(out, "                if test $ac_cv_sizeof_long -eq 8; then\n");
  fprintf(out, "                        AC_DEFINE(U64,unsigned long)\n");
  fprintf(out, "                fi\n");
  fprintf(out, "        fi\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Does the C++ compiler support the bool type?\n");
  fprintf(out, "AC_CACHE_CHECK(whether the compiler recognizes bool as a built-in type,\n");
  fprintf(out, "    ac_cv_cxx_bool,[\n");
  fprintf(out, "        AC_LANG_SAVE\n");
  fprintf(out, "        AC_LANG_CPLUSPLUS\n");
  fprintf(out, "        AC_TRY_COMPILE([\n");
  fprintf(out, "            int f(int  x){return 1;}\n");
  fprintf(out, "            int f(char x){return 1;}\n");
  fprintf(out, "            int f(bool x){return 1;}\n");
  fprintf(out, "        ],[\n");
  fprintf(out, "            bool b = true;\n");
  fprintf(out, "            return f(b);\n");
  fprintf(out, "        ], ac_cv_cxx_bool=yes, ac_cv_cxx_bool=no)\n");
  fprintf(out, "        AC_LANG_RESTORE\n");
  fprintf(out, "    ])\n");
  fprintf(out, "\n");
  fprintf(out, "if test \"$ac_cv_cxx_bool\" != yes; then\n");
  fprintf(out, "    CXXFLAGS=\"-Dbool=char -Dfalse=0 -Dtrue=1 $CXXFLAGS\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Standard headers and functions...\n");
  fprintf(out, "AC_HEADER_DIRENT\n");
  fprintf(out, "AC_CHECK_HEADER(sys/select.h,AC_DEFINE(HAVE_SYS_SELECT_H))\n");
  fprintf(out, "AC_CHECK_HEADER(sys/stdtypes.h,AC_DEFINE(HAVE_SYS_SELECT_H))\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Do we have the POSIX compatible scandir() prototype?\n");
  fprintf(out, "AC_CACHE_CHECK([whether we have the POSIX compatible scandir() prototype],\n");
  fprintf(out, "    ac_cv_cxx_scandir_posix,[\n");
  fprintf(out, "        AC_LANG_SAVE\n");
  fprintf(out, "        AC_LANG_CPLUSPLUS\n");
  fprintf(out, "        AC_TRY_COMPILE([\n");
  fprintf(out, "          #include <dirent.h>\n");
  fprintf(out, "          int func (const char *d, dirent ***list, void *sort) {\n");
  fprintf(out, "            int n = scandir(d, list, 0, (int(*)(const dirent **, const dirent **))sort);\n");
  fprintf(out, "          }\n");
  fprintf(out, "        ],[\n");
  fprintf(out, "        ], ac_cv_cxx_scandir_posix=yes, ac_cv_cxx_scandir_posix=no)\n");
  fprintf(out, "        AC_LANG_RESTORE\n");
  fprintf(out, "    ])\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Define both HAVE_SCANDIR... macros, if the POSIX compatible function is\n");
  fprintf(out, "dnl available. Otherwise: check, whether any scandir prototype is available,\n");
  fprintf(out, "dnl but don't use it on SunOS and QNX because of an incompatibility in pre-Y2K\n");
  fprintf(out, "dnl SunOS scandir versions. We assume, though, that the POSIX compatible\n");
  fprintf(out, "dnl version on newer SunOS/Solaris versions works as expected.\n");
  fprintf(out, "if test \"$ac_cv_cxx_scandir_posix\" = yes; then\n");
  fprintf(out, "    AC_DEFINE(HAVE_SCANDIR)\n");
  fprintf(out, "    AC_DEFINE(HAVE_SCANDIR_POSIX)\n");
  fprintf(out, "else\n");
  fprintf(out, "    AC_CHECK_FUNC(scandir,\n");
  fprintf(out, "        if test \"x$uname\" = xSunOS -o \"x$uname\" = xQNX; then\n");
  fprintf(out, "            AC_MSG_WARN(Not using $uname scandir emulation function.)\n");
  fprintf(out, "        else\n");
  fprintf(out, "            AC_DEFINE(HAVE_SCANDIR)\n");
  fprintf(out, "        fi)\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "AC_CHECK_FUNC(vsnprintf,[\n");
  fprintf(out, "    case \"$uname\" in\n");
  fprintf(out, "            HP-UX*)\n");
  fprintf(out, "            if test \"$uversion\" = \"1020\"; then\n");
  fprintf(out, "                AC_MSG_WARN(Not using built-in vsnprintf function because you are running HP-UX 10.20.)\n");
  fprintf(out, "            else\n");
  fprintf(out, "                AC_DEFINE(HAVE_VSNPRINTF)\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "\n");
  fprintf(out, "            OSF1*)\n");
  fprintf(out, "            if test \"$uversion\" = \"40\"; then\n");
  fprintf(out, "                AC_MSG_WARN(Not using built-in vsnprintf function because you are running Tru64 4.0.)\n");
  fprintf(out, "            else\n");
  fprintf(out, "                AC_DEFINE(HAVE_VSNPRINTF)\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "\n");
  fprintf(out, "        *)\n");
  fprintf(out, "            AC_DEFINE(HAVE_VSNPRINTF)\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "    esac])\n");
  fprintf(out, "AC_CHECK_FUNC(snprintf,[\n");
  fprintf(out, "    case \"$uname\" in\n");
  fprintf(out, "            HP-UX*)\n");
  fprintf(out, "            if test \"$uversion\" = \"1020\"; then\n");
  fprintf(out, "                AC_MSG_WARN(Not using built-in snprintf function because you are running HP-UX 10.20.)\n");
  fprintf(out, "            else\n");
  fprintf(out, "                AC_DEFINE(HAVE_SNPRINTF)\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "\n");
  fprintf(out, "            OSF1*)\n");
  fprintf(out, "            if test \"$uversion\" = \"40\"; then\n");
  fprintf(out, "                AC_MSG_WARN(Not using built-in snprintf function because you are running Tru64 4.0.)\n");
  fprintf(out, "            else\n");
  fprintf(out, "                AC_DEFINE(HAVE_SNPRINTF)\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "\n");
  fprintf(out, "        *)\n");
  fprintf(out, "            AC_DEFINE(HAVE_SNPRINTF)\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "    esac])\n");
  fprintf(out, "AC_CHECK_HEADER(strings.h, AC_DEFINE(HAVE_STRINGS_H))\n");
  fprintf(out, "AC_CHECK_FUNCS(strcasecmp strlcat strlcpy)\n");
  fprintf(out, "\n");
  fprintf(out, "AC_CHECK_HEADER(locale.h, AC_DEFINE(HAVE_LOCALE_H))\n");
  fprintf(out, "AC_CHECK_FUNCS(localeconv)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl FLTK library uses math library functions...\n");
  fprintf(out, "AC_SEARCH_LIBS(pow, m)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Check for largefile support...\n");
  fprintf(out, "AC_SYS_LARGEFILE\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Define largefile options as needed...\n");
  fprintf(out, "LARGEFILE=\"\"\n");
  fprintf(out, "if test x$enable_largefile != xno; then\n");
  fprintf(out, "        LARGEFILE=\"-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE\"\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$ac_cv_sys_large_files = x1; then\n");
  fprintf(out, "                LARGEFILE=\"$LARGEFILE -D_LARGE_FILES\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$ac_cv_sys_file_offset_bits = x64; then\n");
  fprintf(out, "                LARGEFILE=\"$LARGEFILE -D_FILE_OFFSET_BITS=64\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "fi\n");
  fprintf(out, "AC_SUBST(LARGEFILE)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Check for \"long long\" support...\n");
  fprintf(out, "AC_CACHE_CHECK(for long long int, ac_cv_c_long_long,\n");
  fprintf(out, "        [if test \"$GCC\" = yes; then\n");
  fprintf(out, "                ac_cv_c_long_long=yes\n");
  fprintf(out, "        else\n");
  fprintf(out, "                AC_TRY_COMPILE(,[long long int i;],\n");
  fprintf(out, "                        ac_cv_c_long_long=yes,\n");
  fprintf(out, "                        ac_cv_c_long_long=no)\n");
  fprintf(out, "        fi])\n");
  fprintf(out, "\n");
  fprintf(out, "if test $ac_cv_c_long_long = yes; then\n");
  fprintf(out, "        AC_DEFINE(HAVE_LONG_LONG)\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "AC_CHECK_FUNC(strtoll, AC_DEFINE(HAVE_STRTOLL))\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Check for dlopen/dlsym...\n");
  fprintf(out, "AC_SEARCH_LIBS(dlsym, dl, AC_DEFINE(HAVE_DLSYM))\n");
  fprintf(out, "AC_CHECK_HEADER(dlfcn.h, AC_DEFINE(HAVE_DLFCN_H))\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Check for audio libraries...\n");
  fprintf(out, "AUDIOLIBS=\"\"\n");
  fprintf(out, "\n");
  fprintf(out, "case $uname in\n");
  fprintf(out, "    CYGWIN* | MINGW*)\n");
  fprintf(out, "        dnl Cygwin environment...\n");
  fprintf(out, "        AUDIOLIBS=\"-lwinmm\"\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "\n");
  fprintf(out, "    Darwin*)\n");
  fprintf(out, "        AUDIOLIBS=\"-framework CoreAudio\"\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "\n");
  fprintf(out, "    *)\n");
  fprintf(out, "        AC_CHECK_HEADER(alsa/asoundlib.h,\n");
  fprintf(out, "            AC_DEFINE(HAVE_ALSA_ASOUNDLIB_H)\n");
  fprintf(out, "            AUDIOLIBS=\"-lasound\")\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "esac\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(AUDIOLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Check for image libraries...\n");
  fprintf(out, "SAVELIBS=\"$LIBS\"\n");
  fprintf(out, "IMAGELIBS=\"\"\n");
  fprintf(out, "STATICIMAGELIBS=\"\"\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(IMAGELIBS)\n");
  fprintf(out, "AC_SUBST(STATICIMAGELIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "# Handle the JPEG lib linking mode (use fltk local or system lib)\n");
  fprintf(out, "# If --enable-(resp. --disable-)localjpeg parameter is not set by user\n");
  fprintf(out, "# Then we check the JPEG lib usability, with result in sysjpeglib_ok variable\n");
  fprintf(out, "AC_ARG_ENABLE(localjpeg, [  --enable-localjpeg      use local JPEG library, default=yes])\n");
  fprintf(out, "# Check for System lib use if automatic mode or --disable-localjpeg is requested\n");
  fprintf(out, "sysjpeglib_ok=no\n");
  fprintf(out, "sysjpeginc_ok=no\n");
  fprintf(out, "if test x$enable_localjpeg != xyes; then\n");
  fprintf(out, "    AC_CHECK_LIB(jpeg,jpeg_CreateCompress,\n");
  fprintf(out, "        [AC_CHECK_HEADER(jpeglib.h,\n");
  fprintf(out, "            sysjpeginc_ok=yes)\n");
  fprintf(out, "            if test x$sysjpeginc_ok = xyes; then\n");
  fprintf(out, "                sysjpeglib_ok=yes\n");
  fprintf(out, "                fi])\n");
  fprintf(out, "fi\n");
  fprintf(out, "# Now set the jpeg lib and include flags according to the requested mode and availability \n");
  fprintf(out, "if test x$enable_localjpeg = xyes -o x$sysjpeglib_ok = xno; then\n");
  fprintf(out, "    JPEGINC=\"-I../include/fltk3jpeg\"\n");
  fprintf(out, "    JPEG=\"jpeg\"\n");
  fprintf(out, "    IMAGELIBS=\"-lfltk3jpeg $IMAGELIBS\"\n");
  fprintf(out, "    STATICIMAGELIBS=\"\\$libdir/libfltk3jpeg.a $STATICIMAGELIBS\"\n");
  fprintf(out, "    AC_DEFINE(HAVE_LIBJPEG)\n");
  fprintf(out, "    #ac_cv_lib_jpeg_jpeg_CreateCompress=no # from ima: should not be necessary\n");
  fprintf(out, "    # Finally, warn user if system lib was requested but not found\n");
  fprintf(out, "    if test x$enable_localjpeg = xno; then\n");
  fprintf(out, "        AC_MSG_WARN(Cannot find system jpeg lib or header: choosing the local lib mode.)\n");
  fprintf(out, "    fi\n");
  fprintf(out, "else\n");
  fprintf(out, "    JPEGINC=\"\"\n");
  fprintf(out, "    JPEG=\"\"\n");
  fprintf(out, "    IMAGELIBS=\"-ljpeg $IMAGELIBS\"\n");
  fprintf(out, "    STATICIMAGELIBS=\"-ljpeg $STATICIMAGELIBS\"\n");
  fprintf(out, "    AC_DEFINE(HAVE_LIBJPEG)\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "# Handle the ZLIB lib linking mode (use fltk local or system lib)\n");
  fprintf(out, "# If --enable-(resp. --disable-)localzlib parameter is not set by user\n");
  fprintf(out, "# Then we check the ZLIB lib usability, with result in syszlib_ok variable\n");
  fprintf(out, "AC_ARG_ENABLE(localzlib, [  --enable-localzlib      use local ZLIB library, default=yes])\n");
  fprintf(out, "# Check for System lib use if automatic mode or --disable-localzlib is requested\n");
  fprintf(out, "syszlib_ok=no\n");
  fprintf(out, "syszinc_ok=no\n");
  fprintf(out, "if test x$enable_localzlib != xyes; then\n");
  fprintf(out, "    AC_CHECK_LIB(z,gzgets,\n");
  fprintf(out, "        [AC_CHECK_HEADER(zlib.h, \n");
  fprintf(out, "                syszinc_ok=yes)\n");
  fprintf(out, "        if test x$syszinc_ok = xyes; then\n");
  fprintf(out, "            syszlib_ok=yes\n");
  fprintf(out, "            fi])\n");
  fprintf(out, "fi\n");
  fprintf(out, "# Now set the Z lib and include flags according to the requested mode and availability \n");
  fprintf(out, "if test x$enable_localzlib = xyes -o x$syszlib_ok = xno ; then\n");
  fprintf(out, "    ZLIBINC=\"-I../include/fltk3zlib\"\n");
  fprintf(out, "    ZLIB=\"zlib\"\n");
  fprintf(out, "    LIBS=\"-lfltk3zlib $LIBS\"\n");
  fprintf(out, "    IMAGELIBS=\"-lfltk3zlib $IMAGELIBS\"\n");
  fprintf(out, "    STATICIMAGELIBS=\"\\$libdir/libfltk3zlib.a $STATICIMAGELIBS\"\n");
  fprintf(out, "    AC_DEFINE(HAVE_LIBZ)\n");
  fprintf(out, "    ac_cv_lib_z_gzgets=no # fc: is still necessary ?\n");
  fprintf(out, "    # Finally, warn user if system lib was requested but not found\n");
  fprintf(out, "    if test x$enable_localzlib = xno; then\n");
  fprintf(out, "        AC_MSG_WARN(Cannot find system z lib or header: choosing the local lib mode.)\n");
  fprintf(out, "    fi\n");
  fprintf(out, "else\n");
  fprintf(out, "    ZLIBINC=\"\"\n");
  fprintf(out, "    ZLIB=\"\"\n");
  fprintf(out, "    LIBS=\"-lz $LIBS\"\n");
  fprintf(out, "    IMAGELIBS=\"-lz $IMAGELIBS\"\n");
  fprintf(out, "    STATICIMAGELIBS=\"-lz $STATICIMAGELIBS\"\n");
  fprintf(out, "    AC_DEFINE(HAVE_LIBZ)\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "# Handle the PNG lib linking mode (use fltk local or system lib)\n");
  fprintf(out, "# If --enable-(resp. --disable-)localpng parameter is not set by user\n");
  fprintf(out, "# Then we check the png lib usability with result in syspng_lib variable\n");
  fprintf(out, "AC_ARG_ENABLE(localpng, [  --enable-localpng       use local PNG library, default=yes])\n");
  fprintf(out, "\n");
  fprintf(out, "# Now check if system lib is usable, we check Lib AND include availability with inc variant,\n");
  fprintf(out, "# but only, if the builtin lib is not requested\n");
  fprintf(out, "syspnglib_ok=no\n");
  fprintf(out, "syspnginc_ok=no\n");
  fprintf(out, "if test x$enable_localpng != xyes; then\n");
  fprintf(out, "  AC_CHECK_LIB(png, png_read_info, \n");
  fprintf(out, "    [AC_CHECK_HEADER(png.h, \n");
  fprintf(out, "        AC_DEFINE(HAVE_PNG_H)\n");
  fprintf(out, "        syspnginc_ok=yes)\n");
  fprintf(out, "    AC_CHECK_HEADER(libpng/png.h, \n");
  fprintf(out, "        AC_DEFINE(HAVE_LIBPNG_PNG_H)\n");
  fprintf(out, "        syspnginc_ok=yes)\n");
  fprintf(out, "    if test x$syspnginc_ok = xyes; then\n");
  fprintf(out, "        syspnglib_ok=yes\n");
  fprintf(out, "    fi])\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "# The following is executed if the lib was not found usable or if local lib is required explicitly\n");
  fprintf(out, "if test x$enable_localpng = xyes -o x$syspnglib_ok = xno ; then\n");
  fprintf(out, "    PNGINC=\"-I../include/fltk3png\"\n");
  fprintf(out, "    PNG=\"png\"\n");
  fprintf(out, "    IMAGELIBS=\"-lfltk3png $IMAGELIBS\"\n");
  fprintf(out, "    STATICIMAGELIBS=\"\\$libdir/libfltk3png.a $STATICIMAGELIBS\"\n");
  fprintf(out, "    AC_DEFINE(HAVE_LIBPNG)\n");
  fprintf(out, "    AC_DEFINE(HAVE_PNG_H)\n");
  fprintf(out, "    AC_DEFINE(HAVE_PNG_GET_VALID)\n");
  fprintf(out, "    AC_DEFINE(HAVE_PNG_SET_TRNS_TO_ALPHA)\n");
  fprintf(out, "    # Finally, warn user if system lib was requested but not found\n");
  fprintf(out, "    if test x$enable_localpng = xno; then\n");
  fprintf(out, "        AC_MSG_WARN(Cannot find system png lib or header: choosing the local lib mode.)\n");
  fprintf(out, "    fi\n");
  fprintf(out, "else\n");
  fprintf(out, "    PNGINC=\"\"\n");
  fprintf(out, "    PNG=\"\"\n");
  fprintf(out, "    IMAGELIBS=\"-lpng $IMAGELIBS\"\n");
  fprintf(out, "    STATICIMAGELIBS=\"-lpng $STATICIMAGELIBS\"\n");
  fprintf(out, "    AC_DEFINE(HAVE_LIBPNG)\n");
  fprintf(out, "    AC_CHECK_LIB(png,png_get_valid, AC_DEFINE(HAVE_PNG_GET_VALID))\n");
  fprintf(out, "    AC_CHECK_LIB(png,png_set_tRNS_to_alpha, AC_DEFINE(HAVE_PNG_SET_TRNS_TO_ALPHA))\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(JPEG)\n");
  fprintf(out, "AC_SUBST(JPEGINC)\n");
  fprintf(out, "AC_SUBST(PNG)\n");
  fprintf(out, "AC_SUBST(PNGINC)\n");
  fprintf(out, "AC_SUBST(ZLIB)\n");
  fprintf(out, "AC_SUBST(ZLIBINC)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Restore original LIBS settings...\n");
  fprintf(out, "LIBS=\"$SAVELIBS\"\n");
  fprintf(out, "\n");
  fprintf(out, "dnl See if we need a .exe extension on executables...\n");
  fprintf(out, "AC_EXEEXT\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Check for pthreads for multi-threaded apps...\n");
  fprintf(out, "have_pthread=no\n");
  fprintf(out, "PTHREAD_FLAGS=\"\"\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Test whether we want to check for pthreads. We must not do it on Windows\n");
  fprintf(out, "dnl unless we run under Cygwin with --enable-cygwin, since we always use\n");
  fprintf(out, "dnl native threads on Windows (even if libpthread is available)\n");
  fprintf(out, "check_pthread=yes\n");
  fprintf(out, "case $uname in\n");
  fprintf(out, "    MINGW*)\n");
  fprintf(out, "        check_pthread=no\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "    CYGWIN*)\n");
  fprintf(out, "        if test \"x$enable_cygwin\" != xyes; then\n");
  fprintf(out, "            check_pthread=no\n");
  fprintf(out, "        fi\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "    *)\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "esac\n");
  fprintf(out, "\n");
  fprintf(out, "if test \"x$enable_threads\" != xno -a x$check_pthread = xyes; then\n");
  fprintf(out, "    AC_CHECK_HEADER(pthread.h, AC_DEFINE(HAVE_PTHREAD_H))\n");
  fprintf(out, "\n");
  fprintf(out, "    if test x$ac_cv_header_pthread_h = xyes; then\n");
  fprintf(out, "        dnl Check various threading options for the platforms we support\n");
  fprintf(out, "        for flag in -lpthreads -lpthread -pthread; do\n");
  fprintf(out, "            AC_MSG_CHECKING([for pthread_create using $flag])\n");
  fprintf(out, "            SAVELIBS=\"$LIBS\"\n");
  fprintf(out, "            LIBS=\"$flag $LIBS\"\n");
  fprintf(out, "            AC_TRY_LINK([#include <pthread.h>],\n");
  fprintf(out, "                [pthread_create(0, 0, 0, 0);],\n");
  fprintf(out, "                have_pthread=yes,\n");
  fprintf(out, "                LIBS=\"$SAVELIBS\")\n");
  fprintf(out, "            AC_MSG_RESULT([$have_pthread])\n");
  fprintf(out, "\n");
  fprintf(out, "            if test $have_pthread = yes; then\n");
  fprintf(out, "                AC_DEFINE(HAVE_PTHREAD)\n");
  fprintf(out, "                PTHREAD_FLAGS=\"-D_THREAD_SAFE -D_REENTRANT\"\n");
  fprintf(out, "\n");
  fprintf(out, "                # Solaris requires -D_POSIX_PTHREAD_SEMANTICS to\n");
  fprintf(out, "                # be POSIX-compliant... :(\n");
  fprintf(out, "                if test $uname = SunOS; then\n");
  fprintf(out, "                    PTHREAD_FLAGS=\"$PTHREAD_FLAGS -D_POSIX_PTHREAD_SEMANTICS\"\n");
  fprintf(out, "                fi\n");
  fprintf(out, "                break\n");
  fprintf(out, "            fi\n");
  fprintf(out, "        done\n");
  fprintf(out, "    fi\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(PTHREAD_FLAGS)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Define OS-specific stuff...\n");
  fprintf(out, "HLINKS=\n");
  fprintf(out, "OSX_ONLY=:\n");
  fprintf(out, "THREADS=\n");
  fprintf(out, "\n");
  fprintf(out, "AC_ARG_WITH(links, [  --with-links            make header links for common misspellings (default=no)])\n");
  fprintf(out, "\n");
  fprintf(out, "INSTALL_DESKTOP=\"\"\n");
  fprintf(out, "UNINSTALL_DESKTOP=\"\"\n");
  fprintf(out, "\n");
  fprintf(out, "case $uname_GUI in\n");
  fprintf(out, "    CYGWIN* | MINGW*)\n");
  fprintf(out, "        dnl Cygwin environment, using windows GDI ...\n");
  fprintf(out, "        # Recent versions of Cygwin are seriously broken and the size\n");
  fprintf(out, "        # checks don't work because the shell puts out \\r\\n instead of\n");
  fprintf(out, "        # \\n.  Here we just force U32 to be defined to \"unsigned\"...\n");
  fprintf(out, "        AC_DEFINE(U32,unsigned)\n");
  fprintf(out, "        CFLAGS=\"-mwindows -DWIN32 -DUSE_OPENGL32 $CFLAGS\"\n");
  fprintf(out, "        CXXFLAGS=\"-mwindows -DWIN32 -DUSE_OPENGL32 $CXXFLAGS\"\n");
  fprintf(out, "        LDFLAGS=\"-mwindows $LDFLAGS\"\n");
  fprintf(out, "        DSOFLAGS=\"-mwindows $DSOFLAGS\"\n");
  fprintf(out, "        LIBS=\"$LIBS -lole32 -luuid -lcomctl32\"\n");
  fprintf(out, "        if test \"x$with_optim\" = x; then\n");
  fprintf(out, "            dnl Avoid -Os optimization on Cygwin/MinGW\n");
  fprintf(out, "            with_optim=\"-O3\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$enable_gl != xno; then\n");
  fprintf(out, "            AC_CHECK_HEADER(GL/gl.h,\n");
  fprintf(out, "                AC_DEFINE(HAVE_GL)\n");
  fprintf(out, "                GLLIB=\"-lopengl32\")\n");
  fprintf(out, "            AC_CHECK_HEADER(GL/glu.h,\n");
  fprintf(out, "                AC_DEFINE(HAVE_GL_GLU_H)\n");
  fprintf(out, "                GLLIB=\"-lglu32 $GLLIB\")\n");
  fprintf(out, "        else\n");
  fprintf(out, "            LINK_FLTK3GL=\"\"\n");
  fprintf(out, "            FLTK3GL_LIBNAME=\"\"\n");
  fprintf(out, "            FLTK3GL_DSONAME=\"\"\n");
  fprintf(out, "            GLDEMOS=\"\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        if test \"x$enable_threads\" != xno; then\n");
  fprintf(out, "            if test x$have_pthread = xyes; then\n");
  fprintf(out, "                AC_DEFINE(HAVE_PTHREAD)\n");
  fprintf(out, "            fi\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        THREADS=\"threads$EXEEXT\"\n");
  fprintf(out, "\n");
  fprintf(out, "        # Don't make symlinks since Windows is not case sensitive.\n");
  fprintf(out, "        if test \"x$with_links\" != xyes; then\n");
  fprintf(out, "                HLINKS=\"#\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "\n");
  fprintf(out, "    Darwin*)\n");
  fprintf(out, "        # MacOS X uses Cocoa for graphics.\n");
  fprintf(out, "        LIBS=\"$LIBS -framework Cocoa\"\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$have_pthread = xyes; then\n");
  fprintf(out, "            AC_DEFINE(HAVE_PTHREAD)\n");
  fprintf(out, "            THREADS=\"threads$EXEEXT\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$enable_gl != xno; then\n");
  fprintf(out, "            AC_DEFINE(HAVE_GL)\n");
  fprintf(out, "            AC_DEFINE(HAVE_GL_GLU_H)\n");
  fprintf(out, "            GLLIB=\"-framework AGL -framework OpenGL -framework ApplicationServices\"\n");
  fprintf(out, "        else\n");
  fprintf(out, "            LINK_FLTK3GL=\"\"\n");
  fprintf(out, "            FLTK3GL_LIBNAME=\"\"\n");
  fprintf(out, "            FLTK3GL_DSONAME=\"\"\n");
  fprintf(out, "            GLDEMOS=\"\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        # Don't make symlinks because HFS+ is not case sensitive...\n");
  fprintf(out, "        if test \"x$with_links\" != xyes; then\n");
  fprintf(out, "                HLINKS=\"#\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        # Some steps are only done for OS X package management\n");
  fprintf(out, "        OSX_ONLY=\n");
  fprintf(out, "\n");
  fprintf(out, "        # Install/Uninstall FLUID application\n");
  fprintf(out, "        INSTALL_DESKTOP=\"install-osx\"\n");
  fprintf(out, "        UNINSTALL_DESKTOP=\"uninstall-osx\"\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "\n");
  fprintf(out, "    *)\n");
  fprintf(out, "        # All others are UNIX/X11...\n");
  fprintf(out, "        # This includes Cygwin target combined with X11\n");
  fprintf(out, "        if test x$have_pthread = xyes; then\n");
  fprintf(out, "            AC_DEFINE(HAVE_PTHREAD)\n");
  fprintf(out, "            THREADS=\"threads$EXEEXT\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        dnl Check for X11...\n");
  fprintf(out, "        AC_PATH_XTRA\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$no_x = xyes; then\n");
  fprintf(out, "            AC_MSG_ERROR(Configure could not find required X11 libraries, aborting.)\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        if test \"x$X_PRE_LIBS\" != x; then\n");
  fprintf(out, "            AC_MSG_WARN(Ignoring libraries \\\"$X_PRE_LIBS\\\" requested by configure.)\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        LIBS=\"$LIBS -lX11 $X_EXTRA_LIBS\"\n");
  fprintf(out, "        CFLAGS=\"$CFLAGS $X_CFLAGS\"\n");
  fprintf(out, "        CXXFLAGS=\"$CXXFLAGS $X_CFLAGS\"\n");
  fprintf(out, "        LDFLAGS=\"$X_LIBS $LDFLAGS\"\n");
  fprintf(out, "        DSOFLAGS=\"$X_LIBS $DSOFLAGS\"\n");
  fprintf(out, "        AC_DEFINE(USE_X11)\n");
  fprintf(out, "        if test \"x$x_includes\" != x; then\n");
  fprintf(out, "            ac_cpp=\"$ac_cpp -I$x_includes\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        dnl Check for OpenGL unless disabled...\n");
  fprintf(out, "        GLLIB=\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$enable_gl != xno; then\n");
  fprintf(out, "            AC_SEARCH_LIBS(dlopen, dl)\n");
  fprintf(out, "            AC_CHECK_HEADER(GL/gl.h,\n");
  fprintf(out, "                AC_CHECK_LIB(GL, glXMakeCurrent, AC_DEFINE(HAVE_GL) GLLIB=\"-lGL\",\n");
  fprintf(out, "                    AC_CHECK_LIB(MesaGL,glXMakeCurrent, AC_DEFINE(HAVE_GL) GLLIB=\" -lMesaGL\",,\n");
  fprintf(out, "                        -lm),\n");
  fprintf(out, "                    -lm)\n");
  fprintf(out, "                AC_CHECK_LIB(GL, glXGetProcAddressARB,\n");
  fprintf(out, "                             AC_DEFINE(HAVE_GLXGETPROCADDRESSARB),, -lm)\n");
  fprintf(out, "            )\n");
  fprintf(out, "            AC_CHECK_HEADER(GL/glu.h,\n");
  fprintf(out, "                AC_DEFINE(HAVE_GL_GLU_H)\n");
  fprintf(out, "                if test x$ac_cv_lib_GL_glXMakeCurrent = xyes; then\n");
  fprintf(out, "                    GLLIB=\"-lGLU $GLLIB\"\n");
  fprintf(out, "                fi\n");
  fprintf(out, "                if test x$ac_cv_lib_MesaGL_glXMakeCurrent = xyes; then\n");
  fprintf(out, "                    GLLIB=\"-lMesaGLU $GLLIB\"\n");
  fprintf(out, "                fi\n");
  fprintf(out, "            )\n");
  fprintf(out, "\n");
  fprintf(out, "            if test x$ac_cv_lib_GL_glXMakeCurrent != xyes -a x$ac_cv_lib_MesaGL_glXMakeCurrent != xyes; then\n");
  fprintf(out, "                    LINK_FLTK3GL=\"\"\n");
  fprintf(out, "                    FLTK3GL_LIBNAME=\"\"\n");
  fprintf(out, "                    FLTK3GL_DSONAME=\"\"\n");
  fprintf(out, "                    GLDEMOS=\"\"\n");
  fprintf(out, "            fi\n");
  fprintf(out, "        else\n");
  fprintf(out, "            LINK_FLTK3GL=\"\"\n");
  fprintf(out, "            FLTK3GL_LIBNAME=\"\"\n");
  fprintf(out, "            FLTK3GL_DSONAME=\"\"\n");
  fprintf(out, "            GLDEMOS=\"\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        dnl Check for Xinerama support unless disabled...\n");
  fprintf(out, "        AC_ARG_ENABLE(xinerama, [  --enable-xinerama       turn on Xinerama support [default=yes]])\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$enable_xinerama != xno; then\n");
  fprintf(out, "            AC_CHECK_LIB(Xinerama,XineramaIsActive,\n");
  fprintf(out, "                    AC_DEFINE(HAVE_XINERAMA)\n");
  fprintf(out, "                    LIBS=\"-lXinerama $LIBS\")\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        dnl Check for the Xft library unless disabled...\n");
  fprintf(out, "        AC_ARG_ENABLE(xft, [  --enable-xft            turn on Xft support [default=yes]])\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$enable_xft != xno; then\n");
  fprintf(out, "            AC_PATH_PROG(FTCONFIG,freetype-config)\n");
  fprintf(out, "\n");
  fprintf(out, "            if test \"x$FTCONFIG\" != x; then\n");
  fprintf(out, "                CPPFLAGS=\"`$FTCONFIG --cflags` $CPPFLAGS\"\n");
  fprintf(out, "                CXXFLAGS=\"`$FTCONFIG --cflags` $CXXFLAGS\"\n");
  fprintf(out, "\n");
  fprintf(out, "                AC_CHECK_LIB(fontconfig, FcPatternCreate)\n");
  fprintf(out, "                AC_CHECK_HEADER(X11/Xft/Xft.h,\n");
  fprintf(out, "                    AC_CHECK_LIB(Xft, XftDrawCreate,\n");
  fprintf(out, "                        AC_DEFINE(USE_XFT)\n");
  fprintf(out, "                        LIBS=\"-lXft $LIBS\"))\n");
  fprintf(out, "            fi\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        dnl Check for the Xdbe extension unless disabled...\n");
  fprintf(out, "        AC_ARG_ENABLE(xdbe, [  --enable-xdbe           turn on Xdbe support [default=yes]])\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$enable_xdbe != xno; then\n");
  fprintf(out, "            AC_CHECK_HEADER(X11/extensions/Xdbe.h, AC_DEFINE(HAVE_XDBE),,\n");
  fprintf(out, "                [#include <X11/Xlib.h>])\n");
  fprintf(out, "            AC_CHECK_LIB(Xext, XdbeQueryExtension,\n");
  fprintf(out, "                LIBS=\"-lXext $LIBS\")\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "	dnl Check for the XRandR extension unless disabled...\n");
  fprintf(out, "        AC_ARG_ENABLE(xrandr, [  --enable-xrandr         turn on XRandR support [default=yes]])\n");
  fprintf(out, "\n");
  fprintf(out, "	if test x$enable_xrandr != xno; then\n");
  fprintf(out, "	    AC_CHECK_HEADER(X11/extensions/Xrandr.h, AC_DEFINE(HAVE_XRANDR),,\n");
  fprintf(out, "	        [#include <X11/Xlib.h>])\n");
  fprintf(out, "	    AC_CHECK_LIB(Xrandr, XRRQueryExtension,\n");
  fprintf(out, "		LIBS=\"-lXrandr $LIBS\")\n");
  fprintf(out, "	fi\n");
  fprintf(out, "\n");
  fprintf(out, "        dnl Check for overlay visuals...\n");
  fprintf(out, "        AC_PATH_PROG(XPROP, xprop)\n");
  fprintf(out, "        AC_CACHE_CHECK(for X overlay visuals, ac_cv_have_overlay,\n");
  fprintf(out, "            if test \"x$XPROP\" != x; then\n");
  fprintf(out, "                if $XPROP -root 2>/dev/null | grep -c \"SERVER_OVERLAY_VISUALS\" >/dev/null; then\n");
  fprintf(out, "                    ac_cv_have_overlay=yes\n");
  fprintf(out, "                else\n");
  fprintf(out, "                    ac_cv_have_overlay=no\n");
  fprintf(out, "                fi\n");
  fprintf(out, "            else\n");
  fprintf(out, "                ac_cv_have_overlay=no\n");
  fprintf(out, "            fi)\n");
  fprintf(out, "\n");
  fprintf(out, "        if test x$ac_cv_have_overlay = xyes; then\n");
  fprintf(out, "            AC_DEFINE(HAVE_OVERLAY)\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        # Make symlinks since UNIX/Linux is case sensitive,\n");
  fprintf(out, "        # but Cygwin in general not.\n");
  fprintf(out, "        case $uname in\n");
  fprintf(out, "            CYGWIN*)\n");
  fprintf(out, "                HLINKS=\"#\"\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "            *)\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        esac\n");
  fprintf(out, "        # Make symlinks since UNIX/Linux is case sensitive,\n");
  fprintf(out, "        # but only if explicitly configured (default=no)\n");
  fprintf(out, "        if test \"x$with_links\" != xyes; then\n");
  fprintf(out, "                HLINKS=\"#\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "\n");
  fprintf(out, "        # Install/Uninstall FLUID application support files\n");
  fprintf(out, "        INSTALL_DESKTOP=\"install-linux\"\n");
  fprintf(out, "        UNINSTALL_DESKTOP=\"uninstall-linux\"\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "esac\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(GLDEMOS)\n");
  fprintf(out, "AC_SUBST(GLLIB)\n");
  fprintf(out, "AC_SUBST(HLINKS)\n");
  fprintf(out, "AC_SUBST(OSX_ONLY)\n");
  fprintf(out, "AC_SUBST(THREADS)\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(INSTALL_DESKTOP)\n");
  fprintf(out, "AC_SUBST(UNINSTALL_DESKTOP)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Figure out the appropriate formatted man page extension...\n");
  fprintf(out, "case \"$uname\" in\n");
  fprintf(out, "    *BSD* | Darwin*)\n");
  fprintf(out, "        # *BSD\n");
  fprintf(out, "        CAT1EXT=0\n");
  fprintf(out, "        CAT3EXT=0\n");
  fprintf(out, "        CAT6EXT=0\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "    IRIX*)\n");
  fprintf(out, "        # SGI IRIX\n");
  fprintf(out, "        CAT1EXT=z\n");
  fprintf(out, "        CAT3EXT=z\n");
  fprintf(out, "        CAT6EXT=z\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "    *)\n");
  fprintf(out, "        # All others\n");
  fprintf(out, "        CAT1EXT=1\n");
  fprintf(out, "        CAT3EXT=3\n");
  fprintf(out, "        CAT6EXT=6\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "esac\n");
  fprintf(out, "\n");
  fprintf(out, "AC_SUBST(CAT1EXT)\n");
  fprintf(out, "AC_SUBST(CAT3EXT)\n");
  fprintf(out, "AC_SUBST(CAT6EXT)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Fix \"mandir\" variable...\n");
  fprintf(out, "if test \"$mandir\" = \"\\${prefix}/man\" -a \"$prefix\" = \"/usr\"; then\n");
  fprintf(out, "    case \"$uname\" in\n");
  fprintf(out, "        *BSD* | Darwin* | Linux*)\n");
  fprintf(out, "            # *BSD, Darwin, and Linux\n");
  fprintf(out, "            mandir=\"\\${prefix}/share/man\"\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        IRIX*)\n");
  fprintf(out, "            # SGI IRIX\n");
  fprintf(out, "            mandir=\"\\${prefix}/share/catman\"\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "    esac\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Fix \"libdir\" variable...\n");
  fprintf(out, "if test \"$prefix\" = NONE; then\n");
  fprintf(out, "    prefix=/usr/local\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "if test \"$exec_prefix\" = NONE; then\n");
  fprintf(out, "    exec_prefix=\"\\${prefix}\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "if test \"$uname\" = \"IRIX\" -a $uversion -ge 62 -a \"$libdir\" = \"\\${exec_prefix}/lib\" -a \"$exec_prefix\" = \"\\${prefix}\" -a \"$prefix\" = \"/usr\"; then\n");
  fprintf(out, "    libdir=\"/usr/lib32\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Define the command used to update the dependencies (this option\n");
  fprintf(out, "dnl mainly for FLTK core developers - not necessary for users)\n");
  fprintf(out, "MAKEDEPEND=\"\\$(CXX) -M\"\n");
  fprintf(out, "AC_SUBST(MAKEDEPEND)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Add warnings to compiler switches:\n");
  fprintf(out, "dnl do this last so messing with switches does not break tests\n");
  fprintf(out, "\n");
  fprintf(out, "if test -n \"$GCC\"; then\n");
  fprintf(out, "    # Show all standard warnings + unused variables, conversion errors,\n");
  fprintf(out, "    # and inlining problems when compiling...\n");
  fprintf(out, "    OPTIM=\"-Wall -Wunused -Wno-format-y2k $OPTIM\"\n");
  fprintf(out, "\n");
  fprintf(out, "    # The following additional warnings are useful for tracking down problems...\n");
  fprintf(out, "    #OPTIM=\"-Wshadow -Wconversion $OPTIM\"\n");
  fprintf(out, "\n");
  fprintf(out, "    # We know that Carbon is deprecated on OS X 10.4. To avoid hundreds of warnings\n");
  fprintf(out, "    # we will temporarily disable 'deprecated' warnings on OS X.\n");
  fprintf(out, "    if test \"$uname\" = \"Darwin\" -a $uversion -ge 800; then\n");
  fprintf(out, "        OPTIM=\"-Wno-deprecated-declarations $OPTIM\"\n");
  fprintf(out, "    fi\n");
  fprintf(out, "\n");
  fprintf(out, "    # Set the default compiler optimizations...\n");
  fprintf(out, "    if test -z \"$DEBUGFLAG\"; then\n");
  fprintf(out, "            #\n");
  fprintf(out, "        # Note: Can't use -fomit-frame-pointer - prevents tools like\n");
  fprintf(out, "        #       libsafe from working!\n");
  fprintf(out, "        #\n");
  fprintf(out, "        #       Don't use -fforce-mem, -fforce-addr, or -fcaller-saves.\n");
  fprintf(out, "        #       They all seem to make either no difference or enlarge\n");
  fprintf(out, "        #       the code by a few hundred bytes.\n");
  fprintf(out, "        #\n");
  fprintf(out, "        #       \"-Os\" seems to be the best compromise between speed and\n");
  fprintf(out, "        #       code size.  \"-O3\" and higher seem to make no effective\n");
  fprintf(out, "        #       difference in the speed of the code, but does bloat the\n");
  fprintf(out, "        #       library 10+%%.\n");
  fprintf(out, "        #\n");
  fprintf(out, "\n");
  fprintf(out, "        if test \"x$with_optim\" != x; then\n");
  fprintf(out, "            OPTIM=\"$with_optim $OPTIM\"\n");
  fprintf(out, "        else\n");
  fprintf(out, "            OPTIM=\"-Os $OPTIM\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "    fi\n");
  fprintf(out, "\n");
  fprintf(out, "    # Generate position-independent code when needed...\n");
  fprintf(out, "    if test $PICFLAG = 1; then\n");
  fprintf(out, "            OPTIM=\"$OPTIM -fPIC\"\n");
  fprintf(out, "    fi\n");
  fprintf(out, "\n");
  fprintf(out, "    # See if GCC supports -fno-exceptions...\n");
  fprintf(out, "    AC_MSG_CHECKING(if GCC supports -fno-exceptions)\n");
  fprintf(out, "    OLDCFLAGS=\"$CFLAGS\"\n");
  fprintf(out, "    CFLAGS=\"$CFLAGS -fno-exceptions\"\n");
  fprintf(out, "    AC_TRY_COMPILE(,,\n");
  fprintf(out, "        OPTIM=\"$OPTIM -fno-exceptions\"\n");
  fprintf(out, "        AC_MSG_RESULT(yes),\n");
  fprintf(out, "        AC_MSG_RESULT(no))\n");
  fprintf(out, "    CFLAGS=\"$OLDCFLAGS\"\n");
  fprintf(out, "\n");
  fprintf(out, "    # See if GCC supports -fno-strict-aliasing...\n");
  fprintf(out, "    AC_MSG_CHECKING(if GCC supports -fno-strict-aliasing)\n");
  fprintf(out, "    OLDCFLAGS=\"$CFLAGS\"\n");
  fprintf(out, "    CFLAGS=\"$CFLAGS -fno-strict-aliasing\"\n");
  fprintf(out, "    AC_TRY_COMPILE(,,\n");
  fprintf(out, "        OPTIM=\"$OPTIM -fno-strict-aliasing\"\n");
  fprintf(out, "        AC_MSG_RESULT(yes),\n");
  fprintf(out, "        AC_MSG_RESULT(no))\n");
  fprintf(out, "    CFLAGS=\"$OLDCFLAGS\"\n");
  fprintf(out, "\n");
  fprintf(out, "    # See if we are running Solaris; if so, try the -fpermissive option...\n");
  fprintf(out, "    # This option is required on some versions of Solaris to work around\n");
  fprintf(out, "    # bugs in the X headers up through Solaris 7.\n");
  fprintf(out, "    #\n");
  fprintf(out, "    # Unlike the other compiler/optimization settings, this one is placed\n");
  fprintf(out, "    # in CFLAGS and CXXFLAGS so that fltk3-config will provide the option\n");
  fprintf(out, "    # to clients - otherwise client apps will not compile properly...\n");
  fprintf(out, "    if test \"$uname\" = SunOS; then\n");
  fprintf(out, "        AC_MSG_CHECKING(if GCC supports -fpermissive)\n");
  fprintf(out, "\n");
  fprintf(out, "        OLDCFLAGS=\"$CFLAGS\"\n");
  fprintf(out, "        CFLAGS=\"$CFLAGS -fpermissive\"\n");
  fprintf(out, "        AC_TRY_COMPILE(,,\n");
  fprintf(out, "            CXXFLAGS=\"$CXXFLAGS -fpermissive\"\n");
  fprintf(out, "            AC_MSG_RESULT(yes),\n");
  fprintf(out, "            CFLAGS=\"$OLDCFLAGS\"\n");
  fprintf(out, "            AC_MSG_RESULT(no))\n");
  fprintf(out, "    fi\n");
  fprintf(out, "else\n");
  fprintf(out, "    case \"$uname\" in\n");
  fprintf(out, "        IRIX*)\n");
  fprintf(out, "            # Running some flavor of IRIX; see which version and\n");
  fprintf(out, "            # set things up according...\n");
  fprintf(out, "            if test \"$uversion\" -ge 62; then\n");
  fprintf(out, "                # We are running IRIX 6.2 or higher; uncomment the following\n");
  fprintf(out, "                # lines if you don't have IDO 7.2 or higher:\n");
  fprintf(out, "                #\n");
  fprintf(out, "                #     CXX=\"CC -n32 -mips3\"\n");
  fprintf(out, "                #     CC=\"cc -n32 -mips3\"\n");
  fprintf(out, "                #     LD=\"ld -n32 -mips3\"\n");
  fprintf(out, "                #     MAKEDEPEND=\"CC -M\"\n");
  fprintf(out, "\n");
  fprintf(out, "                if test \"x`grep abi=n32 /etc/compiler.defaults`\" = x; then\n");
  fprintf(out, "                        AC_MSG_WARN(FOR BEST RESULTS BEFORE COMPILING: setenv SGI_ABI \\\"-n32 -mips3\\\")\n");
  fprintf(out, "                fi\n");
  fprintf(out, "\n");
  fprintf(out, "                OPTIM=\"-fullwarn $OPTIM\"\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            if test -z \"$DEBUGFLAG\"; then\n");
  fprintf(out, "                if test \"x$with_optim\" != x; then\n");
  fprintf(out, "                    OPTIM=\"$with_optim $OPTIM\"\n");
  fprintf(out, "                else\n");
  fprintf(out, "                    OPTIM=\"-O2 $OPTIM\"\n");
  fprintf(out, "                    if test $uversion -gt 62; then\n");
  fprintf(out, "                        OPTIM=\"-OPT:Olimit=4000 $OPTIM\"\n");
  fprintf(out, "                    fi\n");
  fprintf(out, "                fi\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        HP-UX*)\n");
  fprintf(out, "            # Running HP-UX; these options should work for the HP compilers.\n");
  fprintf(out, "            if test -z \"$DEBUGFLAG\"; then\n");
  fprintf(out, "                if test \"x$with_optim\" != x; then\n");
  fprintf(out, "                    OPTIM=\"$with_optim $OPTIM\"\n");
  fprintf(out, "                else\n");
  fprintf(out, "                    OPTIM=\"+O2 $OPTIM\"\n");
  fprintf(out, "                fi\n");
  fprintf(out, "            fi\n");
  fprintf(out, "\n");
  fprintf(out, "            if test $PICFLAG = 1; then\n");
  fprintf(out, "                OPTIM=\"+z $OPTIM\"\n");
  fprintf(out, "            fi\n");
  fprintf(out, "\n");
  fprintf(out, "            CXXFLAGS=\"$CXXFLAGS +W336,501,736,740,749,829\"\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        OSF1*)\n");
  fprintf(out, "            # Running Digital/Tru64 UNIX; these options should work for the\n");
  fprintf(out, "            # Digital/Compaq/NewHP compilers.\n");
  fprintf(out, "            if test -z \"$DEBUGFLAG\"; then\n");
  fprintf(out, "                if test \"x$with_optim\" != x; then\n");
  fprintf(out, "                    OPTIM=\"$with_optim $OPTIM\"\n");
  fprintf(out, "                else\n");
  fprintf(out, "                    OPTIM=\"-O2 $OPTIM\"\n");
  fprintf(out, "                fi\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        SunOS*)\n");
  fprintf(out, "            # Solaris\n");
  fprintf(out, "            if test -z \"$DEBUGFLAG\"; then\n");
  fprintf(out, "                if test \"x$with_optim\" != x; then\n");
  fprintf(out, "                    OPTIM=\"$with_optim $OPTIM\"\n");
  fprintf(out, "                else\n");
  fprintf(out, "                    OPTIM=\"-xO3 $OPTIM\"\n");
  fprintf(out, "                fi\n");
  fprintf(out, "            fi\n");
  fprintf(out, "\n");
  fprintf(out, "            if test $PICFLAG = 1; then\n");
  fprintf(out, "                OPTIM=\"-KPIC $OPTIM\"\n");
  fprintf(out, "            fi\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        AIX*)\n");
  fprintf(out, "            if test -z \"$DEBUGFLAG\"; then\n");
  fprintf(out, "                if test \"x$with_optim\" != x; then\n");
  fprintf(out, "                    OPTIM=\"$with_optim $OPTIM\"\n");
  fprintf(out, "                else\n");
  fprintf(out, "                    OPTIM=\"-O2 $OPTIM\"\n");
  fprintf(out, "                fi\n");
  fprintf(out, "            fi\n");
  fprintf(out, "\n");
  fprintf(out, "            AC_MSG_WARN(The AIX C and C++ compilers are known not to correctly compile the FLTK library.)\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "        *)\n");
  fprintf(out, "            # Running some other operating system; inform the user they\n");
  fprintf(out, "            # should contribute the necessary options via the STR form..\n");
  fprintf(out, "            AC_MSG_WARN(Building FLTK with default compiler optimizations)\n");
  fprintf(out, "            AC_MSG_WARN(Send the FLTK developers your uname and compiler options via http://www.fltk.org/str.php)\n");
  fprintf(out, "            ;;\n");
  fprintf(out, "    esac\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "OPTIM=\"$DEBUGFLAG $OPTIM\"\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Take archflags away from CFLAGS (makefiles use ARCHFLAGS explicitly)\n");
  fprintf(out, "if test `uname` = Darwin; then\n");
  fprintf(out, "  if test \"x$with_archflags\" != x ; then\n");
  fprintf(out, "    CFLAGS=\"`echo $CFLAGS | sed -e \"s/$with_archflags//g\"`\"\n");
  fprintf(out, "  fi\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Define the FLTK documentation directory...\n");
  fprintf(out, "case $uname in\n");
  fprintf(out, "  MINGW*)\n");
  fprintf(out, "     # Determine the path where MSys has /usr installed\n");
  fprintf(out, "         msyspath=`mount | grep '\\/usr' | grep -v '\\/usr\\/bin' | cut -d ' ' -f -1 | sed -e 's/\\\\\\/\\// g'`\n");
  fprintf(out, "     # Then substitute that in the WIN32 path instead of /usr\n");
  fprintf(out, "         AC_DEFINE_UNQUOTED(FLTK_DOCDIR, \"$msyspath/local/share/doc/fltk\")\n");
  fprintf(out, "    ;;\n");
  fprintf(out, "  *)\n");
  fprintf(out, "    if test x$prefix = xNONE; then\n");
  fprintf(out, "        AC_DEFINE_UNQUOTED(FLTK_DOCDIR, \"/usr/local/share/doc/fltk\")\n");
  fprintf(out, "    else\n");
  fprintf(out, "        AC_DEFINE_UNQUOTED(FLTK_DOCDIR, \"$prefix/share/doc/fltk\")\n");
  fprintf(out, "    fi\n");
  fprintf(out, "    ;;\n");
  fprintf(out, "esac\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Define the FLTK data directory...\n");
  fprintf(out, "if test x$prefix = xNONE; then\n");
  fprintf(out, "    AC_DEFINE_UNQUOTED(FLTK_DATADIR, \"/usr/local/share/fltk\")\n");
  fprintf(out, "else\n");
  fprintf(out, "    AC_DEFINE_UNQUOTED(FLTK_DATADIR, \"$prefix/share/fltk\")\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Summarize results of configure tests...\n");
  fprintf(out, "echo \"\"\n");
  fprintf(out, "echo \"Configuration Summary\"\n");
  fprintf(out, "echo \"-------------------------------------------------------------------------\"\n");
  fprintf(out, "\n");
  fprintf(out, "case $uname_GUI in\n");
  fprintf(out, "    CYGWIN* | MINGW*)\n");
  fprintf(out, "        graphics=\"GDI\"\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "    Darwin*)\n");
  fprintf(out, "        graphics=\"Quartz\"\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "    *)\n");
  fprintf(out, "        graphics=\"X11\"\n");
  fprintf(out, "        if test x$enable_xft != xno; then\n");
  fprintf(out, "            graphics=\"$graphics+Xft\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "        if test x$enable_xdbe != xno; then\n");
  fprintf(out, "            graphics=\"$graphics+Xdbe\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "	if test x$enable_xrandr != xno; then\n");
  fprintf(out, "	    graphics=\"$graphics+Xrandr\"\n");
  fprintf(out, "	fi\n");
  fprintf(out, "        if test x$enable_xinerama != xno; then\n");
  fprintf(out, "            graphics=\"$graphics+Xinerama\"\n");
  fprintf(out, "        fi\n");
  fprintf(out, "        ;;\n");
  fprintf(out, "esac\n");
  fprintf(out, "\n");
  fprintf(out, "echo \"    Directories: prefix=$prefix\"\n");
  fprintf(out, "echo \"                 bindir=$bindir\"\n");
  fprintf(out, "echo \"                 datadir=$datadir\"\n");
  fprintf(out, "echo \"                 datarootdir=$datarootdir\"\n");
  fprintf(out, "echo \"                 exec_prefix=$exec_prefix\"\n");
  fprintf(out, "echo \"                 includedir=$includedir\"\n");
  fprintf(out, "echo \"                 libdir=$libdir\"\n");
  fprintf(out, "echo \"                 mandir=$mandir\"\n");
  fprintf(out, "case $uname in\n");
  fprintf(out, "  MINGW*)\n");
  fprintf(out, "    echo \"                 MSys docpath=$msyspath/local/share/doc/fltk\"\n");
  fprintf(out, "  ;;\n");
  fprintf(out, "esac\n");
  fprintf(out, "echo \"       Graphics: $graphics\"\n");
  fprintf(out, "\n");
  fprintf(out, "if test x$JPEG = x; then\n");
  fprintf(out, "    echo \"Image Libraries: JPEG=System\"\n");
  fprintf(out, "else\n");
  fprintf(out, "    echo \"Image Libraries: JPEG=Builtin\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "if test x$PNG = x; then\n");
  fprintf(out, "    echo \"                 PNG=System\"\n");
  fprintf(out, "else\n");
  fprintf(out, "    echo \"                 PNG=Builtin\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "if test x$ZLIB = x; then\n");
  fprintf(out, "    echo \"                 ZLIB=System\"\n");
  fprintf(out, "else\n");
  fprintf(out, "    echo \"                 ZLIB=Builtin\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "if test x$enable_largefile != xno; then\n");
  fprintf(out, "    echo \"    Large Files: YES\"\n");
  fprintf(out, "else\n");
  fprintf(out, "    echo \"    Large Files: NO\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "if test x$GLDEMOS = x; then\n");
  fprintf(out, "    echo \"         OpenGL: NO\"\n");
  fprintf(out, "else\n");
  fprintf(out, "    echo \"         OpenGL: YES\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "if test x$THREADS = x; then\n");
  fprintf(out, "    echo \"        Threads: NO\"\n");
  fprintf(out, "else\n");
  fprintf(out, "    echo \"        Threads: YES\"\n");
  fprintf(out, "fi\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Write all of the files...\n");
  fprintf(out, "AC_CONFIG_HEADER(include/config.h:include/configh.in)\n");
  fprintf(out, "AC_OUTPUT(makeinclude fltk.list fltk3-config fltk.spec include/fltk3/Makefile)\n");
  fprintf(out, "\n");
  fprintf(out, "dnl Make sure the fltk3-config script is executable...\n");
  fprintf(out, "chmod +x fltk3-config\n");
  write_footer(out, "dnl");
  return 0;
}

static int write_fluid_makefile(FILE *out, Fl_Workspace_Type *workspace, const char *path, const char *base_dir, const char *tgt_base, Fl_Target_Type *first_target) {
  
  Fl_Target_Type *tgt = Fl_Target_Type::find("Fluid");
  Fl_Target_Dependency_Type *dep;
  Fl_File_Type * file;
  
  write_header(out, "FLUID makefile for the Fast Light Tool Kit (FLTK).");
  fprintf(out, "FLUID_CPPFILES =");
  for (file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->builds_in(FL_ENV_MAKE)) {
      if (file->file_is_fluid_ui()) {
        fprintf(out, " \\\n\t");
        write_file(out, file, base_dir, tgt_base, ".cxx");
      } else if (file->file_is_code()) {
        fprintf(out, " \\\n\t");
        write_file(out, file, base_dir, tgt_base);
      }
    }
  }
  fprintf(out, "\n\n");
  fprintf(out, "################################################################\n");
  fprintf(out, "\n");
  fprintf(out, "FLUID_OBJECTS = $(FLUID_CPPFILES:.cxx=.o)\n");
  fprintf(out, "\n");
  fprintf(out, "include ../makeinclude\n");
  fprintf(out, "\n");
  fprintf(out, "all:\t$(FLUID) fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "fluid$(EXEEXT): $(FLUID_OBJECTS)");
  for (dep = Fl_Target_Dependency_Type::first_dependency(tgt); dep; dep = dep->next_dependency(tgt)) {
    if (dep->builds_in(FL_ENV_MAKE)) {
      Fl_Target_Type *tgt_dep = Fl_Target_Type::find(dep->name());
      fprintf(out, " $(%s_LIBNAME)", tgt_dep->caps_name());
    }
  }
  fprintf(out, "\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ $(FLUID_OBJECTS) $(LINK_FLTK3IMAGES) $(LINK_FLTK3PNG) $(LINK_FLTK3JPEG) $(LINK_FLTK3ZLIB) $(LINK_FLTK3) $(LDLIBS)\n");
  fprintf(out, "\t$(OSX_ONLY) $(INSTALL_BIN) fluid fluid.app/Contents/MacOS\n");
  fprintf(out, "\n");
  fprintf(out, "fluid-shared$(EXEEXT):\t$(FLUID_OBJECTS)");
  for (dep = Fl_Target_Dependency_Type::first_dependency(tgt); dep; dep = dep->next_dependency(tgt)) {
    if (dep->builds_in(FL_ENV_MAKE)) {
      Fl_Target_Type *tgt_dep = Fl_Target_Type::find(dep->name());
      fprintf(out, " ../src/$(%s_DSONAME)", tgt_dep->caps_name());
    }
  }
  fprintf(out, "\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ $(FLUID_OBJECTS) $(LINKSHARED) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "clean:\n");
  fprintf(out, "\t-$(RM) *.o core.* *~ *.bck *.bck\n");
  fprintf(out, "\t-$(RM) core fluid$(EXEEXT) fluid-shared$(EXEEXT)\n");
  fprintf(out, "\t-$(RM) fluid.app/Contents/MacOS/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "depend:\t$(FLUID_CPPFILES)\n");
  fprintf(out, "\tmakedepend -Y -I../include -f makedepend $(FLUID_CPPFILES)\n");
  fprintf(out, "\n");
  fprintf(out, "# Automatically generated dependencies...\n");
  fprintf(out, "include makedepend\n");
  fprintf(out, "\n");
  fprintf(out, "install: all\n");
  fprintf(out, "\techo \"Installing FLUID in $(DESTDIR)$(bindir)...\"\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)$(bindir)\n");
  fprintf(out, "\t$(INSTALL_BIN) $(FLUID) $(DESTDIR)$(bindir)/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "install-linux:\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/usr/share/applications\n");
  fprintf(out, "\t$(INSTALL_DATA) fluid.desktop  $(DESTDIR)/usr/share/applications\n");
  fprintf(out, "\tfor size in 16 32 48 64 128; do \\\n");
  fprintf(out, "\t\tif test ! -d $(DESTDIR)/usr/share/icons/hicolor/$${size}x$${size}/apps; then \\\n");
  fprintf(out, "\t\t\t$(INSTALL_DIR) $(DESTDIR)/usr/share/icons/hicolor/$${size}x$${size}/apps; \\\n");
  fprintf(out, "\t\tfi; \\\n");
  fprintf(out, "\t\t$(INSTALL_DATA) icons/fluid-$$size.png $(DESTDIR)/usr/share/icons/hicolor/$${size}x$${size}/apps/fluid.png; \\\n");
  fprintf(out, "\tdone\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/usr/share/mimelnk/application\n");
  fprintf(out, "\t$(INSTALL_DATA) x-fluid.desktop  $(DESTDIR)/usr/share/mimelnk/application\n");
  fprintf(out, "\n");
  fprintf(out, "install-osx:\n");
  fprintf(out, "\techo Installing Fluid in $(DESTDIR)/Applications...\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/Applications/fluid.app\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/Applications/fluid.app/Contents\n");
  fprintf(out, "\t$(INSTALL_DATA) fluid.app/Contents/Info.plist $(DESTDIR)/Applications/fluid.app/Contents/Info.plist\n");
  fprintf(out, "\t$(INSTALL_DATA) fluid.app/Contents/PkgInfo $(DESTDIR)/Applications/fluid.app/Contents/PkgInfo\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/Applications/fluid.app/Contents/MacOS\n");
  fprintf(out, "\t$(RM) $(DESTDIR)/Applications/fluid.app/Contents/MacOS/fluid\n");
  fprintf(out, "\t$(LN) $(bindir)/fluid $(DESTDIR)/Applications/fluid.app/Contents/MacOS/fluid\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/Applications/fluid.app/Contents/Resources\n");
  fprintf(out, "\t$(INSTALL_DATA) fluid.app/Contents/Resources/fluid.icns $(DESTDIR)/Applications/fluid.app/Contents/Resources\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall:\n");
  fprintf(out, "\t$(RM) $(DESTDIR)$(bindir)/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall-linux:\n");
  fprintf(out, "\t$(RM) $(DESTDIR)/usr/share/applications/fluid.desktop\n");
  fprintf(out, "\t$(RM) $(DESTDIR)/usr/share/icons/hicolor/*/fluid.png\n");
  fprintf(out, "\t$(RM) $(DESTDIR)/usr/share/mimelnk/application/x-fluid.desktop\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall-osx:\n");
  fprintf(out, "\t$(RM) -r $(DESTDIR)/Applications/fluid.app\n");
  fprintf(out, "\n");
  fprintf(out, "\n");
  fprintf(out, "#\n");
  fprintf(out, "# Note: The rebuild target can only be used if you have the original .fl\n");
  fprintf(out, "#       files.  This is normally only used by the FLTK maintainers...\n");
  fprintf(out, "#\n");
  fprintf(out, "\n");
  fprintf(out, "rebuild:\n");
  fprintf(out, "\t./fluid -c about_panel.fl\n");
  fprintf(out, "\t./fluid -c alignment_panel.fl\n");
  fprintf(out, "\t./fluid -c function_panel.fl\n");
  fprintf(out, "\t./fluid -c print_panel.fl\n");
  fprintf(out, "\t./fluid -c template_panel.fl\n");
  fprintf(out, "\t./fluid -c widget_panel.fl\n");
  fprintf(out, "\n");
  write_footer(out);
  return 0;
}

static int write_fltk_makefile(FILE *out, Fl_Workspace_Type *workspace, const char *path, const char *base_dir, const char *tgt_base, Fl_Target_Type *first_target) {
  
  Fl_Target_Type *tgt;
  
  write_header(out, "Library makefile for the Fast Light Tool Kit (FLTK).");
  
  // list all CPP files for all targets
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0 && target_has_cplusplus_code(tgt)) {
      fprintf(out, "%s_CPPFILES =", tgt->caps_name());
      Fl_File_Type *file;
      for (file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
        if (file->builds_in(FL_ENV_MAKE) && file->file_is_cplusplus_code()) {
          fprintf(out, " \\\n\t");
          write_file(out, file, base_dir, tgt_base);
        }
      }
      fprintf(out, "\n\n");
    }
  }
  
  // list all ObjC files for all targets
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0 && target_has_objc_code(tgt)) {
      fprintf(out, "%s_OBJCPPFILES =", tgt->caps_name());
      Fl_File_Type *file;
      for (file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
        if (file->builds_in(FL_ENV_MAKE) && file->file_is_objc_code()) {
          fprintf(out, " \\\n\t");
          write_file(out, file, base_dir, tgt_base);
        }
      }
      fprintf(out, "\n\n");
    }
  }
  
  // list all C files for all targets
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0 && target_has_c_code(tgt)) {
      fprintf(out, "%s_CFILES =", tgt->caps_name());
      Fl_File_Type *file;
      for (file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
        if (file->builds_in(FL_ENV_MAKE) && file->file_is_c_code()) {
          fprintf(out, " \\\n\t");
          write_file(out, file, base_dir, tgt_base);
        }
      }
      fprintf(out, "\n\n");
    }
  }
  
  fprintf(out, "################################################################\n");
  fprintf(out, "FLTKFLAGS = -DFL_LIBRARY\n");
  fprintf(out, "include ../makeinclude\n");
  fprintf(out, "\n");
  
  // ObjC files only compile on OS X:
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0 && target_has_objc_code(tgt)) {
      fprintf(out, "%s_MMFILES = $(shell if test $(USEMMFILES) = Yes; then echo $(%s_OBJCPPFILES); fi)\n", tgt->caps_name(), tgt->caps_name());
    }
  }
  fprintf(out, "\n");
  
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0) {
      fprintf(out, "%s_OBJECTS =", tgt->caps_name());
      if (target_has_cplusplus_code(tgt)) {
        fprintf(out, " $(%s_CPPFILES:.cxx=.o)", tgt->caps_name());
      }
      if (target_has_objc_code(tgt)) {
        fprintf(out, " $(%s_MMFILES:.mm=.o)", tgt->caps_name());
      }
      if (target_has_c_code(tgt)) {
        fprintf(out, " $(%s_CFILES:.c=.o)", tgt->caps_name());
      }
      fprintf(out, "\n");
    }
  }
  fprintf(out, "\n");

  fprintf(out, "all:");
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0) {
      fprintf(out, " \\\n\t$(%s_LIBNAME) $(%s_DSONAME)", tgt->caps_name(), tgt->caps_name());
    }
  }
  fprintf(out, "\n\n");
  
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0) {
      fprintf(out, "$(%s_LIBNAME): $(%s_OBJECTS)\n", tgt->caps_name(), tgt->caps_name());
      fprintf(out, "\techo $(LIBCOMMAND) $@ ...\n");
      fprintf(out, "\t$(RM) $@\n");
      fprintf(out, "\t$(LIBCOMMAND) $@ $(%s_OBJECTS)\n", tgt->caps_name());
      fprintf(out, "\t$(RANLIB) $@\n");
      fprintf(out, "\n");
    }
  }
  
  fprintf(out, "libfltk.so.1.3: $(FLTK3_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK3_OBJECTS)\n");
  fprintf(out, "\t$(RM) libfltk.so\n");
  fprintf(out, "\t$(LN) libfltk.so.1.3 libfltk.so\n");
  fprintf(out, "\n");
  
  
  fprintf(out, "libfltk.sl.1.3: $(FLTK3_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK3_OBJECTS)\n");
  fprintf(out, "\t$(RM) libfltk.sl\n");
  fprintf(out, "\t$(LN) libfltk.sl.1.3 libfltk.sl\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk.1.3.dylib: $(FLTK3_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ \\\n");
  fprintf(out, "\t\t-install_name $(libdir)/$@ \\\n");
  fprintf(out, "\t\t-current_version 1.3.0 \\\n");
  fprintf(out, "\t\t-compatibility_version 1.3.0 \\\n");
  fprintf(out, "\t\t$(FLTK3_OBJECTS) $(LDLIBS)\n");
  fprintf(out, "\t$(RM) libfltk.dylib\n");
  fprintf(out, "\t$(LN) libfltk.1.3.dylib libfltk.dylib\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_s.a: $(FLTK3_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) libfltk_s.o ...\n");
  fprintf(out, "\t$(DSOCOMMAND) libfltk_s.o $(FLTK3_OBJECTS) $(IMAGELIBS)\n");
  fprintf(out, "\techo $(LIBCOMMAND) libfltk_s.a libfltk_s.o\n");
  fprintf(out, "\t$(RM) $@\n");
  fprintf(out, "\t$(LIBCOMMAND) libfltk_s.a libfltk_s.o\n");
  fprintf(out, "\t$(CHMOD) +x libfltk_s.a\n");
  fprintf(out, "\n");
  /*
  fprintf(out, "$(FLTK3GL_LIBNAME): $(FLTK3GL_OBJECTS)\n");
  fprintf(out, "\techo $(LIBCOMMAND) $@ ...\n");
  fprintf(out, "\t$(RM) $@\n");
  fprintf(out, "\t$(LIBCOMMAND) $@ $(FLTK3GL_OBJECTS)\n");
  fprintf(out, "\t$(RANLIB) $@\n");
  fprintf(out, "\n");
  */
  fprintf(out, "libfltk_gl.so.1.3: $(FLTK3GL_OBJECTS) libfltk.so.1.3\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK3GL_OBJECTS) -L. -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_gl.so\n");
  fprintf(out, "\t$(LN) libfltk_gl.so.1.3 libfltk_gl.so\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_gl.sl.1.3: $(FLTK3GL_OBJECTS) libfltk.sl.1.3\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK3GL_OBJECTS) -L. -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_gl.sl\n");
  fprintf(out, "\t$(LN) libfltk_gl.sl.1.3 libfltk_gl.sl\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_gl.1.3.dylib: $(FLTK3GL_OBJECTS) libfltk.1.3.dylib\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ \\\n");
  fprintf(out, "\t\t-install_name $(libdir)/$@ \\\n");
  fprintf(out, "\t\t-current_version 1.3.0 \\\n");
  fprintf(out, "\t\t-compatibility_version 1.3.0 \\\n");
  fprintf(out, "\t\t$(FLTK3GL_OBJECTS) -L. $(GLDLIBS) -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_gl.dylib\n");
  fprintf(out, "\t$(LN) libfltk_gl.1.3.dylib libfltk_gl.dylib\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_gl_s.a: $(FLTK3GL_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) libfltk_gl_s.o ...\n");
  fprintf(out, "\t$(DSOCOMMAND) libfltk_gl_s.o $(FLTK3GL_OBJECTS)\n");
  fprintf(out, "\techo $(LIBCOMMAND) libfltk_gl_s.a libfltk_gl_s.o\n");
  fprintf(out, "\t$(RM) $@\n");
  fprintf(out, "\t$(LIBCOMMAND) libfltk_gl_s.a libfltk_gl_s.o\n");
  fprintf(out, "\t$(CHMOD) +x libfltk_gl_s.a\n");
  fprintf(out, "\n");
  /*
  fprintf(out, "$(FLTK3IMAGES_LIBNAME): $(FLTK3IMAGES_OBJECTS)\n");
  fprintf(out, "\techo $(LIBCOMMAND) $@ ...\n");
  fprintf(out, "\t$(RM) $@\n");
  fprintf(out, "\t$(LIBCOMMAND) $@ $(FLTK3IMAGES_OBJECTS)\n");
  fprintf(out, "\t$(RANLIB) $@\n");
  fprintf(out, "\n");
   */
  fprintf(out, "libfltk_images.so.1.3: $(FLTK3IMAGES_OBJECTS) libfltk.so.1.3\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK3IMAGES_OBJECTS) -L. $(IMAGELIBS) -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_images.so\n");
  fprintf(out, "\t$(LN) libfltk_images.so.1.3 libfltk_images.so\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_images.sl.1.3: $(FLTK3IMAGES_OBJECTS) libfltk.sl.1.3\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK3IMAGES_OBJECTS) -L. $(IMAGELIBS) -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_images.sl\n");
  fprintf(out, "\t$(LN) libfltk_images.sl.1.3 libfltk_images.sl\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_images.1.3.dylib: $(FLTK3IMAGES_OBJECTS) libfltk.1.3.dylib\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ \\\n");
  fprintf(out, "\t\t-install_name $(libdir)/$@ \\\n");
  fprintf(out, "\t\t-current_version 1.3.0 \\\n");
  fprintf(out, "\t\t-compatibility_version 1.3.0 \\\n");
  fprintf(out, "\t\t$(FLTK3IMAGES_OBJECTS)  -L. $(LDLIBS) $(IMAGELIBS) -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_images.dylib\n");
  fprintf(out, "\t$(LN) libfltk_images.1.3.dylib libfltk_images.dylib\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_images_s.a: $(FLTK3IMAGES_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) libfltk_images_s.o ...\n");
  fprintf(out, "\t$(DSOCOMMAND) libfltk_images_s.o $(FLTK3IMAGES_OBJECTS)\n");
  fprintf(out, "\techo $(LIBCOMMAND) libfltk_images_s.a libfltk_images_s.o\n");
  fprintf(out, "\t$(RM) $@\n");
  fprintf(out, "\t$(LIBCOMMAND) libfltk_images_s.a libfltk_images_s.o\n");
  fprintf(out, "\t$(CHMOD) +x libfltk_images_s.a\n");
  fprintf(out, "\n");
  fprintf(out, "#-----------------------------------------------------------------\n");
  fprintf(out, "# - the import libraries libfltk*.dll.a and the .dll files\n");
  fprintf(out, "#   are created from the libfltk*.a files. They are built\n");
  fprintf(out, "#   into the src dir.\n");
  fprintf(out, "# - The _images, _gl and dlls must be linked\n");
  fprintf(out, "#   against the import libraries in the src dir.\n");
  fprintf(out, "#-----------------------------------------------------------------\n");
  fprintf(out, "\n");
  fprintf(out, "#-----------------------------------------------------------------\n");
  fprintf(out, "# cygwin GDI shared libraries\n");
  fprintf(out, "#-----------------------------------------------------------------\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltknox-1.3.dll: $(FLTK3_LIBNAME)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK3_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK3_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk.dll.a $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltknox_gl-1.3.dll: $(FLTK3GL_LIBNAME) cygfltknox-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK3GL_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK3GL_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_gl.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltknox_images-1.3.dll: $(FLTK3IMAGES_LIBNAME) cygfltknox-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK3IMAGES_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK3IMAGES_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_images.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk -Wl,--exclude-libs -Wl,libfltk_png.a \\\n");
  fprintf(out, "\t\t\t$(IMAGELIBS) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "#-----------------------------------------------------------------\n");
  fprintf(out, "# cygwin X11 shared libraries\n");
  fprintf(out, "#-----------------------------------------------------------------\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltk-1.3.dll: $(FLTK3_LIBNAME)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK3_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK3_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk.dll.a $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltk_gl-1.3.dll: $(FLTK3GL_LIBNAME) cygfltk-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK3GL_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK3GL_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_gl.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltk_images-1.3.dll: $(FLTK3IMAGES_LIBNAME) cygfltk-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK3IMAGES_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK3IMAGES_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_images.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk -Wl,--exclude-libs -Wl,libfltk_png.a \\\n");
  fprintf(out, "\t\t\t$(IMAGELIBS) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "mgwfltknox-1.3.dll: $(FLTK3_LIBNAME)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK3_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK3_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk.dll.a $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "mgwfltknox_gl-1.3.dll: $(FLTK3GL_LIBNAME) mgwfltknox-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK3GL_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK3GL_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_gl.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "#-----------------------------------------------------\n");
  fprintf(out, "# See STR #1585 for --exclude-libs\n");
  fprintf(out, "#-----------------------------------------------------\n");
  fprintf(out, "\n");
  fprintf(out, "mgwfltknox_images-1.3.dll: $(FLTK3IMAGES_LIBNAME) mgwfltknox-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK3IMAGES_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK3IMAGES_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_images.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk -Wl,--exclude-libs -Wl,libfltk_png.a \\\n");
  fprintf(out, "\t\t\t$(IMAGELIBS) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "clean:\n");
  fprintf(out, "\t-$(RM) *.o */*.o */*/*.o *.dll.a core.* *~ *.bak *.bck\n");
  fprintf(out, "\t-$(RM) $(FLTK3_DSONAME) $(FLTK3GL_DSONAME) $(FLTK3IMAGES_DSONAME) \\\n");
  fprintf(out, "\t\t$(FLTK3_LIBNAME) $(FLTK3GL_LIBNAME) \\\n");
  fprintf(out, "\t\t$(FLTK3IMAGES_LIBNAME) \\\n");
  fprintf(out, "\t\tlibfltk.so libfltk_gl.so libfltk_images.so \\\n");
  fprintf(out, "\t\tlibfltk.sl libfltk_gl.sl libfltk_images.sl \\\n");
  fprintf(out, "\t\tlibfltk.dylib \\\n");
  fprintf(out, "\t\tlibfltk_gl.dylib libfltk_images.dylib \\\n");
  fprintf(out, "\t\tcmap\n");
  fprintf(out, "\n");
  fprintf(out, "depend:\t$(FLTK3_CPPFILES) $(FLTK3_MMFILES) $(FLTK3GL_CPPFILES) $(FLTK3IMAGES_CPPFILES) $(FLTK3_CFILES)\n");
  fprintf(out, "\tmakedepend -Y -I../include -f makedepend $(FLTK3_CPPFILES) $(FLTK3_MMFILES) \\\n");
  fprintf(out, "\t\t$(FLTK3GL_CPPFILES) $(FLTK3IMAGES_CPPFILES) $(FLTK3_CFILES)\n");
  fprintf(out, "\n");
  fprintf(out, "# Automatically generated dependencies... generated on a Linux/Unix host !\n");
  fprintf(out, "include makedepend\n");
  fprintf(out, "\n");
  fprintf(out, "# These dependencies aren't part of the makedepend file since\n");
  fprintf(out, "# they are part of the WIN32 and MacOS code base...\n");
  fprintf(out, "# Please add only non-Linux/Unix files or such that are optional\n");
  fprintf(out, "# (like \"*xft*\") here:\n");
  fprintf(out, "Fl_get_key.o:\tFl_get_key_mac.cxx Fl_get_key_win32.cxx\n");
  fprintf(out, "Fl_Native_File_Chooser.o : Fl_Native_File_Chooser_WIN32.cxx\n");
  fprintf(out, "Fl_Native_File_Chooser_MAC.o:\tFl_Native_File_Chooser_MAC.mm\n");
  fprintf(out, "Fl.o:\t\tFl_win32.cxx\n");
  fprintf(out, "Fl_cocoa.o:\tFl_cocoa.mm\n");
  fprintf(out, "fl_color.o:\tfl_color_mac.cxx fl_color_win32.cxx\n");
  fprintf(out, "fl_dnd.o:\tfl_dnd_win32.cxx fl_dnd_x.cxx\n");
  fprintf(out, "fl_draw_image.o: fl_draw_image_mac.cxx fl_draw_image_win32.cxx\n");
  fprintf(out, "fl_font.o:\tfl_font_mac.cxx fl_font_x.cxx fl_font_xft.cxx fl_font_win32.cxx\n");
  fprintf(out, "fl_read_image.o: fl_read_image_mac.cxx fl_read_image_win32.cxx\n");
  fprintf(out, "fl_set_fonts.o:\tfl_set_fonts_mac.cxx fl_set_fonts_x.cxx \\\n");
  fprintf(out, "\t\tfl_set_fonts_xft.cxx fl_set_fonts_win32.cxx\n");
  fprintf(out, "Fl_Printer.o: Fl_GDI_Printer.cxx Fl_PostScript.cxx\n");
  fprintf(out, "Fl_Quartz_Printer.o:\tFl_Quartz_Printer.mm\n");
  fprintf(out, "fl_arci.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_arg.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_ask.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Bitmap.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_color.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_cursor.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_dnd.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Double_Window.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_draw_image.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_draw_image_mac.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_draw_image_win32.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_draw_pixmap.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_encoding_latin1.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_encoding_mac_roman.o:   ../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_File_Chooser2.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_font.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_get_key.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_get_system_colors.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Gl_Choice.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Gl_Overlay.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Gl_Window.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_grab.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Image.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_line_style.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Menu_Window.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_overlay.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_overlay_visual.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Overlay_Window.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_own_colormap.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Pixmap.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Printer.o:  ../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_read_image.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_read_image_mac.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_read_image_win32.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_rect.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_scroll_area.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_set_font.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_set_fonts.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_shortcut.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "fl_vertex.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_visual.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Window_fullscreen.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_Window_iconize.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "Fl_x.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "gl_draw.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "gl_start.o:\t../fltk3/osx.h ../fltk3/win32.h\n");
  fprintf(out, "\n");
  fprintf(out, "################################################################\n");
  fprintf(out, "\n");
  fprintf(out, "install: $(FLTK3_LIBNAME) $(FLTK3_DSONAME) \\\n");
  fprintf(out, "\t$(FLTK3GL_LIBNAME) $(FLTK3GL_DSONAME) \\\n");
  fprintf(out, "\t$(FLTK3IMAGES_LIBNAME) $(FLTK3IMAGES_DSONAME) \n");
  fprintf(out, "\techo \"Installing libraries in $(DESTDIR)$(libdir)...\"\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)$(libdir)\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)$(bindir)\n");
  fprintf(out, "\t$(RM) $(DESTDIR)$(libdir)/$(FLTK3_LIBBASENAME)\n");
  fprintf(out, "\t$(INSTALL_LIB) $(FLTK3_LIBNAME) $(DESTDIR)$(libdir)\n");
  fprintf(out, "\t$(INSTALL_LIB) $(FLTK3IMAGES_LIBNAME) $(DESTDIR)$(libdir)\n");
  fprintf(out, "\t$(RANLIB) $(DESTDIR)$(libdir)/$(FLTK3_LIBBASENAME)\n");
  fprintf(out, "\tif test x$(FLTK3GL_LIBNAME) != x; then \\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK3GL_LIBNAME) $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(RANLIB) $(DESTDIR)$(libdir)/$(FLTK3GL_LIBBASENAME); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\t$(RANLIB) $(DESTDIR)$(libdir)/$(FLTK3IMAGES_LIBBASENAME)\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xlibfltk.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.so*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk.so.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk.so.1.3 $(DESTDIR)$(libdir)/libfltk.so;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xlibfltk.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.sl*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk.sl.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk.sl.1.3 $(DESTDIR)$(libdir)/libfltk.sl;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xlibfltk.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.*dylib;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk.1.3.dylib $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk.1.3.dylib $(DESTDIR)$(libdir)/libfltk.dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xlibfltk_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_s.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_s.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xcygfltknox-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3_DSONAME);\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK3_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xmgwfltknox-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3_DSONAME);\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK3_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xlibfltk_gl.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.so*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl.so.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_gl.so.1.3 $(DESTDIR)$(libdir)/libfltk_gl.so;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xlibfltk_gl.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.sl*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl.sl.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_gl.sl.1.3 $(DESTDIR)$(libdir)/libfltk_gl.sl;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xlibfltk_gl.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.*dylib;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl.1.3.dylib $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_gl.1.3.dylib $(DESTDIR)$(libdir)/libfltk_gl.dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xlibfltk_gl_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl_s.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl_s.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xcygfltknox_gl-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3GL_DSONAME);\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK3GL_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xmgwfltknox_gl-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3GL_DSONAME);\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK3GL_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xlibfltk_images.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.so*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images.so.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_images.so.1.3 $(DESTDIR)$(libdir)/libfltk_images.so;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xlibfltk_images.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.sl*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images.sl.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_images.sl.1.3 $(DESTDIR)$(libdir)/libfltk_images.sl;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xlibfltk_images.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.*dylib;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images.1.3.dylib $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_images.1.3.dylib $(DESTDIR)$(libdir)/libfltk_images.dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xlibfltk_images_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images_s.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images_s.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xcygfltknox_images-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3IMAGES_DSONAME); \\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK3IMAGES_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xmgwfltknox_images-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3IMAGES_DSONAME); \\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK3IMAGES_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall:\n");
  fprintf(out, "\techo \"Uninstalling libraries...\"\n");
  fprintf(out, "\t$(RM) $(DESTDIR)$(libdir)/$(FLTK3_LIBBASENAME)\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xlibfltk.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.so*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xlibfltk.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.sl*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xlibfltk.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.*dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xlibfltk_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_s.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xcygfltknox-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xcygfltk-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3_DSONAME) = xmgwfltknox-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_LIBNAME) != x; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/$(FLTK3GL_LIBBASENAME);\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xlibfltk_gl.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.so*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xlibfltk_gl.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.sl*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xlibfltk_gl.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.*dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xlibfltk_gl_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl_s.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xcygfltknox_gl-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3GL_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xcygfltk_gl-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3GL_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3GL_DSONAME) = xmgwfltknox_gl-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3GL_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_LIBNAME) != x; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/$(FLTK3IMAGES_LIBBASENAME);\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xlibfltk_images.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.so*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xlibfltk_images.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.sl*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xlibfltk_images.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.*dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xlibfltk_images_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images_s.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xcygfltknox_images-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3IMAGES_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xcygfltk_images-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3IMAGES_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK3IMAGES_DSONAME) = xmgwfltknox_images-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK3IMAGES_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  write_footer(out);
  fprintf(out, "# DO NOT DELETE\n");
  return 0;
}

static int write_test_makefile(FILE *out, Fl_Workspace_Type *workspace, const char *path, const char *base_dir, const char *tgt_base, Fl_Target_Type *first_target) {

  Fl_Target_Type *tgt;
  //Fl_Target_Dependency_Type * dep;
  Fl_File_Type *file;
  
  write_header(out, "Test/example program makefile for the Fast Light Tool Kit (FLTK).");
  fprintf(out, "include ../makeinclude\n");
  fprintf(out, "\n");
  
  fprintf(out, "CPPFILES =");
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0) {
      for (file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
        if (file->file_is_code()) {
          fprintf(out, " \\\n\t");
          write_file(out, file, base_dir, tgt_base);
        }
      }
    }
  }
  fprintf(out, "\n\n");
  
  fprintf(out, "ALL =");
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0 && !target_links_with(tgt, "fltk_gl")) {
      fprintf(out, " \\\n\t%s$(EXEEXT)", tgt->name());
    }
  }
  fprintf(out, "\n\n");
  
  fprintf(out, "GLALL =");
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0 && target_links_with(tgt, "fltk_gl")) {
      fprintf(out, " \\\n\t%s$(EXEEXT)", tgt->name());
    }
  }
  fprintf(out, "\n\n");
  
  fprintf(out, "all:\t$(ALL) $(GLDEMOS)\n");
  fprintf(out, "\n");
  fprintf(out, "gldemos:\t$(GLALL)\n");
  fprintf(out, "\n");
  fprintf(out, "depend:\t$(CPPFILES)\n");
  fprintf(out, "\tmakedepend -Y -I../include -f makedepend $(CPPFILES)\n");
  fprintf(out, "\n");
  fprintf(out, "# Automatically generated dependencies...\n");
  fprintf(out, "include makedepend\n");
  fprintf(out, "\n");
  fprintf(out, "clean:\n");
  fprintf(out, "\t$(RM) $(ALL) $(GLALL) core\n");
  fprintf(out, "\t$(RM) *.o core.* *~ *.bck *.bak\n");
  // list all .fl based .cxx and .h files for removal
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0) {
      for (file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
        if (file->builds_in(FL_ENV_MAKE) && file->file_is_fluid_ui()) {
          fprintf(out, "\t$(RM) ");
          write_file(out, file, base_dir, tgt_base, ".cxx");
          fprintf(out, " ");
          write_file(out, file, base_dir, tgt_base, ".h");
          fprintf(out, "\n");
        }
      }
    }
  }
  fprintf(out, "\t$(OSX_ONLY) $(RM) blocks.app/Contents/MacOS/blocks$(EXEEXT)\n");
  fprintf(out, "\t$(OSX_ONLY) $(RM) checkers.app/Contents/MacOS/checkers$(EXEEXT)\n");
  fprintf(out, "\t$(OSX_ONLY) $(RM) sudoku.app/Contents/MacOS/sudoku$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "install:\tall\n");
  fprintf(out, "\techo \"Installing example programs to $(DESTDIR)$(docdir)/examples...\"\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)$(docdir)/examples\n");
  fprintf(out, "\tfor file in *.h *.cxx *.fl demo.menu; do \\\n");
  fprintf(out, "\t\t$(INSTALL_DATA) $$file $(DESTDIR)$(docdir)/examples; \\\n");
  fprintf(out, "\tdone\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)$(docdir)/examples/pixmaps\n");
  fprintf(out, "\tfor file in pixmaps/*.xbm pixmaps/*.xpm; do \\\n");
  fprintf(out, "\t\t$(INSTALL_DATA) $$file $(DESTDIR)$(docdir)/examples/pixmaps; \\\n");
  fprintf(out, "\tdone\n");
  fprintf(out, "\n");
  fprintf(out, "install-linux:\n");
  fprintf(out, "\techo Installing games to $(DESTDIR)$(bindir)...\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)$(bindir)\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/usr/share/applications\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/usr/share/icons/hicolor/32x32/apps\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)/usr/share/icons/hicolor/128x128/apps\n");
  fprintf(out, "\tfor game in blocks checkers sudoku; do \\\n");
  fprintf(out, "\t\t$(INSTALL_BIN) $$game $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(INSTALL_DATA) desktop/$$game.desktop  $(DESTDIR)/usr/share/applications; \\\n");
  fprintf(out, "\t\t$(INSTALL_DATA) desktop/$$game-32.png $(DESTDIR)/usr/share/icons/hicolor/32x32/apps/$$game.png; \\\n");
  fprintf(out, "\t\t$(INSTALL_DATA) desktop/$$game-128.png $(DESTDIR)/usr/share/icons/hicolor/128x128/apps/$$game.png; \\\n");
  fprintf(out, "\tdone\n");
  fprintf(out, "\n");
  fprintf(out, "install-osx:\n");
  fprintf(out, "\techo Installing games in $(DESTDIR)/Applications...\n");
  fprintf(out, "\tfor game in blocks checkers sudoku; do \\\n");
  fprintf(out, "\t\tif test ! -d $(DESTDIR)/Applications/$$game.app; then \\\n");
  fprintf(out, "\t\t\t$(INSTALL_DIR) $(DESTDIR)/Applications/$$game.app; \\\n");
  fprintf(out, "\t\t\t$(INSTALL_DIR) $(DESTDIR)/Applications/$$game.app/Contents; \\\n");
  fprintf(out, "\t\t\t$(INSTALL_DIR) $(DESTDIR)/Applications/$$game.app/Contents/MacOS; \\\n");
  fprintf(out, "\t\t\t$(INSTALL_DIR) $(DESTDIR)/Applications/$$game.app/Contents/Resources; \\\n");
  fprintf(out, "\t\tfi; \\\n");
  fprintf(out, "\t\t$(INSTALL_DATA) $$game.app/Contents/Info.plist $(DESTDIR)/Applications/$$game.app/Contents; \\\n");
  fprintf(out, "\t\t$(INSTALL_DATA) $$game.app/Contents/PkgInfo $(DESTDIR)/Applications/$$game.app/Contents; \\\n");
  fprintf(out, "\t\t$(INSTALL_BIN) $$game.app/Contents/MacOS/$$game $(DESTDIR)/Applications/$$game.app/Contents/MacOS; \\\n");
  fprintf(out, "\t\t$(INSTALL_DATA) $$game.app/Contents/Resources/$$game.icns $(DESTDIR)/Applications/$$game.app/Contents/Resources; \\\n");
  fprintf(out, "\tdone\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall:\n");
  fprintf(out, "\techo \"Removing examples programs from $(DESTDIR)$(docdir)/examples...\"\n");
  fprintf(out, "\t-$(RMDIR) $(DESTDIR)$(docdir)/examples\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall-linux:\n");
  fprintf(out, "\techo Removing games from $(DESTDIR)$(bindir)...\n");
  fprintf(out, "\tfor game in blocks checkers sudoku; do \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$$game; \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)/usr/share/applications/$$game.desktop; \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)/usr/share/icons/hicolor/32x32/apps/$$game.png; \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)/usr/share/icons/hicolor/128x128/apps/$$game.png; \\\n");
  fprintf(out, "\tdone\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall-osx:\n");
  fprintf(out, "\techo Removing games from $(DESTDIR)/Applications...\n");
  fprintf(out, "\t$(RMDIR) $(DESTDIR)/Applications/blocks.app\n");
  fprintf(out, "\t$(RMDIR) $(DESTDIR)/Applications/checkers.app\n");
  fprintf(out, "\t$(RMDIR) $(DESTDIR)/Applications/sudoku.app\n");
  fprintf(out, "\n");
  fprintf(out, "# FLUID file rules\n");
  fprintf(out, ".fl.cxx .fl.h:\t../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\techo Generating $@ and header from $<...\n");
  fprintf(out, "\t../fluid/fluid$(EXEEXT) -c $<\n");
  fprintf(out, "\n");
  fprintf(out, "# All demos depend on the FLTK library...\n");
  fprintf(out, "$(ALL): $(FLTK3_LIBNAME)\n");
  fprintf(out, "\n");

  // write all targets
  
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0) {
      
      // create the dependency line
      fprintf(out, "%s$(EXEEXT):", tgt->name());
      Fl_Type *t;
      for (t=tgt->next; t && (t->level > tgt->level); t=t->next) {
        if (t->is_tool() && ((Fl_Tool_Type*)t)->builds_in(FL_ENV_MAKE)) {
          if (t->is_file()) {
            file = (Fl_File_Type*)t;
            if (file->file_is_code() || file->file_is_fluid_ui()) {
              fprintf(out, " ");
              write_file(out, file, base_dir, tgt_base, ".o");
            }
          }
          if (t->is_target_dependency()) {
            Fl_Target_Type *tgt_dep = Fl_Target_Type::find(t->name());
            if (tgt_dep->is_lib_target()) {
              fprintf(out, " $(%s_LIB_NAME)", tgt_dep->caps_name());
            } else {
              // TODO: depend on another app to be built first
            }
          }
        }
      }
      fprintf(out, "\n");
      
      // create the linker line
      fprintf(out, "\techo Linking $@...\n");
      fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@");
      for (t=tgt->next; t && (t->level > tgt->level); t=t->next) {
        if (t->is_tool() && ((Fl_Tool_Type*)t)->builds_in(FL_ENV_MAKE)) {
          if (t->is_file()) {
            file = (Fl_File_Type*)t;
            if (file->file_is_code() || file->file_is_fluid_ui()) {
              fprintf(out, " ");
              write_file(out, file, base_dir, tgt_base, ".o");
            }
          }
          if (t->is_target_dependency()) {
            Fl_Target_Type *tgt_dep = Fl_Target_Type::find(t->name());
            if (tgt_dep->is_lib_target()) {
              fprintf(out, " $(LINK_%s)", tgt_dep->caps_name());
            } // TODO: depend on another app to be built first
          }
          if (t->is_option()) {
            Fl_Option_Type *opt = (Fl_Option_Type*)t;
            if (opt->value_type()==FL_OPTION_LINKER) {
              fprintf(out, " %s", opt->value());
            }
          }
        }
      }
      fprintf(out, " $(LDLIBS)\n");
      
      // convert .fl files into .cxx files
      for (file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
        if (file->builds_in(FL_ENV_MAKE) && file->file_is_fluid_ui()) {
          // make sure that the header file is built
          write_file(out, file, base_dir, tgt_base, ".o");
          fprintf(out, ": ");
          write_file(out, file, base_dir, tgt_base, ".h");
          fprintf(out, "\n");
          // build rule to convert .fl file
          write_file(out, file, base_dir, tgt_base, ".cxx");
          fprintf(out, ": ");
          write_file(out, file, base_dir, tgt_base);
          fprintf(out, " ../fluid/fluid$(EXEEXT)\n");
        }
      }
      fprintf(out, "\n");
    }
  }
  write_footer(out);
  return 0;
}

static int write_makefile(Fl_Workspace_Type *workspace, const char *path, Fl_Target_Type *first_target) {

  char buf[2048], base_dir[2048], tgt_base[2048];
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  strcpy(tgt_base, base_dir);
  strcat(tgt_base, path); strcat(tgt_base, "/");
  strcpy(buf, tgt_base);
  strcat(buf, "Makefile");
  FILE *out = fopen(buf, "wb");

  if (strcmp(path, "test")==0) {
    write_test_makefile(out, workspace, path, base_dir, tgt_base, first_target);
  } else if (strcmp(path, "fluid")==0) {
    write_fluid_makefile(out, workspace, path, base_dir, tgt_base, first_target);
  } else if (strcmp(path, "src")==0) {
    write_fltk_makefile(out, workspace, path, base_dir, tgt_base, first_target);
  } 
  
  fclose(out);
    
  return 0;
}


int write_fltk_makefiles() {
  Fl_Target_Type *tgt;
  Fl_Workspace_Type *workspace = (Fl_Workspace_Type*)Fl_Type::first;
  
  char buf[2048], base_dir[2048];
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  
  strcpy(buf, base_dir);
  strcat(buf, "makeinclude.in");  
  FILE *out = fopen(buf, "wb");
  write_makeinclude_in(out, workspace, 0, 0, 0, 0);
  fclose(out);
  
  strcpy(buf, base_dir);
  strcat(buf, "configure.in");  
  out = fopen(buf, "wb");
  write_configure_in(out, workspace, 0, 0, 0, 0);
  fclose(out);
  
  // clear all flags that mark the targets that we wrote already
  for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    tgt->clear_flags(1);
  }  

  // walk through all targets an find the directories that need Makefiles
  for (;;) {
    const char *current_path = 0;
    char done = 1;
    for (tgt=Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
      if (tgt->builds_in(FL_ENV_MAKE) && (tgt->flags()&1)==0) { // target not yet written
        if (current_path==0) {
          current_path = tgt->makefile_path();
          write_makefile(workspace, current_path, tgt);
          done = 0;
          tgt->set_flags(1);
        } else if (strcmp(current_path, tgt->makefile_path())==0) {
          tgt->set_flags(1);
        }
      }
    }
    if (done) break;
  }

  return 0;
}



//
// End of "$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $".
//
