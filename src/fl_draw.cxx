//
// "$Id: fl_draw.cxx,v 1.38 2004/02/17 07:46:02 spitzak Exp $"
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

/*! \addtogroup symbols
  "@n" in a label resets the font, color, and postioning offset to
  the settings they were when the label started. */
class NormalSymbol : public Symbol {
public:
  NormalSymbol() : Symbol("n") {}
  void _draw(float x, float y, float w, float h, const Style*, Flags) const {
    setcolor(normal_color);
    setfont(normal_font, normal_size);
  }
  void _measure(float& w, float& h) const {
    setfont(normal_font, normal_size);
    ::dx = ::dy = 0;
    w = 0; h = normal_size;
  }
};
static const NormalSymbol normalsymbol;

/*! \addtogroup symbols
  "@b" in a label changes the font to bold. */
class BoldSymbol : public Symbol {
public:
  BoldSymbol() : Symbol("b") {}
  void _draw(float x, float y, float w, float h, const Style*, Flags) const {
    setfont(getfont()->bold(), h);
  }
  void _measure(float& w, float& h) const {
    _draw(0,0,w,h,0,0);
    w = 0;
  }
};
static const BoldSymbol boldsymbol;

/*! \addtogroup symbols
  "@i" in a label changes the font to italic. */
class ItalicSymbol : public Symbol {
public:
  ItalicSymbol() : Symbol("i") {}
  void _draw(float x, float y, float w, float h, const Style*, Flags) const {
    setfont(getfont()->italic(), h);
  }
  void _measure(float& w, float& h) const {
    _draw(0,0,w,h,0,0);
    w = 0;
  }
};
static const ItalicSymbol italicsymbol;

/*! \addtogroup symbols
  "@f" or "@t" in a label changes the font to fixed fltk::COURIER */
class FixedSymbol : public Symbol {
public:
  FixedSymbol(const char* n) : Symbol(n) {}
  void _draw(float x, float y, float w, float h, const Style*, Flags) const {
    setfont(fltk::COURIER, h);
  }
  void _measure(float& w, float& h) const {
    _draw(0,0,w,h,0,0);
    w = 0;
  }
};
static const FixedSymbol fixedsymbolf("f");
static const FixedSymbol fixedsymbolt("t");

/*! \addtogroup symbols
  "@Cxyz" will change the color to xyz, which is a color number. It
  is easiest to specify it in hex, such as 0xff000000 for red. */
class ColorSymbol : public Symbol {
public:
  ColorSymbol() : Symbol("C") {}
  void _draw(float x, float y, float w, float h, const Style*, Flags) const {
    if (!fl_drawing_shadow) setcolor((Color)strtoul(text()+1,0,0));
  }
  void _measure(float& w, float& h) const {
    w = 0;
  }
};
static const ColorSymbol colorsymbol;

/*! \addtogroup symbols
  "@snumber" will set the font size directly to the number.

  "@s+number" will set it to (12+n)/12 times the current size.

  "@s-number" will set it to 12/(12+n) times the current size.

  "@s0" will restore the initial size.

  Capital S works as well for back-compatability.
*/
class SizeSymbol : public Symbol {
public:
  SizeSymbol(const char* n) : Symbol(n) {}
  void _draw(float x, float y, float w, float h, const Style*, Flags) const {
    float n = float(strtod(text()+1,0));
    if (n < 0) n = h*12/(12-n);
    else if (*(text()+1)=='+') n = h*(12+n)/12;
    else if (n <= 0) n = normal_size;
    setfont(getfont(), n);
  }
  void _measure(float& w, float& h) const {
    _draw(0,0,w,h,0,0);
    w = 0;
  }
};
static const SizeSymbol sizesymbols("s");
static const SizeSymbol sizesymbolS("S");

/*! \addtogroup symbols
  "@." does nothing. This is for back-compatability with fltk1.1,
  but in that it caused all further @-commands to be ignored. This
  is not implemented, all @-commands are interpreted unless you
  set RAW_LABEL in the flags pased to fltk::draw().
*/
class NothingSymbol : public Symbol {
public:
  NothingSymbol() : Symbol(".") {}
  void _draw(float x, float y, float w, float h, const Style*, Flags) const {}
  void _measure(float& w, float& h) const {w = h = 0;}
};
static const NothingSymbol nothingsymbol;

/*! \addtogroup symbols

  "@xnumber" will move the x origin to the right by n*(fontsize/12) pixels.

  This is ignored in width calculations, but can be used to adjust
  the position of a label to line up exactly. You should start
  positive numbers with '+' for compatability with possible
  future versions of fltk.
*/
class DxSymbol : public Symbol {
public:
  DxSymbol() : Symbol("x") {}
  void _draw(float x, float y, float w, float h, const Style*, Flags) const {}
  void _measure(float& w, float& h) const {
    ::dx += strtod(text()+1,0)*h/12;
    w = 0;
  }
};
static const DxSymbol dxsymbol;

