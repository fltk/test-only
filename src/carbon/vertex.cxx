/
// "$Id$
/
// Carbon drawing routines for the Fast Light Tool Kit (FLTK)
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

// Portable drawing code for drawing arbitrary shapes wit
// simple 2D transformations.  See also fl_arc.cx

#include <FL/fl_draw.H
#include <FL/x.H
#include <FL/math.h
#include <stdlib.h
#include "Fl_Carbon_Display.H



static XPoint *p = (XPoint *)0
// typedef what the x,y fields in a point are

typedef short COORD_T

static int p_size
static int n
static int what
enum {LINE, LOOP, POLYGON, POINT_}

void Fl_Carbon_Display::begin_points() {n = 0; what = POINT_;

void Fl_Carbon_Display::begin_line() {n = 0; what = LINE;

void Fl_Carbon_Display::begin_loop() {n = 0; what = LOOP;

void Fl_Carbon_Display::begin_polygon() {n = 0; what = POLYGON;


static void fl_transformed_vertex(COORD_T x, COORD_T y) 
  if (!n || x != p[n-1].x || y != p[n-1].y) 
    if (n >= p_size) 
      p_size = p ? 2*p_size : 16
      p = (XPoint *)realloc((void*)p, p_size*sizeof(*p))
    
    p[n].x = x
    p[n].y = y
    n++
  


void Fl_Carbon_Display::transformed_vertex(double xf, double yf) 
  fl_transformed_vertex(COORD_T(rint(xf)), COORD_T(rint(yf)))


void Fl_Carbon_Display::vertex(double x,double y) 
  Fl_Carbon_Display::transformed_vertex(x*m.a + y*m.c + m.x, x*m.b + y*m.d + m.y)


void Fl_Carbon_Display::end_points() 
  for (int i=0; i<n; i++) { MoveTo(p[i].x, p[i].y); Line(0, 0); }


void Fl_Carbon_Display::end_line() 
  if (n < 2) 
    fl_end_points()
    return
  
  if (n<=1) return
  MoveTo(p[0].x, p[0].y)
  for (int i=1; i<n; i++) LineTo(p[i].x, p[i].y)


static void fixloop() {  // remove equal points from closed pat
  while (n>2 && p[n-1].x == p[0].x && p[n-1].y == p[0].y) n--


void Fl_Carbon_Display::end_loop() 
  fixloop()
  if (n>2) fl_transformed_vertex((COORD_T)p[0].x, (COORD_T)p[0].y)
  Fl_Carbon_Display::end_line()


void Fl_Carbon_Display::end_polygon() 
  fixloop()
  if (n < 3) 
    Fl_Carbon_Display::end_line()
    return
  
  if (n<=1) return
  PolyHandle ph = OpenPoly()
  MoveTo(p[0].x, p[0].y)
  for (int i=1; i<n; i++) LineTo(p[i].x, p[i].y)
  ClosePoly()
  PaintPoly(ph)
  KillPoly(ph)



static int gap_


void Fl_Carbon_Display::begin_complex_polygon() 
  Fl_Carbon_Display::begin_polygon()
  gap_ = 0


void Fl_Carbon_Display::gap() 
  while (n>gap_+2 && p[n-1].x == p[gap_].x && p[n-1].y == p[gap_].y) n--
  if (n > gap_+2) 
    fl_transformed_vertex((COORD_T)p[gap_].x, (COORD_T)p[gap_].y)
    gap_ = n
  } else 
    n = gap_
  


void Fl_Carbon_Display::end_complex_polygon() 
  Fl_Carbon_Display::gap()
  if (n < 3) 
    Fl_Carbon_Display::end_line()
    return
  

  if (n<=1) return
  PolyHandle ph = OpenPoly()
  MoveTo(p[0].x, p[0].y)
  for (int i=1; i<n; i++) LineTo(p[i].x, p[i].y)
  ClosePoly()
  PaintPoly(ph)
  KillPoly(ph)



// shortcut the closed circles so they use XDrawArc
// warning: these do not draw rotated ellipses correctly
// See fl_arc.c for portable version

void Fl_Carbon_Display::circle(double x, double y,double r) 
  double xt = fl_transform_x(x,y)
  double yt = fl_transform_y(x,y)
  double rx = r * (m.c ? sqrt(m.a*m.a+m.c*m.c) : fabs(m.a))
  double ry = r * (m.b ? sqrt(m.b*m.b+m.d*m.d) : fabs(m.d))
  int llx = (int)rint(xt-rx)
  int w = (int)rint(xt+rx)-llx
  int lly = (int)rint(yt-ry)
  int h = (int)rint(yt+ry)-lly

  Rect rt; rt.left=llx; rt.right=llx+w; rt.top=lly; rt.bottom=lly+h
  (what == POLYGON ? PaintOval : FrameOval)(&rt)



/
// End of "$Id$"
/
