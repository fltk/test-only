//
// "$Id$"
//
// Xlib rectangle drawing routines for the Fast Light Tool Kit (FLTK).
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

// These routines from fl_draw.H are used by the standard boxtypes
// and thus are always linked into an fltk program.
// Also all fl_clip routines, since they are always linked in so
// that minimal update works.

#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include "Fl_Xlib_Display.H"

void Fl_Xlib_Display::rect(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  XDrawRectangle(fl_display, fl_window, fl_gc, x, y, w-1, h-1);
}

void Fl_Xlib_Display::rectf(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  if (w && h) XFillRectangle(fl_display, fl_window, fl_gc, x, y, w, h);
}

void Fl_Xlib_Display::xyline(int x, int y, int x1) {
  XDrawLine(fl_display, fl_window, fl_gc, x, y, x1, y);
}

void Fl_Xlib_Display::xyline(int x, int y, int x1, int y2) {
  XPoint p[3];
  p[0].x = x;  p[0].y = p[1].y = y;
  p[1].x = p[2].x = x1; p[2].y = y2;
  XDrawLines(fl_display, fl_window, fl_gc, p, 3, 0);
}

void Fl_Xlib_Display::xyline(int x, int y, int x1, int y2, int x3) {
  XPoint p[4];
  p[0].x = x;  p[0].y = p[1].y = y;
  p[1].x = p[2].x = x1; p[2].y = p[3].y = y2;
  p[3].x = x3;
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
}

void Fl_Xlib_Display::yxline(int x, int y, int y1) {
  XDrawLine(fl_display, fl_window, fl_gc, x, y, x, y1);
}

void Fl_Xlib_Display::yxline(int x, int y, int y1, int x2) {
  XPoint p[3];
  p[0].x = p[1].x = x;  p[0].y = y;
  p[1].y = p[2].y = y1; p[2].x = x2;
  XDrawLines(fl_display, fl_window, fl_gc, p, 3, 0);
}

void Fl_Xlib_Display::yxline(int x, int y, int y1, int x2, int y3) {
  XPoint p[4];
  p[0].x = p[1].x = x;  p[0].y = y;
  p[1].y = p[2].y = y1; p[2].x = p[3].x = x2;
  p[3].y = y3;
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
}

void Fl_Xlib_Display::line(int x, int y, int x1, int y1) {
  XDrawLine(fl_display, fl_window, fl_gc, x, y, x1, y1);
}

void Fl_Xlib_Display::line(int x, int y, int x1, int y1, int x2, int y2) {
  XPoint p[3];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  XDrawLines(fl_display, fl_window, fl_gc, p, 3, 0);
}

void Fl_Xlib_Display::loop(int x, int y, int x1, int y1, int x2, int y2) {
  XPoint p[4];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x;  p[3].y = y;
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
}

void Fl_Xlib_Display::loop(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  XPoint p[5];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x3; p[3].y = y3;
  p[4].x = x;  p[4].y = y;
  XDrawLines(fl_display, fl_window, fl_gc, p, 5, 0);
}

void Fl_Xlib_Display::polygon(int x, int y, int x1, int y1, int x2, int y2) {
  XPoint p[4];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;

  p[3].x = x;  p[3].y = y;
  XFillPolygon(fl_display, fl_window, fl_gc, p, 3, Convex, 0);
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
}

void Fl_Xlib_Display::polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  XPoint p[5];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x3; p[3].y = y3;

  p[4].x = x;  p[4].y = y;
  XFillPolygon(fl_display, fl_window, fl_gc, p, 4, Convex, 0);
  XDrawLines(fl_display, fl_window, fl_gc, p, 5, 0);
}

void Fl_Xlib_Display::point(int x, int y) {
  XDrawPoint(fl_display, fl_window, fl_gc, x, y);
}

////////////////////////////////////////////////////////////////

#define STACK_SIZE 10
#define STACK_MAX (STACK_SIZE - 1)
static Fl_Region rstack[STACK_SIZE];
static int rstackptr=0;
int fl_clip_state_number=0; // used by gl_begin.cxx to update GL clip

// Missing X call: (is this the fastest way to init a 1-rectangle region?)
// MSWindows equivalent exists, implemented inline in win32.H
Fl_Region XRectangleRegion(int x, int y, int w, int h) {
  XRectangle R;
  R.x = x; R.y = y; R.width = w; R.height = h;
  Fl_Region r = XCreateRegion();
  XUnionRectWithRegion(&R, r, r);
  return r;
}

// undo any clobbering of clip done by your program:
void fl_restore_clip() {
  fl_clip_state_number++;
  Fl_Region r = rstack[rstackptr];
  
  if (r) XSetRegion(fl_display, fl_gc, r);
  else XSetClipMask(fl_display, fl_gc, 0);
}

// Replace the top of the clip stack:
void fl_clip_region(Fl_Region r) {
  Fl_Region oldr = rstack[rstackptr];
  if (oldr) XDestroyRegion(oldr);
  rstack[rstackptr] = r;
  fl_restore_clip();
}

// Return the current clip region...
Fl_Region fl_clip_region() {
  return rstack[rstackptr];
}

// Intersect & push a new clip rectangle:
void Fl_Xlib_Display::push_clip(int x, int y, int w, int h) {
  Fl_Region r;
  if (w > 0 && h > 0) {
    r = XRectangleRegion(x,y,w,h);
    Fl_Region current = rstack[rstackptr];
    if (current) {
      Fl_Region temp = XCreateRegion();
      XIntersectRegion(current, r, temp);
      XDestroyRegion(r);
      r = temp;
    }
  } else { // make empty clip region:
    r = XCreateRegion();
  }
  if (rstackptr < STACK_MAX) rstack[++rstackptr] = r;
  fl_restore_clip();
}

// make there be no clip (used by fl_begin_offscreen() only!)
void Fl_Xlib_Display::push_no_clip() {
  if (rstackptr < STACK_MAX) rstack[++rstackptr] = 0;
  fl_restore_clip();
}

// pop back to previous clip:
void Fl_Xlib_Display::pop_clip() {
  if (rstackptr > 0) {
    Fl_Region oldr = rstack[rstackptr--];
    if (oldr) XDestroyRegion(oldr);
  }
  fl_restore_clip();
}

// does this rectangle intersect current clip?
int Fl_Xlib_Display::not_clipped(int x, int y, int w, int h) {
  if (x+w <= 0 || y+h <= 0 || x > Fl_Window::current()->w()
      || y > Fl_Window::current()->h()) return 0;
  Fl_Region r = rstack[rstackptr];

  return r ? XRectInRegion(r, x, y, w, h) : 1;
}

// return rectangle surrounding intersection of this rectangle and clip:
int Fl_Xlib_Display::clip_box(int x, int y, int w, int h, int& X, int& Y, int& W, int& H){
  X = x; Y = y; W = w; H = h;
  Fl_Region r = rstack[rstackptr];
  if (!r) return 0;

  switch (XRectInRegion(r, x, y, w, h)) {
  case 0: // completely outside
    W = H = 0;
    return 2;
  case 1: // completely inside:
    return 0;
  default: // partial:
    break;
  }
  Fl_Region rr = XRectangleRegion(x,y,w,h);
  Fl_Region temp = XCreateRegion();
  XIntersectRegion(r, rr, temp);
  XRectangle rect;
  XClipBox(temp, &rect);
  X = rect.x; Y = rect.y; W = rect.width; H = rect.height;
  XDestroyRegion(temp);
  XDestroyRegion(rr);
  return 1;
}

//
// End of "$Id$".
//
