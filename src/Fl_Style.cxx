//
// "$Id: Fl_Style.cxx,v 1.33 2002/02/10 22:57:48 spitzak Exp $"
//
// Code for managing Fl_Style structures.
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

#include <fltk/Fl.h>
#include <fltk/Fl_Widget.h>
#include <fltk/fl_load_plugin.h>
#include <fltk/vsnprintf.h>
#include <string.h>
#include <stdlib.h>
#include <config.h>
#if defined(_WIN32)
# include <io.h>
# define access(a,b) _access(a,b)
# define R_OK 4
#else
# include <unistd.h>
#endif

Fl_Named_Style* Fl_Named_Style::first;

// Do not change the contents of this ever.  The themes depend on getting
// a known state initially.
static void revert(Fl_Style* s) {
  s->box                   = FL_DOWN_BOX;
  s->button_box		   = FL_UP_BOX;
  s->glyph                 = Fl_Widget::default_glyph;
  s->label_font            = FL_HELVETICA;
  s->text_font             = FL_HELVETICA;
  s->label_type            = FL_NORMAL_LABEL;
  s->color                 = fl_gray_ramp(FL_NUM_GRAY-1);
  s->button_color	   = FL_GRAY;
  s->label_color           = FL_BLACK;
  s->selection_color	   = FL_BLUE_SELECTION_COLOR;
  s->selection_text_color  = FL_WHITE;
  s->highlight_color       = FL_NO_COLOR;
  s->highlight_label_color = FL_NO_COLOR;
  s->text_color            = FL_BLACK;
  s->label_size		   = 12;
  s->text_size             = 12;
  s->leading		   = 0;
  s->parent                = 0;	// this is the topmost style always
}

static Fl_Named_Style default_named_style("default", ::revert, &Fl_Widget::default_style);
Fl_Named_Style* Fl_Widget::default_style = &default_named_style;

// Copying a style pointer from another widget is not safe if that
// style is dynamic() because it may change or be deleted.  This makes
// another dynamic() copy if necessary.

bool Fl_Widget::copy_style(const Fl_Style* t) {
  if (style_ == t) return false;
  if (style_ && style_->dynamic()) delete (Fl_Style*)style_;
  if (!t->dynamic()) {style_ = t; return false;}
  Fl_Style* newstyle = new Fl_Style;
  newstyle->parent = (Fl_Style*)t;
  style_ = newstyle;
  return true;
}

// Widgets set their own attributes by (possibly) creating a unique copy
// of their current style and setting it there.  Because this copy does
// not have any children the recursive search is not needed:

Fl_Style* fl_unique_style(const Fl_Style* & pointer) {
  if (pointer->dynamic()) return (Fl_Style*)pointer;
  Fl_Style* newstyle = new Fl_Style;
  newstyle->parent = (Fl_Style*)pointer;
  pointer = newstyle;
  return newstyle;
}

// Retrieve/set values from a style, using parent's value if not in child:

#define style_functions(TYPE,FIELD)	\
TYPE Fl_Widget::FIELD() const {		\
  for (const Fl_Style* s = style_;;) {	\
    if (s->FIELD) return s->FIELD;	\
    s = s->parent;			\
    if (!s) return 0;			\
  }					\
}					\
void Fl_Widget::FIELD(TYPE v) {		\
  fl_unique_style(style_)->FIELD = v;	\
}

style_functions(Fl_Boxtype,box)
style_functions(Fl_Boxtype,button_box)
style_functions(Fl_Glyph,glyph)
style_functions(Fl_Font,label_font)
style_functions(Fl_Font,text_font)
style_functions(Fl_Labeltype,label_type)
style_functions(Fl_Color,color)
style_functions(Fl_Color,button_color)
style_functions(Fl_Color,label_color)
style_functions(Fl_Color,selection_color)
style_functions(Fl_Color,selection_text_color)
style_functions(Fl_Color,highlight_color)
style_functions(Fl_Color,highlight_label_color)
style_functions(Fl_Color,text_color)
style_functions(unsigned,label_size)
style_functions(unsigned,text_size)
style_functions(unsigned,leading)

// Named styles provide a list that can be searched by theme plugins.
// The "revert" function is mostly provided to make it easy to initialize
// the fields even though C++ does not allow a structure constant.
// It is also used to undo theme changes.

static void plainrevert(Fl_Style*) {}

Fl_Named_Style::Fl_Named_Style(const char* n, void (*revert)(Fl_Style*), Fl_Named_Style** pds) {
  memset((void*)this, 0, sizeof(*this));
  parent = Fl_Widget::default_style; // revert may want to change this
  if (revert) { revertfunc = revert; revert(this); }
  else revertfunc = plainrevert;
  next = Fl_Named_Style::first;
  Fl_Named_Style::first = this;
  back_pointer = pds;
  name = n;
}

// This constructor is used to create dynamic() styles for widgets that
// change their own attributes:
Fl_Style::Fl_Style() {
  memset((void*)this, 0, sizeof(*this));
}

