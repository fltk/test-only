//
// "$Id: fl_boxtype.cxx,v 1.19 1999/11/10 14:18:52 carl Exp $"
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
#include <FL/fl_draw.H>
#include <string.h>
#include <config.h>

////////////////////////////////////////////////////////////////

static void no_draw(Fl_Boxtype, int, int, int, int,
		    Fl_Color, Fl_Flags)
{}
const Fl_Boxtype_ fl_no_box = {
  no_draw, 0, &fl_no_box, &fl_no_box, 0,0,0,0, false
};
static Fl_Boxtype_Definer none("none", fl_no_box);

static void flat_draw(Fl_Boxtype, int x, int y, int w, int h,
		      Fl_Color c, Fl_Flags f)
{
  if (!(f & FL_FRAME_ONLY)) {
    fl_color(c);
    fl_rectf(x,y,w,h);
  }
}
const Fl_Boxtype_ fl_flat_box = {
  flat_draw, 0, &fl_flat_box, &fl_flat_box, 0,0,0,0, true
};
static Fl_Boxtype_Definer flat("flat", fl_flat_box);

////////////////////////////////////////////////////////////////

FL_EXPORT void fl_frame(Fl_Boxtype b, int x, int y, int w, int h,
			Fl_Color c, Fl_Flags f)
{
  if (f & FL_VALUE)
    { b->down->draw(x, y, w, h, c, f&~FL_VALUE); return; }
  if (f & FL_HIGHLIGHT)
    { b->highlight->draw(x, y, w, h, c, f&~FL_HIGHLIGHT); return; }

  const char* s = (const char*)(b->data);
  Fl_Color col;
  if (h > 0 && w > 0) {
    if (*s == '2') {s++; goto HACK;}
    for (;;) {
      // draw top line:
      col = fl_inactive(*s++ + (FL_GRAY_RAMP-'A'), f);
      fl_color(col);
      fl_xyline(x, y, x+w-1);
      y++; if (--h <= 0) break;
      // draw left line:
      col = fl_inactive(*s++ + (FL_GRAY_RAMP-'A'), f);
      fl_color(col);
      fl_yxline(x, y+h-1, y);
      x++; if (--w <= 0) break;
      if (!*s) break;
    HACK:
      // draw bottom line:
      col = fl_inactive(*s++ + (FL_GRAY_RAMP-'A'), f);
      fl_color(col);
      fl_xyline(x, y+h-1, x+w-1);
      if (--h <= 0) break;
      // draw right line:
      col = fl_inactive(*s++ + (FL_GRAY_RAMP-'A'), f);
      fl_color(col);
      fl_yxline(x+w-1, y+h-1, y);
      if (--w <= 0) break;
      if (!*s) break;
    }
  }
  if (!(f & FL_FRAME_ONLY) && h > 0 && w > 0) {
    fl_color(c);
    fl_rectf(x,y,w,h);
  }
}

Fl_Boxtype_ fl_up_box = {
  fl_frame, "2AAXXIIUU", &fl_down_box, &fl_up_box, 2,2,4,4, true
};
static Fl_Boxtype_Definer up("up", fl_up_box);

Fl_Boxtype_ fl_down_box = {
  fl_frame, "2XXIIUUAA", &fl_down_box, &fl_down_box, 2,2,4,4, true
};
static Fl_Boxtype_Definer down("down", fl_down_box);

const Fl_Boxtype_ fl_thin_box = {
  fl_frame, "2HHVV", &fl_thin_down_box, &fl_thin_box, 1,1,2,2,true
};
static Fl_Boxtype_Definer thin("thin", fl_thin_box);

const Fl_Boxtype_ fl_thin_down_box = {
  fl_frame, "2VVHH", &fl_thin_down_box, &fl_thin_down_box, 1,1,2,2,true
};
static Fl_Boxtype_Definer thindown("thin down", fl_thin_down_box);

