//
// "$Id: Fl_Style.cxx,v 1.50 2003/11/11 07:36:31 spitzak Exp $"
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
  s->parent_		= 0;	// this is the topmost style always
  s->box_		= DOWN_BOX;
  s->buttonbox_		= UP_BOX;
  s->focusbox_		= DOTTED_FRAME;
  s->glyph_		= Widget::default_glyph;
  s->labelfont_		= HELVETICA;
  s->textfont_		= HELVETICA;
  s->labeltype_		= NORMAL_LABEL;
  s->color_		= WHITE; // GRAY99?
  s->textcolor_		= BLACK;
  s->selection_color_	= WINDOWS_BLUE;
  s->selection_textcolor_= WHITE;
  s->buttoncolor_	= GRAY75;
  s->labelcolor_	= BLACK;
  s->highlight_color_	= NO_COLOR;
  s->highlight_textcolor_= NO_COLOR;
  s->labelsize_		= 12;
  s->textsize_		= 12;
  s->leading_		= 2;
  s->scrollbar_width_	= 15;
  s->scrollbar_align_	= ALIGN_RIGHT|ALIGN_BOTTOM;
}

static NamedStyle default_named_style("default", ::revert, &Widget::default_style);
NamedStyle* Widget::default_style = &default_named_style;

/*! Copy the Style from another widget. Copying a style pointer from
  another widget is not safe if that
  style is dynamic() because it may change or be deleted.  This makes
  another dynamic() copy if necessary. For non-dynamic styles the
  pointer is copied.
*/
bool Widget::copy_style(const Style* t) {
  if (style_ == t) return false;
  if (style_ && style_->dynamic()) delete (Style*)style_;
  if (!t->dynamic()) {style_ = t; return false;}
  Style* newstyle = new Style;
  newstyle->parent_ = (Style*)t;
  style_ = newstyle;
  return true;
}

// Widgets set their own attributes by (possibly) creating a unique copy
// of their current style and setting it there.  Because this copy does
// not have any children the recursive search is not needed:

static Style* unique_style(const Style* & pointer) {
  if (pointer->dynamic()) return (Style*)pointer;
  Style* newstyle = new Style;
  newstyle->parent_ = (Style*)pointer;
  pointer = newstyle;
  return newstyle;
}

// Retrieve/set values from a style, using parent's value if not in child:

