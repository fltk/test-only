//
// "$Id: Fl_Menu_Bar.cxx,v 1.24 1999/11/10 12:21:52 bill Exp $"
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

void Fl_Menu_Bar::draw() {
  if (damage()&(~FL_DAMAGE_HIGHLIGHT)) draw_box();
  if (!menu() || !menu()->text) { last_ = 0; return; }
  const Fl_Menu_Item* m;
  int X = x()+3;
  for (m=menu(); m->text; m = m->next()) {
    int W =m->measure(0, this, 1) + 16;
    int selected = (m == highlight_ && m->highlight_color()) ? 3 : 1;
    if (damage()&(~FL_DAMAGE_HIGHLIGHT) || last_ == m || highlight_ == m)
      m->draw(X, y() + 3, W, h() - 6, this, 2, selected);
    X += W;
  }
  last_ = highlight_;
}

int Fl_Menu_Bar::handle(int event) {
  const Fl_Menu_Item* v;
  const Fl_Menu_Item* m;
  int X = x()+3;
  highlight_ = 0;
  // FL_LEAVE events don't get the right coordinates
  if (event != FL_LEAVE) for (m=menu(); m->text; m = m->next()) {
    int W = m->measure(0, this, 1) + 16;
    if (Fl::event_inside(X, y() + 3, W, h() -6)) { highlight_ = m; break; }
    X += W;
  }
  if (menu() && menu()->text) switch (event) {
  case FL_MOVE:
    if (highlight_ == last_) return 1;
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
    return 1;
  case FL_PUSH:
    v = 0;
  J1:
    highlight_ = 0; damage(FL_DAMAGE_HIGHLIGHT);
    v = menu()->pulldown(x(), y(), w(), h(), v, this, 0, 1);
    picked(v);
    return 1;
  case FL_SHORTCUT:
    v = menu()->test_shortcut();
    if (v) {picked(v); return 1;}
    if (visible_r() && (v = menu()->find_shortcut())) goto J1;
    return 0;
  }
  return 0;
}

static void revert(Fl_Style* s) {
  s->box = FL_FLAT_BOX;
}

Fl_Named_Style Fl_Menu_Bar::default_style("Menu_Bar", revert);

//
// End of "$Id: Fl_Menu_Bar.cxx,v 1.24 1999/11/10 12:21:52 bill Exp $".
//
