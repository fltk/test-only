//
// "$Id: fl_clip.cxx,v 1.25 2004/06/23 07:17:19 spitzak Exp $"
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

#include <config.h>
#include <fltk/Window.h>
#include <fltk/draw.h>
#include <fltk/x.h>
#include <stdlib.h>
using namespace fltk;

/*! \defgroup clipping Clipping
    \ingroup drawing

  You can limit all your drawing to a region by calling
  fltk::push_clip(), and put the drawings back by using
  fltk::pop_clip(). Fltk may also set up clipping before draw() is
  called to limit the drawing to the region of the window that is
  damaged.

  When drawing you can also test the current clip region with
  fltk::not_clipped() and fltk::clip_box(). By using these to skip
  over complex drawings that are clipped you can greatly speed up your
  program's redisplay.

  <i>The width and height of the clipping region is measured in
  transformed coordianates.</i>
*/

#if USE_X11
// Region == Region
#elif defined(_WIN32)
# define Region HRGN
#elif defined(__APPLE__)
# define Region RgnHandle
#endif

static Region emptyrstack = 0;
static Region* rstack = &emptyrstack;
static int rstacksize = 0;
static int rstackptr = 0;

static inline void pushregion(Region r) {
  if (rstackptr >= rstacksize) {
    if (rstacksize) {
      rstacksize = 2*rstacksize;
      rstack = (Region*)realloc(rstack, rstacksize*sizeof(Region));
    } else {
      rstacksize = 16;
      rstack = (Region*)malloc(rstacksize*sizeof(Region));
      rstack[0] = 0;
    }
  }
  rstack[++rstackptr] = r;
}

int fl_clip_state_number = 0; // used by code that needs to update clip regions

/*! Return the current region as a system-specific structure. You must
  include <fltk/x.h> to use this. Returns null if there is no clipping.
*/
Region fltk::clip_region() {
  return rstack[rstackptr];
}

#if USE_X11
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
#if USE_CAIRO
#elif USE_X11
  if (r) XSetRegion(xdisplay, gc, r);
  else XSetClipMask(xdisplay, gc, 0);
#elif defined(_WIN32)
  SelectClipRgn(dc, r); //if r is NULL, clip is automatically cleared
#elif defined(__APPLE__)
  // We must intersect with the clip region for child windows:
  GrafPtr port; GDHandle GD; GetGWorld(&port, &GD);
  if ( port ) { // appaently this is zero for offscreen drawables
    if (r) {
      RgnHandle portClip = NewRgn();
      CopyRgn(CreatedWindow::find(Window::current())->subRegion, portClip );
      SectRgn( portClip, r, portClip );
      SetPortClipRegion( port, portClip );
      DisposeRgn( portClip );
    } else {
      SetPortClipRegion( port,
			 CreatedWindow::find(Window::current())->subRegion);
    }
  }
#else
# error
#endif
}

/*! Replace the top of the clip stack. */
void fltk::clip_region(Region r) {
  Region oldr = rstack[rstackptr];
#if USE_X11
  if (oldr) XDestroyRegion(oldr);
#elif defined(_WIN32)
  if (oldr) DeleteObject(oldr);
#elif defined(__APPLE__)
  if (oldr) DisposeRgn(oldr);
#endif
  rstack[rstackptr] = r;
  fl_restore_clip();
}

/*!
  Pushes the \e intersection of the current region and this rectangle
  onto the clip stack. */
void fltk::push_clip(int x, int y, int w, int h) {
  Region r;
  if (w > 0 && h > 0) {
    transform(x,y);
    Region current = rstack[rstackptr];
#if USE_X11
    r = XRectangleRegion(x, y, w, h);
    if (current) {
      Region temp = XCreateRegion();
      XIntersectRegion(current, r, temp);
      XDestroyRegion(r);
      r = temp;
    }
#elif defined(_WIN32)
    r = CreateRectRgn(x, y, x+w, y+h);
    if (current) CombineRgn(r,r,current,RGN_AND);
#elif defined(__APPLE__)
    r = NewRgn();
    SetRectRgn(r, x, y, x+w, y+h);
    if (current) SectRgn(r, current, r); 
#else
# error
#endif
  } else { // make empty clip region:
#if USE_X11
    r = XCreateRegion();
#elif defined(_WIN32)
    r = CreateRectRgn(0,0,0,0);
#elif defined(__APPLE__)
    r = NewRgn(); 
    SetEmptyRgn(r);
#else
# error
#endif
  }
  pushregion(r);
  fl_restore_clip();
}

