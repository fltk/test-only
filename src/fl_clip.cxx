//
// "$Id: fl_clip.cxx,v 1.14 2002/12/10 02:00:57 easysw Exp $"
//
// The fltk graphics clipping stack.  These routines are always
// linked into an fltk program.
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

#include <fltk/x.h>
#include <fltk/Window.h>
#include <fltk/draw.h>
using namespace fltk;

#define STACK_SIZE 11
#define STACK_MAX (STACK_SIZE - 2)
static Region rstack[STACK_SIZE];
static int rstackptr=0;
int fl_clip_state_number = 0; // used by code that needs to update clip regions

// Return the current region (for Xft and Xrender use), return null if
// no clipping.
Region fltk::clip_region() {
  return rstack[rstackptr];
}

#ifndef _WIN32
// Missing X call: (is this the fastest way to init a 1-rectangle region?)
// MSWindows equivalent exists, implemented inline in win32.h
Region XRectangleRegion(int x, int y, int w, int h) {
  XRectangle R;
  R.x = x; R.y = y; R.width = w; R.height = h;
  Region r = XCreateRegion();
  XUnionRectWithRegion(&R, r, r);
  return r;
}
#endif

// Make the system's clip match the top of the clip stack.  This can
// be used after changing the stack, or to undo any clobbering of clip
// done by your program:
void fl_restore_clip() {
  Region r = rstack[rstackptr];
  fl_clip_state_number++;
#ifdef _WIN32
  SelectClipRgn(gc, r); //if r is NULL, clip is automatically cleared
#else
  if (r) XSetRegion(xdisplay, gc, r);
  else XSetClipMask(xdisplay, gc, 0);
#endif
}

// Replace the top of the clip stack:
void fltk::clip_region(Region r) {
  Region oldr = rstack[rstackptr];
  if (oldr) XDestroyRegion(oldr);
  rstack[rstackptr] = r;
  fl_restore_clip();
}

// Intersect & push a new clip rectangle:
void fltk::push_clip(int x, int y, int w, int h) {
  Region r;
  if (w > 0 && h > 0) {
    transform(x,y);
    r = XRectangleRegion(x, y, w, h);
    Region current = rstack[rstackptr];
    if (current) {
#ifndef _WIN32
      Region temp = XCreateRegion();
      XIntersectRegion(current, r, temp);
      XDestroyRegion(r);
      r = temp;
#else
      CombineRgn(r,r,current,RGN_AND);
#endif
    }
  } else { // make empty clip region:
#ifndef _WIN32
    r = XCreateRegion();
#else
    r = CreateRectRgn(0,0,0,0);
#endif
  }
  if (rstackptr < STACK_MAX) rstack[++rstackptr] = r;
  fl_restore_clip();
}

// Replace top of stack with top of stack minus this rectangle:
void fltk::clip_out(int x, int y, int w, int h) {
  if (w <= 0 || h <= 0) return;
  Region current = rstack[rstackptr];
  // current must not be zero, you must push a rectangle first.  I
  // return without doing anything because that makes some old fltk code work:
  if (!current) return;
  transform(x,y);
  Region r = XRectangleRegion(x, y, w, h);
#ifndef _WIN32
  Region temp = XCreateRegion();
  XSubtractRegion(current, r, temp);
  XDestroyRegion(r);
  XDestroyRegion(current);
  rstack[rstackptr] = temp;
#else
  CombineRgn(current,current,r,RGN_DIFF);
  DeleteObject(r);
#endif
  fl_restore_clip();
}

// make there be no clip (used by begin_offscreen() only!)
void fltk::push_no_clip() {
  // this does not test maximum so that this is guaranteed to work,
  // there is one extra slot at the top of the stack.
  /*if (rstackptr < STACK_MAX)*/ rstack[++rstackptr] = 0;
  fl_restore_clip();
}

// pop back to previous clip:
void fltk::pop_clip() {
  if (rstackptr > 0) {
    Region oldr = rstack[rstackptr--];
    if (oldr) XDestroyRegion(oldr);
    fl_restore_clip();
  }
}

