//
// "$Id: Fl_Input_Browser.cxx,v 1.19 2002/10/26 09:55:30 spitzak Exp $"
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

#include <fltk/Fl.h>
#include <fltk/Fl_Input_Browser.h>
#include <fltk/Fl_Menu_Window.h>
#include <fltk/Fl_Browser.h>
#include <fltk/fl_draw.h>

static Fl_Named_Style style("Input_Browser", 0, &Fl_Input_Browser::default_style);
Fl_Named_Style* Fl_Input_Browser::default_style = &::style;

Fl_Input_Browser::Fl_Input_Browser(int x, int y, int w, int h, const char *l)
  : Fl_Menu_(x, y, w, h, l)
{
  align(FL_ALIGN_LEFT);
  style(default_style);
  input = new Fl_Input(x, y, w, h);
  if (input->parent()) input->parent()->remove(input);
  input->parent(this);
  minh_ = 10;
  maxw_ = 600;
  maxh_ = 400;
  over_now = 0; over_last = 1;
}

// these are only used when in grabbed state so only one exists at once
static Fl_Menu_Window *mw;
static Fl_Input_Browser *ib;
static Fl_Browser *b;

class ComboWindow : public Fl_Menu_Window {
  public:
    int handle(int);
    ComboWindow(int x, int y, int w, int h) : Fl_Menu_Window(x, y, w, h) {}
};

int
ComboWindow::handle(int event) {
  switch (event) {
  case FL_MOVE:
  case FL_DRAG:
  case FL_RELEASE:
    return b->handle(event);
  }
  return Fl_Menu_Window::handle(event);
}

class ComboBrowser : public Fl_Browser {
  public:
    int handle(int);
    ComboBrowser(int x, int y, int w, int h)
      : Fl_Browser(x, y, w, h, 0) {}
};

int
ComboBrowser::handle(int event) {
  switch (event) {
  case FL_KEY:
    if (Fl::event_key() == FL_Escape) {
      Fl::exit_modal();
      return 1;
    }
    break;
  case FL_PUSH:
    if (!Fl::event_inside(0, 0, w(), h())) {
      Fl::exit_modal();
      return 0;
    }
    break;
  case FL_MOVE:
    event = FL_DRAG;
  case FL_RELEASE:
  case FL_DRAG:
    // this causes a drag-in to the widget to work:
    if (Fl::event_inside(0, 0, w(), h())) Fl::pushed(this);
    else return 0;
  }
#if 0
  // vertical scrollbar event
  int vse = scrollbar.visible() && Fl::event_inside(scrollbar.x(),
            scrollbar.y(), scrollbar.w(), h());

  // horizontal scrollbar event
  int hse = hscrollbar.visible() && Fl::event_inside(hscrollbar.x(),
            hscrollbar.y(), w(), hscrollbar.h());

//  int X = x(), Y = y(), W = w(), H = h(); box()->inset(X, Y, W, H);
//  if (!Fl::event_inside(X, Y, W, H)) return 0;
  if (!Fl::event_inside(0, 0, w(), h())) return 0;

  if (event == FL_MOVE && !vse && !hse) event = FL_DRAG;
#endif
  return Fl_Browser::handle(event);
}

static void ComboBrowser_cb(Fl_Widget*, void*) {
  // we get callbacks for all keys?
  if (Fl::event() != FL_KEY && Fl::event() != FL_RELEASE) return;
  if (Fl::event() == FL_KEY && Fl::event_key() != FL_Enter
      && Fl::event_key() != ' ')
    return;
  Fl_Widget *item = b->item();
  if (item->is_group()) return; // can't select a group!
  ib->item(item);
  ib->value(item->label());
  ib->redraw(FL_DAMAGE_VALUE);
  mw->hide();
}

// CET - FIXME - this doesn't seem to be working
// Use this to copy all the items out of one group into another:
class Share_List : public Fl_List {
public:
  Fl_Menu_* other;
  int children(const Fl_Menu_*, const int* indexes, int level) {
    return other->children(indexes, level);
  }
  Fl_Widget* child(const Fl_Menu_*, const int* indexes, int level) {
    return other->child(indexes, level);
  }
  void flags_changed(const Fl_Menu_*, Fl_Widget* widget) {
    other->list()->flags_changed(other,widget);
  }
} share_list; // only one instance of this.

