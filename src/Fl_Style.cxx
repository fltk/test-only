//
// "$Id: Fl_Style.cxx,v 1.39 2002/12/10 01:46:17 easysw Exp $"
//
// Code for managing Style structures.
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/Widget.h>
#include <fltk/load_plugin.h>
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
#include <ctype.h>
#include <fltk/math.h>
using namespace fltk;

NamedStyle* NamedStyle::first;

// Do not change the contents of this ever.  The themes depend on getting
// a known state initially.
static void revert(Style* s) {
  s->parent		= 0;	// this is the topmost style always
  s->box		= DOWN_BOX;
  s->buttonbox		= UP_BOX;
  s->focusbox		= DOTTED_FRAME;
  s->glyph		= Widget::default_glyph;
  s->labelfont		= HELVETICA;
  s->textfont		= HELVETICA;
  s->labeltype		= NORMAL_LABEL;
  s->color		= WHITE; // GRAY99?
  s->buttoncolor	= GRAY75;
  s->labelcolor		= BLACK;
  s->textcolor		= BLACK;
  s->selection_color	= WINDOWS_BLUE;
  s->selection_textcolor= WHITE;
  s->highlight_color	= NO_COLOR;
  s->highlight_labelcolor= NO_COLOR;
  s->labelsize		= 12;
  s->textsize		= 12;
  s->leading		= 2;
  s->scrollbar_width	= 15;
  s->scrollbar_align	= ALIGN_RIGHT|ALIGN_BOTTOM;
}

static NamedStyle default_named_style("default", ::revert, &Widget::default_style);
NamedStyle* Widget::default_style = &default_named_style;

// Copying a style pointer from another widget is not safe if that
// style is dynamic() because it may change or be deleted.  This makes
// another dynamic() copy if necessary.

bool Widget::copy_style(const Style* t) {
  if (style_ == t) return false;
  if (style_ && style_->dynamic()) delete (Style*)style_;
  if (!t->dynamic()) {style_ = t; return false;}
  Style* newstyle = new Style;
  newstyle->parent = (Style*)t;
  style_ = newstyle;
  return true;
}

// Widgets set their own attributes by (possibly) creating a unique copy
// of their current style and setting it there.  Because this copy does
// not have any children the recursive search is not needed:

Style* unique_style(const Style* & pointer) {
  if (pointer->dynamic()) return (Style*)pointer;
  Style* newstyle = new Style;
  newstyle->parent = (Style*)pointer;
  pointer = newstyle;
  return newstyle;
}

// Retrieve/set values from a style, using parent's value if not in child:

#define style_functions(TYPE,FIELD)	\
TYPE Widget::FIELD() const {		\
  for (const Style* s = style_;;) {	\
    if (s->FIELD) return s->FIELD;	\
    s = s->parent;			\
    if (!s) return 0;			\
  }					\
}					\
void Widget::FIELD(TYPE v) {		\
  unique_style(style_)->FIELD = v;	\
}

style_functions(Box*,		box		)
style_functions(Box*,		buttonbox	)
style_functions(Box*,		focusbox	)
style_functions(GlyphStyle,	glyph		)
style_functions(Font*,		labelfont	)
style_functions(Font*,		textfont	)
style_functions(LabelType*,	labeltype	)
style_functions(Color,		color		)
style_functions(Color,		buttoncolor	)
style_functions(Color,		labelcolor	)
style_functions(Color,		selection_color	)
style_functions(Color,		selection_textcolor)
style_functions(Color,		highlight_color	)
style_functions(Color,		highlight_labelcolor)
style_functions(Color,		textcolor	)
style_functions(float,		labelsize	)
style_functions(float,		textsize	)
style_functions(float,		leading		)
style_functions(unsigned char,	scrollbar_align	)
style_functions(unsigned char,	scrollbar_width	)

// Named styles provide a list that can be searched by theme plugins.
// The "revert" function is mostly provided to make it easy to initialize
// the fields even though C++ does not allow a structure constant.
// It is also used to undo theme changes.

static void plainrevert(Style*) {}

NamedStyle::NamedStyle(const char* n, void (*revert)(Style*), NamedStyle** pds) {
  memset((void*)this, 0, sizeof(*this));
  parent = Widget::default_style; // revert may want to change this
  if (revert) { revertfunc = revert; revert(this); }
  else revertfunc = plainrevert;
  next = NamedStyle::first;
  NamedStyle::first = this;
  back_pointer = pds;
  name = n;
}

