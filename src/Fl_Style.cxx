//
// "$Id: Fl_Style.cxx,v 1.47 2003/08/03 16:55:13 spitzak Exp $"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/Widget.h>
#include <fltk/string.h>
#include <fltk/math.h>
#include <fltk/run.h>
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
  s->textcolor		= BLACK;
  s->selection_color	= WINDOWS_BLUE;
  s->selection_textcolor= WHITE;
  s->buttoncolor	= GRAY75;
  s->labelcolor		= BLACK;
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
style_functions(Color,		textcolor	)
style_functions(Color,		selection_color	)
style_functions(Color,		selection_textcolor)
style_functions(Color,		buttoncolor	)
style_functions(Color,		labelcolor	)
style_functions(Color,		highlight_color	)
style_functions(Color,		highlight_labelcolor)
style_functions(float,		labelsize	)
style_functions(float,		textsize	)
style_functions(float,		leading		)
style_functions(unsigned char,	scrollbar_align	)
style_functions(unsigned char,	scrollbar_width	)

/*! \class NamedStyle

  Typically a widget class will define a single NamedStyle that is
  used by all instances of that widget. A "theme" can locate this
  structure by looking it up by name (using the find() method) and
  then change it to change the appearance of all widgets of
  that class.

  The reason a string name is used, rather than making the style be
  a public static data member of the class, is so that a theme can
  modify a large number of types of widgets without having them
  all linked into a program. If find() returns null it should just skip
  that setting code because that widget is not used by this program.

  The "revert" function is mostly provided to make it easy to initialize
  the fields even though C++ does not allow a structure constant.
  It is also used to undo theme changes when fltk::reload_theme()
  is called.
*/

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

bool Style::hide_shortcut = true;
bool Style::draw_boxes_inactive = true;
int Style::wheel_scroll_lines = 3;

////////////////////////////////////////////////////////////////
// Themes:

extern "C" FL_API bool fltk_theme();

/*! \fn fltk::theme(Theme)
  Set a function that sets all the NamedStyle structures to the
  correct values for the appearance selected by the user and
  operating system. This function is called by fltk before the
  first window is shown, and also whenever it receives a signal
  from the operating system that the user's preferences have
  changed.

  The return value is ignored but you should return true for future
  compatability.

  The default version points at a function that calls reset_theme()
  and then system_theme().

  This pointer is declared as a "C" function to make it easier to load
  the correct function by name from a plugin, if you would like to
  write a scheme where the appearance is controlled by plugins.  Fltk
  provides a convienence function to portably load plugins called
  fltk::load_plugin() that you may want to use if you are writing such
  a system.

  You can also statically override this at compile time. The default
  value is a function called fltk_theme() (not in the fltk namespace).
  You may be able to link your program so your own fltk_theme()
  replaces this. For instance building the fltk library with the
  KDE.cxx file will make it always load KDE color and font settings.
*/

Theme fltk::theme_ = fltk_theme;

Color fl_bg_switch = 0; // set by -bg in arg.cxx

static char theme_loaded;

/*! Force the theme to be loaded. If this has already been called, it
  does nothing (call reload_theme() if you want to make a change to
  theme() take effect). Normally fltk calls this just before the first
  Window::show() is done. You can call this earlier to make sure all
  the styles are correct, for instance if you want to measure the size
  of labels, which depends on the font selected.
*/
void fltk::load_theme() {
  if (theme_loaded) return;
  theme_loaded = 2; // signal reset_theme to do nothing
  theme_();
  if (fl_bg_switch) set_background(fl_bg_switch);
  theme_loaded = 1;
}

/*! If load_theme() has been called, this will call the theme()
  function again and then call redraw(). If the theme function is
  written correctly, this should change the display to the new theme.
  You should call this if you change the theme() or if external
  information changes such that the result of your theme() function
  changes.

  If load_theme() has \e not been called, this does nothing. This
  means you can call it before the first window is displayed.

  On Windows this is automatically called when a message comes from
  the OS indicating the user's preferences have changed. The KDE.cxx
  code also sets things up so KDE change messages cause this to be
  called.
*/
void fltk::reload_theme() {
  if (theme_loaded != 1) return;
  theme_();
  if (fl_bg_switch) set_background(fl_bg_switch);
  fltk::redraw();
}

/*! Change the theme to the compiled-in default by calling the reset
  function for all NamedStyle structures. If your theme() function
  modifies a large or varied set of settings it may be easiest to
  call this first to clear the previous settings.
*/
bool fltk::reset_theme() {
  // Don't waste time if the themes are just starting up:
  if (theme_loaded==2) return false;
  // revert to compiled defaults:
  // set_background((Color)0xc0c0c000); // not necessary?
  Style::draw_boxes_inactive = 1;
  for (NamedStyle* p = NamedStyle::first; p; p = p->next) {
    if (p->name) {
      Style temp = *p;
      memset((void*)p, 0, sizeof(Style));
      p->parent = temp.parent;
      p->revertfunc = temp.revertfunc;
      p->revertfunc(p);
    }
  }
  return true;
}

/*! \fn fltk::system_theme()
  Modify the current styles according to standard information read
  from the operating system. On Windows this will match the settings
  in the appearance tab of the display preferences. On X this will
  read some color settings from the xrdb database.
*/

///////////////////////////////////////////////////////////////

// GRAY is replaced with the passed color. For intermediate colors
// the gray ramp is replaced with a gamma curve that passes through that
// color. If the color is too dark or light then the gray ramp is left
// at a straight line and GRAY is not between the two adjacent entries.

void fltk::set_background(Color c) {
  uchar r, g, b;
  split_color( c, r, g, b );
  double powr;
  if (r < 0x30 || r > 0xf0) powr = 1;
  else powr = log(r/255.0)/log((GRAY75-GRAY00)/float(GRAY99-GRAY00));
  double powg;
  if (g < 0x30 || g > 0xf0) powg = 1;
  else powg = log(g/255.0)/log((GRAY75-GRAY00)/float(GRAY99-GRAY00));
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
// End of "$Id: Fl_Style.cxx,v 1.47 2003/08/03 16:55:13 spitzak Exp $".
//
