//
// "$Id: fl_vertex.cxx,v 1.28 2004/06/09 05:38:58 spitzak Exp $"
//
// Path construction and filling. I think this file is always linked
// into any fltk program, so try to keep it reasonably small.
// Also see arc.cxx and curve.cxx
//
// Copyright 2001 by Bill Spitzak and others.
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
#include <fltk/draw.h>
#include <fltk/math.h>
#include <fltk/x.h>
#include <stdlib.h>
using namespace fltk;

/** \defgroup drawing Drawing Functions

  There are only certain places you can execute drawing code in
  FLTK. Calling these functions at other places will result in
  undefined behavior!

  - The most common is inside the virtual method Widget::draw(). To
    write code here, you must subclass one of the existing
    Widget classes and implement your own version of draw().
  - You can also write boxtypes and labeltypes. These are small
    structures with functions in them that rae called by existing
    fltk::Widget::draw() methods. Pointers to these structures are
    stored in the widget's box() , labeltype(), and possibly other
    properties.
  - You can write fltk::Image classes, which can then be put into the
    fltk::Widget::image() and be called by existing
    fltk::Widget::draw() methods.
  - You can call fltk::Window::make_current() to do incremental update
    of a widget. Use fltk::Widget::window() to find the window.

  All functions are defined by including <fltk/draw.h>
*/

////////////////////////////////////////////////////////////////
/*! \defgroup transformation Transformation
    \ingroup drawing

  FLTK provides an arbitrary 2-D linear transformation (ie rotation,
  scale, skew, reflections, and translation). This is very similar to
  PostScript, PDF, and SVG.

  Due to limited graphics capabilities of some systems, all drawing
  methods that take int values only translate the x and y values, and
  round them to the nearest integer. You should use functions that
  take floating-point coordinates if you want accurately scaled
  drawings.

  \{
*/

struct Matrix {
  float a, b, c, d, x, y;
  int ix, iy; // x & y rounded to nearest integer
  bool trivial; // true if no rotation or scale
};

static Matrix m = {1, 0, 0, 1, 0, 0, 0, 0, true};
static Matrix* stack;
static int stacksize = 0;
static int sptr = 0;

/*! Save the current transformation on a stack, so you can restore it
  with pop_matrix(). */
void fltk::push_matrix() {
  if (sptr >= stacksize) {
    stacksize = stacksize ? 2*stacksize : 16;
    stack = (Matrix*)realloc(stack, stacksize*sizeof(Matrix));
  }
  stack[sptr++] = m;
}

/*! Put the transformation back to the way it was before the last
  push_matrix(). Calling this without a matching push_matrix will crash!
*/
void fltk::pop_matrix() {m = stack[--sptr];}

/*! Multiply the current transformation by
\code
a b 0
c d 0
x y 1
\endcode
*/
void fltk::concat(float a, float b, float c, float d, float x, float y) {
  if (m.trivial) {
    m.a = a; m.b = b; m.c = c; m.d = d;
    m.x += x; m.ix = int(floorf(m.x+.5f));
    m.y += y; m.iy = int(floorf(m.y+.5f));
    m.trivial = false;
  } else {
    Matrix o;
    o.a = a*m.a + b*m.c;
    o.b = a*m.b + b*m.d;
    o.c = c*m.a + d*m.c;
    o.d = c*m.b + d*m.d;
    o.x = x*m.a + y*m.c + m.x; o.ix = int(floorf(o.x+.5f));
    o.y = x*m.b + y*m.d + m.y; o.iy = int(floorf(o.y+.5f));
    o.trivial = false;
    m = o;
  }
}

/*! Scale the current transformation by multiplying it by
\code
x 0 0
0 y 0
0 0 1
\endcode
*/
void fltk::scale(float x,float y) {
  if (x != 1.0 && y != 1.0) concat(x,0,0,y,0,0);
}

/*! Scale the current transformation by multiplying it by
\code
x 0 0
0 x 0
0 0 1
\endcode
*/
void fltk::scale(float x) {
  if (x != 1.0) concat(x,0,0,x,0,0);
}

