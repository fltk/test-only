/
// "$Id$
/
// Carbon rectangle drawing routines for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2003 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

// These routines from fl_draw.H are used by the standard boxtype
// and thus are always linked into an fltk program
// Also all fl_clip routines, since they are always linked in s
// that minimal update works

#include <FL/Fl_Widget.H
#include <FL/fl_draw.H
#include <FL/x.H
#include "Fl_Carbon_Display.H

void Fl_Carbon_Display::rect(int x, int y, int w, int h) 
  if (w<=0 || h<=0) return
  Rect rect
  SetRect(&rect, x, y, x+w, y+h)
  FrameRect(&rect)


void Fl_Carbon_Display::rectf(int x, int y, int w, int h) 
  if (w<=0 || h<=0) return
  Rect rect
  SetRect(&rect, x, y, x+w, y+h)
  PaintRect(&rect)


void Fl_Carbon_Display::xyline(int x, int y, int x1) 
  MoveTo(x, y); LineTo(x1, y)


void Fl_Carbon_Display::xyline(int x, int y, int x1, int y2) 
  MoveTo(x, y);
  LineTo(x1, y)
  LineTo(x1, y2)


void Fl_Carbon_Display::xyline(int x, int y, int x1, int y2, int x3) 
  MoveTo(x, y);
  LineTo(x1, y)
  LineTo(x1, y2)
  LineTo(x3, y2)


void Fl_Carbon_Display::yxline(int x, int y, int y1) 
  MoveTo(x, y); LineTo(x, y1)


void Fl_Carbon_Display::yxline(int x, int y, int y1, int x2) 
  MoveTo(x, y);
  LineTo(x, y1)
  LineTo(x2, y1)


void Fl_Carbon_Display::yxline(int x, int y, int y1, int x2, int y3) 
  MoveTo(x, y);
  LineTo(x, y1)
  LineTo(x2, y1)
  LineTo(x2, y3)


void Fl_Carbon_Display::line(int x, int y, int x1, int y1) 
  MoveTo(x, y);
  LineTo(x1, y1)


void Fl_Carbon_Display::line(int x, int y, int x1, int y1, int x2, int y2) 
  MoveTo(x, y);
  LineTo(x1, y1)
  LineTo(x2, y2)


void Fl_Carbon_Display::loop(int x, int y, int x1, int y1, int x2, int y2) 
  MoveTo(x, y);
  LineTo(x1, y1)
  LineTo(x2, y2)
  LineTo(x, y)


void Fl_Carbon_Display::loop(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) 
  MoveTo(x, y);
  LineTo(x1, y1)
  LineTo(x2, y2)
  LineTo(x3, y3)
  LineTo(x, y)


void Fl_Carbon_Display::polygon(int x, int y, int x1, int y1, int x2, int y2) 
  XPoint p[4]
  p[0].x = x;  p[0].y = y
  p[1].x = x1; p[1].y = y1
  p[2].x = x2; p[2].y = y2

  PolyHandle poly = OpenPoly()
  MoveTo(x, y)
  LineTo(x1, y1)
  LineTo(x2, y2)
  ClosePoly()
  PaintPoly(poly)
  KillPoly(poly)


void Fl_Carbon_Display::polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) 
  XPoint p[5]
  p[0].x = x;  p[0].y = y
  p[1].x = x1; p[1].y = y1
  p[2].x = x2; p[2].y = y2
  p[3].x = x3; p[3].y = y3

  PolyHandle poly = OpenPoly()
  MoveTo(x, y)
  LineTo(x1, y1)
  LineTo(x2, y2)
  LineTo(x3, y3)
  ClosePoly()
  PaintPoly(poly)
  KillPoly(poly)


void Fl_Carbon_Display::point(int x, int y) 
  MoveTo(x, y); Line(0, 0);


///////////////////////////////////////////////////////////////

#define STACK_SIZE 1
#define STACK_MAX (STACK_SIZE - 1
static Fl_Region rstack[STACK_SIZE]
static int rstackptr=0
int fl_clip_state_number=0; // used by gl_begin.cxx to update GL cli


extern Fl_Region fl_window_region


// undo any clobbering of clip done by your program
void fl_restore_clip() 
  fl_clip_state_number++
  Fl_Region r = rstack[rstackptr]

#  if 
  if ( fl_window )
  
    GrafPtr port = GetWindowPort( fl_window )
    if ( port ) { // port will be NULL if we are using a GWorld (and fl_window_region is invalid
      RgnHandle portClip = NewRgn()
      CopyRgn( fl_window_region, portClip ); // change
      if ( r )
        SectRgn( portClip, r, portClip )
      SetPortClipRegion( port, portClip )
      DisposeRgn( portClip )
    
  
#  els
  if (r) SetClip(r)
  else 
    Rect rect; rect.left=0; rect.top=0; rect.right=0x7fff; rect.bottom=0x7fff
    ClipRect(&rect)
  
#  endi


// Replace the top of the clip stack
void fl_clip_region(Fl_Region r) 
  Fl_Region oldr = rstack[rstackptr]
  if (oldr) XDestroyRegion(oldr)
  rstack[rstackptr] = r
  fl_restore_clip()


// Return the current clip region..
Fl_Region fl_clip_region() 
  return rstack[rstackptr]


// Intersect & push a new clip rectangle
void Fl_Carbon_Display::push_clip(int x, int y, int w, int h) 
  Fl_Region r
  if (w > 0 && h > 0) 
    r = XRectangleRegion(x,y,w,h)
    Fl_Region current = rstack[rstackptr]
    if (current) 
      SectRgn(r, current, r);
    
  } else { // make empty clip region
    r = NewRgn();
    SetEmptyRgn(r)
  
  if (rstackptr < STACK_MAX) rstack[++rstackptr] = r
  fl_restore_clip()


// make there be no clip (used by fl_begin_offscreen() only!
void Fl_Carbon_Display::push_no_clip() 
  if (rstackptr < STACK_MAX) rstack[++rstackptr] = 0
  fl_restore_clip()


// pop back to previous clip
void Fl_Carbon_Display::pop_clip() 
  if (rstackptr > 0) 
    Fl_Region oldr = rstack[rstackptr--]
    if (oldr) XDestroyRegion(oldr)
  
  fl_restore_clip()


// does this rectangle intersect current clip
int Fl_Carbon_Display::not_clipped(int x, int y, int w, int h) 
  if (x+w <= 0 || y+h <= 0 || x > Fl_Window::current()->w(
      || y > Fl_Window::current()->h()) return 0
  Fl_Region r = rstack[rstackptr]

  if (!r) return 1
  Rect rect
  rect.left = x; rect.top = y; rect.right = x+w; rect.bottom = y+h
  return RectInRgn(&rect, r)


// return rectangle surrounding intersection of this rectangle and clip
int Fl_Carbon_Display::clip_box(int x, int y, int w, int h, int& X, int& Y, int& W, int& H)
  X = x; Y = y; W = w; H = h
  Fl_Region r = rstack[rstackptr]
  if (!r) return 0

  RgnHandle rr = NewRgn()
  SetRectRgn( rr, x, y, x+w, y+h )
  SectRgn( r, rr, rr )
  Rect rp; GetRegionBounds(rr, &rp)
  X = rp.left
  Y = rp.top
  W = rp.right - X
  H = rp.bottom - Y
  DisposeRgn( rr )
  if ( H==0 ) return 2
  if ( h==H && w==W ) return 0
  return 0



/
// End of "$Id$"
/
