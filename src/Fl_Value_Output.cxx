//
// "$Id: Fl_Value_Output.cxx,v 1.30 2001/07/23 09:50:05 spitzak Exp $"
//
// Value output widget for the Fast Light Tool Kit (FLTK).
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

// Fltk widget for drag-adjusting a floating point value.
// This is much lighter than Fl_Value_Input because it has no text editor
// If step() is zero then it can be used to display a floating-point value

#include <fltk/Fl.h>
#include <fltk/Fl_Value_Output.h>
#include <fltk/fl_draw.h>

void Fl_Value_Output::draw() {
  int X=0; int Y=0; int W=w(); int H=h(); box()->inset(X,Y,W,H);
  if (damage()&(FL_DAMAGE_ALL|FL_DAMAGE_HIGHLIGHT)) {
    draw_text_box();
  } else {
    fl_color(text_background());
    fl_rectf(X+2, Y+2, W-4, H-4);
  }
  char buf[128];
  format(buf);
  fl_font(text_font(), text_size());
  if (focused()) {
    fl_color(selection_color());
    int h = fl_height(text_font(), text_size())+leading();
    fl_rectf(X+3,Y+(H-h)/2,fl_width(buf),h);
    fl_color(selection_text_color());
  } else {
    fl_color(active_r() ? text_color() : fl_inactive(text_color()));
  }
  fl_draw(buf,X+3,Y,W-6,H,FL_ALIGN_LEFT);
}

int Fl_Value_Output::handle(int event) {
  if (!step()) return 0;
  double v;
  int delta;
  int mx = Fl::event_x()-Fl::event_y();
  static int ix, drag;
  switch (event) {
  case FL_PUSH:
    take_focus();
    ix = mx;
    drag = Fl::event_button();
    handle_push();
    return 1;
  case FL_DRAG:
    delta = mx-ix;
    if (delta > 5) delta -= 5;
    else if (delta < -5) delta += 5;
    else delta = 0;
    switch (drag) {
    case 3: v = increment(previous_value(),delta*100); break;
    case 2: v = increment(previous_value(),delta*10); break;
    default:v = increment(previous_value(),delta); break;
    }
    v = round(v);
    handle_drag(soft()?softclamp(v):clamp(v));;
    return 1;
  case FL_RELEASE:
    handle_release();
    return 1;
  default:
    return Fl_Valuator::handle(event);
  }
}

#include <fltk/Fl_Output.h>

Fl_Value_Output::Fl_Value_Output(int x,int y,int w,int h,const char *l)
: Fl_Valuator(x,y,w,h,l) {
  style(Fl_Output::default_style);
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_LEFT);
  soft_ = 0;
}

//
// End of "$Id: Fl_Value_Output.cxx,v 1.30 2001/07/23 09:50:05 spitzak Exp $".
//
