//
// "$Id: Fl_Input_Browser.cxx,v 1.20 2002/12/09 04:52:25 spitzak Exp $"
//
// Input Browser (Combo Box) widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2001 by Bill Spitzak and others.
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

#include <fltk/InputBrowser.h>
#include <fltk/MenuWindow.h>
#include <fltk/Browser.h>
#include <fltk/ScreenInfo.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>

using namespace fltk;

static NamedStyle style("InputBrowser", 0, &InputBrowser::default_style);
NamedStyle* InputBrowser::default_style = &::style;

InputBrowser::InputBrowser(int x, int y, int w, int h, const char *l)
  : Menu(x, y, w, h, l),
    input(x, y, w, h)
{
  align(ALIGN_LEFT);
  style(default_style);
  if (input.parent()) input.parent()->remove(input);
  input.parent(this);
  minh_ = 10;
  maxw_ = 600;
  maxh_ = 400;
  over_now = 0; over_last = 1;
}

// these are only used when in grabbed state so only one exists at once
static MenuWindow *mw;
static InputBrowser *ib;
static Browser *b;

class ComboWindow : public MenuWindow {
  public:
    int handle(int);
    ComboWindow(int x, int y, int w, int h) : MenuWindow(x, y, w, h) {}
};

int
ComboWindow::handle(int event) {
  switch (event) {
  case MOVE:
  case DRAG:
  case RELEASE:
    return b->handle(event);
  }
  return MenuWindow::handle(event);
}

class ComboBrowser : public Browser {
  public:
    int handle(int);
    ComboBrowser(int x, int y, int w, int h)
      : Browser(x, y, w, h, 0) {}
};

int
ComboBrowser::handle(int event) {
  switch (event) {
  case KEY:
    if (event_key() == EscapeKey) {
      exit_modal();
      return 1;
    }
    break;
  case PUSH:
    if (!event_inside(0, 0, w(), h())) {
      exit_modal();
      return 0;
    }
    break;
  case MOVE:
    event = DRAG;
  case RELEASE:
  case DRAG:
    // this causes a drag-in to the widget to work:
    if (event_inside(0, 0, w(), h())) fltk::pushed(this);
    else return 0;
  }
#if 0
  // vertical scrollbar event
  int vse = scrollbar.visible() && event_inside(scrollbar.x(),
            scrollbar.y(), scrollbar.w(), h());

  // horizontal scrollbar event
  int hse = hscrollbar.visible() && event_inside(hscrollbar.x(),
            hscrollbar.y(), w(), hscrollbar.h());

//  int X = x(), Y = y(), W = w(), H = h(); box()->inset(X, Y, W, H);
//  if (!event_inside(X, Y, W, H)) return 0;
  if (!event_inside(0, 0, w(), h())) return 0;

  if (event == MOVE && !vse && !hse) event = DRAG;
#endif
  return Browser::handle(event);
}

static void ComboBrowser_cb(Widget*, void*) {
  // we get callbacks for all keys?
  if (event() != KEY && event() != RELEASE) return;
  if (event() == KEY && event_key() != ReturnKey
      && event_key() != ' ')
    return;
  Widget *item = b->item();
  if (item->is_group()) return; // can't select a group!
  ib->item(item);
  ib->value(item->label());
  ib->redraw(DAMAGE_VALUE);
  mw->hide();
}

// CET - FIXME - this doesn't seem to be working
// Use this to copy all the items out of one group into another:
class Share_List : public List {
public:
  Menu* other;
  int children(const Menu*, const int* indexes, int level) {
    return other->children(indexes, level);
  }
  Widget* child(const Menu*, const int* indexes, int level) {
    return other->child(indexes, level);
  }
  void flags_changed(const Menu*, Widget* widget) {
    other->list()->flags_changed(other,widget);
  }
} share_list; // only one instance of this.

