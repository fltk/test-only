//
// "$Id$"
//
// Simple test of the fltk3::NativeFileChooser.
//
// Copyright 1998-2010 by Bill Spitzak and others.
// Copyright 2004 Greg Ercolano.
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
#include <fltk3/run.h>
#include <fltk3/ask.h>		// fltk3::beep()
#include <fltk3/Window.h>
#include <fltk3/Button.h>
#include <fltk3/Input.h>
#include <fltk3/Box.h>
#include <fltk3/NativeFileChooser.h>

// GLOBALS
fltk3::Input *G_filename = NULL;

void Butt_CB(fltk3::Widget*, void*) {
  // Create native chooser
  fltk3::NativeFileChooser native;
  native.title("Pick a file");
  native.type(fltk3::NativeFileChooser::BROWSE_FILE);
  native.filter("Text\t*.txt\n"
                "C Files\t*.{cxx,h,c}\n"
                "Apps\t*.{app}\n");		// TODO: need to add kNavSupportPackages to non-cocoa <FNFC>_MAC.cxx
  native.preset_file(G_filename->value());
  // Show native chooser
  switch ( native.show() ) {
    case -1: fprintf(stderr, "ERROR: %s\n", native.errmsg()); break;	// ERROR
    case  1: fprintf(stderr, "*** CANCEL\n"); fltk3::beep(); break;		// CANCEL
    default: 								// PICKED FILE
      if ( native.filename() ) {
        G_filename->value(native.filename());
      } else {
	G_filename->value("NULL");
      }
      break;
  }
}

int main(int argc, char **argv) {
  //// For a nicer looking browser under linux, call fltk3::FileIcon::load_system_icons();
  //// (If you do this, you'll need to link with fltk_images)
  //// NOTE: If you do not load the system icons, the file chooser will still work, but
  ////       no icons will be shown. However, this means you do not need to link in the
  ////       fltk_images library, potentially reducing the size of your executable.
  //// Loading the system icons is not required by the OSX or Windows native file choosers.
#if !defined(WIN32) && !defined(__APPLE__)
  fltk3::FileIcon::load_system_icons();
#endif

  int argn = 1;
#ifdef __APPLE__
  // OS X may add the process number as the first argument - ignore
  if (argc>argn && strncmp(argv[1], "-psn_", 5)==0)
    argn++;
  if (argc>argn+1 && strcmp(argv[1], "-NSDocumentRevisionsDebugMode")==0)
    argn+=2;
#endif
  
  fltk3::Window *win = new fltk3::Window(600, 100, "Native File Chooser Test");
  win->size_range(300, 100, 0, 100);
  win->begin();
  {
    int y = 10;
    G_filename = new fltk3::Input(80, y, win->w()-80-10, 25, "Filename");
    G_filename->value(argc <= argn ? "." : argv[argn]);
    G_filename->tooltip("Default filename");
    y += G_filename->h() + 5;
    fltk3::Button *but = new fltk3::Button(win->w()-80-10, win->h()-25-10, 80, 25, "Pick File");
    but->callback(Butt_CB);
    fltk3::Box *dummy = new fltk3::Box(80, 0, 430, 100);
    dummy->hide();
    win->resizable(dummy);
  }
  win->end();
  win->show(argc, argv);
  return(fltk3::run());
}

//
// End of "$Id$".
//

