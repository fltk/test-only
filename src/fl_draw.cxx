//
// "$Id: fl_draw.cxx,v 1.26 2003/06/30 07:55:17 spitzak Exp $"
//
// Label drawing code for the Fast Light Tool Kit (FLTK).
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

// Implementation of draw(const char*,int,int,int,int,Align)
// Used to draw all the labels and text, this routine:

// Breaks them into lines at \n characters.

// Splits it at every \t tab character and uses column_widths() to
// set each section into a column.

// Parses '&x' combinations to produce MicroSoft style underscores,
// unless RAW_LABEL flag is set.

// Word wraps the labels to fit into their column (if ALIGN_WRAP
// flag is on) and aligns them agains the inside of their boxes.

#include <fltk/draw.h>
#include <fltk/math.h>
#include <string.h>
#include <ctype.h>
using namespace fltk;

// These are for getting the default leading:
#include <fltk/Widget.h>
#include <fltk/Style.h>

static int line_spacing() {
  return int(getsize() + Widget::default_style->leading + .5);
}
static float line_ascent() {
  return (line_spacing() + getascent() - getdescent()) / 2;
}

namespace fltk {
class Symbol {
  public:
    virtual void draw(const char* start,
		      const char* end,
		      float x, float y) const = 0;
    virtual void measure(const char* start,
		      const char* end,
		      float& w, float& h) const = 0;
};
}

const int* fltk::column_widths_ = 0;

// The implementation splits the text up into individual "symbols"
// Each has an x/y position and a segment of text and a pointer to
// a Symbol object that uses that information to draw it. These calls
// may also have side effects like changing the color or font.
struct Segment {
  const Symbol* symbol;
  const char* start;
  const char* end; // points after last character
  float x,y;
};

// Sequences of characters that are not interpreted are turned into
// this "symbol" which draws the text in the current font:
class PlainText : public Symbol {
public:
  void draw(const char* start, const char* end, float x, float y) const {
    drawtext_transformed(start, end-start, x, y+line_ascent());
  }
  void measure(const char* start, const char* end, float& w, float& h) const {
    w = getwidth(start, end-start);
    h = getsize();
  }
};
static const PlainText plaintext;

namespace fltk {void load_identity();}

// Dummy standin for symbols:
class SymbolSymbol : public Symbol {
public:
  void draw(const char* start, const char* end, float x, float y) const {
    // copy the name to zero-terminate it:
    char buf[10];
    int i = 0;
    for (const char* q = start; q < end && i < 9;) buf[i++] = *q++;
    buf[i] = 0;
    int n = line_spacing();
    draw_symbol(buf, int(x), int(y), n, n, getcolor());
  }
  void measure(const char* start, const char* end, float& w, float& h) const {
    w = h = line_spacing();
  }
};
static const SymbolSymbol symbol;

// The current set of segments. Call add() to add a new one:
static Segment* segments;
static int segment_count;
static int segment_array_size;

// Create a new segment and insert it at index:
static /*inline*/ void add(int index,
			   const Symbol* symbol,
			   const char* start,
			   const char* end,
			   float x, float y
			   )
{
  // enlarge the array if necessary:
  if (segment_count >= segment_array_size) {
    segment_array_size = segment_count ? 2*segment_count : 32;
    Segment* new_array = new Segment[segment_array_size];
    memcpy(new_array, segments, index*sizeof(Segment));
    memcpy(new_array+index+1, segments+index, (segment_count-index)*sizeof(Segment));
    delete[] segments;
    segments = new_array;
  } else {
    memmove(segments+index+1, segments+index, (segment_count-index)*sizeof(Segment));
  }
  segment_count++;
  Segment& s = segments[index];
  s.symbol = symbol;
  s.start = start;
  s.end = end;
  s.x = x;
  s.y = y;
}

bool fl_hide_shortcut; // set by Choice

// As we build the segments we keep track of the furthest-right edge:
static float max_x;

