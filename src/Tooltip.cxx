//
// "$Id$"
//
// Tooltip code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".

// Try to keep this code from bloating up, as it is always linked in.

#include <fltk/Tooltip.h>
#include <fltk/MenuWindow.h>
#include <fltk/Monitor.h>
#include <fltk/events.h>
#include <fltk/run.h>
#include <fltk/draw.h>
#include <fltk/Box.h>
using namespace fltk;

float		Tooltip::delay_ = 1.0f;
bool		Tooltip::enabled_ = true;

#define MAX_WIDTH 400

static bool recent_tooltip;
static void tooltip_timeout(void*);

class TooltipBox : public MenuWindow {
public:
  TooltipBox() : MenuWindow(0, 0) {
    style(Tooltip::default_style);
    flags(ALIGN_LEFT|ALIGN_WRAP|ALIGN_INSIDE);
  }
  void draw();
  void layout();
#ifdef _WIN32
  // You have to destroy the window or it will not raise next time:
  void hide() {destroy();}
#endif
};

static Tooltip::Generator generator;
static void* argument;
Widget* Tooltip::widget;
static Rectangle rectangle;
static TooltipBox *window = 0;

void TooltipBox::layout() {
  setfont(textfont(), textsize());
  int ww=MAX_WIDTH, hh=0;
  measure(label(), ww, hh, flags()|OUTPUT);
  ww += 7; hh += 6;

  // find position on the screen of the rectangle that popped up the tooltip:
  Rectangle r(rectangle);
  for (Widget* p = Tooltip::current(); p; p = p->parent()) r.move(p->x(), p->y());

  const Monitor& monitor = Monitor::find(event_x_root(),event_y_root());

  // figure out the corner of the tooltip:
  int ox = event_x_root();
  if (ox+ww > monitor.r()) ox = monitor.r()-ww;
  if (ox < monitor.x()) ox = monitor.x();

  int oy = event_y_root()+16;
  if (oy < r.b()) oy = r.b();
  if (oy+hh > monitor.b()) oy = r.y()-hh;
  if (oy < monitor.y()) oy = monitor.y();

  resize(ox, oy, ww, hh);
  MenuWindow::layout();
}

void TooltipBox::draw() {
  draw_box();
  Rectangle r(w(),h()); //box()->inset(r);
  draw_label(r, style(), flags()|OUTPUT);
}

static void recent_timeout(void*) {
  recent_tooltip = false;
}

static bool recursion;

static void tooltip_timeout(void*) {
  if (recursion) return;
  recursion = true;
  const char* tip =
    generator ? generator(Tooltip::current(), argument) : (const char*)argument;
  if (!tip || !*tip) {
    if (window) window->hide();
  } else {
    //if (grab()) return;
    if (!window) window = new TooltipBox;
    // this cast bypasses the normal Window label() code:
    ((Widget*)window)->label(tip);
    window->layout();
    window->redraw();
    window->show();
  }
  remove_timeout(recent_timeout);
  recent_tooltip = true;
  recursion = false;
}

// If this widget or one of it's parents has a tooltip, enter it. This
// will do nothing if this is the current widget (even if the mouse moved
// out so an exit() was done and then moved back in). If no tooltip can
// be found do exit(). If you don't want this behavior (for instance
// if you want the tooltip to reappear when the mouse moves back in)
// call the fancier enter() below.
void Tooltip::enter(Widget* w) {
  //if (w == ::window) return;
  // find the enclosing group with a tooltip:
  Widget* tw = w;
  for (;;) {
    if (!tw) {exit(); return;}
    if (tw == widget) return;
    if (tw->tooltip()) break;
    tw = tw->parent();
  }
  enter(w, Rectangle(w->w(), w->h()), tw->tooltip());
}

// Acts as though enter(widget) was done but does not pop up a
// tooltip.  This is useful to prevent a tooltip from reappearing when
// a modal overlapping window is deleted. Fltk does this automatically
// when you click the mouse button.
void Tooltip::current(Widget* w) {
  exit();
  // find the enclosing group with a tooltip:
  Widget* tw = w;
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
void Tooltip::exit() {
  if (!widget) return;
  widget = 0;
  remove_timeout(tooltip_timeout);
  remove_timeout(recent_timeout);
  if (window) window->hide();
  if (recent_tooltip) {
    if (event_state() & ANY_BUTTON)
      recent_tooltip = 0;
    else
      add_timeout(.2f, recent_timeout);
  }
}

/**
  Get ready to display a tooltip. The widget and the rectangle \a r inside
  it define an area the tooltip is for, this along with the current
  mouse position places the tooltip (the mouse is assummed to point
  inside or near the box). The generator function is called with the
  widget and data to generate the text, this must be a static data
  buffer, this function allows you to defer the calculation of the
  tooltip text until it is first needed.
*/
void Tooltip::enter(Widget* wid, const Rectangle& r,
		       Tooltip::Generator gen, void* data)
{
  if (recursion) return;
  // act like exit() if nothing:
  if (!enabled() || !wid || !data || (!gen && !*(char*)data)) {
    exit(); return;
  }
  // do nothing if it is the same:
  if (wid==widget && gen == generator && data == argument) return;
  remove_timeout(tooltip_timeout);
  remove_timeout(recent_timeout);
  // remember it:
  widget = wid; rectangle = r;
  generator = gen; argument = data;
  // popup the tooltip immediately if it was recently up:
  if (recent_tooltip || Tooltip::delay() < .1) {
#ifdef WIN32
    // possible fix for the Windows titlebar, it seems to want the
    // window to be destroyed, moving it messes up the parenting:
    if (window) window->hide();
#endif
    tooltip_timeout(0);
  } else {
    if (window) window->hide();
    add_timeout(Tooltip::delay(), tooltip_timeout);
  }
}

static void revert(Style* s) {
  s->box_ = BORDER_BOX;
  s->color_ = (Color)215;
  //s->textcolor_ = GRAY33;
  //s->labelcolor_ = BLACK;
}
static NamedStyle style("Tooltip", revert, &Tooltip::default_style);
NamedStyle* Tooltip::default_style = &::style;

//
// End of "$Id$".
//
