//
// "$Id: Fl_Adjuster.cxx,v 1.45 2002/12/10 01:46:16 easysw Exp $"
//
// Adjuster widget for the Fast Light Tool Kit (FLTK).
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


#include <fltk/Adjuster.h>
#include <fltk/Box.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/draw.h>
#include <fltk/xbmImage.h>
using namespace fltk;

#include "fastarrow.h"
static xbmImage fastarrow(fastarrow_bits, fastarrow_width, fastarrow_height);
#include "mediumarrow.h"
static xbmImage mediumarrow(mediumarrow_bits, mediumarrow_width, mediumarrow_height);
#include "slowarrow.h"
static xbmImage slowarrow(slowarrow_bits, slowarrow_width, slowarrow_height);

static xbmImage* arrows[3] = {&fastarrow,&mediumarrow,&slowarrow};

enum {
  GLYPH_FASTARROW,
  GLYPH_MEDIUMARROW,
  GLYPH_SLOWARROW
};

static void glyph(const Widget* widget, int t,
		  int x,int y,int w,int h, Flags flags)
{
  Color color;

  if (flags & HIGHLIGHT && (color = widget->highlight_color())) ;
  else color = widget->buttoncolor();
  widget->buttonbox()->draw(x,y,w,h, color, flags);

  if (flags&HIGHLIGHT && (color = widget->highlight_labelcolor()));
  else color = inactive(widget->textcolor(),flags);
  setcolor(color);
  xbmImage* b = arrows[t-GLYPH_FASTARROW];
  b->draw(x+((w-b->width())>>1), y+((h-b->height())>>1));
}

// changing the value does not change the appearance:
void Adjuster::value_damage() {}

void Adjuster::draw() {
  int dx, dy, W, H;
  if (w()>=h()) {
    dx = W = w()/3;
    dy = 0; H = h();
  } else {
    dx = 0; W = w();
    dy = H = h()/3;
  }

  Flags f[4];
  for (int i = 1; i < 4; i++) {
    f[i] = flags();
    if (drag == i) f[i] |= VALUE;
    else if (highlight == i) f[i] |= HIGHLIGHT;
  }

  if (damage()&DAMAGE_ALL || last == 1 || highlight == 1)
    draw_glyph(GLYPH_FASTARROW, 0, 2*dy, W, H, f[1]);
  if (damage()&DAMAGE_ALL || last == 2 || highlight == 2)
    draw_glyph(GLYPH_MEDIUMARROW, dx, dy, W, H, f[2]);
  if (damage()&DAMAGE_ALL || last == 3 || highlight == 3)
    draw_glyph(GLYPH_SLOWARROW, 2*dx, 0, W, H, f[3]);

  if (focused()) {
    Box* box = buttonbox();
    focusbox()->draw(box->dx()+1, box->dy()+1,
		      w()-box->dw()-2, h()-box->dh()-2,
		      textcolor(), INVISIBLE);
  }

  last = highlight;
}

int Adjuster::handle(int event) {
  int delta;
  int mx = event_x();

  int which_button;
  if (w()>=h())
    which_button = 3*mx/w() + 1;
  else
    which_button = 3-3*(event_y()-1)/h();
  if (which_button > 3) which_button = 3;
  else if (which_button < 1) which_button = 1;

  switch (event) {

  case PUSH:
    handle_push();
    ix = mx;
    drag = highlight = which_button;
    redraw();
    return 1;

  case DRAG:
    if (w() >= h()) {
      delta = (drag-1)*w()/3;	// left edge of button
      if (mx < delta)
	delta = mx-delta;
      else if (mx > (delta+w()/3)) // right edge of button
	delta = mx-delta-w()/3;
      else
	delta = 0;
    } else {
      if (mx < 0)
	delta = mx;
      else if (mx > w())
	delta = mx-w();
      else
	delta = 0;
    }
    if (drag == 1) delta *= 100; else if (drag == 2) delta *= 10;
    handle_drag(previous_value() + (step() ? step() : .01f) * delta);
    return 1;

  case RELEASE:
    if (pushed()) return 1;
    if (event_is_click()) { // detect click but no drag
      if (drag == 1) delta = 1000; else if (drag == 2) delta = 100;
      else delta = 10;
      if (event_state()& (SHIFT|CAPSLOCK|CTRL|ALT)) 
	delta = -delta;
      handle_drag(previous_value() + (step() ? step() : .01f) * delta);
    }
    drag = 0;
    redraw();
    handle_release();
    return 1;

  case LEAVE:
    which_button = 0;
  case ENTER:
  case MOVE:
    highlight = which_button;
    if (last != highlight) redraw();
    return 1;

  case FOCUS:
  case UNFOCUS:
    redraw();
  default:
    return Valuator::handle(event);
  }
}

static void revert(Style* s) {
  s->selection_color = GRAY75;
  s->selection_textcolor = BLACK;
  s->box = NO_BOX; // for compatability if in the future it draws the box
  s->glyph = glyph;
}
static NamedStyle style("Adjuster", revert, &Adjuster::default_style);
NamedStyle* Adjuster::default_style = &::style;

Adjuster::Adjuster(int x,int y,int w,int h,const char *l) : Valuator(x,y,w,h,l) {
  style(default_style);
  step(1.0/10000);
  drag = highlight = 0;
  soft_ = 1;
  //  set_click_to_focus();
}

//
// End of "$Id: Fl_Adjuster.cxx,v 1.45 2002/12/10 01:46:16 easysw Exp $".
//
