//
// "$Id: Fl_Boxtype.cxx,v 1.6 2002/03/10 23:10:23 spitzak Exp $"
//
// Box drawing code for the Fast Light Tool Kit (FLTK).
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

// These are the most common, square box types, which are always
// linked in because the default styles use them.

#include <fltk/Fl_Boxtype.h>
#include <fltk/Fl_Style.h>
#include <fltk/Fl_Widget.h>
#include <fltk/fl_draw.h>
#include <string.h>
#include <config.h>
#include <fltk/x.h>

////////////////////////////////////////////////////////////////

// this draws the highlight rectangle inside a passed rectangle
void Fl_Dotted_Frame::draw(int x,int y,int w,int h, Fl_Color c, Fl_Flags) const
{
  if (w <= 1 || h <= 1) return;
  fl_color(c);
#ifndef _WIN32
  // X version uses stipple pattern because there seem to be too many
  // servers with bugs when drawing dotted lines:
  static const char pattern[] = {0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
  static Pixmap evenstipple, oddstipple;
  if (!evenstipple) {
    Window root = RootWindow(fl_display, fl_screen);
    evenstipple = XCreateBitmapFromData(fl_display, root, pattern, 8, 8);
    oddstipple = XCreateBitmapFromData(fl_display, root, pattern+1, 8, 8);
  }
  XSetStipple(fl_display, fl_gc,
	      (fl_x_ + fl_y_)&1 ? oddstipple : evenstipple);
  XSetFillStyle(fl_display, fl_gc, FillStippled);
  // X documentation claims a nonzero line width is necessary for stipple
  // to work, but on the X servers I tried it does not seem to be needed:
  //XSetLineAttributes(fl_display, fl_gc, 1, LineSolid, CapButt, JoinMiter);
  fl_rect(x, y, w, h);
  XSetFillStyle(fl_display, fl_gc, FillSolid);
  // put line width back to zero:
  //XSetLineAttributes(fl_display, fl_gc, 0, LineSolid, CapButt, JoinMiter);
#else
  fl_line_style(FL_DOT);
  fl_rect(x, y, w, h);
  fl_line_style(0);
#endif
}

Fl_Dotted_Frame::Fl_Dotted_Frame(const char* name) : Fl_Boxtype_(name) {
  dx_ = dy_ = dw_ = dh_ = 1;
  fills_rectangle_ = 0;
}
const Fl_Dotted_Frame fl_dotted_frame("dotted frame");

////////////////////////////////////////////////////////////////

void Fl_No_Box::draw(int, int, int, int, Fl_Color, Fl_Flags) const {}

Fl_No_Box::Fl_No_Box(const char* name) : Fl_Boxtype_(name) {
  dx_ = dy_ = dw_ = dh_ = 0;
  fills_rectangle_ = 0;
}
const Fl_No_Box fl_no_box("none");

void Fl_Flat_Box::draw(
       int x, int y, int w, int h, Fl_Color color, Fl_Flags f) const
{
  if (!(f & FL_INVISIBLE) && h > 0 && w > 0) {
    fl_color(color);
    fl_rectf(x,y,w,h);
  }
}
Fl_Flat_Box::Fl_Flat_Box(const char* name) : Fl_Boxtype_(name) {
  dx_ = dy_ = dw_ = dh_ = 0;
  fills_rectangle_ = 1;
}
const Fl_Flat_Box fl_flat_box("flat");

////////////////////////////////////////////////////////////////

FL_API void fl_to_inactive(const char* s, char* to) {
  if (*s == '2') *to++ = *s++;
  while (*s) *to++ = 'M'+(*s++ - 'A')/3;
  *to = 0;
}

// widget draw() methods can use this to draw boxes at arbitary places
// in the window:
void Fl_Frame_Box::draw(int x, int y, int w, int h,
			Fl_Color c, Fl_Flags f) const
{
  const char* s = data();
  if (f & FL_VALUE) s = down->data();
  char buf[26]; if (f&FL_INACTIVE && Fl_Style::draw_boxes_inactive) {
    fl_to_inactive(s, buf); s = buf;}
  if (h > 0 && w > 0) {
    if (*s == '2') {s++; goto HACK;}
    for (;;) {
      // draw top line:
      fl_color(*s++ + (FL_GRAY_RAMP-'A'));
      fl_line(x, y, x+w-1, y);
      y++; if (--h <= 0) break;
      // draw left line:
      fl_color(*s++ + (FL_GRAY_RAMP-'A'));
      fl_line(x, y, x, y+h-1);
      x++; if (--w <= 0) break;
      if (!*s) break;
    HACK:
      // draw bottom line:
      fl_color(*s++ + (FL_GRAY_RAMP-'A'));
      fl_line(x, y+h-1, x+w-1, y+h-1);
      if (--h <= 0) break;
      // draw right line:
      fl_color(*s++ + (FL_GRAY_RAMP-'A'));
      fl_line(x+w-1, y, x+w-1, y+h-1);
      if (--w <= 0) break;
      if (!*s) break;
    }
  }
  if (!(f & FL_INVISIBLE)) {
    fl_color(c);
    fl_rectf(x,y,w,h);
  }
}

#ifdef _MSC_VER
#pragma warning(disable: 4355) // quiet warning about using this as base class init
#endif
Fl_Frame_Box::Fl_Frame_Box(const char* n, const char* s, const Fl_Frame_Box* d)
  : Fl_Boxtype_(n), data_(s), down(d ? d : this)
{
  fills_rectangle_ = 1;
  int i = strlen(s)/2;
  dw_ = dh_ = i;
  i /= 2;
  dx_ = dy_ = i;
}

const Fl_Frame_Box fl_down_box("down", "2WWMMPPAA");
const Fl_Frame_Box fl_up_box("up", "2AAWWMMTT", &fl_down_box);

const Fl_Frame_Box fl_thin_down_box("thin down", "2WWHH");
const Fl_Frame_Box fl_thin_up_box("thin up", "2HHWW", &fl_thin_down_box);

// in fltk 1.0 these used to point at each other as a "down" version:
const Fl_Frame_Box fl_engraved_box("engraved", "HHWWWWHH");
const Fl_Frame_Box fl_embossed_box("embossed", "WWHHHHWW");

const Fl_Frame_Box fl_border_box("border", "HHHH");

////////////////////////////////////////////////////////////////
// Deprecated "frame" box, appaently needed for fltk 1.0 compatability?

void Fl_Border_Frame::draw(
	int x, int y, int w, int h, Fl_Color color, Fl_Flags) const
{
  fl_color(color); fl_rect(x, y, w, h);
}
Fl_Border_Frame::Fl_Border_Frame(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = 1;
  dw_ = dh_ = 2;
  fills_rectangle_ = 0;
}
const Fl_Border_Frame fl_border_frame("border frame");

////////////////////////////////////////////////////////////////
// draw a box only when highlighted or selected:

void Fl_Highlight_Box::draw(
	int x, int y, int w, int h, Fl_Color color, Fl_Flags f) const
{
  if (f & (FL_HIGHLIGHT|FL_SELECTED|FL_VALUE)) {
    down->draw(x,y,w,h,color,f);
  } else {
    Fl_Flat_Box::draw(x,y,w,h,color,f);
  }
}
Fl_Highlight_Box::Fl_Highlight_Box(const char* n, const Fl_Boxtype_* b)
  : Fl_Flat_Box(n), down(b)
{
  dx_ = b->dx();
  dy_ = b->dy();
  dw_ = b->dw();
  dh_ = b->dh();
  fills_rectangle_ = 1;
}

const Fl_Highlight_Box fl_highlight_up_box("highlight up", &fl_thin_up_box);
const Fl_Highlight_Box fl_highlight_down_box("highlight down", &fl_thin_down_box);

const Fl_Boxtype_* Fl_Boxtype_::find(const char* name) {
  for (const Fl_Boxtype_* p = Fl_Boxtype_::first; p; p = p->next)
    if (p->name && !strcasecmp(name, p->name)) return p;
  return 0;
}

const Fl_Boxtype_* Fl_Boxtype_::first = 0;

//
// End of "$Id: Fl_Boxtype.cxx,v 1.6 2002/03/10 23:10:23 spitzak Exp $".
//
