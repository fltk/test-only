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

int write_fltk_makefiles() {
  /* This function will eventually create a complet FLTK makefile system. */
  /* We are currently limited to creating the Fluid Makefile. */
  if (!filename) {
    printf("Workspace must be saved first\n");
    return -1;
  }
  
  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Type::first;
  while (tgt) {
    if (tgt->is_target() && strcmp(tgt->name(), "Fluid")==0)
      break;
    tgt = tgt->next;
  }
  if (!tgt) {
    printf("FLUID target not found\n");
    return -1;
  }
  
  /* Create a new Makefile */
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "fluid/Makefile");
  FILE *out = fopen(buf, "wb");
  if (!out) {
    printf("Can't open FLUID Makefile\n");
    return -1;
  }
  
  fprintf(out, "#\n");
  fprintf(out, "# \"$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $\"\n");
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
  fprintf(out, "\n");
  
  /* loop through the target and write out all C++ files */
  fprintf(out, "CPPFILES = ");
  Fl_Type *src = tgt->next;
  while (src && src->level>tgt->level) {
    if (src->is_file()) {
      Fl_File_Type *f = (Fl_File_Type*)src;
      const char *fn = f->filename();
      if (fn) {
        const char *ext = fltk3::filename_ext(fn);
        if (ext && (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0)) {
          fprintf(out, "\\\n\t%s", fltk3::filename_name(fn));
        }
      }
    }
    src = src->next;
  }
  fprintf(out, "\n\n");
  
  fprintf(out, "################################################################\n");
  fprintf(out, "\n");
  fprintf(out, "OBJECTS = $(CPPFILES:.cxx=.o)\n");
  fprintf(out, "\n");
  fprintf(out, "include ../makeinclude\n");
  fprintf(out, "\n");
  fprintf(out, "all:\t$(FLUID) fluid$(EXEEXT)\n");
  fprintf(out, "\n");
  fprintf(out, "fluid$(EXEEXT):\t\t$(OBJECTS) $(LIBNAME) $(FLLIBNAME) \\\n");
  fprintf(out, "\t\t\t$(IMGLIBNAME)\n");
  fprintf(out, "\techo Linking $@...\n");
  fprintf(out, "\t$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LINKFLTKFORMS) $(LINKFLTKIMG) $(LDLIBS)\n");
  fprintf(out, "\t$(OSX_ONLY) $(INSTALL_BIN) fluid fluid.app/Contents/MacOS\n");
  fprintf(out, "\n");
  fprintf(out, "fluid-shared$(EXEEXT):\t$(OBJECTS) ../src/$(DSONAME) ../src/$(FLDSONAME) \\\n");
  fprintf(out, "\t\t\t../src/$(IMGDSONAME)\n");
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
  fprintf(out, "# End of \"$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $\".\n");
  fprintf(out, "#\n");

  fclose(out);
  
  return 0;
}


//
// End of "$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $".
//
