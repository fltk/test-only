//
// "$Id: Fl_Style_Plugins.cxx,v 1.4 1999/10/09 15:32:16 vincent Exp $"
//
// Style definition and plugin support
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

#include "config.h"
#include <FL/Fl_Plugins.H>
#include <FL/Fl_Style_Util.H>
#include <stdio.h>
#include <stdlib.h>

#ifdef FLDLopen

#if !HAVE_VSNPRINTF || defined(__hpux)
extern "C" {
int snprintf(char* str, size_t size, const char* fmt, ...);
}
#endif

#include <FL/dirent.h>

static bool theme = 0, loaded = 0;

static void ReadPlugin(char* s, const char* location, char* ext, char* func)
{
  FLDLhandle handle;
  loaded = 0;
  if(!strcmp(s+strlen(s)-strlen(ext), ext)) {
    char s2[256];

    snprintf(s2, 256, "%s%s", location, s);
    handle = FLDLopen(s2 );

    if(handle) {

      int used = 0;

      Fl_Style_Plugin_Entry* e = (Fl_Style_Plugin_Entry*) FLDLsym( handle, func);
      if(e) {
	used = 1;
	loaded = 1;
	e();
      }

      if(!used) FLDLclose(handle);

    } else {
      fprintf(stderr, "FLTK plugin error : %s\n", FLDLerror());
    }
  }
}

static void ReadPlugins(const char* location, char* ext, char* func)
{
  dirent **d = 0;
  int n = filename_list(location, &d);

  for (int i = n; i > 0;) 
  {
    if (!theme || !loaded) ReadPlugin(d[i-1]->d_name, location, ext, func);
    free((void*)(d[--i]));
  }
  free((void*)d);

}

#ifndef WIN32
#include <unistd.h>
#endif

void fl_read_style_plugins(const char* location)
{
  char s[1024];
  // Priority order is from the most local to the most global

  theme = 0;
#ifndef WIN32
  if (getuid() == geteuid()) {
#endif
    if (location) ReadPlugins(location, STYLE_EXTENSION, STYLE_FUNCTION);
    ReadPlugins("./", STYLE_EXTENSION, STYLE_FUNCTION);
    ReadPlugins("fltk.plugins/", STYLE_EXTENSION, STYLE_FUNCTION);
#ifndef WIN32
    snprintf(s, 1024, "%s/.fltk/plugins/", getenv("HOME"));
    ReadPlugins(s, STYLE_EXTENSION, STYLE_FUNCTION);
  }
  ReadPlugins(FLTK_LIBDIR"/lib/fltk/plugins/", STYLE_EXTENSION, STYLE_FUNCTION);
#else
  snprintf(s, 1024, "%s/fltk/plugins/", getenv("WINDIR"));
  ReadPlugins(s, STYLE_EXTENSION, STYLE_FUNCTION);
#endif

  theme = 1;
  loaded = 0;
#ifndef WIN32
  if (getuid() == geteuid()) {
#endif
    if (location) {
      ReadPlugins(location, THEME_EXTENSION, THEME_FUNCTION);
      if (loaded) return;
    }
    ReadPlugins("fltk.plugins/", THEME_EXTENSION, THEME_FUNCTION);
    if (loaded) return;
    ReadPlugins("./", THEME_EXTENSION, THEME_FUNCTION);
    if (loaded) return;
#ifndef WIN32
    snprintf(s, 1024, "%s/.fltk/plugins/", getenv("HOME"));
    ReadPlugins(s, THEME_EXTENSION, THEME_FUNCTION);
    if (loaded) return;
  }
  ReadPlugins(FLTK_LIBDIR"/lib/fltk/plugins/", THEME_EXTENSION, THEME_FUNCTION);
#else
  snprintf(s, 1024, "%s/fltk/plugins/", getenv("WINDIR"));
  ReadPlugins(s, THEME_EXTENSION, THEME_FUNCTION);
#endif

}

#else

void fl_read_style_plugins(const char*) {}

#endif

//
// Default parsing functions
//

#include <FL/Fl_Style.H>

inline bool is(char* sep, char c)
{
  while(*sep) if (c == *sep) return 1; else sep++;
  return 0;
}

#include <string.h>
#include <ctype.h>
char* fl_parse_word(char*&s)
{
  static char* word;
  static char symbols[] = ",=();#[]{}";
  if (*s == 0) return s;
  while(*s && isspace(*s)) s++;
  char* s2;
  if (is(symbols, *s)) {
    s2 = s+1;
  } else {
    s2 = s;
    while(*s2 && !is(symbols, *s2) && !isspace(*s2)) s2++;
  }
  if(word) free(word);
  word = (char*) malloc(s2-s+1);
  memcpy(word, s, s2-s);
  word[s2-s] = 0;
  s = s2;
  return word;
}

#include <FL/Fl_Color.H>
Fl_Color fl_parse_color(char*&s)
{
  char* w = strdup(fl_parse_word(s));
  if (*w == '#') {
    w = fl_parse_word(s);
    return strtol(w, 0, 16) << 8;
  } else {
    char* w2 = fl_parse_word(s);
    if (*w2 == ',') {
      uchar r, g, b;
      r = uchar(strtod(w,0)*255);
      g = uchar(strtod(fl_parse_word(s),0)*255);
      fl_parse_word(s);
      b = uchar(strtod(fl_parse_word(s),0)*255);
      return fl_rgb(r, g, b);
    } else
      return Fl_Color(atoi(w));
  }
  free(w);
}

// Link in the Fl_Shared_Image basic routines (~5Kb of code) so that plugins
// with images works (but they still need to provide fl_draw_image and the
// decompression routine ...)
#include <FL/Fl_Shared_Image.H>
fl_use_image_in_style() { Fl_Shared_Image::get(0,0); }

//
// End of "$Id: Fl_Style_Plugins.cxx,v 1.4 1999/10/09 15:32:16 vincent Exp $".
//
