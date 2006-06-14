//
// "$Id$"
//
// Box drawing code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

/** \addtogroup symbols
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
class FL_API FocusFrame : public Box {
public:
  FocusFrame(const char* name) : Box(name) {}
  void _draw(const fltk::Rectangle& r1) const {
    if (!drawflags(FOCUSED)) return;
    pen_mode(PEN_OVERLAY);
    line_style(DOT,1);
    strokerect(r1);
    line_style(SOLID,1);
    pen_mode(PEN_NORMAL);
  }
};

class FL_API DottedFrame : public Box {
public:
  void _draw(const fltk::Rectangle& r1) const {
    if (!drawflags(FOCUSED)) return;

    fltk::Rectangle r; transform(r1,r);
    if (r.w() > 12) {r.move_x(1); r.move_r(-1);}
    else if (r.w() <= 3) return;
    if (r.h() > 15) {r.move_y(1); r.move_b(-1);}
    else if (r.h() <= 3) return;

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
    XSetStipple(xdisplay, gc, (r.x()+r.y()-r1.x()-r1.y())&1 ? oddstipple : evenstipple);
    XSetFillStyle(xdisplay, gc, FillStippled);
    // X documentation claims a nonzero line width is necessary for stipple
    // to work, but on the X servers I tried it does not seem to be needed:
    //XSetLineAttributes(xdisplay, gc, 1, LineSolid, CapButt, JoinMiter);
    XDrawRectangle(xdisplay, xwindow, gc, r.x(), r.y(), r.w()-1, r.h()-1);
    XSetFillStyle(xdisplay, gc, FillSolid);
    // put line width back to zero:
    //XSetLineAttributes(xdisplay, gc, 0, LineSolid, CapButt, JoinMiter);

#elif defined(_WIN32) || defined(_WIN32_WCE)
/*
    // Draw using WIN32 API function (since 95)
    RECT r = {r.x(), r.y(), r.r()-1, r.b()-1};
    DrawFocusRect(dc, &r);
*/
    // Draw using bitmap patterns (like X11) and PatBlt
    static const WORD pattern[] = { 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA };
    static HBRUSH evenbrush, oddbrush;
    if (!evenbrush) {
      // Init stipple brushes
      BITMAP bm;
      bm.bmType = 0;
      bm.bmWidth = 8;
      bm.bmHeight = 8;
      bm.bmWidthBytes = 2;
      bm.bmPlanes = 1;
      bm.bmBitsPixel = 1;
      bm.bmBits = (LPVOID)pattern;
      HBITMAP evenstipple = CreateBitmapIndirect(&bm);
      bm.bmBits = (LPVOID)(pattern+1);
      HBITMAP oddstipple  = CreateBitmapIndirect(&bm);
      // Create the brush from the bitmap bits
      evenbrush = CreatePatternBrush(evenstipple);
      oddbrush  = CreatePatternBrush(oddstipple);
      // Delete the useless bitmaps
      DeleteObject(evenstipple);
      DeleteObject(oddstipple);
    }

    HBRUSH brush = (r.x()+r.y()-r1.x()-r1.y())&1 ? oddbrush : evenbrush;

    // Select the patterned brush into the DC
    HBRUSH old_brush = (HBRUSH)SelectObject(dc, brush);

    // Draw horizontal lines
    PatBlt(dc, r.x(), r.y(), r.w(), 1, PATCOPY);
    PatBlt(dc, r.x(), r.b()-1, r.w(), 1, PATCOPY);

    // Draw vertical lines
    PatBlt(dc, r.x(), r.y(), 1, r.h(), PATCOPY);
    PatBlt(dc, r.r()-1, r.y(), 1, r.h(), PATCOPY);

    // Clean up
    SelectObject(dc, old_brush);
#else
    line_style(DOT);
    strokerect(r);
    line_style(0);
