//
// "$Id: Fl_Input.cxx,v 1.80 2003/04/27 01:54:52 spitzak Exp $"
//
// Input widget for the Fast Light Tool Kit (FLTK).
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

// Most useful changes of behavior can be accomplished by making a
// subclass and replacing the replace(...) function, to make a version
// that rejects changes you don't want to allow.

#include <fltk/Input.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

using namespace fltk;

#define MAXBUF 1024

////////////////////////////////////////////////////////////////

// Copy string p to the buffer, replacing control characters with ^X.
// Stop at then end of the line, or truncate if necessary so the resulting
// string and null terminator fits in a buffer of size MAXBUF.  Returns
// pointer to the character after the last one copied.
const char* Input::expand(const char* p, char* buf,int wordwrap) const {
  char* o = buf;
  char* e = buf+(MAXBUF-4);
  const char* lastspace = p;
  char* lastspace_out = o;
  int width_to_lastspace = 0;
  int word_count = 0;

  if (type() == SECRET) {
    while (o<e && p < value_+size_) {*o++ = '*'; p++;}
  } else while (o<e) {
    if (wordwrap && (p >= value_+size_ || isspace(*p))) {
      width_to_lastspace += (int)getwidth(lastspace_out, o-lastspace_out);
      if (p > lastspace+1) {
	if (word_count && width_to_lastspace > wordwrap) {
	  p = lastspace; o = lastspace_out; break;
	}
	word_count++;
      }
      lastspace = p;
      lastspace_out = o;
    }
    if (p >= value_+size_) break;
    int c = *p++;
    if (c & 0xE0) {
      *o++ = c;
    } else {
      if (c == '\n' && type() >= MULTILINE) {p--; break;}
      if (c == '\t' && type() >= MULTILINE) {
	for (c = (o-buf)%8; c<8 && o<e; c++) *o++ = ' ';
      } else {
	*o++ = '^';
	*o++ = c ^ 0x40;
      }
    }
  }
  *o = 0;
  return p;
}

// After filling in such a buffer, find the width to e:
float Input::expandpos(
  const char* p,	// real string
  const char* e,	// pointer into real string
  const char* buf,	// conversion of real string by expand()
  int* returnn		// return offset into buf here
) const {
  int n = 0;
  if (type() == SECRET) n = e-p;
  else while (p<e) {
    int c = *p++;
    if (c & 0xE0) {
      n++;
    } else {
      if (c == '\t' && type() >= MULTILINE) n += 8-(n%8);
      else n += 2;
    }
  }
  if (returnn) *returnn = n;
  return getwidth(buf, n);
}

////////////////////////////////////////////////////////////////

// minimal update:

// Characters from mu_p to end of widget are redrawn.
// If this == erase_cursor_only, small part at mu_p is redrawn.

// This is done with a clip region to work with xft. The purpose is to
// avoid blinking, not to make draw() less expensive. A few minor attempts
// to make draw() not think about clipped text are implemented, though.

static Input* erase_cursor_only;

void Input::minimal_update(int p) {
  if (erase_cursor_only == this) erase_cursor_only = 0;
  if (damage() & DAMAGE_ALL) return; // don't waste time if it won't be done
  if (damage() & DAMAGE_VALUE) {
    if (p < mu_p) mu_p = p;
  } else {
    mu_p = p;
  }
  redraw(DAMAGE_VALUE);
}

void Input::minimal_update(int p, int q) {
  if (q < p) p = q;
  minimal_update(p);
}

// Erase a cursor that was drawn at the given point and possibly draw new one:
void Input::erase_cursor_at(int p) {
  if (!damage()) {
    mu_p = p;
    redraw(DAMAGE_VALUE);
    erase_cursor_only = this;
  } else {
    minimal_update(p);
  }
}

////////////////////////////////////////////////////////////////

static float up_down_pos;
static bool was_up_down;
static Input* dnd_target;
static int dnd_target_position;

void Input::setfont() const {
  fltk::setfont(textfont(), textsize());
}

void Input::draw() {
  if (damage() & DAMAGE_ALL) draw_frame();
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  draw(X, Y, W, H);
}

static int line_spacing(float leading) {
  return int(getsize() + leading + .5);
}
static float line_ascent(float leading) {
  return (line_spacing(leading) + getascent() - getdescent()) / 2;
}

