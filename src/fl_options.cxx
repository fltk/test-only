//
// "$Id: fl_options.cxx,v 1.60 2000/06/12 09:01:55 carl Exp $"
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
//                                   Shell No 2

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

const char* fl_startup_theme = 0;
const char* Fl::scheme_ = 0;
Fl_Color fl_bg_switch = 0; // set by -bg in Fl_arg.cxx

const char* flconfig = 0;
static const char* flconfig_section = "default";

/* CET - FIXME - Add DISPLAY specific configuration at some point?
static int match(const char* s1, const char* s2) {
 // CET - FIXME - should do something to allow wildcards
  return strcasecmp(s1, s2) ? 0 : 1;
}

static int display_match(const char* s) {
  char display[PATH_MAX];
  char* p = getenv("DISPLAY");
  if (!p) return 0;

#ifndef WIN32
  if (*display == ':') {
    char temp[PATH_MAX];
    strcpy(temp, display);
#endif
  strncpy(display, p, sizeof(display));

  if (match(s, display)) return 1;

  if ( (p = strrchr(display, ':')) ) *p = (char)0; // remove display number
  if (match(s, display)) return 1;

  if ( (p = strchr(display, '.')) ) *p = (char)0; // remove domain
  if (match(s, display)) return 1;

  return 0;
}
*/

static int theme_handler(int);

extern void fl_get_system_colors();

// one-time startup stuff
void fl_startup() {
  if (!flconfig) {
    const char* p = fl_find_config_file("flconfig");
    if (p) flconfig = strdup(p);
  }

  /* CET - FIXME - Add DISPLAY specific configuration at some point?
  conf_list clist = 0;
  conf_entry* cent;
  getconf_sections(flconfig, 0, &clist);
  for (cent = clist; cent; cent = cent->next) if (display_match(cent->data)) {
    flconfig_section = strdup(cent->data);
    break;
  }
  conf_list_free(&clist);
  printf("section: %s\n", flconfig_section); // CET - FIXME
  */

  char temp[PATH_MAX];
  if (Fl::scheme()) Fl::scheme(Fl::scheme());
  else if (!Fl::getconf("scheme", temp, sizeof(temp))) Fl::scheme(temp);
  else fl_get_system_colors();
  if (fl_startup_theme) Fl::theme(fl_startup_theme);
  Fl::add_handler(theme_handler);

/* CET - FIXME - Fix mousewheel stuff?
  char temp[80];
  if (!Fl::getconf("mouse wheel/mode", temp, sizeof(temp)))
    fl_mousewheel_mode = atoi(temp);
  if (!Fl::getconf("mouse wheel/delta", temp, sizeof(temp)))
    fl_mousewheel_sdelta = strtod(temp, 0);
  if (!Fl::getconf("mouse wheel/axis", temp, sizeof(temp))) {
    if (!strcasecmp(temp, "x")) fl_mousewheel_delta = &Fl::e_x_delta;
    if (!strcasecmp(temp, "z")) fl_mousewheel_delta = &Fl::e_z_delta;
  }
#ifndef WIN32
  if (!Fl::getconf("mouse wheel/button 1", temp, sizeof(temp)))
    fl_mousewheel_b1 = atoi(temp);
  if (!Fl::getconf("mouse wheel/button 2", temp, sizeof(temp)))
    fl_mousewheel_b2 = atoi(temp);
#endif
*/

}

static Fl_Theme_Handler _theme_handler = 0;

void Fl::theme_handler(Fl_Theme_Handler handler) {
  _theme_handler = handler;
}

static int theme_handler(int e) {
  return _theme_handler ? _theme_handler(e) : 0;
}

static Fl_Color grok_color(const char* cf, const char *colstr) {
  char key[80], val[32];
  const char *p = colstr;
  snprintf(key, sizeof(key), "aliases/%s", colstr);
  if (!getconf(cf, key, val, sizeof(val))) p = val;

  if (strspn(p, "0123456789") == strlen(p)) return (Fl_Color)atol(p);

  return fl_rgb(p);
}

static Fl_Font grok_font(const char* cf, const char* fontstr) {
  char key[80], val[80];
  const char *p = fontstr;
  snprintf(key, sizeof(key), "aliases/%s", fontstr);
  if (!getconf(cf, key, val, sizeof(val))) p = val;

  if (strspn(p, "0123456789") == strlen(p)) return fl_fonts + atol(p);

  static const char* fonts[] = {
    "helvetica",
    "helvetica bold",
    "helvetica italic",
    "helvetica bold italic",
    "courier",
    "courier bold",
    "courier italic",
    "courier bold italic",
    "times",
    "times bold",
    "times italic",
    "times bold italic",
    "symbol",
    "screen",
    "screen bold",
    "dingbats",
    0
  };

  for (int i = 0; fonts[i]; i++)
    if (!strcasecmp(p, fonts[i])) return fl_fonts + i;

  // not found
  return 0;
}

