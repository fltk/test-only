//
// "$Id: fl_kde.cxx,v 1.8 2000/07/30 03:46:04 spitzak Exp $"
//
// Theme plugin file for FLTK
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

// KDE version 1 & 2 theme.  This reads the KDE .kderc file to get all the
// style information.  If the argument "co" is passed then this skips
// over the windows/motif switching, which allows this to be loaded atop
// another theme easily.

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/fl_config.H>
#include <FL/fl_theme.H>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <config.h>

#ifndef PATH_MAX
#define PATH_MAX 128
#endif

static int colors_only = 0;

static const Fl_Frame_Box kdewin_menu_text_box(
  "kde windows menu window", "2AAUUIIXX", FL_DOWN_BOX);

////////////////////////////////////////////////////////////////
#ifndef WIN32
#include <unistd.h>
#include <FL/x.H>

// KIPC is what KDE2 uses
static Atom General, Style, Palette, KIPC;

// For KDE2
enum KIPCMessage {
  PaletteChanged=0, FontChanged, StyleChanged, BackgroundChanged,
  SettingsChanged, IconChanged, UserMessage=32
};

// this function handles KDE style change messages
static int x_event_handler(int) {
  if (fl_xevent->type != ClientMessage) return 0; // not a Client message
  XClientMessageEvent* cm = (XClientMessageEvent*)fl_xevent;
  if (cm->message_type != General && cm->message_type != Palette
      && cm->message_type != KIPC && cm->message_type != Style)
    return 0; // not the message we want
  if (cm->message_type == KIPC &&
      cm->data.l[0] != PaletteChanged &&
      cm->data.l[0] != FontChanged &&
      cm->data.l[0] != StyleChanged)
    return 0; // only interested in some IPC messages
  if (colors_only && cm->message_type == KIPC &&
      cm->data.l[0] != PaletteChanged &&
      cm->data.l[0] != FontChanged)
    return 0;
  if (colors_only && cm->message_type == Style)
    return 0;
  // Geez, really need to work on that logic...

  fl_kde(colors_only);

  return 1;
}

static void add_event_handler() {
  static int done = 0;
  if (!done) {
    done = 1;
    Atom kde_atom = XInternAtom(fl_display, "KDE_DESKTOP_WINDOW", False);
    long data = 1;
    XChangeProperty(fl_display, fl_message_window, kde_atom, kde_atom, 32,
                    PropModeReplace, (unsigned char *)&data, 1);

    General = XInternAtom(fl_display, "KDEChangeGeneral", False);
    Style = XInternAtom(fl_display, "KDEChangeStyle", False);
    Palette = XInternAtom(fl_display, "KDEChangePalette", False);
    KIPC = XInternAtom(fl_display, "KIPC_COMM_ATOM", False);
    Fl::add_handler(x_event_handler);
  }
}

// WAS: we need to do Fl::remove_handler(x_event_handler) when the
// theme is replaced.  NYI.

#endif

#ifndef R_OK
#define R_OK 4
#endif
////////////////////////////////////////////////////////////////