void Input::draw(int X, int Y, int W, int H)
{
  setfont();
  const float leading = this->leading();
  int height = line_spacing(leading);
  float desc = line_ascent(leading);
  if (damage() & DAMAGE_ALL) {
    // draw and measure the inside label:
    if (label() && label()[0] && (!(flags()&15)||(flags()&ALIGN_INSIDE))) {
      fltk::setfont(labelfont(), labelsize());
      float width = getwidth(label());
      label_width = int(width+getwidth(":")+2.5);
      setcolor(color());
      fillrect(X, Y, label_width, H);
      Color color = labelcolor();
      if (!active_r()) color = inactive(color);
      setcolor(color);
      float y = Y+((H-height)>>1)+desc;
      drawtext(label(), X+2, y);
      drawtext(":", X+2+width, y);
      setfont();
    } else {
      label_width = 0;
    }
  }
  X += label_width; W -= label_width;

  Color background = color();
  bool erase_cursor_only =
    this == ::erase_cursor_only &&
    !(damage() & (DAMAGE_ALL|DAMAGE_EXPOSE));

  // handle a totally blank one quickly:
  if (!size() && !focused() && this != dnd_target) {
    setcolor(background);
    fillrect(X, Y, W, H);
    return;
  }

  int selstart, selend;
  if (!focused() && !pushed())
    selstart = selend = 0;
  else if (position() <= mark()) {
    selstart = position(); selend = mark();
  } else {
    selend = position(); selstart = mark();
  }

  int wordwrap = (type() > MULTILINE) ? W-8 : 0;

  const char *p, *e;
  char buf[MAXBUF];

  // count how many lines and put the last one into the buffer:
  // And figure out where the cursor is:
  int lines;
  int curx, cury;
  int cursor_position = (this==dnd_target) ? dnd_target_position : position();
  for (p=value(), curx=cury=lines=0; ;) {
    e = expand(p, buf, wordwrap);
    if (cursor_position >= p-value() && cursor_position <= e-value()) {
      curx = int(expandpos(p, value()+cursor_position, buf, 0)+.5);
      if (focused() && !was_up_down) up_down_pos = curx;
      cury = lines*height;
      int newscroll = xscroll_;
      if (curx > newscroll+W-20) {
	// figure out scrolling so there is space after the cursor:
	newscroll = curx+20-W;
	// figure out the furthest left we ever want to scroll:
	int ex = int(expandpos(p, e, buf, 0))-W+8;
	// use minimum of both amounts:
	if (ex < newscroll) newscroll = ex;
      } else if (curx < newscroll+20) {
	newscroll = curx-20;
      }
      if (newscroll < 0) newscroll = 0;
      if (newscroll != xscroll_) {
	xscroll_ = newscroll;
	mu_p = 0;
	erase_cursor_only = false;
      }
    }
    lines++;
    if (e >= value_+size_) break;
    if (*e == '\n' || *e == ' ') e++;
    p = e;
  }

  // adjust the scrolling:
  if (type() >= MULTILINE) {
    int newy = yscroll_;
    if (cury < newy) newy = cury;
    if (cury > newy+H-height) newy = cury-H+height;
    if (newy < -1) newy = -1;
    if (newy != yscroll_) {
      yscroll_ = newy;
      mu_p = 0;
      erase_cursor_only = false;
    }
  } else {
    yscroll_ = -((H-height)>>1);
  }

  // if we are not doing minimal update a single erase is done,
  // rather than one per line:
  if (damage() & DAMAGE_ALL) {
    setcolor(background);
    fillrect(X, Y, W, H);
  }

  push_clip(X, Y, W, H);

  Color textcolor = this->textcolor();
  if (!active_r()) textcolor = inactive(textcolor);

  // I leave a small border on the edge which shrinks as the input field
  // gets smaller than 12:
  int xpos = W-9; if (xpos > 3) xpos = 3; else if (xpos < 1) xpos = 1;
  xpos += X-xscroll_;

  int ypos = -yscroll_;

  // visit each line and draw it:
  p = value();
  for (; ypos < H;) {

    // re-expand line unless it is the last one calculated above:
    if (lines>1) e = expand(p, buf, wordwrap);

    if (ypos <= -height) goto CONTINUE; // clipped off top

    if (!(damage()&DAMAGE_ALL)) {	// for minimal update:
      const char* pp = value()+mu_p; // pointer to where minimal update starts
      if (e < pp) goto CONTINUE2; // this line is before the changes
      if (erase_cursor_only && p > pp) goto CONTINUE2; // this line is after
      // calculate area to erase:
      int r = X+W;
      int x;
      if (p >= pp) {
	x = X;
	if (erase_cursor_only) r = xpos+2;
      } else {
	x = xpos+int(expandpos(p, pp, buf, 0));
	if (erase_cursor_only) r = x+2;
      }
      // clip to and erase it:
      setcolor(background);
      fillrect(x, Y+ypos, r-x, height);
      push_clip(x, Y+ypos, r-x, height);
      // it now draws entire line over it
      // this should not draw letters to left of erased area, but
      // that is nyi.
    }

    // Draw selection area if required:
    if (selstart < selend && selstart <= e-value() && selend > p-value()) {
      const char* pp = value()+selstart;
      // draw unselected text before the selection:
      float x1 = xpos;
      int offset1 = 0;
      if (pp > p) {
	setcolor(textcolor);
	x1 += expandpos(p, pp, buf, &offset1);
	drawtext(buf, offset1, xpos, Y+ypos+desc);
      }
      // draw selected text for this line:
      pp = value()+selend;
      float x2 = X+W;
      int offset2;
      if (pp <= e) x2 = xpos+expandpos(p, pp, buf, &offset2);
      else offset2 = strlen(buf);
      setcolor(selection_color());
      int xx = int(x1); fillrect(xx, Y+ypos, int(x2+.5)-xx, height);
      setcolor(selection_textcolor());
      drawtext(buf+offset1, offset2-offset1, x1, Y+ypos+desc);
      // draw unselected text after the selection:
      if (pp < e) {
	setcolor(textcolor);
	drawtext(buf+offset2, x2, Y+ypos+desc);
      }
    } else {
      // draw unselected text:
      setcolor(textcolor);
      drawtext(buf, xpos, Y+ypos+desc);
    }

    if (!(damage()&DAMAGE_ALL)) pop_clip();

  CONTINUE2:
    // draw the cursor:
    if ((this==dnd_target || focused() && selstart == selend) &&
	cursor_position >= p-value() && cursor_position <= e-value()) {
      setcolor(textcolor);
      fillrect(xpos+curx, Y+ypos, 2, height);
    }

  CONTINUE:
    ypos += height;
    if (e >= value_+size_) break;
    if (*e == '\n' || *e == ' ') e++;
    p = e;
  }

  // for minimal update, erase all lines below last one if necessary:
  if (!(damage()&DAMAGE_ALL) && type() >= MULTILINE && ypos<H
      && (!erase_cursor_only || p <= value()+mu_p)) {
    if (ypos < 0) ypos = 0;
    setcolor(background);
    fillrect(X, Y+ypos, W, H-ypos);
  }

  pop_clip();
}