/*! \addtogroup symbols
  "@ynumber" will move the y origin \e up (not down) by n*(fontsize/12) pixels.

  This is ignored in width calculations, but can be used to adjust
  the position of a label to line up exactly. You should start
  positive numbers with '+' for compatability with possible
  future versions of fltk.
*/
class DySymbol : public Symbol {
public:
  DySymbol() : Symbol("y") {}
  void _draw(float x, float y, float w, float h, const Style*, Flags) const {}
  void _measure(float& w, float& h) const {
    ::dy -= strtod(text()+1,0)*h/12;
    w = 0;
  }
};
static const DySymbol dysymbol;

static Color bgboxcolor;

/*! \addtogroup symbols
  "@Bcolor" draws a solid box of the given color behind the text. Not
  sure if this is compatable with fltk1.1. */
class BgBox : public Symbol {
public:
  BgBox() : Symbol("B") {}
  void _draw(float x, float y, float w, float h, const Style*, Flags) const {}
  void _measure(float& w, float& h) const {
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
  float x,y,w,h; // Postion of left end of baseline
};

// The current set of segments. Call add() to add a new one:
static Segment* segments;
static int segment_count;
static int segment_array_size;

// Create and append a new segment:
static /*inline*/ void add(const Symbol* symbol,
			   const char* start,
			   const char* end,
			   float x, float y, float w, float h
			   )
{
  // enlarge the array if necessary:
  if (segment_count >= segment_array_size) {
    segment_array_size = segment_count ? 2*segment_count : 32;
    Segment* new_array = new Segment[segment_array_size];
    memcpy(new_array, segments, segment_count*sizeof(Segment));
    delete[] segments;
    segments = new_array;
  }
  Segment& s = segments[segment_count++];
  s.symbol = symbol;
  s.start = start;
  s.end = end;
  s.x = x+dx;
  s.y = y+dy;
  s.w = w;
  s.h = h;
}

// As we build the segments we keep track of the furthest-right edge:
static float max_x;

// Move a line horizontally for alignment:
static void align(int first_segment, float x, float w, float r, Flags flags) {
  if (r > max_x) max_x = r;
  float dx; 
  if (flags & ALIGN_RIGHT) {
    dx = w-(r-x);
    if ((flags & ALIGN_LEFT) && dx > 0) return;
  } else if (flags & ALIGN_LEFT) {
    return;
  } else {
    dx = (w-(r-x))/2;
  }
  for (int i = first_segment; i < segment_count; i++)
    segments[i].x += dx;
}

bool fl_hide_shortcut; // set by Choice

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
  int first_segment = segment_count;

  for (const char* p = start; ;) {
    // figure out what we have next:
    const Symbol* symbol = 0;
    bool underscore = false;
    const char* q = p;

    // stop only on "interesting" characters:
  SKIP_LETTERS:
    if (p >= end) ;
    else if (*p==' ' && (flags&ALIGN_WRAP)) {
      // spaces, find the end of all the sequential ones:
      q = p+1; while (q<end && *q==' ') q++;
    } else if (*p=='@' && !(flags&RAW_LABEL) && p+1<end) {
      q = p+1;
      if (*q == '@') q++;
      else {
	while (q<end && *q && !isspace(*q) && *q!='@' && *q!=';') q++;
	symbol = Symbol::find(p+1,q);
	if (!symbol) {++p; goto SKIP_LETTERS;}
      }
    } else if (*p=='&' && !(flags&RAW_LABEL) && p+1<end) {
      q = p+1;
      if (*q == '&') q++;
      else {
	if (isalnum(*q)) underscore = true;
	else {++p; goto SKIP_LETTERS;}
      }
    } else {
      ++p; goto SKIP_LETTERS;
    }

    // Wrap the current block of text:
    if (flags & ALIGN_WRAP) {
      float newwidth = width + getwidth(word_end, p-word_end);
      if (x+newwidth+(symbol?getsize():0) > ix+w && word_start > start) {
	// break before this word
	if (word_end > start) {
	  add(0, start, word_end, x, y+line_ascent,
	      width, getsize());
	  x += width;
	}
	align(first_segment, ix, w, x, flags);
	y += line_spacing;
	x = ix;
	width = 0;
	start = word_end = word_start;
	first_segment = segment_count;
	// back up and start formatting from start of new line:
	if (word_start < p) {p = word_start+1; continue;}
      } else {
	width = newwidth;
	word_end = p;
      }
    }
    // spaces are just added to the current block:
    if (*p == ' ') {p = word_start = q; continue;}

    // add text before this next object:
    if (start < p) {
      width += getwidth(word_end, p-word_end);
      add(0, start, p, x, y+line_ascent, width, getsize());
      x += width;
    }
    if (p >= end) break;
    width = 0;
    if (underscore) {
      if (!fl_hide_shortcut) {
	const char* us = "_";
	add(0, us, us+1, x, y+line_ascent, getsize(),getsize());
      }
      p = q;
    } else if (symbol) {
      Symbol::text(p+1);
      float W,H; W = H = getsize(); symbol->measure(W,H);
      Symbol::text("");
      add(symbol, p+1, q, x,
	  // center it's height vertically about the font center:
	  y+line_ascent-(getascent()-getdescent()+H+1)/2, W, H);
      x += W;
      // skip the terminating space or semicolon:
      if (q < end && *q==';') q++;
      p = q;
    } else {
      // handle @@ and &&
      p++; q = p+1;
    }
    // start the next block of text:
    word_start = word_end = start = p; p = q;
  }
  align(first_segment, ix, w, x, flags);
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
  line_spacing = int(getsize() + Widget::default_style->leading() + .5);
  line_ascent = (line_spacing + getascent() - getdescent() - 1) / 2;
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
    // add symbol to reset fonts to normal:
    //float a,b; normalsymbol._measure(a,b);
    //add(segment_count, &normalsymbol, 0, 0, 0, 0);
    str = ++p;
  }
}