/*! Translate the current transformation by multiplying it by
\code
1 0 0
0 1 0
x y 1
\endcode
*/
void fltk::translate(float x,float y) {
  if (m.trivial) {
    m.x += x; m.ix = int(floorf(m.x+.5f));
    m.y += y; m.iy = int(floorf(m.y+.5f));
    m.trivial = m.ix==m.x && m.iy==m.y;
  } else {
    concat(1,0,0,1,x,y);
  }
}

/*!
  This integer version is provided because it is much faster than the
  floating-point version. However C++ will not "resolve" which one you
  want to call if you try passing doubles as arguments. To get it to
  compile, make sure you cast the arguments to float (add 'f' after
  floating-point constants). Use the 'f' versions (ie sinf(), cosf(),
  etc) of the math functions from <fltk/math.h> to produce floats and
  get maximum calculation speed.
*/
void fltk::translate(int x, int y) {
  if (m.trivial) {
    m.ix += x; m.x = float(m.ix);
    m.iy += y; m.y = float(m.iy);
  } else {
    concat(1,0,0,1,float(x),float(y));
  }
}

/*!
  Rotate the current transformation counter-clockwise by \a d degrees
  (not radians!!). This is done by multiplying the matrix by:
\code
cos -sin 0
sin  cos 0
0     0  1
\endcode
*/
void fltk::rotate(float d) {
  if (d) {
    float s, c;
    if (d == 0) {s = 0; c = 1;}
    else if (d == 90) {s = 1; c = 0;}
    else if (d == 180) {s = 0; c = -1;}
    else if (d == 270 || d == -90) {s = -1; c = 0;}
    else {s = sinf(d*float(M_PI/180)); c = cosf(d*float(M_PI/180));}
    concat(c,-s,s,c,0,0);
  }
}

/*!
  Replace the current transform with the identity transform, which
  puts 0,0 in the top-left corner of the window and each unit is 1
  pixel in size.
*/
void fltk::load_identity() {
  m.a = 1; m.b = 0; m.c = 0; m.d = 1;
  m.x = 0; m.y = 0;
  m.ix = 0; m.iy = 0;
  m.trivial = true;
}

////////////////////////////////////////////////////////////////

/*! Replace \a x and \a y with the transformed coordinates. */
void fltk::transform(float& x, float& y) {
  if (!m.trivial) {
    float t = x*m.a + y*m.c + m.x;
    y = x*m.b + y*m.d + m.y;
    x = t;
  } else {
    x += m.x;
    y += m.y;
  }
}

/*!
  Replace x and y with the tranformed coordinates, ignoring
  translation. This transforms a vector which is measuring a distance
  between two positions, rather than a position.
*/
void fltk::transform_distance(float& x, float& y) {
  if (!m.trivial) {
    float t = x*m.a + y*m.c;
    y = x*m.b + y*m.d;
    x = t;
  }
}

/*! Replace x and y with the transformed coordinates, rounded to the
  nearest integer. */
void fltk::transform(int& x, int& y) {
  if (!m.trivial) {
    int t = int(floorf(x*m.a + y*m.c + m.x + .5f));
    y = int(floorf(x*m.b + y*m.d + m.y + .5f));
    x = t;
  } else {
    x += m.ix;
    y += m.iy;
  }
}

/*! Replace x and y with the transformed vector, rounded to the
  nearest integer. */
void fltk::transform_distance(int& x, int& y) {
  if (!m.trivial) {
    int t = int(floorf(x*m.a + y*m.c + .5f));
    y = int(floorf(x*m.b + y*m.d + .5f));
    x = t;
  }
}

////////////////////////////////////////////////////////////////
/** \}
    \defgroup path Path Construction
    \ingroup drawing

  These functions let you draw arbitrary shapes with 2-D linear
  transformations. The functionality matches that found in Adobe®
  PostScript<sup>TM</sup>. On both X and WIN32 the transformed vertices are
  rounded to integers before drawing the line segments, this severely
  limits the accuracy of these functions for complex graphics, so use
  OpenGL when greater accuracy and/or performance is required.

  Unlike PostScript<sup>TM</sup> the path is cleared after you
  draw it. Instead fltk provides operators that do multiple operations
  on the same path, such as fillstrokepath().

  \{
*/

// typedef what the x,y fields in a point are:
#if USE_X11
typedef short COORD_T;
#elif defined(_WIN32)
typedef int COORD_T;
typedef POINT XPoint;
#else
typedef int COORD_T;
struct XPoint { int x, y; };
#endif

