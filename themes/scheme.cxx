//
// "$Id: scheme.cxx,v 1.1 2002/02/10 22:57:50 spitzak Exp $"
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

// The "scheme" theme. This reads an earlier design for configuring fltk,
// a text-based "scheme" file, which described exactly what to put into
// the style structures for each widget class. We rejected this design
// because it was apparent that all interesting themes were completely
// defined by plugin code and thus the only part that was being used was
// the "themes" line from the file.

// The scheme argument (set by Fl_Style::scheme() or by the -scheme
// switch when Fl::arg() is used) is used to choose the scheme file to
// read, by adding ".scheme" to the end. If not specified or null,
// "default" is used.  There are some sample scheme files provided for
// your amusement, such as OldMotif.scheme.

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <fltk/Fl.h>
#include <fltk/fl_load_plugin.h>
#include <fltk/Fl_Color.h>
#include <fltk/Fl_Font.h>
#include <fltk/Fl_Labeltype.h>
#include <fltk/Fl_Style.h>
#include <fltk/Fl_Widget.h>
#include <fltk/x.h>
#include <config.h>
#include "conf.h"

#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#define access(a,b) _access(a,b)
#define R_OK        04
#endif

#ifndef PATH_MAX
#define PATH_MAX 128
#endif

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

extern "C" bool fltk_theme() {

  const char* scheme = Fl_Style::scheme();
  if (!scheme || !*scheme) scheme = "default";

  char temp[PATH_MAX];
  snprintf(temp, PATH_MAX, "%s.scheme", scheme);
  char sfile_buf[PATH_MAX];
  const char* sfile = fl_find_config_file(sfile_buf, PATH_MAX, temp);
  if (!sfile) {
    fprintf(stderr, "Cannot find scheme \"%s\"\n", temp);
    return false;
  }

  static bool recurse;
  if (recurse) {
    fprintf(stderr, "%s recusively loaded scheme.theme\n", sfile);
    return false;
  }

  conf_clear_cache();

  if (!::getconf(sfile, "general/themes", temp, sizeof(temp))) {
    recurse = true;
    Fl_Theme f = Fl_Style::load_theme(temp);
    if (f) f();
    else fprintf(stderr,"Unable to load %s theme\n", temp);
    recurse = false;
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

      // box around buttons within widget
      if (!getconf_list(key_list, "button box", valstr, sizeof(valstr)))
        if ( (boxtype = Fl_Boxtype_::find(valstr)) ) style->button_box = boxtype;

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

      // background of buttons within widget
      if (!getconf_list(key_list, "button color", valstr, sizeof(valstr)))
        style->button_color = grok_color(sfile, valstr);

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
  return true;
}

//
// End of "$Id: scheme.cxx,v 1.1 2002/02/10 22:57:50 spitzak Exp $".
//
