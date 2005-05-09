//
// "$Id$"
//
// Rectangle drawing routines for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include "Fl_Win_Display.H"

void Fl_Win_Display::rect(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  fl_pen();

/*

  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x+w-1, y);
  LineTo(fl_gc, x+w-1, y+h-1);
  LineTo(fl_gc, x, y+h-1);
  LineTo(fl_gc, x, y);
*/

  POINT p[5];
  p[0].x = x;
  p[0].y = y;
  p[1].x = x+w-1;
  p[1].y = y;
  p[2].x = x+w-1;
  p[2].y = y+h-1;
  p[3].x = x;
  p[3].y = y+h-1;
  p[4].x = x;
  p[4].y = y;
  Polyline(fl_gc, p, 5);
}

void Fl_Win_Display::rectf(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  RECT rect;
  rect.left = x; rect.top = y;  
  rect.right = x + w; rect.bottom = y + h;
  fl_brush();
  FillRect(fl_gc, &rect, fl_brush());
}

void Fl_Win_Display::xyline(int x, int y, int x1) {
  
  if (x1>x) x1++; else x++;

  fl_pen();
  MoveToEx(fl_gc, x, y, 0L); LineTo(fl_gc, x1, y);
}

void Fl_Win_Display::xyline(int x, int y, int x1, int y2) {

  if(x>x1) x++;
  if(y2>y) y2++;

  fl_pen();

  
  POINT p[3];
  p[0].x = x;
  p[0].y = y;
  p[1].x = x1;
  p[1].y = y;
  p[2].x = x1;
  p[2].y = y2;
  Polyline(fl_gc, p, 3);

  

}

void Fl_Win_Display::xyline(int x, int y, int x1, int y2, int x3) {

  if(x>x1) x++;
  if(x3>x1) x3++;


  fl_pen();

  POINT p[4];
  p[0].x = x;
  p[0].y = y;
  p[1].x = x1;
  p[1].y = y;
  p[2].x = x1;
  p[2].y = y2;
  p[3].x = x3;
  p[3].y = y2;
  Polyline(fl_gc, p, 4);


}

void Fl_Win_Display::yxline(int x, int y, int y1) {
  //if (y1 < y) y1--;
  //else y1++;
  fl_pen();
  
  if(y1>y){
    y1++;
    MoveToEx(fl_gc, x, y, 0L); LineTo(fl_gc, x, y1);
  }else{
    y++;
    MoveToEx(fl_gc, x, y1, 0L); LineTo(fl_gc, x, y);
  }
}

void Fl_Win_Display::yxline(int x, int y, int y1, int x2) {
  if(y>y1) y++;
  if(x2>x) x2++;

  fl_pen();


  POINT p[3];
  p[0].x = x;
  p[0].y = y;
  p[1].x = x;
  p[1].y = y1;
  p[2].x = x2;
  p[2].y = y1;
  Polyline(fl_gc, p, 3);


}

void Fl_Win_Display::yxline(int x, int y, int y1, int x2, int y3) {

  fl_pen();
  if(y>y1) y++;
  if(y3>y1) y3++;
  
  // PS_SOLID if used as a logical brush lives a gap in the joint between two patrs using LineTo in a sequence!!!!
  // Have to use Polyline instead...

  POINT p[4];
  p[0].x = x;
  p[0].y = y;
  p[1].x = x;
  p[1].y = y1;
  p[2].x = x2;
  p[2].y = y1;
  p[3].x = x2;
  p[3].y = y3;
  Polyline(fl_gc, p, 4);


}

void Fl_Win_Display::line(int x, int y, int x1, int y1) {
  fl_pen();

  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y1);
  // Draw the last point *again* because the GDI line drawing
  // functions will not draw the last point ("it's a feature!"...)
  SetPixel(fl_gc, x1, y1, fl_RGB());
  SetPixel(fl_gc, x, y, fl_RGB()); // has to do also
}

void Fl_Win_Display::line(int x, int y, int x1, int y1, int x2, int y2) {



  fl_pen();

  
// PS_SOLID if used as a logical brush lives a gap in the joint between two patrs using LineTo in a sequence!!!!
// Have to use Polyline instead...

  POINT p[3];
  p[0].x = x;
  p[0].y = y;
  p[1].x = x1;
  p[1].y = y1;
  p[2].x = x2;
  p[2].y = y2;
  Polyline(fl_gc, p, 3);


}

void Fl_Win_Display::loop(int x, int y, int x1, int y1, int x2, int y2) {
  fl_pen();
  

  // PS_SOLID if used as a logical brush lives a gap in the joint between two patrs using LineTo in a sequence!!!!
  // Have to use Polyline instead...

  POINT p[4];
  p[0].x = x;
  p[0].y = y;
  p[1].x = x1;
  p[1].y = y1;
  p[2].x = x2;
  p[2].y = y2;
  p[3].x = x;
  p[3].y = y;
  Polyline(fl_gc, p, 4);
  /*
  MoveToEx(fl_gc, x, y, 0L); 
  LineTo(fl_gc, x1, y1);
  LineTo(fl_gc, x2, y2);
  LineTo(fl_gc, x, y);
  */
}

