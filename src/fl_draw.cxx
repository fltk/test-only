//
// "$Id: fl_draw.cxx,v 1.15 2001/07/23 09:50:05 spitzak Exp $"
//
// Label drawing code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

// Implementation of fl_draw(const char*,int,int,int,int,Fl_Align)
// Used to draw all the labels and text, this routine:

// Breaks them into lines at \n characters.

// Splits it at every \t tab character and uses fl_column_widths() to
// set each section into a column.

// Parses '&x' combinations to produce MicroSoft style underscores,
// unless FL_RAW_LABEL flag is set.

// Word wraps the labels to fit into their column (if FL_ALIGN_WRAP
// flag is on) and aligns them agains the inside of their boxes.

#include <fltk/fl_draw.h>
#include <string.h>

#define MAXBUF 1024

const int* fl_column_widths_ = 0;

// The implementation splits the text up into individual calls to fl_draw.
// Each has an x/y position and a segment of text. Probably should be
// expanded to have a font & size, This will allow
// much more complex drawing than we currently support, such as imbedded
// words in different fonts.
struct Segment {
  const char* start;
  const char* end; // points after last character
  int x,y;
};

static int max_x;

// Create a new segment:
static /*inline*/ void set(Segment* s,
		  const char* start,
		  const char* end,
		  int width,
		  int x, int y, int w,
		  Fl_Flags flags
		  )
{
  s->start = start;
  s->end = end;
  if (x+width > max_x) max_x = x+width;
  if (flags & FL_ALIGN_RIGHT) {
    s->x = x+w-width;
    if (flags & FL_ALIGN_LEFT && s->x > x) s->x = x;
  }
  else if (flags & FL_ALIGN_LEFT) s->x = x;
  else s->x = x+w/2-width/2;
  s->y = y+fl_height()-fl_descent();
}

// word-wrap a section of text into one segment per line:
// Returns the y of the last line
static int wrap(
  const char* start,
  const char* end,
  int x, int y, int w,
  Fl_Flags flags,
  Segment*& segment,
  Segment* last_segment // prevent array overflow
  )
{
  int width = 0;
  if (flags & FL_ALIGN_WRAP) {
    const char* word_start = start;
    const char* word_end = start;
    for (const char* p = start; ; p++) {
      if (p >= end || *p == ' ') {
	// test for word-wrap:
	if (word_start < p) {
	  int newwidth = width + fl_width(word_end, p-word_end);
	  if (word_end > start && newwidth > w) { // break before this word
	    set(segment++, start, word_end, width, x, y, w, flags);
	    y += fl_height();
	    if (segment >= last_segment) return y; // quit on array overflow
	    start = word_end = p = word_start;
	    width = 0;
	    continue;
	  } else {
	    width = newwidth;
	    word_end = p;
	  }
	}
	word_start = p+1;
	if (p >= end) break;
      }
    }
  } else {
    width = fl_width(start, end-start);
  }
  if (start < end) set(segment++, start, end, width, x, y, w, flags);
  return y;
}

bool fl_hide_shortcut; // set by Fl_Choice

// Parses and lays out the text into segments. Return value is the
// y height of the text. The width is stored in max_x.
static int split(
    const char* str,
    int W, int /*H*/,
    Fl_Flags flags,
    Segment*& segment,
    Segment* last_segment,
    char* tempbuf // for the underscore stuff...
    )
{
  const int* column = fl_column_widths_;

  bool look_for_underscore = !(flags & FL_RAW_LABEL);
  bool saw_underscore = false;

  int x = 0;
  max_x = 0;
  int y = 0;
  int max_y = 0;
  const char* p = str;
  for (;;) {
    // find the next newline or tab:
    int w; // width to format this segment into
    if (!*p || *p == '\n') {
      w = W-x;
    } else if (*p == '\t') {
      if (column && *column) w = *column++;
      else w = ((p-str+8)&-8)*fl_width('2');
    } else {
      if (*p == '&' && look_for_underscore) saw_underscore = true;
      p++;
      continue;
    }
    int newy;
    // Edit out any '&' sign if this is reasonably short label and use
    // it's position later to underscore a letter. This is done by
    // copying the text to the tempbuf and then reusing that buffer
    // as the source for this text:
    if (saw_underscore && p-str < 128) {
      look_for_underscore = false;
      char* a = tempbuf;
      const char* b = str;
      const char* underscore_at = 0;
      while (b < p) {
	if (*b == '&' && b+1 < p) {
	  b++;
	  if (*b != '&' && !fl_hide_shortcut && !underscore_at)
	    underscore_at = a;
	}
	*a++ = *b++;
      }
      Segment* s = segment;
      newy = wrap(tempbuf, a, x, y, w, flags, segment, last_segment);
      // add something to print the underscore:
      if (underscore_at) for (; s < segment; s++) {
	if (underscore_at >= s->start && underscore_at < s->end) {
	  Segment* t = segment++;
	  t->start = "_"; t->end = t->start+1;
	  t->x = s->x+fl_width(s->start,underscore_at-s->start);
	  t->y = s->y;
	  break;
	}
      }
    } else {
      newy = wrap(str, p, x, y, w, flags, segment, last_segment);
    }
    if (newy > max_y) max_y = newy;
    if (!*p) {
      return max_y+fl_height();
    } else if (*p == '\n') {
      x = 0; y = max_y+fl_height(); max_y = y; column = fl_column_widths_;
    } else { // tab
      x += w;
    }
    str = ++p;
  }
}

#define MAXSEGMENTS 50

void fl_draw(
    const char* str,	// the (multi-line) string
    int X, int Y, int W, int H,	// bounding box
    Fl_Flags flags
) {
  if (!str || !*str) return;
  Segment segments[MAXSEGMENTS];
  Segment* segment = segments;
  char tempbuf[128];
  int h = split(str, W, H, flags, segment, segments+MAXSEGMENTS-1, tempbuf);
  int dy;
  if (flags & FL_ALIGN_BOTTOM) {
    dy = Y+H-h;
    if ((flags & FL_ALIGN_TOP) && dy > Y) dy = Y;
  } else if (flags & FL_ALIGN_TOP) {
    dy = Y;
  } else {
    dy = Y+(H-h)/2;
  }
  for (Segment* s = segments; s < segment; s++) {
    fl_draw(s->start, s->end-s->start, s->x+X, s->y+dy);
  }
}

void fl_measure(const char* str, int& w, int& h, Fl_Flags flags) {
  if (!str || !*str) {w = 0; h = fl_height(); return;}
  Segment segments[MAXSEGMENTS];
  Segment* segment = segments;
  char tempbuf[128];
  h = split(str, w, h, flags, segment, segments+MAXSEGMENTS-1, tempbuf);
  w = max_x;
}

// back-compatable one:
//  void fl_measure(const char* str, int& w, int& h) {
//    fl_measure(str, w, h, w ? FL_ALIGN_WRAP : 0);
//  }

//
// End of "$Id: fl_draw.cxx,v 1.15 2001/07/23 09:50:05 spitzak Exp $".
//
