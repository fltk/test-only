//
// "$Id: Fl_Boxtype.cxx,v 1.12 2003/01/15 07:55:20 spitzak Exp $"
//
// Box drawing code for the Fast Light Tool Kit (FLTK).
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

// These are the most common, square box types, which are always
// linked in because the default styles use them.

#include <fltk/Box.h>
#include <fltk/Style.h>
#include <fltk/Widget.h>
#include <fltk/draw.h>
#include <fltk/x.h>
#include <fltk/string.h>
#include <config.h>
using namespace fltk;

////////////////////////////////////////////////////////////////

// Used to draw the focus rectangle around things:

class FL_API DottedFrame : public Box {
public:
  void draw(int x,int y,int w,int h, Color c, Flags) const {
    if (w <= 1 || h <= 1) return;
    setcolor(c);
#if !defined(_WIN32) && !defined(__APPLE__) || USE_X11
    // X version uses stipple pattern because there seem to be too many
    // servers with bugs when drawing dotted lines:
    static const char pattern[] 
      = {0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
    static Pixmap evenstipple, oddstipple;
    if (!evenstipple) {
      XWindow root = RootWindow(xdisplay, xscreen);
      evenstipple = XCreateBitmapFromData(xdisplay, root, pattern, 8, 8);
      oddstipple = XCreateBitmapFromData(xdisplay, root, pattern+1, 8, 8);
    }
    int xx = x; int yy = y; transform(xx,yy);
    XSetStipple(xdisplay, gc, (xx+yy-x-y)&1 ? oddstipple : evenstipple);
    XSetFillStyle(xdisplay, gc, FillStippled);
    // X documentation claims a nonzero line width is necessary for stipple
    // to work, but on the X servers I tried it does not seem to be needed:
    //XSetLineAttributes(xdisplay, gc, 1, LineSolid, CapButt, JoinMiter);
    XDrawRectangle(xdisplay, xwindow, gc, xx, yy, w-1, h-1);
    XSetFillStyle(xdisplay, gc, FillSolid);
    // put line width back to zero:
    //XSetLineAttributes(xdisplay, gc, 0, LineSolid, CapButt, JoinMiter);
#else
    line_style(DOT);
    strokerect(x, y, w, h);
    line_style(0);
#endif
  }
  DottedFrame(const char* name) : Box(name) {
    dx_ = dy_ = dw_ = dh_ = 1;
    fills_rectangle_ = 0;
  }
};
static DottedFrame dottedFrame("dotted frame");
Box* const fltk::DOTTED_FRAME = &dottedFrame;

////////////////////////////////////////////////////////////////

class NoBox : public Box {
public:
  void draw(int,int,int,int, Color, Flags=0) const {}
  NoBox(const char* n) : Box(name) {
    dx_ = dy_ = dw_ = dh_ = 0;
    fills_rectangle_ = 0;
  }
};
static NoBox noBox("none");
Box* const fltk::NO_BOX = &noBox;

////////////////////////////////////////////////////////////////

void FlatBox::draw(int x, int y, int w, int h, Color color, Flags f) const {
  if (!(f & INVISIBLE) && h > 0 && w > 0) {
    setcolor(color);
    fillrect(x,y,w,h);
  }
}
FlatBox::FlatBox(const char* name) : Box(name) {
  dx_ = dy_ = dw_ = dh_ = 0;
  fills_rectangle_ = 1;
}
static FlatBox flatBox("flat");
Box* const fltk::FLAT_BOX = &flatBox;

////////////////////////////////////////////////////////////////

void fl_to_inactive(const char* s, char* to) {
  if (*s == '2') *to++ = *s++;
  while (*s) *to++ = 'M'+(*s++ - 'A')/3;
  *to = 0;
}

// widget draw() methods can use this to draw boxes at arbitary places
// in the window:
void FrameBox::draw(int x, int y, int w, int h,
			Color c, Flags f) const
{
  const char* s = data();
  if (f & VALUE) s = down->data();
  char buf[26]; if (f&INACTIVE && Style::draw_boxes_inactive) {
    fl_to_inactive(s, buf); s = buf;}
  if (h > 0 && w > 0) {
    if (*s == '2') {s++; goto HACK;}
    for (;;) {
      // draw top line:
      setcolor(*s++ + (GRAY00-'A'));
      drawline(x, y, x+w-1, y);
      y++; if (--h <= 0) break;
      // draw left line:
      setcolor(*s++ + (GRAY00-'A'));
      drawline(x, y, x, y+h-1);
      x++; if (--w <= 0) break;
      if (!*s) break;
    HACK:
      // draw bottom line:
      setcolor(*s++ + (GRAY00-'A'));
      drawline(x, y+h-1, x+w-1, y+h-1);
      if (--h <= 0) break;
      // draw right line:
      setcolor(*s++ + (GRAY00-'A'));
      drawline(x+w-1, y, x+w-1, y+h-1);
      if (--w <= 0) break;
      if (!*s) break;
    }
  }
  if (!(f & INVISIBLE)) {
    setcolor(c);
    fillrect(x,y,w,h);
  }
}

#ifdef _MSC_VER
#pragma warning(disable: 4355) // quiet warning about using this as base class init
#endif
FrameBox::FrameBox(const char* n, const char* s, const FrameBox* d)
  : Box(n), data_(s), down(d ? d : this)
{
  fills_rectangle_ = 1;
  int i = strlen(s)/2;
  dw_ = dh_ = i;
  i /= 2;
  dx_ = dy_ = i;
}

static FrameBox downBox("down", "2WWMMPPAA");
Box* const fltk::DOWN_BOX = &downBox;

static FrameBox upBox("up", "2AAWWMMTT", &downBox);
Box* const fltk::UP_BOX = &upBox;

static FrameBox thinDownBox("thin down", "2WWHH");
Box* const fltk::THIN_DOWN_BOX = &thinDownBox;

static FrameBox thinUpBox("thin up", "2HHWW", &thinDownBox);
Box* const fltk::THIN_UP_BOX = &thinUpBox;

// in fltk 1.0 these used to point at each other as a "down" version:
static FrameBox engravedBox("engraved", "HHWWWWHH");
Box* const fltk::ENGRAVED_BOX = &engravedBox;

static FrameBox embossedBox("embossed", "WWHHHHWW");
Box* const fltk::EMBOSSED_BOX = &embossedBox;

static FrameBox borderBox("border", "HHHH");
Box* const fltk::BORDER_BOX = &borderBox;

////////////////////////////////////////////////////////////////
// Deprecated "frame" box, appaently needed for fltk 1.0 compatability?

class BorderFrame : public Box {
public:
  void draw(int x, int y, int w, int h, Color color, Flags) const {
    setcolor(color); strokerect(x, y, w, h);
  }
  BorderFrame(const char* n) : Box(n) {
    dx_ = dy_ = 1;
    dw_ = dh_ = 2;
    fills_rectangle_ = 0;
  }
};
static BorderFrame borderFrame("border frame");
Box* const fltk::BORDER_FRAME = &borderFrame;

////////////////////////////////////////////////////////////////
// draw a box only when highlighted or selected:

void HighlightBox::draw(
	int x, int y, int w, int h, Color color, Flags f) const
{
  if (f & (HIGHLIGHT|SELECTED|VALUE)) {
    down->draw(x,y,w,h,color,f);
  } else {
    FlatBox::draw(x,y,w,h,color,f);
  }
}
HighlightBox::HighlightBox(const char* n, const Box* b)
  : FlatBox(n), down(b)
{
  dx_ = b->dx();
  dy_ = b->dy();
  dw_ = b->dw();
  dh_ = b->dh();
  fills_rectangle_ = 1;
}

static HighlightBox highlightUpBox("highlight up", THIN_UP_BOX);
Box* const fltk::HIGHLIGHT_UP_BOX = &highlightUpBox;
static HighlightBox highlightDownBox("highlight down", THIN_DOWN_BOX);
Box* const fltk::HIGHLIGHT_DOWN_BOX = &highlightDownBox;

Box* Box::find(const char* name) {
  for (Box* p = Box::first; p; p = p->next)
    if (p->name && !strcasecmp(name, p->name)) return p;
  return 0;
}

Box* Box::first = 0;

//
// End of "$Id: Fl_Boxtype.cxx,v 1.12 2003/01/15 07:55:20 spitzak Exp $".
//
