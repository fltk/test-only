//
// "$Id: fl_options.cxx,v 1.69 2000/07/30 03:46:04 spitzak Exp $"
//
// Scheme and theme option handling code for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl.H>
#include <FL/conf.h>
#include <FL/fl_load_plugin.H>
#include <FL/Fl_Color.H>
#include <FL/Fl_Font.H>
#include <FL/Fl_Labeltype.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_theme.H>
#include <config.h>
#if defined(WIN32) || defined(__EMX__)
#  include <windows.h>
#  include <io.h>
extern "C" int access(const char *, int);
#  define F_OK 0
#else
#  include <unistd.h>
#endif /* WIN32 || __EMX__ */

#ifndef R_OK
#define R_OK 4
#endif

#ifndef PATH_MAX
#define PATH_MAX 128
#endif

const char* Fl::scheme_ = 0;
Fl_Color fl_bg_switch = 0; // set by -bg in Fl_arg.cxx
const char* fl_startup_theme = 0; // set by -theme in Fl_arg.cxx

static const char* flconfig = 0;
extern void fl_get_system_colors();

// this is necessary for future compatibility
static const char* flconfig_section = "default";

static int load_scheme(const char *s);
FL_API const char* fl_find_config_file(const char*);
static int fl_getconf(const char *key, char *value, int value_length);

// one-time startup stuff
static char beenhere;
void fl_startup() {
  if (!flconfig) {
    const char* p = fl_find_config_file("flconfig");
    if (p) flconfig = strdup(p);
  }
  Fl_Style::revert();

  char temp[PATH_MAX];
  const char* s = Fl::scheme();
  if (!s && !fl_getconf("scheme", temp, sizeof(temp))) s = temp;
  if (!s) fl_get_system_colors(); // WAS: I think it should do this always
  load_scheme(s);

  const char *t = fl_startup_theme;
  if (!t && !fl_getconf("theme", temp, sizeof(temp))) t = temp;
  if (t) Fl::theme(t);
  if (fl_bg_switch) fl_background(fl_bg_switch);

  beenhere = 1;
}

// When we change the scheme we automatically call reload_scheme if needed:
int Fl::scheme(const char* s) {
  if (scheme_ == s) return 0;
  if (s && scheme_ && !strcmp(s,scheme_)) return 0;
  scheme_ = s;
  if (beenhere) {
    // this is in fact what I think reload_scheme() should do:
    Fl_Style::revert();
    load_scheme(s);
  }
  return 1;
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

  return fl_font(p);
}

static int load_scheme(const char *s) {
  // The "none" case is apparently so -scheme none works.  There may be
  // a better way to do this
  if (!s || !strcasecmp(s, "none")) return 0;

  char temp[PATH_MAX];
  strncpy(temp, s, sizeof(temp));
  const char* p = access(temp, R_OK) ? 0 : temp;
  if (!p && !conf_is_path_rooted(s)) {
    snprintf(temp, sizeof(temp), "schemes/%s", s);
    p = fl_find_config_file(temp);
  }

  if (!p) {
    fprintf(stderr, "Cannot find scheme \"%s\"\n", temp);
    return -1;
  }

  char sfile[PATH_MAX];
  strncpy(sfile, p, sizeof(sfile));
  if (!::getconf(sfile, "general/themes", temp, sizeof(temp))) Fl::theme(temp);
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

      // box type
      if (!getconf_list(key_list, "box", valstr, sizeof(valstr)))
        if ( (boxtype = Fl_Boxtype_::find(valstr)) ) style->box = boxtype;


      // glyph box type
      if (!getconf_list(key_list, "text box", valstr, sizeof(valstr)))
        if ( (boxtype = Fl_Boxtype_::find(valstr)) ) style->text_box = boxtype;

      // color
      if (!getconf_list(key_list, "color", valstr, sizeof(valstr)))
        style->color = grok_color(sfile, valstr);

      // label color
      if (!getconf_list(key_list, "label color", valstr, sizeof(valstr)))
        style->label_color = grok_color(sfile, valstr);

      // selection color
      if (!getconf_list(key_list, "selection color", valstr, sizeof(valstr)))
        style->selection_color = grok_color(sfile, valstr);

      // selection text color
      if (!getconf_list(key_list, "selection text color", valstr, sizeof(valstr)))
        style->selection_text_color = grok_color(sfile, valstr);

      // off color
      if (!getconf_list(key_list, "text background", valstr, sizeof(valstr)))
        style->text_background = grok_color(sfile, valstr);

      // highlight color
      if (!getconf_list(key_list, "highlight color", valstr, sizeof(valstr)))
        style->highlight_color = grok_color(sfile, valstr);

      // highlight label color
      if (!getconf_list(key_list, "highlight label color", valstr, sizeof(valstr)))
        style->highlight_label_color = grok_color(sfile, valstr);

      // color
      if (!getconf_list(key_list, "text color", valstr, sizeof(valstr)))
        style->text_color = grok_color(sfile, valstr);

      // label font
      if (!getconf_list(key_list, "label font", valstr, sizeof(valstr)))
        if ( (font = grok_font(sfile, valstr)) ) style->label_font = font;

      // text font
      if (!getconf_list(key_list, "text font", valstr, sizeof(valstr)))
        if ( (font = grok_font(sfile, valstr)) ) style->text_font = font;

      // label type
      if (!getconf_list(key_list, "label type", valstr, sizeof(valstr)))
        if ( (labeltype = Fl_Labeltype_::find(valstr)) ) style->label_type = labeltype;

      // label size
      if (!getconf_list(key_list, "label size", valstr, sizeof(valstr)))
        style->label_size = (int)strtol(valstr,0,0);

      // text size
      if (!getconf_list(key_list, "text size", valstr, sizeof(valstr)))
        style->text_size = (int)strtol(valstr,0,0);

      conf_list_free(&key_list);
    }
    conf_list_free(&section_list);
  }

  Fl::redraw();
  return 0;
}

