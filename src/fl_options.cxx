//
// "$Id: fl_options.cxx,v 1.18 1999/11/10 12:21:55 bill Exp $"
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
#include <config.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifndef R_OK
#define R_OK 4
#endif

#ifndef PATH_MAX
#define PATH_MAX 128
#endif

const char* Fl::scheme_ = 0;
int Fl::use_schemes = 1;

const char* Fl::theme_ = 0;
int Fl::use_themes = 1;

int fl_scheme_loaded = 0;

static int is_path_rooted(const char *fn) {
  // see if an absolute name was given:
  if (fn[0] == '/' || fn[0] == '.'
#ifdef WIN32
      || fn[0] == '\\' || fn[1]==':'
#endif
      )
    return 1;
  return 0;
}

static Fl_Color grok_color(const char* cf, const char *colstr) {
  char key[80], val[32];
  const char *p = colstr;
  snprintf(key, sizeof(key), "colors/%s", colstr);
  if (!getconf(cf, key, val, sizeof(val))) p = val;

  if (strspn(p, "0123456789") == strlen(p)) return (Fl_Color)atol(p);

  return fl_rgb(p);
}


static Fl_Font grok_font(const char* cf, const char* fontstr) {
  char key[80], val[80];
  const char *p = fontstr;
  snprintf(key, sizeof(key), "fonts/%s", fontstr);
  if (!getconf(cf, key, val, sizeof(val))) p = val;

  if (strspn(p, "0123456789") == strlen(p)) return fl_fonts + atol(p);

  static char* fonts[] = {
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

int Fl::loadscheme(int b) {
  use_schemes = b;
  if (!b) return 0;

  char temp[PATH_MAX];
  if (is_path_rooted(scheme())) strncpy(temp, scheme(), sizeof(temp));
  else snprintf(temp, sizeof(temp), "schemes/%s", scheme());

  const char *p = fl_find_config_file(temp);
  if (!p) {
    if (strcasecmp(temp, "schemes/default"))
      fprintf(stderr, "Cannot load scheme: %s\n", temp);
    return -1;
  }

  //Fl_Style::revert();

  char sfile[PATH_MAX];
  strcpy(sfile, p);

  if (!::getconf(sfile, "general/themes", temp, sizeof(temp)))
    for (p = strtok(temp, CONF_WHITESPACE); p; p = strtok(NULL, CONF_WHITESPACE))
      theme(p);

  char valstr[80];
  Fl_Color col;

  if (!::getconf(sfile, "global colors/background", valstr, sizeof(valstr))) {
    col = grok_color(sfile, valstr);
    fl_background(fl_get_color(col));
  }

  static struct { char* key; Fl_Color col; } colors[] = {
    { "DARK1", FL_DARK1 },
    { "DARK2", FL_DARK2 },
    { "DARK3", FL_DARK3 },
    { "LIGHT1", FL_LIGHT1 },
    { "LIGHT2", FL_LIGHT2 },
    { "LIGHT3", FL_LIGHT3 },
    { 0 }
  };

  char s[32];

  for (int i = 0; colors[i].key; i++) {
    sprintf(s, "global colors/%s", colors[i].key);
    int res = ::getconf(sfile, s, valstr, sizeof(valstr));
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
        if ( (boxtype = Fl_Boxtype_::find(valstr)) ) style->set_box(boxtype);


      // glyph box type
      snprintf(temp, sizeof(temp), "widgets/%s/glyph box", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        if ( (boxtype = Fl_Boxtype_::find(valstr)) ) style->set_glyph_box(boxtype);


      // color
      snprintf(temp, sizeof(temp), "widgets/%s/color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->set_color(grok_color(sfile, valstr));

      // label color
      snprintf(temp, sizeof(temp), "widgets/%s/label color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->set_label_color(grok_color(sfile, valstr));

      // selection color
      snprintf(temp, sizeof(temp), "widgets/%s/selection color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->set_selection_color(grok_color(sfile, valstr));

      // selection text color
      snprintf(temp, sizeof(temp), "widgets/%s/selection text color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->set_selection_text_color(grok_color(sfile, valstr));

      // off color
      snprintf(temp, sizeof(temp), "widgets/%s/off color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->set_off_color(grok_color(sfile, valstr));

      // highlight color
      snprintf(temp, sizeof(temp), "widgets/%s/highlight color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->set_highlight_color(grok_color(sfile, valstr));

      // highlight label color
      snprintf(temp, sizeof(temp), "widgets/%s/highlight label color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->set_highlight_label_color(grok_color(sfile, valstr));

      // color
      snprintf(temp, sizeof(temp), "widgets/%s/text color", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->set_text_color(grok_color(sfile, valstr));

      // label font
      snprintf(temp, sizeof(temp), "widgets/%s/label font", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        if ( (font = grok_font(sfile, valstr)) ) style->set_label_font(font);

      // text font
      snprintf(temp, sizeof(temp), "widgets/%s/text font", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        if ( (font = grok_font(sfile, valstr)) ) style->set_text_font(font);

      // label type
      snprintf(temp, sizeof(temp), "widgets/%s/label type", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        if ( (labeltype = Fl_Labeltype_::find(valstr)) ) style->set_label_type(labeltype);

      // label size
      snprintf(temp, sizeof(temp), "widgets/%s/label size", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->set_label_size(atol(valstr));

      // text size
      snprintf(temp, sizeof(temp), "widgets/%s/text size", cent->data);
      if (!::getconf(sfile, temp, valstr, sizeof(valstr)))
        style->set_text_size(atol(valstr));

    }
    conf_list_free(&clist);
  }

  Fl::redraw();
  return 0;
}

int Fl::scheme(const char *s) {
  if (scheme_) free((void*)scheme_);
  if (s) {
    scheme_ = strdup(s);
    return loadscheme(1);
  }

  scheme_ = 0;
  return loadscheme(0);
}

const char* Fl::scheme() {
  static char* s = new char[PATH_MAX];

  if (!use_schemes) return 0;
  if (!scheme_) scheme_ = strdup("default");
  strncpy(s, scheme_, PATH_MAX);
  if (!strcasecmp(s, "default")) getconf("default scheme", s, PATH_MAX);

  return s;
}


int Fl::loadtheme(int b) {
  use_themes = b;
  if (!b) return 0;

  char temp[PATH_MAX];
  if (is_path_rooted(theme())) strncpy(temp, theme(), sizeof(temp));
  else snprintf(temp, sizeof(temp), "themes/%s", theme());

  const char *tfile = fl_find_config_file(temp);
  if (!tfile) {
    if (strcasecmp(temp, "themes/default"))
      fprintf(stderr, "Cannot load theme: %s\n", temp);
    return -1;
  }

  use_schemes = 1;

  int r;
  if ( (r = fl_load_plugin(tfile, "fltk_theme")) ) {
    fprintf(stderr, "Can't load theme \"%s\": %d\n", tfile, r);
    return r;
  }

  Fl::redraw();
  return 0;
}

int Fl::theme(const char *t) {
  if (theme_) free((void*)theme_);
  if (t) {
    theme_ = strdup(t);
    return loadtheme(1);
  }

  theme_ = 0;
  return loadtheme(0);
}

const char* Fl::theme() {
  static char* t = new char[PATH_MAX];

  if (!use_themes) return 0;
  if (!theme_) theme_ = strdup("default");
  strncpy(t, theme_, PATH_MAX);
  if (!strcasecmp(t, "default")) getconf("default theme", t, PATH_MAX);

  return t;
}


const char* fl_find_config_file(const char* fn) {
  static char* path = new char[PATH_MAX];

  if (is_path_rooted(fn)) {
    strcpy(path, fn);
  } else {
    char *cptr = getenv("HOME");
    if (cptr) {
      snprintf(path, PATH_MAX, "%s/.fltk/%s", cptr, fn);
      if (!access(path, R_OK)) return path;
    }

#ifndef WIN32
    snprintf(path, PATH_MAX, FLTK_LIBDIR "/lib/fltk/%s", fn);
#else
    char windir[PATH_MAX];
    GetWindowsDirectoryA(windir, sizeof(windir));
    snprintf(path, PATH_MAX, "%s\\fltk\\%s", windir, fn);
#endif
  }

  return access(path, R_OK) ? 0 : path;
}

int Fl::getconf(const char *key, char *value, int value_length)
{ return ::getconf(fl_find_config_file("flconfig"), key, value, value_length); }

Fl_Style* Fl_Style::find(const char* name) {
  for (Fl_Named_Style* p = Fl_Named_Style::first; p; p = p->next) {
    const char* a = p->name;
    const char* b = name;
    for (;;) {
      if (*a == '_') {
	if (*b == ' ' || *b == '_');
	else {a++; continue;}
      } else if (tolower(*a) != tolower(*b)) break;
      if (!*a && !*b) return p;
      a++;
      b++;
    }
  }
  return 0;
}

static void style_clear(Fl_Style *s) {
  Fl_Style *p = s->parent;
  memset(s, 0, sizeof(*s));
  s->parent = p;
}

extern int fl_extra_menu_spacing;

void Fl_Style::revert() {
  fl_background((Fl_Color)0xc0c0c000);

  fl_extra_menu_spacing = 0;

  fl_up_box.data = "2AAXXIIUU";
  fl_up_box.dx_ = 2;
  fl_up_box.dy_ = 2;
  fl_up_box.dw_ = 4;
  fl_up_box.dh_ = 4;
  fl_down_box.data = "2XXIIUUAA";
  fl_down_box.dx_ = 2;
  fl_down_box.dy_ = 2;
  fl_down_box.dw_ = 4;
  fl_down_box.dh_ = 4;

  for (Fl_Named_Style* p = Fl_Named_Style::first; p; p = p->next) {
    style_clear(p);
    if (p->revert) p->revert(p);
  }
  Fl::redraw();
}

//
// End of "$Id: fl_options.cxx,v 1.18 1999/11/10 12:21:55 bill Exp $".
//


