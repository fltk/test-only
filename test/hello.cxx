//
// "$Id: hello.cxx,v 1.10 2002/12/09 04:52:31 spitzak Exp $"
//
// The first demo program from the fltk documentation.
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <fltk/Window.h>
#include <fltk/Widget.h>
#include <fltk/run.h>
using namespace fltk;

int main(int argc, char **argv) {
  Window *window = new Window(300, 180);
  window->begin();
  Widget *box = new Widget(20, 40, 260, 100, "Hello, World!");
  box->box(UP_BOX);
  box->labelfont(HELVETICA_BOLD_ITALIC);
  box->labelsize(36);
  box->labeltype(SHADOW_LABEL);
  window->end();
  window->show(argc, argv);
  return run();
}

//
// End of "$Id: hello.cxx,v 1.10 2002/12/09 04:52:31 spitzak Exp $".
//