int Fl::scheme(const char *s) {
  Fl::scheme_ = s;
  Fl_Style::revert();
  if (!s || !strcasecmp(s, "none")) return 0;

  char temp[PATH_MAX];
  strncpy(temp, Fl::scheme(), sizeof(temp));
  const char* p = access(temp, R_OK) ? 0 : temp;
  if (!p && !conf_is_path_rooted(Fl::scheme())) {
    snprintf(temp, sizeof(temp), "schemes/%s", Fl::scheme());
    p = fl_find_config_file(temp);
  }

  if (!p) {
    fprintf(stderr, "Cannot find scheme \"%s\"\n", temp);
    return -1;
  }

  char sfile[PATH_MAX];
  strncpy(sfile, p, sizeof(sfile));
  if (!::getconf(sfile, "general/themes", temp, sizeof(temp)))
    for (p = strtok(temp, CONF_WHITESPACE); p; p = strtok(NULL, CONF_WHITESPACE))
      Fl::theme(p);
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

  conf_list clist = 0;
  conf_entry* cent;

  Fl_Font font;
  Fl_Labeltype labeltype;
  Fl_Boxtype boxtype;

  if (!getconf_sections(sfile, "widgets", &clist)) {
    for (cent = clist; cent; cent = cent->next) {
      Fl_Style* style = Fl_Style::find(cent->data);
      if (!style) continue;

      // box type
      snprintf(temp, sizeof(temp), "widgets/%s/box", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        if ( (boxtype = Fl_Boxtype_::find(valstr)) ) style->box = boxtype;


      // glyph box type
      snprintf(temp, sizeof(temp), "widgets/%s/text box", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        if ( (boxtype = Fl_Boxtype_::find(valstr)) ) style->text_box = boxtype;

      // color
      snprintf(temp, sizeof(temp), "widgets/%s/color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->color = grok_color(sfile, valstr);

      // label color
      snprintf(temp, sizeof(temp), "widgets/%s/label color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->label_color = grok_color(sfile, valstr);

      // selection color
      snprintf(temp, sizeof(temp), "widgets/%s/selection color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->selection_color = grok_color(sfile, valstr);

      // selection text color
      snprintf(temp, sizeof(temp), "widgets/%s/selection text color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->selection_text_color = grok_color(sfile, valstr);

      // off color
      snprintf(temp, sizeof(temp), "widgets/%s/text background", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->text_background = grok_color(sfile, valstr);

      // highlight color
      snprintf(temp, sizeof(temp), "widgets/%s/highlight color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->highlight_color = grok_color(sfile, valstr);

      // highlight label color
      snprintf(temp, sizeof(temp), "widgets/%s/highlight label color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->highlight_label_color = grok_color(sfile, valstr);

      // color
      snprintf(temp, sizeof(temp), "widgets/%s/text color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->text_color = grok_color(sfile, valstr);

      // label font
      snprintf(temp, sizeof(temp), "widgets/%s/label font", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        if ( (font = grok_font(sfile, valstr)) ) style->label_font = font;

      // text font
      snprintf(temp, sizeof(temp), "widgets/%s/text font", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        if ( (font = grok_font(sfile, valstr)) ) style->text_font = font;

      // label type
      snprintf(temp, sizeof(temp), "widgets/%s/label type", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        if ( (labeltype = Fl_Labeltype_::find(valstr)) ) style->label_type = labeltype;

      // label size
      snprintf(temp, sizeof(temp), "widgets/%s/label size", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->label_size = atol(valstr);

      // text size
      snprintf(temp, sizeof(temp), "widgets/%s/text size", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->text_size = atol(valstr);

    }
    conf_list_free(&clist);
  }

  Fl::redraw();
  return 0;
}

int Fl::theme(const char *t) {
// don't try to load themes if not linked to shared libraries
#ifdef FL_SHARED
  if (!t) { Fl_Style::revert(); return 0; }
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
  int r = fl_load_plugin(temp, "fltk_theme");
  if (r) {
    fprintf(stderr, "Cannot load theme \"%s\": %d\n", temp, r);
    return r;
  }

  Fl::redraw();
#endif
  return 0;
}

#ifndef WIN32
#define HOMEVAR "HOME"
#define HOMEFLTKDIR "/.fltk/"
#else
#define HOMEVAR "HOMEPATH"
#define HOMEFLTKDIR "\\fltk\\"
#endif

#ifndef CONFIGDIR
#define CONFIGDIR "/fltk"
#endif

const char* fl_find_config_file(const char* fn, int cflag) {
  static char path[PATH_MAX];

  if (conf_is_path_rooted(fn)) {
    strcpy(path, fn);
    return (cflag || !access(path, R_OK)) ? path : 0;
  }
  char *cptr = getenv(HOMEVAR);
  if (cptr) {
    snprintf(path, sizeof(path), "%s%s%s", cptr, HOMEFLTKDIR, fn);
    if (cflag || !access(path, R_OK)) return path;
  }
#ifdef WIN32
  cptr = getenv("USERPROFILE");
  if (cptr) {
    snprintf(path, sizeof(path), "%s\\Application Data\\fltk\\%s", cptr, fn);
    if (cflag || !access(path, R_OK)) return path;
  }
#endif

  snprintf(path, sizeof(path), CONFIGDIR "/%s", fn);
  return (cflag || !access(path, R_OK)) ? path : 0;
}


int Fl::getconf(const char *key, char *value, int value_length) {
  char temp[80];
  snprintf(temp, sizeof(temp), "%s/%s", flconfig_section, key);
  return ::getconf(flconfig, temp, value, value_length);
}

//
// End of "$Id: fl_options.cxx,v 1.60 2000/06/12 09:01:55 carl Exp $".
//


