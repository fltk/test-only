//
// "$Id: KDE.cxx,v 1.11 2003/03/31 07:17:49 spitzak Exp $"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// KDE version 1 & 2 theme.  This reads the KDE .kderc file to get all the
// style information.  If Fl::scheme() is not null then this skips over
// the windows/motif switching, which allows this to be loaded atop
// another theme easily.

#include <fltk/Widget.h>
#include <fltk/draw.h>
#include <fltk/ScreenInfo.h>
#include <fltk/events.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "conf.h"
#ifndef _WIN32
# include <fltk/x.h>
#endif
#ifndef PATH_MAX
# define PATH_MAX 128
#endif

using namespace fltk;

static bool colors_only = 0;

//kdewin_menu_text_box("kde windows menu window", "2AAUUIIXX", &fltk::downBox);

////////////////////////////////////////////////////////////////
#ifndef _WIN32

// I think these are for KDE1:
static Atom ChangeGeneral, ChangeStyle, ChangePalette;

// For KDE2:
static Atom KIPC;
enum KIPCMessage {
  PaletteChanged=0, FontChanged, StyleChanged, BackgroundChanged,
  SettingsChanged, IconChanged, UserMessage=32
};

// this function handles KDE style change messages
static int x_event_handler(int,Window*) {
  if (fltk::xevent.type != ClientMessage) return 0; // not a Client message
  XClientMessageEvent* cm = (XClientMessageEvent*)&fltk::xevent;
  if (cm->message_type == KIPC) {
    if (cm->data.l[0] == PaletteChanged) ;
    else if (cm->data.l[0] == FontChanged) ;
    else if (!colors_only && cm->data.l[0] == StyleChanged) ;
    else return 0;
  } else if (cm->message_type == ChangeGeneral) ;
  else if (cm->message_type == ChangePalette) ;
  else if (!colors_only && cm->message_type == ChangeStyle) ;
  else return 0;
  Style::reload_theme();

  return 1;
}

static void add_event_handler() {
  static bool done;
  if (!done) {
    done = 1;
    open_display();
    Atom kde_atom = XInternAtom(xdisplay, "KDE_DESKTOP_WINDOW", False);
    long data = 1;
    XChangeProperty(xdisplay, message_window, kde_atom, kde_atom, 32,
                    PropModeReplace, (unsigned char *)&data, 1);

    ChangeGeneral = XInternAtom(xdisplay, "KDEChangeGeneral", False);
    ChangeStyle = XInternAtom(xdisplay, "KDEChangeStyle", False);
    ChangePalette = XInternAtom(xdisplay, "KDEChangePalette", False);
    KIPC = XInternAtom(xdisplay, "KIPC_COMM_ATOM", False);
    fltk::add_event_handler(x_event_handler);
  }
}

#endif

#ifndef R_OK
#define R_OK 4
#endif
////////////////////////////////////////////////////////////////

