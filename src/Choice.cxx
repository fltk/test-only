// "$Id$"
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

/*! \class fltk::Choice

Subclass of fltk::Menu that provides a button that pops up the menu, and
also displays the text of the most-recently selected menu item.

\image html choice.gif

The appearance is designed to look like an "uneditable ComboBox" in
Windows, but it is somewhat different in that it does not contain a
text editor, also the menu pops up with the current item under the
cursor, which is immensely easier to use once you get used to it. This
is the same UI as the Macintosh and Motif, which called this an
OptionButton.

The user can change the value by popping up the menu by clicking
anywhere in the widget and moving the cursor to a different item, or
by typing up and down arrow keys to cycle amoung the items.  Typing
the fltk::Widget::shortcut() of any of the
items will also change the value to that item.

If you set a shortcut() on this widget itself or put &x in the label,
that shortcut will pop up the menu. The user can then use arrow keys
or the mouse to change the selected item.

When the user changes the value() the callback is done.

If you wish to display text that is different than any of the menu
items, you may instead want an fltk::PopupMenu. It works identically
but instead displays an empty box with the label() inside it, you
can then change the label() as needed.

If you want a "real" ComboBox where the user edits the text, this is
a planned addition to the fltk::Input widget. All text input will have
menus of possible replacements and completions. Not yet implemented,
unfortunately.

*/

#include <fltk/Choice.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/Item.h>
#include <fltk/draw.h>
using namespace fltk;

// The dimensions for the glyph in this and the PopupMenu are exactly
// the same, so that glyphs may be shared between them.

extern bool fl_hide_underscore;

/*! You can change the icon drawn on the right edge by setting glyph()
  to your own function that draws whatever you want.
*/
void Choice::draw() {
  if (damage() & DAMAGE_ALL) draw_frame();
  Rectangle r(w(),h()); box()->inset(r);
  int w1 = r.h()*4/5;
  r.move_r(-w1);
  // draw the little mark at the right:
  if (damage() & (DAMAGE_ALL|DAMAGE_HIGHLIGHT)) {
    drawstyle(style(), flags() & ~FOCUSED | OUTPUT);
    Rectangle gr(r.r(), r.y(), w1, r.h());
    draw_glyph(GLYPH_DOWN_BUTTON, gr);
  }
  if (damage() & (DAMAGE_ALL|DAMAGE_VALUE)) {
    setcolor(color());
    fillrect(r);
    if (flags() & FOCUSED) {
      setcolor(selection_color());
      Rectangle fr(r); fr.inset(2); fillrect(fr);
    }
    Widget* o = get_item();
    //if (!o && children()) o = child(0);
    if (o) {
      Item::set_style(this);
      Flags saved = o->flags();
      if (focused()) o->set_flag(SELECTED);
      else o->clear_flag(SELECTED);
      if (flags()&INACTIVE) o->set_flag(NOTACTIVE|INACTIVE);
      r.move_x(2);
      push_clip(r);
      push_matrix();
      translate(r.x(), r.y()+((r.h()-o->height())>>1));
      int save_w = o->w(); o->w(r.r());
      fl_hide_underscore = true;
      o->draw();
      fl_hide_underscore = false;
      Item::clear_style();
      o->w(save_w);
      o->flags(saved);
      pop_matrix();
      pop_clip();
    }
  }
}

static bool try_item(Choice* choice, int i) {
  Widget* w = choice->child(i);
  if (!w->takesevents()) return false;
  choice->value(i);
  choice->execute(w);
  return true;
}

int Choice::handle(int e) {
  int children = this->children(0,0);
  if (!children) return 0;
  switch (e) {

  case FOCUS:
  case UNFOCUS:
    redraw(DAMAGE_VALUE);
    return 1;

  case ENTER:
  case LEAVE:
    redraw_highlight();
  case MOVE:
    return 1;

  case PUSH:
    // Normally a mouse click pops up the menu. If you uncomment this line
    // (or make a subclass that does this), a mouse click will re-pick the
    // current item (it will popup the menu and immediately dismiss it).
    // Depending on the size and usage of the menu this may be more
    // user-friendly.
//  event_is_click(0);
    if (click_to_focus()) take_focus();
  EXECUTE:
    if (popup(0, 0, w(), h(), 0)) redraw(DAMAGE_VALUE);
    return 1;

  case SHORTCUT:
    if (test_shortcut()) goto EXECUTE;
    if (handle_shortcut()) {redraw(DAMAGE_VALUE); return 1;}
    return 0;

  case KEY:
    switch (event_key()) {

    case ReturnKey:
    case SpaceKey:
      goto EXECUTE;

    case UpKey: {
      int i = value(); if (i < 0) i = children;
      while (i > 0) {--i; if (try_item(this, i)) return 1;}
      return 1;}
    case DownKey: {
      int i = value();
      while (++i < children) if (try_item(this,i)) return 1;
      return 1;}
    }
    return 0;

  default:
    return 0;
  }
}

#define MOTIF_STYLE 0
#define MAC_STYLE 0

#if MOTIF_STYLE
// Glyph erases the area and draws a long, narrow box:
static void glyph(int, const Rectangle& r, const Style* s, Flags)
{
  color(s->buttoncolor());
  fillrect(r);
  // draw a long narrow box:
  Rectangle r1(r, r.w()-2, r.h()/3, ALIGN_LEFT);
  Widget::default_glyph(0, r1, s, 0);
}
#endif

#if MAC_STYLE
// Attempt to draw an up/down arrow like the Mac uses, since the
// popup menu is more like how the Mac works:
static void glyph(int, const Rectangle& r, const Style* s, Flags flags)
{
  Widget::default_glyph(0, r, s, flags);
  Rectangle r1(r);
  r1.h(r1.h()/2);
  r1.move_y(r1.h()/3);
  Widget::default_glyph(GLYPH_UP, r1, s, flags);
  r1.move(0,r1.h());
  Widget::default_glyph(GLYPH_DOWN, r1, s, flags);
}
#endif

static void revert(Style* s) {
#if MOTIF_STYLE
  s->color_ = GRAY75;
  s->box_ = s->buttonbox_ = Widget::default_style->buttonbox_;
  s->glyph_ = ::glyph;
#endif
#if MAC_STYLE
  s->glyph_ = ::glyph;
#endif
}
static NamedStyle style("Choice", revert, &Choice::default_style);
NamedStyle* Choice::default_style = &::style;

/*! The constructor makes the menu empty. See Menu and StringList
  for information on how to set the menu to a list of items.
*/
Choice::Choice(int x,int y,int w,int h, const char *l) : Menu(x,y,w,h,l) {
  value(0);
  // copy the leading from Menu:
  if (!default_style->leading_) default_style->leading_ = style()->leading_;
  style(default_style);
  clear_flag(ALIGN_MASK);
  set_flag(ALIGN_LEFT);
  set_click_to_focus();
  when(WHEN_RELEASE);
}

//
// End of "$Id$".
//