static int load_theme(const char *t) {
// don't try to load themes if not linked to shared libraries
#if 1 // def FL_SHARED // I temporarily enabled this for testing
  char temp[PATH_MAX];
  strncpy(temp, t, sizeof(temp));
  if (strlen(temp)<6 || strcasecmp(temp+strlen(temp)-6, ".theme"))
    strncat(temp, ".theme", sizeof(temp)-strlen(temp)-1);
  const char* tfile = access(temp, R_OK) ? 0 : temp;
  if (!tfile && !conf_is_path_rooted(t)) {
    snprintf(temp, sizeof(temp), "themes/%s", t);
    if (strlen(temp)<6 || strcasecmp(temp+strlen(temp)-6, ".theme"))
      strncat(temp, ".theme", sizeof(temp)-strlen(temp)-1);
    tfile = fl_find_config_file(temp);
  }
  if (!tfile) {
    fprintf(stderr, "Cannot find theme \"%s\"\n", temp);
    return -1;
  }

  strncpy(temp, tfile, sizeof(temp));
  Fl_Theme_Function f = (Fl_Theme_Function)fl_load_plugin(temp, "fltk_theme");
  if (!f) {
    fprintf(stderr, "Cannot load theme \"%s\"\n", temp);
    return -2;
  }

  if ( f() ) {
    fprintf(stderr, "Cannot start theme \"%s\"\n", temp);
    return -3;
  }

  Fl::redraw();
#endif
  return 0;
}

int Fl::theme(const char* t) {
  if (!t) return 0;
  char temp[PATH_MAX], *p, *s;
  strncpy(temp, t, sizeof(temp));
  p = strtok_r(temp, CONF_WHITESPACE, &s);
  while (p) {
    int r;
    if ( (r = load_theme(p)) ) return r;
    p = strtok_r(0, CONF_WHITESPACE, &s);
  }

  return 0;
}

#ifndef CONFIGDIR
#define CONFIGDIR "/fltk"
#endif

// WAS: This is public so the essai plugin can get at it.  This should be
// fixed by having the essai plugin use relative pathnames and passing it's
// location to it somehow.
const char* fl_find_config_file(const char* fn) {
  static char path[PATH_MAX];

  if (conf_is_path_rooted(fn)) {
    strcpy(path, fn);
    return (!access(path, R_OK)) ? path : 0;
  }
  char *cptr = getenv("HOME");
  if (cptr) {
    snprintf(path, sizeof(path), "%s%s%s", cptr, "/.fltk/", fn);
    if (!access(path, R_OK)) return path;
  }
#ifdef WIN32
  cptr = getenv("HOMEPATH");
  if (cptr) {
    snprintf(path, sizeof(path), "%s%s%s", cptr, "/fltk/", fn);
    if (!access(path, R_OK)) return path;
  }
  cptr = getenv("USERPROFILE");
  if (cptr) {
    snprintf(path, sizeof(path), "%s/Application Data/fltk/%s", cptr, fn);
    if (!access(path, R_OK)) return path;
  }
#endif

  snprintf(path, sizeof(path), CONFIGDIR "/%s", fn);
  return (!access(path, R_OK)) ? path : 0;
}

void Fl::reload_scheme() {
#ifdef WIN32
  // reload windows colors only if we haven't forced a scheme
  // WAS: This means a scheme will disable windows color changes, which
  // seems kind of wrong to me.  The schemes as implemented above will
  // allow the system colors to be used.  I think it should always
  // call get_system_colors and them load the scheme.
  if (!Fl::scheme()) { fl_get_system_colors(); window->redraw(); }
#else
  // Not clear what this is doing.  It will not handle the creation or
  // deletion of the flconfig file, or changes to the xrdb database.
  char scheme[80];
  if (!fl_getconf("scheme", scheme, sizeof(scheme))) load_scheme(scheme);
#endif
}

// flconfig_section is always "default".
static int fl_getconf(const char *key, char *value, int value_length) {
  char temp[80];
  snprintf(temp, sizeof(temp), "%s/%s", flconfig_section, key);
  return ::getconf(flconfig, temp, value, value_length);
}

//
// End of "$Id: fl_options.cxx,v 1.69 2000/07/30 03:46:04 spitzak Exp $".
//