int fl_kde(int co) {
  colors_only = co;

  char kderc_path[PATH_MAX], home[PATH_MAX] = "", s[80] = "";
  const char* p = getenv("HOME");
  if (p) strncpy(home, p, sizeof(home));
  int kde1 = 0;
  snprintf(kderc_path, sizeof(kderc_path), "%s/.kde/share/config/kdeglobals", home);
  if (access(kderc_path, R_OK)) {
    snprintf(kderc_path, sizeof(kderc_path), "%s/.kderc", home);
    kde1 = 1;
  }
  Fl_Config kderc(kderc_path);

  int motif_style = 0;
  if (!kderc.get("KDE/widgetStyle", s, sizeof(s)) && !strcasecmp(s, "Motif"))
    motif_style = 1;
  if (!colors_only) {
    Fl::theme(motif_style ? "motif" : "windows");
    // see below for modifications to the motif/windows themes
  }

  Fl_Color foreground = FL_NO_COLOR;
  if (!kderc.get("General/foreground", s, sizeof(s)))
    foreground = fl_rgb(s);

  Fl_Color background = FL_NO_COLOR;
  if (!kderc.get("General/background", s, sizeof(s)))
    background = fl_rgb(s);

  Fl_Color select_foreground = FL_NO_COLOR;
  if (!kderc.get("General/selectForeground", s, sizeof(s)))
    select_foreground = fl_rgb(s);

  Fl_Color select_background = FL_NO_COLOR;
  if (!kderc.get("General/selectBackground", s, sizeof(s)))
    select_background = fl_rgb(s);

  // this one seems to do absolutely nothing
  Fl_Color window_foreground = FL_NO_COLOR;
  if (!kderc.get("General/windowForeground", s, sizeof(s)))
    window_foreground = fl_rgb(s);

  Fl_Color window_background = FL_NO_COLOR;
  if (!kderc.get("General/windowBackground", s, sizeof(s)))
    window_background = fl_rgb(s);

  Fl_Color button_foreground = FL_NO_COLOR;
  if (!kderc.get("General/buttonForeground", s, sizeof(s)))
    button_foreground = fl_rgb(s);

  Fl_Color button_background = FL_NO_COLOR;
  if (!kderc.get("General/buttonBackground", s, sizeof(s)))
    button_background = fl_rgb(s);

  Fl_Font font = 0;
  int fontsize = FL_NORMAL_SIZE;
  static char fontencoding[32] = "";
  char* sv; // to save strtok_r() state
  if (!kderc.get("General/font", s, sizeof(s))) {
    char fontname[64] = "";
    int fontbold = 0, fontitalic = 0;

    if ( (p = strtok_r(s, ",", &sv)) ) strncpy(fontname, p, sizeof(fontname));
    if ( (p = strtok_r(0, ",", &sv)) ) fontsize = atoi(p);
    strtok_r(0, ",", &sv); // I have no idea what this is
    if ( (p = strtok_r(0, ",", &sv)) ) {
      strncpy(fontencoding, p, sizeof(fontencoding));
      if (!strncasecmp(fontencoding, "iso-", 4))
        memmove(fontencoding+3,fontencoding+4, strlen(fontencoding+4)+1); // hack!
    }
    if ( (p = strtok_r(0, ",", &sv)) && !strcmp(p, "75") ) fontbold = 1;
    if ( (p = strtok_r(0, ",", &sv)) && !strcmp(p, "1") ) fontitalic = 1;
    font = fl_font(fontname);
    if (font && fontbold) font = font->bold();
    if (font && fontitalic) font = font->italic();
  }

  Fl_Font menufont = 0;
  int menufontsize = FL_NORMAL_SIZE;
  static char menufontencoding[32] = "";
  if (!kderc.get("General/menuFont", s, sizeof(s))) {
    char fontname[64] = "";
    int fontbold = 0, fontitalic = 0;

    if ( (p = strtok_r(s, ",", &sv)) ) strncpy(fontname, p, sizeof(fontname));
    if ( (p = strtok_r(0, ",", &sv)) ) menufontsize = atoi(p);
    strtok_r(0, ",", &sv); // I have no idea what this is
    if ( (p = strtok_r(0, ",", &sv)) ) {
      strncpy(menufontencoding, p, sizeof(menufontencoding));
      if (!strncasecmp(menufontencoding, "iso-", 4))
        memmove(menufontencoding+3,menufontencoding+4, strlen(menufontencoding+4)+1); // hack!
    }
    if ( (p = strtok_r(0, ",", &sv)) && !strcmp(p, "75") ) fontbold = 1;
    if ( (p = strtok_r(0, ",", &sv)) && !strcmp(p, "1") ) fontitalic = 1;
    menufont = fl_font(fontname);
    if (menufont && fontbold) menufont = font->bold();
    if (menufont && fontitalic) menufont = font->italic();
  }

  // turn off highlighting:
  Fl_Widget::default_style->highlight_color = 0;
  Fl_Widget::default_style->highlight_label_color = 0;

  Fl_Style* style = Fl_Widget::default_style;
  if (background) fl_background(background);

  if (foreground) {
    style->label_color = foreground;
    style->text_color = foreground;
    style->selection_text_color = foreground;
  }

  if (window_background) style->text_background = window_background;

  if (select_background) style->selection_color = select_background;

  if (select_foreground) style->selection_text_color = select_foreground;

  if ((style = Fl_Style::find("check button"))) {
    // this should be only on round and not check...
    if (foreground) style->selection_color = foreground;
  }

// Don't bother.  KDE gets it wrong.
//  if ((style = Fl_Style::find("scrollbar"))) {
//    if (background && window_background)
//      style->color = fl_color_average(background, window_background, 0.5);
//  }

/* looks better for dark backgrounds
  if ((style = Fl_Style::find("scrollbar"))) {
    if (foreground) style->color = 48;
  }

  if ((style = Fl_Style::find("slider"))) {
    if (foreground) style->color = 48;
  }

  if ((style = Fl_Style::find("value slider"))) {
    if (foreground) style->color = 48;
  }
*/
  if (button_background) {
    if ((style = Fl_Style::find("button"))) {
      style->color = button_background;
      style->label_color = button_foreground;
    }

    if ((style = Fl_Style::find("highlight button"))) {
      style->color = button_background;
      style->label_color = button_foreground;
    }

    if ((style = Fl_Style::find("return button"))) {
      style->color = button_background;
      style->label_color = button_foreground;
    }

    if ((style = Fl_Style::find("check button"))) {
      style->color = button_background;
      style->label_color = button_foreground;
    }

    if ((style = Fl_Style::find("light button"))) {
      style->color = button_background;
      style->label_color = button_foreground;
    }

//    if ((style = Fl_Style::find("menu button"))) {
//      style->color = button_background;
//      style->label_color = button_foreground;
//    }

    if ((style = Fl_Style::find("choice"))) {
      style->color = button_background;
      style->label_color = button_foreground;
    }

    if ((style = Fl_Style::find("scrollbar"))) {
      style->color = button_background;
      style->label_color = button_foreground;
    }

    if ((style = Fl_Style::find("slider"))) {
      style->color = button_background;
      style->label_color = button_foreground;
    }

    if ((style = Fl_Style::find("menu"))) {
      style->color = button_background;
    }

    if ((style = Fl_Style::find("item"))) {
      style->label_color = button_foreground;
    }
  }

  if (font) {
// CET - FIXME    if (*fontencoding) fl_encoding = fontencoding;
    Fl_Widget::default_style->label_font = font;
    Fl_Widget::default_style->text_font = font;
    Fl_Widget::default_style->label_size = fontsize;
    Fl_Widget::default_style->text_size = fontsize;
  }

  if (menufont && (style = Fl_Style::find("item"))) {
    style->label_font = style->text_font = menufont;
    style->label_size = style->text_size = menufontsize;

    if ((style = Fl_Style::find("menu bar"))) {
      style->label_font = style->text_font = menufont;
      style->label_size = style->text_size = menufontsize;
    }

    if ((style = Fl_Style::find("menu title"))) {
      style->label_font = style->text_font = menufont;
      style->label_size = style->text_size = menufontsize;
    }
  }

  if (!colors_only) {
    if (motif_style) {
//    fl_set_color(FL_LIGHT2, FL_LIGHT1); // looks better for dark backgrounds
      if ((style = Fl_Style::find("menu"))) {
	style->leading = 4;
      }
      if ((style = Fl_Style::find("check button"))) {
	style->selection_color = FL_DARK1;
	style->text_background = FL_GRAY;
      }
    } else {

      if ((style = Fl_Style::find("menu"))) {
	style->leading = 8;
	style->box = &kdewin_menu_text_box;
      }

      if ((style = Fl_Style::find("scrollbar"))) {
	style->box = &kdewin_menu_text_box;
      }
    }
  }

#ifndef WIN32
  add_event_handler();
#endif

  Fl::redraw();

  return 0;
}

//
// End of "$Id: fl_kde.cxx,v 1.8 2000/07/30 03:46:04 spitzak Exp $".
//
