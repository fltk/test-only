#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/conf.h>
#include <config.h>
#ifdef WIN32
#include <windows.h>
#endif

const char* Fl::_style = "default";

// initialization to zero not necessary, compiler should do it.

#if (defined(WIN32) && !defined(FL_SGI_STYLE)) || defined(FL_WINDOWS_STYLE)
Fl_Widget_Style Fl::_widget_style = FL_WINDOWS_STYLE;
#else
Fl_Widget_Style Fl::_widget_style = FL_SGI_STYLE;
#endif

// styles loaded
int Fl::s_menu_item = 0;
int Fl::s_browser = 0;
int Fl::s_button = 0;
int Fl::s_chart = 0;
int Fl::s_light_button = 0;
int Fl::s_check_button = 0;
int Fl::s_choice = 0;
int Fl::s_menu_ = 0;
int Fl::s_menu_bar = 0;
int Fl::s_menu_button = 0;
int Fl::s_widget = 0;
int Fl::s_window = 0;
int Fl::s_slider = 0;
int Fl::s_return_button = 0;
int Fl::s_round_button = 0;
int Fl::s_fly_button = 0;
int Fl::s_scrollbar = 0;
int Fl::s_input = 0;
int Fl::s_output = 0;
int Fl::s_adjuster = 0;
int Fl::s_counter = 0;
int Fl::s_roller = 0;
int Fl::s_tooltip = 0;
int Fl::s_background = 0;
int Fl::s_mi_button = 0;

void Fl::load_styles(int b) {
  s_menu_item =
  s_browser =
  s_button =
  s_chart =
  s_light_button =
  s_check_button =
  s_choice =
  s_menu_ =
  s_menu_bar =
  s_menu_button =
  s_widget =
  s_window =
  s_slider =
  s_return_button =
  s_round_button =
  s_fly_button =
  s_scrollbar =
  s_input =
  s_output =
  s_adjuster =
  s_counter =
  s_roller =
  s_tooltip =
  s_background =
  s_mi_button =
  b ? 0 : 1;
  
  if (b) Fl::redraw();
}

void Fl::style(const char *s) {
  _style = s;

  if (s) load_styles(1);
  else load_styles(0);
}

const char *Fl::style() {
  static char temp[80];

  if (_style && !strcasecmp(_style, "default") &&
      !find("default style", temp, sizeof(temp)))
    return temp;

  return _style;
}

// load a bunch of attributes
void Fl::load_attributes(const char *section, uchar array[], Attribute *attrib) {
  char key[256];

  for (; attrib->key; attrib++) {
    snprintf(key, sizeof(key), "%s/%s", section, attrib->key);
    find(key, array[attrib->which], 1);
  }
}

// get the string value of a key from the global or user's config file
int Fl::find(const char *key, char *value, int value_length, int sf) {
  int  r;
  char cf[PATH_MAX];

  if (sf && !Fl::style()) return CONF_ERR_ARGUMENT;

#ifndef WIN32
  char *cptr = getenv("HOME");

  if (sf)
    snprintf(cf, sizeof(cf), "%s/.fltk/styles/%s", cptr ? cptr : "", Fl::style());
  else
    snprintf(cf, sizeof(cf), "%s/.fltk/config", cptr ? cptr : "");

  r = getconf(cf, key, value, value_length);
  if (!r) return r;

  if (sf)
    snprintf(cf, sizeof(cf), "/etc/fltk/styles/%s", Fl::style());
  else
    strcpy(cf, "/etc/fltk/config");
#else
  char windir[PATH_MAX];
  GetWindowsDirectoryA(windir, sizeof(windir));
  if (sf)
    snprintf(cf, sizeof(cf), "%s\\fltk\\styles\\%s", windir, Fl::style());
  else
    snprintf(cf, sizeof(cf), "%s\\fltk\\config", windir);
#endif
  r = getconf(cf, key, value, value_length);
  return r;
}

// get the uchar value of a key from the global or user's config file
int Fl::find(const char *key, uchar &ucvalue, int sf) {
  int  r;
  char buf[80];

  r = find(key, buf, sizeof(buf), sf);

  if (!r) ucvalue = (uchar)atoi(buf);

  return r;
}

// get the int value of a key from the global or user's config file
int Fl::find(const char *key, int &ivalue, int sf) {
  int  r;
  char buf[80];

  r = find(key, buf, sizeof(buf), sf);

  if (!r) ivalue = atoi(buf);

  return r;
}

/* CET - These methods are not used currently

// get the long value of a key from the global or user's config file
int Fl::find(const char *key, long &lvalue, int sf) {
  int  r;
  char buf[80];
  
  r = find(key, buf, sizeof(buf), sf);
 
  if (!r) lvalue = atol(buf);
 
  return r;
}

// get the boolean value of a key from the global or user's config file
int Fl::find_boolean(const char *key, int &bvalue, int sf)
{
  int  r;
  char svalue[80];
  
  r = find(key, svalue, sizeof(svalue), sf);
  if (r) return r;

  if (!strcasecmp(svalue, "ON") ||
      !strcasecmp(svalue, "YES") ||
      !strcasecmp(svalue, "Y") ||
      !strcmp(svalue, "1") ||
      !strcasecmp(svalue, "TRUE") ||
      !strcasecmp(svalue, "T"))
  {
    bvalue = 1;
  }
  else
  if (!strcasecmp(svalue, "OFF") ||
      !strcasecmp(svalue, "NO") ||
      !strcasecmp(svalue, "N") ||
      !strcmp(svalue, "0") ||
      !strcasecmp(svalue, "FALSE") ||
      !strcasecmp(svalue, "F"))
  {
    bvalue = 0;
  }
  else
  {
    return CONF_ERR_NOVALUE;
  }

  return 0;
}

*/
