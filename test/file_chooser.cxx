//
// "$Id: file_chooser.cxx,v 1.10 2002/12/10 02:01:05 easysw Exp $"
//
// File chooser test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/run.h>
#include <fltk/Button.h>
#include <fltk/Window.h>
#include <fltk/Input.h>
#include <fltk/file_chooser.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

fltk::Input *pattern, *current;

void pickfile(fltk::Widget *) {
  const char *p;
  p = fltk::file_chooser("Pick a file",pattern->value(),current->value());
  if (p) current->value(p);
}

void thecb(const char *name) {
  printf("Callback '%s'\n",name);
}

int main(int argc, char **argv) {
  fltk::Window window(200,115);
  window.begin();
  pattern = new fltk::Input(60,10,130,25,"Pattern: ");
  pattern->static_value("*");
  current = new fltk::Input(60,45,130,25,"Current: ");
  fltk::Button button(110,80,80,25,"&Choose file");
  button.callback(pickfile);
  window.end();
  window.show(argc, argv);
  //  Fl_FileIcon::load_system_icons();
  fltk::file_chooser_callback(thecb);
  return fltk::run();
}

//
// End of "$Id: file_chooser.cxx,v 1.10 2002/12/10 02:01:05 easysw Exp $".
//
