//
// "$Id: Fl_Window_Type.cxx,v 1.41 2002/12/09 04:52:22 spitzak Exp $"
//
// Window type code for the Fast Light Tool Kit (FLTK).
//
// The widget describing an fltk::Window.  This is also all the code
// for interacting with the overlay, which allows the user to
// select, move, and resize the children widgets.
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

#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/OverlayWindow.h>
#include <fltk/ask.h>
#include <fltk/draw.h>
#include <fltk/Box.h>
#include "FluidType.h"
#include <math.h>
#include <stdlib.h>
#include "alignment_panel.h"
#include <stdio.h>

int gridx = 5;
int gridy = 5;
int snap = 3;

int include_H_from_C = 1;

void alignment_cb(fltk::Input *i, long v) {
  int n = (int)strtol(i->value(),0,0);
  if (n < 0) n = 0;
  switch (v) {
  case 1: gridx = n; break;
  case 2: gridy = n; break;
  case 3: snap  = n; break;
  }
}

extern const char* header_file_name;
extern const char* code_file_name;

void show_alignment_cb(fltk::Widget *, void *) {
  if (!alignment_window) make_alignment_window();
  include_H_from_C_button->value(include_H_from_C);
  header_file_input->value(header_file_name);
  code_file_input->value(code_file_name);
  char buf[128];
  sprintf(buf,"%d",gridx); horizontal_input->value(buf);
  sprintf(buf,"%d",gridy); vertical_input->value(buf);
  sprintf(buf,"%d",snap); snap_input->value(buf);
  alignment_window->show();
}

void header_input_cb(fltk::Input* i, void*) {
  header_file_name = i->value();
}
void code_input_cb(fltk::Input* i, void*) {
  code_file_name = i->value();
}

void include_H_from_C_button_cb(fltk::CheckButton* b, void*) {
  include_H_from_C = b->value();
}

////////////////////////////////////////////////////////////////

const char* WindowType::type_name() const {return "fltk::Window";}

static const Enumeration window_type_menu[] = {
  {"Single", 0, (void*)fltk::Widget::WINDOW_TYPE},
  {"Double", 0, (void*)(fltk::Widget::WINDOW_TYPE+1), "fltk::DoubleBufferWindow"},
  {0}};

const Enumeration* WindowType::subtypes() const {return window_type_menu;}

int overlays_invisible;

// The following fltk::Widget is used to simulate the windows.  It has
// an overlay for the fluid ui, and special-cases the fltk::NO_BOX.

class Overlay_Window : public fltk::OverlayWindow {
  void draw();
  void draw_overlay();
public:
  WindowType *window;
  int handle(int);
  Overlay_Window(int w,int h) : fltk::OverlayWindow(w,h) {fltk::Group::current(0);}
};
void Overlay_Window::draw() {
  const int CHECKSIZE = 8;
  // see if box is clear or a frame or rounded:
  if ((damage()&fltk::DAMAGE_ALL) && !box()->fills_rectangle()) {
    // if so, draw checkerboard so user can see what areas are clear:
    for (int y = 0; y < h(); y += CHECKSIZE) 
      for (int x = 0; x < w(); x += CHECKSIZE) {
	fltk::setcolor(((y/(2*CHECKSIZE))&1) != ((x/(2*CHECKSIZE))&1) ?
		 fltk::WHITE : fltk::BLACK);
	fltk::fillrect(x,y,CHECKSIZE,CHECKSIZE);
      }
  }
  fltk::OverlayWindow::draw();
}

void Overlay_Window::draw_overlay() {
  window->draw_overlay();
}
int Overlay_Window::handle(int e) {
  return window->handle(e);
}

#include <fltk/StyleSet.h>
extern fltk::StyleSet* fluid_style_set;
extern fltk::StyleSet* style_set;

