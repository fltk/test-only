//
// "$Id$"
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

// Based on Frametab V2 contributed by Curtis Edwards (curt1@trilec.com)

#include <fltk/BarGroup.h>
#include <fltk/Box.h>
#include <fltk/events.h>
#include <fltk/damage.h>
using namespace fltk;

static void revert(Style *s) {
  s->box_ = THIN_UP_BOX;
  //s->box_ = FLAT_BOX;
  s->color_ = GRAY75;
  s->labelsize_ = 10;
}
static NamedStyle style("BarGroup", revert, &BarGroup::default_style);
NamedStyle* BarGroup::default_style = &::style;

BarGroup::BarGroup(int x, int y, int w, int h, const char* title)
  : Group(x, y, w, h,title)
{
  style(default_style);
  open_ = true;
  highlighted = false;
  pushed = false;
  glyph_size_ = 10;
  saved_size = h;
  align(ALIGN_INSIDE);
}

void BarGroup::glyph_box(Rectangle& r) const {
  int z = open_ ? glyph_size_ : saved_size;
  int w = this->w();
  int h = this->h();
  if (horizontal()) w = z; else h = z;
  r.set(0,0,w,h); //box()->inset(r);
}

int BarGroup::handle(int event)
{
  Rectangle r;
  switch (event) {
  case ENTER:
  case MOVE:
    if (highlight_color() && takesevents()) {
      glyph_box(r);
      bool hl = event_inside(r);
      if (hl != highlighted) {
	highlighted = hl;
	redraw(DAMAGE_HIGHLIGHT);
      }
    }
    break;
  case LEAVE:
    if (highlighted) {
      highlighted = false;
      redraw(DAMAGE_HIGHLIGHT);
    }
    break;
  case PUSH:
    glyph_box(r);
    if (event_inside(r)) {
      pushed = highlighted = true;
      redraw(DAMAGE_HIGHLIGHT);
      return true;
    }
    break;
  case DRAG:
    glyph_box(r);
    if (event_inside(r)) {
      if (!pushed) {
	pushed = highlighted = true;
	redraw(DAMAGE_HIGHLIGHT);
      }
    } else {
      if (pushed) {
	pushed = false;
	redraw(DAMAGE_HIGHLIGHT);
      }
    }
    return true;
  case RELEASE:
    if (pushed) {
      opened(!open_);
      pushed = false;
      highlighted = true;
      redraw(DAMAGE_HIGHLIGHT);
      do_callback();
    } else if (highlighted) {
      highlighted = false;
      redraw(DAMAGE_HIGHLIGHT);
    }
    return true;
  case SHORTCUT:
    return Group::handle(event);
  }
  if (open_) return Group::handle(event);
  else return 0;
}

void BarGroup::draw()
{
  if (open_) {
    if (damage() & ~DAMAGE_HIGHLIGHT) {
      // make it not draw the inside label:
      //int saved = flags(); align(ALIGN_TOP);
      Group::draw();
      //flags(saved);
    }
  } else if (damage() & ~(DAMAGE_CHILD|DAMAGE_HIGHLIGHT)) {
    draw_box();
    Rectangle r(w(),h());
    Flags flags = this->flags();
    box()->inset(r, style(), flags);
    if (horizontal()) {
      r.x(saved_size); r.w(r.w()-saved_size);
      flags &= ~(ALIGN_TOP|ALIGN_BOTTOM);
      flags |= ALIGN_LEFT|ALIGN_INSIDE;
    } else {
      r.y(saved_size); r.h(r.h()-saved_size);
    }
    draw_label(r, style(), flags);
  }
  if (damage() & (DAMAGE_EXPOSE|DAMAGE_HIGHLIGHT|DAMAGE_ALL)) {
    Flags flags = 0;
    if (pushed) flags |= VALUE;
    if (highlighted) flags |= HIGHLIGHT;
    Rectangle r; glyph_box(r);
    draw_glyph(horizontal() ? GLYPH_RIGHT_BUTTON : GLYPH_DOWN_BUTTON, r,flags);
  }
}

bool BarGroup::opened(bool v)
{
  if (open_) {
    if (v) return false;
    open_ = false;
    if (horizontal()) { // horizontal
      saved_size = h();
      Widget::resize(w(), glyph_size_);
    } else {
      saved_size = w();
      Widget::resize(glyph_size_, h());
    }
  } else {
    if (!v) return false;
    open_ = true;
    if (horizontal()) // horizontal
      Widget::resize(w(), saved_size);
    else
      Widget::resize(saved_size, h());
  }
  relayout();
  redraw();
  return true;
}

// Don't move widgets around while we are closed!
void BarGroup::layout() {
  if (open_) Group::layout();
  else Widget::layout();
}
