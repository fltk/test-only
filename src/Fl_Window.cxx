//
// "$Id: Fl_Window.cxx,v 1.19 1999/04/07 18:45:56 carl Exp $"
//
// Window widget class for the Fast Light Tool Kit (FLTK).
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

// The Fl_Window is a window in the fltk library.
// This is the system-independent portions.  The huge amount of 
// crap you need to do to communicate with X is in Fl_x.C, the
// equivalent (but totally different) crap for MSWindows is in Fl_win32.C

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/conf.h>
#include <config.h>

#define DEFAULT_STYLE ((Style*)default_style())

Fl_Widget::Style* Fl_Window::_default_style = 0;

Fl_Window::Style::Style() : Fl_Widget::Style() {
  widget(LABELTYPE) = FL_NO_LABEL;
  widget(BOX) = FL_FLAT_BOX;
}

// CET - for testing only-- will be removed
#ifndef WIN32
#include <signal.h>
static void stylechange(int) {
  if (Fl::style()) Fl::load_styles(1);
}
#endif

void Fl_Window::loadstyle() const {
  if (!Fl::s_background) {
    Fl::s_background = 1;

    char color[32];
    uchar r = 0xc0, g=0xc0, b=0xc0;

    int res = Fl::find("global/color/background", color, sizeof(color), 1);
    if (res == CONF_ERR_FILE) { Fl::load_styles(0); return; }
    if (!res) {
      fl_parse_color(color, r, g, b);
      Fl::background(r, g, b);
    }

    static struct { char* key; Fl_Color col; } colors[] = {
      { "DARK1", FL_DARK1 },
      { "DARK2", FL_DARK2 },
      { "DARK3", FL_DARK3 },
      { "LIGHT1", FL_LIGHT1 },
      { "LIGHT2", FL_LIGHT2 },
      { "LIGHT3", FL_LIGHT3 },
      { "FREE1", FL_FREE_COLOR },
      { "FREE2", (Fl_Color)(FL_FREE_COLOR+1) },
      { "FREE3", (Fl_Color)(FL_FREE_COLOR+2) },
      { "FREE4", (Fl_Color)(FL_FREE_COLOR+3) },
      { 0 }
    };

    char s[32];

    for (int i = 0; colors[i].key; i++) {
      sprintf(s, "global/color/%s", colors[i].key);
      int res = Fl::find(s, color, sizeof(color), 1);
      if (!res) {
        fl_parse_color(color, r, g, b);
        Fl::set_color(colors[i].col, r, g, b);
      }
    }

    if (!Fl::find("global/widget style", s, sizeof(s), 1)) {
      if (!strcasecmp(s, "sgi")) Fl::widget_style(FL_SGI_STYLE);
      else if(!strcasecmp(s, "windows")) Fl::widget_style(FL_WINDOWS_STYLE);
      else Fl::widget_style(FL_MOTIF_STYLE);
    }
  }

  if (!Fl::s_window) {
    Fl::s_window = 1;

    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "box", BOX },
      { 0 }
    };
    Fl::load_attributes("window", DEFAULT_STYLE->widget_, widget_attributes);
  }
  // CET - for testing only-- will be removed
#ifndef WIN32
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = (void (*)(int))stylechange;
  sigaction(SIGUSR1, &sa, 0);
#endif
}

void Fl_Window::_Fl_Window() {
  type(FL_WINDOW);
  i = 0;
  xclass_ = 0;
  icon_ = 0;
  iconlabel_ = 0;
  resizable(0);
  size_range_set = 0;
  callback((Fl_Callback*)default_callback);
}

Fl_Window::Fl_Window(int X,int Y,int W, int H, const char *l)
: Fl_Group(X, Y, W, H, l) {
  _Fl_Window();
  set_flag(FL_FORCE_POSITION);
}

Fl_Window::Fl_Window(int W, int H, const char *l)
// fix common user error of a missing end() with current(0):
  : Fl_Group((Fl_Group::current(0),0), 0, W, H, l) {
  _Fl_Window();
  clear_visible();
}

Fl_Window *Fl_Widget::window() const {
  for (Fl_Widget *o = parent(); o; o = o->parent())
    if (o->type() >= FL_WINDOW) return (Fl_Window*)o;
  return 0;
}

int Fl_Window::x_root() const {
  Fl_Window *p = window();
  if (p) return p->x_root() + x();
  return x();
}

int Fl_Window::y_root() const {
  Fl_Window *p = window();
  if (p) return p->y_root() + y();
  return y();
}

void Fl_Window::draw() {
  loadstyle();
  int savex = x(); x(0);
  int savey = y(); y(0);
  Fl_Group::draw();
  y(savey);
  x(savex);
}

void Fl_Window::label(const char *name) {label(name, iconlabel());}

void Fl_Window::iconlabel(const char *iname) {label(label(), iname);}

// the Fl::atclose pointer is provided for back compatability.  You
// can now just change the callback for the window instead.

void Fl::default_atclose(Fl_Window* window, void* v) {
  window->hide();
  Fl_Widget::default_callback(window, v); // put on Fl::read_queue()
}

void (*Fl::atclose)(Fl_Window*, void*) = default_atclose;

void Fl_Window::default_callback(Fl_Window* window, void* v) {
  Fl::atclose(window, v);
}

//
// End of "$Id: Fl_Window.cxx,v 1.19 1999/04/07 18:45:56 carl Exp $".
//