static int isword(char c) {
  return (c&128 || isalnum(c) || strchr("#%&-/@\\_~", c));
}

int Input::word_end(int i) const {
  if (type() == SECRET) return size();
  while (!i || !isword(index(i-1))) i++;
  while (i < size() && isword(index(i))) i++;
  return i;
}

int Input::word_start(int i) const {
  if (type() == SECRET) return 0;
  while (!isword(index(i))) i--;
  while (i > 0 && isword(index(i-1))) i--;
  return i;
}

int Input::line_end(int i) const {
  if (type() >= WORDWRAP) {
    // go to the start of the paragraph:
    int j = i;
    while (j > 0 && index(j-1) != '\n') j--;
    // now measure lines until we get past i, end of that line is real eol:
    int wordwrap = w()-box()->dw()-6;
    setfont();
    for (const char* p=value()+j; ;) {
      char buf[MAXBUF];
      p = expand(p, buf, wordwrap);
      if (p-value() >= i) return p-value();
      p++;
    }
  } else if (type() >= MULTILINE) {
    while (i < size() && index(i) != '\n') i++;
    return i;
  } else {
    return size();
  }
}

int Input::line_start(int i) const {
  if (type() < MULTILINE) return 0;
  int j = i;
  while (j > 0 && index(j-1) != '\n') j--;
  if (type() >= WORDWRAP) {
    // now measure lines until we get past i, start of that line is real eol:
    int wordwrap = w()-box()->dw()-6;
    setfont();
    for (const char* p=value()+j; ;) {
      char buf[MAXBUF];
      const char* e = expand(p, buf, wordwrap);
      if (e-value() >= i) return p-value();
      p = e+1;
    }
  }
  return j;
}

int Input::mouse_position(int X, int Y, int W, int /*H*/) const
{
  if (!size()) return 0;

  setfont();

  // figure out what line we are pointing at:
  int theline = 0;
  if (type() >= MULTILINE) {
    theline = event_y()-Y+yscroll_;
    if (theline < 0) return 0;
    // CET - FIXME - this widget should keep track of the line heights
    // internally.  Using the style accessor functions is not guaranteed
    // to give correct results for what is actually displayed!  They
    // should _only_ be used in layout() and draw().
    // WAS: I think this is ok. Calls in response to user events can
    // assumme layout() has been called recently.
    theline /= line_spacing(leading());
  }

  int wordwrap = (type() > MULTILINE) ? W-8 : 0;

  // Step through all the lines until we reach the pointed-to line.
  // Expand the lines to printed representation into the buffer:
  const char *p, *e;
  char buf[MAXBUF];
  for (p=value();; ) {
    e = expand(p, buf, wordwrap);
    theline--; if (theline < 0) break;
    if (*e == '\n' || *e == ' ') e++;
    p = e;
    if (e >= value_+size_) break;
  }

  // Do a binary search for the character that starts before this position:
  int xpos = X-xscroll_; if (W > 12) xpos += 3;
  const char *l, *r, *t; float f0 = event_x()-xpos;
  for (l = p, r = e; l<r; ) {
    t = l+(r-l+1)/2;
    int f = xpos+int(expandpos(p, t, buf, 0)+.5);
    if (f <= event_x()) {l = t; f0 = event_x()-f;}
    else r = t-1;
  }
  // see if closer to character on the right:
  if (l < e) {
    int f1 = xpos+int(expandpos(p, l+1, buf, 0)+.5)-event_x();
    if (f1 < f0) l = l+1;
  }
  return l-value();
}