int
InputBrowser::handle(int e) {
  int TX, TY = 0, TW, TH = h();
  if (type()&NONEDITABLE) {
    TX = 0; TW = w();
  } else {
    TX = input.x()+input.w(); TW = w()-(input.x()+input.w());
  }
  if (event_inside(TX, TY, TW, TH))
    over_now = 1;
  else
    over_now = 0;
  if (over_now != over_last) redraw(DAMAGE_HIGHLIGHT);

  if (e == FOCUS) fltk::focus(input);

  if ((event_inside(input.x(), input.y(), input.w(), input.h()) || e == KEY)
    && !(type()&NONEDITABLE) && !pushed())
  {
    if (e == PUSH) fltk::pushed(input);
    return input.handle(e);
  }

  switch (e) {
    case PUSH: {
      redraw(DAMAGE_VALUE);
      if (!children()) return 1;
      ib = this;
      // dummy W,H used -- will be replaced.
      Group::current(0);
      mw = new ComboWindow(event_x_root()-event_x(),
                           event_y_root()-event_y()+h(),
                           200,400);
      mw->begin();
      // dummy W,H used -- will be replaced.
      b = new ComboBrowser(0,0,200,400);
      b->indented((type()&INDENTED) != 0);
      b->box(BORDER_BOX);
      share_list.other = this;
      b->list(&share_list);
      b->when(WHEN_RELEASE_ALWAYS);
      b->callback(ComboBrowser_cb);
      mw->end();
      b->layout(); // (WAS: it is ok to do this)
      int W = b->width()+b->scrollbar.w()+b->box()->dw();
      int H = b->height()+b->box()->dh();
      if (W > maxw_) W = maxw_;
      if (H > maxh_) H = maxh_;
      if (W < minw_) W = minw_;
      if (H < minh_) H = minh_;
      int X = mw->x();
      int Y = mw->y();
      int down = screenInfo().height - Y;
      int up = event_y_root() - event_y();
      if (H > down) {
        if (up > down) {
          Y = event_y_root() - event_y() - H;
          if (Y < 0) { Y = 0; H = up; }
        } else {
          H = down;
        }
      }
      if (X + W > screenInfo().width) {
        X = screenInfo().width - W;
        if (X < 0) { X = 0; W = screenInfo().width; }
      }
      mw->resize(X, Y, W, H);
      b->Widget::size(W, H);

      b->value(item() ? b->Group::find(item()) : 0);
      b->make_item_visible();

      mw->exec(0, true);

      delete mw;
      if (type()&NONEDITABLE) throw_focus();
      else fltk::focus(input);

      ib = 0;
      redraw(DAMAGE_VALUE);
      return 1;
    }

    case FOCUS:
    case UNFOCUS:
      if (type()&NONEDITABLE) break;
      return input.handle(e);

    case ENTER: case MOVE: return 1;
  }
  return 0;
}

void
InputBrowser::draw() {
  Flags f = flags();
  if (!active_r()) f |= INACTIVE;
  minw_ = w();
  if (damage()&DAMAGE_ALL) draw_frame();
  int X = 0, Y = 0, W = w(), H = h(); box()->inset(X, Y, W, H);
  int W1 = H*4/5;
  if (damage()&(DAMAGE_ALL|DAMAGE_CHILD)) {
    input.resize(X, Y, W-W1, H);
    input.set_damage(DAMAGE_ALL);
    input.copy_style(style()); // force it to use this style
    input.box(FLAT_BOX);
    // fix for relative coordinates
    push_matrix();
    translate(X,Y);
    input.draw();
    pop_matrix();
    input.set_damage(0);
  }
  if (damage()&(DAMAGE_ALL|DAMAGE_VALUE|DAMAGE_HIGHLIGHT)) {
    if (ib == this) f |= VALUE;
    if (over_now) f |= HIGHLIGHT;
    X += W-W1; W = W1;
    // draw the little mark at the right:
    draw_glyph(GLYPH_DOWN_BUTTON, X, Y, W, H, f);
    over_last = over_now;
  }
}

//
// End of "$Id: Fl_Input_Browser.cxx,v 1.20 2002/12/09 04:52:25 spitzak Exp $".
//
