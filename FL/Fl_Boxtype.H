//
// "$Id: Fl_Boxtype.H,v 1.6 1999/10/17 20:39:59 vincent Exp $"
//
// Boxes used by FLTK styles.
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

#ifndef Fl_Boxtype_h
#define Fl_Boxtype_h

#include "Fl_Color.H"
#include "Fl_Flags.H"

// struct rather than class to allow static constants:
struct FL_API Fl_Boxtype_ {
  void (*draw_)(const Fl_Boxtype_*,
	       int,int,int,int, Fl_Color fill, Fl_Flags);
  const void* data;
  const Fl_Boxtype_* down;
  int dx_, dy_, dw_, dh_;
  bool rectangular;
// public:
  void draw(int x, int y, int w, int h, Fl_Color c, Fl_Flags f=0) const
    {draw_(this, x, y, w, h, c, f);}
  int dx() const {return dx_;}
  int dy() const {return dy_;}
  int dw() const {return dw_;}
  int dh() const {return dh_;}
};

typedef const Fl_Boxtype_* Fl_Boxtype;

extern FL_API const Fl_Boxtype_ fl_flat_box;
#define FL_FLAT_BOX (&fl_flat_box)
extern FL_API const Fl_Boxtype_ fl_flat_up_box;
#define FL_FLAT_UP_BOX (&fl_flat_up_box)
extern FL_API const Fl_Boxtype_ fl_flat_down_box;
#define FL_FLAT_DOWN_BOX (&fl_flat_down_box)
extern FL_API Fl_Boxtype_ fl_normal_box;
#define FL_NORMAL_BOX (&fl_normal_box)
extern FL_API Fl_Boxtype_ fl_down_box;
#define FL_DOWN_BOX (&fl_down_box)
extern FL_API const Fl_Boxtype_ fl_thin_box;
#define FL_THIN_BOX (&fl_thin_box)
extern FL_API const Fl_Boxtype_ fl_thin_down_box;
#define FL_THIN_DOWN_BOX (&fl_thin_down_box)
extern FL_API const Fl_Boxtype_ fl_engraved_box;
#define FL_ENGRAVED_BOX (&fl_engraved_box)
extern FL_API const Fl_Boxtype_ fl_embossed_box;
#define FL_EMBOSSED_BOX (&fl_embossed_box)
extern FL_API const Fl_Boxtype_ fl_border_box;
#define FL_BORDER_BOX (&fl_border_box)
extern FL_API const Fl_Boxtype_ fl_shadow_box;
#define FL_SHADOW_BOX (&fl_shadow_box)
extern FL_API const Fl_Boxtype_ fl_rounded_box;
#define FL_ROUNDED_BOX (&fl_rounded_box)
extern FL_API const Fl_Boxtype_ fl_rshadow_box;
#define FL_RSHADOW_BOX (&fl_rshadow_box)
extern FL_API const Fl_Boxtype_ fl_rflat_box;
#define FL_RFLAT_BOX (&fl_rflat_box)
extern FL_API const Fl_Boxtype_ fl_round_box;
#define FL_ROUND_BOX (&fl_round_box)
extern FL_API const Fl_Boxtype_ fl_round_down_box;
#define FL_ROUND_DOWN_BOX (&fl_round_down_box)
extern FL_API const Fl_Boxtype_ fl_diamond_box;
#define FL_DIAMOND_BOX (&fl_diamond_box)
extern FL_API const Fl_Boxtype_ fl_diamond_down_box;
#define FL_DIAMOND_DOWN_BOX (&fl_diamond_down_box)
extern FL_API const Fl_Boxtype_ fl_oval_box;
#define FL_OVAL_BOX (&fl_oval_box)
extern FL_API const Fl_Boxtype_ fl_oval_shadow_box;
#define FL_OSHADOW_BOX (&fl_oval_shadow_box)
extern FL_API const Fl_Boxtype_ fl_oval_flat_box;
#define FL_OFLAT_BOX (&fl_oval_flat_box)

// depreciated but still-useful values:
extern FL_API const Fl_Boxtype_ fl_no_box;
#define FL_NO_BOX (&fl_no_box)
extern FL_API const Fl_Boxtype_ fl_border_frame;
#define FL_BORDER_FRAME (&fl_border_frame)

#ifndef FLTK_2 // back compatability section
#define FL_UP_FRAME (&fl_normal_box)
#define FL_DOWN_FRAME (&fl_down_box)
#define FL_THIN_UP_FRAME (&fl_thin_box)
#define FL_THIN_DOWN_FRAME (&fl_thin_down_box)
#define FL_ENGRAVED_FRAME (&fl_engraved_box)
#define FL_EMBOSSED_FRAME (&fl_embossed_box)
#define FL_SHADOW_FRAME (&fl_shadow_box)
#define FL_ROUNDED_FRAME (&fl_rounded_box)
#define FL_OVAL_FRAME (&fl_oval_box)
#define FL_UP_BOX (&fl_normal_box)
#define FL_THIN_UP_BOX (&fl_thin_box)
#define FL_ROUND_UP_BOX (&fl_round_box)
#define FL_DIAMOND_UP_BOX (&fl_diamond_box)
#define FL_FRAME FL_ENGRAVED_FRAME
#define FL_FRAME_BOX FL_ENGRAVED_BOX
#endif

#endif

//
// End of "$Id: Fl_Boxtype.H,v 1.6 1999/10/17 20:39:59 vincent Exp $".
//