#define style_functions(TYPE,FIELD)	\
TYPE Widget::FIELD() const {return style()->FIELD();} \
TYPE Style::FIELD() const {		\
  for (const Style* s = this;;) {	\
    if (s->FIELD##_) return s->FIELD##_;\
    s = s->parent_;			\
    if (!s) return 0;			\
  }					\
}					\
void Widget::FIELD(TYPE v) {		\
  unique_style(style_)->FIELD##_ = v;	\
}

style_functions(Symbol*,	box		)
style_functions(Symbol*,	buttonbox	)
style_functions(Symbol*,	focusbox	)
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
style_functions(Color,		highlight_textcolor)
style_functions(float,		labelsize	)
style_functions(float,		textsize	)
style_functions(float,		leading		)
style_functions(unsigned char,	scrollbar_align	)
style_functions(unsigned char,	scrollbar_width	)

#if 0
/** Color to draw text atop the selection_color.

    This searches the styles for the first one that has selection_textcolor,
    textcolor, or selection_color set. If selection_textcolor is set it is
    returned.
    Otherwise if textcolor() is considered to "contrast" with
    selection_color() then return that.
    Otherwise return white or black, chosen to contrast with
    selection_color().
*/
Color Widget::selection_textcolor() const
{
  for (const Style* s = style_;;) {
    if (s->selection_textcolor) return s->selection_textcolor;
    if (s->textcolor) return contrast(s->textcolor, selection_color());
    if (s->selection_color) return contrast(textcolor(), s->selection_color);
    s = s->parent;
    if (!s) return 0;
  }
}
/** Set the value returned selection_textcolor(). This will create a
    new style() if necessary and set the field there.
*/
void Widget::selection_textcolor(Color v) {
  unique_style(style_)->selection_textcolor = v;
}

/** Color to draw text atop the highlight_color.

    This searches the styles for the first one that has highlight_textcolor
    or highlight_color set. If hightlight_textcolor is set it is returned.
    Otherwise if textcolor() is considered to "contrast" with
    highlight_color() then return that.
    Otherwise return white or black, chosen to contrast with
    highlight_color().
*/
Color Widget::highlight_textcolor() const
{
  for (const Style* s = style_;;) {
    if (s->highlight_textcolor) return s->highlight_textcolor;
    if (s->highlight_color) return contrast(textcolor(), s->highlight_color);
    s = s->parent;
    if (!s) return textcolor();
  }
}
/** Set the value returned highlight_textcolor(). This will create a
    new style() if necessary and set the field there.
*/
void Widget::highlight_textcolor(Color v) {
  unique_style(style_)->highlight_textcolor = v;
}
#endif

/** Calculate the colors to draw a box and labels inside that box.

    This is the standard function used by all the built-in fltk
    Symbols and boxes to select the colors to draw. It provides a
    somewhat Windows-like coloring scheme. The calling Widget
    picks what flags to pass to the Symbols so that when they
    call this they get the correct colors for each part of the
    widget.

    Flags that are understood:

    HIGHLIGHT: if highlight_color() is non-zero, set bg to
    highlight_color() and fg to highlight_textcolor().

    SELECTED: if selection_color() is non-zero, set bg to
    selection_color() and fg to selection_textcolor().

    OUTPUT: Set bg to color(), fg to textcolor(). If false bg is set
    to buttoncolor() and fg to labelcolor().  Widgets set this true
    for their main box, and false for any buttons inside themselves.

    INACTIVE: Change the fg to a gray color.

    It then further modifies fg so that it contrasts with the bg.

    Return value is \a flags with unused flags turned off. Currently
    HIGHLIGHT is turned off if not usable due to a missing highlight_color().  */
Flags Style::boxcolors(Flags flags, Color& bg, Color& fg) const {
  // this is not correct! It should search the styles in order and
  // decide what to do as it searches. For instance highligh_textcolor
  // should only be used if set before or at the highlight_color style.
  if ((flags & HIGHLIGHT) && (bg = highlight_color())) {
    fg = contrast(highlight_textcolor(), bg);
  } else {
    flags &= ~HIGHLIGHT;
    if ((flags & SELECTED) && (bg = selection_color())) {
      fg = contrast(selection_textcolor(), bg);
    } else {
      flags &= ~SELECTED;
      if (flags & OUTPUT) {bg = color(); fg = textcolor();}
      else {bg = buttoncolor(); fg = labelcolor();}
    }
  }
  if (flags & INACTIVE) fg = inactive(fg);
  return flags;
}

/*! \class fltk::NamedStyle

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
  is called.  */

static void plainrevert(Style*) {}

NamedStyle::NamedStyle(const char* n, void (*revert)(Style*), NamedStyle** pds) {
  memset((void*)this, 0, sizeof(*this));
  parent_ = Widget::default_style; // revert may want to change this
  if (revert) { revertfunc = revert; revert(this); }
  else revertfunc = plainrevert;
  next = NamedStyle::first;
  NamedStyle::first = this;
  back_pointer = pds;
  name = n;
}

/*! This constructor is used to create dynamic() styles for widgets that
  change their own attributes. */
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

bool Style::hide_shortcut_ = true;
bool Style::draw_boxes_inactive_ = true;
int Style::wheel_scroll_lines_ = 3;

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
  Style::draw_boxes_inactive_ = 1;
  for (NamedStyle* p = NamedStyle::first; p; p = p->next) {
    if (p->name) {
      Style temp = *p;
      memset((void*)p, 0, sizeof(Style));
      p->parent_ = temp.parent_;
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
// End of "$Id: Fl_Style.cxx,v 1.50 2003/11/11 07:36:31 spitzak Exp $".
//
