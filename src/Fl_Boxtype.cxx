//
// "$Id: Fl_Boxtype.cxx,v 1.19 2004/07/10 08:29:56 laza2000 Exp $"
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

/** \addtogroup boxes FLTK Box Types
    These variables point at static instances of several classes of
    Box which can be put in as the box() attribute of widgets.

    \image html boxtypes.gif
*/

// These are the most common, square box types, which are always
// linked in because the default styles use them.

#include <config.h>
#include <fltk/Box.h>
#include <fltk/Style.h>
#include <fltk/Widget.h>
#include <fltk/draw.h>
#include <fltk/x.h>
#include <fltk/string.h>
#include <fltk/math.h>
using namespace fltk;

////////////////////////////////////////////////////////////////

class FL_API DottedFrame : public Box {
public:
  void _draw(int x, int y, int w, int h,const Style* s, Flags flags) const {
    if (w <= 1 || h <= 1) return;
    Color bg, fg; s->boxcolors(flags, bg, fg);
    setcolor(fg);
#if USE_X11
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

#elif defined(_WIN32) || defined(_WIN32_WCE)
  // Windows 95/98/ME do not implement the dotted line style, so draw
  // every other pixel around the focus area...
  //
  // Also, QuickDraw (MacOS) does not support line styles specifically,
  // and the hack we use in fl_line_style() will not draw horizontal lines
  // on odd-numbered rows...
  int i, xx, yy; 
	w--; h--;

  for (xx = 0, i = 1; xx < w; xx ++, i ++) if (i & 1) drawpoint(x + xx, y);
  for (yy = 0; yy < h; yy ++, i ++) if (i & 1) drawpoint(x + w, y + yy);
  for (xx = w; xx > 0; xx --, i ++) if (i & 1) drawpoint(x + xx, y + h);
  for (yy = h; yy > 0; yy --, i ++) if (i & 1) drawpoint(x, y + yy);

#else
    line_style(DOT);
    strokerect(x, y, w, h);
    line_style(0);
#endif
  }
  DottedFrame(const char* name) : Box(name) {}
};
static DottedFrame dottedFrame("dotted_frame");

/*! \ingroup boxes
  Used to draw the dotted focus rectangle around widgets.
*/
Box* const fltk::DOTTED_FRAME = &dottedFrame;

////////////////////////////////////////////////////////////////

class NoBox : public Box {
public:
  void _draw(int x,int y,int w,int h,const Style* s, Flags) const {}
  NoBox(const char* name) : Box(name) {}
};
static NoBox noBox("none");

/*! \ingroup boxes
  Draws nothing.
  Can be used as a box to make the background of a widget invisible.
  Also some widgets check specifically for this and change their
  behavior or drawing methods.
*/
Box* const fltk::NO_BOX = &noBox;

////////////////////////////////////////////////////////////////

/*! \class fltk::FlatBox
  Draws a rectangle filled with style->color(). This is a useful
  base class for some box types.
*/
void FlatBox::_draw(int x, int y, int w, int h, const Style* style, Flags flags) const
{
  if (!(flags & INVISIBLE) && h > 0 && w > 0) {
    Color bg, fg; style->boxcolors(flags, bg, fg);
    setcolor(bg);
    fillrect(x, y, w, h);
  }
}
const BoxInfo* FlatBox::boxinfo() const {
  static const BoxInfo b = {0,0,0,0,3};
  return &b;
}
FlatBox::FlatBox(const char* name) : Box(name) {}
static FlatBox flatBox("flat");

/*! \ingroup boxes
  Draws a flat rectangle of style->color().
*/
Box* const fltk::FLAT_BOX = &flatBox;

////////////////////////////////////////////////////////////////

/** \class fltk::FrameBox
    This box class interprets a small string stored in data() to
    indicate the gray shades to draw around the edge of the box
    and can be used to draw simple bezels.

    The box is drawn as a spiral, starting with the bottom edge and
    going in a counter-clockwise direction, from the outside in
    toward the center. The string is interpreted to get a gray
    shade: A is black, X is white, and all other letters are 24
    possible steps of gray shade, and R is the normal background
    color of GRAY75. A leading '2' makes it start with the top
    edge, which will reverse exactly which pixels are drawn in
    the corner.

    The normal up box draws the pattern "AAWWMMTT"

    The normal down box draws the pattern "WWMMPPAA"

    The VALUE flag will cause the pattern from down() to be used
    instead, allowing you to draw a different bezel when pushed
    in.

    The INVISIBLE flag will not draw the interior, which can make
    many widgets draw faster and with less blinking.
*/

void fl_to_inactive(const char* s, char* to) {
  if (*s == '2') *to++ = *s++;
  while (*s) *to++ = 'M'+(*s++ - 'A')/3;
  *to = 0;
}
#include <stdio.h>
void FrameBox::_draw(int x, int y, int w, int h, const Style* style, Flags flags) const
{
  const char* s = data();
  if (flags & VALUE) s = down->data();
  char buf[26]; if (flags&INACTIVE && style->draw_boxes_inactive()) {
    fl_to_inactive(s, buf); s = buf;}
  if (h > 0 && w > 0) {
    if (*s == '2') {s++; goto HACK;}
    for (;;) {
      // draw bottom line:
      setcolor(*s++ + (GRAY00-'A'));
      drawline(x, y+h-1, x+w-1, y+h-1);
      if (--h <= 0) break;
      // draw right line:
      setcolor(*s++ + (GRAY00-'A'));
      drawline(x+w-1, y, x+w-1, y+h-1);
      if (--w <= 0) break;
      if (!*s) break;
    HACK:
      // draw top line:
      setcolor(*s++ + (GRAY00-'A'));
      drawline(x, y, x+w-1, y);
      y++; if (--h <= 0) break;
      // draw left line:
      setcolor(*s++ + (GRAY00-'A'));
      drawline(x, y, x, y+h-1);
      x++; if (--w <= 0) break;
      if (!*s) break;
    }
  }
  if (w > 0 && h > 0 && !(flags & INVISIBLE)) {
    Color bg, fg; style->boxcolors(flags, bg, fg);
    setcolor(bg);
    fillrect(x, y, w, h);
  }
}