FluidType *WindowType::make() {
  FluidType *p = FluidType::current;
  while (p && !p->is_code_block()) p = p->parent;
  if (!p) {
    fltk::message("Please select a function");
    return 0;
  }
  style_set->make_current();
  WindowType *o = new WindowType();
  if (!this->o) {// template widget
    this->o = new fltk::Window(100,100);
    fltk::Group::current(0);
  }
  o->factory = this;
  o->drag = 0;
  o->numselected = 0;
  Overlay_Window *w = new Overlay_Window(100,100);
  fluid_style_set->make_current();
  w->window = o;
  o->o = w;
  o->add(p);
  o->modal = false;
  o->non_modal = false;
  o->set_xy = false;
  o->border = true;
  return o;
}

void WindowType::add_child(FluidType* cc, FluidType* before) {
  WidgetType* c = (WidgetType*)cc;
  fltk::Widget* b = before ? ((WidgetType*)before)->o : 0;
  ((fltk::Window*)o)->insert(*(c->o), b);
  o->redraw();
}

void WindowType::remove_child(FluidType* cc) {
  WidgetType* c = (WidgetType*)cc;
  ((fltk::Window*)o)->remove(c->o);
  o->redraw();
}

void WindowType::move_child(FluidType* cc, FluidType* before) {
  WidgetType* c = (WidgetType*)cc;
  ((fltk::Window*)o)->remove(c->o);
  fltk::Widget* b = before ? ((WidgetType*)before)->o : 0;
  ((fltk::Window*)o)->insert(*(c->o), b);
  o->redraw();
}

////////////////////////////////////////////////////////////////

// Double-click on window widget shows the window, or if already shown,
// it shows the control panel.
void WindowType::open() {
  Overlay_Window *w = (Overlay_Window *)o;
  if (w->shown()) {
    w->show();
    WidgetType::open();
  } else {
    w->size_range(10, 10, 0, 0, gridx, gridy);
    w->show();
  }
}

// control panel items:
#include "widget_panel.h"

void modal_cb(fltk::CheckButton* i, void* v) {
  if (v == LOAD) {
    if (!current_widget->is_window()) {i->hide(); return;}
    i->show();
    i->value(((WindowType *)current_widget)->modal);
  } else {
    ((WindowType *)current_widget)->modal = i->value();
  }
}

void non_modal_cb(fltk::CheckButton* i, void* v) {
  if (v == LOAD) {
    if (!current_widget->is_window()) {i->hide(); return;}
    i->show();
    i->value(((WindowType *)current_widget)->non_modal);
  } else {
    ((WindowType *)current_widget)->non_modal = i->value();
  }
}

void border_cb(fltk::CheckButton* i, void* v) {
  if (v == LOAD) {
    if (!current_widget->is_window()) {i->hide(); return;}
    i->show();
    i->value(((WindowType *)current_widget)->border);
  } else {
    ((WindowType *)current_widget)->border = i->value();
  }
}

////////////////////////////////////////////////////////////////

void WindowType::setlabel(const char *n) {
  if (o) ((fltk::Window *)o)->label(n);
}

// make() is called on this widget when user picks window off New menu:
WindowType Windowtype;

// calculate actual move by moving mouse position (mx,my) to
// nearest multiple of gridsize, and snap to original position
void WindowType::newdx() {
  int dx, dy;
  if (fltk::event_state(fltk::ALT) || drag == BOX) {
    dx = mx-x1;
    dy = my-y1;
  } else {
    int dx0 = mx-x1;
    int ix = (drag&RIGHT) ? br : bx;
    dx = gridx ? ((ix+dx0+gridx/2)/gridx)*gridx - ix : dx0;
    if (dx0 > snap) {
      if (dx < 0) dx = 0;
    } else if (dx0 < -snap) {
      if (dx > 0) dx = 0;
    } else 
      dx = 0;
    int dy0 = my-y1;
    int iy = (drag&BOTTOM) ? by : bt;
    dy = gridy ? ((iy+dy0+gridy/2)/gridy)*gridy - iy : dy0;
    if (dy0 > snap) {
      if (dy < 0) dy = 0;
    } else if (dy0 < -snap) {
      if (dy > 0) dy = 0;
    } else 
      dy = 0;
  }
  if (this->dx != dx || this->dy != dy) {
    this->dx = dx; this->dy = dy;
    ((Overlay_Window *)(this->o))->redraw_overlay();
  }
}