#if !NO_CLIP_OUT
/*!
  Remove the rectangle from the current clip region, thus making it a
  more complex shape. This does not push the stack, it just replaces
  the top of it.
*/
void fltk::clip_out(int x, int y, int w, int h) {
  if (w <= 0 || h <= 0) return;
  Region current = rstack[rstackptr];
  // current must not be zero, you must push a rectangle first.  I
  // return without doing anything because that makes some old fltk code work:
  if (!current) return;
  transform(x,y);
#if USE_X11
  Region r = XRectangleRegion(x, y, w, h);
  Region temp = XCreateRegion();
  XSubtractRegion(current, r, temp);
  XDestroyRegion(r);
  XDestroyRegion(current);
  rstack[rstackptr] = temp;
#elif defined(_WIN32)
  Region r = CreateRectRgn(x, y, x+w, y+h);
  CombineRgn(current,current,r,RGN_DIFF);
  DeleteObject(r);
#elif defined(__APPLE__)
  Region r = NewRgn();
  SetRectRgn(r, x, y, x+w, y+h);
  DiffRgn(current, r, current);
#endif
  fl_restore_clip();
}
#endif

/*!
  Pushes an empty clip region on the stack so nothing will be
  clipped. This lets you draw outside the current clip region. This should
  only be used to temporarily ignore the clip region to draw into
  an offscreen area.
*/
void fltk::push_no_clip() {
  pushregion(0);
  fl_restore_clip();
}

/*! 
  Restore the previous clip region. You must call fltk::pop_clip()
  exactly once for every time you call fltk::push_clip(). If you return to
  FLTK with the clip stack not empty unpredictable results occur.
*/
void fltk::pop_clip() {
  if (rstackptr > 0) {
    Region oldr = rstack[rstackptr--];
#if USE_X11
    if (oldr) XDestroyRegion(oldr);
#elif defined(_WIN32)
    if (oldr) DeleteObject(oldr);
#elif defined(__APPLE__)
    if (oldr) DisposeRgn(oldr);
#endif
    fl_restore_clip();
  }
}

////////////////////////////////////////////////////////////////
// clipping tests:

/*! 
  Tests the rectangle against the current clip region.
  The return value indicates the result:
  - Returns 0 if it does not intersect
  - Returns 1 if if the result is equal to the rectangle (i.e. it is
    entirely inside or equal to the clip region)
  - Returns 2 if it is partially clipped.
*/
int fltk::not_clipped(int x, int y, int w, int h) {
  transform(x,y);
  // first check against the window so we get rid of coordinates
  // outside the 16-bit range the X/Win32 calls take:
  if (x+w <= 0 || y+h <= 0 || x >= Window::current()->w()
      || y >= Window::current()->h()) return 0;
  Region r = rstack[rstackptr];
  if (!r) return 1;
#if USE_X11
  return XRectInRegion(r, x, y, w, h);
#elif defined(_WIN32)
  //RECT rect;
  //rect.left = x; rect.top = y; rect.right = x+w; rect.bottom = y+h;
  //return RectInRegion(r,&rect);
// The win32 API makes no distinction between partial and complete
// intersection, so we have to check for partial intersection ourselves.
  int ret = 0;
  Region rr = CreateRectRgn(x, y, x+w, y+h);
  Region temp = CreateRectRgn(0, 0, 0, 0);
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
#elif defined(__APPLE__)
  if (!r) return 1;
  Rect rect;
  rect.left = x; rect.top = y; rect.right = x+w; rect.bottom = y+h;
  return RectInRgn(&rect, r);
#endif
}

/*! 
  Find the smallest rectangle that surrounds the intersection of the
  rectangle x,y,w,h with the current clip region. This "bounding box"
  is returned in X,Y,W,H. If the intersection is empty then W and H
  are set to zero.

  This can be used to limit complex pixel operations (like drawing
  images) to the smallest rectangle needed to update the visible area.

  The return values are the same as for fltk::not_clipped():
  - Returns 0 if it does not intersect, and W and H are set to zero.
  - Returns 1 if if the result is equal to the rectangle (i.e. it is
    entirely inside or equal to the clip region)
  - Returns 2 if it is partially clipped.
*/
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
  if (w <= 0 || h <= 0) {X = Y = W = H = 0; return 0;}
#if USE_X11
  switch (XRectInRegion(r, x, y, w, h)) {
  case 0: // completely outside
    X = Y = W = H = 0;
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
#elif defined(_WIN32)
// The win32 API makes no distinction between partial and complete
// intersection, so we have to check for partial intersection ourselves.
// However, given that the regions may be composite, we have to do
// some voodoo stuff...
  Region rr = CreateRectRgn(x, y, x+w, y+h);
  Region temp = CreateRectRgn(0,0,0,0);
  if (CombineRgn(temp, rr, r, RGN_AND) == NULLREGION) { // disjoint
    X = Y = W = H = 0;
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
#elif defined(__APPLE__)
  RgnHandle rr = NewRgn();
  SetRectRgn( rr, x, y, x+w, y+h );
  SectRgn( r, rr, rr );
  Rect rp; GetRegionBounds(rr, &rp);
  X = rp.left;
  Y = rp.top;
  W = rp.right - X;
  H = rp.bottom - Y;
  DisposeRgn( rr );
  if ( H==0 ) return 2;
  if ( h==H && w==W ) return 0;
  return 0;
#else
# error
#endif
}

//
// End of "$Id: fl_clip.cxx,v 1.25 2004/06/23 07:17:19 spitzak Exp $"
//