// Storage of the current path:
static XPoint *xpoint; // all the points
static int point_array_size;
static int numpoints; // number of points
static int loop_start; // point at start of current loop
static int* loop; // number of points in each loop
static int loops; // number of loops
static int loop_array_size;

static void add_n_points(int n) {
  point_array_size = point_array_size ? 2*point_array_size : 16;
  if (numpoints+n >= point_array_size) point_array_size = n;
  xpoint = (XPoint*)realloc((void*)xpoint,(point_array_size+1)*sizeof(XPoint));
}

/*!
  Add a single vertex to the current path. (If you are familiar
  with PostScript, this does a "moveto" if the path is clear or
  fltk::closepath was done last, otherwise it does a "lineto").
*/
void fltk::addvertex(float X, float Y) {
  COORD_T x = COORD_T(floorf(X*m.a + Y*m.c + m.x + .5f));
  COORD_T y = COORD_T(floorf(X*m.b + Y*m.d + m.y + .5f));
  if (!numpoints || x != xpoint[numpoints-1].x || y != xpoint[numpoints-1].y) {
    if (numpoints+1 >= point_array_size) add_n_points(1);
    xpoint[numpoints].x = x;
    xpoint[numpoints].y = y;
    numpoints++;
  }
}

/*!
  This integer version is provided because it is much faster than the
  floating-point version. However C++ will not "resolve" which one you
  want to call if you try passing doubles as arguments. To get it to
  compile, make sure you cast the arguments to float (add 'f' after
  floating-point constants). Use the 'f' versions (ie sinf(), cosf(),
  etc) of the math functions from <fltk/math.h> to produce floats and
  get maximum calculation speed.
*/
void fltk::addvertex(int X, int Y) {
  COORD_T x,y;
  if (m.trivial) {
    x = COORD_T(X+m.ix);
    y = COORD_T(Y+m.iy);
  } else {
    x = COORD_T(floorf(X*m.a + Y*m.c + m.x + .5f));
    y = COORD_T(floorf(X*m.b + Y*m.d + m.y + .5f));
  }
  if (!numpoints || x != xpoint[numpoints-1].x || y != xpoint[numpoints-1].y) {
    if (numpoints+1 >= point_array_size) add_n_points(1);
    xpoint[numpoints].x = x;
    xpoint[numpoints].y = y;
    numpoints++;
  }
}

/*!
  Add a whole set of vertices to the current path. This is much faster
  than calling fltk::addvertex once for each point.
*/
void fltk::addvertices(int n, const float array[][2]) {
  if (numpoints+n >= point_array_size) add_n_points(n);
  const float* a = array[0];
  const float* e = a+2*n;
  int pn = numpoints;
  if (m.trivial) {
    for (; a < e; a += 2) {
      COORD_T x = COORD_T(floorf(a[0] + m.x + .5f));
      COORD_T y = COORD_T(floorf(a[1] + m.y + .5f));
      if (!pn || x != xpoint[pn-1].x || y != xpoint[pn-1].y) {
	xpoint[pn].x = x;
	xpoint[pn].y = y;
	pn++;
      }
    }
  } else {
    for (; a < e; a += 2) {
      COORD_T x = COORD_T(floorf(a[0]*m.a + a[1]*m.c + m.x + .5f));
      COORD_T y = COORD_T(floorf(a[0]*m.b + a[1]*m.d + m.y + .5f));
      if (!pn || x != xpoint[pn-1].x || y != xpoint[pn-1].y) {
	xpoint[pn].x = x;
	xpoint[pn].y = y;
	pn++;
      }
    }
  }
  numpoints = pn;
}

