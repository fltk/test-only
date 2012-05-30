//
// "$Id$"
//
// Box widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#include <fltk3/run.h>
#include <fltk3/draw.h>
#include <fltk3/Box.h>


fltk3::Box* fltk3::up(fltk3::Box* b) {
  if (b) {
    return b->up();
  } else {
    return UP_BOX;
  }
}


fltk3::Box* fltk3::down(fltk3::Box* b) {
  if (b) {
    return b->down();
  } else {
    return DOWN_BOX;
  }
}


fltk3::Box* fltk3::frame(fltk3::Box* b) {
  if (b) {
    return b->down();
  } else {
    return BORDER_FRAME;
  }
}


fltk3::Box* fltk3::box(fltk3::Box* b) {
  if (b) {
    return b->box();
  } else {
    return FRAME_BOX;
  }
}

//------------------------------------------------------------------------------

void fltk3::NoBox::_draw(const fltk3::Rectangle& r) const
{
  // empty
}

static fltk3::NoBox noBox("noBox");

/*!
 Draws an empty rectangle (a.k.a. nothing).
 */
fltk3::Box* const fltk3::NO_BOX = &noBox;


//------------------------------------------------------------------------------

void fltk3::FlatBox::_draw(const fltk3::Rectangle& r) const
{
  // FIXME: if (drawflags(INVISIBLE)) return;
  if (r.empty()) return;
  const fltk3::Color fg = fltk3::color();
  // FIXME: setcolor(fltk3::bgcolor());
  fltk3::rectf(r.x(), r.y(), r.w(), r.h());
  fltk3::color(fg);
}

static fltk3::FlatBox flatBox("flatBox");

/*!
 Draws a flat rectangle of bgcolor().
 */
fltk3::Box* const fltk3::FLAT_BOX = &flatBox;

//------------------------------------------------------------------------------

fltk3::Box* const fltk3::FRAME_BOX = fltk3::ENGRAVED_BOX;
fltk3::Box* const fltk3::GTK_UP_BOX = fltk3::UP_BOX;
fltk3::Box* const fltk3::GTK_DOWN_BOX = fltk3::DOWN_BOX;
fltk3::Box* const fltk3::GTK_UP_FRAME = fltk3::UP_FRAME;
fltk3::Box* const fltk3::GTK_DOWN_FRAME = fltk3::DOWN_FRAME;
fltk3::Box* const fltk3::GTK_THIN_UP_BOX = fltk3::THIN_UP_BOX;
fltk3::Box* const fltk3::GTK_THIN_DOWN_BOX = fltk3::THIN_DOWN_BOX;
fltk3::Box* const fltk3::GTK_THIN_UP_FRAME = fltk3::THIN_UP_FRAME;
fltk3::Box* const fltk3::GTK_THIN_DOWN_FRAME = fltk3::THIN_DOWN_FRAME;
fltk3::Box* const fltk3::GTK_ROUND_UP_BOX = fltk3::ROUND_UP_BOX;
fltk3::Box* const fltk3::GTK_ROUND_DOWN_BOX = fltk3::ROUND_DOWN_BOX;



//
// End of "$Id$".
//
