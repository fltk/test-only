//
// "$Id: Carl.cxx,v 1.1 2000/01/07 08:50:45 bill Exp $"
//
// This is the default theme provided with fltk.  It reads a database file
// that allows most of the Fl_Styles to be modified.  This should be named
// something other than "Carl" but I can't think of a good name for these
// data files.
//
// Copyright 1998-2000 by Bill Spitzak and others.
// Copyright 1995-2000 Carl Everard Thompson (clip@home.net)
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
#include "conf.h"
#include <FL/fl_load_plugin.H>
#include <FL/Fl_Color.H>
#include <FL/Fl_Font.H>
#include <FL/Fl_Labeltype.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Widget.H>

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
#if 1
  return fl_font(fontstr);
#else
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
#endif
}

extern "C"
int fltk_theme(int argc, char** argv)
{
  static int recurse = 0;
  if (recurse) {
    fprintf(stderr, "Recursive load of scheme file reader.  Try using include(file) instead");
    return -1;
  }

  const char* scheme = argc>1 ? argv[1] : "default";

  const char *p = fl_find_config_file(scheme, "schemes/", 0);
  if (!p) {
    if (!strcasecmp(scheme, "default")) return 0; // not an error for default
    fprintf(stderr, "Can't find scheme \"%s\"\n", scheme);
    return -1;
  }

  char sfile[PATH_MAX]; strncpy(sfile, p, PATH_MAX);
  char temp[PATH_MAX];

  if (!::getconf(sfile, "general/themes", temp, sizeof(temp)))
    for (p = strtok(temp, CONF_WHITESPACE); p;
	 p = strtok(NULL, CONF_WHITESPACE)) {
      recurse = 1;
      Fl::loadtheme(p);
      recurse = 0;
    }

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
  return 0;
}

//
// End of "$Id: Carl.cxx,v 1.1 2000/01/07 08:50:45 bill Exp $".
//
