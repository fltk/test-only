//
// "$Id: fl_kde1_colors.cxx,v 1.7 1999/11/20 04:42:47 vincent Exp $"
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
#include <FL/Fl_Window.H>
#include <FL/fl_config.H>
#include <FL/fl_draw.H>
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

#ifndef WIN32
#include <FL/x.H>

// this function handles KDE style change messages
static int x_event_handler(int) {
  if (fl_xevent->type != ClientMessage) return 0; // not a Client message

  Atom Palette = XInternAtom(fl_display, "KDEChangePalette", False);
  Atom General = XInternAtom(fl_display, "KDEChangeGeneral", False);
  XClientMessageEvent* cm = (XClientMessageEvent*)fl_xevent;
  if (cm->message_type != Palette && cm->message_type != General) return 0; // not the message we want

  fl_kde1_colors();

  return 1;
}
#endif

int fl_kde1_colors() {
  char kderc_path[PATH_MAX], home[PATH_MAX] = "", s[80];
  const char* p = getenv("HOME");
  if (p) strncpy(home, p, sizeof(home));
  snprintf(kderc_path, sizeof(kderc_path), "%s/.kderc", home);
  Fl_Config kderc(kderc_path);

  Fl_Color foreground = FL_NO_COLOR;
  if (!kderc.get("General/foreground", s, sizeof(s)))
    foreground = parse_color(s);

  Fl_Color background = FL_NO_COLOR;
  if (!kderc.get("General/background", s, sizeof(s)))
    background = parse_color(s);

  Fl_Color select_foreground = FL_NO_COLOR;
  if (!kderc.get("General/selectForeground", s, sizeof(s)))
    select_foreground = parse_color(s);

  Fl_Color select_background = FL_NO_COLOR;
  if (!kderc.get("General/selectBackground", s, sizeof(s)))
    select_background = parse_color(s);

  // this one seems to do absolutely nothing
  Fl_Color window_foreground = FL_NO_COLOR;
  if (!kderc.get("General/windowForeground", s, sizeof(s)))
    window_foreground = parse_color(s);

  Fl_Color window_background = FL_NO_COLOR;
  if (!kderc.get("General/windowBackground", s, sizeof(s)))
    window_background = parse_color(s);

  Fl_Font font = 0;
  int fontsize = 0;
  static char fontencoding[32] = "";
  if (!kderc.get("General/font", s, sizeof(s))) {
    char fontname[64] = "";
    int fontbold = 0, fontitalic = 0;

    if ( (p = strtok(s, ",")) ) strncpy(fontname, p, sizeof(fontname));
    if ( (p = strtok(NULL, ",")) ) fontsize = atoi(p);
    strtok(NULL, ","); // I have no idea what this is
    if ( (p = strtok(NULL, ",")) ) {
      strncpy(fontencoding, p, sizeof(fontencoding));
      if (!strncasecmp(fontencoding, "iso-", 4))
        memmove(fontencoding+3,fontencoding+4, strlen(fontencoding+4)+1); // hack!
    }
    if ( (p = strtok(NULL, ",")) && !strcmp(p, "75") ) fontbold = 1;
    if ( (p = strtok(NULL, ",")) && !strcmp(p, "1") ) fontitalic = 1;

#if 0
    // when this method exists this will be a lot easier!
    font = fl_font(fontname);
#else
    // doing these three manually saves startup time-- fl_list_fonts()
    // is _very_ slow
    if (!strcasecmp(fontname, "helvetica")) {
      font = FL_HELVETICA;
    } else if (!strcasecmp(fontname, "times")) {
      font = FL_TIMES;
    } else if (!strcasecmp(fontname, "courier")) {
      font = FL_COURIER;
    } else {
      Fl_Font* fontlist;
      int i, numfonts;
      for (i = 0, numfonts = fl_list_fonts(fontlist); i < numfonts; i++)
        if (!strcasecmp(fontlist[i]->name(), fontname)) break;

      if (i != numfonts) font = fontlist[i];
    }
#endif
    if (font && fontbold) font = font->bold;
    if (font && fontitalic) font = font->italic;
  }

  Fl_Widget::default_style->set_highlight_color(0); // turn off widget highlighting

  Fl_Style* style;
  if (background) {
    fl_background(background);
    Fl_Widget::default_style->set_off_color(background);
  }

  if (foreground) {
    Fl_Widget::default_style->set_label_color(foreground);
    Fl_Widget::default_style->set_highlight_label_color(foreground);
    Fl_Widget::default_style->set_text_color(foreground);
    Fl_Widget::default_style->set_selection_text_color(foreground);
  }

  if ((style = Fl_Style::find("input"))) {
    if (foreground) style->set_off_color(foreground);
    if (window_background) style->set_color(window_background);
    if (select_background) style->set_selection_color(select_background);
    if (select_foreground) style->set_selection_text_color(select_foreground);
  }

  if ((style = Fl_Style::find("output"))) {
    if (window_background) style->set_color(window_background);
    if (select_background) style->set_selection_color(select_background);
    if (select_foreground) style->set_selection_text_color(select_foreground);
  }

  if ((style = Fl_Style::find("counter"))) {
    if (window_background) style->set_color(window_background);
  }

  if ((style = Fl_Style::find("browser"))) {
    if (window_background) style->set_color(window_background);
    if (select_background) style->set_selection_color(select_background);
    if (select_foreground) style->set_selection_text_color(select_foreground);
  }

  if ((style = Fl_Style::find("check button"))) {
    // this should be only on round and not check...
    if (foreground) style->set_selection_color(foreground);
    if (window_background) style->set_off_color(window_background);
  }

  if ((style = Fl_Style::find("menu item"))) {
    if (select_background) style->set_selection_color(select_background);
    if (select_foreground) style->set_selection_text_color(select_foreground);
  }

  if ((style = Fl_Style::find("menu title"))) {
    if (select_background) style->set_highlight_color(background);
    if (select_foreground) style->set_highlight_label_color(foreground);
    if (select_background) style->set_selection_color(background);
    if (select_foreground) style->set_selection_text_color(foreground);
  }

// Don't bother.  KDE gets it wrong.
//  if ((style = Fl_Style::find("scrollbar"))) {
//    if (background && window_background)
//      style->set_color(fl_color_average(background, window_background, 0.5));
//  }

/* looks better for dark backgrounds
  if ((style = Fl_Style::find("scrollbar"))) {
    if (foreground) style->set_color(48);
  }

  if ((style = Fl_Style::find("slider"))) {
    if (foreground) style->set_color(48);
  }

  if ((style = Fl_Style::find("value slider"))) {
    if (foreground) style->set_color(48);
  }
*/
  if (font) {
    if (*fontencoding) fl_encoding = fontencoding;
    Fl_Widget::default_style->set_label_font(font);
    Fl_Widget::default_style->set_text_font(font);
    Fl_Widget::default_style->set_label_size(fontsize);
    Fl_Widget::default_style->set_text_size(fontsize);
  }

#ifndef WIN32
  // create an X window to catch KDE style change messages
  static int do_once = 0;
  if (!do_once) {
    do_once = 1;
    // turn schemes off, this takes their place
    // if this is run by a theme plugin, we turn it back on there
    Fl::use_schemes = 0;

    if (!fl_display) fl_open_display();
    Atom kde_atom = XInternAtom(fl_display, "KDE_DESKTOP_WINDOW", False);
    Window root = RootWindow(fl_display, fl_screen);
    Window kde_message_win = XCreateSimpleWindow(fl_display, root, 0,0,1,1,0, 0, 0);
    long data = 1;
    XChangeProperty(fl_display, kde_message_win, kde_atom, kde_atom, 32,
                    PropModeReplace, (unsigned char *)&data, 1);
  }

  // add handler to process KDE Change X events
  fl_theme_handler(x_event_handler);
#endif

  Fl::redraw();

  return 0;
}

//
// End of "$Id: fl_kde1_colors.cxx,v 1.7 1999/11/20 04:42:47 vincent Exp $".
//
