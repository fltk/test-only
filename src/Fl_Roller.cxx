//
// "$Id: Fl_Roller.cxx,v 1.31 2002/07/01 15:28:19 spitzak Exp $"
//
// Roller widget for the Fast Light Tool Kit (FLTK).
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

// SGI Inventor / Rapid-App style knob

#include <fltk/Fl.h>
#include <fltk/Fl_Roller.h>
#include <fltk/fl_draw.h>
#include <math.h>

int Fl_Roller::handle(int event) {
  static int ipos;
  int newpos = type()==HORIZONTAL ? Fl::event_x() : -Fl::event_y();
  switch (event) {
  case FL_PUSH:
    handle_push();
    ipos = newpos;
    return 1;
  case FL_DRAG:
    handle_drag(previous_value()+(newpos-ipos)*(step()?step():.01));
    return 1;
  case FL_RELEASE:
    handle_release();
    return 1;
  case FL_KEY:
    // Only arrows in the correct direction are used.  This allows the
    // opposite arrows to be used to navigate between a set of parellel
    // rollers.
    switch (Fl::event_key()) {
    case FL_Up:
    case FL_Down:
    case FL_Home:
    case FL_End:
      if (type()==HORIZONTAL) return 0;
      break;
    case FL_Left:
    case FL_Right:
      if (type() != HORIZONTAL) return 0;
    } // else fall through...
  default:
    return Fl_Valuator::handle(event);
  }
}

void Fl_Roller::draw() {
  if (damage()&(FL_DAMAGE_ALL|FL_DAMAGE_HIGHLIGHT)) draw_box();
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  if (W<=0 || H<=0) return;

  double s = step();
  if (!s) s = (maximum()-minimum())/100;
  int offset = int(value()/s);

  const double ARC = 1.5; // 1/2 the number of radians visible
  const double delta = .2; // radians per knurl
  if (type()==HORIZONTAL) {
    // draw shaded ends of wheel:
    int h1 = W/4+1; // distance from end that shading starts
    fl_color(button_color()); fl_rectf(X+h1,Y,W-2*h1,H);
    for (int i=0; h1; i++) {
      fl_color((Fl_Color)(FL_GRAY-i-1));
      int h2 = FL_GRAY-i-1 > FL_DARK3 ? 2*h1/3+1 : 0;
      fl_rectf(X+h2,Y,h1-h2,H);
      fl_rectf(X+W-h1,Y,h1-h2,H);
      h1 = h2;
    }
    if (active_r()) {
      // draw ridges:
      double junk;
      for (double y = -ARC+modf(offset*sin(ARC)/(W/2)/delta,&junk)*delta;;
	   y += delta) {
	int y1 = int((sin(y)/sin(ARC)+1)*W/2);
	if (y1 <= 0) continue; else if (y1 >= W-1) break;
	fl_color(FL_DARK3); fl_line(X+y1,Y+1,X+y1,Y+H-1);
	if (y < 0) y1--; else y1++;
	fl_color(FL_LIGHT1);fl_line(X+y1,Y+1,X+y1,Y+H-1);
      }
      // draw edges:
      h1 = W/8+1; // distance from end the color inverts
      fl_color(FL_DARK2);
      fl_line(X+h1,Y+H-1,X+W-h1,Y+H-1);
      fl_color(FL_DARK3);
      fl_line(X,Y+H,X,Y);
      fl_line(X,Y,X+h1,Y);
      fl_line(X+W-h1,Y,X+W,Y);
      fl_color(FL_LIGHT2);
      fl_line(X+h1,Y,X+W-h1,Y);
      fl_line(X+W,Y,X+W,Y+H);
      fl_line(X+W,Y+H,X+W-h1,Y+H);
      fl_line(X+h1,Y+H,X,Y+H);
    }
  } else { // vertical one
    offset = (1-offset);
    // draw shaded ends of wheel:
    int h1 = H/4+1; // distance from end that shading starts
    fl_color(button_color()); fl_rectf(X,Y+h1,W,H-2*h1);
    for (int i=0; h1; i++) {
      fl_color((Fl_Color)(FL_GRAY-i-1));
      int h2 = FL_GRAY-i-1 > FL_DARK3 ? 2*h1/3+1 : 0;
      fl_rectf(X,Y+h2,W,h1-h2);
      fl_rectf(X,Y+H-h1,W,h1-h2);
      h1 = h2;
    }
    if (active_r()) {
      // draw ridges:
      double junk;
      for (double y = -ARC+modf(offset*sin(ARC)/(H/2)/delta,&junk)*delta;
	   ; y += delta) {
	int y1 = int((sin(y)/sin(ARC)+1)*H/2);
	if (y1 <= 0) continue; else if (y1 >= H-1) break;
	fl_color(FL_DARK3); fl_line(X+1,Y+y1,X+W-1,Y+y1);
	if (y < 0) y1--; else y1++;
	fl_color(FL_LIGHT1);fl_line(X+1,Y+y1,X+W-1,Y+y1);
      }
      // draw edges:
      h1 = H/8+1; // distance from end the color inverts
      fl_color(FL_DARK2);
      fl_line(X+W-1,Y+h1,X+W-1,Y+H-h1);
      fl_color(FL_DARK3);
      fl_line(X+W,Y,X,Y);
      fl_line(X,Y,X,Y+h1);
      fl_line(X,Y+H-h1,X,Y+H);
      fl_color(FL_LIGHT2);
      fl_line(X,Y+h1,X,Y+H-h1);
      fl_line(X,Y+H,X+W,Y+H);
      fl_line(X+W,Y+H,X+W,Y+H-h1);
      fl_line(X+W,Y+h1,X+W,Y);
    }
  }
  if (focused()) {
    focus_box()->draw(0,0,w(),h(), FL_BLACK, FL_INVISIBLE);
  }
}

Fl_Roller::Fl_Roller(int X,int Y,int W,int H,const char* L) : Fl_Valuator(X,Y,W,H,L) {
  step(.001);
  //set_click_to_focus();
}

//
// End of "$Id: Fl_Roller.cxx,v 1.31 2002/07/01 15:28:19 spitzak Exp $".
//