#ifdef _MSC_VER
#pragma warning(disable: 4355) // quiet warning about using this as base class init
#endif
FrameBox::FrameBox(const char* n, const char* s, const FrameBox* d)
  : Box(n), data_(s), down(d ? d : this)
{
  int i = strlen(s)/2;
  boxinfo_.dw = boxinfo_.dh = i;
  i /= 2;
  boxinfo_.dx = boxinfo_.dy = i;
  boxinfo_.fills_rectangle = 3;
}

const BoxInfo* FrameBox::boxinfo() const {return &boxinfo_;}

static FrameBox downBox("down", "WWMMPPAA");
/*! \ingroup boxes
  A pushed-down button in fltk's standard theme.
*/
Box* const fltk::DOWN_BOX = &downBox;

static FrameBox upBox("up", "AAWWMMTT", &downBox);
/*! \ingroup boxes
  A up button in fltk's standard theme.
*/
Box* const fltk::UP_BOX = &upBox;

static FrameBox thinDownBox("thin_down", "WWHH");
/*! \ingroup boxes
  1-pixel-thick inset box.
*/
Box* const fltk::THIN_DOWN_BOX = &thinDownBox;

static FrameBox thinUpBox("thin_up", "HHWW", &thinDownBox);
/*! \ingroup boxes
  1-pixel-thick raised box.
*/
Box* const fltk::THIN_UP_BOX = &thinUpBox;

// in fltk 1.0 these used to point at each other as a "down" version:
static FrameBox engravedBox("engraved", "2HHWWWWHH", &downBox);
/*! \ingroup boxes
  2-pixel thick engraved line around edge.
*/
Box* const fltk::ENGRAVED_BOX = &engravedBox;

static FrameBox embossedBox("embossed", "2WWHHHHWW", &downBox);
/*! \ingroup boxes
  2-pixel thick raised line around edge.
*/
Box* const fltk::EMBOSSED_BOX = &embossedBox;

static FrameBox borderBox("border", "HHHH", &downBox);
/*! \ingroup boxes
  1-pixel thick gray line around rectangle.
*/
Box* const fltk::BORDER_BOX = &borderBox;

////////////////////////////////////////////////////////////////
// Deprecated "frame" box, appaently needed for fltk 1.0 compatability?

class BorderFrame : public Box {
public:
  void _draw(int x, int y, int w, int h, const Style* style,Flags flags) const
  {
    setcolor(style->textcolor());
    strokerect(x, y, w, h);
  }
  BorderFrame(const char* n) : Box(n) {}
  const BoxInfo* boxinfo() const {
    static const BoxInfo b = {1,1,2,2,0};
    return &b;
  }
};
static BorderFrame borderFrame("border_frame");
/*! \ingroup boxes
  Obsolete. Draws colored edge and draws nothing inside rectangle.
*/
Box* const fltk::BORDER_FRAME = &borderFrame;

////////////////////////////////////////////////////////////////
// draw a box only when highlighted or selected:

/*! \class fltk::HighlightBox
  Draws as fltk::FLAT_BOX normally, this can draw as any other box
  (passed to the constructor) when HIGHLIGHT or SELECTED or VALUE
  is turned on in the flags. This can be used to make frames appear
  when the mouse points at widgets or when the widget is turned on.
*/
void HighlightBox::_draw(
  int x, int y, int w, int h, const Style* style, Flags flags) const
{
  if (flags & (HIGHLIGHT|SELECTED|VALUE)) {
    down->draw(x,y,w,h,style,flags);
  } else {
    FlatBox::_draw(x,y,w,h,style,flags);
  }
}
const BoxInfo* HighlightBox::boxinfo() const {
  return down->boxinfo();
}
HighlightBox::HighlightBox(const char* n, const Box* b)
  : FlatBox(n), down(b) {}

static HighlightBox highlightUpBox("highlight_up", THIN_UP_BOX);
/*! \ingroup boxes
  Draws like FLAT_BOX normally, and as THIN_UP_BOX when the mouse
  pointer points at it or the value of the widget is turned on.
*/
Box* const fltk::HIGHLIGHT_UP_BOX = &highlightUpBox;
static HighlightBox highlightDownBox("highlight_down", THIN_DOWN_BOX);
/*! \ingroup boxes
  Draws like FLAT_BOX normally, and as THIN_DOWN_BOX when the mouse
  pointer points at it or the value of the widget is turned on.
*/
Box* const fltk::HIGHLIGHT_DOWN_BOX = &highlightDownBox;

//
// End of "$Id: Fl_Boxtype.cxx,v 1.19 2004/07/10 08:29:56 laza2000 Exp $".
//