/*! Add a whole set of integer vertices to the current path. */
void fltk::addvertices(int n, const int array[][2]) {
  if (numpoints+n >= point_array_size) add_n_points(n);
  const int* a = array[0];
  const int* e = a+2*n;
  int pn = numpoints;
  if (m.trivial) {
    for (; a < e; a += 2) {
      COORD_T x = COORD_T(a[0]+m.ix);
      COORD_T y = COORD_T(a[1]+m.iy);
      if (!pn || x != xpoint[pn-1].x || y != xpoint[pn-1].y) {
	xpoint[pn].x = x;
	xpoint[pn].y = y;
	pn++;
      }
    }
  } else {
    for (; a < e; a += 2) {
      COORD_T x = COORD_T(floorf(a[0]*m.a + a[1]*m.c + m.x + .5f));
      COORD_T y = COORD_T(floorf(a[0]*m.b + a[1]*m.d + m.y + .5f));
      if (!pn || x != xpoint[pn-1].x || y != xpoint[pn-1].y) {
	xpoint[pn].x = x;
	xpoint[pn].y = y;
	pn++;
      }
    }
  }
  numpoints = pn;
}

/*! Adds a whole set of vertcies that have been produced from values
  returned by fltk::transform(). This is how curve() and arc() are
  implemented.
*/
void fltk::addvertices_transformed(int n, const float array[][2]) {
  if (numpoints+n >= point_array_size) add_n_points(n);
  const float* a = array[0];
  const float* e = a+2*n;
  int pn = numpoints;
  for (; a < e; a += 2) {
    COORD_T x = COORD_T(floorf(a[0] + .5f));
    COORD_T y = COORD_T(floorf(a[1] + .5f));
    if (!pn || x != xpoint[pn-1].x || y != xpoint[pn-1].y) {
      xpoint[pn].x = x;
      xpoint[pn].y = y;
      pn++;
    }
  }
  numpoints = pn;
}

/*!
  Similar to drawing another vertex back at the starting point, but
  fltk knows the path is closed. The next fltk::vertex will start a
  new disconnected part of the shape.

  It is harmless to call fltk::closepath() several times in a row, or
  to call it before the first point. Sections with less than 3 points
  in them will not draw anything when filled.
*/
void fltk::closepath() {
  if (numpoints > loop_start+2) {
    // close the shape by duplicating first point:
    XPoint& q = xpoint[loop_start];
    // the array always has one extra point so we don't need to check
    XPoint& p = xpoint[numpoints-1];
    if (p.x != q.x || p.y != q.y) xpoint[numpoints++] = q;
    // remember the new loop:
    if (loops >= loop_array_size) {
      loop_array_size = loop_array_size ? 2*loop_array_size : 16;
      loop = (int*)realloc((void*)loop, loop_array_size*sizeof(int));
    }
    loop[loops++] = numpoints-loop_start;
    loop_start = numpoints;
  } else {
    numpoints = loop_start;
  }
}

////////////////////////////////////////////////////////////////
// Enormous kludge to add arcs to a path but try to take advantage
// of primitive arc-drawing functions provided by the OS. This mess
// should not be needed on newer systems...
//
// We keep track of exactly one "nice" circle:

static int circle_x, circle_y, circle_w, circle_h;

/*!
  Add a circle to the path. It is always a circle, irregardless of the
  transform. The center is at x,y and the radius is r (r is not transformed
  by the current transformation).

  This tries to take advantage of the primitive calls provided by
  Xlib and GDI32. Currently you can only draw one circle per path.
*/
void fltk::addcircle(float x, float y, float r) {
  transform(x,y);
  float rt = r * sqrtf(fabsf(m.a*m.d-m.b*m.c));
  circle_w = circle_h = int(rt*2 + .5);
  circle_x = int(floorf(x - circle_w*.5f + .5f));
  circle_y = int(floorf(y - circle_h*.5f + .5f));
}

/*!
  Add a closed ellipse to the path, inscribed in the x,y,w,h rectangle.

  This tries to take advantage of the primitive calls provided by
  Xlib and GDI32. Currently you can only draw one ellipse per path,
  and if \a w and \a h are unequal it only draws the correct thing
  for 90 degree rotations and reflections.
*/
void fltk::addellipse(float x, float y, float w, float h) {
#if 1
  // Use X/Win32 drawing functions as best we can. Only works for 90
  // degree rotations:
  x += w/2;
  y += h/2;
  transform(x,y);
  float d1x,d1y; d1x = w; d1y = 0; transform_distance(d1x, d1y);
  float d2x,d2y; d2x = 0; d2y = h; transform_distance(d2x, d2y);
  float rx = sqrtf(d1x*d1x+d2x*d2x)/2;
  float ry = sqrtf(d1y*d1y+d2y*d2y)/2;
  circle_w = int(rx*2 + .5f);
  circle_x = int(floorf(x - circle_w*.5f + .5f));
  circle_h = int(ry*2 + .5f);
  circle_y = int(floorf(y - circle_h*.5f + .5f));
#else
  // This produces the correct image, but not as nice as using circles
  // produced by the server:
  closepath();
  add_arc(x, y, w, h, 0, 360);
  closepath();
#endif
}