static void _drawtext(
    const char* str,	// the (multi-line) string
    int X, int Y, int W, int H,	// bounding box
    Flags flags
) {
  if (!str || !*str) return;
  bgboxcolor = 0;
  normal_color = getcolor();
  int h = int(split(str, W, H, flags)+.5);
  int dy;
  if (flags & ALIGN_BOTTOM) {
    dy = Y+H-h;
    if ((flags & ALIGN_TOP) && dy > Y) dy = Y;
  } else if (flags & ALIGN_TOP) {
    dy = Y;
  } else {
    dy = Y+((H-h)>>1);
  }
  setfont(normal_font, normal_size);
  push_matrix();
  load_identity();
  if (bgboxcolor) {
    if (!fl_drawing_shadow) setcolor(bgboxcolor);
    int w = int(max_x+.5);
    int dx = 0;
    if (flags & ALIGN_RIGHT) {
      dx = W-w;
      if (flags & ALIGN_LEFT) if (dx > 0) dx = 0;
    } else if (!(flags & ALIGN_LEFT)) {
      dx = (W-w)>>1;
    }
    fillrect(X+dx, int(dy), w, h);
    if (fl_drawing_shadow) goto DONE;
    setcolor(normal_color);
  }
  for (h = 0; h < segment_count; h++) {
    Segment& s = segments[h];
    if (s.symbol) {
      Symbol::text(s.start);
      s.symbol->draw(s.x+X, s.y+dy, s.w, s.h, Widget::default_style, 0);
    } else {
      drawtext_transformed(s.start, s.end-s.start, s.x+X, s.y+dy);
    }
  }
  Symbol::text("");
 DONE:
  pop_matrix();
  setfont(normal_font, normal_size);
  setcolor(normal_color);
}

/*! \addtogroup font
  \{ */

/*!
  This is the fancy string-drawing function that is used to draw all
  labels in fltk. The string is formatted and aligned inside the
  passed box. Only X and Y are transformed, W and H are assummed to
  be in the space used to select the font (the float version of this
  function transforms all 4 coordinates). This also:
  - Splits it at every \\t tab character and uses column_widths() to
    set each section into a column.
  - Breaks the text into lines at \\n characters. Word-wraps (if
    flags has fltk::ALIGN_WRAP set) so the words fit in the
    columns.
  - Looks up "@xyz;" sequeces to see if they are a Symbol, if so it
    prints that symbol instead. This is skipped if the flags has
    fltk::RAW_LABEL set.
  - Parses "&x" combinations to produce Microsoft style underscores,
    unless RAW_LABEL flag is set.
*/
void fltk::drawtext(
    const char* str,	// the (multi-line) string
    int X, int Y, int W, int H,	// bounding box
    Flags flags
) {
  if (!str || !*str) return; // speeds up very common widgets
  transform(X,Y);
  _drawtext(str, X,Y,W,H, flags);
}

/*! Format text into a box in the current coordinate space.
  Warning: unpredictable effects if the current coordianates are
  rotated from the ones in use when the font was chosen.
*/
void fltk::drawtext(
    const char* str,
    float X, float Y, float W, float H,
    Flags flags
) {
  transform(X,Y);
  transform_distance(W,H);
  _drawtext(str,
	    int(floorf(X)+.5), int(floorf(Y)+.5), int(W+.5), int(H+.5),
	    flags);
}

/*!
  Measure the size of box necessary for drawtext() to draw the
  given string inside of it. \a w must be preset to the width
  you want to wrap at if fltk::ALIGN_WRAP is on in the flags!
  \a w and \a h are changed to the size of the resulting box.
*/
void fltk::measure(const char* str, int& w, int& h, Flags flags) {
  if (!str || !*str) {w = 0; h = int(getsize()+.5); return;}
  h = int(split(str, w, h, flags)+.5);
  w = int(max_x+.5);
  setfont(normal_font, normal_size);
}

//
// End of "$Id: fl_draw.cxx,v 1.38 2004/02/17 07:46:02 spitzak Exp $".
//