// Find all the segments in a section of raw text and arrange them as
// though they are aligned with the top-left corner at x,y and wrap at
// w.  This is complex because a stream of letters may turn into 1 or
// 2 segments due to word wrapping.  Returns the y of the last
// line. Sets max_x to the rightmost edge
static float wrap(
  const char* start,
  const char* end,
  float ix, float y, float w,
  Flags flags
  )
{
  float x = ix; // accumulated position if we don't wrap

  const char* word_end = start; // where to end it if we wrap
  const char* word_start = start; // where to start new segment
  float width = 0; 		// width of current text
  // start..p indicates current text segment being built

  for (const char* p = start; ;p++) {
    if ((flags & ALIGN_WRAP) && (p >= end || *p == ' ')) {
      if (word_start < p) {
	float newwidth = width + getwidth(word_end, p-word_end);
	if ((flags & ALIGN_WRAP) && word_end > start && x+newwidth > ix+w) {
	  // break before thsi word
	  add(segment_count, &plaintext, start, word_end, x, y);
	  if (x+width > max_x+ix) max_x = x+width-ix;
	  y += line_spacing();
	  start = word_end = p = word_start;
	  x = ix;
	  width = 0;
	  continue;
	} else {
	  width = newwidth;
	  word_end = p;
	}
      }
      word_start = p+1;
    }
    if (p < end && ((flags & RAW_LABEL) || (*p != '&' && *p != '@'))) continue;
    if (start < p) {
      // add text before this next object
      width += getwidth(word_end, p-word_end);
      add(segment_count, &plaintext, start, p, x, y);
      x += width;
      if (x > max_x+ix) max_x = x-ix;
    }
    for (;;) {
      if (p >= end) return y;
      if (*p == '&') {
	p++;
	if (*p == '&') break;
	// add an underscore under next letter:
	if (!fl_hide_shortcut) {
	  const char* us = "_";
	  add(segment_count, &plaintext, us, us+1, x, y);
	}
      } else if (*p == '@') {
	p++;
	if (*p == '@') break;
	// parse out an @-command:
	const char* q = p;
	if (*q == '#') q++; // optional leading '#'
	if (*q == '+' || *q == '-') q++;
	while (isdigit(*q)) q++; // leading numbers
	while (q < end && !isdigit(*q) && *q != '@' && *q != ' ') q++;
	while (isdigit(*q)) q++; // trailing numbers
	add(segment_count, &symbol, p, q, x, y);
	float W,H; symbol.measure(p,q,W,H); x += W;
	if (x > max_x+ix) max_x = x-ix;
	p = q;
	if (p < end && *p == ' ') p++; // ignore trailing space
      } else break;
    }
    // start the next block of text:
    word_start = word_end = start = p; width = 0;
  }
}

// Split at newlines and tabs into sections and then call wrap on them
// Return value is the y height of the text. The width is stored in
// max_x.
static float split(
    const char* str,
    int W, int /*H*/,
    Flags flags
    )
{
  const int* column = column_widths_;

  segment_count = 0;
  float x = 0;
  max_x = 0;
  float y = 0;
  float max_y = 0;
  const char* p = str;
  for (;;) {
    // find the next newline or tab:
    float w; // width to format this segment into
    if (!*p || *p == '\n') {
      w = W-x;
    } else if (*p == '\t') {
      if (column && *column) w = *column++;
      else w = ((p-str+8)&-8)*getwidth("2",1);
    } else {
      p++;
      continue;
    }
    float newy;
    newy = wrap(str, p, x, y, w, flags);
    if (newy > max_y) max_y = newy;
    if (!*p) {
      return max_y+line_spacing();
    } else if (*p == '\n') {
      x = 0; y = max_y+line_spacing(); max_y = y; column = column_widths_;
    } else { // tab
      x += w;
    }
    str = ++p;
  }
}

void fltk::drawtext(
    const char* str,	// the (multi-line) string
    int X, int Y, int W, int H,	// bounding box
    Flags flags
) {
  if (!str || !*str) return;
  int h = int(split(str, W, H, flags)+.5);
  transform(X,Y);
  int dx, dy;
  if (flags & ALIGN_BOTTOM) {
    dy = Y+H-h;
    if ((flags & ALIGN_TOP) && dy > Y) dy = Y;
  } else if (flags & ALIGN_TOP) {
    dy = Y;
  } else {
    dy = Y+((H-h)>>1);
  }
  if (flags & ALIGN_RIGHT) {
    dx = X+W-int(max_x+.5);
    if ((flags & ALIGN_LEFT) && dx > X) dx = X;
  } else if (flags & ALIGN_LEFT) {
    dx = X;
  } else {
    dx = X+((W-int(max_x+.5))>>1);
  }
  push_matrix();
  load_identity();
  for (int i = 0; i < segment_count; i++) {
    Segment& s = segments[i];
    s.symbol->draw(s.start, s.end, s.x+dx, s.y+dy);
  }
  pop_matrix();
}

void fltk::measure(const char* str, int& w, int& h, Flags flags) {
  if (!str || !*str) {w = 0; h = int(getsize()+.5); return;}
  h = int(split(str, w, h, flags)+.5);
  w = int(max_x+.5);
}

//
// End of "$Id: fl_draw.cxx,v 1.26 2003/06/30 07:55:17 spitzak Exp $".
//
