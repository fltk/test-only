//
// "$Id$"
//
// Colormap color selection dialog for the Fast Light Tool Kit (FLTK).
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
#include <fltk3/SingleWindow.h>
#include <fltk3/draw.h>
#include <fltk3/show_colormap.h>
#include <fltk3/Wrapper.h>
#include <config.h>

#define BOXSIZE 14
#define BORDER 4

namespace fltk3 {
  
  /** 
   This widget creates a modal window for selecting a color from the colormap.
   Pretty much unchanged from Forms.
   */
  class ColorMenu : public fltk3::Window {
    fltk3::Color initial;
    fltk3::Color which, previous;
    int done;
    void drawbox(fltk3::Color);
    void draw();
    int handle(int);
  public:
    ColorMenu(fltk3::Color oldcol);
    fltk3::Color run();
  };
  
  ColorMenu::ColorMenu(fltk3::Color oldcol) :
  fltk3::Window(BOXSIZE*8+1+2*BORDER, BOXSIZE*32+1+2*BORDER) {
    clear_border();
    set_modal();
    initial = which = oldcol;
  }
  
  void ColorMenu::drawbox(fltk3::Color c) {
    if (c < 0 || c > 255) return;
    int X = (c%8)*BOXSIZE+BORDER;
    int Y = (c/8)*BOXSIZE+BORDER;
#if BORDER_WIDTH < 3
    if (c == which) fltk3::draw_box(fltk3::DOWN_BOX, X+1, Y+1, BOXSIZE-1, BOXSIZE-1, c);
    else fltk3::draw_box(fltk3::BORDER_BOX, X, Y, BOXSIZE+1, BOXSIZE+1, c);
#else
    fltk3::draw_box(c == which ? fltk3::DOWN_BOX : fltk3::BORDER_BOX,
                    X, Y, BOXSIZE+1, BOXSIZE+1, c);
#endif
  }
  
  void ColorMenu::draw() {
    FLTK3_OBJECT_VCALLS_WRAPPER(draw(), Draw)
    if (damage() != fltk3::DAMAGE_CHILD) {
      fltk3::draw_box(fltk3::UP_BOX,0,0,w(),h(),color());
      for (int c = 0; c < 256; c++) drawbox((fltk3::Color)c);
    } else {
      drawbox(previous);
      drawbox(which);
    }
    previous = which;
  }
  
  int ColorMenu::handle(int e) {
    FLTK3_OBJECT_VCALLS_WRAPPER_INT(handle(e), Handle)
    fltk3::Color c = which;
    switch (e) {
      case fltk3::PUSH:
      case fltk3::DRAG: {
        int X = (fltk3::event_x_root() - x() - BORDER);
        if (X >= 0) X = X/BOXSIZE;
        int Y = (fltk3::event_y_root() - y() - BORDER);
        if (Y >= 0) Y = Y/BOXSIZE;
        if (X >= 0 && X < 8 && Y >= 0 && Y < 32)
          c = 8*Y + X;
        else
          c = initial;
      } break;
      case fltk3::RELEASE:
        done = 1;
        return 1;
      case fltk3::KEYBOARD:
        switch (fltk3::event_key()) {
          case fltk3::UpKey: if (c > 7) c -= 8; break;
          case fltk3::DownKey: if (c < 256-8) c += 8; break;
          case fltk3::LeftKey: if (c > 0) c--; break;
          case fltk3::RightKey: if (c < 255) c++; break;
          case fltk3::EscapeKey: which = initial; done = 1; return 1;
          case fltk3::KPEnterKey:
          case fltk3::EnterKey: done = 1; return 1;
          default: return 0;
        }
        break;
      default:
        return 0;
    }
    if (c != which) {
      which = (fltk3::Color)c; damage(fltk3::DAMAGE_CHILD);
      int bx = (c%8)*BOXSIZE+BORDER;
      int by = (c/8)*BOXSIZE+BORDER;
      int px = x();
      int py = y();
      int scr_x, scr_y, scr_w, scr_h;
      fltk3::screen_xywh(scr_x, scr_y, scr_w, scr_h);
      if (px < scr_x) px = scr_x;
      if (px+bx+BOXSIZE+BORDER >= scr_x+scr_w) px = scr_x+scr_w-bx-BOXSIZE-BORDER;
      if (py < scr_y) py = scr_y;
      if (py+by+BOXSIZE+BORDER >= scr_y+scr_h) py = scr_y+scr_h-by-BOXSIZE-BORDER;
      if (px+bx < BORDER) px = BORDER-bx;
      if (py+by < BORDER) py = BORDER-by;
      position(px,py);
    }
    return 1;
  }
  
  extern char fl_override_redirect; // hack for menus
  
#ifdef _MSC_VER
#pragma optimize("a",off) // needed to get the done check to work
#endif
  fltk3::Color ColorMenu::run() {
    if (which < 0 || which > 255) {
      position(fltk3::event_x_root()-w()/2, fltk3::event_y_root()-y()/2);
    } else {
      position(fltk3::event_x_root()-(initial%8)*BOXSIZE-BOXSIZE/2-BORDER,
               fltk3::event_y_root()-(initial/8)*BOXSIZE-BOXSIZE/2-BORDER);
    }
    show();
    fltk3::grab(*this);
    done = 0;
    while (!done) fltk3::wait();
    fltk3::grab(0);
    return which;
  }
  
  fltk3::Color show_colormap(fltk3::Color oldcol) {
    ColorMenu m(oldcol);
    return m.run();
  }
  
}

//
// End of "$Id$".
//
