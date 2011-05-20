//
// "$Id$"
//
// Menu bar widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <fltk3/run.h>
#include <fltk3/MenuBar.h>
#include <fltk3/draw.h>

void fltk3::MenuBar::draw() {
  draw_box();
  if (!menu() || !menu()->text) return;
  const fltk3::MenuItem* m;
  int X = x()+6;
  for (m=menu()->first(); m->text; m = m->next()) {
    int W = m->measure(0,this) + 16;
    m->draw(X, y(), W, h(), this);
    X += W;
    if (m->flags & fltk3::MENU_DIVIDER) {
      int y1 = y() + fltk3::box_dy(box());
      int y2 = y1 + h() - fltk3::box_dh(box()) - 1;

      // Draw a vertical divider between menus...
      fltk3::color(fltk3::DARK3);
      fltk3::yxline(X - 6, y1, y2);
      fltk3::color(fltk3::LIGHT3);
      fltk3::yxline(X - 5, y1, y2);
    }
  }
}

int fltk3::MenuBar::handle(int event) {
  const fltk3::MenuItem* v;
  if (menu() && menu()->text) switch (event) {
  case fltk3::ENTER:
  case fltk3::LEAVE:
    return 1;
  case fltk3::PUSH:
    v = 0;
  J1:
    v = menu()->pulldown(x(), y(), w(), h(), v, this, 0, 1);
    picked(v);
    return 1;
  case fltk3::SHORTCUT:
    if (visible_r()) {
      v = menu()->find_shortcut(0, true);
      if (v && v->submenu()) goto J1;
    }
    return test_shortcut() != 0;
  }
  return 0;
}

//
// End of "$Id$".
//
