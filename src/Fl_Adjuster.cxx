//
// "$Id: Fl_Adjuster.cxx,v 1.33 2001/07/23 09:50:04 spitzak Exp $"
//
// Adjuster widget for the Fast Light Tool Kit (FLTK).
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


#include <fltk/Fl.h>
#include <fltk/Fl_Adjuster.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Bitmap.h>

#include "fastarrow.h"
static Fl_Bitmap fastarrow(fastarrow_bits, fastarrow_width, fastarrow_height);
#include "mediumarrow.h"
static Fl_Bitmap mediumarrow(mediumarrow_bits, mediumarrow_width, mediumarrow_height);
#include "slowarrow.h"
static Fl_Bitmap slowarrow(slowarrow_bits, slowarrow_width, slowarrow_height);

static Fl_Bitmap* arrows[3] = {&fastarrow,&mediumarrow,&slowarrow};

enum {
  FL_GLYPH_FASTARROW,
  FL_GLYPH_MEDIUMARROW,
  FL_GLYPH_SLOWARROW
};

static void glyph(const Fl_Widget* widget, int t,
		  int x,int y,int w,int h, Fl_Flags f)
{
  widget->box()->draw(x,y,w,h,widget->get_box_color(f),f);
  fl_color(widget->get_glyph_color(f));
  Fl_Bitmap* b = arrows[t-FL_GLYPH_FASTARROW];
  b->draw(x,y,w,h, (b->w-w)/2, (b->h-h)/2);
}

// changing the value does not change the appearance:
void Fl_Adjuster::value_damage() {}

extern void fl_dotted_box(int,int,int,int);

void Fl_Adjuster::draw() {
  int dx, dy, W, H;
  if (w()>=h()) {
    dx = W = w()/3;
    dy = 0; H = h();
  } else {
    dx = 0; W = w();
    dy = H = h()/3;
  }

  Fl_Flags f[4];
  for (int i = 1; i < 4; i++) {
    f[i] = flags();
    if (drag == i) f[i] |= FL_VALUE|FL_SELECTED;
    else if (highlight == i) f[i] |= FL_HIGHLIGHT;
  }

  if (damage()&FL_DAMAGE_ALL || last == 1 || highlight == 1)
    draw_glyph(FL_GLYPH_FASTARROW, 0, 2*dy, W, H, f[1]);
  if (damage()&FL_DAMAGE_ALL || last == 2 || highlight == 2)
    draw_glyph(FL_GLYPH_MEDIUMARROW, dx, dy, W, H, f[2]);
  if (damage()&FL_DAMAGE_ALL || last == 3 || highlight == 3)
    draw_glyph(FL_GLYPH_SLOWARROW, 2*dx, 0, W, H, f[3]);

  if (focused()) {
    Fl_Boxtype box = this->box();
    fl_color(get_glyph_color());
    fl_dotted_box(box->dx()+1, box->dy()+1, w()-box->dw()-2, h()-box->dh()-2);
  }

  last = highlight;
}

int Fl_Adjuster::handle(int event) {
  double v;
  int delta;
  int mx = Fl::event_x();

  int which_button;
  if (w()>=h())
    which_button = 3*mx/w() + 1;
  else
    which_button = 3-3*(Fl::event_y()-1)/h();
  if (which_button > 3) which_button = 3;
  else if (which_button < 1) which_button = 1;

  switch (event) {

  case FL_PUSH:
#ifdef WINDOWS_COMPATABILITY
    take_focus();
#endif
    handle_push();
    ix = mx;
    drag = highlight = which_button;
    redraw();
    return 1;

  case FL_DRAG:
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
    v = increment(previous_value(), delta);
    handle_drag(soft() ? softclamp(v) : clamp(v));
    return 1;

  case FL_RELEASE:
    if (Fl::pushed()) return 1;
    if (Fl::event_is_click()) { // detect click but no drag
      if (drag == 1) delta = 1000; else if (drag == 2) delta = 100;
      else delta = 10;
      if (Fl::event_state()& (FL_SHIFT|FL_CAPS_LOCK|FL_CTRL|FL_ALT)) 
	delta = -delta;
      v = increment(value(), delta);
      handle_drag(soft() ? softclamp(v) : clamp(v));
    }
    drag = 0;
    redraw();
    handle_release();
    return 1;

  case FL_LEAVE:
    which_button = 0;
  case FL_ENTER:
  case FL_MOVE:
    highlight = which_button;
    if (last != highlight) redraw();
    return 1;

  case FL_FOCUS:
  case FL_UNFOCUS:
    redraw();
  default:
    return Fl_Valuator::handle(event);
  }
}

static void revert(Fl_Style* s) {
  s->selection_color = FL_GRAY;
  s->selection_text_color = FL_BLACK;
  s->text_box = FL_NO_BOX;
  s->glyph = glyph;
}
static Fl_Named_Style style("Adjuster", revert, &Fl_Adjuster::default_style);
Fl_Named_Style* Fl_Adjuster::default_style = &::style;

Fl_Adjuster::Fl_Adjuster(int x,int y,int w,int h,const char *l) : Fl_Valuator(x,y,w,h,l) {
  style(default_style);
  step(1/10000);
  drag = highlight = 0;
  soft_ = 1;
}

//
// End of "$Id: Fl_Adjuster.cxx,v 1.33 2001/07/23 09:50:04 spitzak Exp $".
//
