//
// "$Id: Fl_Menu_Bar.cxx,v 1.42 2001/01/23 18:47:54 spitzak Exp $"
//
// Menu bar widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_draw.H>

void Fl_Menu_Bar::draw() {
  if (damage()&(~FL_DAMAGE_HIGHLIGHT)) draw_box();
  if (!children()) { last_ = -1; return; }
  int X = 3;
  for (int i = 0; i < children(); i++) {
    Fl_Widget* widget = child(i);
    if (!widget->visible()) continue;
    int W = widget->width() + 10;
    if (damage()&(~FL_DAMAGE_HIGHLIGHT) || last_ == i || highlight_ == i) {
      // If you change how the items are drawn, you probably need to
      // change MenuTitle::draw and the functions find_selected and
      // titlex in Fl_Menu.cxx.
      if (i == highlight_ && highlight_color() && takesevents())
	widget->set_flag(FL_HIGHLIGHT);
      else
	widget->clear_flag(FL_HIGHLIGHT);
      int x1 = X; int y1 = 0; int w1 = W; int h1 = this->h();
      box()->inset(x1,y1,w1,h1);
      text_box()->draw(this, X, y1+1, W, h1-2, widget->flags()&~FL_VALUE);
      int save_x = fl_x_; fl_x_ += X+5;
      int save_y = fl_y_; fl_y_ += (h()-widget->h())/2;
      int save_w = widget->w(); widget->w(W-10);
      widget->draw();
      widget->w(save_w);
      fl_y_ = save_y;
      fl_x_ = save_x;
    }
    X += W;
  }
  last_ = highlight_;
}

int Fl_Menu_Bar::handle(int event) {
  if (!children()) return 0;
  int X = 3;
  int i;
  highlight_ = -1;
  // FL_LEAVE events don't get the right coordinates
  if (event != FL_LEAVE) for (i = 0; i < children(); ++i) {
    Fl_Widget* widget = child(i);
    if (!widget->visible()) continue;
    int W = widget->width() + 10;
    if (Fl::event_inside(X, 0, W, h())) {
      highlight_ = i;
      break;
    }
    X += W;
  }
  switch (event) {
  case FL_MOVE:
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_ == last_) return 1;
    if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
    return 1;
  case FL_PUSH:
    if (highlight_ < 0) return 0;
    value(-1);
  J1:
    highlight_ = -1; damage(FL_DAMAGE_HIGHLIGHT);
    pulldown(0, 0, w(), h(), 0, 1);
    return 1;
  case FL_SHORTCUT:
    for (i = 0; i < children(); i++) {
      Fl_Widget* w = child(i);
      if (w->is_group() && w->active() && w->test_shortcut()) {
	value(i); goto J1;
      }
    }
    if (handle_shortcut()) return 1;
    return 0;
  case FL_KEYUP:
    if ((Fl::event_key() == FL_Alt_L || Fl::event_key() == FL_Alt_R)
	&& Fl::event_clicks()) {
      // checking for event_clicks insures that the keyup matches the
      // keydown that preceeded it, so Alt was pressed & released without
      // any intermediate values.  On X it is false if Alt is held down
      // for a long time, too.
      for (i = 0; i < children(); i++) {
	Fl_Widget* w = child(i);
	if (w->active()) {value(i); goto J1;}
      }
      return 0;
    }
  }
  return 0;
}

// The default style for menu bars.  The text_box() is used to draw
// the boxes around the popup titles, this is done by Fl_Menu.cxx.

static void revert(Fl_Style* s) {
  s->box = FL_FLAT_BOX;
  s->text_background = FL_GRAY; // controls color of the menu
#if 0
  // NT 4.0 style
  s->text_box = FL_FLAT_BOX;
#else
  // Windows98 style:
  s->text_box = FL_HIGHLIGHT_BOX;
#endif
  s->leading = 4;
}

static Fl_Named_Style* style = new Fl_Named_Style("Menu Bar", revert, &style);

Fl_Menu_Bar::Fl_Menu_Bar(int x,int y,int w,int h,const char *l)
  : Fl_Menu_(x,y,w,h,l)
{
  style(::style);
}

//
// End of "$Id: Fl_Menu_Bar.cxx,v 1.42 2001/01/23 18:47:54 spitzak Exp $".
//
