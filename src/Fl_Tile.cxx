//
// "$Id: Fl_Tile.cxx,v 1.22 2002/12/10 02:00:51 easysw Exp $"
//
// Tile widget for the Fast Light Tool Kit (FLTK).
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

// Group of 2 or 4 "tiles" that can be resized by dragging border
// The size of the first child determines where the resize border is.
// The resizebox is used to limit where the border can be dragged to.

#include <fltk/TiledGroup.h>
#include <fltk/Window.h>
#include <fltk/events.h>
#include <fltk/layout.h>
#include <fltk/Cursor.h>
#include <stdlib.h>
using namespace fltk;

// Drag the edges that were initially at oldx,oldy to newx,newy:
// pass zero as oldx or oldy to disable drag in that direction:

void TiledGroup::position(int oix, int oiy, int newx, int newy) {
  int* p = sizes();
  p += 8; // skip group & resizable's saved size
  int numchildren = children();
  for (int i=0; i < numchildren; p += 4, i++) {
    Widget* o = child(i);
    //if (o == resizable()) continue;
    int X = o->x();
    int R = X+o->w();
    if (oix) {
      int t = p[0];
      if (t == oix || t>oix && X<newx || t<oix && X>newx) X = newx;
      t = p[1];
      if (t == oix || t>oix && R<newx || t<oix && R>newx) R = newx;
    }
    int Y = o->y();
    int B = Y+o->h();
    if (oiy) {
      int t = p[2];
      if (t == oiy || t>oiy && Y<newy || t<oiy && Y>newy) Y = newy;
      t = p[3];
      if (t == oiy || t>oiy && B<newy || t<oiy && B>newy) B = newy;
    }
    if (o->resize(X,Y,R-X,B-Y)) o->redraw();
  }
}

// resizing is equivalent to moving  the lower-right corner (sort of):
void TiledGroup::layout() {
  int* p = sizes(); // remember the initial positions on first call here
  if (layout_damage() & LAYOUT_WH) {
    layout_damage(layout_damage() & ~LAYOUT_WH);
    // drag the corner of the group to the new position:
    position(p[1], p[3], w(), h());
    // drag the corner of the resizable() to the new position:
    if (p[5] != p[1] || p[7] != p[3])
      position(p[5], p[7], p[5]+w()-p[1], p[7]+h()-p[3]);
  }
  Group::layout();
}

static Cursor* cursors[4] = {
  0,
  CURSOR_WE,
  CURSOR_NS,
  CURSOR_MOVE};

int TiledGroup::handle(int event) {
  static int sdrag;
  static int sdx, sdy;
  static int sx, sy;
#define DRAGH 1
#define DRAGV 2
#define GRABAREA 4

  int mx = event_x();
  int my = event_y();

  switch (event) {

  case MOVE:
  case ENTER:
  case PUSH: {
    int mindx = 100;
    int mindy = 100;
    int oldx = 0;
    int oldy = 0;
    int* q = sizes();
    int* p = q+8;
    int numchildren = children();
    for (int i=0; i < numchildren; p += 4, i++) {
      Widget* o = child(i);
      if (o == resizable()) continue;
      if (p[1]<q[1] && o->y()<=my+GRABAREA && o->y()+o->h()>=my-GRABAREA) {
	int t = mx - (o->x()+o->w());
	if (abs(t) < mindx) {
	  sdx = t;
	  mindx = abs(t);
	  oldx = p[1];
	}
      }
      if (p[3]<q[3] && o->x()<=mx+GRABAREA && o->x()+o->w()>=mx-GRABAREA) {
	int t = my - (o->y()+o->h());
	if (abs(t) < mindy) {
	  sdy = t;
	  mindy = abs(t);
	  oldy = p[3];
	}
      }
    }
    sdrag = 0; sx = sy = 0;
    if (mindx <= GRABAREA) {sdrag = DRAGH; sx = oldx;}
    if (mindy <= GRABAREA) {sdrag |= DRAGV; sy = oldy;}
    cursor(cursors[sdrag]);
    if (sdrag) return 1;
    return Group::handle(event);
  }

  case DRAG:
    // This is necessary if CONSOLIDATE_MOTION in x.C is turned off:
    // if (damage()) return 1; // don't fall behind
  case RELEASE: {
    if (!sdrag) return 0; // should not happen
    Widget* r = resizable(); if (!r) r = this;
    int newx;
    if (sdrag&DRAGH) {
      newx = event_x()-sdx;
      if (newx < r->x()) newx = r->x();
      else if (newx > r->x()+r->w()) newx = r->x()+r->w();
    } else
      newx = sx;
    int newy;
    if (sdrag&DRAGV) {
      newy = event_y()-sdy;
      if (newy < r->y()) newy = r->y();
      else if (newy > r->y()+r->h()) newy = r->y()+r->h();
    } else
      newy = sy;
    position(sx,sy,newx,newy);
    do_callback();
    return 1;}

  }

  return Group::handle(event);
}

//
// End of "$Id: Fl_Tile.cxx,v 1.22 2002/12/10 02:00:51 easysw Exp $".
//
