//
// "$Id: Fl_Value_Output.cxx,v 1.15 1999/06/15 17:02:32 gustavo Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Value_Output.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>
#include <stdio.h>
#include <config.h>

void Fl_Value_Output::output_cb(Fl_Widget*, void* v) {
  Fl_Value_Output& t = *(Fl_Value_Output*)v;
  double nv;
  if (t.step()>=1.0) nv = strtol(t.output.value(), 0, 0);
  else nv = strtod(t.output.value(), 0);
  t.handle_push();
  t.handle_drag(nv);
  t.handle_release();
}

void Fl_Value_Output::value_damage() {
  char buf[128];
  format(buf);
  output.value(buf);
//  output.mark(output.position()); // turn off selection highlight
}

void Fl_Value_Output::draw() {
  if (damage()&~FL_DAMAGE_CHILD) output.clear_damage(FL_DAMAGE_ALL);
  output.draw();
  output.clear_damage();
}

void Fl_Value_Output::layout() {
  Fl_Valuator::layout();
  output.resize(x(), y(), w(), h());output.layout();
}

int Fl_Value_Output::handle(int event) {
//  if (!step()) return 0;
  double v;
  int delta;
  int mx = Fl::event_x();
  static int ix, drag;
  switch (event) {
  case FL_ENTER: return 1; // For tooltips
  case FL_PUSH:
  if (!step()) goto DEFAULT;
    ix = mx;
    drag = Fl::event_button();
    handle_push();
    return 1;
  case FL_DRAG:
  if (!step()) goto DEFAULT;
    delta = Fl::event_x()-ix;
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
    if (!step()) goto DEFAULT;
    if (!Fl::pushed()) {
      if (value() != previous_value() || !Fl::event_is_click())
        handle_release();
      else {
        output.handle(FL_PUSH);
        output.handle(FL_RELEASE);
      }
    }
    return 1;
  default:
  DEFAULT:
    output.type(step()>=1.0 ? FL_INT_INPUT : FL_FLOAT_INPUT);
    return output.handle(event);
  }
}

Fl_Value_Output::Fl_Value_Output(int x,int y,int w,int h,const char *l)
: Fl_Valuator(x,y,w,h,l), output(x, y, w, h, 0) {
  align(FL_ALIGN_LEFT);
  soft_ = 0;
  if (output.parent())  // defeat automatic-add
    ((Fl_Group*)output.parent())->remove(output);
  output.parent(this); // kludge!
  output.callback(output_cb, this);
  output.when((Fl_When)(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY));
  value_damage();

  output.mstyle(&output._style);
  _style = output._style;
}

//
// End of "$Id: Fl_Value_Output.cxx,v 1.15 1999/06/15 17:02:32 gustavo Exp $".
//
