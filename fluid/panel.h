//
// "$Id$"
//
// Panel base class header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#ifndef FLUID3_PANEL_H
#define FLUID3_PANEL_H

#include "Fl_Type.h"

class Fl_File_Type;
class Fl_Tool_Type;
class Fl_Widget_Type;


extern class Fl_Panel *the_widget_panel;
extern class Fl_Panel *the_workspace_panel;

/*
 * This is a base class for all panels that allow editing multiple items.
 * Currently, this is the widget panel (the_widget_panel) and the workspace panel
 */
class Fl_Panel : public fltk3::DoubleWindow {
  static Fl_Type *pSelectedType;
public:
  Fl_Panel(int x, int y, int w, int h, const char *name=0L);
  ~Fl_Panel();
  void load(RTTI_Query query);
  void show(); // override the widget show()
  static void *const LOAD;
  static int numselected;
  static void select_type(Fl_Type *t);
  static Fl_Type *selected_type();
  static Fl_Widget_Type *selected_widget();
  static Fl_File_Type *selected_file();
  static Fl_Tool_Type *selected_tool();
  static void propagate_load(fltk3::Group* g, void* v=LOAD);
};


#endif

//
// End of "$Id$".
//
