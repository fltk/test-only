//
// "$Id: fl_boxtype.cxx,v 1.13 1999/10/07 07:04:59 bill Exp $"
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
#include <config.h>

////////////////////////////////////////////////////////////////

static void no_draw(Fl_Boxtype, int, int, int, int,
		    Fl_Color, Fl_Flags)
{}
const Fl_Boxtype_ fl_no_box = {
  no_draw, 0, &fl_no_box, 0,0,0,0, false
};

static void flat_draw(Fl_Boxtype, int x, int y, int w, int h,
		      Fl_Color c, Fl_Flags f)
{
  if (!(f & FL_FRAME_ONLY)) {
    fl_color(c);
    fl_rectf(x,y,w,h);
  }
}
const Fl_Boxtype_ fl_flat_box = {
  flat_draw, 0, &fl_flat_box, 0,0,0,0, true
};

////////////////////////////////////////////////////////////////

FL_EXPORT void fl_to_inactive(const char* s, char* to) {
  if (*s == '2') *to++ = *s++;
  while (*s) *to++ = 'R'+(*s++ - 'R')*10/17;
  *to = 0;
}

FL_EXPORT void fl_frame(Fl_Boxtype b, int x, int y, int w, int h,
			Fl_Color c, Fl_Flags f)
{
  if (f & FL_VALUE) b = b->down;
  const char* s = (const char*)(b->data);
  char buf[26]; if (f&FL_INACTIVE) {fl_to_inactive(s, buf); s = buf;}
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
  if (!(f & FL_FRAME_ONLY) && h > 0 && w > 0) {
    fl_color(c);
    fl_rectf(x,y,w,h);
  }
}

#if 1 // Windoze style:

Fl_Boxtype_ fl_normal_box = {
  fl_frame, "2AAUWMMTT", &fl_down_box, 2,2,4,4, true
};

Fl_Boxtype_ fl_down_box = {
  fl_frame, "2UWMMPPAA", &fl_down_box, 2,2,4,4, true
};

#else // classic fltk look:

Fl_Boxtype_ fl_normal_box = {
  fl_frame, "AAAAWUJJUTNN", &fl_down_box, 3,3,6,6, true
};

Fl_Boxtype_ fl_down_box = {
  fl_frame, "NNTUJJUWAAAA", &fl_down_box, 3,3,6,6, true
};

#endif

const Fl_Boxtype_ fl_thin_box = {
  fl_frame, "2HHWW", &fl_thin_down_box, 1,1,2,2, true
};

const Fl_Boxtype_ fl_thin_down_box = {
  fl_frame, "2WWHH", &fl_thin_down_box, 1,1,2,2, true
};

const Fl_Boxtype_ fl_engraved_box = {
  fl_frame, "HHWWWWHH", &fl_engraved_box, 2,2,4,4, true
};

const Fl_Boxtype_ fl_embossed_box = {
  fl_frame, "WWHHHHWW", &fl_engraved_box, 2,2,4,4, true
};

////////////////////////////////////////////////////////////////

static void border_draw(Fl_Boxtype, int x, int y, int w, int h,
			Fl_Color c, Fl_Flags f)
{
  fl_color((f&FL_INACTIVE) ? FL_INACTIVE_COLOR : FL_NO_COLOR);
  fl_rect(x, y, w, h);
  fl_color(c); fl_rectf(x+1, y+1, w-2, h-2);
}
const Fl_Boxtype_ fl_border_box = {
  border_draw, 0, &fl_border_box, 1,1,2,2, true
};

////////////////////////////////////////////////////////////////
// Depreciated "frame" values:

static void bf_draw(Fl_Boxtype, int x, int y, int w, int h,
		    Fl_Color c, Fl_Flags)
{
  fl_color(c); fl_rect(x, y, w, h);
}
const Fl_Boxtype_ fl_border_frame = {
  bf_draw, 0, &fl_border_frame, 1,1,2,2, false
};

#if 0
// These frame types are removed and it draws the boxes instead

static void frammitz(Fl_Boxtype b, int x, int y, int w, int h,
		     Fl_Color c, Fl_Flags f)
{
  ((Fl_Boxtype)(b->data))->draw(x,y,w,h,c,f|FL_FRAME_ONLY);
}

const Fl_Boxtype_ fl_up_frame = {
  frammitz, &fl_normal_box, 0, 2,2,4,4, false
};

const Fl_Boxtype_ fl_down_frame = {
  frammitz, &fl_down_box, 0, 2,2,4,4, false
};

const Fl_Boxtype_ fl_thin_up_frame = {
  frammitz, &fl_thin_box, 0, 2,2,4,4, false
};

const Fl_Boxtype_ fl_thin_down_frame = {
  frammitz, &fl_thin_down_box, 0, 2,2,4,4, false
};

const Fl_Boxtype_ fl_engraved_frame = {
  frammitz, &fl_engraved_box, 0, 2,2,4,4, false
};

const Fl_Boxtype_ fl_embossed_frame = {
  frammitz, &fl_embossed_box, 0, 2,2,4,4, false
};

#endif

////////////////////////////////////////////////////////////////
// combination of any boxtype and flat:

static void flatx(Fl_Boxtype b, int x, int y, int w, int h,
		  Fl_Color c, Fl_Flags f)
{
  if (f & FL_VALUE)
    ((Fl_Boxtype)(b->data))->draw(x,y,w,h,c,f&~FL_VALUE);
  else
    FL_FLAT_BOX->draw(x,y,w,h,c,f);
}

const Fl_Boxtype_ fl_flat_up_box = {
  flatx, FL_NORMAL_BOX, 0, 2,2,4,4, true
};

const Fl_Boxtype_ fl_flat_down_box = {
  flatx, FL_DOWN_BOX, 0, 2,2,4,4, true
};

//
// End of "$Id: fl_boxtype.cxx,v 1.13 1999/10/07 07:04:59 bill Exp $".
//
