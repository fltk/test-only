//
// "$Id: Fl_Tooltip.cxx,v 1.43 2002/04/25 16:39:33 spitzak Exp $"
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

float Fl_Tooltip::delay_ = 1.0f;
bool Fl_Tooltip::enabled_ = true;

#define MAX_WIDTH 400

class Fl_TooltipBox : public Fl_Menu_Window {
public:
  Fl_TooltipBox() : Fl_Menu_Window(0, 0) {
    style(Fl_Tooltip::default_style);
    set_override();
    end();
  }
  void draw();
  void layout();
#ifdef _WIN32
  // You have to destroy the window or it will not raise next time:
  void hide() {destroy();}
#endif
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

static bool recent_tooltip;

static void recent_timeout(void*) {
  recent_tooltip = false;
}

static bool recursion;

static void tooltip_timeout(void*) {
  if (recursion) return;
  recursion = true;
  if (!tip || !*tip) return;
  //if (Fl::grab()) return;
  if (!window) window = new Fl_TooltipBox;
  // this cast bypasses the normal Fl_Window label() code:
  ((Fl_Widget*)window)->label(tip);
  window->redraw();
  window->relayout();
  window->show();
  Fl::remove_timeout(recent_timeout);
  recent_tooltip = true;
  recursion = false;
}

// This is called when a widget is destroyed:
static void
tt_exit(Fl_Widget *w) {
  if (w && w == widget) Fl_Tooltip::enter_area(0,0,0,0,0,0);
}

static void
tt_enter(Fl_Widget* widget) {
  // find the enclosing group with a tooltip:
  Fl_Widget* w = widget;
  while (w && !w->tooltip()) {
    if (w == window) return; // don't do anything if pointed at tooltip
    w = w->parent();
  }
  if (!w) {
    Fl_Tooltip::enter_area(0, 0, 0, 0, 0, 0);
  } else {
    Fl_Tooltip::enter_area(widget,0,0,widget->w(), widget->h(), w->tooltip());
  }
}
#include <stdio.h>

void
Fl_Tooltip::enter_area(Fl_Widget* wid, int x,int y,int w,int h, const char* t)
{
  if (recursion) return;
  Fl::remove_timeout(tooltip_timeout);
  Fl::remove_timeout(recent_timeout);
  if (t && *t && enabled()) { // there is a tooltip
    // do nothing if it is the same:
    if (wid==widget && x==X && y==Y && w==W && h==H && t==tip) return;
    // remember it:
    widget = wid; X = x; Y = y; W = w; H = h; tip = t;
    if (recent_tooltip || Fl_Tooltip::delay() < .1) {
      // switch directly from a previous tooltip to the new one:
      tooltip_timeout(0);
    } else {
      if (window) window->hide();
      Fl::add_timeout(Fl_Tooltip::delay(), tooltip_timeout);
    }
  } else { // no tooltip
    tip = 0;
    widget = 0;
    if (window) window->hide();
    if (recent_tooltip) Fl::add_timeout(.2, recent_timeout);
  }
}

void Fl_Widget::tooltip(const char *tt) {
  static bool beenhere = false;
  if (!beenhere) {
    beenhere = true;
    Fl_Tooltip::enter = tt_enter;
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
// End of "$Id: Fl_Tooltip.cxx,v 1.43 2002/04/25 16:39:33 spitzak Exp $".
//