// Move a widget according to dx and dy calculated above
void WindowType::newposition(WidgetType *o,int &X,int &Y,int &R,int &T) {
  X = o->o->x();
  Y = o->o->y();
  R = X+o->o->w();
  T = Y+o->o->h();
  if (!drag) return;
  if (drag&DRAG) {
    X += dx;
    Y += dy;
    R += dx;
    T += dy;
  } else {
    int ox = 0; int oy = 0;
    fltk::Group* p = o->o->parent();
    while (p->parent()) {ox += p->x(); oy += p->y(); p = p->parent();}
    if (drag&LEFT) if (X+ox==bx) X += dx; else if (X<bx+dx-ox) X = bx+dx-ox;
    if (drag&BOTTOM) if (Y+oy==by) Y += dy; else if (Y<by+dy-oy) Y = by+dy-oy;
    if (drag&RIGHT) if (R+ox==br) R += dx; else if (R>br+dx-ox) R = br+dx-ox;
    if (drag&TOP) if (T+oy==bt) T += dy; else if (T>bt+dx-oy) T = bt+dx-oy;
  }
  if (R<X) {int n = X; X = R; R = n;}
  if (T<Y) {int n = Y; Y = T; T = n;}
}

void WindowType::draw_overlay() {
  if (recalc) {
    bx = o->w(); by = o->h(); br = 0; bt = 0;
    numselected = 0;
    for (FluidType* q = first_child; q; q = q->walk(this)) {
      if (q->selected && q->is_widget() && !q->is_menu_item()) {
	numselected++;
	fltk::Widget* o = ((WidgetType*)q)->o;
	int x = o->x(); int y = o->y();
	fltk::Group* p = o->parent();
	while (p->parent()) {x += p->x(); y += p->y(); p = p->parent();}
	if (x < bx) bx = x;
	if (y < by) by = y;
	if (x+o->w() > br) br = x+o->w();
	if (y+o->h() > bt) bt = y+o->h();
      }
    }
    recalc = 0;
  }
  fltk::setcolor(fltk::RED);
  if (drag==BOX && (x1 != mx || y1 != my)) {
    int x = x1; int r = mx; if (x > r) {x = mx; r = x1;}
    int y = y1; int b = my; if (y > b) {y = my; b = y1;}
    fltk::strokerect(x,y,r-x,b-y);
  }
  if (overlays_invisible && !drag) return;
  if (selected) fltk::strokerect(0,0,o->w(),o->h());
  if (!numselected) return;
  int bx,by,br,bt;
  bx = o->w(); by = o->h(); br = 0; bt = 0;
  for (FluidType* q = first_child; q; q = q->walk(this)) {
    if (q->selected && q->is_widget() && !q->is_menu_item()) {
      int x,y,r,t;
      newposition((WidgetType*)q,x,y,r,t);
      fltk::Widget* o = ((WidgetType*)q)->o;
      fltk::Group* p = o->parent();
      while (p->parent()) {
	x += p->x(); r += p->x();
	y += p->y(); t += p->y();
	p = p->parent();
      }
      int hidden = (!o->visible_r());
      if (hidden) fltk::line_style(fltk::DASH);
      fltk::strokerect(x,y,r-x,t-y);
      if (x < bx) bx = x;
      if (y < by) by = y;
      if (r > br) br = r;
      if (t > bt) bt = t;
      if (hidden) fltk::line_style(fltk::SOLID);
    }
  }
  if (selected) return;
  if (numselected>1) fltk::strokerect(bx,by,br-bx,bt-by);
  fltk::fillrect(bx,by,5,5);
  fltk::fillrect(br-5,by,5,5);
  fltk::fillrect(br-5,bt-5,5,5);
  fltk::fillrect(bx,bt-5,5,5);
}

