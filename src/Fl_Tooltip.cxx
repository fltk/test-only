//
// "$Id: Fl_Tooltip.cxx,v 1.41 2002/02/10 22:57:49 spitzak Exp $"
//
// Tooltip code for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl_Tooltip.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Menu_Window.h>

float Fl_Tooltip::delay_ = 0.5f;
int Fl_Tooltip::enabled_ = 1;

#define MAX_WIDTH 400

class Fl_TooltipBox : public Fl_Menu_Window {
public:
  Fl_TooltipBox() : Fl_Menu_Window(0, 0, 0, 0) {
    style(Fl_Tooltip::default_style);}
  void draw();
  void layout();
};

static const char* tip;
static Fl_Widget* widget;
static Fl_TooltipBox *window = 0;
static int X,Y,W,H;

void Fl_TooltipBox::layout() {
  fl_font(label_font(), label_size());
  int ww, hh;
  ww = MAX_WIDTH;
  fl_measure(tip, ww, hh, FL_ALIGN_LEFT|FL_ALIGN_WRAP|FL_ALIGN_INSIDE);
  ww += 6; hh += 6;

  // find position on the screen of the widget:
  int ox = Fl::event_x_root();
  //int ox = X+W/2;
  int oy = Y + H+2;
  for (Fl_Widget* p = widget; p; p = p->parent()) {
    //ox += p->x();
    oy += p->y();
  }
  if (ox+ww > Fl::w()) ox = Fl::w() - ww;
  if (ox < 0) ox = 0;
  if (H > 30) {
    oy = Fl::event_y_root()+13;
    if (oy+hh > Fl::h()) oy -= 23+hh;
  } else {
    if (oy+hh > Fl::h()) oy -= (4+hh+H);
  }
  if (oy < 0) oy = 0;

  resize(ox, oy, ww, hh);
  Fl_Menu_Window::layout();
}

void Fl_TooltipBox::draw() {
  draw_box();
  draw_label(3, 3, w()-6, h()-6, FL_ALIGN_LEFT|FL_ALIGN_WRAP|FL_ALIGN_INSIDE);
}

static void tooltip_timeout(void*) {
  //if (Fl::grab()) return;

  if (window) delete window;
  Fl_Group::current(0);
  window = new Fl_TooltipBox;
  window->set_override();
  window->end();

  // this cast bypasses the normal Fl_Window label() code:
  ((Fl_Widget*)window)->label(tip);
  window->relayout();
  window->show();
}

static bool cheesy_flag;

static void
tt_exit(Fl_Widget *w) {
  if (!w || w != widget) return;
  widget = 0;
  Fl::remove_timeout(tooltip_timeout);
  if (window) {
    // This flag makes sure that tootip_enter() isn't executed because of
    // this destroy() which could cause unwanted recursion in tooltip_enter()
    cheesy_flag = true;
    window->destroy();
    cheesy_flag = false;
  }
}

static void
tt_enter_area(Fl_Widget* w, int X, int Y, int W, int H, const char* t) {
  if (cheesy_flag) return;
  if (w == widget && X == ::X && Y == ::Y && W == ::W && H == ::H && t == tip)
    return;
  tt_exit(widget);
  Fl::remove_timeout(tooltip_timeout);
  widget = w; ::X = X; ::Y = Y; ::W = W; ::H = H; tip = t;
  if (!t || !Fl_Tooltip::enabled()) return;
  float d = Fl_Tooltip::delay();
  Fl::add_timeout(d, tooltip_timeout);
}

static void
tt_enter(Fl_Widget* w) {
  if (cheesy_flag) return;
  // find the enclosing group with a tooltip:
  while (w && !w->tooltip()) w = w->parent();
  if (w == widget || w == window) return;
  if (!w || !*(w->tooltip())) tt_exit(widget);
  else tt_enter_area(w, 0, 0, w->w(), w->h(), w->tooltip());
}

void Fl_Widget::tooltip(const char *tt) {
  static bool beenhere = false;
  if (!beenhere) {
    beenhere = true;
    Fl_Tooltip::enter = tt_enter;
    Fl_Tooltip::enter_area = tt_enter_area;
    Fl_Tooltip::exit = tt_exit;
  }
  tooltip_ = tt;
}

static void revert(Fl_Style* s) {
  s->box = FL_BORDER_BOX;
  s->color = (Fl_Color)215;
  s->text_color = FL_DARK3;
  s->label_color = FL_BLACK;
}
static Fl_Named_Style style("Tooltip", revert, &Fl_Tooltip::default_style);
Fl_Named_Style* Fl_Tooltip::default_style = &::style;

//
// End of "$Id: Fl_Tooltip.cxx,v 1.41 2002/02/10 22:57:49 spitzak Exp $".
//
