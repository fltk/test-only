//
// "$Id: Fl_Tooltip.cxx,v 1.20 1999/11/18 04:33:21 carl Exp $"
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

#include <FL/Fl_Tooltip.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Window.H>

float Fl_Tooltip::delay_ = 0.5f;
int Fl_Tooltip::enabled_ = 1;

#define MAX_WIDTH 400

class Fl_TooltipBox : public Fl_Menu_Window {
public:
  Fl_TooltipBox() : Fl_Menu_Window(0, 0, 0, 0) {
    style(Fl_Tooltip::default_style);}
  void draw() {
    box()->draw(0,0,w(),h(),color(),0);
    draw_label(3, 3, w()-6, h()-6,
	       Fl_Flags(FL_ALIGN_LEFT|FL_ALIGN_WRAP|FL_ALIGN_INSIDE));
  }
};

static Fl_TooltipBox *window = 0;
static Fl_Widget* widget;
static int x,y,w,h;
static const char* tip;

static void tooltip_timeout(void*) {
  if (Fl::grab()) return;

  if (!window) { // create the reusable X window
    Fl_Group* saveCurrent = Fl_Group::current();
    Fl_Group::current(0);
    window = new Fl_TooltipBox;
    window->clear_border();
    window->end();
    Fl_Group::current(saveCurrent);
  }

  // this cast bypasses the normal Fl_Window label() code:
  ((Fl_Widget*)window)->label(tip);

  fl_font(window->label_font(), window->label_size());
  int ww, hh;
  ww = MAX_WIDTH;
  fl_measure(tip, ww, hh);
  ww += 6; hh += 6;

  // find position on the screen of the widget:
  int ox = Fl::event_x_root()+5;
  //int ox = x+w/2;
  int oy = y + h+2;
  for (Fl_Window* p = widget->window(); p; p = p->window()) {
    //ox += p->x();
    oy += p->y();
  }
  if (ox+ww > Fl::w()) ox = Fl::w() - ww;
  if (ox < 0) ox = 0;
  if (h > 30) {
    oy = Fl::event_y_root()+13;
    if (oy+hh > Fl::h()) oy -= 23+hh;
  } else {
    if (oy+hh > Fl::h()) oy -= (4+hh+h);
  }
  if (oy < 0) oy = 0;

  window->resize(ox, oy, ww, hh);

  window->show();
}

static int cheesy_flag = 0;

void
Fl_Tooltip::enter(Fl_Widget* w) {
  if (cheesy_flag || w == window || w == widget) return;
  if (!w) {exit(widget); widget = 0; return;}
  enter(w, w->x(), w->y(), w->w(), w->h(), w->tooltip());
}

void
Fl_Tooltip::enter(Fl_Widget* w, int X, int Y, int W, int H, const char* t) {
  if (cheesy_flag) return;
  if (w == widget && X == x && Y == y && W ==::w && H == h && t == tip) return;
  exit(widget);
  widget = w; x = X; y = Y; ::w = W; h = H; tip = t;
  if (!t || !enabled_) return;
  float d = Fl_Tooltip::delay();
  if (d < .01) d = .01;
  Fl::add_timeout(d, tooltip_timeout);
}

void
Fl_Tooltip::exit(Fl_Widget *w) {
  if (!w || w != widget) return;
  widget = 0;
  Fl::remove_timeout(tooltip_timeout);
  if (window) {
    // This flag makes sure that tootip_enter() isn't executed because of
    // this hide() which could cause unwanted recursion in tooltip_enter
    cheesy_flag = 1;
    window->hide();
    cheesy_flag = 0;
  }
}

static void revert(Fl_Style* s) {
  s->box = FL_BORDER_BOX;
  s->color = (Fl_Color)215;
  s->label_color = FL_BLACK;
}

Fl_Style Fl_Tooltip::default_style("Tooltip", revert);

//
// End of "$Id: Fl_Tooltip.cxx,v 1.20 1999/11/18 04:33:21 carl Exp $".
//