int
Fl_Input_Browser::handle(int e) {
  int TX, TY = 0, TW, TH = h();
  if (type()&NONEDITABLE) {
    TX = 0; TW = w();
  } else {
    TX = input->x()+input->w(); TW = w()-(input->x()+input->w());
  }
  if (Fl::event_inside(TX, TY, TW, TH))
    over_now = 1;
  else
    over_now = 0;
  if (over_now != over_last) redraw(FL_DAMAGE_HIGHLIGHT);

  if (e == FL_FOCUS) Fl::focus(input);

  if ((Fl::event_inside(input->x(), input->y(), input->w(), input->h()) || e == FL_KEY)
    && !(type()&NONEDITABLE) && Fl::pushed() != this)
  {
    if (e == FL_PUSH) Fl::pushed(input);
    return input->handle(e);
  }

  switch (e) {
    case FL_PUSH: {
      redraw(FL_DAMAGE_VALUE);
      if (!children()) return 1;
      ib = this;
      // dummy W,H used -- will be replaced.
      Fl_Group::current(0);
      mw = new ComboWindow(Fl::event_x_root()-Fl::event_x(),
                           Fl::event_y_root()-Fl::event_y()+h(),
                           200,400);
      // dummy W,H used -- will be replaced.
      b = new ComboBrowser(0,0,200,400);
      b->indented((type()&INDENTED) != 0);
      b->box(FL_BORDER_BOX);
      share_list.other = this;
      b->list(&share_list);
      b->when(FL_WHEN_RELEASE_ALWAYS);
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
      int down = Fl::info().height - Y;
      int up = Fl::event_y_root() - Fl::event_y();
      if (H > down) {
        if (up > down) {
          Y = Fl::event_y_root() - Fl::event_y() - H;
          if (Y < 0) { Y = 0; H = up; }
        } else {
          H = down;
        }
      }
      if (X + W > Fl::info().width) {
        X = Fl::info().width - W;
        if (X < 0) { X = 0; W = Fl::info().width; }
      }
      mw->resize(X, Y, W, H);
      b->Fl_Widget::size(W, H);

      b->value(item() ? b->Fl_Group::find(item()) : 0);
      b->make_item_visible();

      mw->exec(0, true);

      delete mw;
      if (type()&NONEDITABLE) throw_focus();
      else Fl::focus(input);

      ib = 0;
      redraw(FL_DAMAGE_VALUE);
      return 1;
    }

    case FL_FOCUS:
    case FL_UNFOCUS:
      if (type()&NONEDITABLE) break;
      return input->handle(e);

    case FL_ENTER: case FL_MOVE: return 1;
  }
  return 0;
}

void
Fl_Input_Browser::draw() {
  Fl_Flags f = flags();
  if (!active_r()) f |= FL_INACTIVE;
  minw_ = w();
  if (damage()&FL_DAMAGE_ALL) draw_frame();
  int X = 0, Y = 0, W = w(), H = h(); box()->inset(X, Y, W, H);
  int W1 = H*4/5;
  if (damage()&(FL_DAMAGE_ALL|FL_DAMAGE_CHILD)) {
    input->resize(X, Y, W-W1, H);
    input->set_damage(FL_DAMAGE_ALL);
    input->copy_style(style()); // force it to use this style
    input->box(FL_FLAT_BOX);
    // fix for relative coordinates
    fl_push_matrix();
    fl_translate(X,Y);
    input->draw();
    fl_pop_matrix();
    input->set_damage(0);
  }
  if (damage()&(FL_DAMAGE_ALL|FL_DAMAGE_VALUE|FL_DAMAGE_HIGHLIGHT)) {
    if (ib == this) f |= FL_VALUE;
    if (over_now) f |= FL_HIGHLIGHT;
    X += W-W1; W = W1;
    // draw the little mark at the right:
    draw_glyph(FL_GLYPH_DOWN_BUTTON, X, Y, W, H, f);
    over_last = over_now;
  }
}

//
// End of "$Id: Fl_Input_Browser.cxx,v 1.19 2002/10/26 09:55:30 spitzak Exp $".
//
