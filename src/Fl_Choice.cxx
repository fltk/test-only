//
// "$Id: Fl_Choice.cxx,v 1.72 2003/02/02 10:39:23 spitzak Exp $"
//
// Choice widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Choice.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
using namespace fltk;

// The dimensions for the glyph in this and the PopupMenu are exactly
// the same, so that glyphs may be shared between them.

extern bool fl_hide_shortcut;
extern const Widget* fl_item_parent;

void Choice::draw() {
  int X=0; int Y=0; int W=w(); int H=h();
  box()->inset(X,Y,W,H);
  int w1 = H*4/5;
  if (damage() & DAMAGE_ALL) {
    draw_frame();
    // draw the little mark at the right:
    Flags flags = this->flags();
    if (!active_r())
      flags |= INACTIVE;
    else if (belowmouse())
      flags |= HIGHLIGHT;
    draw_glyph(GLYPH_DOWN_BUTTON, X+W-w1, Y, w1, H, flags);
  }
  setcolor(color());
  fillrect(X,Y,W-w1,H);
  if (focused()) {
    setcolor(selection_color());
    fillrect(X+2, Y+2, W-w1-4, H-4);
  }
  Widget* o = get_focus();
  //if (!o && children()) o = child(0);
  if (o) {
    const Widget* saved_parent = fl_item_parent;
    fl_item_parent = this;
    if (focused()) o->set_flag(SELECTED);
    else o->clear_flag(SELECTED);
    push_clip(X+2, Y, W-w1-2, H);
    push_matrix();
    translate(X, Y+((H-o->height())>>1));
    int save_w = o->w(); o->w(W-w1);
    fl_hide_shortcut = true;
    o->draw();
    fl_hide_shortcut = false;
    o->w(save_w);
    pop_matrix();
    pop_clip();
    fl_item_parent = saved_parent;
  }
}

int Choice::value(int v) {
  if (focus(&v, 0)) {redraw(DAMAGE_VALUE); return true;}
  return false;
}

#if 0
int Choice::focus(const int* indexes, int level) {
  // rather annoying kludge to try to detect if the item from an List
  // has changed by looking for the label and user data to change:
  Widget* save_item = item();
  const char* save_label = 0;
  void* save_data = 0;
  if (save_item) {
    save_label = save_item->label();
    save_data = save_item->user_data();
  }
  Menu::focus(indexes, level);
  if (item() == save_item) {
    if (!save_item) return 0;
    if (save_label == save_item->label() && save_data==save_item->user_data())
      return 0;
  }
  redraw();
  return 1;
}
#endif

static bool try_item(Choice* choice, int i) {
  Widget* w = choice->child(i);
  if (!w->takesevents()) return false;
  choice->value(i);
  choice->execute(w);
  choice->redraw(DAMAGE_VALUE);
  return true;
}  

int Choice::handle(int e) {
  int children = this->children(0,0);
  if (!children) return 0;
  switch (e) {

  case FOCUS:
  case UNFOCUS:
    redraw(DAMAGE_HIGHLIGHT);
    return 1;

  case ENTER:
  case LEAVE:
    if (highlight_color() && takesevents()) redraw(DAMAGE_HIGHLIGHT);
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

#if MOTIF_STYLE
// Glyph erases the area and draws a long, narrow box:
static void glyph(const Widget* widget, int,
		  int x,int y,int w,int h, Flags)
{
  color(widget->buttoncolor());
  fillrect(x,y,w,h);
  // draw a long narrow box:
  Widget::default_style->glyph(widget, 0, x, y+(h-h/3)/2, w-2, h/3, 0);
}
#endif

#define MAC_STYLE 0
#if MAC_STYLE
// Attempt to draw an up/down arrow like the Mac uses, since the
// popup menu is more like how the Mac works:
static void glyph(const Widget* widget, int,
		  int x,int y,int w,int h, Flags flags)
{
  Widget::default_style->glyph(widget, 0, x, y, w, h, flags);
  x += 2;
  w -= 4;
  y = y+h/2;
  h = (w+1)/2;
  Widget::default_style->glyph(widget, GLYPH_UP, x, y-h-1, w, h, flags);
  Widget::default_style->glyph(widget, GLYPH_DOWN, x, y+1, w, h, flags);
}
#endif

static void revert(Style* s) {
#if MOTIF_STYLE
  s->color = GRAY75;
  s->box = s->buttonbox = Widget::default_style->buttonbox;
  s->glyph = ::glyph;
#endif
#if MAC_STYLE
  s->glyph = ::glyph;
#endif
}
static NamedStyle style("Choice", revert, &Choice::default_style);
NamedStyle* Choice::default_style = &::style;

Choice::Choice(int x,int y,int w,int h, const char *l) : Menu(x,y,w,h,l) {
  value(0);
  style(default_style);
  clear_flag(ALIGN_MASK);
  set_flag(ALIGN_LEFT);
  set_click_to_focus();
  when(WHEN_RELEASE);
}

//
// End of "$Id: Fl_Choice.cxx,v 1.72 2003/02/02 10:39:23 spitzak Exp $".
//