void Input::position(int p, int m) {
  was_up_down = false;
  if (p<0) p = 0;
  if (p>size()) p = size();
  if (m<0) m = 0;
  if (m>size()) m = size();
  if (p == position_ && m == mark_) return;
  if (p != m) {
    // new position is a selection:
    if (p != position_) minimal_update(position_, p);
    if (m != mark_) minimal_update(mark_, m);
  } else {
    // new position is a cursor:
    if (position_ == mark_) erase_cursor_at(position_);
    else minimal_update(position_, mark_);
  }
  position_ = p;
  mark_ = m;
}

void Input::up_down_position(int i, bool keepmark) {
  // cursor must already be at start of line!
  setfont();
  int wordwrap = type() > MULTILINE ? w()-box()->dw()-6 : 0;
  char buf[MAXBUF];
  const char* p = value()+i;
  const char* e = expand(p, buf, wordwrap);
  const char *l, *r, *t;
  for (l = p, r = e; l<r; ) {
    t = l+(r-l+1)/2;
    int f = (int)expandpos(p, t, buf, 0);
    if (f <= up_down_pos) l = t; else r = t-1;
  }
  int j = l-value();
  position(j, keepmark ? mark_ : j);
  was_up_down = true;
}

bool Input::copy(bool clipboard) {
  int b = position();
  int e = mark();
  if (b != e) {
    if (b > e) {b = mark(); e = position();}
    if (type() == SECRET) e = b;
    fltk::copy(value()+b, e-b, clipboard);
    return true;
  }
  return false;
}

#define MAXFLOATSIZE 40

static char* undobuffer;
static int undobufferlength;
static Input* undowidget;
static bool undo_is_redo;
static int undoat;	// points after insertion
static int undocut;	// number of characters deleted there
static int undoinsert;	// number of characters inserted
static int yankcut;	// length of valid contents of buffer, even if undocut=0

static void undobuffersize(int n) {
  if (n > undobufferlength) {
    if (undobuffer) {
      do {undobufferlength *= 2;} while (undobufferlength < n);
      undobuffer = (char*)realloc(undobuffer, undobufferlength);
    } else {
      undobufferlength = n+9;
      undobuffer = (char*)malloc(undobufferlength);
    }
  }
}

// all changes go through here, delete characters b-e and insert text:
bool Input::replace(int b, int e, const char* text, int ilen) {

  was_up_down = false;

  if (b<0) b = 0;
  if (e<0) e = 0;
  if (b>size_) b = size_;
  if (e>size_) e = size_;
  if (e<b) {int t=b; b=e; e=t;}
  if (e<=b && !ilen) return false; // don't clobber undo for a null operation
#if 0
  // this can be done by a subclass!
  if (size_+ilen-(e-b) > maximum_size_) {
    ilen = maximum_size_-size_+(e-b);
    if (ilen < 0) ilen = 0;
  }
#endif

  put_in_buffer(size_+ilen);

  if (undo_is_redo) {undowidget = 0; undo_is_redo = false;}

  if (e>b) {
    if (undowidget == this && b == undoat) {
      undobuffersize(undocut+(e-b));
      memcpy(undobuffer+undocut, value_+b, e-b);
      undocut += e-b;
    } else if (undowidget == this && e == undoat && !undoinsert) {
      undobuffersize(undocut+(e-b));
      memmove(undobuffer+(e-b), undobuffer, undocut);
      memcpy(undobuffer, value_+b, e-b);
      undocut += e-b;
    } else if (undowidget == this && e == undoat && (e-b)<undoinsert) {
      undoinsert -= e-b;
    } else {
      undobuffersize(e-b);
      memcpy(undobuffer, value_+b, e-b);
      undocut = e-b;
      undoinsert = 0;
    }
    memmove(buffer+b, buffer+e, size_-e+1);
    size_ -= e-b;
    undowidget = this;
    undoat = b;
    if (type() == SECRET) yankcut = 0; else yankcut = undocut;
  }

  if (ilen) {
    if (undowidget == this && b == undoat)
      undoinsert += ilen;
    else {
      undocut = 0;
      undoinsert = ilen;
    }
    memmove(buffer+b+ilen, buffer+b, size_-b+1);
    memcpy(buffer+b, text, ilen);
    size_ += ilen;
  }
  undowidget = this;
  undoat = b+ilen;

  // When editing with word wrap, it is possible to effectively turn
  // the space before the current word into a newline or back, so the
  // minimal update pointer must point at it.  This will
  // result in sub-optimal update when such wrapping does not happen
  // but it is too hard to figure out for now...
  if (type() > MULTILINE) {
    int c = b - 1;
    while (c > 0 && !isspace(index(c))) c--;
    if (c > 0) b = c;
  }

  // make sure we redraw the old selection or cursor:
  if (mark_ < b) b = mark_;
  if (position_ < b) b = position_;

  minimal_update(b);

  mark_ = position_ = undoat;

  if (when()&WHEN_CHANGED) do_callback(); else set_changed();
  return true;
}

