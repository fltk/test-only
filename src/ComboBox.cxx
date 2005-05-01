// "$Id: ComboBox.cxx,v 1.2 2005/02/04 22:45:05 spitzak Exp $"
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

// single line input field with predefined choices via popup menu

#include <fltk/ComboBox.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/string.h>

using namespace fltk;

static void revert(Style* s) {
#if MOTIF_STYLE
  s->color = GRAY75;
  s->box = s->buttonbox = Widget::default_style->buttonbox;
  s->glyph = ::glyph;
#endif
#if MAC_STYLE
  s->glyph = ::glyph;
#endif
}
static NamedStyle style("ComboBox", revert, &ComboBox::default_style);
NamedStyle* ComboBox::default_style = &::style;

ComboBox::ComboBox(int x,int y,int w,int h, const char *l) :
  Choice(x,y,w,h,l)
{
  style(default_style);
  int w1 = h*4/5;
  Group *g = current();
  current(0);
  input_ = new ComboInput(x, y, w-w1, h, this);
  input_->box(fltk::NO_BOX);
  input_->parent(this);
  current(g);
}

ComboBox::~ComboBox() {
  delete input_;
}

void ComboBox::draw() {
  if (damage() & DAMAGE_ALL) {
    drawstyle(style(), update_flags()|OUTPUT);
    draw_frame();
    Rectangle r(w(),h());
    r.set_x(w()-h()*4/5);
    box()->inset(r);
    draw_glyph(GLYPH_DOWN_BUTTON, r);
  }
  input_->set_damage(damage()|input_->damage());
  if (input_->damage()) {
    fltk::push_matrix();
    fltk::translate(input_->x(), input_->y());
    input_->color(color());
    input_->draw();
    fltk::pop_matrix();
    input_->set_damage(0);
  }
}

void ComboBox::layout() {
  Choice::layout();
  int w1 = h()*4/5;
  Rectangle r(w()-w1,h());
  box()->inset(r);
  input_->resize(r.x(),r.y(),r.w(),r.h());
}

int ComboBox::handle(int event) {
  static bool mouse_on_input = false;
  static bool want_mouse_drag = false;
  int ret = 0;
  switch (event) {
  case PUSH:
	if (event_x()<w()-h()*4/5) {
	  mouse_on_input = true;
	  ret = input_->handle(event);
	} else {
	  mouse_on_input = false;
	  if (click_to_focus()) take_focus();
	EXECUTE:
	  if (popup(0, 0, w(), h(), 0)) redraw(DAMAGE_VALUE);
	}
	want_mouse_drag = ret!=0;
	ret = 1;
	break;
  case DRAG:
	if (want_mouse_drag) {
	  if (mouse_on_input)
		ret = input_->handle(event);
	  else
		ret = Choice::handle(event);
	}
	ret = 1;
	break;
  case RELEASE:
	if (want_mouse_drag) {
	  if (mouse_on_input)
		ret = input_->handle(event);
	  else
		ret = Choice::handle(event);
	}
	mouse_on_input = false;
	ret = 1;
	break;
  case SHORTCUT:
	if (test_shortcut()) goto EXECUTE;
	if (handle_shortcut()) {
	  redraw(DAMAGE_VALUE);
	  return 1;
	} else {
	  input_->handle(event);
	}
	break;
  case KEY:
	if (event_key()==DownKey||event_key()==UpKey) {
	  if (event_key()==DownKey) Choice::value(0); else Choice::value(Choice::size()-1);
	  e_keysym = ReturnKey;
	  ret = Choice::handle(event);
	  break;
	}
  case KEYUP:
	// handle arrow up/down to select items from the menu
	ret = input_->handle(event);
	break;
  // events for input alone
  case PASTE:
  case TIMEOUT:
  case DND_ENTER:
  case DND_DRAG:
  case DND_LEAVE:
  case DND_RELEASE:
  case FOCUS_CHANGE:
	ret = input_->handle(event);
	break;
  // events that both widgets should receive
  case FOCUS:
	input_->take_focus();
  case ACTIVATE:
  case ENTER:
  case SHOW:
	ret = Choice::handle(event);
    ret |= input_->handle(event);
	break;
  case UNFOCUS:
  case DEACTIVATE:
  case HIDE:
  case LEAVE:
    ret = input_->handle(event);
	ret |= Choice::handle(event);
	break;
  // events for choice alone
  default: // MOVE, MOUSWHEEL, TOOLTIP
	ret = Choice::handle(event);
  }
  if (input_->damage())
	redraw();
  return ret;
}

int ComboBox::choice(int v) {
  int ret = Choice::value(v);
  Widget *f = get_item();
  if (f) input_->value(f->label());
  text_changed_();
  return ret;
}

int ComboBox::choice() const {
  ComboBox *This = (ComboBox*)this;
  Widget *f = This->get_item();
  if (!f) return -1;
  if (strcmp(input_->value(), f->label())==0)
	return Choice::value();
  return -1;
}

int ComboBox::find_choice() const {
  const char *t = input_->value();
  int n = children();
  for (int i=0;i<n;i++) {
    const char *m = child(i)->label();
    if (m && strcmp(m, t)==0) {
      return i;
    }
  }
  return -1;
}

bool ComboBox::text_changed_(bool ret) {
  if (input_->damage()) {
    redraw(input_->damage());
  }
  // we should also update the current choice
  return ret;
}

void ComboBox::input_callback_(Widget *w,void *d) {
  ComboBox *This = (ComboBox*)d;
  This->text_changed_();
  This->do_callback();
}

int ComboBox::popup(
    int X, int Y, int W, int H,
    const char* title,
    bool menubar)
{
  Widget *selected = try_popup(X, Y, W, H, title, menubar);
  if (selected) {
    if (selected->label())
      input_->value(selected->label());
    else
      input_->value("");
    execute(selected);
    return 1;
  }
  return 0;
}

//---- Combo Input ---------------------------------------------

ComboBox::ComboInput::ComboInput(int x, int y, int w, int h, ComboBox *c) :
  Input(x, y, w, h)
{
  combo_ = c;
  callback(input_callback_, combo_);
}

int ComboBox::ComboInput::handle(int event) {
  int ret;
  switch (event) {
  case FOCUS:
	break;
  case UNFOCUS:
	break;
  }
  ret = Input::handle(event);
  if (damage())
	combo_->redraw();
  return ret;
}

// End of $Id: ComboBox.cxx,v 1.2 2005/02/04 22:45:05 spitzak Exp $
