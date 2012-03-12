//
// "$Id: draw.h 8621 2011-04-23 15:46:30Z AlbrechtS $"
//
// Portable drawing function header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/**
 \file draw.h
 \brief utility header to pull drawing functions together
 */

#ifndef fltk3_draw_H
#define fltk3_draw_H

#include <fltk3/x.h>	      // for fltk3::Region
#include <fltk3/enumerations.h>  // for the color names
#include <fltk3/Window.h>     // for fltk3::set_spot()
#include <fltk3/Device.h>     // for fltk3::graphics_driver

// Image class...

namespace fltk3 {

  class Image;

  // Label flags...
  FLTK3_EXPORT extern char draw_shortcut;
  
  /** \addtogroup fl_attributes
   @{
   */
  
  // Colors:
  /**
   Sets the color for all subsequent drawing operations.
   For colormapped displays, a color cell will be allocated out of
   \p fl_colormap the first time you use a color. If the colormap fills up
   then a least-squares algorithm is used to find the closest color.
   If no valid graphical context (fl_gc) is available,
   the foreground is not set for the current window.
   \param[in] c color 
   */
  inline void color(fltk3::Color c) {fltk3::graphics_driver->color(c); } // select indexed color
  /** for back compatibility - use fltk3::color(fltk3::Color c) instead */
  inline void color(int c) {color((fltk3::Color)c);}
  /**
   Sets the color for all subsequent drawing operations.
   The closest possible match to the RGB color is used.
   The RGB color is used directly on TrueColor displays.
   For colormap visuals the nearest index in the gray
   ramp or color cube is used.
   If no valid graphical context (fl_gc) is available,
   the foreground is not set for the current window.
   \param[in] r,g,b color components
   */
  inline void color(uchar r, uchar g, uchar b) {fltk3::graphics_driver->color(r,g,b); } // select actual color
  /**
   Returns the last fltk3::color() that was set.
   This can be used for state save/restore.
   */
  inline fltk3::Color color() {return fltk3::graphics_driver->color();}
  /** @} */
  
  /** \addtogroup fl_drawings
   @{
   */
  // clip:
  
  /**
   Intersects the current clip region with a rectangle and pushes this
   new region onto the stack.
   \param[in] x,y,w,h position and size
   */
  inline void push_clip(int x, int y, int w, int h) {
    fltk3::graphics_driver->push_clip(x,y,w,h); 
  }
  
  /**
   Pushes an empty clip region onto the stack so nothing will be clipped.
   */
  inline void push_no_clip() {fltk3::graphics_driver->push_no_clip(); }
  
  /**
   Restores the previous clip region.
   
   You must call fltk3::pop_clip() once for every time you call fltk3::push_clip().
   Unpredictable results may occur if the clip stack is not empty when
   you return to FLTK.
   */
  inline void pop_clip() {fltk3::graphics_driver->pop_clip(); }
  
