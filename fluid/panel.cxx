//
// "$Id$"
//
// Panel base class code for the Fast Light Tool Kit (FLTK).
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

#include "panel.h"

extern fltk3::Window* widgetbin_panel;


void *const Fl_Panel::LOAD = (void *)"LOAD"; // "magic" pointer to indicate that we need to load values into the dialog
int Fl_Panel::numselected = 0;
Fl_Type *Fl_Panel::pSelectedType = 0L;


// ------------ Panel Base Class -----------------------------------------------


Fl_Panel::Fl_Panel(int x, int y, int w, int h, const char *name) 
: fltk3::DoubleWindow(w, h, name) {
}


Fl_Panel::~Fl_Panel() {
}


void Fl_Panel::select_type(Fl_Type *t) { 
  pSelectedType = t; 
}


Fl_Type *Fl_Panel::selected_type() {
  return pSelectedType;
}


Fl_Widget_Type *Fl_Panel::selected_widget() { 
  return (pSelectedType && pSelectedType->is_widget())?(Fl_Widget_Type*)pSelectedType:0L; 
}


Fl_File_Type *Fl_Panel::selected_file() { 
  return (pSelectedType && pSelectedType->is_file())?(Fl_File_Type*)pSelectedType:0L; 
}


Fl_Tool_Type *Fl_Panel::selected_tool() { 
  return (pSelectedType && pSelectedType->is_tool())?(Fl_Tool_Type*)pSelectedType:0L; 
}


void Fl_Panel::propagate_load(fltk3::Group* g, void* v) {
  if (v == Fl_Panel::LOAD) {
    fltk3::Widget*const* a = g->array();
    for (int i=g->children(); i--;) {
      fltk3::Widget* o = *a++;
      o->do_callback(o,Fl_Panel::LOAD);
    }
  }
}


// update the panel according to current widget set:
void Fl_Panel::load(RTTI_Query type_query) {
  if (!this) return;
  
  // find all the fltk3::Widget subclasses currently selected:
  numselected = 0;
  select_type(0);
  if (Fl_Type::current) {
    if ((Fl_Type::current->*type_query)())
      select_type(Fl_Type::current);
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if ((o->*type_query)() && o->selected) {
	numselected++;
	if (!selected_type()) select_type(o);
      }
    }
  }
  if (numselected)
    propagate_load(this);
  else
    hide();
}


/**
 Override show() to make sure that widgets don't overlap.
 Specifically, the widget bin and widget panel tend open in the same position. 
 One panel needs to be moved which interrupts workflow. This function moves
 the WIdget Panel away from the Widget Bin.
 */
void Fl_Panel::show()
{
  if (shown()) {
    // just raise th window, nothing else
    fltk3::DoubleWindow::show();
  } else {
    // show the window, then avoid overlap
    fltk3::DoubleWindow::show();
    if (widgetbin_panel && widgetbin_panel->shown()) {
      if (this->intersects(*widgetbin_panel)) {
        position(x(), widgetbin_panel->b()+40);
      }
    }
  }
}


//
// End of "$Id$".
//
