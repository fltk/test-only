//
// "$Id: Fl_Menu_global.cxx,v 1.10 2002/12/10 01:46:17 easysw Exp $"
//
// Global menu shortcut code for the Fast Light Tool Kit (FLTK).
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

// Make all the shortcuts in this menu global.

// Currently only one menu at a time and you cannot destruct the menu,
// is this sufficient?

// This should probably also put the items on the the Mac menubar.

#include <fltk/events.h>
#include <fltk/Menu.h>
#include <fltk/Window.h>

using namespace fltk;

static Menu* the_widget;

static int handler(int e, Window*) {
  if (e != SHORTCUT || modal()) return 0;
  Window::first(the_widget->window());
  return the_widget->handle_shortcut();
}

void Menu::global() {
  if (!the_widget) add_event_handler(handler);
  the_widget = this;
}

//
// End of "$Id: Fl_Menu_global.cxx,v 1.10 2002/12/10 01:46:17 easysw Exp $".
//