  /**
   Does the rectangle intersect the current clip region?
   \param[in] x,y,w,h position and size of rectangle
   \returns non-zero if any of the rectangle intersects the current clip
   region. If this returns 0 you don't have to draw the object.
   
   \note
   Under X this returns 2 if the rectangle is partially clipped, 
   and 1 if it is entirely inside the clip region.
   */
  inline int not_clipped(int x, int y, int w, int h) {return fltk3::graphics_driver->not_clipped(x,y,w,h); }
  /**
   Intersects the rectangle with the current clip region and returns the
   bounding box of the result.
   
   Returns non-zero if the resulting rectangle is different to the original.
   This can be used to limit the necessary drawing to a rectangle.
   \p W and \p H are set to zero if the rectangle is completely outside the region.
   \param[in] x,y,w,h position and size of rectangle
   \param[out] X,Y,W,H position and size of resulting bounding box.
   \returns Non-zero if the resulting rectangle is different to the original.
   */
  inline int clip_box(int x , int y, int w, int h, int& X, int& Y, int& W, int& H) 
  {return fltk3::graphics_driver->clip_box(x,y,w,h,X,Y,W,H); }
  /** Undoes any clobbering of clip done by your program */
  inline void restore_clip() { fltk3::graphics_driver->restore_clip(); }
  /**
   Replaces the top of the clipping stack with a clipping region of any shape.
   
   fltk3::Region is an operating system specific type.
   \param[in] r clipping region
   */
  inline void clip_region(fltk3::Region r) { fltk3::graphics_driver->clip_region(r); }
  /**
   Returns the current clipping region.
   */
  inline fltk3::Region clip_region() { return fltk3::graphics_driver->clip_region(); }
  
  
  // points:
  /**
   Draws a single pixel at the given coordinates
   */
  inline void point(int x, int y) { fltk3::graphics_driver->point(x,y); }
  
  
  // line type:
  /**
   Sets how to draw lines (the "pen").
   If you change this it is your responsibility to set it back to the default
   using \c fltk3::line_style(0).
   
   \param[in] style A bitmask which is a bitwise-OR of a line style, a cap
   style, and a join style. If you don't specify a dash type you
   will get a solid line. If you don't specify a cap or join type
   you will get a system-defined default of whatever value is
   fastest.
   \param[in] width The thickness of the lines in pixels. Zero results in the
   system defined default, which on both X and Windows is somewhat
   different and nicer than 1.
   \param[in] dashes A pointer to an array of dash lengths, measured in pixels.
   The first location is how long to draw a solid portion, the next
   is how long to draw the gap, then the solid, etc. It is terminated
   with a zero-length entry. A \c NULL pointer or a zero-length
   array results in a solid line. Odd array sizes are not supported
   and result in undefined behavior.
   
   \note      Because of how line styles are implemented on Win32 systems,
   you \e must set the line style \e after setting the drawing
   color. If you set the color after the line style you will lose
   the line style settings.
   \note      The \p dashes array does not work under Windows 95, 98 or Me,
   since those operating systems do not support complex line styles.
   */
  inline void line_style(int style, int width=0, char* dashes=0) {fltk3::graphics_driver->line_style(style,width,dashes); }
  enum {
    SOLID	= 0,		///< line style: <tt>___________</tt>
    DASH	= 1,		///< line style: <tt>_ _ _ _ _ _</tt>
    DOT	= 2,		///< line style: <tt>. . . . . .</tt>
    DASHDOT	= 3,		///< line style: <tt>_ . _ . _ .</tt>
    DASHDOTDOT	= 4,		///< line style: <tt>_ . . _ . .</tt>
    
    CAP_FLAT	= 0x100,	///< cap style: end is flat
    CAP_ROUND	= 0x200,	///< cap style: end is round
    CAP_SQUARE	= 0x300,	///< cap style: end wraps end point
    
    JOIN_MITER	= 0x1000,	///< join style: line join extends to a point
    JOIN_ROUND	= 0x2000,	///< join style: line join is rounded
    JOIN_BEVEL	= 0x3000	///< join style: line join is tidied
  };
  
  // rectangles tweaked to exactly fill the pixel rectangle:
  
  /** 
   Draws a 1-pixel border \e inside the given bounding box.
   This function is meant for quick drawing of simple boxes. The behavior is 
   undefined for line widths that are not 1.
   */
  inline void rect(int x, int y, int w, int h) { fltk3::graphics_driver->rect(x,y,w,h); }
  
  /** Draws with passed color a 1-pixel border \e inside the given bounding box */
  inline void rect(int x, int y, int w, int h, fltk3::Color c) {fltk3::color(c); fltk3::rect(x,y,w,h);}
  /** Colors with current color a rectangle that exactly fills the given bounding box */
  inline void rectf(int x, int y, int w, int h) { fltk3::graphics_driver->rectf(x,y,w,h); }
  /** Colors with passed color a rectangle that exactly fills the given bounding box */
  inline void rectf(int x, int y, int w, int h, fltk3::Color c) {fltk3::color(c); fltk3::rectf(x,y,w,h);}
  
