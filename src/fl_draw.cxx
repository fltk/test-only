//
// "$Id: fl_draw.cxx,v 1.28 2003/08/04 06:55:33 spitzak Exp $"
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
#include <fltk/Font.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
using namespace fltk;

// These are for getting the default leading:
#include <fltk/Widget.h>
#include <fltk/Style.h>

////////////////////////////////////////////////////////////////
// @-sign commands

#include <fltk/Symbol.h>

static Font* normal_font;
static float normal_size;
static Color normal_color;
static int line_spacing;
static float line_ascent;
static float dx, dy;
bool fl_drawing_shadow; // true for engraved labels

/** "@n" in a label resets the font, color, and postioning offset to
    the settings they were when the label started. */
class NormalSymbol : public Symbol {
public:
  NormalSymbol() : Symbol("n") {}
  void draw(float x, float y, float w, float h, Flags) const {
    setcolor(normal_color);
    setfont(normal_font, normal_size);
  }
  void measure(float& w, float& h) const {
    setfont(normal_font, normal_size);
    dx = dy = 0;
    w = h = 0;
  }
};
static const NormalSymbol normalsymbol;

/** "@b" in a label changes the font to bold. */
class BoldSymbol : public Symbol {
public:
  BoldSymbol() : Symbol("b") {}
  void draw(float x, float y, float w, float h, Flags=0) const {
    setfont(getfont()->bold(), h);
  }
  void measure(float& w, float& h) const {
    draw(0,0,w,h);
    w = h = 0;
  }
};
static const BoldSymbol boldsymbol;

/** "@i" in a label changes the font to italic. */
class ItalicSymbol : public Symbol {
public:
  ItalicSymbol() : Symbol("i") {}
  void draw(float x, float y, float w, float h, Flags=0) const {
    setfont(getfont()->italic(), h);
  }
  void measure(float& w, float& h) const {
    draw(0,0,w,h);
    w = h = 0;
  }
};
static const ItalicSymbol italicsymbol;

/** "@f" or "@t" in a label changes the font to fixed fltk::COURIER */
class FixedSymbol : public Symbol {
public:
  FixedSymbol(const char* n) : Symbol(n) {}
  void draw(float x, float y, float w, float h, Flags=0) const {
    setfont(fltk::COURIER, h);
  }
  void measure(float& w, float& h) const {
    draw(0,0,w,h);
    w = h = 0;
  }
};
static const FixedSymbol fixedsymbolf("f");
static const FixedSymbol fixedsymbolt("t");

/** "@Cxyz" will change the color to xyz, which is a color number. It
    is easiest to specify it in hex, such as 0xff000000 for red. */
class ColorSymbol : public Symbol {
public:
  ColorSymbol() : Symbol("C") {}
  void draw(float x, float y, float w, float h, Flags) const {
    if (!fl_drawing_shadow) setcolor((Color)strtoul(text()+1,0,0));
  }
  void measure(float& w, float& h) const {
    w = h = 0;
  }
};
static const ColorSymbol colorsymbol;

/** "@snumber" will set the font size directly to the number.
    "@s+number" will set it to (12+n)/12 times the current size.
    "@s-number" will set it to 12/(12+n) times the current size.
    "@s0" will restore the initial size.
    Capital S works as well for back-compatability.
*/
class SizeSymbol : public Symbol {
public:
  SizeSymbol(const char* n) : Symbol(n) {}
  void draw(float x, float y, float w, float h, Flags=0) const {
    float n = float(strtod(text()+1,0));
    if (n < 0) n = h*12/(12-n);
    else if (*(text()+1)=='+') n = h*(12+n)/12;
    else if (n <= 0) n = normal_size;
    setfont(getfont(), n);
  }
  void measure(float& w, float& h) const {
    draw(0,0,w,h);
    w = h = 0;
  }
};
static const SizeSymbol sizesymbols("s");
static const SizeSymbol sizesymbolS("S");

/** "@." does nothing. This is for back-compatability with fltk1.1,
    but in that it caused all further @-commands to be ignored. This
    is not implemented, all @-commands are interpreted unless you
    set RAW_LABEL in the flags pased to fltk::draw().
*/
class NothingSymbol : public Symbol {
public:
  NothingSymbol() : Symbol(".") {}
  void draw(float x, float y, float w, float h, Flags=0) const {}
  void measure(float& w, float& h) const {w = h = 0;}
};
static const NothingSymbol nothingsymbol;

/** "@xnumber" will move the x origin to the right by
    n*(fontsize/12) pixels.
    This is ignored in width calculations, but can be used to adjust
    the position of a label to line up exactly. You should start
    positive numbers with '+' for compatability with possible
    future versions of fltk.
*/
class DxSymbol : public Symbol {
public:
  DxSymbol() : Symbol("x") {}
  void draw(float x, float y, float w, float h, Flags) const {}
  void measure(float& w, float& h) const {
    dx += strtod(text()+1,0)*h/12;
    w = h = 0;
  }
};
static const DxSymbol dxsymbol;

/** "@ynumber" will move the y origin \e up (not down) by
    n*(fontsize/12) pixels.
    This is ignored in width calculations, but can be used to adjust
    the position of a label to line up exactly. You should start
    positive numbers with '+' for compatability with possible
    future versions of fltk.
*/
class DySymbol : public Symbol {
public:
  DySymbol() : Symbol("y") {}
  void draw(float x, float y, float w, float h, Flags) const {}
  void measure(float& w, float& h) const {
    dy -= strtod(text()+1,0)*h/12;
    w = h = 0;
  }
};
static const DySymbol dysymbol;

static Color bgboxcolor;

/** "@Bcolor" draws a solid box of the given color behind the text. Not
    sure if this is compatable with fltk1.1. */
