//
// "$Id: Fl_Style_Set.cxx,v 1.1 2002/02/10 22:57:48 spitzak Exp $"
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

#include <fltk/Fl_Style_Set.h>
#include <fltk/Fl_Widget.h>
#include <string.h>
#include <stdlib.h>

static Fl_Style_Set* current_set;

Fl_Style_Set::Fl_Style_Set() {
  // The first one constructed becomes the current one:
  if (!current_set) {
    current_set = this;
    return;
  }
  // Otherwise we must init to a blank set:
  theme = 0;
  scheme = 0;
  background = 0xc0c0c000; // fl_get_color(FL_GRAY); // 
  // Make all the named styles build copies of themselves and then
  // assign that list to this set:
  Fl_Named_Style* saved = Fl_Named_Style::first;
  Fl_Named_Style::first = 0;
  Fl_Named_Style* l = saved;
  Fl_Named_Style* default_style = Fl_Widget::default_style;
  while (l) {
    Fl_Named_Style* newstyle =
      new Fl_Named_Style(l->name,l->revertfunc,l->back_pointer);
    if (l == default_style) default_style = newstyle;
    l = l->next;
  }
  // Update parent entries 
  // (suppose only possible parent is Fl_Widget::default_style    
  for (l = Fl_Named_Style::first; l; l = l->next) {
    if (l->parent) l->parent = default_style;
  }
  first_style = Fl_Named_Style::first;
  Fl_Named_Style::first = saved;
}

void Fl_Style_Set::make_current() {
  if (this == current_set) return;

  // update the fields in the current set so they are saved:
  Fl_Style_Set* c = current_set;
  c->theme = Fl_Style::theme();
  c->scheme = Fl_Style::scheme();
  c->background = fl_get_color(FL_GRAY);
  c->first_style = Fl_Named_Style::first;

  current_set = this;

  Fl_Style::theme(theme);
  Fl_Style::scheme(scheme);
  fl_background(background);
  Fl_Named_Style *l = Fl_Named_Style::first = first_style;
  while (l) {
    *(l->back_pointer) = l;
    l = l->next;
  }
}

// The destructor has not been tested and I'm not sure what it should do
Fl_Style_Set::~Fl_Style_Set() {
#if 0
  if (current_set == this) {
    current_set = 0;
    return;
  }
  // probably should delete the list of styles? But widgets may be using them?
#endif
}

//
// End of "$Id: Fl_Style_Set.cxx,v 1.1 2002/02/10 22:57:48 spitzak Exp $".
//