  /**
   Colors a rectangle with "exactly" the passed <tt>r,g,b</tt> color.
   On screens with less than 24 bits of color this is done by drawing a
   solid-colored block using fltk3::draw_image() so that the correct color
   shade is produced.
   */
  /* note: doxygen comment here to avoid triplication in os-speciic files */
  FLTK3_EXPORT void rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b);
  
  // line segments:
  /**
   Draws a line from (x,y) to (x1,y1)
   */
  inline void line(int x, int y, int x1, int y1) {fltk3::graphics_driver->line(x,y,x1,y1); }
  /**
   Draws a line from (x,y) to (x1,y1) and another from (x1,y1) to (x2,y2)
   */
  inline void line(int x, int y, int x1, int y1, int x2, int y2) {fltk3::graphics_driver->line(x,y,x1,y1,x2,y2); }
  
  // closed line segments:
  /**
   Outlines a 3-sided polygon with lines
   */
  inline void loop(int x, int y, int x1, int y1, int x2, int y2) {fltk3::graphics_driver->loop(x,y,x1,y1,x2,y2); }
  /**
   Outlines a 4-sided polygon with lines
   */
  inline void loop(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) 
  {fltk3::graphics_driver->loop(x,y,x1,y1,x2,y2,x3,y3); }
  
  // filled polygons
  /**
   Fills a 3-sided polygon. The polygon must be convex.
   */
  inline void polygon(int x, int y, int x1, int y1, int x2, int y2) {fltk3::graphics_driver->polygon(x,y,x1,y1,x2,y2); }
  /**
   Fills a 4-sided polygon. The polygon must be convex.
   */
  inline void polygon(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) 
  { fltk3::graphics_driver->polygon(x,y,x1,y1,x2,y2,x3,y3); }
  
  // draw rectilinear lines, horizontal segment first:
  /**
   Draws a horizontal line from (x,y) to (x1,y)
   */
  inline void xyline(int x, int y, int x1) {fltk3::graphics_driver->xyline(x,y,x1);}
  /**
   Draws a horizontal line from (x,y) to (x1,y), then vertical from (x1,y) to (x1,y2)
   */
  inline void xyline(int x, int y, int x1, int y2) {fltk3::graphics_driver->xyline(x,y,x1,y2);}
  /**
   Draws a horizontal line from (x,y) to (x1,y), then a vertical from (x1,y) to (x1,y2)
   and then another horizontal from (x1,y2) to (x3,y2)
   */
  inline void xyline(int x, int y, int x1, int y2, int x3) {fltk3::graphics_driver->xyline(x,y,x1,y2,x3);}
  
  // draw rectilinear lines, vertical segment first:
  /**
   Draws a vertical line from (x,y) to (x,y1)
   */
  inline void yxline(int x, int y, int y1) {fltk3::graphics_driver->yxline(x,y,y1);}
  /**
   Draws a vertical line from (x,y) to (x,y1), then a horizontal from (x,y1) to (x2,y1)
   */
  inline void yxline(int x, int y, int y1, int x2) {fltk3::graphics_driver->yxline(x,y,y1,x2);}
  /**
   Draws a vertical line from (x,y) to (x,y1) then a horizontal from (x,y1)
   to (x2,y1), then another vertical from (x2,y1) to (x2,y3)
   */
  inline void yxline(int x, int y, int y1, int x2, int y3) {fltk3::graphics_driver->yxline(x,y,y1,x2,y3);}
  
  // circular lines and pie slices (code in fltk3::arci.C):
  /**
   Draw ellipse sections using integer coordinates.
   
   These functions match the rather limited circle drawing code provided by X
   and WIN32. The advantage over using fltk3::arc with floating point coordinates
   is that they are faster because they often use the hardware, and they draw
   much nicer small circles, since the small sizes are often hard-coded bitmaps.
   
   If a complete circle is drawn it will fit inside the passed bounding box.
   The two angles are measured in degrees counter-clockwise from 3 o'clock and
   are the starting and ending angle of the arc, \p a2 must be greater or equal
   to \p a1.
   
   fltk3::arc() draws a series of lines to approximate the arc. Notice that the
   integer version of fltk3::arc() has a different number of arguments than the
   double version fltk3::arc(double x, double y, double r, double start, double end)
   
   \param[in] x,y,w,h bounding box of complete circle
   \param[in] a1,a2 start and end angles of arc measured in degrees
   counter-clockwise from 3 o'clock. \p a2 must be greater
   than or equal to \p a1.
   */
  inline void arc(int x, int y, int w, int h, double a1, double a2) {fltk3::graphics_driver->arc(x,y,w,h,a1,a2); }
  /**
   Draw filled ellipse sections using integer coordinates.
   
   Like fltk3::arc(), but fltk3::pie() draws a filled-in pie slice.
   This slice may extend outside the line drawn by fltk3::arc();
   to avoid this use w - 1 and h - 1.
   
   \param[in] x,y,w,h bounding box of complete circle
   \param[in] a1,a2 start and end angles of arc measured in degrees
   counter-clockwise from 3 o'clock. \p a2 must be greater
   than or equal to \p a1.
   */
  inline void pie(int x, int y, int w, int h, double a1, double a2) {fltk3::graphics_driver->pie(x,y,w,h,a1,a2); }
  /** fltk3::chord declaration is a place holder - the function does not yet exist */
  FLTK3_EXPORT void chord(int x, int y, int w, int h, double a1, double a2); // nyi
  
  // scalable drawing code (code in fltk3::vertex.C and fltk3::arc.C):
  /**
   Saves the current transformation matrix on the stack. 
   The maximum depth of the stack is 32.
   */
  inline void push_matrix() { fltk3::graphics_driver->push_matrix(); }
  /**
   Restores the current transformation matrix from the stack.
   */
  inline void pop_matrix() { fltk3::graphics_driver->pop_matrix(); }
  /**
   Concatenates scaling transformation onto the current one.
   \param[in] x,y scale factors in x-direction and y-direction
   */
  inline void scale(double x, double y) { fltk3::graphics_driver->scale(x, y); }
  /**
   Concatenates scaling transformation onto the current one.
   \param[in] x scale factor in both x-direction and y-direction
   */
  inline void scale(double x) { fltk3::graphics_driver->scale(x, x); }
  /**
   Concatenates translation transformation onto the current one.
   \param[in] x,y translation factor in x-direction and y-direction
   */
  inline void translate(double x, double y) { fltk3::graphics_driver->translate(x, y); }
  /**
   Concatenates rotation transformation onto the current one.
   \param[in] d - rotation angle, counter-clockwise in degrees (not radians)
   */
  inline void rotate(double d) { fltk3::graphics_driver->rotate(d); }
  /**
   Concatenates another transformation onto the current one.
   
   \param[in] a,b,c,d,x,y transformation matrix elements such that
   <tt> X' = aX + cY + x </tt> and <tt> Y' = bX +dY + y </tt>
   */
  inline void mult_matrix(double a, double b, double c, double d, double x,double y) 
  { fltk3::graphics_driver->mult_matrix(a, b, c, d, x, y); }
  /**
   Starts drawing a list of points. Points are added to the list with fltk3::vertex()
   */
  inline void begin_points() {fltk3::graphics_driver->begin_points(); }
  /**
   Starts drawing a list of lines.
   */
  inline void begin_line() {fltk3::graphics_driver->begin_line(); }
  /**
   Starts drawing a closed sequence of lines.
   */
  inline void begin_loop() {fltk3::graphics_driver->begin_loop(); }
  
  /**
   Starts drawing a convex filled polygon.
   */
  inline void begin_polygon() {fltk3::graphics_driver->begin_polygon(); }
  /**
   Adds a single vertex to the current path.
   \param[in] x,y coordinate
   */
  inline void vertex(double x, double y) {fltk3::graphics_driver->vertex(x,y); }
  /**
   Adds a series of points on a Bezier curve to the path.
   The curve ends (and two of the points) are at X0,Y0 and X3,Y3.
   \param[in] X0,Y0 curve start point
   \param[in] X1,Y1 curve control point
   \param[in] X2,Y2 curve control point
   \param[in] X3,Y3 curve end point
   */
  inline void curve(double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3)
  {fltk3::graphics_driver->curve(X0,Y0,X1,Y1,X2,Y2,X3,Y3); }
  /**
   Adds a series of points to the current path on the arc of a circle.
   You can get elliptical paths by using scale and rotate before calling fltk3::arc().
   \param[in] x,y,r center and radius of circular arc
   \param[in] start,end angles of start and end of arc measured in degrees
   counter-clockwise from 3 o'clock. If \p end is less than \p start
   then it draws the arc in a clockwise direction.
   */
  inline void arc(double x, double y, double r, double start, double end) {fltk3::graphics_driver->arc(x,y,r,start,end); }
  /**
   fltk3::circle() is equivalent to fltk3::arc(x,y,r,0,360), but may be faster.
   
   It must be the \e only thing in the path: if you want a circle as part of
   a complex polygon you must use fltk3::arc()
   \param[in] x,y,r center and radius of circle
   */
  inline void circle(double x, double y, double r) {fltk3::graphics_driver->circle(x,y,r); }
  /**
   Ends list of points, and draws.
   */
  inline void end_points() {fltk3::graphics_driver->end_points(); }
  /**
   Ends list of lines, and draws.
   */
  inline void end_line() {fltk3::graphics_driver->end_line(); }
  /**
   Ends closed sequence of lines, and draws.
   */
  inline void end_loop() {fltk3::graphics_driver->end_loop(); }
  /**
   Ends convex filled polygon, and draws.
   */
  inline void end_polygon() {fltk3::graphics_driver->end_polygon(); }
  /**
   Starts drawing a complex filled polygon.
   
   The polygon may be concave, may have holes in it, or may be several
   disconnected pieces. Call fltk3::gap() to separate loops of the path.
   
   To outline the polygon, use fltk3::begin_loop() and replace each fltk3::gap()
   with fltk3::end_loop();fltk3::begin_loop() pairs.
   
   \note
   For portability, you should only draw polygons that appear the same
   whether "even/odd" or "non-zero" winding rules are used to fill them.
   Holes should be drawn in the opposite direction to the outside loop.
   */
  inline void begin_complex_polygon() {fltk3::graphics_driver->begin_complex_polygon(); }
  
  /**
   Call fltk3::gap() to separate loops of the path.
   
   It is unnecessary but harmless to call fltk3::gap() before the first vertex,
   after the last vertex, or several times in a row.
   */
  inline void gap() {fltk3::graphics_driver->gap(); }
  /**
   Ends complex filled polygon, and draws.
   */
  inline void end_complex_polygon() {fltk3::graphics_driver->end_complex_polygon(); }
  // get and use transformed positions:
  /**
   Transforms coordinate using the current transformation matrix.
   \param[in] x,y coordinate
   */
  inline double transform_x(double x, double y) {return fltk3::graphics_driver->transform_x(x, y); }
  /**
   Transforms coordinate using the current transformation matrix.
   \param[in] x,y coordinate
   */
  inline double transform_y(double x, double y) {return fltk3::graphics_driver->transform_y(x, y); }
  /**
   Transforms distance using current transformation matrix.
   \param[in] x,y coordinate
   */
  inline double transform_dx(double x, double y) {return fltk3::graphics_driver->transform_dx(x, y); }
  /**
   Transforms distance using current transformation matrix.
   \param[in] x,y coordinate
   */
  inline double transform_dy(double x, double y) {return fltk3::graphics_driver->transform_dy(x, y); }
  /**
   Adds coordinate pair to the vertex list without further transformations.
   \param[in] xf,yf transformed coordinate
   */
  inline void transformed_vertex(double xf, double yf) {fltk3::graphics_driver->transformed_vertex(xf,yf); }
  /** @} */
  
  /** \addtogroup  fl_attributes
   @{ */
  /* NOTE: doxygen comments here to avoid triplication in os-specific sources */
  
  // Fonts:
  /**
   Sets the current font, which is then used in various drawing routines.
   You may call this outside a draw context if necessary to call fltk3::width(),
   but on X this will open the display.
   
   The font is identified by a \p face and a \p size.
   The size of the font is measured in pixels and not "points".
   Lines should be spaced \p size pixels apart or more.
   */
  inline void font(fltk3::Font face, fltk3::Fontsize size) { fltk3::graphics_driver->font(face,size); }
  
  /**
   Returns the \p face set by the most recent call to fltk3::font().
   This can be used to save/restore the font.
   */
  inline fltk3::Font font() {return fltk3::graphics_driver->font();}
  /**
   Returns the \p size set by the most recent call to fltk3::font().
   This can be used to save/restore the font.
   */
  inline fltk3::Fontsize size() {return fltk3::graphics_driver->size();}
  
  // information you can get about the current font:
  /**
   Returns the recommended minimum line spacing for the current font.
   You can also use the value of \p size passed to fltk3::font()
   */
  inline int height() {return fltk3::graphics_driver->height();}
  FLTK3_EXPORT int height(int font, int size);
  /**
   Returns the recommended distance above the bottom of a fltk3::height() tall box to
   draw the text at so it looks centered vertically in that box.
   */
  inline int  descent() {return fltk3::graphics_driver->descent();}
  /** Returns the typographical width of a nul-terminated string */
  FLTK3_EXPORT double width(const char* txt);
  /** Returns the typographical width of a sequence of \p n characters */
  inline double width(const char* txt, int n) {return fltk3::graphics_driver->width(txt, n);}
  /** Returns the typographical width of a single character.
   \note if a valid fl_gc is NOT found then it uses the first window gc,
   or the screen gc if no fltk window is available when called. */
  inline double width(unsigned int c)  {return fltk3::graphics_driver->width(c);}
  /** Determines the minimum pixel dimensions of a nul-terminated string.
   
   Usage: given a string "txt" drawn using fltk3::draw(txt, x, y) you would determine
   its pixel extents on the display using fltk3::text_extents(txt, dx, dy, wo, ho)
   such that a bounding box that exactly fits around the text could be drawn with
   fltk3::rect(x+dx, y+dy, wo, ho). Note the dx, dy values hold the offset of the first
   "colored in" pixel of the string, from the draw origin.
   */
  FLTK3_EXPORT void text_extents(const char*, int& dx, int& dy, int& w, int& h); // NO fltk symbol expansion will be performed
  
  /** Determines the minimum pixel dimensions of a sequence of \p n characters.
   \see fltk3::text_extents(const char*, int& dx, int& dy, int& w, int& h)
   */
  inline void text_extents(const char *t, int n, int& dx, int& dy, int& w, int& h)
  {fltk3::graphics_driver->text_extents(t, n, dx, dy, w, h);}
  
  // font encoding:
  // Note: doxygen comments here to avoid duplication for os-sepecific cases
  /**
   Converts text from Windows/X11 latin1 character set to local encoding.
   \param[in] t character string (latin1 encoding)
   \param[in] n optional number of characters to convert (default is all)
   \returns pointer to internal buffer containing converted characters
   */
  FLTK3_EXPORT const char *latin1_to_local(const char *t, int n=-1);
  /**
   Converts text from local encoding to Windowx/X11 latin1 character set.
   \param[in] t character string (local encoding)
   \param[in] n optional number of characters to convert (default is all)
   \returns pointer to internal buffer containing converted characters
   */
  FLTK3_EXPORT const char *local_to_latin1(const char *t, int n=-1);
  /**
   Converts text from Mac Roman character set to local encoding.
   \param[in] t character string (Mac Roman encoding)
   \param[in] n optional number of characters to convert (default is all)
   \returns pointer to internal buffer containing converted characters
   */
  FLTK3_EXPORT const char *mac_roman_to_local(const char *t, int n=-1);
  /**
   Converts text from local encoding to Mac Roman character set.
   \param[in] t character string (local encoding)
   \param[in] n optional number of characters to convert (default is all)
   \returns pointer to internal buffer containing converted characters
   */
  FLTK3_EXPORT const char *local_to_mac_roman(const char *t, int n=-1);
  /** @} */
  
  /** \addtogroup  fl_drawings
   @{ */
  /**
   Draws a nul-terminated UTF-8 string starting at the given \p x, \p y location.
   
   Text is aligned to the left and to the baseline of the font.
   To align to the bottom, subtract fltk3::descent() from \p y.
   To align to the top, subtract fltk3::descent() and add fltk3::height().
   This version of fltk3::draw provides direct access to the text drawing
   function of the underlying OS. It does not apply any special handling
   to control characters.
   */
  FLTK3_EXPORT void draw(const char* str, int x, int y);
  /**
   Draws a nul-terminated UTF-8 string starting at the given \p x, \p y location and 
   rotating \p angle degrees counter-clockwise.
   This version of fltk3::draw provides direct access to the text drawing
   function of the underlying OS and is supported by Xft, Win32 and MacOS
   fltk subsets.
   */
  FLTK3_EXPORT void draw(int angle, const char* str, int x, int y);
  /**
   Draws starting at the given \p x, \p y location a UTF-8 string of length \p n bytes.
   */
  inline void draw(const char* str, int n, int x, int y) {fltk3::graphics_driver->draw(str,n,x,y); }
  /**
   Draws at the given \p x, \p y location a UTF-8 string of length \p n bytes 
   rotating \p angle degrees counter-clockwise.
   */
  inline void draw(int angle, const char* str, int n, int x, int y) {fltk3::graphics_driver->draw(angle,str,n,x,y); }
  
  /**
   Draws a UTF-8 string of length \p n bytes right to left starting at the given \p x, \p y location.
   */
  inline void rtl_draw(const char* str, int n, int x, int y) {fltk3::graphics_driver->rtl_draw(str,n,x,y); }
  FLTK3_EXPORT void measure(const char* str, int& x, int& y,
                            int draw_symbols = 1);
  FLTK3_EXPORT void draw(const char* str, int x, int y, int w, int h,
                         fltk3::Align align,
                         fltk3::Image* img=0, int draw_symbols = 1);
  FLTK3_EXPORT void draw(const char* str, int x, int y, int w, int h,
                         fltk3::Align align,
                         void (*callthis)(const char *,int,int,int),
                         fltk3::Image* img=0, int draw_symbols = 1);
  
  // boxtypes:
  FLTK3_EXPORT void frame(const char* s, int x, int y, int w, int h);
  FLTK3_EXPORT void frame2(const char* s, int x, int y, int w, int h);
  FLTK3_EXPORT void draw_box(fltk3::Boxtype, int x, int y, int w, int h, fltk3::Color);
  
  // images:
  
  /**
   Draws an 8-bit per color RGB or luminance image.
   \param[in] buf points at the "r" data of the top-left pixel.
   Color data must be in <tt>r,g,b</tt> order.
   Luminance data is only one <tt>gray</tt> byte.
   \param[in] X,Y position where to put top-left corner of image
   \param[in] W,H size of the image
   \param[in] D   delta to add to the pointer between pixels. It may be
   any value greater than or equal to 1, or it can be
   negative to flip the image horizontally
   \param[in] L   delta to add to the pointer between lines (if 0 is
   passed it uses \p W * \p D), and may be larger than
   \p W * \p D to crop data, or negative to flip the
   image vertically
   
   It is highly recommended that you put the following code before the
   first <tt>show()</tt> of \e any window in your program to get rid of
   the dithering if possible:
   \code
   fltk3::visual(fltk3::RGB);
   \endcode
   
   Gray scale (1-channel) images may be drawn. This is done if
   <tt>abs(D)</tt> is less than 3, or by calling fltk3::draw_image_mono().
   Only one 8-bit sample is used for each pixel, and on screens with
   different numbers of bits for red, green, and blue only gray colors
   are used. Setting \p D greater than 1 will let you display one channel
   of a color image.
   
   \par Note:
   The X version does not support all possible visuals. If FLTK cannot
   draw the image in the current visual it will abort. FLTK supports
   any visual of 8 bits or less, and all common TrueColor visuals up
   to 32 bits.
   */
  inline void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0)
  { fltk3::graphics_driver->draw_image(buf, X, Y, W, H, D, L); }
  
  /**
   Draws a gray-scale (1 channel) image.
   \see fltk3::draw_image(const uchar* buf, int X,int Y,int W,int H, int D, int L)
   */
  inline void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0)
  { fltk3::graphics_driver->draw_image_mono(buf, X, Y, W, H, D, L); }
  
  /**
   Draws an image using a callback function to generate image data.
   
   You can generate the image as it is being drawn, or do arbitrary
   decompression of stored data, provided it can be decompressed to
   individual scan lines easily.
   \param[in] cb   callback function to generate scan line data
   \param[in] data user data passed to callback function
   \param[in] X,Y  screen position of top left pixel
   \param[in] W,H  image width and height
   \param[in] D    data size in bytes (must be greater than 0)
   \see fltk3::draw_image(const uchar* buf, int X,int Y,int W,int H, int D, int L)
   
   The callback function \p cb is called with the <tt>void*</tt> \p data
   user data pointer to allow access to a structure of information about
   the image, and the \p x, \p y, and \p w of the scan line desired from
   the image. 0,0 is the upper-left corner of the image, not \p x, \p y.
   A pointer to a buffer to put the data into is passed. You must copy
   \p w pixels from scanline \p y, starting at pixel \p x, to this buffer.
   
   Due to cropping, less than the whole image may be requested. So \p x
   may be greater than zero, the first \p y may be greater than zero,
   and \p w may be less than \p W. The buffer is long enough to store
   the entire \p W * \p D pixels, this is for convenience with some
   decompression schemes where you must decompress the entire line at
   once: decompress it into the buffer, and then if \p x is not zero,
   copy the data over so the \p x'th pixel is at the start of the buffer.
   
   You can assume the \p y's will be consecutive, except the first one
   may be greater than zero.
   
   If \p D is 4 or more, you must fill in the unused bytes with zero.
   */
  inline void draw_image(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D=3)
  { fltk3::graphics_driver->draw_image(cb, data, X, Y, W, H, D); }
  
  /**
   Draws a gray-scale image using a callback function to generate image data.
   \see fltk3::draw_image(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D)
   */
  inline void draw_image_mono(fltk3::DrawImageCb cb, void* data, int X,int Y,int W,int H, int D=1)
  { fltk3::graphics_driver->draw_image_mono(cb, data, X, Y, W, H, D); }
  
  /**
   Checks whether platform supports true alpha blending for RGBA images.
   \returns 1 if true alpha blending supported by platform
   \returns 0 not supported so FLTK will use screen door transparency
   */
  /* note: doxygen comment here to avoid triplication in os-speciic files */
  FLTK3_EXPORT char can_do_alpha_blending();
  
  /**
   Reads an RGB(A) image from the current window or off-screen buffer.
   \param[in] p     pixel buffer, or NULL to allocate one
   \param[in] X,Y   position of top-left of image to read
   \param[in] W,H   width and height of image to read
   \param[in] alpha alpha value for image (0 for none)
   \returns pointer to pixel buffer, or NULL if allocation failed.
   
   The \p p argument points to a buffer that can hold the image and must
   be at least \p W*H*3 bytes when reading RGB images, or \p W*H*4 bytes
   when reading RGBA images. If NULL, fltk3::read_image() will create an
   array of the proper size which can be freed using <tt>delete[]</tt>.
   
   The \p alpha parameter controls whether an alpha channel is created
   and the value that is placed in the alpha channel. If 0, no alpha
   channel is generated.
   */
  /* note: doxygen comment here to avoid triplication in os-speciic files */
  FLTK3_EXPORT uchar *read_image(uchar *p,int X,int Y,int W,int H,int alpha=0);
  
  FLTK3_EXPORT void copy_offscreen(int x, int y, int w, int h, fltk3::Offscreen pixmap, int srcx, int srcy);

  // pixmaps:
  FLTK3_EXPORT int draw_pixmap(/*const*/ char* const* data, int x,int y,fltk3::Color=fltk3::GRAY);
  FLTK3_EXPORT int draw_pixmap(const char* const* cdata, int x,int y,fltk3::Color=fltk3::GRAY);
  FLTK3_EXPORT int measure_pixmap(/*const*/ char* const* data, int &w, int &h);
  FLTK3_EXPORT int measure_pixmap(const char* const* cdata, int &w, int &h);
  
  // other:
  FLTK3_EXPORT void scroll(int X, int Y, int W, int H, int dx, int dy,
                           void (*draw_area)(void*, int,int,int,int), void* data);
  FLTK3_EXPORT const char* shortcut_label(unsigned int shortcut);
  FLTK3_EXPORT const char* shortcut_label(unsigned int shortcut, const char **eom);
  FLTK3_EXPORT unsigned int old_shortcut(const char* s);
  FLTK3_EXPORT void overlay_rect(int x,int y,int w,int h);
  FLTK3_EXPORT void overlay_clear();
  FLTK3_EXPORT void cursor(fltk3::Cursor, fltk3::Color fg=fltk3::BLACK, fltk3::Color bg=fltk3::WHITE);
  FLTK3_EXPORT const char* expand_text(const char* from, char* buf, int maxbuf,
                                       double maxw, int& n, double &width,
                                       int wrap, int draw_symbols = 0);
  
  // XIM:
  /** \todo provide user documentation for fltk3::set_status function */
  FLTK3_EXPORT void set_status(int X, int Y, int W, int H);
  /** \todo provide user documentation for fltk3::set_spot function */
  FLTK3_EXPORT void set_spot(int font, int size, int X, int Y, int W, int H, fltk3::Window *win=0);
  /** \todo provide user documentation for fltk3::reset_spot function*/
  FLTK3_EXPORT void reset_spot(void);
  
  
  
  // XForms symbols:
  FLTK3_EXPORT int draw_symbol(const char* label,int x,int y,int w,int h, fltk3::Color);
  FLTK3_EXPORT int add_symbol(const char* name, void (*drawit)(fltk3::Color), int scalable);
  /** @} */
  
} // namespace

#endif

//
// End of "$Id: draw.h 8621 2011-04-23 15:46:30Z AlbrechtS $".
//
