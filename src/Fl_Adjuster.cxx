//
// "$Id: Fl_Adjuster.cxx,v 1.13 1999/11/01 02:21:29 carl Exp $"
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


#include <FL/Fl.H>
#include <FL/Fl_Adjuster.H>
#include <FL/fl_draw.H>


// changing the value does not change the appearance:
void Fl_Adjuster::value_damage() {}

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
    if (!active_r()) { f[i] |= FL_INACTIVE; highlight = 0; }
    if (drag == i) f[i] |= FL_VALUE;
    else if (highlight == i) f[i] |= FL_HIGHLIGHT;
  }

  if (damage()&FL_DAMAGE_ALL || last == 1 || highlight == 1)
    draw_glyph(FL_GLYPH_FASTARROW, x(), y()+2*dy, W, H, f[1]);
  if (damage()&FL_DAMAGE_ALL || last == 2 || highlight == 2)
    draw_glyph(FL_GLYPH_MEDIUMARROW, x()+dx, y()+dy, W, H, f[2]);
  if (damage()&FL_DAMAGE_ALL || last == 3 || highlight == 3)
    draw_glyph(FL_GLYPH_SLOWARROW, x()+2*dx, y(), W, H, f[3]);
  last = highlight;
}

int Fl_Adjuster::handle(int event) {
  double v;
  int delta;
  int mx = Fl::event_x();

  if (Fl::event_inside(this)) {
    if (w()>=h())
      highlight = 3*(mx-x())/w() + 1;
    else
      highlight = 3-3*(Fl::event_y()-y()-1)/h();
  } else highlight = 0;

  switch (event) {
  case FL_PUSH:
    ix = mx;
    if (!drag) {
      drag = highlight;
      handle_push();
      redraw();
    }
    return 1;
  case FL_DRAG:
    if (w() >= h()) {
      delta = x()+(drag-1)*w()/3;	// left edge of button
      if (mx < delta)
	delta = mx-delta;
      else if (mx > (delta+w()/3)) // right edge of button
	delta = mx-delta-w()/3;
      else
	delta = 0;
    } else {
      if (mx < x())
	delta = mx-x();
      else if (mx > (x()+w()))
	delta = mx-x()-w();
      else
	delta = 0;
    }
    switch (drag) {
    case 3: v = increment(previous_value(), delta); break;
    case 2: v = increment(previous_value(), delta*10); break;
    default:v = increment(previous_value(), delta*100); break;
    }
    handle_drag(soft() ? softclamp(v) : clamp(v));
    return 1;
  case FL_RELEASE:
    if (Fl::event_is_click()) { // detect click but no drag
      if (Fl::event_state()& (FL_SHIFT|FL_CAPS_LOCK|FL_CTRL|FL_ALT) ) 
	delta = -10;
	else delta = 10;
      switch (drag) {
      case 3: v = increment(previous_value(), delta); break;
      case 2: v = increment(previous_value(), delta*10); break;
      default:v = increment(previous_value(), delta*100); break;
      }
      handle_drag(soft() ? softclamp(v) : clamp(v));
    }
    if (!Fl::pushed()) {
      drag = 0;
      redraw();
      handle_release();
    }
    return 1;
  case FL_MOVE:
    if (last == highlight) return 1;
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
    return 1;
  }
  return 0;
}

Fl_Adjuster::Fl_Adjuster(int x,int y,int w,int h,const char *l) : Fl_Valuator(x,y,w,h,l) {
  style(default_style);
  step(1, 10000);
  drag = highlight = 0;
  soft_ = 1;
}

Fl_Style Fl_Adjuster::default_style = {
  0,                    // box
  0,                    // glyph_box
  0,	                // glyphs
  0,		        // label_font
  0,		        // text_font
  0,		        // label_type
  0,		        // color
  0,		        // label_color
  0,                    // selection_color / on_color
  0,		        // selection_text_color
  FL_GRAY,              // off_color - button color
  FL_LIGHT2             // highlight color
};

static Fl_Style_Definer x("adjuster", Fl_Adjuster::default_style);

//
// End of "$Id: Fl_Adjuster.cxx,v 1.13 1999/11/01 02:21:29 carl Exp $".
//
