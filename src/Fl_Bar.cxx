//
// "$Id: Fl_Bar.cxx,v 1.4 2002/12/10 01:46:16 easysw Exp $"
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

// Based on Frametab V2 contributed by Curtis Edwards (curt1@trilec.com)

#include <fltk/BarGroup.h>
#include <fltk/Box.h>
#include <fltk/events.h>
#include <fltk/damage.h>
using namespace fltk;

static void revert(Style *s) {
  //s->box = UP_BOX;
  s->box = FLAT_BOX;
  s->color = GRAY75;
  s->labelsize = 10;
}
static NamedStyle style("BarGroup", revert, &BarGroup::default_style);
NamedStyle* BarGroup::default_style = &::style;

BarGroup::BarGroup(int x, int y, int w, int h, const char* title)
  : Group(x, y, w, h,title)
{
  type(HORIZONTAL);
  style(default_style);
  open_ = true;
  highlighted = false;
  pushed = false;
  glyph_size_ = 10;
  saved_size = h;
  align(ALIGN_LEFT|ALIGN_INSIDE);
}

void BarGroup::glyph_box(int& x, int& y, int& w, int& h) {
  x = y = 0; w = this->w(); h = this->h(); //box()->inset(x,y,w,h);
  if (type() & 1) { // horizontal
    w = open_ ? glyph_size_ : saved_size;
  } else {
    h = open_ ? glyph_size_ : saved_size;
  }
}

int BarGroup::handle(int event)
{
  int x,y,w,h;
  switch (event) {
  case ENTER:
  case MOVE:
  case LEAVE:
    if (highlight_color() && takesevents()) {
      glyph_box(x,y,w,h);
      bool hl = event_inside(x,y,w,h);
      if (hl != highlighted) {
	highlighted = hl;
	redraw(DAMAGE_HIGHLIGHT);
      }
    }
    break;
  case PUSH:
    glyph_box(x,y,w,h);
    if (event_inside(x,y,w,h)) {
      pushed = highlighted = true;
      redraw(DAMAGE_HIGHLIGHT);
      return true;
    }
    break;
  case DRAG:
    glyph_box(x,y,w,h);
    if (event_inside(x,y,w,h)) {
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
    } else if (highlighted) {
      highlighted = false;
      redraw(DAMAGE_HIGHLIGHT);
    }
    return true;
  }
  if (open_) return Group::handle(event);
  else return 0;
}

void BarGroup::draw()
{
  if (open_) {
    if (damage() & ~DAMAGE_HIGHLIGHT) {
      // make it not draw the inside label:
      int saved = flags(); align(ALIGN_TOP);
      Group::draw();
      flags(saved);
    }
  } else if (damage() & ~(DAMAGE_CHILD|DAMAGE_HIGHLIGHT)) {
    draw_box();
    int x = 0,y = 0,w = this->w(),h = this->h(); box()->inset(x,y,w,h);
    if (type() & 1) // horizontal
      draw_inside_label(saved_size, y, w-saved_size, h, 0);
    else
      draw_inside_label(x, saved_size, w, h-saved_size, 0);
  }
  if (damage() & (DAMAGE_EXPOSE|DAMAGE_HIGHLIGHT)) {
    Flags f = 0;
    if (pushed) f |= VALUE;
    if (highlighted) f |= HIGHLIGHT;
    int x,y,w,h; glyph_box(x,y,w,h);
    draw_glyph(0, x, y, w, h, f);
  }
}

bool BarGroup::opened(bool v)
{
  if (open_) {
    if (v) return false;
    open_ = false;
    if (type() & 1) { // horizontal
      saved_size = h();
      resize(x(), y(), w(), glyph_size_);
    } else {
      saved_size = w();
      resize(x(), y(), glyph_size_, h());
    }
  } else {
    if (!v) return false;
    open_ = true;
    if (type() & 1) // horizontal
      resize(x(), y(), w(), saved_size);
    else
      resize(x(), y(), saved_size, h());
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
