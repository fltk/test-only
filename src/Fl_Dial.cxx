//
// "$Id: Fl_Dial.cxx,v 1.38 2002/06/09 23:20:18 spitzak Exp $"
//
// Circular dial widget for the Fast Light Tool Kit (FLTK).
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
#include <fltk/Fl_Dial.h>
#include <fltk/Fl_Group.h>
#include <fltk/fl_draw.h>
#include <stdlib.h>
#include <fltk/math.h>

// All angles are measured with 0 to the right and counter-clockwise

void Fl_Dial::draw() {
  int X = 0; int Y = 0; int W = w(); int H = h();
  if (!(type() == FILL && box() == FL_OVAL_BOX)) {
    if (damage()&FL_DAMAGE_ALL) draw_box();
    box()->inset(X,Y,W,H);
  }
  Fl_Color fillcolor = selection_color();
  Fl_Color linecolor = highlight_color();
  if (!active_r()) {
    fillcolor = fl_inactive(fillcolor);
    linecolor = fl_inactive(linecolor);
  }
  double angle = (a2-a1)*(value()-minimum())/(maximum()-minimum()) + a1;
  if (type() == FILL) {
    if (damage()&FL_DAMAGE_EXPOSE && box() == FL_OVAL_BOX) {
      fl_push_clip(0, 0, w(), h());
      parent()->draw_group_box();
      fl_pop_clip();
    }
    fl_color(color());
    fl_pie(X, Y, W-1, H-1, 270-a1, angle > a1 ? 360+270-angle : 270-360-angle);
    fl_color(fillcolor);
    fl_pie(X, Y, W-1, H-1, 270-angle, 270-a1);
    if (box() == FL_OVAL_BOX) {
      fl_ellipse(X, Y, W-1, H-1);
      fl_color(linecolor); fl_stroke();
    }
  } else {
    if (!(damage()&FL_DAMAGE_ALL)) {
      fl_ellipse(X+1, Y+1, W-2, H-2);
      fl_color(color()); fl_fill();
    }
    fl_push_matrix();
    fl_translate(X+W/2-.5, Y+H/2-.5);
    fl_scale(W-1, H-1);
    fl_rotate(45-angle);
    if (type() == LINE) {
      fl_vertex(0.0,   0.0);
      fl_vertex(-0.04, 0.0);
      fl_vertex(-0.25, 0.25);
      fl_vertex(0.0,   0.04);
    } else {
      fl_circle(-0.20, 0.20, 0.07);
    }
    fl_color(fillcolor); fl_fill_stroke(linecolor);
    fl_pop_matrix();
  }
  if (focused()) {
    fl_ellipse(X+2, Y+2, W-5, H-5);
    fl_color(linecolor);
    fl_line_style(FL_DASH);
    fl_stroke();
    fl_line_style(0);
  }
}

int Fl_Dial::handle(int event) {
  int X = 0; int Y = 0; int W = w(); int H = h();
  box()->inset(X,Y,W,H);
  switch (event) {
  case FL_PUSH:
    handle_push();
  case FL_DRAG: {
    int mx = Fl::event_x()-X-W/2;
    int my = Fl::event_y()-Y-H/2;
    if (!mx && !my) return 1;
    double angle = 270-atan2((float)-my, (float)mx)*180/M_PI;
    double oldangle = (a2-a1)*(value()-minimum())/(maximum()-minimum()) + a1;
    while (angle < oldangle-180) angle += 360;
    while (angle > oldangle+180) angle -= 360;
    double val;
    if ((a1<a2) ? (angle <= a1) : (angle >= a1)) {
      val = minimum();
    } else if ((a1<a2) ? (angle >= a2) : (angle <= a2)) {
      val = maximum();
    } else {
      val = minimum() + (maximum()-minimum())*(angle-a1)/(a2-a1);
    }
    handle_drag(clamp(round(val)));
  } return 1;
  case FL_RELEASE:
    if (!Fl::pushed()) handle_release();
    return 1;
  default:
    return Fl_Valuator::handle(event);
  }
}

static void revert(Fl_Style* s) {
  s->box = FL_OVAL_BOX;
  s->selection_color = FL_DARK2;
  s->highlight_color = FL_BLACK;
}
static Fl_Named_Style style("Dial", revert, &Fl_Dial::default_style);
Fl_Named_Style* Fl_Dial::default_style = &::style;

Fl_Dial::Fl_Dial(int x, int y, int w, int h, const char* l)
  : Fl_Valuator(x, y, w, h, l) {
  style(default_style);
  a1 = 45;
  a2 = 315;
  //set_click_to_focus();
}

//
// End of "$Id: Fl_Dial.cxx,v 1.38 2002/06/09 23:20:18 spitzak Exp $".
//
