//
// "$Id: fl_conf.cxx,v 1.4 2001/07/29 22:04:44 spitzak Exp $"
//
// Config file reading routines for the Fast Light Tool Kit (FLTK).
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

#include <string.h>
#include <stdio.h>
#include <limits.h>

#include <fltk/Fl.h>
#include <fltk/conf.h>
#include <config.h>

#if USE_CONF || USE_PLUGINS

#if defined(_WIN32)
# include <io.h>
# define access(a,b) _access(a,b)
# define R_OK 4
#else
# include <unistd.h>
#endif /* _WIN32 */

#ifndef PATH_MAX
# define PATH_MAX 128
#endif

// This should stay public so that programs can locate their config files
// easily.
const char* fl_find_config_file(const char* fn, bool cflag) {
  static char path[PATH_MAX];

  if (conf_is_path_rooted(fn)) {
    strcpy(path, fn);
    return (cflag || !access(path, R_OK)) ? path : 0;
  }
  char *cptr = getenv("HOME");
  if (cptr) {
    snprintf(path, sizeof(path), "%s%s%s", cptr, "/.fltk/", fn);
    if (cflag || !access(path, R_OK)) return path;
  }
#ifdef _WIN32
  cptr = getenv("HOMEPATH");
  if (cptr) {
    snprintf(path, sizeof(path), "%s%s%s", cptr, "/fltk/", fn);
    if (cflag || !access(path, R_OK)) return path;
  }
  cptr = getenv("USERPROFILE");
  if (cptr) {
    snprintf(path, sizeof(path), "%s/Application Data/fltk/%s", cptr, fn);
    if (cflag || !access(path, R_OK)) return path;
  }
#endif

  snprintf(path, sizeof(path), CONFIGDIR "/%s", fn);
  return (cflag || !access(path, R_OK)) ? path : 0;
}

#endif

#if USE_CONF

// flconfig_section is always "default".
// For now... - CET

extern FL_API const char* fl_config;
extern FL_API const char* fl_config_section;

int fl_getconf(const char *key, char *value, int value_length) {
  char temp[80];
  snprintf(temp, sizeof(temp), "%s/%s", fl_config_section, key);
  return ::getconf(fl_config, temp, value, value_length);
}

#endif

//
// End of "$Id: fl_conf.cxx,v 1.4 2001/07/29 22:04:44 spitzak Exp $".
//