extern "C" bool fltk_theme() {

  colors_only = Style::scheme() != 0;

  char kderc[PATH_MAX], home[PATH_MAX] = "", s[80] = "";
  const char* p = getenv("HOME");
  if (p) strncpy(home, p, sizeof(home));
  int kde1 = 0;
  snprintf(kderc, sizeof(kderc), "%s/.kde/share/config/kdeglobals", home);
  if (access(kderc, R_OK)) {
    snprintf(kderc, sizeof(kderc), "%s/.kderc", home);
    kde1 = 1;
  }
  conf_clear_cache();
  
#if 0
  int motif_style = 0;
  if (!colors_only) {
    if (!getconf(kderc, "KDE/widgetStyle", s, sizeof(s)) && !strcasecmp(s, "Motif"))
      motif_style = 1;
    if (motif_style) {
      // load the motif.theme to set it to motif appearance:
      Theme f = Style::load_theme("motif");
      if (f) f();
      else {
	fprintf(stderr,"Unable to load motif theme as part of kde\n");
	motif_style = 0;
      }
      // see below for modifications to the motif/windows themes
    }
  }
#endif

  Color foreground = NO_COLOR;
  if (!getconf(kderc, "General/foreground", s, sizeof(s)))
    foreground = color(s);

  Color background = NO_COLOR;
  if (!getconf(kderc, "General/background", s, sizeof(s)))
    background = color(s);

  Color select_foreground = NO_COLOR;
  if (!getconf(kderc, "General/selectForeground", s, sizeof(s)))
    select_foreground = color(s);

  Color select_background = NO_COLOR;
  if (!getconf(kderc, "General/selectBackground", s, sizeof(s)))
    select_background = color(s);

  // this one seems to do absolutely nothing
  Color window_foreground = NO_COLOR;
  if (!getconf(kderc, "General/windowForeground", s, sizeof(s)))
    window_foreground = color(s);

  Color window_background = NO_COLOR;
  if (!getconf(kderc, "General/windowBackground", s, sizeof(s)))
    window_background = color(s);

  Color button_foreground = NO_COLOR;
  if (!getconf(kderc, "General/buttonForeground", s, sizeof(s)))
    button_foreground = color(s);

  Color button_background = NO_COLOR;
  if (!getconf(kderc, "General/buttonBackground", s, sizeof(s)))
    button_background = color(s);

  const ScreenInfo& info = screenInfo();
  float pixels_per_point = info.dpi_y/72;

  char* sv; // to save strtok_r() state
  if (!getconf(kderc, "General/font", s, sizeof(s))) {
    char fontname[64] = "";

    if ( (p = strtok_r(s, ",", &sv)) ) {
      // strip leading foundry name, if any:
      char* q = strchr(p, '-');
      if (q) p = q+1;
      strncpy(fontname, p, sizeof(fontname));
    }

    float fontsize = 12;
    if ( (p = strtok_r(0, ",", &sv)) )
      fontsize = atoi(p) * pixels_per_point;

    strtok_r(0, ",", &sv); // I have no idea what this is
    static char fontencoding[32] = "";
    if ( (p = strtok_r(0, ",", &sv)) ) {
      strncpy(fontencoding, p, sizeof(fontencoding));
      if (!strncasecmp(fontencoding, "iso-", 4))
        memmove(fontencoding+3,fontencoding+4, strlen(fontencoding+4)+1); // hack!
    }
    int attrib = 0;
    if ( (p = strtok_r(0, ",", &sv)) && atoi(p) >= 75 ) attrib = BOLD;
    if ( (p = strtok_r(0, ",", &sv)) && atoi(p) > 0 ) attrib |= ITALIC;
    fltk::Font* font = fltk::font(fontname, attrib);
    if (font) {
// CET - FIXME    if (*fontencoding) fltk::encoding(fontencoding);
      Widget::default_style->labelfont = font;
      Widget::default_style->textfont = font;
      Widget::default_style->labelsize = fontsize;
      Widget::default_style->textsize = fontsize;
    }
  }

  if (!getconf(kderc, "General/menuFont", s, sizeof(s))) {
    char fontname[64] = "";

    if ( (p = strtok_r(s, ",", &sv)) ) {
      // strip leading foundry name, if any:
      char* q = strchr(p, '-');
      if (q) p = q+1;
      strncpy(fontname, p, sizeof(fontname));
    }

    float menufontsize = 12;
    if ( (p = strtok_r(0, ",", &sv)) ) {
      menufontsize = atoi(p) * pixels_per_point;
    }

    strtok_r(0, ",", &sv); // I have no idea what this is

    static char menufontencoding[32] = "";
    if ( (p = strtok_r(0, ",", &sv)) ) {
      strncpy(menufontencoding, p, sizeof(menufontencoding));
      if (!strncasecmp(menufontencoding, "iso-", 4))
        memmove(menufontencoding+3,menufontencoding+4, strlen(menufontencoding+4)+1); // hack!
    }
    int attrib = 0;
    if ( (p = strtok_r(0, ",", &sv)) && atoi(p) >= 75 ) attrib = BOLD;
    if ( (p = strtok_r(0, ",", &sv)) && atoi(p) > 0 ) attrib |= ITALIC;
    fltk::Font* menufont = fltk::font(fontname, attrib);
    Style* style;
    if (menufont && (style = Style::find("item"))) {
      style->labelfont = style->textfont = menufont;
      style->labelsize = style->textsize = menufontsize;

      if ((style = Style::find("menu bar"))) {
	style->labelfont = style->textfont = menufont;
	style->labelsize = style->textsize = menufontsize;
      }

      if ((style = Style::find("menu title"))) {
	style->labelfont = style->textfont = menufont;
	style->labelsize = style->textsize = menufontsize;
      }
    }
  }


  Style* style = Widget::default_style;

  // turn off highlighting:
  style->highlight_color = NO_COLOR;
  style->highlight_labelcolor = NO_COLOR;

  if (background)
    fltk::set_background(background);

  if (foreground) {
    style->labelcolor = foreground;
    style->textcolor = foreground;
    style->selection_textcolor = foreground;
  }

  if (button_background && button_background != background)
    style->buttoncolor = button_background;

  if (window_background && window_background != background)
    style->color = window_background;

  if (window_foreground)
    style->textcolor = style->selection_textcolor = window_foreground;

  if (select_background)
    style->selection_color = select_background;

  if (select_foreground)
    style->selection_textcolor = select_foreground;

  if (button_foreground && button_foreground != foreground &&
      (style = Style::find("button"))) {
    style->labelcolor = button_foreground;
  }

// Don't bother.  KDE gets it wrong.
//  if ((style = Style::find("scrollbar"))) {
//    if (background && window_background)
//      style->color = fltk::lerp(background, window_background, 0.5);
//  }

/* looks better for dark backgrounds
  if ((style = Style::find("scrollbar"))) {
    if (foreground) style->color = 48;
  }

  if ((style = Style::find("slider"))) {
    if (foreground) style->color = 48;
  }

  if ((style = Style::find("value slider"))) {
    if (foreground) style->color = 48;
  }
*/

#if 0
  if (!colors_only) {
    if (motif_style) {
//    setcolor(GRAY90, GRAY85); // looks better for dark backgrounds
      if ((style = Style::find("menu"))) {
	style->leading = 4;
      }
      if ((style = Style::find("check button"))) {
	style->selection_color = GRAY66;
	style->buttoncolor = GRAY75;
      }
    } else {

//      if ((style = Style::find("menu"))) {
//        style->leading = 8;
//        style->box = &kdewin_menu_text_box;
//      }

//      if ((style = Style::find("scrollbar"))) {
//        style->box = &kdewin_menu_text_box;
//      }
    }
  }
#endif

#ifndef _WIN32
  add_event_handler();
#endif
  return true;
}

//
// End of "$Id: KDE.cxx,v 1.11 2003/03/31 07:17:49 spitzak Exp $".
//