void Fl_Win_Display::loop(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {


  fl_pen();

    // PS_SOLID if used as a logical brush lives a gap in the joint between two patrs using LineTo in a sequence!!!!
  // Have to use Polyline instead...

  POINT p[5];
  p[0].x = x;
  p[0].y = y;
  p[1].x = x1;
  p[1].y = y1;
  p[2].x = x2;
  p[2].y = y2;
  p[3].x = x3;
  p[3].y = y3;
  p[4].x = x;
  p[4].y = y;
  Polyline(fl_gc, p, 5);

}

void Fl_Win_Display::polygon(int x, int y, int x1, int y1, int x2, int y2) {
  XPoint p[4];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;

  SelectObject(fl_gc, fl_brush());
  Polygon(fl_gc, p, 3);
}

void Fl_Win_Display::polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) {
  XPoint p[5];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
  p[3].x = x3; p[3].y = y3;

  SelectObject(fl_gc, fl_brush());
  Polygon(fl_gc, p, 4);
}

void Fl_Win_Display::point(int x, int y) {
  SetPixel(fl_gc, x, y, fl_RGB());
}

////////////////////////////////////////////////////////////////

#define STACK_SIZE 10
#define STACK_MAX (STACK_SIZE - 1)
static Fl_Region rstack[STACK_SIZE];
static int rstackptr=0;
int fl_clip_state_number=0; // used by gl_begin.cxx to update GL clip

Fl_Region * fl_clip_stack  = rstack;
int * fl_clip_stack_pointer = &rstackptr;


// undo any clobbering of clip done by your program:
void fl_restore_clip() {
  fl_clip_state_number++;
  Fl_Region r = rstack[rstackptr];

  SelectClipRgn(fl_gc, r); //if r is NULL, clip is automatically cleared
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
void Fl_Win_Display::push_clip(int x, int y, int w, int h) {
  Fl_Region r;
  if (w > 0 && h > 0) {
    r = XRectangleRegion(x,y,w,h);
    Fl_Region current = rstack[rstackptr];
    if (current) {
      CombineRgn(r,r,current,RGN_AND);
    }
  } else { // make empty clip region:
    r = CreateRectRgn(0,0,0,0);
  }
  if (rstackptr < STACK_MAX) rstack[++rstackptr] = r;
  else Fl::warning("fl_push_clip: clip stack overflow!\n");
  fl_restore_clip();
}

// make there be no clip (used by fl_begin_offscreen() only!)
void Fl_Win_Display::push_no_clip() {
  if (rstackptr < STACK_MAX) rstack[++rstackptr] = 0;
  else Fl::warning("fl_push_clip: clip stack overflow!\n");
  fl_restore_clip();
}

// pop back to previous clip:
void Fl_Win_Display::pop_clip() {
  if (rstackptr > 0) {
    Fl_Region oldr = rstack[rstackptr--];
    if (oldr) XDestroyRegion(oldr);
  }else Fl::warning("fl_pop_clip: clip stack underflow!\n");
  fl_restore_clip();
}

// does this rectangle intersect current clip?
int Fl_Win_Display::not_clipped(int x, int y, int w, int h) {
  if (x+w <= 0 || y+h <= 0) return 0;
  Fl_Region r = rstack[rstackptr];

  if (!r) return 1;
  RECT rect;
  rect.left = x; rect.top = y; rect.right = x+w; rect.bottom = y+h;
  return RectInRegion(r,&rect);
}

// return rectangle surrounding intersection of this rectangle and clip:
int Fl_Win_Display::clip_box(int x, int y, int w, int h, int& X, int& Y, int& W, int& H){
  X = x; Y = y; W = w; H = h;
  Fl_Region r = rstack[rstackptr];
  if (!r) return 0;

// The win32 API makes no distinction between partial and complete
// intersection, so we have to check for partial intersection ourselves.
// However, given that the regions may be composite, we have to do
// some voodoo stuff...
  Fl_Region rr = XRectangleRegion(x,y,w,h);
  Fl_Region temp = CreateRectRgn(0,0,0,0);
  int ret;
  if (CombineRgn(temp, rr, r, RGN_AND) == NULLREGION) { // disjoint
    W = H = 0;
    ret = 2;
  } else if (EqualRgn(temp, rr)) { // complete
    ret = 0;
  } else {	// parital intersection
    RECT rect;
    GetRgnBox(temp, &rect);
    X = rect.left; Y = rect.top; W = rect.right - X; H = rect.bottom - Y;
    ret = 1;
  }
  DeleteObject(temp);
  DeleteObject(rr);
  return ret;
}

//
// End of "$Id$".
//
