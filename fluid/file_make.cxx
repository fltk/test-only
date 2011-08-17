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
#include "../fltk3/filename.h"

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

static int write_caps(FILE *out, const char *name, const char *ext=0L) {
  while (*name) {
    fputc(toupper(*name++), out);
  }
  if (ext) fputs(ext, out);
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


static int write_fluid_makefile(FILE *out, Fl_Workspace_Type *workspace, const char *path, const char *base_dir, const char *tgt_base, Fl_Target_Type *first_target) {
  
  fprintf(out, "#\n");
  fprintf(out, "# \"$Id: Makefile 8966 2011-08-15 14:23:40Z matt $\"\n");
  fprintf(out, "#\n");
  fprintf(out, "# FLUID makefile for the Fast Light Tool Kit (FLTK).\n");
  fprintf(out, "#\n");
  fprintf(out, "# Copyright 1998-2011 by Bill Spitzak and others.\n");
  fprintf(out, "#\n");
  fprintf(out, "# This library is free software; you can redistribute it and/or\n");
  fprintf(out, "# modify it under the terms of the GNU Library General Public\n");
  fprintf(out, "# License as published by the Free Software Foundation; either\n");
  fprintf(out, "# version 2 of the License, or (at your option) any later version.\n");
  fprintf(out, "#\n");
  fprintf(out, "# This library is distributed in the hope that it will be useful,\n");
  fprintf(out, "# but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(out, "# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
  fprintf(out, "# Library General Public License for more details.\n");
  fprintf(out, "#\n");
  fprintf(out, "# You should have received a copy of the GNU Library General Public\n");
  fprintf(out, "# License along with this library; if not, write to the Free Software\n");
  fprintf(out, "# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307\n");
  fprintf(out, "# USA.\n");
  fprintf(out, "#\n");
  fprintf(out, "# Please report all bugs and problems on the following page:\n");
  fprintf(out, "#\n");
  fprintf(out, "#      http://www.fltk.org/str.php\n");
  fprintf(out, "#\n");
  
  Fl_Target_Type *tgt = first_target; // currently we support only a single target in this module
  
  fprintf(out, "\n");
  fprintf(out, "CPPFILES =");
  
  for (Fl_File_Type *file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->builds_in(FL_ENV_MAKE)) {
      if (file->file_is_fluid_ui() || file->file_is_code()) {
        fprintf(out, " \\\n\t");
        write_file(out, file, base_dir, tgt_base, ".cxx");
      }
    }
  }
  fprintf(out, "\n\n");
  
  fprintf(out, "################################################################\n");
  fprintf(out, "\n");
  fprintf(out, "OBJECTS = $(CPPFILES:.cxx=.o)\n");
  fprintf(out, "\n");
  fprintf(out, "include ../makeinclude\n");
  fprintf(out, "\n");
  
  fprintf(out, "all:\t$(");
  write_caps(out, tgt->name());
  fprintf(out, ") fluid$(EXEEXT)\n"); // name() is mixed case :-(
  fprintf(out, "\n");
  
  fprintf(out, "fluid$(EXEEXT):\t\t$(OBJECTS) $(FLTK_LIBNAME) \\\n");
  fprintf(out, "\t\t\t$(FLTK_IMAGES_LIBNAME)\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LINKFLTKFORMS) $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\t$(OSX_ONLY) $(INSTALL_BIN) fluid fluid.app/Contents/MacOS\n");
  fprintf(out, "\n");
  fprintf(out, "fluid-shared$(EXEEXT):\t$(OBJECTS) ../src/$(FLTK_DSONAME) ../src/$(FLDSONAME) \\\n");
  fprintf(out, "\t\t\t../src/$(FLTK_IMAGES_DSONAME)\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LINKSHARED) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "clean:\n");
  fprintf(out, "\t-$(RM) *.o core.* *~ *.bck *.bck\n");
  fprintf(out, "\t-$(RM) core fluid$(EXEEXT) fluid-shared$(EXEEXT)\n");
  fprintf(out, "\t-$(RM) fluid.app/Contents/MacOS/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "depend:\t$(CPPFILES)\n");
  fprintf(out, "\tmakedepend -Y -I.. -f makedepend $(CPPFILES)\n");
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
  fprintf(out, "#\n");
  fprintf(out, "# End of \"$Id: Makefile 8966 2011-08-15 14:23:40Z matt $\".\n");
  fprintf(out, "#\n");

  return 0;
}

static int write_fltk_makefile(FILE *out, Fl_Workspace_Type *workspace, const char *path, const char *base_dir, const char *tgt_base, Fl_Target_Type *first_target) {
  
  Fl_Target_Type *tgt;
  
  fprintf(out, "#\n");
  fprintf(out, "# \"$Id: Makefile.tmpl 8897 2011-08-02 09:27:30Z matt $\"\n");
  fprintf(out, "#\n");
  fprintf(out, "# Library makefile for the Fast Light Tool Kit (FLTK).\n");
  fprintf(out, "#\n");
  fprintf(out, "# Copyright 1998-2010 by Bill Spitzak and others.\n");
  fprintf(out, "#\n");
  fprintf(out, "# This library is free software; you can redistribute it and/or\n");
  fprintf(out, "# modify it under the terms of the GNU Library General Public\n");
  fprintf(out, "# License as published by the Free Software Foundation; either\n");
  fprintf(out, "# version 2 of the License, or (at your option) any later version.\n");
  fprintf(out, "#\n");
  fprintf(out, "# This library is distributed in the hope that it will be useful,\n");
  fprintf(out, "# but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(out, "# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
  fprintf(out, "# Library General Public License for more details.\n");
  fprintf(out, "#\n");
  fprintf(out, "# You should have received a copy of the GNU Library General Public\n");
  fprintf(out, "# License along with this library; if not, write to the Free Software\n");
  fprintf(out, "# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307\n");
  fprintf(out, "# USA.\n");
  fprintf(out, "#\n");
  fprintf(out, "# Please report all bugs and problems on the following page:\n");
  fprintf(out, "#\n");
  fprintf(out, "#      http://www.fltk.org/str.php\n");
  fprintf(out, "#\n");
  fprintf(out, "\n");
  
  // list all CPP files for all targets
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0 && target_has_cplusplus_code(tgt)) {
      write_caps(out, tgt->name());
      fprintf(out, "_CPPFILES =");
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
      write_caps(out, tgt->name());
      fprintf(out, "_OBJCPPFILES =");
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
      write_caps(out, tgt->name());
      fprintf(out, "_CFILES =");
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
      write_caps(out, tgt->name());
      fprintf(out, "_MMFILES = $(shell if test $(USEMMFILES) = Yes; then echo $(");
      write_caps(out, tgt->name());
      fprintf(out, "_OBJCPPFILES); fi)\n");
    }
  }
  fprintf(out, "\n");
  
  for (tgt = first_target; tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_MAKE) && strcmp(tgt->makefile_path(), path)==0) {
      write_caps(out, tgt->name());
      fprintf(out, "_OBJECTS =");
      if (target_has_cplusplus_code(tgt)) {
        fprintf(out, " $(");
        write_caps(out, tgt->name());
        fprintf(out, "_CPPFILES:.cxx=.o)");
      }
      if (target_has_objc_code(tgt)) {
        fprintf(out, " $(");
        write_caps(out, tgt->name());
        fprintf(out, "_MMFILES:.mm=.o)");
      }
      if (target_has_c_code(tgt)) {
        fprintf(out, " $(");
        write_caps(out, tgt->name());
        fprintf(out, "_CFILES:.c=.o)");
      }
      fprintf(out, "\n");
    }
  }
  fprintf(out, "\n");
  
  fprintf(out, "all:\t$(FLTK_LIBNAME) $(FLTK_DSONAME) \\\n");
  fprintf(out, "\t$(FLTK_GL_LIBNAME) $(FLTK_GL_DSONAME) \\\n");
  fprintf(out, "\t$(FLTK_IMAGES_LIBNAME) $(FLTK_IMAGES_DSONAME) \n");
  fprintf(out, "\n");
  fprintf(out, "$(FLTK_LIBNAME): $(FLTK_OBJECTS)\n");
  fprintf(out, "\techo $(LIBCOMMAND) $@ ...\n");
  fprintf(out, "\t$(RM) $@\n");
  fprintf(out, "\t$(LIBCOMMAND) $@ $(FLTK_OBJECTS)\n");
  fprintf(out, "\t$(RANLIB) $@\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk.so.1.3: $(FLTK_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK_OBJECTS)\n");
  fprintf(out, "\t$(RM) libfltk.so\n");
  fprintf(out, "\t$(LN) libfltk.so.1.3 libfltk.so\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk.sl.1.3: $(FLTK_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK_OBJECTS)\n");
  fprintf(out, "\t$(RM) libfltk.sl\n");
  fprintf(out, "\t$(LN) libfltk.sl.1.3 libfltk.sl\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk.1.3.dylib: $(FLTK_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ \\\n");
  fprintf(out, "\t\t-install_name $(libdir)/$@ \\\n");
  fprintf(out, "\t\t-current_version 1.3.0 \\\n");
  fprintf(out, "\t\t-compatibility_version 1.3.0 \\\n");
  fprintf(out, "\t\t$(FLTK_OBJECTS) $(LDLIBS)\n");
  fprintf(out, "\t$(RM) libfltk.dylib\n");
  fprintf(out, "\t$(LN) libfltk.1.3.dylib libfltk.dylib\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_s.a: $(FLTK_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) libfltk_s.o ...\n");
  fprintf(out, "\t$(DSOCOMMAND) libfltk_s.o $(FLTK_OBJECTS) $(IMAGELIBS)\n");
  fprintf(out, "\techo $(LIBCOMMAND) libfltk_s.a libfltk_s.o\n");
  fprintf(out, "\t$(RM) $@\n");
  fprintf(out, "\t$(LIBCOMMAND) libfltk_s.a libfltk_s.o\n");
  fprintf(out, "\t$(CHMOD) +x libfltk_s.a\n");
  fprintf(out, "\n");
  fprintf(out, "$(FLTK_GL_LIBNAME): $(FLTK_GL_OBJECTS)\n");
  fprintf(out, "\techo $(LIBCOMMAND) $@ ...\n");
  fprintf(out, "\t$(RM) $@\n");
  fprintf(out, "\t$(LIBCOMMAND) $@ $(FLTK_GL_OBJECTS)\n");
  fprintf(out, "\t$(RANLIB) $@\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_gl.so.1.3: $(FLTK_GL_OBJECTS) libfltk.so.1.3\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK_GL_OBJECTS) -L. -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_gl.so\n");
  fprintf(out, "\t$(LN) libfltk_gl.so.1.3 libfltk_gl.so\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_gl.sl.1.3: $(FLTK_GL_OBJECTS) libfltk.sl.1.3\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK_GL_OBJECTS) -L. -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_gl.sl\n");
  fprintf(out, "\t$(LN) libfltk_gl.sl.1.3 libfltk_gl.sl\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_gl.1.3.dylib: $(FLTK_GL_OBJECTS) libfltk.1.3.dylib\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ \\\n");
  fprintf(out, "\t\t-install_name $(libdir)/$@ \\\n");
  fprintf(out, "\t\t-current_version 1.3.0 \\\n");
  fprintf(out, "\t\t-compatibility_version 1.3.0 \\\n");
  fprintf(out, "\t\t$(FLTK_GL_OBJECTS) -L. $(GLDLIBS) -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_gl.dylib\n");
  fprintf(out, "\t$(LN) libfltk_gl.1.3.dylib libfltk_gl.dylib\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_gl_s.a: $(FLTK_GL_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) libfltk_gl_s.o ...\n");
  fprintf(out, "\t$(DSOCOMMAND) libfltk_gl_s.o $(FLTK_GL_OBJECTS)\n");
  fprintf(out, "\techo $(LIBCOMMAND) libfltk_gl_s.a libfltk_gl_s.o\n");
  fprintf(out, "\t$(RM) $@\n");
  fprintf(out, "\t$(LIBCOMMAND) libfltk_gl_s.a libfltk_gl_s.o\n");
  fprintf(out, "\t$(CHMOD) +x libfltk_gl_s.a\n");
  fprintf(out, "\n");
  fprintf(out, "$(FLTK_IMAGES_LIBNAME): $(FLTK_IMAGES_OBJECTS)\n");
  fprintf(out, "\techo $(LIBCOMMAND) $@ ...\n");
  fprintf(out, "\t$(RM) $@\n");
  fprintf(out, "\t$(LIBCOMMAND) $@ $(FLTK_IMAGES_OBJECTS)\n");
  fprintf(out, "\t$(RANLIB) $@\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_images.so.1.3: $(FLTK_IMAGES_OBJECTS) libfltk.so.1.3\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK_IMAGES_OBJECTS) -L. $(IMAGELIBS) -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_images.so\n");
  fprintf(out, "\t$(LN) libfltk_images.so.1.3 libfltk_images.so\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_images.sl.1.3: $(FLTK_IMAGES_OBJECTS) libfltk.sl.1.3\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ $(FLTK_IMAGES_OBJECTS) -L. $(IMAGELIBS) -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_images.sl\n");
  fprintf(out, "\t$(LN) libfltk_images.sl.1.3 libfltk_images.sl\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_images.1.3.dylib: $(FLTK_IMAGES_OBJECTS) libfltk.1.3.dylib\n");
  fprintf(out, "\techo $(DSOCOMMAND) $@ ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $@ \\\n");
  fprintf(out, "\t\t-install_name $(libdir)/$@ \\\n");
  fprintf(out, "\t\t-current_version 1.3.0 \\\n");
  fprintf(out, "\t\t-compatibility_version 1.3.0 \\\n");
  fprintf(out, "\t\t$(FLTK_IMAGES_OBJECTS)  -L. $(LDLIBS) $(IMAGELIBS) -lfltk\n");
  fprintf(out, "\t$(RM) libfltk_images.dylib\n");
  fprintf(out, "\t$(LN) libfltk_images.1.3.dylib libfltk_images.dylib\n");
  fprintf(out, "\n");
  fprintf(out, "libfltk_images_s.a: $(FLTK_IMAGES_OBJECTS)\n");
  fprintf(out, "\techo $(DSOCOMMAND) libfltk_images_s.o ...\n");
  fprintf(out, "\t$(DSOCOMMAND) libfltk_images_s.o $(FLTK_IMAGES_OBJECTS)\n");
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
  fprintf(out, "cygfltknox-1.3.dll: $(FLTK_LIBNAME)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk.dll.a $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltknox_gl-1.3.dll: $(FLTK_GL_LIBNAME) cygfltknox-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK_GL_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK_GL_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_gl.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltknox_images-1.3.dll: $(FLTK_IMAGES_LIBNAME) cygfltknox-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_images.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk -Wl,--exclude-libs -Wl,libfltk_png.a \\\n");
  fprintf(out, "\t\t\t$(IMAGELIBS) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "#-----------------------------------------------------------------\n");
  fprintf(out, "# cygwin X11 shared libraries\n");
  fprintf(out, "#-----------------------------------------------------------------\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltk-1.3.dll: $(FLTK_LIBNAME)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk.dll.a $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltk_gl-1.3.dll: $(FLTK_GL_LIBNAME) cygfltk-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK_GL_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK_GL_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_gl.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "cygfltk_images-1.3.dll: $(FLTK_IMAGES_LIBNAME) cygfltk-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_images.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk -Wl,--exclude-libs -Wl,libfltk_png.a \\\n");
  fprintf(out, "\t\t\t$(IMAGELIBS) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "mgwfltknox-1.3.dll: $(FLTK_LIBNAME)\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk.dll.a $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "mgwfltknox_gl-1.3.dll: $(FLTK_GL_LIBNAME) mgwfltknox-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK_GL_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK_GL_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_gl.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "#-----------------------------------------------------\n");
  fprintf(out, "# See STR #1585 for --exclude-libs\n");
  fprintf(out, "#-----------------------------------------------------\n");
  fprintf(out, "\n");
  fprintf(out, "mgwfltknox_images-1.3.dll: $(FLTK_IMAGES_LIBNAME) mgwfltknox-1.3.dll\n");
  fprintf(out, "\techo $(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) ...\n");
  fprintf(out, "\t$(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) -Wl,--no-whole-archive \\\n");
  fprintf(out, "\t\t-Wl,--out-implib=libfltk_images.dll.a \\\n");
  fprintf(out, "\t\t-L. -lfltk -Wl,--exclude-libs -Wl,libfltk_png.a \\\n");
  fprintf(out, "\t\t\t$(IMAGELIBS) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "clean:\n");
  fprintf(out, "\t-$(RM) *.o xutf8/*.o *.dll.a core.* *~ *.bak *.bck\n");
  fprintf(out, "\t-$(RM) $(FLTK_DSONAME) $(FLDSONAME) $(FLTK_GL_DSONAME) $(FLTK_IMAGES_DSONAME) \\\n");
  fprintf(out, "\t\t$(FLTK_LIBNAME) $(FLTK_GL_LIBNAME) \\\n");
  fprintf(out, "\t\t$(FLTK_IMAGES_LIBNAME) \\\n");
  fprintf(out, "\t\tlibfltk.so libfltk_gl.so libfltk_images.so \\\n");
  fprintf(out, "\t\tlibfltk.sl libfltk_gl.sl libfltk_images.sl \\\n");
  fprintf(out, "\t\tlibfltk.dylib \\\n");
  fprintf(out, "\t\tlibfltk_gl.dylib libfltk_images.dylib \\\n");
  fprintf(out, "\t\tcmap core\n");
  fprintf(out, "\n");
  fprintf(out, "depend:\t$(FLTK_CPPFILES) $(FLTK_MMFILES) $(FLTK_GL_CPPFILES) $(FLTK_IMAGES_CPPFILES) $(FLTK_CFILES)\n");
  fprintf(out, "\tmakedepend -Y -I.. -f makedepend $(FLTK_CPPFILES) $(FLTK_MMFILES) \\\n");
  fprintf(out, "\t\t$(FLTK_GL_CPPFILES) $(FLTK_IMAGES_CPPFILES) $(FLTK_CFILES)\n");
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
  fprintf(out, "install: $(FLTK_LIBNAME) $(FLTK_DSONAME) \\\n");
  fprintf(out, "\t$(FLTK_GL_LIBNAME) $(FLTK_GL_DSONAME) \\\n");
  fprintf(out, "\t$(FLTK_IMAGES_LIBNAME) $(FLTK_IMAGES_DSONAME) \n");
  fprintf(out, "\techo \"Installing libraries in $(DESTDIR)$(libdir)...\"\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)$(libdir)\n");
  fprintf(out, "\t-$(INSTALL_DIR) $(DESTDIR)$(bindir)\n");
  fprintf(out, "\t$(RM) $(DESTDIR)$(libdir)/$(LIBBASENAME)\n");
  fprintf(out, "\t$(INSTALL_LIB) $(FLTK_LIBNAME) $(DESTDIR)$(libdir)\n");
  fprintf(out, "\t$(INSTALL_LIB) $(FLTK_IMAGES_LIBNAME) $(DESTDIR)$(libdir)\n");
  fprintf(out, "\t$(RANLIB) $(DESTDIR)$(libdir)/$(LIBBASENAME)\n");
  fprintf(out, "\t$(RANLIB) $(DESTDIR)$(libdir)/$(FLLIBBASENAME)\n");
  fprintf(out, "\tif test x$(FLTK_GL_LIBNAME) != x; then \\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK_GL_LIBNAME) $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(RANLIB) $(DESTDIR)$(libdir)/$(GLLIBBASENAME); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\t$(RANLIB) $(DESTDIR)$(libdir)/$(IMGLIBBASENAME)\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xlibfltk.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.so*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk.so.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk.so.1.3 $(DESTDIR)$(libdir)/libfltk.so;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xlibfltk.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.sl*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk.sl.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk.sl.1.3 $(DESTDIR)$(libdir)/libfltk.sl;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xlibfltk.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.*dylib;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk.1.3.dylib $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk.1.3.dylib $(DESTDIR)$(libdir)/libfltk.dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xlibfltk_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_s.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_s.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xcygfltknox-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_DSONAME);\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xmgwfltknox-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_DSONAME);\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xlibfltk_gl.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.so*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl.so.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_gl.so.1.3 $(DESTDIR)$(libdir)/libfltk_gl.so;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xlibfltk_gl.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.sl*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl.sl.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_gl.sl.1.3 $(DESTDIR)$(libdir)/libfltk_gl.sl;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xlibfltk_gl.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.*dylib;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl.1.3.dylib $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_gl.1.3.dylib $(DESTDIR)$(libdir)/libfltk_gl.dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xlibfltk_gl_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl_s.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl_s.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xcygfltknox_gl-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_GL_DSONAME);\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK_GL_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xmgwfltknox_gl-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_GL_DSONAME);\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK_GL_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_gl.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.so*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images.so.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_images.so.1.3 $(DESTDIR)$(libdir)/libfltk_images.so;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.sl*;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images.sl.1.3 $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_images.sl.1.3 $(DESTDIR)$(libdir)/libfltk_images.sl;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.*dylib;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images.1.3.dylib $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\t\t$(LN) libfltk_images.1.3.dylib $(DESTDIR)$(libdir)/libfltk_images.dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images_s.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images_s.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xcygfltknox_images-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_IMAGES_DSONAME); \\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK_IMAGES_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xmgwfltknox_images-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_IMAGES_DSONAME); \\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) $(FLTK_IMAGES_DSONAME) $(DESTDIR)$(bindir); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\\\n");
  fprintf(out, "\t\t$(INSTALL_LIB) libfltk_images.dll.a $(DESTDIR)$(libdir); \\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\n");
  fprintf(out, "\n");
  fprintf(out, "uninstall:\n");
  fprintf(out, "\techo \"Uninstalling libraries...\"\n");
  fprintf(out, "\t$(RM) $(DESTDIR)$(libdir)/$(LIBBASENAME)\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xlibfltk.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.so*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xlibfltk.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.sl*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xlibfltk.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.*dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xlibfltk_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_s.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xcygfltknox-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xcygfltk-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_DSONAME) = xmgwfltknox-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\t$(RM) $(DESTDIR)$(libdir)/$(FLLIBBASENAME);\n");
  fprintf(out, "\tif test x$(FLTK_GL_LIBNAME) != x; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/$(GLLIBBASENAME);\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xlibfltk_gl.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.so*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xlibfltk_gl.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.sl*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xlibfltk_gl.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.*dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xlibfltk_gl_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl_s.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xcygfltknox_gl-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_GL_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xcygfltk_gl-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_GL_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_GL_DSONAME) = xmgwfltknox_gl-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_GL_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_LIBNAME) != x; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/$(IMGLIBBASENAME);\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.so.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.so*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.sl.1.3; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.sl*;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.1.3.dylib; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.*dylib;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images_s.a; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images_s.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xcygfltknox_images-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_IMAGES_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xcygfltk_images-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_IMAGES_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\tif test x$(FLTK_IMAGES_DSONAME) = xmgwfltknox_images-1.3.dll; then\\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(bindir)/$(FLTK_IMAGES_DSONAME); \\\n");
  fprintf(out, "\t\t$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\\\n");
  fprintf(out, "\tfi\n");
  fprintf(out, "\n");
  fprintf(out, "#\n");
  fprintf(out, "# End of \"$Id: Makefile.tmpl 8897 2011-08-02 09:27:30Z matt $\".\n");
  fprintf(out, "#\n");
  fprintf(out, "# DO NOT DELETE\n");
  
  return 0;
}


static int write_test_makefile(FILE *out, Fl_Workspace_Type *workspace, const char *path, const char *base_dir, const char *tgt_base, Fl_Target_Type *first_target) {

  fprintf(out, "#\n");
  fprintf(out, "# \"$Id: Makefile 8740 2011-05-26 14:43:46Z matt $\"\n");
  fprintf(out, "#\n");
  fprintf(out, "# Test/example program makefile for the Fast Light Tool Kit (FLTK).\n");
  fprintf(out, "#\n");
  fprintf(out, "# Copyright 1998-2010 by Bill Spitzak and others.\n");
  fprintf(out, "#\n");
  fprintf(out, "# This library is free software; you can redistribute it and/or\n");
  fprintf(out, "# modify it under the terms of the GNU Library General Public\n");
  fprintf(out, "# License as published by the Free Software Foundation; either\n");
  fprintf(out, "# version 2 of the License, or (at your option) any later version.\n");
  fprintf(out, "#\n");
  fprintf(out, "# This library is distributed in the hope that it will be useful,\n");
  fprintf(out, "# but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(out, "# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
  fprintf(out, "# Library General Public License for more details.\n");
  fprintf(out, "#\n");
  fprintf(out, "# You should have received a copy of the GNU Library General Public\n");
  fprintf(out, "# License along with this library; if not, write to the Free Software\n");
  fprintf(out, "# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307\n");
  fprintf(out, "# USA.\n");
  fprintf(out, "#\n");
  fprintf(out, "# Please report all bugs and problems on the following page:\n");
  fprintf(out, "#\n");
  fprintf(out, "#      http://www.fltk.org/str.php\n");
  fprintf(out, "#\n");
  fprintf(out, "\n");
  fprintf(out, "include ../makeinclude\n");
  fprintf(out, "\n");
  fprintf(out, "CPPFILES =\\\n");
  fprintf(out, "\tunittests.cxx \\\n");
  fprintf(out, "\tadjuster.cxx \\\n");
  fprintf(out, "\tarc.cxx \\\n");
  fprintf(out, "\task.cxx \\\n");
  fprintf(out, "\tbitmap.cxx \\\n");
  fprintf(out, "\tblocks.cxx \\\n");
  fprintf(out, "\tboxtype.cxx \\\n");
  fprintf(out, "\tbrowser.cxx \\\n");
  fprintf(out, "\tbutton.cxx \\\n");
  fprintf(out, "\tbuttons.cxx \\\n");
  fprintf(out, "\tcairo_test.cxx \\\n");
  fprintf(out, "\tcheckers.cxx \\\n");
  fprintf(out, "\tclock.cxx \\\n");
  fprintf(out, "\tcolbrowser.cxx \\\n");
  fprintf(out, "\tcolor_chooser.cxx \\\n");
  fprintf(out, "\tcube.cxx \\\n");
  fprintf(out, "\tCubeMain.cxx \\\n");
  fprintf(out, "\tCubeView.cxx \\\n");
  fprintf(out, "\tcursor.cxx \\\n");
  fprintf(out, "\tcurve.cxx \\\n");
  fprintf(out, "\tdemo.cxx \\\n");
  fprintf(out, "\tdevice.cxx \\\n");
  fprintf(out, "\tdoublebuffer.cxx \\\n");
  fprintf(out, "\teditor.cxx \\\n");
  fprintf(out, "\tfast_slow.cxx \\\n");
  fprintf(out, "\tfile_chooser.cxx \\\n");
  fprintf(out, "\tfonts.cxx \\\n");
  fprintf(out, "\tfractals.cxx \\\n");
  fprintf(out, "\tfullscreen.cxx \\\n");
  fprintf(out, "\tgl_overlay.cxx \\\n");
  fprintf(out, "\tglpuzzle.cxx \\\n");
  fprintf(out, "\thello.cxx \\\n");
  fprintf(out, "\thelp.cxx \\\n");
  fprintf(out, "\ticonize.cxx \\\n");
  fprintf(out, "\timage.cxx \\\n");
  fprintf(out, "\tinactive.cxx \\\n");
  fprintf(out, "\tinput.cxx \\\n");
  fprintf(out, "\tinput_choice.cxx \\\n");
  fprintf(out, "\tkeyboard.cxx \\\n");
  fprintf(out, "\tlabel.cxx \\\n");
  fprintf(out, "\tline_style.cxx \\\n");
  fprintf(out, "\tlist_visuals.cxx \\\n");
  fprintf(out, "\tmandelbrot.cxx \\\n");
  fprintf(out, "\tmenubar.cxx \\\n");
  fprintf(out, "\tmessage.cxx \\\n");
  fprintf(out, "\tminimum.cxx \\\n");
  fprintf(out, "\tnative-filechooser.cxx \\\n");
  fprintf(out, "\tnavigation.cxx \\\n");
  fprintf(out, "\toutput.cxx \\\n");
  fprintf(out, "\toverlay.cxx \\\n");
  fprintf(out, "\tpack.cxx \\\n");
  fprintf(out, "\tpixmap_browser.cxx \\\n");
  fprintf(out, "\tpixmap.cxx \\\n");
  fprintf(out, "\tpreferences.cxx \\\n");
  fprintf(out, "\tdevice.cxx \\\n");
  fprintf(out, "\tradio.cxx \\\n");
  fprintf(out, "\tresizebox.cxx \\\n");
  fprintf(out, "\tresize.cxx \\\n");
  fprintf(out, "\trotated_text.cxx \\\n");
  fprintf(out, "\tscroll.cxx \\\n");
  fprintf(out, "\tshape.cxx \\\n");
  fprintf(out, "\tsubwindow.cxx \\\n");
  fprintf(out, "\tsudoku.cxx \\\n");
  fprintf(out, "\tsymbols.cxx \\\n");
  fprintf(out, "\ttable.cxx \\\n");
  fprintf(out, "\ttabs.cxx \\\n");
  fprintf(out, "\tthreads.cxx \\\n");
  fprintf(out, "\ttile.cxx \\\n");
  fprintf(out, "\ttiled_image.cxx \\\n");
  fprintf(out, "\ttree.cxx \\\n");
  fprintf(out, "\tvaluators.cxx \\\n");
  fprintf(out, "\tutf8.cxx\n");
  fprintf(out, "\n");
  fprintf(out, "ALL =\t\\\n");
  fprintf(out, "\tunittests$(EXEEXT) \\\n");
  fprintf(out, "\tadjuster$(EXEEXT) \\\n");
  fprintf(out, "\tarc$(EXEEXT) \\\n");
  fprintf(out, "\task$(EXEEXT) \\\n");
  fprintf(out, "\tbitmap$(EXEEXT) \\\n");
  fprintf(out, "\tblocks$(EXEEXT) \\\n");
  fprintf(out, "\tboxtype$(EXEEXT) \\\n");
  fprintf(out, "\tbrowser$(EXEEXT) \\\n");
  fprintf(out, "\tbutton$(EXEEXT) \\\n");
  fprintf(out, "\tbuttons$(EXEEXT) \\\n");
  fprintf(out, "\tcairo_test$(EXEEXT) \\\n");
  fprintf(out, "\tcheckers$(EXEEXT) \\\n");
  fprintf(out, "\tclock$(EXEEXT) \\\n");
  fprintf(out, "\tcolbrowser$(EXEEXT) \\\n");
  fprintf(out, "\tcolor_chooser$(EXEEXT) \\\n");
  fprintf(out, "\tcursor$(EXEEXT) \\\n");
  fprintf(out, "\tcurve$(EXEEXT) \\\n");
  fprintf(out, "\tdemo$(EXEEXT) \\\n");
  fprintf(out, "\tdevice$(EXEEXT) \\\n");
  fprintf(out, "\tdoublebuffer$(EXEEXT) \\\n");
  fprintf(out, "\teditor$(EXEEXT) \\\n");
  fprintf(out, "\tfast_slow$(EXEEXT) \\\n");
  fprintf(out, "\tfile_chooser$(EXEEXT) \\\n");
  fprintf(out, "\tfonts$(EXEEXT) \\\n");
  fprintf(out, "\thello$(EXEEXT) \\\n");
  fprintf(out, "\thelp$(EXEEXT) \\\n");
  fprintf(out, "\ticonize$(EXEEXT) \\\n");
  fprintf(out, "\timage$(EXEEXT) \\\n");
  fprintf(out, "\tinactive$(EXEEXT) \\\n");
  fprintf(out, "\tinput$(EXEEXT) \\\n");
  fprintf(out, "\tinput_choice$(EXEEXT) \\\n");
  fprintf(out, "\tkeyboard$(EXEEXT) \\\n");
  fprintf(out, "\tlabel$(EXEEXT) \\\n");
  fprintf(out, "\tline_style$(EXEEXT) \\\n");
  fprintf(out, "\tlist_visuals$(EXEEXT) \\\n");
  fprintf(out, "\tmandelbrot$(EXEEXT) \\\n");
  fprintf(out, "\tmenubar$(EXEEXT) \\\n");
  fprintf(out, "\tmessage$(EXEEXT) \\\n");
  fprintf(out, "\tminimum$(EXEEXT) \\\n");
  fprintf(out, "\tnative-filechooser$(EXEEXT) \\\n");
  fprintf(out, "\tnavigation$(EXEEXT) \\\n");
  fprintf(out, "\toutput$(EXEEXT) \\\n");
  fprintf(out, "\toverlay$(EXEEXT) \\\n");
  fprintf(out, "\tpack$(EXEEXT) \\\n");
  fprintf(out, "\tpixmap$(EXEEXT) \\\n");
  fprintf(out, "\tpixmap_browser$(EXEEXT) \\\n");
  fprintf(out, "\tpreferences$(EXEEXT) \\\n");
  fprintf(out, "\tdevice$(EXEEXT) \\\n");
  fprintf(out, "\tradio$(EXEEXT) \\\n");
  fprintf(out, "\tresize$(EXEEXT) \\\n");
  fprintf(out, "\tresizebox$(EXEEXT) \\\n");
  fprintf(out, "\trotated_text$(EXEEXT) \\\n");
  fprintf(out, "\tscroll$(EXEEXT) \\\n");
  fprintf(out, "\tsubwindow$(EXEEXT) \\\n");
  fprintf(out, "\tsudoku$(EXEEXT) \\\n");
  fprintf(out, "\tsymbols$(EXEEXT) \\\n");
  fprintf(out, "\ttable$(EXEEXT) \\\n");
  fprintf(out, "\ttabs$(EXEEXT) \\\n");
  fprintf(out, "\t$(THREADS) \\\n");
  fprintf(out, "\ttile$(EXEEXT) \\\n");
  fprintf(out, "\ttiled_image$(EXEEXT) \\\n");
  fprintf(out, "\ttree$(EXEEXT) \\\n");
  fprintf(out, "\tvaluators$(EXEEXT) \\\n");
  fprintf(out, "\tcairotest$(EXEEXT) \\\n");
  fprintf(out, "\tutf8$(EXEEXT) \n");
  fprintf(out, "\n");
  fprintf(out, "\n");
  fprintf(out, "GLALL = \\\n");
  fprintf(out, "\tcube$(EXEEXT) \\\n");
  fprintf(out, "\tCubeView$(EXEEXT) \\\n");
  fprintf(out, "\tfractals$(EXEEXT) \\\n");
  fprintf(out, "\tfullscreen$(EXEEXT) \\\n");
  fprintf(out, "\tgl_overlay$(EXEEXT) \\\n");
  fprintf(out, "\tglpuzzle$(EXEEXT) \\\n");
  fprintf(out, "\tshape$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "all:\t$(ALL) $(GLDEMOS)\n");
  fprintf(out, "\n");
  fprintf(out, "gldemos:\t$(GLALL)\n");
  fprintf(out, "\n");
  fprintf(out, "depend:\t$(CPPFILES)\n");
  fprintf(out, "\tmakedepend -Y -I.. -f makedepend $(CPPFILES)\n");
  fprintf(out, "\n");
  fprintf(out, "# Automatically generated dependencies...\n");
  fprintf(out, "include makedepend\n");
  fprintf(out, "\n");
  fprintf(out, "clean:\n");
  fprintf(out, "\t$(RM) $(ALL) $(GLALL) core\n");
  fprintf(out, "\t$(RM) *.o core.* *~ *.bck *.bak\n");
  fprintf(out, "\t$(RM) CubeViewUI.cxx CubeViewUI.h\n");
  fprintf(out, "\t$(RM) fast_slow.cxx fast_slow.h\n");
  fprintf(out, "\t$(RM) inactive.cxx inactive.h\n");
  fprintf(out, "\t$(RM) keyboard_ui.cxx keyboard_ui.h\n");
  fprintf(out, "\t$(RM) mandelbrot_ui.cxx mandelbrot_ui.h\n");
  fprintf(out, "\t$(RM) preferences.cxx preferences.h\n");
  fprintf(out, "\t$(RM) radio.cxx radio.h\n");
  fprintf(out, "\t$(RM) resize.cxx resize.h\n");
  fprintf(out, "\t$(RM) tabs.cxx tabs.h\n");
  fprintf(out, "\t$(RM) tree.cxx tree.h\n");
  fprintf(out, "\t$(RM) valuators.cxx valuators.h\n");
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
  fprintf(out, "$(ALL): $(FLTK_LIBNAME)\n");
  fprintf(out, "\n");
  fprintf(out, "# General demos...\n");
  fprintf(out, "unittests$(EXEEXT): unittests.o\n");
  fprintf(out, "\n");
  fprintf(out, "unittests.cxx: unittest_about.cxx unittest_points.cxx unittest_lines.cxx unittest_circles.cxx \\\n");
  fprintf(out, "\tunittest_rects.cxx unittest_text.cxx unittest_viewport.cxx unittest_images.cxx\n");
  fprintf(out, "\n");
  fprintf(out, "adjuster$(EXEEXT): adjuster.o\n");
  fprintf(out, "\n");
  fprintf(out, "arc$(EXEEXT): arc.o\n");
  fprintf(out, "\n");
  fprintf(out, "ask$(EXEEXT): ask.o\n");
  fprintf(out, "\n");
  fprintf(out, "bitmap$(EXEEXT): bitmap.o\n");
  fprintf(out, "\n");
  fprintf(out, "boxtype$(EXEEXT): boxtype.o\n");
  fprintf(out, "\n");
  fprintf(out, "browser$(EXEEXT): browser.o\n");
  fprintf(out, "\n");
  fprintf(out, "button$(EXEEXT): button.o\n");
  fprintf(out, "\n");
  fprintf(out, "buttons$(EXEEXT): buttons.o\n");
  fprintf(out, "\n");
  fprintf(out, "blocks$(EXEEXT): blocks.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) blocks.o -o $@ $(AUDIOLIBS) $(LINKFLTK) $(LDLIBS)\n");
  fprintf(out, "\t$(OSX_ONLY) $(INSTALL_BIN) blocks$(EXEEXT) blocks.app/Contents/MacOS\n");
  fprintf(out, "\n");
  fprintf(out, "checkers$(EXEEXT): checkers.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) checkers.o -o $@ $(LINKFLTK) $(LDLIBS)\n");
  fprintf(out, "\t$(OSX_ONLY) $(INSTALL_BIN) checkers$(EXEEXT) checkers.app/Contents/MacOS\n");
  fprintf(out, "\n");
  fprintf(out, "clock$(EXEEXT): clock.o\n");
  fprintf(out, "\n");
  fprintf(out, "colbrowser$(EXEEXT): colbrowser.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ colbrowser.o $(LINKFLTK) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "color_chooser$(EXEEXT): color_chooser.o\n");
  fprintf(out, "\n");
  fprintf(out, "cursor$(EXEEXT): cursor.o\n");
  fprintf(out, "\n");
  fprintf(out, "curve$(EXEEXT): curve.o\n");
  fprintf(out, "\n");
  fprintf(out, "demo$(EXEEXT): demo.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ demo.o $(LINKFLTK) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "device$(EXEEXT): device.o $(FLTK_IMAGES_LIBNAME)\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) device.o -o $@ $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "doublebuffer$(EXEEXT): doublebuffer.o\n");
  fprintf(out, "\n");
  fprintf(out, "editor$(EXEEXT): editor.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) editor.o -o $@ $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "fast_slow$(EXEEXT): fast_slow.o\n");
  fprintf(out, "fast_slow.cxx:\tfast_slow.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "file_chooser$(EXEEXT): file_chooser.o $(FLTK_IMAGES_LIBNAME)\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) file_chooser.o -o $@ $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "fonts$(EXEEXT): fonts.o\n");
  fprintf(out, "\n");
  fprintf(out, "hello$(EXEEXT): hello.o\n");
  fprintf(out, "\n");
  fprintf(out, "help$(EXEEXT): help.o $(FLTK_IMAGES_LIBNAME)\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) help.o -o $@ $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "iconize$(EXEEXT): iconize.o\n");
  fprintf(out, "\n");
  fprintf(out, "image$(EXEEXT): image.o\n");
  fprintf(out, "\n");
  fprintf(out, "inactive$(EXEEXT): inactive.o\n");
  fprintf(out, "inactive.cxx:\tinactive.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "input$(EXEEXT): input.o\n");
  fprintf(out, "\n");
  fprintf(out, "input_choice$(EXEEXT): input_choice.o\n");
  fprintf(out, "\n");
  fprintf(out, "keyboard$(EXEEXT): keyboard_ui.o keyboard.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ keyboard.o keyboard_ui.o $(LINKFLTK) $(LDLIBS)\n");
  fprintf(out, "keyboard_ui.o:\tkeyboard_ui.h\n");
  fprintf(out, "keyboard_ui.cxx:\tkeyboard_ui.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "label$(EXEEXT): label.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ label.o $(LINKFLTK) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "line_style$(EXEEXT): line_style.o\n");
  fprintf(out, "\n");
  fprintf(out, "list_visuals$(EXEEXT): list_visuals.o\n");
  fprintf(out, "\n");
  fprintf(out, "mandelbrot$(EXEEXT): mandelbrot_ui.o mandelbrot.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ mandelbrot.o mandelbrot_ui.o $(LINKFLTK) $(LDLIBS)\n");
  fprintf(out, "mandelbrot_ui.o:\tmandelbrot_ui.h\n");
  fprintf(out, "mandelbrot_ui.cxx:\tmandelbrot_ui.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "menubar$(EXEEXT): menubar.o\n");
  fprintf(out, "\n");
  fprintf(out, "message$(EXEEXT): message.o\n");
  fprintf(out, "\n");
  fprintf(out, "minimum$(EXEEXT): minimum.o\n");
  fprintf(out, "\n");
  fprintf(out, "native-filechooser$(EXEEXT): native-filechooser.o $(FLTK_IMAGES_LIBNAME)\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) native-filechooser.o -o $@ $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "navigation$(EXEEXT): navigation.o\n");
  fprintf(out, "\n");
  fprintf(out, "output$(EXEEXT): output.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ output.o $(LINKFLTK) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "overlay$(EXEEXT): overlay.o\n");
  fprintf(out, "\n");
  fprintf(out, "pack$(EXEEXT): pack.o\n");
  fprintf(out, "\n");
  fprintf(out, "pixmap$(EXEEXT): pixmap.o\n");
  fprintf(out, "\n");
  fprintf(out, "pixmap_browser$(EXEEXT): pixmap_browser.o $(FLTK_IMAGES_LIBNAME)\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) pixmap_browser.o -o $@ $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "preferences$(EXEEXT):\tpreferences.o\n");
  fprintf(out, "preferences.cxx:\tpreferences.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "device$(EXEEXT): device.o\n");
  fprintf(out, "\n");
  fprintf(out, "radio$(EXEEXT): radio.o\n");
  fprintf(out, "radio.cxx:\tradio.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "resize$(EXEEXT): resize.o\n");
  fprintf(out, "resize.cxx:\tresize.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "resizebox$(EXEEXT): resizebox.o\n");
  fprintf(out, "\n");
  fprintf(out, "rotated_text$(EXEEXT): rotated_text.o\n");
  fprintf(out, "\n");
  fprintf(out, "scroll$(EXEEXT): scroll.o\n");
  fprintf(out, "\n");
  fprintf(out, "subwindow$(EXEEXT): subwindow.o\n");
  fprintf(out, "\n");
  fprintf(out, "sudoku: sudoku.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) sudoku.o -o $@ $(AUDIOLIBS) $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\t$(OSX_ONLY) $(INSTALL_BIN) sudoku$(EXEEXT) sudoku.app/Contents/MacOS\n");
  fprintf(out, "\n");
  fprintf(out, "sudoku.exe: sudoku.o sudoku.rc\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(RC) sudoku.rc sudokures.o\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) sudoku.o sudokures.o -o $@ $(AUDIOLIBS) $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "symbols$(EXEEXT): symbols.o\n");
  fprintf(out, "\n");
  fprintf(out, "table$(EXEEXT): table.o\n");
  fprintf(out, "\n");
  fprintf(out, "tabs$(EXEEXT): tabs.o\n");
  fprintf(out, "tabs.cxx:\ttabs.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "threads$(EXEEXT): threads.o\n");
  fprintf(out, "# This ensures that we have this dependency even if threads are not\n");
  fprintf(out, "# enabled in the current tree...\n");
  fprintf(out, "threads.o:\tthreads.h\n");
  fprintf(out, "\n");
  fprintf(out, "tile$(EXEEXT): tile.o\n");
  fprintf(out, "\n");
  fprintf(out, "tiled_image$(EXEEXT): tiled_image.o\n");
  fprintf(out, "\n");
  fprintf(out, "tree$(EXEEXT): tree.o\n");
  fprintf(out, "tree.cxx:\ttree.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "valuators$(EXEEXT): valuators.o\n");
  fprintf(out, "valuators.cxx:\tvaluators.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "# All OpenGL demos depend on the FLTK and FLTK_GL libraries...\n");
  fprintf(out, "$(GLALL): $(FLTK_LIBNAME) $(FLTK_GL_LIBNAME)\n");
  fprintf(out, "\n");
  fprintf(out, "# OpenGL demos...\n");
  fprintf(out, "CubeView$(EXEEXT): CubeMain.o CubeView.o CubeViewUI.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ \\\n");
  fprintf(out, "\t\tCubeMain.o CubeView.o CubeViewUI.o \\\n");
  fprintf(out, "\t\t$(LINKFLTKGL) $(LINKFLTK) $(GLDLIBS)\n");
  fprintf(out, "CubeMain.o: CubeViewUI.h CubeView.h CubeViewUI.cxx\n");
  fprintf(out, "CubeView.o: CubeView.h\n");
  fprintf(out, "CubeViewUI.o:\tCubeViewUI.cxx CubeViewUI.h\n");
  fprintf(out, "CubeViewUI.cxx:\tCubeViewUI.fl ../fluid/fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "cube$(EXEEXT): cube.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ cube.o $(LINKFLTKGL) $(LINKFLTK) $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "fractals$(EXEEXT): fractals.o fracviewer.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ fractals.o fracviewer.o $(LINKFLTKGL) $(LINKFLTK) $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "fullscreen$(EXEEXT): fullscreen.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ fullscreen.o $(LINKFLTKGL) $(LINKFLTK) $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "glpuzzle$(EXEEXT): glpuzzle.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ glpuzzle.o $(LINKFLTKGL) $(LINKFLTK) $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "gl_overlay$(EXEEXT): gl_overlay.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ gl_overlay.o $(LINKFLTKGL) $(LINKFLTK) $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "shape$(EXEEXT): shape.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ shape.o $(LINKFLTKGL) $(LINKFLTK) $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "cairo_test$(EXEEXT): cairo_test.o\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(CAIROFLAGS) $(LDFLAGS) -o $@ cairo_test.o $(LINKFLTK) $(LINKFLTKCAIRO) $(GLDLIBS)\n");
  fprintf(out, "\n");
  fprintf(out, "#\n");
  fprintf(out, "# End of \"$Id: Makefile 8740 2011-05-26 14:43:46Z matt $\".\n");
  fprintf(out, "#\n");

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