static inline void inline_newpath() {
  numpoints = loop_start = loops = circle_w = 0;
}
/*! Clear the current "path". This is normally done by fltk::fillpath() or
  any other drawing command. */
void fltk::newpath() {inline_newpath();}

////////////////////////////////////////////////////////////////

/*!
  Draw a point (one pixel) for every vertex in the path, then clear the path.
  In theory the line_style() should affect how big the points are, but I
  don't think that works on X.
*/
void fltk::drawpoints() {
#if USE_X11
  if (numpoints > 0) XDrawPoints(xdisplay, xwindow, gc, xpoint, numpoints, 0);
#elif defined(_WIN32)
  for (int i=0; i<numpoints; i++)
    SetPixel(dc, xpoint[i].x, xpoint[i].y, current_xpixel);
#elif defined(__APPLE__)
  for (int i=0; i<numpoints; i++) {
    MoveTo(xpoint[i].x, xpoint[i].y); Line(0, 0);
  } 
#endif
  inline_newpath();
}

/*!
  Draw a line between all the points in the path (see
  fltk::line_style() for ways to set the thicknesss and dot pattern of
  the line), then clear the path.
*/
void fltk::strokepath() {
#if USE_X11
  if (circle_w > 0)
    XDrawArc(xdisplay, xwindow, gc,
	     circle_x, circle_y, circle_w, circle_h, 0, 360*64);
  int loop_start = 0;
  for (int n = 0; n < loops; n++) {
    int loop_size = loop[n];
    XDrawLines(xdisplay, xwindow, gc, xpoint+loop_start, loop_size, 0);
    loop_start += loop_size;
  }
  int loop_size = numpoints-loop_start;
  if (loop_size > 1)
    XDrawLines(xdisplay, xwindow, gc, xpoint+loop_start, loop_size, 0);
#elif defined(_WIN32)
  setpen();
  if (circle_w > 0)
    Arc(dc, circle_x, circle_y, circle_x+circle_w+1, circle_y+circle_h+1,
	0,0, 0,0);
  int loop_start = 0;
  for (int n = 0; n < loops; n++) {
    int loop_size = loop[n];
    Polyline(dc, xpoint+loop_start, loop_size);
    loop_start += loop_size;
  }
  int loop_size = numpoints-loop_start;
  if (loop_size > 1)
    Polyline(dc, xpoint+loop_start, loop_size);
#elif defined(__APPLE__)
  if (circle_w > 0) {
    Rect r; r.left=circle_x; r.right=circle_x+circle_w+1;
    r.top=circle_y; r.bottom=circle_y+circle_h+1;
    FrameArc(&r, 0, 360);
  }
  int i = 0;
  for (int n = 0; ; n++) {
    int loop_end;
    if (n < loops) loop_end = i+loop[n];
    else if (n == loops && numpoints > i+1) loop_end = numpoints;
    else break;
    MoveTo(xpoint[i].x, xpoint[i].y);
    while (++i < loop_end) LineTo(xpoint[i].x, xpoint[i].y);
  }
#endif
  inline_newpath();
}