#include <ctype.h>
Fl_Style* Fl_Style::find(const char* name) {
  for (Fl_Named_Style* p = Fl_Named_Style::first; p; p = p->next) {
    const char* a = p->name;
    const char* b = name;
    if (!a) continue;
    for (;;) {
      if (*a == '_') {
	if (*b == ' ' || *b == '_');
	else {a++; continue;}
      } else if (tolower(*a) != tolower(*b)) break;
      if (!*a && !*b) return p;
      a++;
      b++;
    }
  }
  return 0;
}

bool Fl_Style::draw_boxes_inactive = true;
unsigned Fl_Style::scrollbar_width = 15;
Fl_Flags Fl_Style::scrollbar_align = FL_ALIGN_RIGHT|FL_ALIGN_BOTTOM;
int Fl_Style::wheel_scroll_lines = 3;

////////////////////////////////////////////////////////////////
// Themes:

void Fl_Style::revert() {
  // fl_background((Fl_Color)0xc0c0c000); // not necessary?
  draw_boxes_inactive = 1;
  scrollbar_width = 15;
  scrollbar_align = FL_ALIGN_RIGHT|FL_ALIGN_BOTTOM;
  for (Fl_Named_Style* p = Fl_Named_Style::first; p; p = p->next) {
    if (p->name) {
      Fl_Style temp = *p;
      memset((void*)p, 0, sizeof(Fl_Style));
      p->parent = temp.parent;
      p->revertfunc = temp.revertfunc;
      p->revertfunc(p);
    }
  }
  Fl::redraw();
}

Fl_Color fl_bg_switch = 0; // set by -bg in Fl_arg.cxx

static bool theme_loaded;

// Force the theme to be reloaded, if it already has been loaded.
// This does nothing if load_theme() has not been called before (when
// load_theme() is called this same actions will be done). Otherwise
// this will call revert() on the styles and run the current theme
// procedure.
void Fl_Style::reload_theme() {
  if (theme_loaded) {
    theme_loaded = false;
    revert();
    load_theme();
  }
}

// Force the theme to be loaded. This does nothing if this has already
// been called. Normally this is called when the first window is shown()
// but you can call this if you want your program to be able to use any
// settings the theme made.
void Fl_Style::load_theme() {
  if (theme_loaded) return;
  theme_loaded = true;
  if (!theme_) theme_ = load_theme("default");
  theme_();
  if (fl_bg_switch) fl_background(fl_bg_switch);
}

Fl_Theme Fl_Style::theme_;
const char* Fl_Style::scheme_;

////////////////////////////////////////////////////////////////
// Theme plugin finder & loader

#ifndef FL_SHARED

Fl_Theme Fl_Style::load_theme(const char* name) {
  // no name leaves the built-in default:
  if (!name || !*name) return fltk_theme;
  // "default" works:
  if (!strcmp(name, "default")) return fltk_theme;
  // otherwise we can't do anything
  fprintf(stderr, "%s : FLTK linked statically, cannot load plugins\n", name);
  return 0;
}

#else

#ifndef PATH_MAX
# define PATH_MAX 256
#endif

Fl_Theme Fl_Style::load_theme(const char* name) {
  // no name leaves the built-in default:
  if (!name || !*name) return fltk_theme;

  // add ".theme" if it is not there:
  char name_buf[PATH_MAX];
  int n = strlen(name);
  if (n < 6 || strcasecmp(name+n-6, ".theme")) {
    snprintf(name_buf, PATH_MAX, "%s.theme", name);
    name = name_buf;
  }

  // search for the file:
  char path_buf[PATH_MAX];
  const char *path = fl_find_config_file(path_buf, PATH_MAX, name);

  if (!path) {
    // If they said "default" it is ok if the plugin is not found:
    if (!strncmp(name, "default.", 8)) return fltk_theme;
    return 0;
  }

  return (Fl_Theme)fl_load_plugin(path, "fltk_theme");
}

const char* fl_find_config_file(char* path, int size, const char* name)
{
  // See if the user typed in an "absolute" path name
  if (name[0] == '/' || name[0] == '.'
#ifdef _WIN32
      || name[0] == '\\' || name[1] == ':'
#endif
      ) {
    return name;
  }
  // Look in the ~/.fltk directory:
  char *cptr = getenv("HOME");
  if (cptr) {
    snprintf(path, size, "%s/.fltk/%s", cptr, name);
    if (access(path, R_OK) == 0) return path;
  }
#ifdef _WIN32
  cptr = getenv("HOMEPATH");
  if (cptr) {
    snprintf(path, size, "%s/fltk/%s", cptr, name);
    if (access(path, R_OK) == 0) return path;
  }
  cptr = getenv("USERPROFILE");
  if (cptr) {
    snprintf(path,sizeof(path), "%s/Application Data/fltk/%s", cptr, name);
    if (access(path, R_OK) == 0) return path;
  }
#endif
  // try the default /usr/local/lib/fltk:
  snprintf(path, sizeof(path), CONFIGDIR "/%s", name);
  if (access(path, R_OK) == 0) return path;
  return 0;
}

#endif

///////////////////////////////////////////////////////////////

#include <fltk/math.h>

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
// End of "$Id: Fl_Style.cxx,v 1.33 2002/02/10 22:57:48 spitzak Exp $".
//