bool Input::undo() {
  if (undowidget != this || !undocut && !undoinsert) return false;
  was_up_down = false;

  int ilen = undocut;
  int xlen = undoinsert;
  int b = undoat-xlen;
  int b1 = b;

  put_in_buffer(size_+ilen);

  if (ilen) {
    memmove(buffer+b+ilen, buffer+b, size_-b+1);
    memcpy(buffer+b, undobuffer, ilen);
    size_ += ilen;
    b += ilen;
  }

  if (xlen) {
    undobuffersize(xlen);
    memcpy(undobuffer, buffer+b, xlen);
    memmove(buffer+b, buffer+b+xlen, size_-xlen-b+1);
    size_ -= xlen;
  }

  undocut = xlen;
  if (xlen) yankcut = xlen;
  undoinsert = ilen;
  undoat = b;
  mark_ = b /* -ilen */;
  position_ = b;
  undo_is_redo = !undo_is_redo;

  minimal_update(b1);
  if (when()&WHEN_CHANGED) do_callback(); else set_changed();
  return true;
}

#if 0
bool Input::yank() {
  // fake yank by trying to get it out of undobuffer
  return yankcut && change(position(), position(), undobuffer, yankcut);
}
#endif

void Input::maybe_do_callback() {
  if (changed() || (when()&WHEN_NOT_CHANGED)) {
    clear_changed(); do_callback();}
}

////////////////////////////////////////////////////////////////

static void revert(Style *s) {
  //s->leading = 2; // this is the default now
}
static NamedStyle style("Input", revert, &Input::default_style);
NamedStyle* Input::default_style = &::style;

Input::Input(int x, int y, int w, int h, const char* l)
  : Widget(x, y, w, h, l)
{
  clear_flag(ALIGN_MASK);
  set_flag(ALIGN_LEFT);
  set_click_to_focus();
  mark_ = position_ = size_ = 0;
  bufsize = 0;
  buffer  = 0;
  value_ = "";
  xscroll_ = yscroll_ = 0;
  style(default_style);
  label_width = 0;
}

void Input::put_in_buffer(int len) {
  if (value_ == buffer && bufsize > len) {
    buffer[size_] = 0;
    return;
  }
  if (!bufsize) {
    if (len > size_) len += 9; // let a few characters insert before realloc
    bufsize = len+1;
    buffer = (char*)malloc(bufsize);
  } else if (bufsize <= len) {
    // we may need to move old value in case it points into buffer:
    int moveit = (value_ >= buffer && value_ < buffer+bufsize);
    // enlarge current buffer
    if (len > size_) {
      do {bufsize *= 2;} while (bufsize <= len);
    } else {
      bufsize = len+1;
    }
    char* nbuffer = (char*)realloc(buffer, bufsize);
    if (moveit) value_ += (nbuffer-buffer);
    buffer = nbuffer;
  }
  memmove(buffer, value_, size_); buffer[size_] = 0;
  value_ = buffer;
}

bool Input::static_value(const char* str, int len) {
  clear_changed();
  if (undowidget == this) undowidget = 0;
  if (str == value_ && len == size_) return false;
  if (len) { // non-empty new value:
    if (xscroll_ || yscroll_) {
      xscroll_ = yscroll_ = 0;
      minimal_update(0);
    } else {
      int i = 0;
      // find first different character:
      if (value_) {
	for (; i<size_ && i<len && str[i]==value_[i]; i++);
	if (i==size_ && i==len) return false;
      }
      minimal_update(i);
    }
    value_ = str;
    size_ = len;
  } else { // empty new value:
    if (!size_) return false; // both old and new are empty.
    size_ = 0;
    value_ = "";
    xscroll_ = yscroll_ = 0;
    minimal_update(0);
  }
  position(0, size());
  return true;
}

bool Input::static_value(const char* str) {
  return static_value(str, str ? strlen(str) : 0);
}

bool Input::value(const char* str, int len) {
  bool ret = static_value(str, len);
  if (len) put_in_buffer(len);
  return ret;
}

bool Input::value(const char* str) {
  return value(str, str ? strlen(str) : 0);
}

Input::~Input() {
  if (undowidget == this) undowidget = 0;
  if (bufsize) free((void*)buffer);
}

////////////////////////////////////////////////////////////////

// Move the point, move the mark only if shift is not held down:
void Input::shift_position(int p) {
  position(p, event_state(SHIFT) ? mark() : p);
}
void Input::shift_up_down_position(int p) {
  up_down_position(p, event_state(SHIFT));
}

