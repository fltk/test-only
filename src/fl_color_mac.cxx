//
// "$Id$"
//
// MacOS color functions for the Fast Light Tool Kit (FLTK).
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

// we need to remember some settings for the current context

namespace fltk {

float quartz_line_width_ = 1.0f;
static enum CGLineCap quartz_line_cap_ = kCGLineCapButt;
static enum CGLineJoin quartz_line_join_ = kCGLineJoinMiter;
static float *quartz_line_pattern = 0;
static int quartz_line_pattern_size = 0;

void restore_quartz_line_style() {
  CGContextSetLineWidth(quartz_gc, quartz_line_width_);
  CGContextSetLineCap(quartz_gc, quartz_line_cap_);
  CGContextSetLineJoin(quartz_gc, quartz_line_join_);
  CGContextSetLineDash(quartz_gc, 0, 
      quartz_line_pattern, quartz_line_pattern_size);
}

} // end of namespace

// Because carbon has a 'current color' in the drawing context this
// is really simple.

Color fltk::current_color_;

void fltk::setcolor(Color i) {
  current_color_ = i;
  // get fltk indexed color:
  if (!(i & 0xFFFFFF00)) i = cmap[i];
  // get the individual colors and put into Mac color structure:
  if (!quartz_gc) return; // no context yet? We will assign the color later.
  uchar r, g, b;
  r = i>>24;
  g = i>>16;
  b = i>> 8;
  float fr = r/255.0f;
  float fg = g/255.0f;
  float fb = b/255.0f;
  CGContextSetRGBFillColor(quartz_gc, fr, fg, fb, 1.0f);
  CGContextSetRGBStrokeColor(quartz_gc, fr, fg, fb, 1.0f);
}

// Used by setcolor_index
static void free_color(Color) {}

static enum CGLineCap Cap[4] = {
  kCGLineCapButt, kCGLineCapButt, kCGLineCapRound, kCGLineCapSquare 
};

static enum CGLineJoin Join[4] = {
  kCGLineJoinMiter, kCGLineJoinMiter, kCGLineJoinRound, kCGLineJoinBevel 
};

void fltk::line_style(int style, int width, char* dashes) {
  if (width<1) width = 1;
  quartz_line_width_ = (float)width;
  quartz_line_cap_ = Cap[(style>>8)&3];
  quartz_line_join_ = Join[(style>>12)&3];
  char *d = dashes;
  static float pattern[16];
  if (d && *d) {
    float *p = pattern;
    while (*d) { *p++ = (float)*d++; }
    quartz_line_pattern = pattern;
    quartz_line_pattern_size = d-dashes;
  } else if (style & 0xff) {
    char dash, dot, gap;
    // adjust lengths to account for cap:
    if (style & 0x200) {
      dash = char(2*width);
      dot = 1;
      gap = char(2*width-1);
    } else {
      dash = char(3*width);
      dot = gap = char(width);
    }
    float *p = pattern;
    switch (style & 0xff) {
    case DASH:       *p++ = dash; *p++ = gap; break;
    case DOT:        *p++ = dot; *p++ = gap; break;
    case DASHDOT:    *p++ = dash; *p++ = gap; *p++ = dot; *p++ = gap; break;
    case DASHDOTDOT: *p++ = dash; *p++ = gap; *p++ = dot; *p++ = gap; 
                     *p++ = dot; *p++ = gap; break;
    }
    quartz_line_pattern_size = p-pattern;
    quartz_line_pattern = pattern;
  } else {
    quartz_line_pattern = 0; 
    quartz_line_pattern_size = 0;
  }
  restore_quartz_line_style();
}

//
// End of "$Id$".
//
