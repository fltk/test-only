//
// "$Id: Fl_Input_Browser.cxx,v 1.2 2001/03/01 02:00:53 clip Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Input_Browser.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Browser.H>
#include <FL/fl_draw.H>

static Fl_Named_Style* cb_style = new Fl_Named_Style("Input Browser", 0, &cb_style);

Fl_Input_Browser::Fl_Input_Browser(int x, int y, int w, int h, const char *l)
  : Fl_Menu_(x, y, w, h, l)
{
  align(FL_ALIGN_LEFT);
  style(cb_style);
  input = new Fl_Input(x, y, w, h);
  if (input->parent()) input->parent()->remove(input);
  input->parent(this);
  minh_ = 10;
  maxw_ = 600;
  maxh_ = 400;
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
ComboWindow::handle(int e) {
  if ((!Fl::event_inside(0, 0, w(), h()) && e == FL_PUSH) ||
      (e == FL_SHORTCUT && Fl::event_key() == FL_Escape))
  {
    hide();
    return 1;
  }
  return Fl_Menu_Window::handle(e);
}

class ComboBrowser : public Fl_Browser {
  public:
    int handle(int);
    ComboBrowser(int x, int y, int w, int h)
      : Fl_Browser(x, y, w, h, 0) {}
};

int
ComboBrowser::handle(int e) {
  // vertical scrollbar event
  int vse = scrollbar.visible() && Fl::event_inside(scrollbar.x(),
            scrollbar.y(), scrollbar.w(), h());

  // horizontal scrollbar event
  int hse = hscrollbar.visible() && Fl::event_inside(hscrollbar.x(),
            hscrollbar.y(), w(), hscrollbar.h());

  int X = x(), Y = y(), W = w(), H = h(); text_box()->inset(X, Y, W, H);
  if (e == FL_MOVE && !vse && !hse && Fl::event_inside(X, Y, W, H))
    { Fl_Browser::handle(e); e = FL_DRAG; }

  return Fl_Browser::handle(e);
}

static void ComboBrowser_cb(Fl_Widget *w, void *v) {
  // we get callbacks for all keys?
  if (Fl::event() == FL_KEY && Fl::event_key() != FL_Enter) return;
  Fl_Widget *item = (Fl_Widget *)v;
  if (item->is_group()) return; // can't select a group!
  ib->item(item);
  ib->value(item->label());
  ib->damage(FL_DAMAGE_CHILD);
  mw->hide();
}

int
Fl_Input_Browser::handle(int e) {
  if ((Fl::event_inside(input->x(), input->y(), input->w(), input->h()) || e == FL_KEY)
    && !(type()&FL_NONEDITABLE_INPUT_BROWSER) && Fl::pushed() != this)
  {
    if (e == FL_PUSH) Fl::pushed(input);
    return input->handle(e);
  }
  switch (e) {
    case FL_PUSH: {
      Fl::pushed(this);
      damage(FL_DAMAGE_SCROLL);
      if (!children()) return 1;
      ib = this;
      // dummy W,H used -- will be replaced.
      mw = new ComboWindow(Fl::event_x_root()-Fl::event_x(),
                           Fl::event_y_root()-Fl::event_y()+h(),
                           200,400);
      if (mw->parent()) mw->parent()->remove(mw);
      mw->parent(0);
      mw->set_modal();
      mw->clear_border();
      // dummy W,H used -- will be replaced.
      b = new ComboBrowser(0,0,200,400);
      if (type()&FL_INDENTED_INPUT_BROWSER) b->indented(1);
      b->text_box(FL_BORDER_BOX);
      while (children()) b->add(child(0));
      b->when(FL_WHEN_RELEASE_ALWAYS);
      b->callback(ComboBrowser_cb);
      mw->end();
      Fl::grab(mw);
      b->layout();// shouldn't call this directly
      int W = b->last_max_width+b->scrollbar.w()+b->box()->dx();
      int H = b->last_height+b->box()->dy();
      if (W > maxw_) W = maxw_;
      if (H > maxh_) H = maxh_;
      if (W < minw_) W = minw_;
      if (H < minh_) H = minh_;
      mw->size(W, H);
      b->Fl_Widget::size(W, H);
      b->layout();// shouldn't call this directly
      b->goto_number(item() ? b->Fl_Group::find(item()) : 0);
      b->item_select();
      b->set_middle();
      Fl::focus(b);
      mw->exec();
      Fl::release();
      while (b->children()) add(b->child(0));
      delete mw;
      if (type()&FL_NONEDITABLE_INPUT_BROWSER) throw_focus();
      else Fl::focus(input);
      return 1;
    }

    case FL_ENTER: case FL_MOVE: return 1; break;

    case FL_RELEASE:
      damage(FL_DAMAGE_SCROLL);
      break;

    case FL_FOCUS:
    case FL_UNFOCUS:
      if (type()&FL_NONEDITABLE_INPUT_BROWSER) break;
      return input->handle(e);

    case FL_DRAG:
      if (!Fl::event_inside(input->w(), 0, w()-input->w(), h())) {
        Fl::pushed(b);
        damage(FL_DAMAGE_SCROLL);
        return 1;
      }
      return 1;
  }
  return Fl_Menu_::handle(e);
}

void
Fl_Input_Browser::draw() {
  Fl_Flags f = flags();
  if (!active_r()) f |= FL_INACTIVE;
  minw_ = w();
  if (damage()&FL_DAMAGE_ALL)
    text_box()->draw(this, 0, 0, w(), h(), f);
  int X = 0, Y = 0, W = w(), H = h();
  text_box()->inset(X, Y, W, H);
  int W1 = H*4/5;
  if (damage()&(FL_DAMAGE_ALL|FL_DAMAGE_CHILD)) {
    input->resize(X, Y, W-W1, H);
    input->set_damage(FL_DAMAGE_ALL);
    input->copy_style(style()); // force it to use this style
    input->text_box(FL_FLAT_BOX);
    // fix for relative coordinates
    fl_x_ += X; fl_y_ += Y;
    input->draw();
    fl_x_ -= X; fl_y_ -= Y;
    input->clear_damage();
  }
  if (damage()&(FL_DAMAGE_ALL|FL_DAMAGE_SCROLL)) {
    if  (Fl::pushed() == this) f |= FL_VALUE;
    X += W-W1; W = W1;
    box()->draw(this, X, Y, W, H, f);
    box()->inset(X, Y, W, H);
    draw_glyph(FL_GLYPH_DOWN, X, Y, W, H, f);
  }
}

//
// End of "$Id: Fl_Input_Browser.cxx,v 1.2 2001/03/01 02:00:53 clip Exp $".
//