// Due to MicroSoft-compatable programs assigning the Emacs control keys
// to menu items, this is about the best design I could come up with that
// allows Emacs bindings but allows people who want to reuse those keys
// for shortcuts to assign them. Previous solutions were not local to
// Input, which made it difficult to add or change the key assignments.
// This has a recursion test as some programs will turn the shortcut back
// into a keystroke and report it here...
bool Input::key_is_shortcut() {
  static bool recursion;
  if (recursion) return false;
  recursion = true;
  bool ret = fltk::handle(SHORTCUT, window()) != 0;
  recursion = false;
  return ret;
}

bool Input::handle_key() {

  int i;

  // insert any text:
  if (compose(i)) {
    if (!i && !event_length()) return 1;
    return replace(position(), i ? position()-i : mark(),
		   event_text(), event_length());
  }

  bool ctrl = event_state(CTRL);
  bool shift = event_state(SHIFT);
  bool alt = event_state(ALT|COMMAND);

  switch (event_key()) {

  case 'b':
    if (key_is_shortcut()) return true;
    ctrl = alt;
  case LeftKey:
    i = position_-1; if (!shift && mark_<i) i = mark_;
    shift_position(ctrl ? word_start(i) : i);
    return true;

  case 'f':
    if (key_is_shortcut()) return true;
    ctrl = alt;
  case RightKey:
    i = position_+1; if (!shift && mark_>i) i = mark_;
    shift_position(ctrl ? word_end(i) : i);
    return true;

  case 'p':
    if (key_is_shortcut()) return true;
    ctrl = alt;
  case UpKey:
    if (type() < MULTILINE) return false;
    i = position_; if (!shift && mark_<i) i = mark_;
    i = line_start(i);
    if (!i) shift_position(0);
    else up_down_position(line_start(i-1), shift);
    return true;

  case 'n':
    if (key_is_shortcut()) return true;
    ctrl = alt;
  case DownKey:
    if (type() < MULTILINE) return false;
    i = position_; if (!shift && mark_>i) i = mark_;
    i = line_end(i);
    if (i >= size()) shift_position(i);
    else up_down_position(i+1, shift);
    return true;

  case PageUpKey: {
    if (type() < MULTILINE) return false;
    i = line_start(position());
    setfont();
    for (int n = h()/line_spacing(leading()); n--;) i = line_start(i-1);
    shift_position(i);
    return true;}

  case PageDownKey: {
    if (type() < MULTILINE) return false;
    i = line_end(position());
    setfont();
    for (int n = h()/line_spacing(leading()); n--;) i = line_end(i)+1;
    shift_position(i+1);
    return true;}

  case 'a':
    if (ctrl) {
      // if already at start of line, select the entire buffer. This
      // makes two ^A's do a select-all for both Emacs & Win32 compatability
      i = line_start(position());
      if (i == position() && i == mark()) position(0, size());
      else shift_position(i);
      return true;
    }
    if (key_is_shortcut()) return true;
    ctrl = alt;
  case HomeKey:
    shift_position(ctrl ? 0 : line_start(position()));
    return true;

  case 'e':
    if (key_is_shortcut()) return true;
    ctrl = alt;
  case EndKey:
    shift_position(ctrl ? size() : line_end(position()));
    return true;

  case InsertKey:
    if (ctrl) copy();
    else if (shift) paste(*this, true);
    else return false; // CUA toggles insert mode on/off
    return true;

  case 'd':
    if (key_is_shortcut()) return true;
    ctrl = alt;
  case DeleteKey:
    // I don't know what CUA does with ctrl+delete, I made it delete words
    if (shift) copy();
    if (mark() != position()) cut();
    else cut(ctrl ? word_end(position()+1)-position() : 1);
    return true;

  case 'h': // retro-Emacs, modern versions do "help"
    if (key_is_shortcut()) return true;
    ctrl = alt;
  case BackSpaceKey:
    // I don't know what CUA does with ctrl+backspace, I made it delete words
    if (mark() != position()) cut();
    else cut(ctrl ? word_start(position()-1)-position() : -1);
    return true;

  case ReturnKey:
  case KeypadEnter:
    // if (key_is_shortcut()) return true;
    if (when() & WHEN_ENTER_KEY) {
      position(size(), 0);
      maybe_do_callback();
      return true;
    }
    if (type() < MULTILINE || ctrl || shift) return false;
    return replace(position(), mark(), '\n');

  case TabKey:
    /*if (type() < MULTILINE || ctrl || shift)*/ return false;
    //return replace(position(), mark(), event_text(), 1);

  case 'k': // Emacs clear-to-end-of-line
    if (key_is_shortcut()) return true;
    // alt should clear to end of paragraph, nyi
    i = line_end(position());
    if (i == position() && i < size()) i++;
    if (cut(position(), i))
      // Make all the adjacent ^K's go into the clipboard, like Emacs:
      fltk::copy(undobuffer, yankcut, true);
    return true;

  case 'c':
    if (!ctrl && key_is_shortcut()) return true;
    return copy();

  case 'o': // Emacs insert newline after cursor
    if (key_is_shortcut()) return true;
    if (replace(position(),mark(),"\n",1)) {
      position(position()-1);
      return 1;
    }
    return 0;

  case 't': // Emacs swap characters
    if (key_is_shortcut()) return true;
    if (size()<2) return 1;
    i = position();
    if (i <= 0 || value_[i-1]=='\n') i++;
    if (i >= size() || value_[i]=='\n') i--;
    {char t[2]; t[0] = value_[i]; t[1] = value_[i-1];
    if (!replace(i-1,i+1,t,2)) break;}
    position(i+1);
    return 1;

  case 'u': // Clear the field
    if (key_is_shortcut()) return true;
    position(size(), 0);
    return cut();

  case 'v':
    if (!ctrl && key_is_shortcut()) return true;
    paste(*this,true);
    return true;

  case 'w': // Emacs cut
    if (key_is_shortcut()) return true;
    copy();
    return cut();

  case 'x':
    if (!ctrl && key_is_shortcut()) return true;
    copy();
    return cut();

  case 'y':
    // Check for more global redo action first:
    if (key_is_shortcut()) return true;
    return undo_is_redo && undo();
#if 0
    // This is actually Emacs paste so do that if nothing else:
    paste(*this,true);
    return true;
#endif

  case '/': // Emacs undo
    if (key_is_shortcut()) return true;
  case 'z':
    // For undo we undo local typing first. Only if this fails do
    // we run some appliation menu item for undo:
    return !undo_is_redo && undo();

    // Other interesting Emacs characters:
    // 'q' quotes next character
    // 'r' backward incremental search
    // 's' incremental search
    // 'u' repeat count prefix

  }

  // Insert any other keys (like ^J) into the text, if no shortcuts eat them:
  if (event_length()) {
    if (key_is_shortcut()) return true;
    return replace(position(), mark(), event_text(), event_length());
  }

  return false;
}

