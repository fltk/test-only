//
// "$Id: Fl_Choice.cxx,v 1.66 2002/08/11 04:49:54 spitzak Exp $"
//
// Choice widget for the Fast Light Tool Kit (FLTK).
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
#include <fltk/Fl_Choice.h>
#include <fltk/fl_draw.h>

// The dimensions for the glyph in this and the Fl_Menu_Button are exactly
// the same, so that glyphs may be shared between them.

extern bool fl_hide_shortcut;

void Fl_Choice::draw() {
  int X=0; int Y=0; int W=w(); int H=h();
  box()->inset(X,Y,W,H);
  int w1 = H*4/5;
  if (damage() & FL_DAMAGE_ALL) {
    draw_frame();
    // draw the little mark at the right:
    Fl_Flags flags = this->flags();
    if (!active_r())
      flags |= FL_INACTIVE;
    else if (belowmouse())
      flags |= FL_HIGHLIGHT;
    draw_glyph(FL_GLYPH_DOWN_BUTTON, X+W-w1, Y, w1, H, flags);
  }
  fl_color(color());
  fl_rectf(X,Y,W-w1,H);
  if (focused()) {
    fl_color(selection_color());
    fl_rectf(X+2, Y+2, W-w1-4, H-4);
  }
  Fl_Widget* o = get_focus();
  if (!o) o = child(0);
  if (o) {
    if (focused()) o->set_flag(FL_SELECTED);
    else o->clear_flag(FL_SELECTED);
    fl_push_clip(X+2, Y, W-w1-2, H);
    fl_push_matrix();
    fl_translate(X, Y+(H-o->height())/2);
    int save_w = o->w(); o->w(W-w1);
    fl_hide_shortcut = true;
    o->draw();
    fl_hide_shortcut = false;
    o->w(save_w);
    fl_pop_matrix();
    fl_pop_clip();
  }
}

int Fl_Choice::value(int v) {
  if (focus(&v, 0)) {redraw(FL_DAMAGE_VALUE); return true;}
  return false;
}

#if 0
int Fl_Choice::focus(const int* indexes, int level) {
  // rather annoying kludge to try to detect if the item from an Fl_List
  // has changed by looking for the label and user data to change:
  Fl_Widget* save_item = item();
  const char* save_label = 0;
  void* save_data = 0;
  if (save_item) {
    save_label = save_item->label();
    save_data = save_item->user_data();
  }
  Fl_Menu_::focus(indexes, level);
  if (item() == save_item) {
    if (!save_item) return 0;
    if (save_label == save_item->label() && save_data==save_item->user_data())
      return 0;
  }
  redraw();
  return 1;
}
#endif

static bool try_item(Fl_Choice* choice, int i) {
  Fl_Widget* w = choice->child(i);
  if (!w->takesevents()) return false;
  choice->value(i);
  choice->execute(w);
  choice->redraw(FL_DAMAGE_VALUE);
  return true;
}  

int Fl_Choice::handle(int e) {
  int children = this->children(0,0);
  if (!children) return 0;
  switch (e) {

  case FL_FOCUS:
  case FL_UNFOCUS:
    redraw(FL_DAMAGE_HIGHLIGHT);
    return 1;

  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && takesevents()) redraw(FL_DAMAGE_HIGHLIGHT);
  case FL_MOVE:
    return 1;

  case FL_PUSH:
    // Normally a mouse click pops up the menu. If you uncomment this line
    // (or make a subclass that does this), a mouse click will re-pick the
    // current item (it will popup the menu and immediately dismiss it).
    // Depending on the size and usage of the menu this may be more
    // user-friendly.
//  Fl::event_is_click(0);
    if (click_to_focus()) take_focus();
  EXECUTE:
    if (popup(0, 0, w(), h(), 0)) redraw(FL_DAMAGE_VALUE);
    return 1;

  case FL_SHORTCUT:
    if (test_shortcut()) goto EXECUTE;
    if (handle_shortcut()) {redraw(FL_DAMAGE_VALUE); return 1;}
    return 0;

  case FL_KEY:
    switch (Fl::event_key()) {

    case FL_Enter:
    case ' ':
      goto EXECUTE;

    case FL_Up: {
      int i = value(); if (i < 0) i = children;
      while (i > 0) {--i; if (try_item(this, i)) return 1;}
      return 1;}
    case FL_Down: {
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
static void glyph(const Fl_Widget* widget, int,
		  int x,int y,int w,int h, Fl_Flags)
{
  fl_color(widget->button_color());
  fl_rectf(x,y,w,h);
  // draw a long narrow box:
  Fl_Widget::default_style->glyph(widget, 0, x, y+(h-h/3)/2, w-2, h/3, 0);
}
#endif

#define MAC_STYLE 0
#if MAC_STYLE
// Attempt to draw an up/down arrow like the Mac uses, since the
// popup menu is more like how the Mac works:
static void glyph(const Fl_Widget* widget, int,
		  int x,int y,int w,int h, Fl_Flags flags)
{
  Fl_Widget::default_style->glyph(widget, 0, x, y, w, h, flags);
  x += 2;
  w -= 4;
  y = y+h/2;
  h = (w+1)/2;
  Fl_Widget::default_style->glyph(widget, FL_GLYPH_UP, x, y-h-1, w, h, flags);
  Fl_Widget::default_style->glyph(widget, FL_GLYPH_DOWN, x, y+1, w, h, flags);
}
#endif

static void revert(Fl_Style* s) {
#if MOTIF_STYLE
  s->color = FL_GRAY;
  s->box = s->button_box = Fl_Widget::default_style->button_box;
  s->glyph = ::glyph;
#endif
#if MAC_STYLE
  s->glyph = ::glyph;
#endif
}
static Fl_Named_Style style("Choice", revert, &Fl_Choice::default_style);
Fl_Named_Style* Fl_Choice::default_style = &::style;

Fl_Choice::Fl_Choice(int x,int y,int w,int h, const char *l) : Fl_Menu_(x,y,w,h,l) {
  value(0);
  style(default_style);
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_LEFT);
  set_click_to_focus();
  when(FL_WHEN_RELEASE);
}

//
// End of "$Id: Fl_Choice.cxx,v 1.66 2002/08/11 04:49:54 spitzak Exp $".
//
