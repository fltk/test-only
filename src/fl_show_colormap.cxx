//
// "$Id: fl_show_colormap.cxx,v 1.26 2002/12/10 02:01:02 easysw Exp $"
//
// Colormap color selection dialog for the Fast Light Tool Kit (FLTK).
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

// Select a color from the colormap.
// Pretty much unchanged from Forms.

#include <fltk/show_colormap.h>
#include <fltk/MenuWindow.h>
#include <fltk/ScreenInfo.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/events.h>
#include <fltk/damage.h>
using namespace fltk;

#define BOXSIZE 14
#define BORDER 4

class ColorMenu : public MenuWindow {
public:
  Color initial, which, previous;
  void drawbox(Color);
  void draw();
  int handle(int);
  ColorMenu(Color oldcol);
};

ColorMenu::ColorMenu(Color oldcol)
  : MenuWindow(BOXSIZE*8+1+2*BORDER, BOXSIZE*32+1+2*BORDER)
{
  initial = which = oldcol;
  style(Widget::default_style);
}

void ColorMenu::drawbox(Color C) {
  Color c = Color(C & 0xFF); // get the color index
  int x = (c%8)*BOXSIZE+BORDER;
  int y = (c/8)*BOXSIZE+BORDER;
  if (c == which) DOWN_BOX->draw(x+1, y+1, BOXSIZE-1, BOXSIZE-1, c);
  else BORDER_BOX->draw(x, y, BOXSIZE+1, BOXSIZE+1, c);
}

void ColorMenu::draw() {
  if (damage() != DAMAGE_CHILD) {
    draw_box();
    for (int c = 0; c < 256; c++) drawbox((Color)c);
  } else {
    drawbox(previous);
    drawbox(which);
  }
  previous = which;
}

int ColorMenu::handle(int e) {
  unsigned c = which;
  switch (e) {
  case PUSH:
  case MOVE:
  case DRAG: {
    int X = (event_x_root() - x() - BORDER);
    if (X >= 0) X = X/BOXSIZE;
    int Y = (event_y_root() - y() - BORDER);
    if (Y >= 0) Y = Y/BOXSIZE;
    if (X >= 0 && X < 8 && Y >= 0 && Y < 32)
      c = 8*Y + X;
    else
      c = initial;
    } break;
  case RELEASE:
    if (!(event_state(ANY_BUTTON))) exit_modal();
    return 1;
  case KEY:
    switch (event_key()) {
    case UpKey: if (c > 7) c -= 8; break;
    case DownKey: if (c < 256-8) c += 8; break;
    case LeftKey: if (c > 0) c--; break;
    case RightKey: if (c < 255) c++; break;
    case EscapeKey: which = initial; exit_modal(); return 1;
    case ReturnKey: exit_modal(); return 1;
    default: return 0;
    }
    break;
  default:
    return Window::handle(e);
  }
  if (c != which) {
    which = (Color)c; redraw(DAMAGE_CHILD);
    int bx = (c%8)*BOXSIZE+BORDER;
    int by = (c/8)*BOXSIZE+BORDER;
    int px = x();
    int py = y();
    int w = screenInfo().w;
    if (px+bx+BOXSIZE+BORDER >= w) px = w-bx-BOXSIZE-BORDER;
    int h = screenInfo().h;
    if (py+by+BOXSIZE+BORDER >= h) py = h-by-BOXSIZE-BORDER;
    if (px+bx < BORDER) px = BORDER-bx;
    if (py+by < BORDER) py = BORDER-by;
    position(px,py);
  }
  return 1;
}

Color fltk::show_colormap(Color oldcol) {
  ColorMenu m(oldcol);
  if (m.which > 255) {
    m.position(event_x_root()+5, event_y_root()-m.h()/2);
  } else {
    m.position(event_x_root()-(oldcol%8)*BOXSIZE-BOXSIZE/2-BORDER,
	       event_y_root()-(oldcol/8)*BOXSIZE-BOXSIZE/2-BORDER);
  }
  m.exec(0, true);
  return m.which;
}

//
// End of "$Id: fl_show_colormap.cxx,v 1.26 2002/12/10 02:01:02 easysw Exp $".
//
