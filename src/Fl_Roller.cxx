//
// "$Id: Fl_Roller.cxx,v 1.33 2002/12/10 01:46:17 easysw Exp $"
//
// Roller widget for the Fast Light Tool Kit (FLTK).
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

// SGI Inventor / Rapid-App style knob

#include <fltk/ThumbWheel.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/math.h>
using namespace fltk;

int ThumbWheel::handle(int event) {
  static int ipos;
  int newpos = type()==HORIZONTAL ? event_x() : -event_y();
  switch (event) {
  case PUSH:
    handle_push();
    ipos = newpos;
    return 1;
  case DRAG:
    handle_drag(previous_value()+(newpos-ipos)*(step()?step():.01));
    return 1;
  case RELEASE:
    handle_release();
    return 1;
  case KEY:
    // Only arrows in the correct direction are used.  This allows the
    // opposite arrows to be used to navigate between a set of parellel
    // rollers.
    switch (event_key()) {
    case UpKey:
    case DownKey:
    case HomeKey:
    case EndKey:
      if (type()==HORIZONTAL) return 0;
      break;
    case LeftKey:
    case RightKey:
      if (type() != HORIZONTAL) return 0;
    } // else fall through...
  default:
    return Valuator::handle(event);
  }
}

void ThumbWheel::draw() {
  if (damage()&(DAMAGE_ALL|DAMAGE_HIGHLIGHT)) draw_box();
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
    setcolor(buttoncolor()); fillrect(X+h1,Y,W-2*h1,H);
    for (int i=0; h1; i++) {
      setcolor((Color)(GRAY75-i-1));
      int h2 = GRAY75-i-1 > GRAY33 ? 2*h1/3+1 : 0;
      fillrect(X+h2,Y,h1-h2,H);
      fillrect(X+W-h1,Y,h1-h2,H);
      h1 = h2;
    }
    if (active_r()) {
      // draw ridges:
      double junk;
      for (double y = -ARC+modf(offset*sin(ARC)/(W/2)/delta,&junk)*delta;;
	   y += delta) {
	int y1 = int((sin(y)/sin(ARC)+1)*W/2);
	if (y1 <= 0) continue; else if (y1 >= W-1) break;
	setcolor(GRAY33); drawline(X+y1,Y+1,X+y1,Y+H-1);
	if (y < 0) y1--; else y1++;
	setcolor(GRAY85);drawline(X+y1,Y+1,X+y1,Y+H-1);
      }
      // draw edges:
      h1 = W/8+1; // distance from end the color inverts
      setcolor(GRAY60);
      drawline(X+h1,Y+H-1,X+W-h1,Y+H-1);
      setcolor(GRAY33);
      drawline(X,Y+H,X,Y);
      drawline(X,Y,X+h1,Y);
      drawline(X+W-h1,Y,X+W,Y);
      setcolor(GRAY90);
      drawline(X+h1,Y,X+W-h1,Y);
      drawline(X+W,Y,X+W,Y+H);
      drawline(X+W,Y+H,X+W-h1,Y+H);
      drawline(X+h1,Y+H,X,Y+H);
    }
  } else { // vertical one
    offset = (1-offset);
    // draw shaded ends of wheel:
    int h1 = H/4+1; // distance from end that shading starts
    setcolor(buttoncolor()); fillrect(X,Y+h1,W,H-2*h1);
    for (int i=0; h1; i++) {
      setcolor((Color)(GRAY75-i-1));
      int h2 = GRAY75-i-1 > GRAY33 ? 2*h1/3+1 : 0;
      fillrect(X,Y+h2,W,h1-h2);
      fillrect(X,Y+H-h1,W,h1-h2);
      h1 = h2;
    }
    if (active_r()) {
      // draw ridges:
      double junk;
      for (double y = -ARC+modf(offset*sin(ARC)/(H/2)/delta,&junk)*delta;
	   ; y += delta) {
	int y1 = int((sin(y)/sin(ARC)+1)*H/2);
	if (y1 <= 0) continue; else if (y1 >= H-1) break;
	setcolor(GRAY33); drawline(X+1,Y+y1,X+W-1,Y+y1);
	if (y < 0) y1--; else y1++;
	setcolor(GRAY85);drawline(X+1,Y+y1,X+W-1,Y+y1);
      }
      // draw edges:
      h1 = H/8+1; // distance from end the color inverts
      setcolor(GRAY60);
      drawline(X+W-1,Y+h1,X+W-1,Y+H-h1);
      setcolor(GRAY33);
      drawline(X+W,Y,X,Y);
      drawline(X,Y,X,Y+h1);
      drawline(X,Y+H-h1,X,Y+H);
      setcolor(GRAY90);
      drawline(X,Y+h1,X,Y+H-h1);
      drawline(X,Y+H,X+W,Y+H);
      drawline(X+W,Y+H,X+W,Y+H-h1);
      drawline(X+W,Y+h1,X+W,Y);
    }
  }
  if (focused()) {
    focusbox()->draw(0,0,w(),h(), BLACK, INVISIBLE);
  }
}

ThumbWheel::ThumbWheel(int X,int Y,int W,int H,const char* L) : Valuator(X,Y,W,H,L) {
  step(.001);
  //set_click_to_focus();
}

//
// End of "$Id: Fl_Roller.cxx,v 1.33 2002/12/10 01:46:17 easysw Exp $".
//
