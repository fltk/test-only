//
// "$Id: fl_boxtype.cxx,v 1.40 2000/09/27 16:25:52 spitzak Exp $"
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

#include <FL/Fl_Boxtype.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <string.h>
#include <config.h>

////////////////////////////////////////////////////////////////

static void focus_rectangle(int x, int y, int w, int h, Fl_Color c) {
  if (h > 4 && w > 4) {
    fl_color(fl_contrast(FL_BLACK, c));
    fl_line_style(FL_DOT);
    fl_rect(x+1,y+1,w-2,h-2);
    fl_line_style(0);
  }
}

void Fl_No_Box::draw(const Fl_Widget*,
		     int x, int y, int w, int h, Fl_Flags f) const {
  if (!(f & FL_FRAME_ONLY) && (f & FL_FOCUSED))
    focus_rectangle(x,y,w,h,FL_GRAY);
}
Fl_No_Box::Fl_No_Box(const char* name) : Fl_Boxtype_(name) {
  dx_ = dy_ = dw_ = dh_ = 0;
  fills_rectangle_ = 0;
}
const Fl_No_Box fl_no_box("none");

void Fl_Flat_Box::draw(const Fl_Widget* widget,
		       int x, int y, int w, int h, Fl_Flags f) const
{
  if (!(f & FL_FRAME_ONLY) && h > 0 && w > 0) {
    Fl_Color color = widget->box_color(f);
    fl_color(color);
    fl_rectf(x,y,w,h);
    if (f & FL_FOCUSED) focus_rectangle(x, y, w, h, color);
  }
}
Fl_Flat_Box::Fl_Flat_Box(const char* name) : Fl_Boxtype_(name) {
  dx_ = dy_ = dw_ = dh_ = 0;
  fills_rectangle_ = 0;
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
      fl_xyline(x, y, x+w-1);
      y++; if (--h <= 0) break;
      // draw left line:
      fl_color(*s++ + (FL_GRAY_RAMP-'A'));
      fl_yxline(x, y+h-1, y);
      x++; if (--w <= 0) break;
      if (!*s) break;
    HACK:
      // draw bottom line:
      fl_color(*s++ + (FL_GRAY_RAMP-'A'));
      fl_xyline(x, y+h-1, x+w-1);
      if (--h <= 0) break;
      // draw right line:
      fl_color(*s++ + (FL_GRAY_RAMP-'A'));
      fl_yxline(x+w-1, y+h-1, y);
      if (--w <= 0) break;
      if (!*s) break;
    }
  }
  if (!(f & FL_FRAME_ONLY)) {
    fl_color(c);
    fl_rectf(x,y,w,h);
    if (f & FL_FOCUSED) focus_rectangle(x,y,w,h,c);
  }
}

void Fl_Frame_Box::draw(const Fl_Widget* widget,
			int x, int y, int w, int h, Fl_Flags f) const {
  draw(x,y,w,h, widget->box_color(f), f);
}
Fl_Frame_Box::Fl_Frame_Box(const char* n, const char* s, const Fl_Frame_Box* d)
  : Fl_Boxtype_(n), data_(s), down(d ? d : this)
{
  fills_rectangle_ = 1;
  int i = strlen(s)/2;
  dw_ = dh_ = i;
  i /= 2;
  dx_ = dy_ = i;
}

const Fl_Frame_Box fl_down_box("down", "2WWMMPPAA", &fl_up_box);
const Fl_Frame_Box fl_up_box("up", "2AAWWMMTT", &fl_down_box);

const Fl_Frame_Box fl_thin_down_box("thin down", "2WWHH", &fl_thin_up_box);
const Fl_Frame_Box fl_thin_up_box("thin up", "2HHWW", &fl_thin_down_box);

// in fltk 1.0 these used to point at each other as a "down" version:
const Fl_Frame_Box fl_engraved_box("engraved", "HHWWWWHH");
const Fl_Frame_Box fl_embossed_box("embossed", "WWHHHHWW");

const Fl_Frame_Box fl_border_box("border", "AAAA");

////////////////////////////////////////////////////////////////
// Deprecated "frame" box, appaently needed for fltk 1.0 compatability?

void Fl_Border_Frame::draw(const Fl_Widget* widget,
			   int x, int y, int w, int h, Fl_Flags) const
{
  fl_color(widget->color()); fl_rect(x, y, w, h);
}
Fl_Border_Frame::Fl_Border_Frame(const char* n) : Fl_Boxtype_(n) {
  dx_ = dy_ = 1;
  dw_ = dh_ = 2;
  fills_rectangle_ = 0;
}
const Fl_Border_Frame fl_border_frame("border frame");

////////////////////////////////////////////////////////////////
// draw a box only when highlighted or selected:

void Fl_Highlight_Box::draw(const Fl_Widget* widget,
			    int x, int y, int w, int h, Fl_Flags f) const
{
  if (f & (FL_HIGHLIGHT|FL_SELECTED|FL_VALUE)) {
    down->draw(widget,x,y,w,h,f);
  } else {
    Fl_Flat_Box::draw(widget,x,y,w,h,f);
  }
}
Fl_Highlight_Box::Fl_Highlight_Box(const char* n, const Fl_Boxtype b)
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
// End of "$Id: fl_boxtype.cxx,v 1.40 2000/09/27 16:25:52 spitzak Exp $".
//
