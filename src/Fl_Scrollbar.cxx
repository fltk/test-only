//
// "$Id: Fl_Scrollbar.cxx,v 1.15 1999/05/06 05:52:20 carl Exp $"
//
// Scroll bar widget for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Scrollbar.H>
#include <FL/fl_draw.H>
#include <math.h>

#define DEFAULT_STYLE ((Style*)default_style())

#define INITIALREPEAT .5
#define REPEAT .05

void Fl_Scrollbar::increment_cb() {
  handle_drag(clamp(value() + (
	((pushed_>1) == (maximum()>=minimum())) ? linesize_ : -linesize_)));
}

void Fl_Scrollbar::timeout_cb(void* v) {
  Fl_Scrollbar* s = (Fl_Scrollbar*)v;
  s->increment_cb();
  Fl::add_timeout(REPEAT, timeout_cb, s);
}

int Fl_Scrollbar::handle(int event) {
  if (!pushed_) {
    if (horizontal()) {
      if (w() < 3*h()) return Fl_Slider::handle(event);
      if (Fl_Slider::handle(event, x()+h(), y(), w()-2*h(), h())) return 1;
    } else {
      if (h() < 3*w()) return Fl_Slider::handle(event);
      if (Fl_Slider::handle(event, x(), y()+w(), w(), h()-2*w())) return 1;
    }
  }
  switch (event) {
  case FL_RELEASE:
    if (!Fl::pushed()) {
      if (pushed_) {
        Fl::remove_timeout(timeout_cb, this);
        pushed_ = 0;
        redraw();
      }
      handle_release();
    }
    return 1;
  case FL_PUSH:
    if (horizontal()) {
      int delta = (Fl::widget_style() == FL_MOTIF_STYLE) ? Fl::box_dx(box()) : 0;
      if (Fl::event_inside(x(), y(), h()-delta, h())) pushed_ = 1;
      if (Fl::event_inside(x()+w()-h()+delta, y(), h()-delta, h())) pushed_ = 2;
    } else {
      int delta = (Fl::widget_style() == FL_MOTIF_STYLE) ? Fl::box_dy(box()) : 0;
      if (Fl::event_inside(x(), y(), w()-delta, w())) pushed_ = 1;
      if (Fl::event_inside(x(), y()+h()-w()+delta, w()-delta, w())) pushed_ = 2;
    }
    if (pushed_) {
      handle_push();
      Fl::add_timeout(INITIALREPEAT, timeout_cb, this);
      increment_cb();
      redraw();
    }
    return 1;
  case FL_DRAG:
    return pushed_;
  case FL_SHORTCUT: {
    int v = value();
    int ls = maximum()>=minimum() ? linesize_ : -linesize_;
    if (horizontal()) {
      switch (Fl::event_key()) {
      case FL_Left:
	v -= ls;
	break;
      case FL_Right:
	v += ls;
	break;
      default:
	return 0;
      }
    } else { // vertical
      switch (Fl::event_key()) {
      case FL_Up:
	v -= ls;
	break;
      case FL_Down:
	v += ls;
	break;
      case FL_Page_Up:
	if (slider_size() >= 1.0) return 0;
	v -= int((maximum()-minimum())*slider_size()/(1.0-slider_size()));
	v += ls;
	break;
      case FL_Page_Down:
	if (slider_size() >= 1.0) return 0;
	v += int((maximum()-minimum())*slider_size()/(1.0-slider_size()));
	v -= ls;
	break;
      case FL_Home:
	v = int(minimum());
	break;
      case FL_End:
	v = int(maximum());
	break;
      default:
	return 0;
      }
    }
    v = int(clamp(v));
    if (v != value()) {
      Fl_Slider::value(v);
      value_damage();
      do_callback();
    }
    return 1;}
  }
  return 0;
}