/*!
  Does fltk::closepath() and then fill with the current color, and
  then clear the path.

  For portability, you should only draw polygons that appear the same
  whether "even/odd" or "non-zero" winding rules are used to fill
  them. This mostly means that holes should be drawn in the opposite
  direction of the outside.

  Warning: result is somewhat different on X and Win32! Use
  fillstrokepath() to make matching shapes. In my opinion X is correct,
  we may change the Win32 version to match in the future, perhaps by
  making the current pen invisible?
*/
void fltk::fillpath() {
#if USE_X11
  if (circle_w > 0)
    XFillArc(xdisplay, xwindow, gc,
	     circle_x, circle_y, circle_w, circle_h, 0, 64*360);
  if (loops) closepath();
  if (numpoints > 2) {
    if (loops > 2) {
      // back-trace the lines between each "disconnected" part so they
      // are actually connected:
      if (numpoints+loops-2 >= point_array_size) add_n_points(loops-2);
      int n = numpoints-1;
      for (int i = loops; --i > 1;) {
	n -= loop[i];
	xpoint[numpoints++] = xpoint[n];
      }
    }
    XFillPolygon(xdisplay, xwindow, gc, xpoint, numpoints, 0, 0);
  }
#elif defined(_WIN32)
  setbrush();
  setpen();
  if (circle_w > 0)
    Chord(dc, circle_x, circle_y, circle_x+circle_w+1, circle_y+circle_h+1,
	  0,0, 0,0);
  if (loops) {
    closepath();
    PolyPolygon(dc, xpoint, loop, loops);
  } else if (numpoints > 2) {
    Polygon(dc, xpoint, numpoints);
  }
#elif defined(__APPLE__)
  if (circle_w > 0) {
    Rect r; r.left=circle_x; r.right=circle_x+circle_w+1;
    r.top=circle_y; r.bottom=circle_y+circle_h+1;
    PaintArc(&r, 0, 360);
  }
  PolyHandle ph = OpenPoly();
  int i = 0;
  for (int n = 0; ; n++) {
    int loop_end;
    if (n < loops) loop_end = i+loop[n];
    else if (n == loops && numpoints > i+1) loop_end = numpoints;
    else break;
    MoveTo(xpoint[i].x, xpoint[i].y);
    while (++i < loop_end) LineTo(xpoint[i].x, xpoint[i].y);
    ClosePoly();
  }
  PaintPoly(ph);
  KillPoly(ph);
#endif
  inline_newpath();
}

/*!
  Does fltk::fill(), then sets the current color to linecolor and does
  fltk::stroke with the same closed path, and then clears the path.

  This seems to produce very similar results on X and Win32. Also it
  takes advantage of a single GDI32 call that does this and should
  be faster.
*/
void fltk::fillstrokepath(Color color) {
#if USE_X11
  if (circle_w > 0)
    XFillArc(xdisplay, xwindow, gc,
	     circle_x, circle_y, circle_w, circle_h, 0, 64*360);
  closepath();
  if (numpoints > 2) {
    int saved_points = numpoints;
    if (loops > 2) {
      // back-trace the lines between each "disconnected" part so they
      // are actually connected:
      if (numpoints+loops-2 >= point_array_size) add_n_points(loops-2);
      int n = saved_points-1;
      for (int i = loops; --i > 1;) {
	n -= loop[i];
	xpoint[numpoints++] = xpoint[n];
      }
    }
    XFillPolygon(xdisplay, xwindow, gc, xpoint, numpoints, 0, 0);
    numpoints = saved_points; // throw away the extra points
  }
  setcolor(color);
  strokepath();
#elif defined(_WIN32)
  setbrush();
  setcolor(color);
  setpen();
  if (circle_w > 0)
    Chord(dc, circle_x, circle_y, circle_x+circle_w+1, circle_y+circle_h+1,
	  0,0, 0,0);
  if (loops) {
    closepath();
    PolyPolygon(dc, xpoint, loop, loops);
  } else if (numpoints > 2) {
    Polygon(dc, xpoint, numpoints);
  }
  inline_newpath();
#elif defined(__APPLE__)
  if (circle_w > 0) {
    Rect r; r.left=circle_x; r.right=circle_x+circle_w+1;
    r.top=circle_y; r.bottom=circle_y+circle_h+1;
    PaintArc(&r, 0, 360);
  }
  PolyHandle ph = OpenPoly();
  int i = 0;
  for (int n = 0; ; n++) {
    int loop_end;
    if (n < loops) loop_end = i+loop[n];
    else if (n == loops && numpoints > i+1) loop_end = numpoints;
    else break;
    MoveTo(xpoint[i].x, xpoint[i].y);
    while (++i < loop_end) LineTo(xpoint[i].x, xpoint[i].y);
    ClosePoly();
  }
  PaintPoly(ph);
  KillPoly(ph);
  setcolor(color);
  strokepath();
#endif
}

/** \} */

//
// End of "$Id: fl_vertex.cxx,v 1.28 2004/06/09 05:38:58 spitzak Exp $".
//