// Calculate new bounding box of selected widgets:
void WindowType::fix_overlay() {
  recalc = 1;
  ((Overlay_Window *)(this->o))->redraw_overlay();
}

// do that for every window (when selected set changes):
void redraw_overlays() {
  for (FluidType *o=FluidType::first; o; o=o->walk())
    if (o->is_window()) ((WindowType*)o)->fix_overlay();
}

#include <fltk/MenuBar.h>
extern fltk::MenuBar* menubar;

void toggle_overlays(fltk::Widget *,void *) {
  if (overlays_invisible)
    menubar->find("&Edit/Show Overlays")->set_value();
  else
    menubar->find("&Edit/Show Overlays")->clear_value();
  if (overlaybutton) overlaybutton->value(overlays_invisible);
  overlays_invisible = !overlays_invisible;
  for (FluidType *o=FluidType::first; o; o=o->walk())
    if (o->is_window()) {
      WidgetType* w = (WidgetType*)o;
      ((Overlay_Window*)(w->o))->redraw_overlay();
    }
}

extern void select(FluidType *,int);
extern void select_only(FluidType *);
extern FluidType* in_this_only;
extern void fix_group_size(FluidType *t);

// move the selected children according to current dx,dy,drag state:
void WindowType::moveallchildren()
{
  FluidType *i;
  for (i = first_child; i;) {
    if (i->selected && i->is_widget() && !i->is_menu_item()) {
      WidgetType* o = (WidgetType*)i;
      int x,y,r,t;
      newposition(o,x,y,r,t);
      o->o->resize(x,y,r-x,t-y);
      i = i->next_brother;
    } else {
      i = i->walk(this);
    }
  }
  for (i = first_child; i; i = i->walk(this))
    fix_group_size(i);
  o->redraw();
  recalc = 1;
  ((Overlay_Window *)(this->o))->redraw_overlay();
  modflag = 1;
  dx = dy = 0;
}

#include <FL/Fl_Menu_Item.h>

extern Fl_Menu_Item Main_Menu[];
extern Fl_Menu_Item New_Menu[];

// find the innermost item clicked on:
WidgetType* WindowType::clicked_widget() {
  WidgetType* selection = this;
  int x = 0; int y = 0;
  for (;;) {
    WidgetType* inner_selection = 0;
    for (FluidType* i = selection->first_child; i; i = i->next_brother) {
      if (i->is_widget() && !i->is_menu_item()) {
	WidgetType* o = (WidgetType*)i;
	fltk::Widget* w = o->o;
	if (w->visible_r()&&fltk::event_inside(w->x()+x,w->y()+y,w->w(),w->h()))
	  inner_selection = o;
      }
    }
    if (inner_selection) {
      selection = inner_selection;
      fltk::Widget* w = inner_selection->o;
      x += w->x();
      y += w->y();
    } else {
      break;
    }
  }
  return selection;
}

