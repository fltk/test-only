//
// "$Id: fl_engraved_label.cxx,v 1.6 1999/11/01 02:21:38 carl Exp $"
//
// Engraved label drawing routines for the Fast Light Tool Kit (FLTK).
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

// Drawing code for XForms style engraved & embossed labels

#include <FL/Fl_Labeltype.H>
#include <FL/fl_draw.H>

// data is dx, dy, color triples

void fl_pattern_label(Fl_Labeltype l, const char* label,
		      int X, int Y, int W, int H,
		      Fl_Color fill, Fl_Flags f)
{
  Fl_Flags a1 = f;
  if (a1 & FL_ALIGN_CLIP) {
    fl_clip(X, Y, W, H);
    a1 = (Fl_Flags)(a1&~FL_ALIGN_CLIP);
  }
  for (int *data = (int*)(l->data); ; data += 3) {
    Fl_Color c = (Fl_Color)(data[2]);
    fl_color(c ? c : (f&FL_INACTIVE) ? Fl_Color(FL_GRAY) : fill);
    fl_draw(label, X+data[0], Y+data[1], W, H, a1);
    if (!c) break;
  }
  if (f & FL_ALIGN_CLIP) fl_pop_clip();
}

static int shadow_data[2][3] = {{2,2,FL_DARK3},{0,0,0}};
const Fl_Labeltype_ fl_shadow_label = {fl_pattern_label, shadow_data};
static Fl_Labeltype_Definer shadow("shadow", fl_shadow_label);

static int engraved_data[7][3] = {
  {1,0,FL_LIGHT3},{1,1,FL_LIGHT3},{0,1,FL_LIGHT3},
  {-1,0,FL_DARK3},{-1,-1,FL_DARK3},{0,-1,FL_DARK3},
  {0,0,0}};
const Fl_Labeltype_ fl_engraved_label = {fl_pattern_label, engraved_data};
static Fl_Labeltype_Definer engraved("engraved", fl_engraved_label);

static int embossed_data[7][3] = {
  {-1,0,FL_LIGHT3},{-1,-1,FL_LIGHT3},{0,-1,FL_LIGHT3},
  {1,0,FL_DARK3},{1,1,FL_DARK3},{0,1,FL_DARK3},
  {0,0,0}};
const Fl_Labeltype_ fl_embossed_label = {fl_pattern_label, embossed_data};
static Fl_Labeltype_Definer embossed("embossed", fl_embossed_label);

//
// End of "$Id: fl_engraved_label.cxx,v 1.6 1999/11/01 02:21:38 carl Exp $".
//
