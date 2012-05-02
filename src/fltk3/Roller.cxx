//
// "$Id$"
//
// Roller widget for the Fast Light Tool Kit (FLTK).
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

// Rapid-App style knob

#include <fltk3/run.h>
#include <fltk3/Roller.h>
#include <fltk3/draw.h>
#include <math.h>

int fltk3::Roller::handle(int event) {
  static int ipos;
  int newpos = horizontal() ? fltk3::event_x() : fltk3::event_y();
  switch (event) {
    case fltk3::PUSH:
      if (fltk3::visible_focus()) {
        fltk3::focus(this);
        redraw();
      }
      handle_push();
      ipos = newpos;
      return 1;
    case fltk3::DRAG:
      handle_drag(clamp(round(increment(previous_value(),newpos-ipos))));
      return 1;
    case fltk3::RELEASE:
      handle_release();
      return 1;
    case fltk3::KEYBOARD :
      switch (fltk3::event_key()) {
        case fltk3::UpKey:
          if (horizontal()) return 0;
          handle_drag(clamp(increment(value(),-1)));
          return 1;
        case fltk3::DownKey:
          if (horizontal()) return 0;
          handle_drag(clamp(increment(value(),1)));
          return 1;
        case fltk3::LeftKey:
          if (!horizontal()) return 0;
          handle_drag(clamp(increment(value(),-1)));
          return 1;
        case fltk3::RightKey:
          if (!horizontal()) return 0;
          handle_drag(clamp(increment(value(),1)));
          return 1;
        default:
          return 0;
      }
      // break not required because of switch...
    case fltk3::FOCUS :
    case fltk3::UNFOCUS :
      if (fltk3::visible_focus()) {
        redraw();
        return 1;
      } else return 0;
    case fltk3::ENTER :
    case fltk3::LEAVE :
      return 1;
    default:
      return 0;
  }
}

void fltk3::Roller::draw() {
  if (damage()&fltk3::DAMAGE_ALL) draw_box();
  int X = fltk3::box_dx(box());
  int Y = fltk3::box_dy(box());
  int W = w()-fltk3::box_dw(box())-1;
  int H = h()-fltk3::box_dh(box())-1;
  if (is_window_relative()) {
    X += x();
    Y += y();
  }
  if (W<=0 || H <=0) return;
  int offset = step() ? int(value()/step()) : 0;
  const double ARC = 1.5; // 1/2 the number of radians visible
  const double delta = .2; // radians per knurl
  if (horizontal()) { // horizontal one
                      // draw shaded ends of wheel:
    int h1 = W/4+1; // distance from end that shading starts
    fltk3::color(color()); fltk3::rectf(X+h1,Y,W-2*h1,H);
    for (int i=0; h1; i++) {
      fltk3::color((fltk3::Color)(fltk3::GRAY-i-1));
      int h2 = fltk3::GRAY-i-1 > fltk3::DARK3 ? 2*h1/3+1 : 0;
      fltk3::rectf(X+h2,Y,h1-h2,H);
      fltk3::rectf(X+W-h1,Y,h1-h2,H);
      h1 = h2;
    }
    if (active_r()) {
      // draw ridges:
      double junk;
      for (double yy = -ARC+modf(offset*sin(ARC)/(W/2)/delta,&junk)*delta;;
	   yy += delta) {
	int yy1 = int((sin(yy)/sin(ARC)+1)*W/2);
	if (yy1 <= 0) continue; else if (yy1 >= W-1) break;
	fltk3::color(fltk3::DARK3); fltk3::yxline(X+yy1,Y+1,Y+H-1);
	if (yy < 0) yy1--; else yy1++;
	fltk3::color(fltk3::LIGHT1);fltk3::yxline(X+yy1,Y+1,Y+H-1);
      }
      // draw edges:
      h1 = W/8+1; // distance from end the color inverts
      fltk3::color(fltk3::DARK2);
      fltk3::xyline(X+h1,Y+H-1,X+W-h1);
      fltk3::color(fltk3::DARK3);
      fltk3::yxline(X,Y+H,Y,X+h1);
      fltk3::xyline(X+W-h1,Y,X+W);
      fltk3::color(fltk3::LIGHT2);
      fltk3::xyline(X+h1,Y-1,X+W-h1);
      fltk3::yxline(X+W,Y,Y+H,X+W-h1);
      fltk3::xyline(X+h1,Y+H,X);
    }
  } else { // vertical one
           // draw shaded ends of wheel:
    int h1 = H/4+1; // distance from end that shading starts
    fltk3::color(color()); fltk3::rectf(X,Y+h1,W,H-2*h1);
    for (int i=0; h1; i++) {
      fltk3::color((fltk3::Color)(fltk3::GRAY-i-1));
      int h2 = fltk3::GRAY-i-1 > fltk3::DARK3 ? 2*h1/3+1 : 0;
      fltk3::rectf(X,Y+h2,W,h1-h2);
      fltk3::rectf(X,Y+H-h1,W,h1-h2);
      h1 = h2;
    }
    if (active_r()) {
      // draw ridges:
      double junk;
      for (double yy = -ARC+modf(offset*sin(ARC)/(H/2)/delta,&junk)*delta;
	   ; yy += delta) {
	int yy1 = int((sin(yy)/sin(ARC)+1)*H/2);
	if (yy1 <= 0) continue; else if (yy1 >= H-1) break;
	fltk3::color(fltk3::DARK3); fltk3::xyline(X+1,Y+yy1,X+W-1);
	if (yy < 0) yy1--; else yy1++;
	fltk3::color(fltk3::LIGHT1);fltk3::xyline(X+1,Y+yy1,X+W-1);
      }
      // draw edges:
      h1 = H/8+1; // distance from end the color inverts
      fltk3::color(fltk3::DARK2);
      fltk3::yxline(X+W-1,Y+h1,Y+H-h1);
      fltk3::color(fltk3::DARK3);
      fltk3::xyline(X+W,Y,X,Y+h1);
      fltk3::yxline(X,Y+H-h1,Y+H);
      fltk3::color(fltk3::LIGHT2);
      fltk3::yxline(X,Y+h1,Y+H-h1);
      fltk3::xyline(X,Y+H,X+W,Y+H-h1);
      fltk3::yxline(X+W,Y+h1,Y);
    }
  }
  
  if (fltk3::focus() == this) draw_focus(fltk3::THIN_UP_FRAME, 0, 0, w(), h());
}

/**
 Creates a new fltk3::Roller widget using the given position,
 size, and label string. The default boxtype is fltk3::NO_BOX.
 <P>Inherited destructor destroys the valuator.
 */
fltk3::Roller::Roller(int X,int Y,int W,int H,const char* L)
: fltk3::Valuator(X,Y,W,H,L) 
{
  set_group_relative();
  box(fltk3::UP_BOX);
  step(1,1000);
}

//
// End of "$Id$".
//