const Fl_Boxtype_ fl_engraved_box = {
  fl_frame, "HHVVVVHH", &fl_engraved_box, &fl_engraved_box, 2,2,4,4,true
};
static Fl_Boxtype_Definer engraved("engraved", fl_engraved_box);

const Fl_Boxtype_ fl_embossed_box = {
  fl_frame, "VVHHHHVV", &fl_engraved_box, &fl_engraved_box, 2,2,4,4,true
};
static Fl_Boxtype_Definer embossed("embossed", fl_embossed_box);

////////////////////////////////////////////////////////////////

static void border_draw(Fl_Boxtype, int x, int y, int w, int h,
			Fl_Color c, Fl_Flags f)
{
  fl_color((f&FL_INACTIVE) ? FL_INACTIVE_COLOR : FL_NO_COLOR);
  fl_rect(x, y, w, h);
  if (!(f & FL_FRAME_ONLY)) {
    fl_color(c);
    fl_rectf(x+1, y+1, w-2, h-2);
  }
}
const Fl_Boxtype_ fl_border_box = {
  border_draw, 0, &fl_border_box, &fl_border_box, 1,1,2,2,true
};
static Fl_Boxtype_Definer border("border", fl_border_box);

////////////////////////////////////////////////////////////////
// Deprecated "frame" values:

static void bf_draw(Fl_Boxtype, int x, int y, int w, int h,
		    Fl_Color c, Fl_Flags)
{
  fl_color(c); fl_rect(x, y, w, h);
}
const Fl_Boxtype_ fl_border_frame = {
  bf_draw, 0, &fl_border_frame, &fl_border_frame, 1,1,2,2, false
};
static Fl_Boxtype_Definer borderframe("border frame", fl_border_frame);

////////////////////////////////////////////////////////////////
// combination of any boxtype and flat:

void fl_flatx(Fl_Boxtype b, int x, int y, int w, int h,
		  Fl_Color c, Fl_Flags f)
{

  if (f & FL_VALUE)
    b->down->draw(x,y,w,h,c,f&~FL_VALUE);
  else if (f & FL_HIGHLIGHT)
    b->highlight->draw(x,y,w,h,c,f&~FL_HIGHLIGHT);
  else
    FL_FLAT_BOX->draw(x,y,w,h,c,f);
}

const Fl_Boxtype_ fl_flat_up_box = {
  fl_flatx, 0, FL_UP_BOX, FL_FLAT_BOX, 2,2,4,4, true
};
static Fl_Boxtype_Definer flatup("flat up", fl_flat_up_box);

const Fl_Boxtype_ fl_flat_down_box = {
  fl_flatx, 0, FL_DOWN_BOX, FL_FLAT_BOX, 2,2,4,4, true
};
static Fl_Boxtype_Definer flatdown("flat down", fl_flat_down_box);

const Fl_Boxtype_ fl_highlight_down_box = {
  fl_flatx, 0, FL_DOWN_BOX, FL_UP_BOX, 2,2,4,4, true
};
static Fl_Boxtype_Definer highlight("highlight", fl_highlight_down_box);
static Fl_Boxtype_Definer highlightdown("highlight down", fl_highlight_down_box);

const Fl_Boxtype_ fl_highlight_up_box = {
  fl_flatx, 0, FL_UP_BOX, FL_UP_BOX, 2,2,4,4, true
};
static Fl_Boxtype_Definer highlightup("highlight up", fl_highlight_up_box);

const Fl_Boxtype_* Fl_Boxtype_::find(const char* name) {
  for (Fl_Boxtype_Definer* p = Fl_Boxtype_Definer::first; p; p = p->next)
    if (!strcasecmp(name, p->name)) return p->boxtype;
  return 0;
}

Fl_Boxtype_Definer* Fl_Boxtype_Definer::first = 0;

//
// End of "$Id: fl_boxtype.cxx,v 1.19 1999/11/10 14:18:52 carl Exp $".
//
