//
// "$Id: Fl_startup.cxx,v 1.5 2001/07/24 21:14:27 robertk Exp $"
//
// Startup, scheme and theme handling code for the Fast Light
// Tool Kit (FLTK).
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

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <fltk/Fl.h>
#include <fltk/conf.h>
#include <fltk/fl_load_plugin.h>
#include <fltk/Fl_Color.h>
#include <fltk/Fl_Font.h>
#include <fltk/Fl_Labeltype.h>
#include <fltk/Fl_Style.h>
#include <fltk/Fl_Widget.h>
#include <fltk/fl_theme.h>
#include <fltk/x.h>
#include <config.h>

#ifndef PATH_MAX
#define PATH_MAX 128
#endif

#ifdef _MSC_VER	// MSVC++
#define R_OK	04
#endif

const char* Fl::scheme_ = 0;
Fl_Color fl_bg_switch = 0; // set by -bg in Fl_arg.cxx
const char* fl_startup_theme = 0; // set by -theme in Fl_arg.cxx

FL_API const char* fl_config = 0;

// This is necessary so that only one theme will process events at a time.
// It's not as easy as just removing the old theme's handler whenever a new
// theme is loaded because multiple themes may be loaded but that may not
// interfere with the use of this by one of them.
static Fl_Theme_Handler _theme_handler = 0;

void fl_theme_handler(Fl_Theme_Handler handler) {
  _theme_handler = handler;
}

static int theme_handler(int e) {
  return _theme_handler ? _theme_handler(e) : 0;
}

// this is necessary for future compatibility
FL_API const char* fl_config_section = "default";

// Startup stuff.  OK to execute more than once.
static bool beenhere = false;
void Fl::startup() {
  beenhere = true;
#ifndef WIN32
  fl_open_display();
#endif
  conf_clear_cache(); // Force rereading of config files.
  char temp[PATH_MAX];
  const char* s = Fl::scheme();
  if (!s && !fl_getconf("scheme", temp, sizeof(temp))) s = temp;
  Fl::scheme(s);

  const char *t = fl_startup_theme;
  if (!t && !fl_getconf("themes", temp, sizeof(temp))) t = temp;
  if (t) Fl::theme(t);
  if (fl_bg_switch) fl_background(fl_bg_switch);
}

////////////////////////////////////////////////////////////////
// The scheme file reader:

static Fl_Color grok_color(const char* cf, const char *colstr) {
  char key[80], val[32];
  const char *p = colstr;
  snprintf(key, sizeof(key), "aliases/%s", colstr);
  if (!getconf(cf, key, val, sizeof(val))) p = val;
  char* q;
  long l = strtoul(p, &q, 0);
  if (!*q) return (Fl_Color)l;
  return fl_rgb(p);
}

static Fl_Font grok_font(const char* cf, const char* fontstr) {
  char key[80], val[80];
  const char *p = fontstr;
  snprintf(key, sizeof(key), "aliases/%s", fontstr);
  if (!getconf(cf, key, val, sizeof(val))) p = val;

  char* q;
  long l = strtoul(p, &q, 0);
  if (!*q) return fl_fonts+l;

  return fl_find_font(p);
}

