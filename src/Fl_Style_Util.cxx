//
// "$Id: Fl_Style_Util.cxx,v 1.5 1999/10/11 01:00:29 vincent Exp $"
//
// Style definition and plugin support
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

#include <FL/Fl_Style.H>
#include <FL/Fl_Style_Util.H>
#include <stdlib.h>
#include <string.h>

#include <FL/Fl_Widget.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Window.H>

static Fl_Style_D default_styles[] = {
  { "global", &Fl_Widget::default_style },
  { 0, 0 }
};
static int nb_styles = sizeof(default_styles)/sizeof(default_styles[0]) - 1;
static Fl_Style_D* styles = default_styles;


static int compare_style(const void* a, const void* b)
{
  return strcmp(((const Fl_Style_D*)a)->name, ((const Fl_Style_D*)b)->name);
}

Fl_Style_Definer::Fl_Style_Definer(char* name, Fl_Style& style)
{
  fl_add_style_def(name, &style);
}

bool fl_add_style_def(char* name, Fl_Style* style)
{
  if ( fl_search_style(name) ) return 0;
  nb_styles++;
  if (styles == default_styles) {
    styles = (Fl_Style_D*) malloc(sizeof(Fl_Style_D)*(nb_styles+1));
    memcpy(styles, default_styles, sizeof(default_styles));
  } else
    styles = (Fl_Style_D*) realloc(styles, sizeof(Fl_Style_D)*(nb_styles+1));

  styles[nb_styles-1].name = name;
  styles[nb_styles-1].style = style;
  styles[nb_styles].name = 0;
  styles[nb_styles].style = 0;

  qsort(styles, nb_styles, sizeof(Fl_Style_D), compare_style);
  return 1;
}

Fl_Style* fl_search_style(char* name)
{
  static bool sorted = 0;
  if (!sorted) {
    qsort(styles, nb_styles, sizeof(Fl_Style_D), compare_style);
    sorted = 1;
  }
  Fl_Style_D key = { name, 0 }, *def;
  def = (Fl_Style_D*)bsearch(&key, styles, nb_styles, sizeof(Fl_Style_D), compare_style);
  return def? def->style : 0;
}



// Definition of an entry in the style structure, 
// methods to set it according to a given string.
struct Fl_Style_Entry_D {
  char*  name;
  int    nb_parsers;
  Fl_Style_Entry_Parser **parsers;
};

static int nb_entries;
static Fl_Style_Entry_D* entries;

static int compare_entry(const void* a, const void* b)
{
  return strcmp(((const Fl_Style_Entry_D*)a)->name, ((const Fl_Style_Entry_D*)b)->name);
}

static Fl_Style_Entry_D* fl_search_style_entry(char* name)
{
  Fl_Style_Entry_D key = { name };
  return (Fl_Style_Entry_D*)bsearch(&key, entries, nb_entries, sizeof(Fl_Style_Entry_D), compare_entry);
}

void fl_add_style_entry_def(char* name, Fl_Style_Entry_Parser parser)
{
  Fl_Style_Entry_D* entry;
  entry = entries? fl_search_style_entry(name) : 0;
  if (entry == 0) {
    nb_entries++;
    if (entries) entries = (Fl_Style_Entry_D*) realloc(entries, sizeof(Fl_Style_Entry_D)*nb_entries);
    else entries = (Fl_Style_Entry_D*) malloc(sizeof(Fl_Style_Entry_D)*nb_entries);
    entries[nb_entries-1].name = name;
    entries[nb_entries-1].nb_parsers = 0;
    entries[nb_entries-1].parsers = 0;
    qsort(entries, nb_entries, sizeof(Fl_Style_Entry_D), compare_entry);
    entry = fl_search_style_entry(name);
  }
  entry->nb_parsers++;
  if (entry->parsers) 
    entry->parsers = (Fl_Style_Entry_Parser**) realloc(entry->parsers, 
			   sizeof(Fl_Style_Entry_Parser*)*(entry->nb_parsers+1));
  else entry->parsers = 
	(Fl_Style_Entry_Parser**) malloc(sizeof(Fl_Style_Entry_Parser*)*(entry->nb_parsers+1));
  entry->parsers[entry->nb_parsers-1] = parser;
  entry->parsers[entry->nb_parsers] = 0;
}

bool fl_parse_style_entry(char* name, Fl_Style& style, char* s)
{
  Fl_Style_Entry_D *e = fl_search_style_entry(name);
  if (e == 0 || e->parsers == 0) return 0;
  for(Fl_Style_Entry_Parser** p = e->parsers ; *p ; p++)
    if ((*p)(style, s)) return 1;
  return 0;
}

//
// End of "$Id: Fl_Style_Util.cxx,v 1.5 1999/10/11 01:00:29 vincent Exp $".
//