class BgBox : public Symbol {
public:
  BgBox() : Symbol("B") {}
  void draw(float x, float y, float w, float h, Flags) const {}
  void measure(float& w, float& h) const {
    bgboxcolor = (Color)strtoul(text()+1,0,0);
    if (!bgboxcolor) bgboxcolor = BLACK;
    w = h = 0;
  }
};
static const BgBox bgbox;

/* Other symbols supported in fltk1.1:

   @l = set size to 24
   @m = set size to 18
   @s = set size to 11 // could be emulated when no number
   @c = center
   @r = right-justify
   @Fnumber = set to fltk font n
   @N = set to grayed-out color
   @- = draw an inset divider line
   @u or @_ = underscore rest of text

*/
////////////////////////////////////////////////////////////////

const int* fltk::column_widths_ = 0;

// The implementation splits the text up into individual "symbols"
// Each has an x/y position and a segment of text and a pointer to
// a Symbol object that uses that information to draw it. These calls
// may also have side effects like changing the color or font.
struct Segment {
  const Symbol* symbol;
  const char* start;
  const char* end; // points after last character
  float x,y; // Postion of left end of baseline
};

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
  s.x = x+dx;
  s.y = y+dy;
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

  for (const char* p = start; ;) {
    float guesswidth = 0;
    // stop only on "interesting" characters:
    if (p >= end) ;
    else if ((flags&ALIGN_WRAP) && *p == ' ') {
      // don't split in the middle of sequential spaces:
      if (word_start >= p) {word_start = ++p; continue;}
    } else if (!(flags&RAW_LABEL) && (*p=='@' || *p=='&')) {
      guesswidth=getsize();
    } else {++p; continue;}

    // see if we want to split the previous text between words:
    if (flags & ALIGN_WRAP) {
      float newwidth = width + getwidth(word_end, p-word_end);
      if (x+newwidth+guesswidth > ix+w && word_start > start) {
	// break before this word
	if (word_end > start) {
	  add(segment_count, 0, start, word_end, x, y+line_ascent);
	  if (x+width > max_x+ix) max_x = x+width-ix;
	}
	y += line_spacing;
	x = ix;
	width = 0;
	start = word_end = word_start;
	// back up and start formatting from start of new line:
	if (word_start < p) {p = word_start+1; continue;}
      } else {
	width = newwidth;
	word_end = p;
      }
    }
    if (*p == ' ') {word_start = ++p; continue;}
    if (start < p) {
      // add text before this next object
      width += getwidth(word_end, p-word_end);
      add(segment_count, 0, start, p, x, y+line_ascent);
      x += width;
      if (x > max_x+ix) max_x = x-ix;
    }
    if (p >= end) break;
    width = 0;
    if (*p == '&') {
      p++;
      if (*p == '&') {
	word_start = word_end = start = p; p++; continue;
      }
      // add an underscore under next letter:
      if (!fl_hide_shortcut) {
	const char* us = "_";
	add(segment_count, 0, us, us+1, x, y+line_ascent);
      }
    } else if (*p == '@') {
      p++;
      if (*p == '@') {
	word_start = word_end = start = p; p++; continue;
      }
      // parse out an @-command:
      const char* q = p;
      while (*q && !isspace(*q) && *q != '@' && *q != ';') q++;
      const Symbol* s = Symbol::find(p,q);
      if (!s) {
	// try a 1-character symbol for back-compatability
	//q = p+1; s = Symbol::find(p,p+1);
	//if (!s) {
	  // try to ignore non-matching symbols
	  word_start = word_end = start = p-1; continue;
	  //}
      }
      add(segment_count, s, p, q, x, y+line_ascent);
      Symbol::text(p);
      float W,H; W = H = getsize(); s->measure(W,H); x += W;
      Symbol::text("");
      if (x > max_x+ix) max_x = x-ix;
      p = q;
      // skip the terminating space or semicolon:
      if (isspace(*p) || *p==';') p++;
    }
    // start the next block of text:
    word_start = word_end = start = p;
  }
  return y;
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
  normal_font = getfont();
  normal_size = getsize();
  line_spacing = int(getsize() + Widget::default_style->leading + .5);
  line_ascent = (line_spacing + getascent() - getdescent()) / 2;
  dx = dy = 0;
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
      return max_y+line_spacing;
    } else if (*p == '\n') {
      x = 0; y = max_y+line_spacing; max_y = y; column = column_widths_;
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
  bgboxcolor = 0;
  normal_color = getcolor();
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
  setfont(normal_font, normal_size);
  push_matrix();
  load_identity();
  if (bgboxcolor) {
    if (!fl_drawing_shadow) setcolor(bgboxcolor);
    fillrect(int(dx), int(dy), int(max_x+.5), h);
    if (fl_drawing_shadow) goto DONE;
    setcolor(normal_color);
  }
  for (h = 0; h < segment_count; h++) {
    Segment& s = segments[h];
    if (s.symbol) {
      Symbol::text(s.start);
      s.symbol->draw(s.x+dx, s.y+dy-(getascent()-getdescent()+getsize())/2,
		     getsize(), getsize());
    } else {
      drawtext_transformed(s.start, s.end-s.start, s.x+dx, s.y+dy);
    }
  }
  Symbol::text("");
 DONE:
  pop_matrix();
  setfont(normal_font, normal_size);
  setcolor(normal_color);
}

void fltk::measure(const char* str, int& w, int& h, Flags flags) {
  if (!str || !*str) {w = 0; h = int(getsize()+.5); return;}
  h = int(split(str, w, h, flags)+.5);
  w = int(max_x+.5);
  setfont(normal_font, normal_size);
}

//
// End of "$Id: fl_draw.cxx,v 1.28 2003/08/04 06:55:33 spitzak Exp $".
//
