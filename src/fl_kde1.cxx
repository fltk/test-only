//
// "$Id: fl_kde1.cxx,v 1.4 1999/11/14 08:42:50 bill Exp $"
//
// Make FLTK do the KDE thing!
//
// Copyright 1999 Bill Spitzak and others.
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

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_config.H>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <config.h>

#ifndef PATH_MAX
#define PATH_MAX 128
#endif

static Fl_Color parse_color(const char *instr) {
  char colstr[32];
  strncpy(colstr, instr, sizeof(colstr));
  const char *p;
  if (!(p = strtok(colstr, ","))) return FL_NO_COLOR;
  uchar r = atoi(p);
  if (!(p = strtok(NULL, ","))) return FL_NO_COLOR;
  uchar g = atoi(p);
  if (!(p = strtok(NULL, ","))) return FL_NO_COLOR;
  uchar b = atoi(p);

  Fl_Color col = fl_rgb(r, g, b);
  if (!col) return FL_BLACK;
  return col;
}

int fl_kde1() {
  Fl::theme(0); Fl::scheme(0); // will not be using themes or schemes

  char kderc_path[PATH_MAX], home[PATH_MAX] = "", s[80];
  const char* p = getenv("HOME");
  if (p) strncpy(home, p, sizeof(home));
  snprintf(kderc_path, sizeof(kderc_path), "%s/.kderc", home);
  Fl_Config kderc(kderc_path);

  if (kderc.get("KDE/widgetStyle", s, sizeof(s))) return 1;
  int motif_style = !strcasecmp(s, "Motif") ? 1 : 0;

  Fl_Color foreground = FL_NO_COLOR;
  if (!kderc.get("General/foreground", s, sizeof(s)))
    foreground = parse_color(s);

  Fl_Color background = FL_NO_COLOR;
  if (!kderc.get("General/background", s, sizeof(s)))
    background = parse_color(s);

  Fl_Style::revert(); // revert to FLTK default styles

  if (motif_style) fl_motif();
  else fl_windows();

  fl_kde1_colors(); // figure out the colors and fonts for KDE1

  Fl_Style* style;
  if ((style = Fl_Style::find("menu title"))) {
    style->set_selection_color(background);
    style->set_selection_text_color(foreground);
    if (motif_style)
      style->set_highlight_color(0);
    else {
      style->set_highlight_color(background);
      style->set_highlight_label_color(foreground);
    }
  }
  if (motif_style) {
//    fl_set_color(FL_LIGHT2, FL_LIGHT1); // looks better for dar backgrounds
    if ((style = Fl_Style::find("menu item"))) {
      style->set_highlight_color(background);
      style->set_highlight_label_color(foreground);
    }
    if ((style = Fl_Style::find("check button"))) {
      style->set_selection_color(FL_DARK1);
      style->set_off_color(background);
    }
  }
  if ((style = Fl_Style::find("menu window"))) {
    style->set_leading(motif_style ? 2 : 5);
  }

  Fl::redraw();

  return 0;
}

//
// End of "$Id: fl_kde1.cxx,v 1.4 1999/11/14 08:42:50 bill Exp $".
//
