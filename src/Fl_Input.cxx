//
// "$Id: Fl_Input.cxx,v 1.39 2000/09/07 08:52:35 spitzak Exp $"
//
// Input widget for the Fast Light Tool Kit (FLTK).
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

// Most useful changes of behavior can be accomplished by making a
// subclass and replacing the replace(...) function, to make a version
// that rejects changes you don't want to allow.

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXBUF 1024

////////////////////////////////////////////////////////////////

// Copy string p..e to the buffer, replacing characters with ^X and \nnn
// as necessary.  Truncate if necessary so the resulting string and
// null terminator fits in a buffer of size n.  Return new end pointer.
const char* Fl_Input::expand(const char* p, char* buf,int wordwrap) const {
  char* o = buf;
  char* e = buf+(MAXBUF-4);
  const char* lastspace = p;
  char* lastspace_out = o;
  int width_to_lastspace = 0;
  int word_count = 0;

  if (type()==FL_SECRET_INPUT) {
    while (o<e && p < value_+size_) {*o++ = '*'; p++;}
  } else while (o<e) {
    if (wordwrap && (p >= value_+size_ || isspace(*p))) {
      width_to_lastspace += (int)fl_width(lastspace_out, o-lastspace_out);
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
      if (c == '\n' && type()>=FL_MULTILINE_INPUT) {p--; break;}
      if (c == '\t' && type()>=FL_MULTILINE_INPUT) {
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
int Fl_Input::expandpos(
  const char* p,	// real string
  const char* e,	// pointer into real string
  const char* buf,	// conversion of real string by expand()
  int* returnn		// return offset into buf here
) const {
  int n = 0;
  if (type()==FL_SECRET_INPUT) n = e-p;
  else while (p<e) {
    int c = *p++;
    if (c & 0xE0) {
      n++;
    } else {
      if (c == '\t' && type()>=FL_MULTILINE_INPUT) n += 8-(n%8);
      else n += 2;
    }
  }
  if (returnn) *returnn = n;
  return fl_width(buf, n);
}

////////////////////////////////////////////////////////////////

// minimal update:
// Characters from mu_p to end of widget are redrawn.
// If erase_cursor_only, small part at mu_p is redrawn.
// Right now minimal update just keeps unchanged characters from
// being erased, so they don't blink.

void Fl_Input::minimal_update(int p) {
  if (damage() & FL_DAMAGE_ALL) return; // don't waste time if it won't be done
  if (damage() & FL_DAMAGE_EXPOSE) {
    if (p < mu_p) mu_p = p;
  } else {
    mu_p = p;
  }
  damage(FL_DAMAGE_EXPOSE);
  erase_cursor_only = 0;
}

void Fl_Input::minimal_update(int p, int q) {
  if (q < p) p = q;
  minimal_update(p);
}

////////////////////////////////////////////////////////////////

static double up_down_pos;
static int was_up_down;

void Fl_Input::setfont() const {
  fl_font(text_font(), text_size());
}

void Fl_Input::draw() {
  if (damage() & FL_DAMAGE_ALL) draw_text_frame();
  int X=x(); int Y=y(); int W=w(); int H=h(); text_box()->inset(X,Y,W,H);
  draw(X,Y,W,H);
}

void Fl_Input::draw(int X, int Y, int W, int H) 
{
  Fl_Color background = text_background();
  if (!show_cursor() && !size()) {
    // we have to erase it if cursor was there
    fl_color(background);
    fl_rectf(X, Y, W, H);
    return;
  }

  if (damage() & FL_DAMAGE_ALL) {
    // erase background
    fl_color(background);
    fl_rectf(X, Y, W, H);
  }
  if (W > 12) {X += 3; W -= 6;} // add a left/right border

  int selstart, selend;
  if (!show_cursor() && !pushed())
    selstart = selend = 0;
  else if (position() <= mark()) {
    selstart = position(); selend = mark();
  } else {
    selend = position(); selstart = mark();
  }

  setfont();
  int wordwrap = (type()>FL_MULTILINE_INPUT) ? W : 0;

  const char *p, *e;
  char buf[MAXBUF];

  // count how many lines and put the last one into the buffer:
  // And figure out where the cursor is:
  int height = text_size()+leading();
  int lines;
  int curx, cury;
  for (p=value(), curx=cury=lines=0; ;) {
    e = expand(p, buf, wordwrap);
    if (position() >= p-value() && position() <= e-value()) {
      curx = expandpos(p, value()+position(), buf, 0);
      if (focused() && !was_up_down) up_down_pos = curx;
      cury = lines*height;
      int newscroll = xscroll_;
      if (curx > newscroll+W-20) {
	// figure out scrolling so there is space after the cursor:
	newscroll = curx+20-W;
	// figure out the furthest left we ever want to scroll:
	int ex = int(expandpos(p, e, buf, 0))+2-W;
	// use minimum of both amounts:
	if (ex < newscroll) newscroll = ex;
      } else if (curx < newscroll+20) {
	newscroll = curx-20;
      }
      if (newscroll < 0) newscroll = 0;
      if (newscroll != xscroll_) {
	xscroll_ = newscroll;
	mu_p = 0; erase_cursor_only = 0;
      }
    }
    lines++;
    if (e >= value_+size_) break;
    if (*e == '\n' || *e == ' ') e++;
    p = e;
  }

  // adjust the scrolling:
  if (type()>=FL_MULTILINE_INPUT) {
    int newy = yscroll_;
    if (cury < newy) newy = cury;
    if (cury > newy+H-height) newy = cury-H+height;
    if (newy < -1) newy = -1;
    if (newy != yscroll_) {yscroll_ = newy; mu_p = 0; erase_cursor_only = 0;}
  } else {
    yscroll_ = -(H-height)/2;
  }

  fl_clip(X, Y, W, H);
  Fl_Color textcolor = text_color();
  if (!active_r()) textcolor = fl_inactive(text_color());

  p = value();
  // visit each line and draw it:
  int desc = height-fl_descent();
  int ypos = -yscroll_;
  for (; ypos < H;) {

    // re-expand line unless it is the last one calculated above:
    if (lines>1) e = expand(p, buf, wordwrap);

    if (ypos <= -height) goto CONTINUE; // clipped off top

    if (!(damage()&FL_DAMAGE_ALL)) {	// for minimal update:
      const char* pp = value()+mu_p; // pointer to where minimal update starts
      if (e >= pp && (!erase_cursor_only || p <= pp)) { // we must erase this
      // calculate area to erase:
      int x1 = -xscroll_;
      if (p < pp) x1 += expandpos(p, pp, buf, 0);
      // erase it:
      fl_color(background);
      fl_rectf(X+x1, Y+ypos, erase_cursor_only?2:W-x1, height);
      // it now draws entire line over it
      // this should not draw letters to left of erased area, but
      // that is nyi.
      }
    }

    // Draw selection area if required:
    if (selstart < selend && selstart <= e-value() && selend > p-value()) {
      const char* pp = value()+selstart;
      int x1 = -xscroll_;
      int offset1 = 0;
      if (pp > p) {
	fl_color(textcolor);
	x1 += expandpos(p, pp, buf, &offset1);
	fl_draw(buf, offset1, X-xscroll_, Y+ypos+desc);
      }
      pp = value()+selend;
      int x2 = W;
      int offset2;
      if (pp <= e) x2 = expandpos(p, pp, buf, &offset2)-xscroll_;
      else offset2 = strlen(buf);
      fl_color(selection_color());
      fl_rectf(X+x1, Y+ypos, x2-x1, height);
      fl_color(selection_text_color());
      fl_draw(buf+offset1, offset2-offset1, X+x1, Y+ypos+desc);
      if (pp < e) {
	fl_color(textcolor);
	fl_draw(buf+offset2, X+x2, Y+ypos+desc);
      }
    } else {
      // draw the cursor:
      fl_color(textcolor);
      if (show_cursor() && selstart == selend &&
	  position() >= p-value() && position() <= e-value()) {
	fl_rectf(X+curx-xscroll_, Y+ypos, 2, height);
      }
      fl_draw(buf, X-xscroll_, Y+ypos+desc);
    }
  CONTINUE:
    ypos += height;
    if (e >= value_+size_) break;
    if (*e == '\n' || *e == ' ') e++;
    p = e;
  }

  // for minimal update, erase all lines below last one if necessary:
  if (!(damage()&FL_DAMAGE_ALL) && type()>=FL_MULTILINE_INPUT && ypos<H
      && (!erase_cursor_only || p <= value()+mu_p)) {
    if (ypos < 0) ypos = 0;
    fl_color(background);
    fl_rectf(X, Y+ypos, W, H-ypos);
  }

  fl_pop_clip();
}

static int isword(char c) {
  return (c&128 || isalnum(c) || strchr("#%&-/@\\_~", c));
}

int Fl_Input::word_end(int i) const {
  if (type() == FL_SECRET_INPUT) return size();
  while (i < size() && isword(index(i))) i++;
  return i;
}

int Fl_Input::word_start(int i) const {
  if (type() == FL_SECRET_INPUT) return 0;
  while (i > 0 && isword(index(i-1))) i--;
  return i;
}

int Fl_Input::line_end(int i) const {
  if (type() >= FL_WORDWRAP_INPUT) {
    // go to the start of the paragraph:
    int j = i;
    while (j > 0 && index(j-1) != '\n') j--;
    // now measure lines until we get past i, end of that line is real eol:
    int wordwrap = w()-Fl::box_dw(box())-6;
    setfont();
    for (const char* p=value()+j; ;) {
      char buf[MAXBUF];
      p = expand(p, buf, wordwrap);
      if (p-value() >= i) return p-value();
      p++;
    }
  } else if (type() >= FL_MULTILINE_INPUT) {
    while (i < size() && index(i) != '\n') i++;
    return i;
  } else {
    return size();
  }
}

int Fl_Input::line_start(int i) const {
  if (type() < FL_MULTILINE_INPUT) return 0;
  int j = i;
  while (j > 0 && index(j-1) != '\n') j--;
  if (type() >= FL_WORDWRAP_INPUT) {
    // now measure lines until we get past i, start of that line is real eol:
    int wordwrap = w()-Fl::box_dw(box())-6;
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

int Fl_Input::mouse_position(int X, int Y, int W, int /*H*/) const
{
  if (!size()) return 0;

  // figure out what line we are pointing at:
  int theline = 0;
  if (type()>=FL_MULTILINE_INPUT) {
    theline = Fl::event_y()-Y+yscroll_;
    if (theline < 0) return 0;
    theline /= (text_size()+leading());
  }

  if (W > 12) {X += 3; W -= 6;} // add a left/right border

  setfont();
  int wordwrap = (type()>FL_MULTILINE_INPUT) ? W : 0;

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
  // Do a binary search for the horizontal character position:
  const char *l, *r, *t; double f0 = Fl::event_x()-X+xscroll_;
  for (l = p, r = e; l<r; ) {
    t = l+(r-l+1)/2;
    int f = X-xscroll_+expandpos(p, t, buf, 0);
    if (f <= Fl::event_x()) {l = t; f0 = Fl::event_x()-f;}
    else r = t-1;
  }
  if (l < e) { // see if closer to character on right:
    int f1 = X-xscroll_+expandpos(p, l+1, buf, 0)-Fl::event_x();
    if (f1 < f0) l = l+1;
  }
  return l-value();
}

int Fl_Input::position(int p, int m) {
  was_up_down = 0;
  if (p<0) p = 0;
  if (p>size()) p = size();
  if (m<0) m = 0;
  if (m>size()) m = size();
  if (p == position_ && m == mark_) return 0;
  if (p != m) {
    // new position is a selection:
    if (p != position_) minimal_update(position_, p);
    if (m != mark_) minimal_update(mark_, m);
  } else {
    // new position is a cursor:
    if (position_ == mark_) erase_cursor_only = 1;
    minimal_update(position_, mark_);
  }
  position_ = p;
  mark_ = m;
  return 1;
}

int Fl_Input::up_down_position(int i, int keepmark) {
  // cursor must already be at start of line!
  setfont();
  int wordwrap = type() > FL_MULTILINE_INPUT ? w()-Fl::box_dw(box())-6 : 0;
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
  j = position(j, keepmark ? mark_ : j);
  was_up_down = 1;
  return j;
}

int Fl_Input::copy() {
  if (mark() != position()) {
    int b, e; if (position() < mark()) {
      b = position(); e = mark();
    } else {
      e = position(); b = mark();
    }
    Fl::copy(value()+b, (type()!=FL_SECRET_INPUT) ? e-b : 0);
    return 1;
  }
  return 0;
}

#define MAXFLOATSIZE 40

static char* undobuffer;
static int undobufferlength;
static Fl_Input* undowidget;
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
int Fl_Input::replace(int b, int e, const char* text, int ilen) {

  was_up_down = 0;

  if (b<0) b = 0;
  if (e<0) e = 0;
  if (b>size_) b = size_;
  if (e>size_) e = size_;
  if (e<b) {int t=b; b=e; e=t;}
  if (e<=b && !ilen) return 0; // don't clobber undo for a null operation
  if (size_+ilen-(e-b) > maximum_size_) {
    ilen = maximum_size_-size_+(e-b);
    if (ilen < 0) ilen = 0;
  }

  put_in_buffer(size_+ilen);

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
    if (type() == FL_SECRET_INPUT) yankcut = 0; else yankcut = undocut;
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
  mark_ = position_ = undoat = b+ilen;

  // Insertions into the word at the end of the line will cause it to
  // wrap to the next line, so we must indicate that the changes may start
  // right after the whitespace before the current word.  This will
  // result in sub-optimal update when such wrapping does not happen
  // but it is too hard to figure out for now...
  if (type() > FL_MULTILINE_INPUT)
    while (b > 0 && !isspace(index(b))) b--;

  minimal_update(b);
  if (when()&FL_WHEN_CHANGED) do_callback(); else set_changed();
  return 1;
}

int Fl_Input::undo() {
  was_up_down = 0;
  if (undowidget != this || !undocut && !undoinsert) return 0;

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

  minimal_update(b1);
  if (when()&FL_WHEN_CHANGED) do_callback(); else set_changed();
  return 1;
}

#if 0
int Fl_Input::yank() {
  // fake yank by trying to get it out of undobuffer
  if (!yankcut) return 0;
  return change(position(), position(), undobuffer, yankcut);
}
#endif

void Fl_Input::maybe_do_callback() {
  if (changed() || (when()&FL_WHEN_NOT_CHANGED)) {
    clear_changed(); do_callback();}
}

void Fl_Input::show_cursor(char v) {
  if (v == show_cursor_) return;
  show_cursor_ = v;
  if (mark_ != position_) {
    // toggle the selection area on/off
    minimal_update(mark_, position_);
  } else if (v) {
    // cause it to draw the cursor next time
    minimal_update(size()+1);
  } else if (!(damage()&FL_DAMAGE_EXPOSE)) {
    // cause next draw to do minimal erase of cursor, if no other damage:
    minimal_update(position_); erase_cursor_only = 1;
  }
}

////////////////////////////////////////////////////////////////

static Fl_Named_Style* style = new Fl_Named_Style("Input", 0, &style);

Fl_Input::Fl_Input(int x, int y, int w, int h, const char* l)
  : Fl_Widget(x, y, w, h, l)
{
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_LEFT);
  show_cursor_ = 0;
  mark_ = position_ = size_ = 0;
  bufsize = 0;
  buffer  = 0;
  value_ = "";
  xscroll_ = yscroll_ = 0;
  maximum_size_ = 32767;
  style(::style);
}

void Fl_Input::put_in_buffer(int len) {
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

int Fl_Input::static_value(const char* str, int len) {
  clear_changed();
  if (undowidget == this) undowidget = 0;
  if (str == value_ && len == size_) return 0;
  if (len) { // non-empty new value:
    if (xscroll_ || yscroll_) {
      xscroll_ = yscroll_ = 0;
      minimal_update(0);
    } else {
      int i = 0;
      // find first different character:
      if (value_) {
	for (; i<size_ && i<len && str[i]==value_[i]; i++);
	if (i==size_ && i==len) return 0;
      }
      minimal_update(i);
    }
    value_ = str;
    size_ = len;
  } else { // empty new value:
    if (!size_) return 0; // both old and new are empty.
    size_ = 0;
    value_ = "";
    xscroll_ = yscroll_ = 0;
    minimal_update(0);
  }
  position(0, size());
  return 1;
}

int Fl_Input::static_value(const char* str) {
  return static_value(str, str ? strlen(str) : 0);
}

int Fl_Input::value(const char* str, int len) {
  int r = static_value(str, len);
  if (len) put_in_buffer(len);
  return r;
}

int Fl_Input::value(const char* str) {
  return value(str, str ? strlen(str) : 0);
}

Fl_Input::~Fl_Input() {
  if (undowidget == this) undowidget = 0;
  if (bufsize) free((void*)buffer);
}

////////////////////////////////////////////////////////////////

// Move the point, move the mark only if shift is not held down:
int Fl_Input::shift_position(int p) {
  return position(p, Fl::event_state(FL_SHIFT) ? mark() : p);
}
int Fl_Input::shift_up_down_position(int p) {
  return up_down_position(p, Fl::event_state(FL_SHIFT));
}

// If you define this symbol as zero you will get the peculiar fltk
// behavior where moving off the end of an input field will move the
// cursor into the next field.
// Define it as 1 to prevent cursor movement from going to next field.
#define NORMAL_INPUT_MOVE 0

#define ctrl(x) (x^0x40)

int Fl_Input::handle_key() {

  int i;
  if (Fl::compose(i))
    return replace(position(), i ? position()-i : mark(),
		   Fl::event_text(), Fl::event_length());

  switch (Fl::event_key()) {
  case FL_Left:
    return shift_position(position()-1) + NORMAL_INPUT_MOVE;
  case FL_Right:
    return shift_position(position()+1) + NORMAL_INPUT_MOVE;
  case FL_Up:
    i = line_start(position());
    if (!i) return NORMAL_INPUT_MOVE;
    shift_up_down_position(line_start(i-1));
    return 1;
  case FL_Down:
    i = line_end(position());
    if (i >= size()) return NORMAL_INPUT_MOVE;
    shift_up_down_position(i+1);
    return 1;
  case FL_Home:
    // if already at start of line, select the entire buffer. This
    // makes two ^A's do a select-all for both Emacs & Win32 compatability
    if (!shift_position(line_start(position()))) position(0, size());
    return 1;
  case FL_End:
    shift_position(line_end(position()));
    return 1;
  case FL_Delete:
    if (mark() != position()) cut(); else cut(1);
    return 1;
  case FL_BackSpace:
    if (mark() != position()) cut(); else cut(-1);
    return 1;
  case FL_Clear:
    // used for clear-to-end-of line, like ^K in Emacs
    i = line_end(position());
    if (i == position() && i < size()) i++;
    if (cut(position(), i))
      // Make all the adjacent ^K's go into the clipboard, like Emacs:
      Fl::copy(undobuffer, yankcut);
    return 1;
  case FL_Enter:
  case FL_KP_Enter:
    if (when() & FL_WHEN_ENTER_KEY) {
      position(size(), 0);
      maybe_do_callback();
      return 1;
    }
    if (type() >= FL_MULTILINE_INPUT && !Fl::event_state(FL_CTRL|FL_SHIFT))
      return replace(position(), mark(), '\n');
    break;
  case FL_Tab:
    if (type() >= FL_MULTILINE_INPUT && !Fl::event_state(FL_CTRL|FL_SHIFT))
      return replace(position(), mark(), Fl::event_text(), 1);
    break;
  case 'z':
  case '/': // Emacs undo, I doubt any programs want this as a menu item.
    if (Fl::event_state(FL_CTRL)) return undo();
    break;
  case 'x':
    if (Fl::event_state(FL_CTRL)) {copy(); return cut();}
    break;
  case 'c':
    if (Fl::event_state(FL_CTRL)) return copy();
    break;
  case 'v':
    if (Fl::event_state(FL_CTRL)) {Fl::paste(*this); return 1;}
    break;
  }

  return 0;
}

int Fl_Input::handle(int event) {
  int X=x(); int Y=y(); int W=w(); int H=h(); text_box()->inset(X,Y,W,H);
  return handle(event, X, Y, W, H);
}

int Fl_Input::handle(int event, int X, int Y, int W, int H) {
  static int dnd_save_position, dnd_save_mark;
  static int drag_start;
  int newpos, newmark;

  switch (event) {

  case FL_ENTER:
  case FL_LEAVE:
  case FL_MOVE:
    return 1; // For tooltips

  case FL_FOCUS:
    switch (Fl::event_key()) {
    case FL_Right:
      position(0);
      break;
    case FL_Left:
      position(size());
      break;
    case FL_Down:
      up_down_position(0, 0);
      break;
    case FL_Up:
      up_down_position(line_start(size()), 0);
      break;
    case FL_Tab:
      position(size(),0);
      break;
    default:
      position(position(),mark());// turns off the saved up/down arrow position
      break;
    }
    show_cursor(1);
    return 1;

  case FL_UNFOCUS:
    show_cursor(0);
    if (when() & FL_WHEN_RELEASE) maybe_do_callback();
    return 1;

  case FL_SHORTCUT:
    // If the user types text to a widget that does not want it, it will
    // call here eventually. Take the focus on the assumption they are
    // trying to type into this text field:
    if (Fl::event_text()[0]<=' ') return 0;
    if (Fl::event_state(FL_ALT|FL_META)) return 0;
    position(size());
    take_focus();
  case FL_KEYBOARD:
    return handle_key();

  case FL_PUSH:
    drag_start = newpos = mouse_position(X, Y, W, H);
    if (focused() && !Fl::event_state(FL_SHIFT) && type()!=FL_SECRET_INPUT &&
	(drag_start >= mark() && drag_start < position() ||
	drag_start >= position() && drag_start < mark())) {
      // user clicked int the selection, may be trying to drag
      return 1;
    }
    drag_start = -1;
    take_focus();
    newmark = Fl::event_state(FL_SHIFT) ? mark() : newpos;
    goto HANDLE_MOUSE;

  case FL_DRAG:
    if (drag_start >= 0) {
      if (Fl::event_is_click()) return 1; // debounce the mouse
      // save the position because sometimes we don't get DND_ENTER:
      dnd_save_position = position();
      dnd_save_mark = mark();
      // drag the data:
      copy(); Fl::dnd();
      return 1;
    }
    newpos = mouse_position(X, Y, W, H);
    newmark = mark();
  HANDLE_MOUSE:
    if (Fl::event_clicks()) {
      // Multiple clicks, expand the selection to word/line boundaries:
      int savepos = newpos;
      if (newpos >= newmark) {
	if (newpos == newmark) {
	  if (newpos < size()) newpos++;
	  else newmark--;
	}
	if (Fl::event_clicks() > 1) {
	  newpos = line_end(newpos);
	  newmark = line_start(newmark);
	} else {
	  newpos = word_end(newpos);
	  newmark = word_start(newmark);
	}
      } else {
	if (Fl::event_clicks() > 1) {
	  newpos = line_start(newpos);
	  newmark = line_end(newmark);
	} else {
	  newpos = word_start(newpos);
	  newmark = word_end(newmark);
	}
      }
      // If the multiple click does not increase the selection, revert
      // to single-click behavior:
      if (event != FL_DRAG && (mark() > position() ?
		    (newmark >= position() && newpos <= mark()) :
		    (newmark >= mark() && newpos <= position()))) {
	Fl::event_clicks(0);
	newmark = newpos = savepos;
      }
    }
    position(newpos, newmark);
    return 1;

  case FL_RELEASE:
    // if they just clicked in the middle of selection, move cursor there:
    if (drag_start >= 0) {
      newpos = newmark = drag_start; drag_start = -1;
      goto HANDLE_MOUSE;
    }
    if (Fl::event_button() == 2) {
      Fl::event_is_click(0); // stop double click from picking a word
      Fl::paste(*this);
    } else if (!Fl::event_is_click()) {
      // copy drag-selected text to the clipboard.
      copy();
    }
    return 1;

  case FL_DND_ENTER:
    Fl::belowmouse(this); // send the leave events first
    dnd_save_position = position();
    dnd_save_mark = mark();
    show_cursor(1);
    // fall through:
  case FL_DND_DRAG:
    position(mouse_position(X, Y, W, H));
    return 1;

  case FL_DND_LEAVE:
    position(dnd_save_position, dnd_save_mark);
    if (!focused()) show_cursor(0);
    return 1;

  case FL_DND_RELEASE:
    take_focus();
    return 1;

  case FL_PASTE: {
    // strip trailing control characters and spaces before pasting:
    const char* t = Fl::event_text();
    const char* e = t+Fl::event_length();
    if (type()<FL_MULTILINE_INPUT) while (e > t && *(uchar*)(e-1) <= ' ') e--;
    return replace(position(), mark(), t, e-t);}

  default:
    return 0;
  }
}

//
// End of "$Id: Fl_Input.cxx,v 1.39 2000/09/07 08:52:35 spitzak Exp $".
//