////////////////////////////////////////////////////////////////
// clipping tests:

// does this rectangle intersect current clip?
int fltk::not_clipped(int x, int y, int w, int h) {
  transform(x,y);
  // first check against the window so we get rid of coordinates
  // outside the 16-bit range the X/Win32 calls take:
  if (x+w <= 0 || y+h <= 0 || x >= Window::current()->w()
      || y >= Window::current()->h()) return 0;
  Region r = rstack[rstackptr];
  if (!r) return 1;
#ifndef _WIN32
  return XRectInRegion(r, x, y, w, h);
#else
  //RECT rect;
  //rect.left = x; rect.top = y; rect.right = x+w; rect.bottom = y+h;
  //return RectInRegion(r,&rect);
// The win32 API makes no distinction between partial and complete
// intersection, so we have to check for partial intersection ourselves.
  int ret = 0;
  Region rr = XRectangleRegion(x,y,w,h);
  Region temp = CreateRectRgn(0,0,0,0);
  if (CombineRgn(temp, rr, r, RGN_AND) == NULLREGION) { // disjoint
    ret = 0;
  } else if (EqualRgn(temp, rr)) { // complete
    ret = 1;
  } else {	// parital intersection
    ret = 2;
  }
  DeleteObject(temp);
  DeleteObject(rr);
  return ret;
#endif
}

// return rectangle surrounding intersection of this rectangle and clip,
int fltk::clip_box(int x,int y,int w,int h, int& X,int& Y,int& W,int& H) {
  Region r = rstack[rstackptr];
  if (!r) {X = x; Y = y; W = w; H = h; return 0;}
  // Test against the window to get 16-bit values (this is only done if
  // a clip region exists as otherwise it breaks push_no_clip()):
  int ret = 1;
  int dx = x; int dy = y; transform(x,y); dx = x-dx; dy = y-dy;
  if (x < 0) {w += x; x = 0; ret = 2;}
  int t = Window::current()->w(); if (x+w > t) {w = t-x; ret = 2;}
  if (y < 0) {h += y; y = 0; ret = 2;}
  t = Window::current()->h(); if (y+h > t) {h = t-y; ret = 2;}
  // check for total clip (or for empty rectangle):
  if (w <= 0 || h <= 0) {W = H = 0; return 0;}
#ifndef _WIN32
  switch (XRectInRegion(r, x, y, w, h)) {
  case 0: // completely outside
    W = H = 0;
    return 0;
  case 1: // completely inside:
    X = x-dx;
    Y = y-dy;
    W = w; H = h;
    return ret;
  default: { // partial:
    Region rr = XRectangleRegion(x,y,w,h);
    Region temp = XCreateRegion();
    XIntersectRegion(r, rr, temp);
    XRectangle rect;
    XClipBox(temp, &rect);
    X = rect.x-dx; Y = rect.y-dy; W = rect.width; H = rect.height;
    XDestroyRegion(temp);
    XDestroyRegion(rr);
    return 2;}
  }
#else
// The win32 API makes no distinction between partial and complete
// intersection, so we have to check for partial intersection ourselves.
// However, given that the regions may be composite, we have to do
// some voodoo stuff...
  Region rr = XRectangleRegion(x,y,w,h);
  Region temp = CreateRectRgn(0,0,0,0);
  if (CombineRgn(temp, rr, r, RGN_AND) == NULLREGION) { // disjoint
    W = H = 0;
    ret = 0;
  } else if (EqualRgn(temp, rr)) { // complete
    X = x-dx;
    Y = y-dy;
    W = w; H = h;
    // ret = ret
  } else {	// parital intersection
    RECT rect;
    GetRgnBox(temp, &rect);
    X = rect.left-dx; Y = rect.top-dy;
    W = rect.right - rect.left; H = rect.bottom - rect.top;
    ret = 2;
  }
  DeleteObject(temp);
  DeleteObject(rr);
  return ret;
#endif
}

//
// End of "$Id: fl_clip.cxx,v 1.14 2002/12/10 02:00:57 easysw Exp $"
//
