//
// "$Id: fl_show_colormap.cxx,v 1.22 2001/12/16 22:32:03 spitzak Exp $"
//
// Colormap color selection dialog for the Fast Light Tool Kit (FLTK).
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

// Select a color from the colormap.
// Pretty much unchanged from Forms.

#include <fltk/Fl.h>
#include <fltk/Fl_Single_Window.h>
#include <fltk/fl_draw.h>
#include <fltk/fl_show_colormap.h>
#include <config.h>

#define BOXSIZE 14
#define BORDER 4

class ColorMenu : public Fl_Window {
public:
  Fl_Color initial, which, previous;
  void drawbox(Fl_Color);
  void draw();
  int handle(int);
  ColorMenu(Fl_Color oldcol);
};

ColorMenu::ColorMenu(Fl_Color oldcol)
  : Fl_Window(BOXSIZE*8+1+2*BORDER, BOXSIZE*32+1+2*BORDER)
{
  set_override();
  initial = which = oldcol;
  style(Fl_Widget::default_style);
}

void ColorMenu::drawbox(Fl_Color C) {
  Fl_Color c = Fl_Color(C & 0xFF); // get the color index
  int x = (c%8)*BOXSIZE+BORDER;
  int y = (c/8)*BOXSIZE+BORDER;
  if (c == which) FL_DOWN_BOX->draw(x+1, y+1, BOXSIZE-1, BOXSIZE-1, c);
  else FL_BORDER_BOX->draw(x, y, BOXSIZE+1, BOXSIZE+1, c);
}

void ColorMenu::draw() {
  if (damage() != FL_DAMAGE_CHILD) {
    draw_box();
    for (int c = 0; c < 256; c++) drawbox((Fl_Color)c);
  } else {
    drawbox(previous);
    drawbox(which);
  }
  previous = which;
}

int ColorMenu::handle(int e) {
  unsigned c = which;
  switch (e) {
  case FL_PUSH:
  case FL_MOVE:
  case FL_DRAG: {
    int X = (Fl::event_x_root() - x() - BORDER);
    if (X >= 0) X = X/BOXSIZE;
    int Y = (Fl::event_y_root() - y() - BORDER);
    if (Y >= 0) Y = Y/BOXSIZE;
    if (X >= 0 && X < 8 && Y >= 0 && Y < 32)
      c = 8*Y + X;
    else
      c = initial;
    } break;
  case FL_RELEASE:
    if (!(Fl::event_state(FL_BUTTONS))) Fl::exit_modal();
    return 1;
  case FL_KEYBOARD:
    switch (Fl::event_key()) {
    case FL_Up: if (c > 7) c -= 8; break;
    case FL_Down: if (c < 256-8) c += 8; break;
    case FL_Left: if (c > 0) c--; break;
    case FL_Right: if (c < 255) c++; break;
    case FL_Escape: which = initial; Fl::exit_modal(); return 1;
    case FL_Enter: Fl::exit_modal(); return 1;
    default: return 0;
    }
    break;
  default:
    return Fl_Window::handle(e);
  }
  if (c != which) {
    which = (Fl_Color)c; redraw(FL_DAMAGE_CHILD);
    int bx = (c%8)*BOXSIZE+BORDER;
    int by = (c/8)*BOXSIZE+BORDER;
    int px = x();
    int py = y();
    if (px+bx+BOXSIZE+BORDER >= Fl::w()) px = Fl::w()-bx-BOXSIZE-BORDER;
    if (py+by+BOXSIZE+BORDER >= Fl::h()) py = Fl::h()-by-BOXSIZE-BORDER;
    if (px+bx < BORDER) px = BORDER-bx;
    if (py+by < BORDER) py = BORDER-by;
    position(px,py);
  }
  return 1;
}

Fl_Color fl_show_colormap(Fl_Color oldcol) {
  ColorMenu m(oldcol);
  if (m.which > 255) {
    m.position(Fl::event_x_root()+5, Fl::event_y_root()-m.h()/2);
  } else {
    m.position(Fl::event_x_root()-(oldcol%8)*BOXSIZE-BOXSIZE/2-BORDER,
	       Fl::event_y_root()-(oldcol/8)*BOXSIZE-BOXSIZE/2-BORDER);
  }
  m.exec(0, true);
  return m.which;
}

//
// End of "$Id: fl_show_colormap.cxx,v 1.22 2001/12/16 22:32:03 spitzak Exp $".
//
