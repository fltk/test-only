//
// "$Id: Fl_theme.cxx,v 1.7 2000/04/03 17:09:21 bill Exp $"
//
// Theme loading code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2000 by Bill Spitzak and others.
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
#include <FL/fl_load_plugin.H>
#include <FL/Fl_Style.H>
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

static const char DEFAULT[] = "default";
const char* Fl::theme_ = DEFAULT;
const char* Fl::scheme_ = 0;
Fl_Color fl_bg_switch = 0; // set by -bg in Fl_arg.cxx

////////////////////////////////////////////////////////////////
// loadtheme():

// this function is public so plugins can call it:
const char*
fl_find_config_file(const char* fn, const char* prefix, const char* suffix)
{
  static char buffer[PATH_MAX];
  if (suffix) {
    // see if it already has the suffix (is this necessary?)
    int n = strlen(suffix);
    int m = strlen(fn);
    if (m >= n && !strcmp(fn+m-n, suffix)) suffix = 0;
  }
  if (fn[0] == '/' || fn[0] == '.'
#ifdef WIN32
	  || fn[0] == '\\' || fn[1]==':'
#endif
      ) {
    if (suffix) {
      snprintf(buffer, PATH_MAX, "%s%s", fn, suffix);
      fn = buffer;
    }
  } else {
    if (!suffix) suffix = "";
    if (!prefix) prefix = "";
    char *cptr = getenv("HOME");
    if (cptr) {
      snprintf(buffer, PATH_MAX, "%s/.fltk/%s%s%s", cptr, prefix,fn,suffix);
      if (!access(buffer, R_OK)) return buffer;
    }
#ifndef WIN32
    snprintf(buffer, PATH_MAX, CONFIGDIR "/%s%s%s",prefix,fn,suffix);
#else
    char windir[PATH_MAX];
    GetWindowsDirectoryA(windir, sizeof(windir));
    snprintf(buffer, PATH_MAX, "%s/fltk/%s%s%s", windir,  prefix,fn,suffix);
#endif
    fn = buffer;
  }

  return access(fn, R_OK) ? 0 : fn;
}

int Fl::loadtheme(const char* theme, const char* scheme) {
#ifndef WIN32
  if (getuid() != geteuid()) {
    if (theme != DEFAULT || scheme)
      fprintf(stderr, "Theme loading disabled for setuid programs.\n");
    return -4;
  }
#endif

  const char *tfile = fl_find_config_file(theme, "themes/", ".fltp");
  if (!tfile) {
    // try using the theme name as a scheme name:
    if (theme != DEFAULT && !scheme)
      return loadtheme(DEFAULT, theme);
    // report errors only if user tried to set things:
    if (theme != DEFAULT || scheme)
      fprintf(stderr, "Can't find theme \"%s\"\n", theme);
    return -1;
  }

  const char* argv[3] = {tfile, scheme, 0 };
  return fl_load_plugin(tfile, "fltk_theme", scheme ? 2 : 1, argv);
}

////////////////////////////////////////////////////////////////

static int theme_loaded; // indicates that themes have been started

extern void fl_windows_colors();

// Warning: this is called by every Fl_Window::show so it must be fast on
// multiple calls:

int Fl::loadtheme() {
  if (theme_loaded) return 0;
  theme_loaded = 1;
  fl_windows_colors();
  if (!theme()) return 0;
  int r = loadtheme(theme(), scheme());
  if (fl_bg_switch) fl_background(fl_bg_switch);
  return r;
}

// string comparison, except NULL==NULL and does not crash if either is null
static int nullstrequ(const char* a, const char* b) {
  return a ? b && !strcmp(a,b) : !b;
}

// Right now only a single theme_unload function is remembered:
static void (*theme_unload)() = 0;
void Fl::add_theme_unloader(void (*f)()) {theme_unload = f;}

extern const char* fl_up_box_revert;
extern const char* fl_down_box_revert;

int Fl::reloadtheme() {
  // we don't do anything until themes are started up:
  if (!theme_loaded) return 0;

  // get rid of current theme:
  if (theme_unload) {theme_unload(); theme_unload = 0;}

  // revert global settings:
  fl_background((Fl_Color)0xc0c0c000);
  fl_up_box.data = fl_up_box_revert;
  fl_down_box.data = fl_down_box_revert;
  Fl_Style::draw_boxes_inactive = 1;

  // revert all the styles:
  for (Fl_Named_Style* p = Fl_Named_Style::first; p; p = p->next) {
    if (p->name) {
      Fl_Style temp = *p;
      memset((void*)p, 0, sizeof(Fl_Style));
      p->parent = temp.parent;
      p->revertfunc = temp.revertfunc;
      p->revertfunc(p);
    }
  }

  // force it to load the new theme:
  theme_loaded = 0;
  int r = loadtheme();
  redraw();
  return r;
}

int Fl::theme(const char* t, const char* s) {
  if (nullstrequ(t, theme()) && nullstrequ(s, scheme())) return 0;
  theme_ = t; scheme_ = s;
  return reloadtheme();
}

////////////////////////////////////////////////////////////////

#include <FL/math.h>

void fl_background(Fl_Color c) {
  // replace the gray ramp so that FL_GRAY is this color
  int r = (c>>24)&255;
  if (!r) r = 1; else if (r==255) r = 254;
  double powr = log(r/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
  int g = (c>>16)&255;
  if (!g) g = 1; else if (g==255) g = 254;
  double powg = log(g/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
  int b = (c>>8)&255;
  if (!b) b = 1; else if (b==255) b = 254;
  double powb = log(b/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
  for (int i = 0; i < FL_NUM_GRAY; i++) {
    double gray = i/(FL_NUM_GRAY-1.0);
    fl_set_color(fl_gray_ramp(i),
		 fl_rgb(uchar(pow(gray,powr)*255+.5),
			uchar(pow(gray,powg)*255+.5),
			uchar(pow(gray,powb)*255+.5)));
  }
}

//
// End of "$Id: Fl_theme.cxx,v 1.7 2000/04/03 17:09:21 bill Exp $".
//

