//
// "$Id: Fl_theme.cxx,v 1.3 2000/01/09 01:06:12 bill Exp $"
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
    if (!suffix) return access(fn, R_OK) ? 0 : fn;
    snprintf(buffer, PATH_MAX, "%s%s", fn, suffix);
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
  }

  return access(buffer, R_OK) ? 0 : buffer;
}

int Fl::loadtheme(const char* theme, const char* scheme) {

  const char *tfile = fl_find_config_file(theme, "themes/", ".fltp");
  if (!tfile) {
    if (strcmp(theme, DEFAULT) || scheme)
      fprintf(stderr, "Can't find theme \"%s\"\n", theme);
    return -1;
  }

  return fl_load_plugin(tfile, "fltk_theme", scheme);

  return 0;
}

static int theme_loaded; // indicates that themes have been started
static void unloadtheme();

// Warning: this is called by every Fl_Window::show so it must be fast on
// multiple calls:
#ifndef WIN32
extern void fl_open_display();
#else
extern int fl_windows_colors();
#endif
int Fl::loadtheme() {
  if (theme_loaded) return 0;
  theme_loaded = 1;
#ifndef WIN32
  fl_open_display();
#else
  fl_windows_colors();
#endif
  if (!theme()) return 0;
  return loadtheme(theme(), scheme());
}

// string comparison, except NULL==NULL and does not crash if either is null
static int nullstrequ(const char* a, const char* b) {
  return a ? b && !strcmp(a,b) : !b;
}

int Fl::theme(const char* t, const char* s) {
  if (nullstrequ(t, theme()) && nullstrequ(s, scheme())) return 0;
  theme_ = t; scheme_ = s;
  return reloadtheme();
}

int Fl::reloadtheme() {
  if (!theme_loaded) return 0;
  unloadtheme();
  int r = loadtheme();
  redraw();
  return r;
}

////////////////////////////////////////////////////////////////
// unloadtheme():

// This is a lame version of Fl::theme_unloader(), it can only be called once!
static void (*theme_unload)() = 0;

void Fl::add_theme_unloader(void (*f)()) {
  if (theme_unload) theme_unload();
  theme_unload = f;
}

static void style_clear(Fl_Style *s) {
  Fl_Style temp = *s;
  memset((void*)s, 0, sizeof(*s));

  s->parent = temp.parent;
  s->revertfunc = temp.revertfunc;
}

extern const char* fl_up_box_revert;
extern const char* fl_down_box_revert;

static void unloadtheme() {
  if (theme_unload) {theme_unload(); theme_unload = 0;}

  fl_background((Fl_Color)0xc0c0c000);
  fl_up_box.data = fl_up_box_revert;
  fl_down_box.data = fl_down_box_revert;
  Fl_Style::draw_boxes_inactive = 1;
  Fl_Style::inactive_menu_hack = 0;
  Fl_Style::inactive_color_weight = 0.33f;

  for (Fl_Named_Style* p = Fl_Named_Style::first; p; p = p->next) {
    if (p->name) {
      style_clear(p);
      p->revertfunc(p);
    }
  }
  theme_loaded = 0;
}

//
// End of "$Id: Fl_theme.cxx,v 1.3 2000/01/09 01:06:12 bill Exp $".
//