int WindowType::handle(int event) {
  static FluidType* selection;
  switch (event) {
  case fltk::PUSH:
    x1 = mx = fltk::event_x();
    y1 = my = fltk::event_y();
    drag = 0;
    // test for popup menu:
    if (fltk::event_button() >= 3) {
      in_this_only = this; // modifies how some menu items work.
      New_Menu->popup(mx,my,"New");
      in_this_only = 0;
      return 1;
    }
    selection = clicked_widget();
    // see if user grabs edges of selected region:
    if (numselected && !(fltk::event_state(fltk::SHIFT)) &&
	mx<=br+snap && mx>=bx-snap && my<=bt+snap && my>=by-snap) {
      int snap1 = snap>5 ? snap : 5;
      int w1 = (br-bx)/4; if (w1 > snap1) w1 = snap1;
      if (mx>=br-w1) drag |= RIGHT;
      else if (mx<bx+w1) drag |= LEFT;
      w1 = (bt-by)/4; if (w1 > snap1) w1 = snap1;
      if (my<=by+w1) drag |= BOTTOM;
      else if (my>bt-w1) drag |= TOP;
      if (!drag) drag = DRAG;
    }
    // do object-specific selection of other objects:
    {FluidType* t = selection->click_test(mx, my);
    if (t) {
      //if (t == selection) return 1; // indicates mouse eaten w/o change
      if (fltk::event_state(fltk::SHIFT)) {
	fltk::event_is_click(0);
	select(t, !t->selected);
      } else {
	select_only(t);
	if (t->is_menu_item()) t->open();
      }
      selection = t;
      drag = 0;
    } else {
      if (!drag) drag = BOX; // if all else fails, start a new selection region
    }}
    return 1;
  case fltk::DRAG:
    if (!drag) return 0;
    mx = fltk::event_x();
    my = fltk::event_y();
    newdx();
    return 1;
  case fltk::RELEASE:
    if (!drag) return 0;
    mx = fltk::event_x();
    my = fltk::event_y();
    newdx();
    if (drag != BOX && (dx || dy || !fltk::event_is_click())) {
      if (dx || dy) moveallchildren();
    } else if ((fltk::event_clicks() || fltk::event_state(fltk::CTRL))) {
      WidgetType::open();
    } else {
      if (mx<x1) {int t = x1; x1 = mx; mx = t;}
      if (my<y1) {int t = y1; y1 = my; my = t;}
      int n = 0;
      int toggle = fltk::event_state(fltk::SHIFT);
      if (toggle) fltk::event_is_click(0);

      // select everything in box:
      for (FluidType* i = first_child; i; i = i->walk(this)) {
	if (i->is_widget() && !i->is_menu_item()) {
	  fltk::Widget* o = ((WidgetType*)i)->o;
	  int x = o->x(); int y = o->y();
	  fltk::Group* p = o->parent(); if (!p->visible_r()) continue;
	  while (p->parent()) {x += p->x(); y += p->y(); p = p->parent();}
	  if (x >= x1 && y > y1 && x+o->w() < mx && y+o->h() < my) {
	    if (toggle) select(i, !i->selected);
	    else if (!n) select_only(i);
	    else select(i, 1);
	    n++;
	  }
	}
      }

      // if nothing in box, select what was clicked on:
      if (!n) {
	// find the innermost item clicked on:
	selection = clicked_widget();
	if (toggle) select(selection, !selection->selected);
	else select_only(selection);
      }
      if (overlays_invisible) toggle_overlays(0,0);
      ((Overlay_Window *)(this->o))->redraw_overlay();
    }
    drag = 0;
    if (widget_x)
      {
	x_cb (widget_x, LOAD);
	y_cb (widget_y, LOAD);
	width_cb (widget_width, LOAD);
	height_cb (widget_height, LOAD);
      }
    return 1;

  case fltk::FOCUS:
  case fltk::UNFOCUS:
    return 1;

  case fltk::KEY: {

    switch (fltk::event_key()) {

    case fltk::EscapeKey:
      ((fltk::Window*)o)->hide();
      return 1;

    case fltk::TabKey: {
      int backtab = (fltk::event_state(fltk::SHIFT));
      // see if the current item is in this window:
      FluidType *i = FluidType::current;
      while (i && i->parent != this) i = i->parent;
      if (i) {
	i = FluidType::current;
	for (;;) {
	  if (backtab) {
	    if (i->previous_brother) {
	      i = i->previous_brother;
	      while (i->first_child) {
		i = i->first_child;
		while (i->next_brother) i = i->next_brother;
	      }
	    } else {
	      i = i->parent;
	      if (i == this) i = 0;
	    }
	  } else i = i->walk(this);
	  if (!i) break;
	  if (i->is_widget() && !i->is_menu_item() &&
	      ((WidgetType*)i)->o->parent()->visible_r()) break;
	}
      }
      if (!i) i = first_child;
      select_only(i);
      return 1;}

    case fltk::LeftKey:  dx = -1; dy = 0; goto ARROW;
    case fltk::RightKey: dx = +1; dy = 0; goto ARROW;
    case fltk::UpKey:    dx = 0; dy = -1; goto ARROW;
    case fltk::DownKey:  dx = 0; dy = +1; goto ARROW;
    ARROW:
      // for some reason BOTTOM/TOP are swapped... should be fixed...
      drag = (fltk::event_state(fltk::SHIFT)) ? (RIGHT|TOP) : DRAG;
      if (fltk::event_state(fltk::CTRL)) {dx *= gridx; dy *= gridy;}
      moveallchildren();
      drag = 0;
      return 1;

    case 'o':
      toggle_overlays(0, 0);
      break;

    default:
      return 0;
    }}

  case fltk::SHORTCUT: {
    in_this_only = this; // modifies how some menu items work.
    const Fl_Menu_Item* r = Main_Menu->test_shortcut();
    in_this_only = 0;
    return r != 0;}

  default:
    return ((Overlay_Window *)o)->fltk::OverlayWindow::handle(event);
  }
}