// This constructor is used to create dynamic() styles for widgets that
// change their own attributes:
Style::Style() {
  memset((void*)this, 0, sizeof(*this));
}

Style* Style::find(const char* name) {
  for (NamedStyle* p = NamedStyle::first; p; p = p->next) {
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

bool Style::draw_boxes_inactive = true;
int Style::wheel_scroll_lines = 3;

////////////////////////////////////////////////////////////////
// Themes:

void Style::revert() {
  // set_background((Color)0xc0c0c000); // not necessary?
  draw_boxes_inactive = 1;
  for (NamedStyle* p = NamedStyle::first; p; p = p->next) {
    if (p->name) {
      Style temp = *p;
      memset((void*)p, 0, sizeof(Style));
      p->parent = temp.parent;
      p->revertfunc = temp.revertfunc;
      p->revertfunc(p);
    }
  }
  fltk::redraw();
}

Color fl_bg_switch = 0; // set by -bg in arg.cxx

static bool theme_loaded;

// Force the theme to be reloaded, if it already has been loaded.
// This does nothing if load_theme() has not been called before (when
// load_theme() is called this same actions will be done). Otherwise
// this will call revert() on the styles and run the current theme
// procedure.
void Style::reload_theme() {
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
void Style::load_theme() {
  if (theme_loaded) return;
  theme_loaded = true;
  if (!theme_) theme_ = load_theme("default");
  theme_();
  if (fl_bg_switch) set_background(fl_bg_switch);
}

Theme Style::theme_;
const char* Style::scheme_;

////////////////////////////////////////////////////////////////
// Theme plugin finder & loader

#ifndef SHARED

Theme Style::load_theme(const char* name) {
  // no name leaves the built-in default:
  if (!name || !*name) return fltk_theme;
  // "default" works:
  if (!strcmp(name, "default")) return fltk_theme;
  // otherwise we can't do anything
  fprintf(stderr, "%s : FLTK linked statically, cannot load plugins\n", name);
  return 0;
}

#else

#ifndef FILENAME_MAX
# define FILENAME_MAX 1024
#endif

Theme Style::load_theme(const char* name) {
  // no name leaves the built-in default:
  if (!name || !*name) return fltk_theme;

  // add ".theme" if it is not there:
  char name_buf[FILENAME_MAX];
  int n = strlen(name);
  if (n < 6 || strcasecmp(name+n-6, ".theme")) {
    snprintf(name_buf, FILENAME_MAX, "%s.theme", name);
    name = name_buf;
  }

  // search for the file:
  char path_buf[FILENAME_MAX];
  const char *path = find_config_file(path_buf, FILENAME_MAX, name);

  if (!path) {
    // If they said "default" it is ok if the plugin is not found:
    if (!strncmp(name, "default.", 8)) return fltk_theme;
    return 0;
  }

  return (Theme)load_plugin(path, "fltk_theme");
}

const char* find_config_file(char* path, int size, const char* name)
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

// GRAY is replaced with the passed color. For intermediate colors
// the gray ramp is replaced with a gamma curve that passes through that
// color. If the color is too dark or light then the gray ramp is left
// at a straight line and GRAY is not between the two adjacent entries.

void fltk::set_background(Color c) {
  int r = (c>>24)&255;
  double powr;
  if (r < 0x30 || r > 0xf0) powr = 1;
  else powr = log(r/255.0)/log((GRAY75-GRAY00)/float(GRAY99-GRAY00));
  int g = (c>>16)&255;
  double powg;
  if (g < 0x30 || g > 0xf0) powg = 1;
  else powg = log(g/255.0)/log((GRAY75-GRAY00)/float(GRAY99-GRAY00));
  int b = (c>>8)&255;
  double powb;
  if (b < 0x30 || b > 0xf0) powb = 1;
  else powb = log(b/255.0)/log((GRAY75-GRAY00)/float(GRAY99-GRAY00));
  for (int i = 0; i <= (GRAY99-GRAY00); i++) if (i != GRAY75) {
    double gray = i/float(GRAY99-GRAY00);
    set_color_index(Color(GRAY00+i),
		    color(uchar(pow(gray,powr)*255+.5),
			  uchar(pow(gray,powg)*255+.5),
			  uchar(pow(gray,powb)*255+.5)));
  }
  set_color_index(GRAY75, c);
}

//
// End of "$Id: Fl_Style.cxx,v 1.39 2002/12/10 01:46:17 easysw Exp $".
//