static int load_scheme(const char *s) {
  Fl_Style::revert();
  if (!s || !*s) {
    fl_get_system_colors();
    return 0;
  }

  char temp[PATH_MAX];
  strncpy(temp, s, sizeof(temp));
  const char* p = access(temp, R_OK) ? 0 : temp;
  if (!p && !conf_is_path_rooted(s)) {
    snprintf(temp, sizeof(temp), "schemes/%s", s);
    p = fl_find_config_file(temp);
  }

  if (!p) {
    fprintf(stderr, "Cannot find scheme \"%s\"\n", temp);
    fl_get_system_colors();
    return -1;
  }

  char sfile[PATH_MAX];
  strncpy(sfile, p, sizeof(sfile));
  if (!::getconf(sfile, "general/themes", temp, sizeof(temp)))
    if (Fl::theme(temp)) {
      fprintf(stderr, "Could not load theme(s).  Will not load scheme!\n");
      fl_get_system_colors();
      return -2;
    }
  char valstr[80];
  Fl_Color col;

  if (!::getconf(sfile, "global colors/background", valstr, sizeof(valstr))) {
    col = grok_color(sfile, valstr);
    fl_background(fl_get_color(col));
  }

  static struct { const char* key; Fl_Color col; } colors[] = {
    { "DARK1", FL_DARK1 },
    { "DARK2", FL_DARK2 },
    { "DARK3", FL_DARK3 },
    { "LIGHT1", FL_LIGHT1 },
    { "LIGHT2", FL_LIGHT2 },
    { "LIGHT3", FL_LIGHT3 },
    { 0, 0 }
  };

  for (int i = 0; colors[i].key; i++) {
    snprintf(temp, sizeof(temp), "global colors/%s", colors[i].key);
    int res = ::getconf(sfile, temp, valstr, sizeof(valstr));
    if (!res) {
      col = grok_color(sfile, valstr);
      fl_set_color(colors[i].col, col);
    }
  }

  conf_list section_list = 0, key_list = 0;
  conf_entry* cent;

  Fl_Font font;
  Fl_Labeltype labeltype;
  Fl_Boxtype boxtype;

  if (!getconf_sections(sfile, "widgets", &section_list)) {
    for (cent = section_list; cent; cent = cent->next) {
      Fl_Style* style = Fl_Style::find(cent->key);
      if (!style) continue;

      snprintf(temp, sizeof(temp), "widgets/%s", cent->key);
      getconf_keys(sfile, temp, &key_list);

      // box around widget
      if (!getconf_list(key_list, "box", valstr, sizeof(valstr)))
        if ( (boxtype = Fl_Boxtype_::find(valstr)) ) style->box = boxtype;

      // box around text/glyph within widget
      if (!getconf_list(key_list, "text box", valstr, sizeof(valstr)))
        if ( (boxtype = Fl_Boxtype_::find(valstr)) ) style->text_box = boxtype;

      // color of widget background
      if (!getconf_list(key_list, "color", valstr, sizeof(valstr)))
        style->color = grok_color(sfile, valstr);

      // color of widget's label
      if (!getconf_list(key_list, "label color", valstr, sizeof(valstr)))
        style->label_color = grok_color(sfile, valstr);

      // color of widget's background when widget is selected
      if (!getconf_list(key_list, "selection color", valstr, sizeof(valstr)))
        style->selection_color = grok_color(sfile, valstr);

      // color of widget's text when text selected
      // color of widget's label when widget selected
      // color of widget's glyph when widget selected and no glyph box
      if (!getconf_list(key_list, "selection text color", valstr, sizeof(valstr)))
        style->selection_text_color = grok_color(sfile, valstr);

      // background of text/glyph within widget
      if (!getconf_list(key_list, "text background", valstr, sizeof(valstr)))
        style->text_background = grok_color(sfile, valstr);

      // color of widget's background when widget is highlighted
      if (!getconf_list(key_list, "highlight color", valstr, sizeof(valstr)))
        style->highlight_color = grok_color(sfile, valstr);

      // color of widget's label when widget highlighted
      // color of widget's glyph/text when widget highlighted and no text/glyph box
      if (!getconf_list(key_list, "highlight label color", valstr, sizeof(valstr)))
        style->highlight_label_color = grok_color(sfile, valstr);

      // color of text/glyph within widget
      if (!getconf_list(key_list, "text color", valstr, sizeof(valstr)))
        style->text_color = grok_color(sfile, valstr);

      // font used for widget's label
      if (!getconf_list(key_list, "label font", valstr, sizeof(valstr)))
        if ( (font = grok_font(sfile, valstr)) ) style->label_font = font;

      // font used for text within widget
      if (!getconf_list(key_list, "text font", valstr, sizeof(valstr)))
        if ( (font = grok_font(sfile, valstr)) ) style->text_font = font;

      // type of widget's label
      if (!getconf_list(key_list, "label type", valstr, sizeof(valstr)))
        if ( (labeltype = Fl_Labeltype_::find(valstr)) ) style->label_type = labeltype;

      // font size of widget's label
      if (!getconf_list(key_list, "label size", valstr, sizeof(valstr)))
        style->label_size = (int)strtol(valstr,0,0);

      // font size of text within widget
      if (!getconf_list(key_list, "text size", valstr, sizeof(valstr)))
        style->text_size = (int)strtol(valstr,0,0);

      // leading
      if (!getconf_list(key_list, "leading", valstr, sizeof(valstr)))
        style->leading = (int)strtol(valstr,0,0);

      conf_list_free(&key_list);
    }
    conf_list_free(&section_list);
  }

  Fl::redraw();
  return 0;
}

