//
// "$Id: Fl_Choice.cxx,v 1.39 2000/02/14 11:32:48 bill Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/fl_draw.H>

// The dimensions for the glyph in this and the Fl_Menu_Button are exactly
// the same, so that glyphs may be shared between them.

void Fl_Choice::draw() {
  Fl_Widget* o = children() ? item() : 0;
  int motif = (box()==FL_UP_BOX);
  if (motif || !o) draw_button(); else draw_box();
  int X=x(); int Y=y(); int W=w(); int H=h(); box()->inset(X,Y,W,H);
  int w1 = H*4/5;
  if (o) {
    if (!o->h() || o->damage() & FL_DAMAGE_LAYOUT) o->layout();
    Fl_Color label_color = o->label_color();
    if (!motif && focused()) {
      Fl_Color c = selection_color();
      if (c) {
	fl_color(c);
	fl_rectf(X+2, Y+2, W-w1-2, H-4);
	c = selection_text_color(); if (c) label_color = c;
      }
    }
    fl_clip(X+2, Y+2, W-w1-2, H-4);
    int save_flags = o->flags();
    if (active_r()) o->clear_flag(FL_INACTIVE);
    else o->set_flag(FL_INACTIVE);
    o->x(X);
    o->y(Y+(H-o->h())/2);
    int save_w = o->w(); o->w(W-w1);
    fl_color(label_color); o->draw();
    o->w(save_w);
    if (save_flags & FL_INACTIVE) o->set_flag(FL_INACTIVE);
    else o->clear_flag(FL_INACTIVE);
    fl_pop_clip();
  }
  // draw the little mark at the right:
  draw_glyph(FL_GLYPH_DOWN, X+W-w1, Y, w1, H, belowmouse() ? FL_HIGHLIGHT : 0);
}

// int Fl_Choice::value(int v) {
//   value(find(v));
//   redraw();
//   return 1;
// }

int Fl_Choice::handle(int e) {
  if (!children()) return 0;
  switch (e) {

  case FL_FOCUS:
  case FL_UNFOCUS:
    damage(FL_DAMAGE_HIGHLIGHT);
    return 1;

  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
    return 1;

  case FL_PUSH:
    // If you uncomment this line (or make a subclass that does this),
    // a mouse click picks the current item, and the menu goes away.  The
    // user must drag the mouse to select a different item.  Depending on
    // the size and usage of the menu, this may be more user-friendly.
//  Fl::event_is_click(0);
    // The first click on the text just moves the focus here:
    if (box()!=FL_UP_BOX // don't do this when doing motif emulation
	&& !focused() && Fl::event_x() < x()+w()-h()*4/5) {
      take_focus();
      return 1;
    }
    take_focus();
  J1:
    if (pulldown(x(), y(), w(), h(), 0)) redraw();
    return 1;

  case FL_SHORTCUT:
    if (Fl_Widget::test_shortcut()) goto J1;
    if (handle_shortcut()) {redraw(); return 1;}
    return 0;

  case FL_KEYBOARD:
    if (Fl::event_key() == ' ') goto J1;
    if (Fl::event_key() == FL_Up) {
      int i = value(); if (i < 0) i = children();
      while (i > 0) {
	Fl_Widget* w = child(--i);
	if (w->takesevents()) {value(i); execute(w); redraw(); break;}
      }
      return 1;
    }
    if (Fl::event_key() == FL_Down) {
      int i = value();
      while (++i < children()) {
	Fl_Widget* w = child(i);
	if (w->takesevents()) {value(i); execute(w); redraw(); break;}
      }
      return 1;
    }
    return 0;

  default:
    return 0;
  }
}

static void revert(Fl_Style* s) {
  s->selection_color = FL_BLUE_SELECTION_COLOR;
  s->selection_text_color = FL_WHITE;
  s->box = FL_DOWN_BOX;
  s->color = FL_WHITE;
}

static Fl_Named_Style* style = new Fl_Named_Style("Choice", revert, &style);

Fl_Choice::Fl_Choice(int x,int y,int w,int h, const char *l) : Fl_Menu_(x,y,w,h,l) {
  value(0);
  style(::style);
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_LEFT);
  when(FL_WHEN_RELEASE);
}

//
// End of "$Id: Fl_Choice.cxx,v 1.39 2000/02/14 11:32:48 bill Exp $".
//
