//
// "$Id: Fl_startup.cxx,v 1.1 2001/07/23 09:50:05 spitzak Exp $"
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
#include <fltk/vsnprintf.h>
#include <config.h>

#if defined(WIN32)
#  include <windows.h>
#  include <io.h>
#  define F_OK 04
#else
#  include <unistd.h>
#endif /* WIN32 */

#ifndef R_OK
#define R_OK 4
#endif

#ifndef PATH_MAX
#define PATH_MAX 128
#endif

const char* Fl::scheme_ = 0;
Fl_Color fl_bg_switch = 0; // set by -bg in Fl_arg.cxx
const char* fl_startup_theme = 0; // set by -theme in Fl_arg.cxx

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

static const char* flconfig = 0;

static void rest_of_reload();
static bool load_scheme(const char*);
static bool load_plugins(const char*);

// Get the display opened and the scheme/theme loaded. This is normally
// called when the first Fl_Window::show() is done. You can also call
// this to manually load the scheme.
static bool started;
void Fl::startup() {
  // This can be called any number of times:
  if (started) return;
  started = true;
  // Set up so the code from fl_theme_handler is called on X events:
  Fl::add_handler(theme_handler);
  // Now load everything:
  rest_of_reload(); // this does XOpenDisplay on X
}

// This is called on receipt of a theme-change event from the system,
// or when the user tries to change the scheme or theme.
void Fl::reload_scheme() {
  if (!started) return;
  // get us back into the vanilla state:
  Fl_Style::revert();
  fl_theme_handler(0);
  conf_clear_cache(); // Force rereading of config files.
  // remake all the changes:
  rest_of_reload();
  // and redraw so the changes are visible:
  Fl::redraw();
}

// This function goes from the "vanilla" state (where all the styles
// and everything else is in the state it was when fltk starts up)
// and the state set by the command line switches, config file, and
// plugins:
static void rest_of_reload() {

  fl_get_system_colors(); // this does XOpenDisplay on X

  // app_defined_reset_callback();

  // WAS: I believe all this getconf stuff can be replaced by code
  // that loads exactly one plugin:

  // Find Carl's file:
  if (!flconfig) {
    const char *p = fl_find_config_file("flconfig");
    if (p) flconfig = strdup(p);
  }

  // Carl's file may name plugins to load:
  char temp[PATH_MAX];
  if (!fl_getconf("plugins", temp, sizeof(temp))) load_plugins(temp);

  // It may name another Carl file to load, or -scheme may do this:
  const char* s = Fl::scheme();
  if (!s && !fl_getconf("scheme", temp, sizeof(temp))) s = temp;
  load_scheme(s);

  // It may name more plugins, or -theme may do this:
  const char *t = fl_startup_theme;
  if (!t && !fl_getconf("themes", temp, sizeof(temp))) t = temp;
  load_plugins(t);

  // always let -bg override anything the plugin did:
  if (fl_bg_switch) fl_background(fl_bg_switch);
}

// If the app changes the scheme, we check for actual differences and
// cause a reload if there are any:
bool Fl::scheme(const char* s) {
  // See if the new string is different. If not return false. If it
  // is, duplicate and store it in Fl::scheme().
  if (s && !strcasecmp(s, "none")) s = 0;
  if (!s) {
    if (!scheme_) return false;
    free((void*)scheme_);
    scheme_ = 0;
  } else {
    if (scheme_) {
      if (!strcasecmp(s, scheme_)) return false;
      free((void*)scheme_);
    }
    scheme_ = strdup(s);
  }
  // If there is a change, make it happen:
  reload_scheme();
  return true;
}

////////////////////////////////////////////////////////////////
// This function loads a space-seperated list of plugins from the
// search path and returns true if they all load ok.

static bool load_plugins(const char* t) {
  if (!t) return true;
  char temp[PATH_MAX], *p, *s;
  strncpy(temp, t, sizeof(temp));
  p = strtok_r(temp, CONF_WHITESPACE, &s);
  while (p) {
    if (!Fl::plugin(p)) return false;
    p = strtok_r(0, CONF_WHITESPACE, &s);
  }
  return true;
}

bool Fl::plugin(const char *t) {
// don't try to load themes if not linked to shared libraries
#ifdef FL_SHARED
//#if 1
// WAS - I temporarily enabled this for testing.
// CET - OK, but we should let people know that all themes will  not
// necessarily work when not linked dynamically to to save ourselves
// a support headache.
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
    return false;
  }

  strncpy(temp, tfile, sizeof(temp));
  Fl_Theme_Function f = (Fl_Theme_Function)fl_load_plugin(temp, "fltk_plugin");
  if (!f) {
    fprintf(stderr, "Cannot load plugin \"%s\"\n", temp);
    return false;
  }

  if ( f() ) {
    fprintf(stderr, "Cannot start plugin \"%s\"\n", temp);
    return false;
  }

  return true;
#else
  fprintf(stderr, "FLTK linked statically, cannot load plugin \"%s\"\n", t);
  return true;
#endif
}

////////////////////////////////////////////////////////////////
// Search for a file in fltk's idea of a search path:

#ifndef CONFIGDIR
#define CONFIGDIR "/fltk"
#endif

// This should stay public so that programs can locate their config files
// easily.
const char* fl_find_config_file(const char* fn, bool create) {
  static char path[PATH_MAX];

  if (conf_is_path_rooted(fn)) {
    strcpy(path, fn);
    return (create || !access(path, R_OK)) ? path : 0;
  }
  char *cptr = getenv("HOME");
  if (cptr) {
    snprintf(path, sizeof(path), "%s%s%s", cptr, "/.fltk/", fn);
    if (create || !access(path, R_OK)) return path;
  }
#ifdef WIN32
  cptr = getenv("HOMEPATH");
  if (cptr) {
    snprintf(path, sizeof(path), "%s%s%s", cptr, "/fltk/", fn);
    if (create || !access(path, R_OK)) return path;
  }
  cptr = getenv("USERPROFILE");
  if (cptr) {
    snprintf(path, sizeof(path), "%s/Application Data/fltk/%s", cptr, fn);
    if (create || !access(path, R_OK)) return path;
  }
#endif
  snprintf(path, sizeof(path), CONFIGDIR "/%s", fn);
  return (create || !access(path, R_OK)) ? path : 0;
}

////////////////////////////////////////////////////////////////
// Read Carl's main file ~/.fltk/flconfig

// this is necessary for future compatibility
static const char* flconfig_section = "default";

// flconfig_section is always "default".
// For now... - CET
int fl_getconf(const char *key, char *value, int value_length) {
  char temp[80];
  snprintf(temp, sizeof(temp), "%s/%s", flconfig_section, key);
  return ::getconf(flconfig, temp, value, value_length);
}

////////////////////////////////////////////////////////////////
// Read Carl's #2 file ~/.fltk/scheme/something

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

static bool load_scheme(const char *s) {

  if (!s || !*s) return 0;

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
  if (!::getconf(sfile, "general/themes", temp, sizeof(temp)))
    if (!load_plugins(temp)) {
      fprintf(stderr, "Could not load theme(s).  Will not load scheme!\n");
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

  // Carl: This does not seem to be working. This 'if' is never done:
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

  return 0;
}

//
// End of "$Id: Fl_startup.cxx,v 1.1 2001/07/23 09:50:05 spitzak Exp $".
//