////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

void WindowType::write_code() {
  WidgetType::write_code1();
  if (first_child) {
    write_c("%so->begin();\n", indent());
    for (FluidType* q = first_child; q; q = q->next_brother) q->write_code();
    write_c("%so->end();\n", indent());
  }
  write_extra_code();
  if (modal) write_c("%so->set_modal();\n", indent());
  else if (non_modal) write_c("%so->set_non_modal();\n", indent());
  if (!border) write_c("%so->clear_border();\n", indent());
  if (((fltk::Window*)o)->resizable() == o)
    write_c("%so->resizable(o);\n", indent());
  write_block_close();
}

void WindowType::write_properties() {
  WidgetType::write_properties();
  if (modal) write_string("modal");
  else if (non_modal) write_string("non_modal");
  if (!border) write_string("noborder");
  if (o->visible()) write_string("visible");
}

extern int pasteoffset;
void WindowType::read_property(const char *c) {
  if (!strcmp(c,"modal")) {
    modal = 1;
  } else if (!strcmp(c,"non_modal")) {
    non_modal = 1;
  } else if (!strcmp(c, "visible")) {
    if (fltk::Window::first()) open(); // only if we are using user interface
  } else if (!strcmp(c,"noborder")) {
    border = 0;
  } else if (!strcmp(c,"xclass")) {
    ; // obsolete string, ignore it
  } else if (!strcmp(c,"xywh")) {
    WidgetType::read_property(c);
    pasteoffset = 0; // make it not apply to contents
  } else {
    WidgetType::read_property(c);
  }
}

int WindowType::read_fdesign(const char* name, const char* value) {
  int x;
  o->box(fltk::NO_BOX); // because fdesign always puts an fltk::Box next
  if (!strcmp(name,"Width")) {
    if (sscanf(value,"%d",&x) == 1) o->size(x,o->h());
  } else if (!strcmp(name,"Height")) {
    if (sscanf(value,"%d",&x) == 1) o->size(o->w(),x);
  } else if (!strcmp(name,"NumberofWidgets")) {
    return 1; // we can figure out count from file
  } else if (!strcmp(name,"border")) {
    if (sscanf(value,"%d",&x) == 1) border = x;
  } else if (!strcmp(name,"title")) {
    label(value);
  } else {
    return WidgetType::read_fdesign(name,value);
  }
  return 1;
}

//
// End of "$Id: Fl_Window_Type.cxx,v 1.41 2002/12/09 04:52:22 spitzak Exp $".
//
