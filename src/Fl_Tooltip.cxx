//
// "$Id: Fl_Tooltip.cxx,v 1.49 2002/10/26 09:55:30 spitzak Exp $"
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

// Try to keep this code from bloating up, as it is always linked in.

#include <fltk/Fl_Tooltip.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Menu_Window.h>

float		Fl_Tooltip::delay_ = 1.0f;
bool		Fl_Tooltip::enabled_ = true;

#define MAX_WIDTH 400

class Fl_TooltipBox : public Fl_Menu_Window {
public:
  Fl_TooltipBox() : Fl_Menu_Window(0, 0) {
    style(Fl_Tooltip::default_style);
    end();
  }
  void draw();
  void layout();
#ifdef _WIN32
  // You have to destroy the window or it will not raise next time:
  void hide() {destroy();}
#endif
};

static Fl_Tooltip::Generator generator;
static void* argument;
Fl_Widget* Fl_Tooltip::widget;
static int X,Y,W,H;
static Fl_TooltipBox *window = 0;

void Fl_TooltipBox::layout() {
  fl_font(label_font(), label_size());
  int ww, hh;
  ww = MAX_WIDTH;
  fl_measure(label(), ww, hh, FL_ALIGN_LEFT|FL_ALIGN_WRAP|FL_ALIGN_INSIDE);
  ww += 6; hh += 6;

  // find position on the screen of the widget:
  int ox = Fl::event_x_root();
  //int ox = X+W/2;
  int oy = Y + H+2;
  for (Fl_Widget* p = Fl_Tooltip::current(); p; p = p->parent()) {
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
  const char* tip =
    generator ? generator(Fl_Tooltip::current(), argument) : (const char*)argument;
  if (!tip || !*tip) {
    if (window) window->hide();
  } else {
    //if (Fl::grab()) return;
    if (!window) window = new Fl_TooltipBox;
    // this cast bypasses the normal Fl_Window label() code:
    ((Fl_Widget*)window)->label(tip);
    window->layout();
    window->redraw();
    window->show();
  }
  Fl::remove_timeout(recent_timeout);
  recent_tooltip = true;
  recursion = false;
}

// If this widget or one of it's parents has a tooltip, enter it. This
// will do nothing if this is the current widget (even if the mouse moved
// out so an exit() was done and then moved back in). If no tooltip can
// be found do exit(). If you don't want this behavior (for instance
// if you want the tooltip to reappear when the mouse moves back in)
// call the fancier enter() below.
void Fl_Tooltip::enter(Fl_Widget* w) {
  // find the enclosing group with a tooltip:
  Fl_Widget* tw = w;
  for (;;) {
    if (!tw) {exit(); return;}
    if (tw->tooltip()) break;
    tw = tw->parent();
  }
  enter(w, 0, 0, w->w(), w->h(), tw->tooltip());
}

// Acts as though enter(widget) was done but does not pop up a
// tooltip.  This is useful to prevent a tooltip from reappearing when
// a modal overlapping window is deleted. Fltk does this automatically
// when you click the mouse button.
void Fl_Tooltip::current(Fl_Widget* w) {
  exit();
  // find the enclosing group with a tooltip:
  Fl_Widget* tw = w;
  for (;;) {
    if (!tw) return;
    if (tw->tooltip()) break;
    tw = tw->parent();
  }
  // act just like enter() except we can remember a zero:
  widget = w;
  generator = 0;
  argument = (void*)tw->tooltip();
}

// Hide any visible tooltip.
void Fl_Tooltip::exit() {
  if (!widget) return;
  widget = 0;
  Fl::remove_timeout(tooltip_timeout);
  Fl::remove_timeout(recent_timeout);
  if (window) window->hide();
  if (recent_tooltip) {
    if (Fl::event_state() & FL_BUTTONS)
      recent_tooltip = 0;
    else
      Fl::add_timeout(.2f, recent_timeout);
  }
}

// Get ready to display a tooltip. The widget and the xywh box inside
// it define an area the tooltip is for, this along with the current
// mouse position places the tooltip (the mouse is assummed to point
// inside or near the box). The generator function is called with the
// widget and data to generate the text, this must be a static data
// buffer, this function allows you to defer the calculation of the
// tooltip text until it is first needed.
void Fl_Tooltip::enter(Fl_Widget* wid, int x,int y,int w,int h,
		       Fl_Tooltip::Generator gen, void* data)
{
  if (recursion) return;
  // act like exit() if nothing:
  if (!enabled() || !wid || !gen && (!data || !*(char*)data)) {
    exit(); return;
  }
  // do nothing if it is the same:
  if (wid==widget && gen == generator && data == argument) return;
  Fl::remove_timeout(tooltip_timeout);
  Fl::remove_timeout(recent_timeout);
  // remember it:
  widget = wid; X = x; Y = y; W = w; H = h;
  generator = gen; argument = data;
  // popup the tooltip immediately if it was recently up:
  if (recent_tooltip || Fl_Tooltip::delay() < .1) {
#ifdef WIN32
    // possible fix for the Windows titlebar, it seems to want the
    // window to be destroyed, moving it messes up the parenting:
    if (window) window->hide();
#endif
    tooltip_timeout(0);
  } else {
    if (window) window->hide();
    Fl::add_timeout(Fl_Tooltip::delay(), tooltip_timeout);
  }
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
// End of "$Id: Fl_Tooltip.cxx,v 1.49 2002/10/26 09:55:30 spitzak Exp $".
//