static int schemes_enabled = 1;
void Fl::enable_schemes(int b) { schemes_enabled = b; }

// When we change the scheme we automatically call load_scheme if needed:
int Fl::scheme(const char* s) {
  if (s) {
    if (!strcasecmp(s, "none")) s = 0;
    else s = strdup(s);
  }
  if (scheme_) free((void*)scheme_);
  scheme_ = s;
  if (beenhere) return schemes_enabled ? load_scheme(s) : -99;
  return 1;
}

int Fl::reload_scheme() {
  if (beenhere) { Fl::startup(); return 0; }
  return 1;
}

static int load_plugin(const char *t) {
// don't try to load themes if not linked to shared libraries
#ifdef FL_SHARED
  char temp[PATH_MAX];
  strncpy(temp, t, sizeof(temp));
  if (strlen(temp)<7 || strcasecmp(temp+strlen(temp)-7, ".plugin"))
    strncat(temp, ".plugin", sizeof(temp)-strlen(temp)-1);
  const char* tfile = access(temp, R_OK) ? 0 : temp;
  if (!tfile) {
    strncpy(temp, t, sizeof(temp));
    if (strlen(temp)<6 || strcasecmp(temp+strlen(temp)-6, ".theme"))
      strncat(temp, ".theme", sizeof(temp)-strlen(temp)-1);
  }
  if (!tfile && !conf_is_path_rooted(t)) {
    snprintf(temp, sizeof(temp), "plugins/%s", t);
    if (strlen(temp)<7 || strcasecmp(temp+strlen(temp)-7, ".plugin"))
      strncat(temp, ".plugin", sizeof(temp)-strlen(temp)-1);
    tfile = fl_find_config_file(temp);
  }
  if (!tfile && !conf_is_path_rooted(t)) {
    snprintf(temp, sizeof(temp), "themes/%s", t);
    if (strlen(temp)<6 || strcasecmp(temp+strlen(temp)-6, ".theme"))
      strncat(temp, ".theme", sizeof(temp)-strlen(temp)-1);
    tfile = fl_find_config_file(temp);
  }
  if (!tfile) {
    fprintf(stderr, "Cannot find plugin \"%s\"\n", t);
    return -1;
  }

  strncpy(temp, tfile, sizeof(temp));
  Fl_Theme_Function f = (Fl_Theme_Function)fl_load_plugin(temp, "fltk_plugin");
  if (!f) {
    fprintf(stderr, "Cannot load plugin \"%s\"\n", temp);
    return -2;
  }

  if ( f() ) {
    fprintf(stderr, "Cannot start plugin \"%s\"\n", temp);
    return -3;
  }

  Fl::redraw();
  return 0;
#else
  fprintf(stderr, "FLTK linked statically, cannot load plugin or theme: %s\n", t);
  return 1;
#endif
}

static int plugin(const char* t) {
  if (!t) return 0;
  char temp[PATH_MAX], *p, *s = 0;
  strncpy(temp, t, sizeof(temp));
  p = strtok_r(temp, CONF_WHITESPACE, &s);
  while (p) {
    int r;
    if ( (r = load_plugin(p)) ) return r;
    p = strtok_r(0, CONF_WHITESPACE, &s);
  }

  return 0;
}

static int themes_enabled = 1;
void Fl::enable_themes(int b) { themes_enabled = b; }

int Fl::theme(const char* t) {
  if (!themes_enabled) return -99;
  return plugin(t);
}

// This startup code is executed only once before main()
static struct Startup {
  Startup() {
    if (!fl_config) {
      const char *p = fl_find_config_file("flconfig");
      if (p) fl_config = strdup(p);
    }
    Fl::add_handler(theme_handler);
    char temp[PATH_MAX];

    if (!fl_getconf("plugins", temp, sizeof(temp))) plugin(temp);
  }
} startup;


//
// End of "$Id: Fl_startup.cxx,v 1.5 2001/07/24 21:14:27 robertk Exp $".
//