void Fl_Scrollbar::draw() {
  Fl_Color col = (Fl::belowmouse()==this &&
                  selection_color() == DEFAULT_STYLE->widget(COLOR2))
                 ? fly_color() : selection_color();

  Fl_Color dc1, dc2, lc1, lc2;
  if (slider() == FL_MOTIF_UP_BOX || slider() == FL_THIN_MOTIF_UP_BOX)
    { dc1 = FL_DARK3; dc2 = FL_DARK3; lc1 = FL_LIGHT2; lc2 = FL_LIGHT2; }
  else
    { dc1 = FL_BLACK; dc2 = FL_DARK2; lc1 = FL_WHITE; lc2 = FL_LIGHT2; }
  if (!active_r())
    { dc1 = inactive(dc1); dc2 = inactive(dc2); lc1 = inactive(lc1); lc2 = inactive(lc2); }
  int bdx = Fl::box_dx(box()), bdy = Fl::box_dy(box());

  if (horizontal()) {
    if (w() < 3*h()) {Fl_Slider::draw(); return;}
    if (Fl::widget_style() == FL_MOTIF_STYLE) {
      fl_clip(x()+h()-1, y(), w()-2*h()+2, h());
      Fl_Slider::draw(x()+h()-bdx-1, y(), w()-2*h()+2*bdx+2, h());
      fl_pop_clip();
    } else {
      Fl_Slider::draw(x()+h(), y(), w()-2*h(), h());
    }
    if (damage()&FL_DAMAGE_ALL) {
      if (Fl::widget_style() == FL_MOTIF_STYLE) {
        fl_clip(x(), y(), h()-1, h());
        draw_box(box(), x(), y(), h()+2*bdx, h(), color());
        fl_pop_clip();
        fl_clip(x()+w()-h()+1, y(), h()-1, h());
        draw_box(box(), x()+w()-h()-2*bdx, y(), h()+2*bdx, h(), color());
        fl_pop_clip();
      } else {
        draw_box((pushed_&1) ? down(slider()) : slider(),
                 x(), y(), h(), h(), col);
        draw_box((pushed_&2) ? down(slider()) : slider(),
                 x()+w()-h(), y(), h(), h(), col);
      }

      if (active_r()) fl_color(color3());
      else fl_color(inactive(color3()));
      int w1 = (h()-1)|1; // use odd sizes only
      int Y = y()+w1/2;
      // I have no idea what I'm doing, but this looks OK - CET
      int WX, WY, DX;
      if (Fl::widget_style() == FL_SGI_STYLE) {
        WX = ((w1/2)-1)|1;
        WY = (w1/4);
        DX = 0;
      } else if (Fl::widget_style() == FL_MOTIF_STYLE) {
        WX = (w1 - 2*bdx - 1)|1;
        WY = (w1/2 - bdx);
        DX = bdx-1;
      } else {
        WX = w1/3;
        WY = w1/3;
        DX = 0;
      }
      int X = x()+w1/2+WX/2;
      if (Fl::widget_style() == FL_MOTIF_STYLE)
        fl_color((pushed_&1) ? color() : col);
      fl_polygon(X-WX+DX, Y, X+DX, Y-WY, X+DX, Y+WY);
      X = x()+w()-(X-x())-1;
      if (Fl::widget_style() == FL_MOTIF_STYLE)
        fl_color((pushed_&2) ? color() : col);
      fl_polygon(X+WX-DX, Y, X-DX, Y+WY, X-DX, Y-WY);
      if (Fl::widget_style() == FL_MOTIF_STYLE) {
        X = x()+w1/2+WX/2;
        fl_color((pushed_&1) ? lc2 : dc2);
        fl_line(X-WX+DX+1, Y, X+DX-1, Y+WY-1, X+DX-1, Y-WY+1);
        fl_color((pushed_&1) ? dc2 : lc2);
        fl_line(X-WX+DX+1, Y, X+DX-1, Y-WY+1);
        fl_color((pushed_&1) ? lc1 : dc1);
        fl_line(X-WX+DX, Y, X+DX, Y+WY, X+DX, Y-WY);
        fl_color((pushed_&1) ? dc1 : lc1);
        fl_line(X-WX+DX, Y, X+DX, Y-WY);

        X = x()+w()-(X-x())-1;
        fl_color((pushed_&2) ? dc2 : lc2);
        fl_line(X+WX-DX-1, Y, X-DX+1, Y-WY+1, X-DX+1, Y+WY-1);
        fl_color((pushed_&2) ? lc2 : dc2);
        fl_line(X+WX-DX-1, Y, X-DX+1, Y+WY-1);
        fl_color((pushed_&2) ? dc1 : lc1);
        fl_line(X+WX-DX, Y, X-DX, Y-WY, X-DX, Y+WY);
        fl_color((pushed_&2) ? lc1 : dc1);
        fl_line(X+WX-DX, Y, X-DX, Y+WY);
      }
    }
  } else { // vertical
    if (h() < 3*w()) {Fl_Slider::draw(); return;}
    if (Fl::widget_style() == FL_MOTIF_STYLE) {
      fl_clip(x(), y()+w()-1, w(), h()-2*w()+2);
      Fl_Slider::draw(x(), y()+w()-bdy-1, w(), h()-2*w()+2*bdy+2);
      fl_pop_clip();
    } else {
      Fl_Slider::draw(x(), y()+w(), w(), h()-2*w());
    }
    if (damage()&FL_DAMAGE_ALL) {
      if (Fl::widget_style() == FL_MOTIF_STYLE) {
        fl_clip(x(), y(), w(), w()-1);
        draw_box(box(), x(), y(), w(), w()+2*bdy, color());
        fl_pop_clip();
        fl_clip(x(), y()+h()-w()+1, w(), w()-1);
        draw_box(box(), x(), y()+h()-w()-2*bdy, w(), w()+2*bdy, color());
        fl_pop_clip();
      } else {
        draw_box((pushed_&1) ? down(slider()) : slider(),
                 x(), y(), w(), w(), col);
        draw_box((pushed_&2) ? down(slider()) : slider(),
                 x(), y()+h()-w(), w(), w(), col);
      }

      if (active_r()) fl_color(color3());
      else fl_color(inactive(color3()));
      int w1 = (w()-1)|1; // use odd sizes only
      int X = x()+w1/2;
      // I have no idea what I'm doing, but this looks OK - CET
      int WX, WY, DY;
      if (Fl::widget_style() == FL_SGI_STYLE) {
        WY = ((w1/2)-1)|1;
        WX = (w1/4);
        DY = 0;
      } else if (Fl::widget_style() == FL_MOTIF_STYLE) {
        WY = (w1 - 2*bdy - 1)|1;
        WX = (w1/2 - bdy);
        DY = bdy-1;
      } else {
        WY = w1/3;
        WX = w1/3;
        DY = 0;
      }
      int Y = y()+w1/2+WY/2;
      if (Fl::widget_style() == FL_MOTIF_STYLE)
        fl_color((pushed_&1) ? color() : col);
      fl_polygon(X, Y-WY+DY, X+WX, Y+DY, X-WX, Y+DY);
      Y = y()+h()-(Y-y())-1;
      if (Fl::widget_style() == FL_MOTIF_STYLE)
        fl_color((pushed_&2) ? color() : col);
      fl_polygon(X, Y+WY-DY, X-WX, Y-DY, X+WX, Y-DY);
      if (Fl::widget_style() == FL_MOTIF_STYLE) {
        Y = y()+w1/2+WY/2;
        fl_color((pushed_&1) ? lc2 : dc2);
        fl_line(X, Y-WY+DY+1, X+WX-1, Y+DY-1, X-WX+1, Y+DY-1);
        fl_color((pushed_&1) ? dc2 : lc2);
        fl_line(X, Y-WY+DY+1, X-WX+1, Y+DY-1);
        fl_color((pushed_&1) ? lc1 : dc1);
        fl_line(X, Y-WY+DY, X+WX, Y+DY, X-WX, Y+DY);
        fl_color((pushed_&1) ? dc1 : lc1);
        fl_line(X, Y-WY+DY, X-WX, Y+DY);

        Y = y()+h()-(Y-y())-1;
        fl_color((pushed_&2) ? dc2 : lc2);
        fl_line(X, Y+WY-DY-1, X-WX+1, Y-DY+1, X+WX-1, Y-DY+1);
        fl_color((pushed_&2) ? lc2 : dc2);
        fl_line(X, Y+WY-DY-1, X+WX-1, Y-DY+1);
        fl_color((pushed_&2) ? dc1 : lc1);
        fl_line(X, Y+WY-DY, X-WX, Y-DY, X+WX, Y-DY);
        fl_color((pushed_&2) ? lc1 : dc1);
        fl_line(X, Y+WY-DY, X+WX, Y-DY);
      }
    }
  }
}
Fl_Widget::Style* Fl_Scrollbar::_default_style = 0;

Fl_Scrollbar::Style::Style() : Fl_Slider::Style() {
  widget(COLOR) = 43;
  widget(COLOR3) = 0;
  widget(BOX) = FL_FLAT_BOX;
}

void Fl_Scrollbar::loadstyle() const {
  if (!Fl::s_scrollbar) {
    Fl::s_scrollbar = 1;

    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "color2", COLOR2 },
      { "arrow color", COLOR3 },
      { "box", BOX },
      { 0 }
    };
    Fl::load_attributes("scroll bar", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute slider_attributes[] = {
     { "highlight color", FLY_COLOR },
     { "slider box", SLIDER_BOX },
     { 0 }
    };
    Fl::load_attributes("scroll bar", DEFAULT_STYLE->slider_, slider_attributes);
  }
}

Fl_Scrollbar::Fl_Scrollbar(int X, int Y, int W, int H, const char* L) : Fl_Slider(X, Y, W, H, L) {
  linesize_ = 16;
  pushed_ = 0;
  step(1);
}

//
// End of "$Id: Fl_Scrollbar.cxx,v 1.15 1999/05/06 05:52:20 carl Exp $".
//
