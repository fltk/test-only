//
// "$Id: Fl_Style_Util.cxx,v 1.3 1999/09/27 11:55:27 vincent Exp $"
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
// Default parsing functions
//

inline bool is(char* sep, char c)
{
  while(*sep) if (c == *sep) return 1; else sep++;
  return 0;
}

#include <string.h>
#include <ctype.h>
char* fl_parse_word(char*&s)
{
  static char* word;
  static char symbols[] = ",=();#[]";
  if (*s == 0) return s;
  while(*s && isspace(*s)) s++;
  char* s2;
  if (is(symbols, *s)) {
    s2 = s+1;
  } else {
    s2 = s;
    while(*s2 && !is(symbols, *s2) && !isspace(*s2)) s2++;
  }
  if(word) free(word);
  word = (char*) malloc(s2-s+1);
  memcpy(word, s, s2-s);
  word[s2-s] = 0;
  s = s2;
  return word;
}

#include <FL/Fl_Color.H>
Fl_Color fl_parse_color(char*&s)
{
  char* w = strdup(fl_parse_word(s));
  if (*w == '#') {
    w = fl_parse_word(s);
    return strtol(w, 0, 16) << 8;
  } else {
    char* w2 = fl_parse_word(s);
    if (*w2 == ',') {
      uchar r, g, b;
      r = uchar(strtod(w,0)*255);
      g = uchar(strtod(fl_parse_word(s),0)*255);
      fl_parse_word(s);
      b = uchar(strtod(fl_parse_word(s),0)*255);
      return fl_rgb(r, g, b);
    } else
      return Fl_Color(atoi(w));
  }
  free(w);
}


static bool parse_color(Fl_Style& style, char* s) { 
  style.set_color(fl_parse_color(s)); return 1; }
static bool parse_label_color(Fl_Style& style, char* s) { 
  style.set_label_color(fl_parse_color(s)); return 1; }
static bool parse_selection_color(Fl_Style& style, char* s) { 
  style.set_selection_color(fl_parse_color(s)); return 1; }
static bool parse_selection_text_color(Fl_Style& style, char* s) { 
  style.set_selection_text_color(fl_parse_color(s)); return 1; }
static bool parse_off_color(Fl_Style& style, char* s) { 
  style.set_off_color(fl_parse_color(s)); return 1; }
static bool parse_highlight_color(Fl_Style& style, char* s) { 
  style.set_highlight_color(fl_parse_color(s)); return 1; }
static bool parse_highlight_label_color(Fl_Style& style, char* s) { 
  style.set_highlight_label_color(fl_parse_color(s)); return 1; }
static bool parse_text_color(Fl_Style& style, char* s) { 
  style.set_text_color(fl_parse_color(s)); return 1; }



#include <FL/Fl_Boxtype.H>
static struct fl_box {
  char* name;
  Fl_Boxtype_ bt;
} boxtypes[] = {
{"flat_box", fl_flat_box},
{"flat_up_box", fl_flat_up_box},
{"flat_down_box", fl_flat_down_box},
{"normal_box", fl_normal_box},
{"down_box", fl_down_box},
{"thin_box", fl_thin_box},
{"thin_down_box", fl_thin_down_box},
{"engraved_box", fl_engraved_box},
{"embossed_box", fl_embossed_box},
{"border_box", fl_border_box},
{"shadow_box", fl_shadow_box},
{"rounded_box", fl_rounded_box},
{"rshadow_box", fl_rshadow_box},
{"rflat_box", fl_rflat_box},
{"round_box", fl_round_box},
{"round_down_box", fl_round_down_box},
{"diamond_box", fl_diamond_box},
{"diamond_down_box", fl_diamond_down_box},
{"oval_box", fl_oval_box},
{"oval_shadow_box", fl_oval_shadow_box},
{"oval_flat_box", fl_oval_flat_box},
{0,0}
};

static bool parse_boxtype(Fl_Style& style, char* s)
{
  char* w = fl_parse_word(s);
  for (fl_box* b = boxtypes; b->name; b++)
    if (!strcmp(w, b->name)) {
      style.set_box(&b->bt);
      return 1;
    }
  return 0;
}




#include "config.h"
#include <FL/Fl_Plugins.H>
#include <stdio.h>

#ifdef FLDLopen

#include <FL/dirent.h>

static void ReadPlugin(char* s, char* location, char* ext, char* func)
{
  FLDLhandle handle;
  if(!strcmp(s+strlen(s)-strlen(ext), ext)) {
    char s2[256];

    sprintf(s2, "%s%s", location, s);
    handle = FLDLopen(s2 );

    if(handle) {

      int used = 0;
      s[strlen(s)-strlen(ext)] = 0;

      Fl_Style_Plugin_Entry* e = (Fl_Style_Plugin_Entry*) FLDLsym( handle, func);
      if(e) {
	used = 1;
	e();
      }

      if(!used) FLDLclose(handle);

    } else {
      fprintf(stderr, "FLTK plugin error : %s\n", FLDLerror());
    }
  }
}

static void ReadPlugins(char* location, char* ext, char* func)
{
  dirent **d = 0;
  int n = filename_list(location, &d);

  for (int i = n; i > 0;) 
  {
    ReadPlugin(d[i-1]->d_name, location, ext, func);
    free((void*)(d[--i]));
  }
  free((void*)d);

}

void fl_read_style_plugins()
{
  static bool default_set = 0;
  if (!default_set) {
    fl_add_style_entry_def("box", parse_boxtype);
    fl_add_style_entry_def("color", parse_color);
    fl_add_style_entry_def("label_color", parse_label_color);
    fl_add_style_entry_def("selection_color", parse_selection_color);
    fl_add_style_entry_def("selection_text_color", parse_selection_text_color);
    fl_add_style_entry_def("off_color", parse_off_color);
    fl_add_style_entry_def("highlight_color", parse_highlight_color);
    fl_add_style_entry_def("highlight_label_color", parse_highlight_label_color);
    fl_add_style_entry_def("text_color", parse_text_color);
    default_set = 1;
  }

  // Priority order in case of duplicate plugin name is from the most local to the most global
  ReadPlugins("./", STYLE_EXTENSION, STYLE_FUNCTION);
  ReadPlugins("fltk.plugins/", STYLE_EXTENSION, STYLE_FUNCTION);
#ifndef WIN32
  char s[256];
  sprintf(s, "%s/.fltk/plugins/", getenv("HOME"));
  ReadPlugins(s, STYLE_EXTENSION, STYLE_FUNCTION);
#endif

  ReadPlugins("fltk.plugins/", THEME_EXTENSION, THEME_FUNCTION);
  ReadPlugins("./", THEME_EXTENSION, THEME_FUNCTION);
#ifndef WIN32
  sprintf(s, "%s/.fltk/plugins/", getenv("HOME"));
  ReadPlugins(s, THEME_EXTENSION, THEME_FUNCTION);
#endif

}

#else

void fl_read_style_plugins() {}

#endif

//
// End of "$Id: Fl_Style_Util.cxx,v 1.3 1999/09/27 11:55:27 vincent Exp $".
//