#endif
  }
  DottedFrame(const char* name) : Box(name) {}
};
static DottedFrame dottedFrame("dotted_frame");
//static FocusFrame focusFrame("focus_frame");

/*! \ingroup boxes
  Default value for focusbox(). This draws nothing if FOCUSED is
  not set in the flags. If it is set, this draws a dashed line
  one pixel inset.
*/
Box* const fltk::DOTTED_FRAME = &dottedFrame;
Box* const fltk::FOCUS_FRAME = &dottedFrame; //&focusFrame;

////////////////////////////////////////////////////////////////

class NoBox : public Box {
public:
  void _draw(const fltk::Rectangle&) const {}
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
  Draws a rectangle filled with getbgcolor(). This is a useful
  base class for some box types.
*/
void FlatBox::_draw(const fltk::Rectangle& r) const
{
  if (drawflags(INVISIBLE)) return;
  if (r.empty()) return;
  const Color fg = getcolor();
  setcolor(getbgcolor());
  fillrect(r);
  setcolor(fg);
}
bool FlatBox::fills_rectangle() const {return true;}
bool FlatBox::is_frame() const {return true;}
FlatBox::FlatBox(const char* name) : Box(name) {}
static FlatBox flatBox("flat");

/*! \ingroup boxes
  Draws a flat rectangle of getbgcolor().
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

    The normal up box draws the pattern "AAWWHHTT"

    The normal down_ box draws the pattern "WWHHPPAA"

    The VALUE flag will cause the pattern from down_() to be used
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

void FrameBox::_draw(const fltk::Rectangle& R) const
{
  if (drawflags(VALUE) && down_) {
    down_->draw(R);
    return;
  }
  fltk::Rectangle r(R);
  if (r.empty()) return;
  const Color fg = getcolor();
  const char* s = data();
  char buf[26]; if (drawflags(INACTIVE_R) && Style::draw_boxes_inactive_) {
    fl_to_inactive(s, buf); s = buf;}
  if (*s == '2') {s++; goto HACK;}
  for (;;) {
    // draw bottom line:
    setcolor(*s++ + (GRAY00-'A'));
    drawline(r.x(), r.b()-1, r.r()-1, r.b()-1);
    r.move_b(-1); if (r.h() <= 0) break;
    // draw right line:
    setcolor(*s++ + (GRAY00-'A'));
    drawline(r.r()-1, r.y(), r.r()-1, r.b()-1);
    r.move_r(-1); if (r.w() <= 0) break;
    if (!*s) break;
  HACK:
    // draw top line:
    setcolor(*s++ + (GRAY00-'A'));
    drawline(r.x(), r.y(), r.r()-1, r.y());
    r.move_y(1); if (r.h() <= 0) break;
    // draw left line:
    setcolor(*s++ + (GRAY00-'A'));
    drawline(r.x(), r.y(), r.x(), r.b()-1);
    r.move_x(1); if (r.w() <= 0) break;
    if (!*s) break;
  }
  if (!drawflags(INVISIBLE)) {
    setcolor(getbgcolor());
    fillrect(r);
  }
  setcolor(fg);
}

/** \fn FrameBox::FrameBox(const char* name, int dx,int dy,int dw,int dh, const char* pattern, const Box* down=0)
    Constructor where you give the thickness of the borders used by inset().
*/

void FrameBox::inset(fltk::Rectangle& r) const {
  if (drawflags(VALUE) && down_) {
    down_->inset(r);
  } else {
    r.x(r.x()+dx_);
    r.y(r.y()+dy_);
    r.w(r.w()-dw_);
    r.h(r.h()-dh_);
  }
}

bool FrameBox::fills_rectangle() const {return true;}
bool FrameBox::is_frame() const {return true;}

static FrameBox downBox("down_", 2,2,4,4, "WWLLRRAA");
/*! \ingroup boxes
  Inset box in fltk's standard theme
*/
Box* const fltk::DOWN_BOX = &downBox;

// The normal pushable button:
static FrameBox downBox2("down_", 2,2,3,3, "2LLWWAA");
static FrameBox upBox("up", 1,1,3,3, "AAWWLL", &downBox2);
/*! \ingroup boxes
  A up button in fltk's standard theme.
*/
Box* const fltk::UP_BOX = &upBox;

static FrameBox thinDownBox("thin_down", 1,1,2,2, "WWLL");
/*! \ingroup boxes
  1-pixel-thick inset box.
*/
Box* const fltk::THIN_DOWN_BOX = &thinDownBox;

static FrameBox thinUpBox("thin_up", 1,1,2,2, "LLWW", &thinDownBox);
/*! \ingroup boxes
  1-pixel-thick raised box.
*/
Box* const fltk::THIN_UP_BOX = &thinUpBox;

// in fltk 1.0 these used to point at each other as a "down_" version:
static FrameBox engravedBox("engraved", 2,2,4,4, "2LLWWWWLL", &downBox);
/*! \ingroup boxes
  2-pixel thick engraved line around edge.
*/
Box* const fltk::ENGRAVED_BOX = &engravedBox;

static FrameBox embossedBox("embossed", 2,2,4,4, "LLWWWWLL", &downBox);
/*! \ingroup boxes
  2-pixel thick raised line around edge.
*/
Box* const fltk::EMBOSSED_BOX = &embossedBox;

static FrameBox borderBox("border", 1,1,1,1, "LLLL", &downBox);
/*! \ingroup boxes
  1-pixel thick gray line around rectangle.
*/
Box* const fltk::BORDER_BOX = &borderBox;

////////////////////////////////////////////////////////////////
// Deprecated "frame" box, appaently needed for fltk 1.0 compatability?

class BorderFrame : public Box {
public:
  void _draw(const fltk::Rectangle& r) const
  {
    strokerect(r);
  }
  void inset(fltk::Rectangle& r) const {r.inset(1);}
  BorderFrame(const char* n) : Box(n) {}
};
static BorderFrame borderFrame("border_frame");
/*! \ingroup boxes
  Obsolete. Draws colored edge and draws nothing inside rectangle.
*/
Box* const fltk::BORDER_FRAME = &borderFrame;

////////////////////////////////////////////////////////////////
// draw a box only when highlighted or selected:

/*! \class fltk::HighlightBox
  Draws nothing normally, this can draw as any other box
  (passed to the constructor) when HIGHLIGHT, VALUE, or PUSHED
  is turned on in the flags. This can be used to make frames appear
  when the mouse points at widgets or when the widget is turned on.
*/
void HighlightBox::_draw(const fltk::Rectangle& r) const
{
  if (drawflags(HIGHLIGHT|VALUE|PUSHED))
    down_->draw(r);
//   else
//     FlatBox::_draw(r);
}
void HighlightBox::inset(fltk::Rectangle& r) const {down_->inset(r);}
bool HighlightBox::fills_rectangle() const {return false;}
bool HighlightBox::is_frame() const {return false;}

HighlightBox::HighlightBox(const char* n, const Box* b)
  : FlatBox(n), down_(b) {}

static HighlightBox highlightUpBox("highlight_up", THIN_UP_BOX);
/*! \ingroup boxes
  Draws nothing normally, and as THIN_UP_BOX when the mouse
  pointer points at it or the value of the widget is turned on.
*/
Box* const fltk::HIGHLIGHT_UP_BOX = &highlightUpBox;
static HighlightBox highlightDownBox("highlight_down", THIN_DOWN_BOX);
/*! \ingroup boxes
  Draws nothing normally, and as THIN_DOWN_BOX when the mouse
  pointer points at it or the value of the widget is turned on.
*/
Box* const fltk::HIGHLIGHT_DOWN_BOX = &highlightDownBox;

//
// End of "$Id$".
//
