//
// "$Id: Fl_Style_start.cxx,v 1.5 2000/01/10 06:31:24 bill Exp $"
//
// Code for switching between named classes of style
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

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Style.H>
#include <string.h>


struct Fl_Style_Class {
  char* name;
  Fl_Named_Style* first_style;
  Fl_Style_Class* next;
  int draw_boxes_inactive; // should boxes ever be drawn inactive?
  double inactive_color_weight; // how much weight to FL_GRAY for inactive colors
  
  static Fl_Style_Class *first;
  static Fl_Style_Class *current;
  static char* current_name;

  void store_global();
  void get_global();

  Fl_Style_Class() { name = 0; }
};

Fl_Style_Class* Fl_Style_Class::first;
Fl_Style_Class* Fl_Style_Class::current = new Fl_Style_Class;
char* Fl_Style_Class::current_name = "style1";

void Fl_Style_Class::store_global()
{
  draw_boxes_inactive = Fl_Style::draw_boxes_inactive;
  inactive_color_weight = Fl_Style::inactive_color_weight;
}

void Fl_Style_Class::get_global()
{
  Fl_Style::draw_boxes_inactive = draw_boxes_inactive;
  Fl_Style::inactive_color_weight = inactive_color_weight;
}

void Fl_Style::start(char* name)
{
  if (!strcmp(name, Fl_Style_Class::current_name)) return;

  Fl_Style_Class *c, *oc = Fl_Style_Class::current;

  Fl_Style_Class::current->store_global();

  c = Fl_Style_Class::first;
  while (c) {
    if (!strcmp(c->name, name)) break;
    c = c->next;
  }

  if (!oc->name) {
    oc->name = Fl_Style_Class::current_name;
    oc->next = Fl_Style_Class::first;
    Fl_Style_Class::first = oc;
  }
  oc->first_style = Fl_Named_Style::first;
  Fl_Style_Class::current_name = name;

  if (c) {
    // The new style class already existed, we only need to change all
    // default_style pointers to the new styles
    c->get_global();
    Fl_Style_Class::current = c;
    Fl_Named_Style::first = c->first_style;
    Fl_Named_Style *l = Fl_Named_Style::first;
    while (l) {
      *(l->back_pointer) = l;
      l = l->next;
    }
  } else {
    // The style class is a new one, need to copy all style structures
    Fl_Style_Class::current = new Fl_Style_Class;

    Fl_Named_Style *l = Fl_Named_Style::first;
    Fl_Named_Style::first = 0;
    while (l) {
      *(l->back_pointer)=new Fl_Named_Style(l->name,l->revertfunc,l->back_pointer);
      l = l->next;
    }

    // Update parent entries 
    // (suppose only possible parent is Fl_Widget::default_style    
    for (l = Fl_Named_Style::first; l; l = l->next) {
      if (l->parent) l->parent = Fl_Widget::default_style;
    }
    // It used to revert the style here but I don't think that is
    // necessary?
  }
}

//
// End of "$Id: Fl_Style_start.cxx,v 1.5 2000/01/10 06:31:24 bill Exp $".
//
