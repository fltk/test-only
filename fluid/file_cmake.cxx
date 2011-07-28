//
// "$Id: file_cmake.cxx 8870 2011-07-26 21:19:35Z matt $"
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

int write_fltk_cmake() {
  /* find the target named "Fluid" */
  Fl_Type *tgt = Fl_Target_Type::find("Fluid");
  if (!tgt) {
    printf("FLUID target not found\n");
    return -1;
  }
  
  /* Create a new CMakeLists.txt */
  char buf[2048];
  strcpy(buf, filename);
  strcpy((char*)fltk3::filename_name(buf), "fluid/CMakeLists.txt");
  FILE *out = fopen(buf, "wb");
  if (!out) {
    printf("Can't open FLUID CMakeLists.txt\n");
    return -1;
  }
  
  fprintf(out, "set(CPPFILES\n");
  
  Fl_File_Type *f;
  for (f = Fl_File_Type::first_file(tgt); f; f = f->next_file(tgt)) {
    if (f->is_cplusplus_code()) {
      fprintf(out, "\t%s\n", f->filename_name());
    }
  }
  
  fprintf(out, ")\n");  
  fprintf(out, "add_executable(fluid ${CPPFILES})\n");
  fprintf(out, "target_link_libraries(fluid fltk fltk_images fltk_forms)\n");
  fprintf(out, "\n");
  fprintf(out, "# link in optional libraries\n");
  fprintf(out, "if(FLTK_HAVE_CAIRO)\n");
  fprintf(out, "    target_link_libraries(fluid fltk_cairo)\n");
  fprintf(out, "endif(FLTK_HAVE_CAIRO)\n");
  fprintf(out, "\n");
  fprintf(out, "if(FLTK_USE_GL)\n");
  fprintf(out, "    target_link_libraries(fluid fltk_gl)\n");
  fprintf(out, "    target_link_libraries(fluid ${OPENGL_LIBRARIES})\n");
  fprintf(out, "endif(FLTK_USE_GL)\n");
  fprintf(out, "\n");
  fprintf(out, "if(USE_XFT)\n");
  fprintf(out, "    target_link_libraries(fluid ${X11_Xft_LIB})\n");
  fprintf(out, "endif(USE_XFT)\n");
  fprintf(out, "\n");
  fprintf(out, "if(HAVE_XINERAMA)\n");
  fprintf(out, "    target_link_libraries(fluid ${X11_Xinerama_LIB})\n");
  fprintf(out, "endif(HAVE_XINERAMA)\n");
  fprintf(out, "\n");
  fprintf(out, "install(TARGETS fluid\n");
  fprintf(out, "    EXPORT fltk-install\n");
  fprintf(out, "    DESTINATION ${PREFIX_BIN}\n");
  fprintf(out, ")\n");

  fclose(out);
  
  return 0;
}


//
// End of "$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $".
//
