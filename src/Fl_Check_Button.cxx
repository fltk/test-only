//
// "$Id: Fl_Check_Button.cxx,v 1.48 2004/08/01 22:28:22 spitzak Exp $"
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

#include <fltk/CheckButton.h>
#include <fltk/Group.h>
#include <fltk/Box.h>
#include <fltk/draw.h>

using namespace fltk;

/*! \class fltk::CheckButton
  This button turns the value() on and off each release of a click
  inside of it, and displays a checkmark to show the user this:

  \image html Fl_Check_Button.gif

  You can control the color of the checkbox with color() and the
  color of the checkmark with textcolor(). You can make it draw
  different colors when turned on by setting selection_color()
  and selection_textcolor() on the widget (these are ignored if
  set in an inherited style()).
*/

static void default_glyph(int glyph,
			  int x,int y,int w,int h,
			  const Style* style, Flags flags)
{
  Box* box = style->box();
  // Use the white rather than the gray color:
  flags = flags&~(SELECTED|HIGHLIGHT)|OUTPUT;
  // for back compatability with some programs that changed the
  // square into a diamond or circle, where the checkmark does
  // not look too good. Draw the box colored in with the
  // selected color instead:
  if (!box->fills_rectangle()) {
    if (flags&VALUE) flags |= SELECTED;
    box->draw(x, y, w, h, style, flags);
    return;
  }
  // Otherwise draw the box with normal colors and then draw checkmark
  // inside it:
  box->draw(x, y, w, h, style, flags);
  if (flags&VALUE) {
    box->inset(x, y, w, h);
    Color bg, fg; style->boxcolors(flags, bg, fg);
    setcolor(fg);
    if (h < 4) {fillrect(x+w/2-1,y+h/2-1,2,2); return;}
    x += 1;
    w = h - 2;
    int d1 = w/3;
    int d2 = w-d1;
    y += (h+d2)/2-d1-2;
    for (int n = 0; n < 3; n++, y++) {
      drawline(x, y, x+d1, y+d1);
      drawline(x+d1, y+d1, x+w-1, y+d1-d2+1);
    }
  }
}

void CheckButton::draw() {
  Button::draw(0, int(textsize())+2);
}

static void revert(Style* s) {
  s->buttonbox_ = NO_BOX;
  //s->box_ = DOWN_BOX;
  s->glyph_ = ::default_glyph;
}
static NamedStyle style("Check_Button", revert, &CheckButton::default_style);
NamedStyle* CheckButton::default_style = &::style;

CheckButton::CheckButton(int x, int y, int w, int h, const char *l)
  : Button(x, y, w, h, l)
{
  style(default_style);
  type(TOGGLE);
  set_flag(ALIGN_LEFT|ALIGN_INSIDE);
}
