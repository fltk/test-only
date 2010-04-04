//
// "$Id$"
//
// Engraved label drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// Drawing code for XForms style engraved & embossed labels

#include <fltk3/run.h>
#include <fltk3/Widget.h>
#include <fltk3/draw.h>

// data[] is dx, dy, color triples

static void innards(
    const fltk3::Label* o, int X, int Y, int W, int H, fltk3::Align align,
    int data[][3], int n)
{
  fltk3::Align a1 = align;
  if (a1 & fltk3::ALIGN_CLIP) {
    fl_push_clip(X, Y, W, H); a1 = (fltk3::Align)(a1&~fltk3::ALIGN_CLIP);}
  fl_font((Fl_Font)o->font, o->size);
  for (int i = 0; i < n; i++) {
    fl_color((fltk3::Color)(i < n-1 ? data[i][2] : o->color));
    fl_draw(o->value, X+data[i][0], Y+data[i][1], W, H, a1);
  }
  if (align & fltk3::ALIGN_CLIP) fl_pop_clip();
}

void fl_shadow_label(
    const fltk3::Label* o, int X, int Y, int W, int H, fltk3::Align align)
{
  static int data[2][3] = {{2,2,fltk3::DARK3},{0,0,0}};
  innards(o, X, Y, W, H, align, data, 2);
}

void fl_engraved_label(
    const fltk3::Label* o, int X, int Y, int W, int H, fltk3::Align align)
{
  static int data[7][3] = {
    {1,0,fltk3::LIGHT3},{1,1,fltk3::LIGHT3},{0,1,fltk3::LIGHT3},
    {-1,0,fltk3::DARK3},{-1,-1,fltk3::DARK3},{0,-1,fltk3::DARK3},
    {0,0,0}};
  innards(o, X, Y, W, H, align, data, 7);
}

void fl_embossed_label(
    const fltk3::Label* o, int X, int Y, int W, int H, fltk3::Align align)
{
  static int data[7][3] = {
    {-1,0,fltk3::LIGHT3},{-1,-1,fltk3::LIGHT3},{0,-1,fltk3::LIGHT3},
    {1,0,fltk3::DARK3},{1,1,fltk3::DARK3},{0,1,fltk3::DARK3},
    {0,0,0}};
  innards(o, X, Y, W, H, align, data, 7);
}


//
// End of "$Id$".
//
