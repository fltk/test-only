//
// "$Id: Fl_Style_Set.cxx,v 1.4 2002/12/10 02:00:51 easysw Exp $"
//
// Code for switching between named classes of style
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

#include <fltk/StyleSet.h>
#include <fltk/Widget.h>
#include <string.h>
#include <stdlib.h>
using namespace fltk;

static StyleSet* current_set;

StyleSet::StyleSet() {
  // The first one constructed becomes the current one:
  if (!current_set) {
    current_set = this;
    return;
  }
  // Otherwise we must init to a blank set:
  theme = 0;
  scheme = 0;
  background = 0xc0c0c000; // get_color_index(GRAY75); // 
  // Make all the named styles build copies of themselves and then
  // assign that list to this set:
  NamedStyle* saved = NamedStyle::first;
  NamedStyle::first = 0;
  NamedStyle* l = saved;
  NamedStyle* default_style = Widget::default_style;
  while (l) {
    NamedStyle* newstyle =
      new NamedStyle(l->name,l->revertfunc,l->back_pointer);
    if (l == default_style) default_style = newstyle;
    l = l->next;
  }
  // Update parent entries 
  // (suppose only possible parent is Widget::default_style    
  for (l = NamedStyle::first; l; l = l->next) {
    if (l->parent) l->parent = default_style;
  }
  first_style = NamedStyle::first;
  NamedStyle::first = saved;
}

void StyleSet::make_current() {
  if (this == current_set) return;

  // update the fields in the current set so they are saved:
  StyleSet* c = current_set;
  c->theme = Style::theme();
  c->scheme = Style::scheme();
  c->background = get_color_index(GRAY75);
  c->first_style = NamedStyle::first;

  current_set = this;

  Style::theme(theme);
  Style::scheme(scheme);
  set_background(background);
  NamedStyle *l = NamedStyle::first = first_style;
  while (l) {
    *(l->back_pointer) = l;
    l = l->next;
  }
}

// The destructor has not been tested and I'm not sure what it should do
StyleSet::~StyleSet() {
#if 0
  if (current_set == this) {
    current_set = 0;
    return;
  }
  // probably should delete the list of styles? But widgets may be using them?
#endif
}

//
// End of "$Id: Fl_Style_Set.cxx,v 1.4 2002/12/10 02:00:51 easysw Exp $".
//
