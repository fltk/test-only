#include <FL/Fl_Plugins.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Style_Util.H>
#include <FL/Fl_Window.H>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(WIN32) && !defined(CYGNUS)
# include <direct.h>
#else
# include <unistd.h>
#endif

extern "C" FLDLE void theme_plugin();


static Fl_Style* style = &Fl_Widget::default_style;


// the minimalistic conf reader !
static bool read_conf(char* f)
{
  FILE* fp = fopen(f, "r");
  if (!fp) return 0;
  while (!feof(fp)) {
    char buf[1024], *s=buf, *w;
    fgets(s, 1023, fp);
    if(*s == '#') continue;
    int l = strlen(s);
    if (l && s[l-1] == '\n') s[l-1]=0;
    if (*s == '[') {
      s++;
      w = fl_parse_word(s);
      style = fl_search_style(w);
    } else if (style) {
      w = strdup(fl_parse_word(s));
      if (*fl_parse_word(s) == '=') {
	fl_parse_style_entry(w, *style, s);
      }
      free(w);
    }
  }
  fclose(fp);
  return 1;
}

#include "config.h"
FLDLE void theme_plugin()
{
  if (!read_conf("fltkrc")) {
    char s[1024];
    getcwd(s, 1023);
    chdir(getenv("HOME"));
    chdir(".fltk");
    if (!read_conf("../.fltkrc")) {
#ifndef WIN32
      chdir(FLTK_LIBDIR"/lib/fltk");
      read_conf("fltkrc");
#else
      chdir(getenv("WINDIR"));
      read_conf("fltk/fltkrc");
#endif
    }
    chdir(s);
  }
}