int Input::handle(int event) {
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  return handle(event, X, Y, W, H);
}

// Set this to 1 to get the ability to drag selected text out to other
// widgets. Qt and KDE does this. Other systems seem to support this only in
// multiline or rich-text type widgets.
#define DND_OUT 1

#if DND_OUT
static int drag_start;
#endif

int Input::handle(int event, int X, int Y, int W, int H) {
  X += label_width; W -= label_width;
  int newpos, newmark;

  switch (event) {

  case ENTER:
  case LEAVE:
  case MOVE:
    return 1; // For tooltips

  case FOCUS:
#if 0
    switch (event_key()) {
    case Right:
      position(0);
      break;
    case Left:
      position(size());
      break;
    case Down:
      up_down_position(0, 0);
      break;
    case Up:
      up_down_position(line_start(size()), 0);
      break;
    case Tab:
      position(size(),0);
      break;
    default:
      position(position(),mark());// turns off the saved up/down arrow position
      break;
    }
#else
    if (fltk::event() == KEY) {
      position(size(),0);
    } else {
      position(position(), mark());
    }
#endif
    // redraw the highlight area:
    if (mark_ != position_) minimal_update(mark_, position_);
    // else just make the cursor appear:
    else minimal_update(size()+1);
    return 2; // returns 2 to make Group think it really important

  case UNFOCUS:
    // redraw the highlight area:
    if (mark_ != position_) minimal_update(mark_, position_);
    // else make the cursor disappear:
    else erase_cursor_at(position_);
  case HIDE:
    if (when() & WHEN_RELEASE) maybe_do_callback();
    return 1;

  case SHORTCUT:
    // If the user types text to a widget that does not want it, it will
    // call here eventually. Take the focus on the assumption they are
    // trying to type into this text field:
#if 1
    if (event_text()[0] < ' ' && event_key() != BackSpaceKey) return 0;
    if (event_state(ALT|COMMAND)) return 0;
#else
    // attempt to see if nobody else interested in key. This did not
    // work as keys that did nothing still moved the focus.
    {static bool recursion;
    if (recursion) return 0;
    recursion = true; bool r = key_is_shortcut(); recursion = false;
    if (r) return 0;}
#endif
    position(size());
    take_focus();
  case KEY:
    return handle_key();

  case PUSH:
    newpos = mouse_position(X, Y, W, H);
#if DND_OUT
    // See if they clicked in the selected test, if so we start a timeout
    // to see if they hold it long enough to start dragging:
    if (!event_state(ALT|COMMAND|CTRL|SHIFT) &&
	focused() && type()!=SECRET &&
	(newpos >= mark() && newpos < position() ||
	newpos >= position() && newpos < mark())) {
      drag_start = newpos;
      add_timeout(.25);
      return 1;
    }
    drag_start = -1;
#endif
    //take_focus();
    newmark = event_state(SHIFT) ? mark() : newpos;
    goto HANDLE_MOUSE;

#if DND_OUT
  case TIMEOUT:
    // This is called if they hold the mouse still long enough to indicate
    // that a drag is being started.
    if (drag_start >= 0) {
      drag_start = -1;
      copy(false);
      fltk::dnd();
    }
    return 1;
#endif

  case DRAG:
#if DND_OUT
    // If it has been long enough that we think the user is dragging the
    // mouse, but not long enough for the above timeout to happen, then
    // the user is probably selecting text and is not doing dnd.
    if (drag_start >= 0) {
      if (event_is_click()) return 1; // wait until debounce is done
      remove_timeout();
      newmark = event_state(SHIFT) ? mark() : drag_start;
      drag_start = -1;
    } else
#endif
      newmark = mark();
    newpos = mouse_position(X, Y, W, H);
  HANDLE_MOUSE:
    // Move the mark & point to word/line ends depending on mouse click count:
    if (event_clicks()) {
      int savepos = newpos;
      if (newpos >= newmark) {
	if (newpos == newmark) {
	  if (newpos < size()) newpos++;
	  else newmark--;
	}
	if (event_clicks() > 1) {
	  newpos = line_end(newpos);
	  newmark = line_start(newmark);
	} else {
	  newpos = word_end(newpos);
	  newmark = word_start(newmark);
	}
      } else {
	if (event_clicks() > 1) {
	  newpos = line_start(newpos);
	  newmark = line_end(newmark);
	} else {
	  newpos = word_start(newpos);
	  newmark = word_end(newmark);
	}
      }
      // If the new click did not increase the selection then cycle
      // back to single-click behavior:
      if (event == PUSH && (mark() > position() ?
		(newmark >= position() && newpos <= mark()) :
		(newmark >= mark() && newpos <= position()))) {
	event_clicks(0);
	newmark = newpos = savepos;
      }
    }
    position(newpos, newmark);
    if (event != RELEASE) return 1;

  case RELEASE:
#if DND_OUT
    // If we are still waiting to see if they are doing dnd or selection,
    // act like a single click at that point:
    if (drag_start >= 0) {
      newpos = newmark = drag_start; drag_start = -1;
      remove_timeout();
      goto HANDLE_MOUSE;
    }
#endif
    if (event_button() == 2) {
      // If user selects with button 2 then it replaces the selected
      // text with the clipboard:
      event_is_click(0); // stop double-click from picking a word
      paste(*this,false);
    } else if (position_ != mark_) {
      // copy to clipboard if the selection is non-zero
      copy(false);
    }
    return 1;

  case DND_ENTER:
    fltk::belowmouse(this); // send the leave events first
    // fall through:
  case DND_DRAG: {
    int p = mouse_position(X, Y, W, H);
#if 0 //DND_OUT
    // detect if they are dropping atop the original selection:
    if (focused() &&
	(p >= position() && p < mark() || p >= mark() && p < position())) {
      if (dnd_target == this) {
	dnd_target = 0;
	erase_cursor_at(dnd_target_position);
      }
      return 0;
    }
#endif
    if (dnd_target != this) {
      dnd_target = this;
      dnd_target_position = p;
      minimal_update(size()+1); // turn on the cursor
    } else if (dnd_target_position != p) {
      erase_cursor_at(dnd_target_position);
      dnd_target_position = p;
      minimal_update(size()+1); // turn on the cursor
    }
    return 1;}

  case DND_LEAVE:
    if (dnd_target == this) {
      dnd_target = 0;
      erase_cursor_at(dnd_target_position);
    }
    return 1;

  case DND_RELEASE:
#if DND_OUT
    if (dnd_target != this) return 0;
    if (focused()) {
      int p = dnd_target_position;
      if (p <= position() && p <= mark()) {
	// we are inserting before the selection
	if (!event_state(SHIFT|CTRL|ALT|COMMAND)) cut();
      } else if (p >= position() && p >= mark()) {
	// we are inserting after the selection, cut & adjust offset
	if (!event_state(SHIFT|CTRL|ALT|COMMAND)) {
	  dnd_target_position -= abs(mark()-position());
	  cut();
	}
      } else {
	// attempting to insert into the selection does nothing
	dnd_target = 0;
	erase_cursor_at(dnd_target_position);
	return 0;
      }
    }
#endif
    dnd_target = 0;
    position(dnd_target_position);
    take_focus();
    return 1;

  case PASTE: {
    const char* t = event_text();
	if (!t) return 0;
    int n = event_length();
    // strip trailing nulls:
    while (n > 0 && !t[n-1]) n--;
    // strip all trailing control & whitespace for single-line inputs:
    if (type() < MULTILINE)
      while (n > 0 && ((unsigned char*)t)[n-1] <= ' ') n--;
    return replace(position(), mark(), t, n);}

  default:
    return 0;
  }
}

//
// End of "$Id: Fl_Input.cxx,v 1.80 2003/04/27 01:54:52 spitzak Exp $".
//
