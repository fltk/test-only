//
// "$Id: KDE1.cxx,v 1.6 2000/01/17 21:36:19 bill Exp $"
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

// KDE version 1 theme.  This reads the KDE .kderc file to get all the
// style information.  If the argument "colors" is passed then this skips
// over the windows/motif switching, which allows this to be loaded atop
// another theme easily.

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "Conf_Database.H"

#ifndef PATH_MAX
#define PATH_MAX 128
#endif

static int colors_only;

static const Fl_Frame_Box kdewin_menu_window_box(
  "kde windows menu window", "2AAUUIIXX", FL_DOWN_BOX);

////////////////////////////////////////////////////////////////
#ifndef WIN32
#include <FL/x.H>

static Atom General, Style, Palette;

// this function handles KDE style change messages
static int x_event_handler(int) {
  if (fl_xevent->type != ClientMessage) return 0; // not a Client message
  XClientMessageEvent* cm = (XClientMessageEvent*)fl_xevent;
  if (cm->message_type != General && cm->message_type != Palette
      && (cm->message_type != Style || colors_only))
    return 0; // not the message we want
  Fl::reloadtheme();
  return 1;
}

static void add_event_handler() {
  // create an X window to catch KDE style change messages
  static int do_once = 0;
  if (do_once) return;
  do_once = 1;
  Atom kde_atom = XInternAtom(fl_display, "KDE_DESKTOP_WINDOW", False);
  Window root = RootWindow(fl_display, fl_screen);
  Window kde_message_win = XCreateSimpleWindow(fl_display, root, 0,0,1,1,0, 0, 0);
  long data = 1;
  XChangeProperty(fl_display, kde_message_win, kde_atom, kde_atom, 32,
		  PropModeReplace, (unsigned char *)&data, 1);
  General = XInternAtom(fl_display, "KDEChangeGeneral", False);
  Style = XInternAtom(fl_display, "KDEChangeStyle", False);
  Palette = XInternAtom(fl_display, "KDEChangePalette", False);
  Fl::add_handler(x_event_handler);
}
  
#endif
////////////////////////////////////////////////////////////////

extern "C" int fltk_theme(int argc, char**)
{
  colors_only = argc > 1;

  char kderc_path[PATH_MAX], home[PATH_MAX] = "", s[80];
  const char* p = getenv("HOME");
  if (p) strncpy(home, p, sizeof(home));
  snprintf(kderc_path, sizeof(kderc_path), "%s/.kderc", home);
  Conf_Database kderc(kderc_path);

  if (kderc.get("KDE/widgetStyle", s, sizeof(s))) return 1;
  int motif_style = !strcasecmp(s, "Motif") ? 1 : 0;
  if (!colors_only) {
    Fl::loadtheme(motif_style ? "motif" : "windows");
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
    font = fl_font(fontname);
    if (font && fontbold) font = font->bold;
    if (font && fontitalic) font = font->italic;
  }

  Fl_Widget::default_style->highlight_color = 0; // turn off widget highlighting

  Fl_Style* style;
  if (background) {
    fl_background(background);
    Fl_Widget::default_style->off_color = background;
  }

  if (foreground) {
    Fl_Widget::default_style->label_color = foreground;
    Fl_Widget::default_style->highlight_label_color = foreground;
    Fl_Widget::default_style->text_color = foreground;
    Fl_Widget::default_style->selection_text_color = foreground;
  }

  if ((style = Fl_Style::find("input"))) {
    if (foreground) style->off_color = foreground;
    if (window_background) style->color = window_background;
    if (select_background) style->selection_color = select_background;
    if (select_foreground) style->selection_text_color = select_foreground;
  }

  if ((style = Fl_Style::find("output"))) {
    if (window_background) style->color = window_background;
    if (select_background) style->selection_color = select_background;
    if (select_foreground) style->selection_text_color = select_foreground;
  }

  if ((style = Fl_Style::find("counter"))) {
    if (window_background) style->color = window_background;
  }

  if ((style = Fl_Style::find("browser"))) {
    if (window_background) style->color = window_background;
    if (select_background) style->selection_color = select_background;
    if (select_foreground) style->selection_text_color = select_foreground;
  }

  if ((style = Fl_Style::find("check button"))) {
    // this should be only on round and not check...
    if (foreground) style->selection_color = foreground;
    if (window_background) style->off_color = window_background;
  }

  if ((style = Fl_Style::find("menu item"))) {
    if (select_background) style->selection_color = select_background;
    if (select_foreground) style->selection_text_color = select_foreground;
  }

  if ((style = Fl_Style::find("menu title"))) {
    if (select_background) style->highlight_color = background;
    if (select_foreground) style->highlight_label_color = foreground;
    if (select_background) style->selection_color = background;
    if (select_foreground) style->selection_text_color = foreground;
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
  if (font) {
    if (*fontencoding) fl_encoding = fontencoding;
    Fl_Widget::default_style->label_font = font;
    Fl_Widget::default_style->text_font = font;
    Fl_Widget::default_style->label_size = fontsize;
    Fl_Widget::default_style->text_size = fontsize;
  }

  if (!colors_only) {

    // for title highlighting
    if ((style = Fl_Style::find("menu bar"))) {
      if (motif_style) style->highlight_color = 0;
      else style->highlight_label_color = foreground;
    }

    if (motif_style) {
//    fl_set_color(FL_LIGHT2, FL_LIGHT1); // looks better for dark backgrounds
      if ((style = Fl_Style::find("menu item"))) {
	style->selection_color = background;
	style->selection_text_color = foreground;
      }
      if ((style = Fl_Style::find("check button"))) {
	style->selection_color = FL_DARK1;
	style->off_color = background;
      }
    } else {
      Fl_Style::inactive_color_weight = 0.15f;

      if ((style = Fl_Style::find("menu window"))) {
	style->box = &kdewin_menu_window_box;
      }

      if ((style = Fl_Style::find("scrollbar"))) {
	style->glyph_box = &kdewin_menu_window_box;
      }
    }

    if ((style = Fl_Style::find("menu window"))) {
      style->leading = motif_style ? 4 : 8;
    }
  }

#ifndef WIN32
  add_event_handler();
#endif

  return 0;
}

//
// End of "$Id: KDE1.cxx,v 1.6 2000/01/17 21:36:19 bill Exp $".
//
