//
// "$Id: Fl_Tabs.cxx,v 1.59 2002/12/09 04:52:26 spitzak Exp $"
//
// Tab widget for the Fast Light Tool Kit (FLTK).
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

// This is the "file card tabs" interface to allow you to put lots and lots
// of buttons and switches in a panel, as popularized by many toolkits.

// Each child widget is a card, and it's label() is printed on the card tab.
// Clicking the tab makes that card visible.

#include <fltk/TabGroup.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/layout.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/Tooltip.h>
using namespace fltk;

#define BORDER 10
#define TABSLOPE 5
#define EXTRASPACE 5

// return the left edges of each tab (plus a fake left edge for a tab
// past the right-hand one).  These position are actually of the left
// edge of the slope.  They are either seperated by the correct distance
// or by TABSLOPE or by zero.
// Return value is the index of the selected item.

int TabGroup::tab_positions(int* p, int* w) {
  int selected = 0;
  int i;
  p[0] = 0;
  int numchildren = children();
  for (i=0; i<numchildren; i++) {
    Widget* o = child(i);
    if (o->visible()) selected = i;
    if (o->label()) {
      int wt = 0; int ht = 0; o->measure_label(wt,ht);
      w[i] = wt+TABSLOPE+EXTRASPACE;
      //if (2*TABSLOPE > w[i]) w[i] = 2*TABSLOPE;
    } else 
      w[i] = 2*TABSLOPE;
    p[i+1] = p[i]+w[i];
  }
  int r = this->w()-TABSLOPE-1;
  if (p[i] <= r) return selected;
  // uh oh, they are too big:
  // pack them against right edge:
  p[i] = r;
  for (i = children(); i--;) {
    int l = r-w[i];
    if (p[i+1]-TABSLOPE < l) l = p[i+1]-TABSLOPE;
    if (p[i] <= l) break;
    p[i] = l;
    r -= TABSLOPE;
  }
  // pack them against left edge and truncate width if they still don't fit:
  for (i = 0; i<children(); i++) {
    if (p[i] >= i*TABSLOPE) break;
    p[i] = i*TABSLOPE;
    int W = this->w()-1-TABSLOPE*(children()-i) - p[i];
    if (w[i] > W) w[i] = W;
  }
  // adjust edges according to visiblity:
  for (i = children(); i > selected; i--) {
    p[i] = p[i-1]+w[i-1];
  }
  return selected;
}

// return space needed for tabs.  Negative to put them on the bottom:
int TabGroup::tab_height() {
  int H = h();
  int H2 = 0;
  int numchildren = children();
  for (int i=0; i < numchildren; i++) {
    Widget* o = child(i);
    if (o->y() < H) H = o->y();
    if (o->y()+o->h() > H2) H2 = o->y()+o->h();
  }
  H2 = h()-H2;
  int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  if (H2 > H) {
    H = H2-dy;
    return (H <= 0) ? 0 : -H-1;
  } else {
    H = H-dy;
    return (H <= 0) ? 0 : H;
  }
}

// this is used by fluid to pick tabs:
Widget *TabGroup::which(int event_x, int event_y) {
  int H = tab_height();
  if (H < 0) {
    if (event_y > h() || event_y < h()+H) return 0;
  } else {
    if (event_y > H || event_y < 0) return 0;
  }
  if (event_x < 0) return 0;
  int p[128], w[128];
  int selected = tab_positions(p, w);
  int d = (event_y-(H>=0?0:h()))*TABSLOPE/H;
  for (int i=0; i<children(); i++) {
    if (event_x < p[i+1]+(i<selected ? TABSLOPE-d : d)) return child(i);
  }
  return 0;
}

int TabGroup::handle(int event) {

  Widget *selected = this->value();
  int i;
  int backwards = 0;

  switch (event) {

  case FOCUS:
    if (contains(fltk::focus())) {
      // this is called to indicate that some child got the focus
      /*if (focus() == this || focus() < 0)*/ redraw(DAMAGE_VALUE);
      focus(fltk::focus() == this ? -1 : 0);
      return true;
    }
    // otherwise this indicates that somebody is trying to give focus to this
    switch (navigation_key()) {
    case LeftKey:
    case UpKey:
      if (tab_height() < 0) goto GOTO_TABS; else goto GOTO_CONTENTS;
    case RightKey:
    case DownKey:
      if (tab_height() < 0) goto GOTO_CONTENTS; else goto GOTO_TABS;
    default:
    GOTO_CONTENTS:
      // Try to give the contents the focus. Also preserve a return value
      // of 2 (which indicates the contents have a text field):
      if (selected) {
	int n = selected->handle(FOCUS);
	if (n) {
	  if (!selected->contains(fltk::focus())) focus(selected);
	  return n;
	}
      }
    GOTO_TABS:
      focus(-1);
      return true;
    }

  case UNFOCUS:
    if (focus() < 0) redraw(DAMAGE_VALUE);
    return 1;

  // handle mouse events in the tabs:
  case PUSH: {
    int H = tab_height();
    if (H >= 0) {
      if (event_y() > H) break;
    } else {
      if (event_y() < h()+H) break;
    }}
  case DRAG:
  case RELEASE:
    selected = which(event_x(), event_y());
    if (event == RELEASE && !pushed()) {
      push(0);
      if (selected && value(selected)) do_callback();
    } else {
      push(selected);
    }
    return 1;

  // handle pointing at the tabs to bring up tooltip:
  case ENTER:
  case MOVE: {
    int H = tab_height();
    if (H >= 0) {
      if (event_y() > H) break;
    } else {
      if (event_y() < h()+H) break;
    }
    Widget* item = which(event_x(), event_y());
    //belowmouse(this);
    if (item) Tooltip::enter(this, 0, H<0?h()+H:0, w(), H<0?-H:H,
			     item->tooltip());
    return 1;}

  case KEY:
    if (fltk::focus()==this) {
      switch (event_key()) {
      case SpaceKey:
      case RightKey:
	goto MOVE;
      case BackSpaceKey:
      case LeftKey:
	backwards = 1;
      MOVE:
	for (i = children()-1; i>0; i--) if (child(i)->visible()) break;
	if (backwards) {i = i ? i-1 : children()-1;}
	else {i++; if (i >= children()) i = 0;}
	value(child(i)); do_callback();
	return 1;
      }
    }      
    switch (navigation_key()) {
    case RightKey:
    case DownKey:
      if (tab_height()<0) goto UP_CASE;
    DOWN_CASE:
      if (focus() < 0 && selected) return selected->take_focus();
      else return 0;
    case LeftKey:
    case UpKey:
      if (tab_height()<0) goto DOWN_CASE;
    UP_CASE:
      if (focus() >= 0) {focus(this); return 1;}
      else return 0;
    default:
      return 0;
    }

  // This makes ctrl+tab move between tabs when the focus is on a
  // widget that is not inside the tabs:
  case SHORTCUT:
    if (event_key() == TabKey && event_state(CTRL)) {
      backwards = event_state(SHIFT);
      goto MOVE;
    }
    break;
  }

  if (selected) return selected->send(event);
  return 0;
}

int TabGroup::push(Widget *o) {
  if (push_ == o) return 0;
  if (push_ && !push_->visible() || o && !o->visible())
    redraw(DAMAGE_VALUE);
  push_ = o;
  return 1;
}

// The value() is the first visible child (or the last child if none
// are visible) and this also hides any other children.
// This allows the tabs to be deleted, moved to other groups, and
// show()/hide() called without it screwing up.
Widget* TabGroup::value() {
  Widget* v = 0;
  int numchildren = children();
  for (int i=0; i < numchildren; i++) {
    Widget* o = child(i);
    if (v) o->hide();
    else if (o->visible()) v = o;
    else if (i==numchildren-1) {o->show(); v = o;}
  }
  return v;
}

// Setting the value hides all other children, and makes this one
// visible, iff it is really a child:
int TabGroup::value(Widget *newvalue) {
  int setfocus = !focused() && contains(fltk::focus());
  int numchildren = children();
  for (int i=0; i < numchildren; i++) {
    Widget* o = child(i);
    if (o == newvalue) {
      if (o->visible()) return 0; // no change
      o->show();
      if (setfocus) o->take_focus();
    } else {
      o->hide();
    }
  }
  return 1;
}

enum {LEFT, RIGHT, SELECTED};

extern Widget* fl_did_clipping;

static int H;
static int p[128];
void TabGroup::draw() {
  Widget *v = value();

  H = tab_height();
  if (damage() & DAMAGE_ALL) { // redraw the entire thing:
    push_clip(0, 0, w(), h());
    if (v) draw_child(*v);
    draw_background();
/*  setcolor(color());
    fillrect(0, H>=0 ? 0 : h()+H, w(), H>=0?H:-H);*/
    pop_clip();
  } else { // redraw the child
    if (v) update_child(*v);
  }

  // draw the tabs if needed:
  if (damage() & (DAMAGE_VALUE|DAMAGE_ALL)) {
    int w[128];
    int selected = tab_positions(p,w);
    int i;

    for (i=0; i<selected; i++)
      draw_tab(p[i], p[i+1], w[i], H, child(i), LEFT);
    for (i=children()-1; i > selected; i--)
      draw_tab(p[i], p[i+1], w[i], H, child(i), RIGHT);
    if (v) {
      i = selected;
      draw_tab(p[i], p[i+1], w[i], H, child(i), SELECTED);
    } else {
      // draw the edge when no selection:
      setcolor(H >= 0 ? GRAY99 : GRAY33);
      int b = H >= 0 ? H : h()+H;
      drawline(0, b, this->w(), b);
    }

  }
  if (damage() & DAMAGE_EXPOSE) {
    clip_out(0, H>=0 ? 0 : h()+H, p[children()]+TABSLOPE, (H>=0?H:-H));
    clip_out(0, H>0 ? H : 0, this->w(), h()-(H>=0?H:-H-1));
    fl_did_clipping = this;
  }
}

void TabGroup::draw_tab(int x1, int x2, int W, int H, Widget* o, int what) {
  if (x2 < x1+W) {
    if (what == LEFT) {
      if (x1+W < x2+TABSLOPE) x2 = x1+W;
      else x2 += TABSLOPE;
    } else {
      if (x1+W < x2+TABSLOPE) x1 = x2-W;
      else x1 -= TABSLOPE;
    }
  }
  int sel = (what == SELECTED);
  setcolor(o->color());
  if (H >= 0) {
    newpath();
    addvertex(x1, H+sel);
    addvertex(x1+TABSLOPE, 0);
    addvertex(x2, 0);
    addvertex(x2+TABSLOPE, H+sel);
    fillpath();
    setcolor(!sel && o==push_ ? GRAY33 : GRAY99);
    drawline(x1, H, x1+TABSLOPE, 0);
    drawline(x1+TABSLOPE, 0, x2, 0);
    if (sel) {
      if (x1>0) drawline(0, H, x1, H);
      if (x2+TABSLOPE < w()-1) drawline(x2+TABSLOPE, H, w()-1, H);
    }
    setcolor(!sel && o==push_ ? GRAY99 : GRAY33);
    drawline(x2, 0, x2+TABSLOPE, H);
  } else {
    newpath();
    addvertex(x1, h()+H-sel);
    addvertex(x1+TABSLOPE, h());
    addvertex(x2, h());
    addvertex(x2+TABSLOPE, h()+H-sel);
    fillpath();
    setcolor(!sel && o==push_ ? GRAY99 : GRAY33);
    newpath();
    addvertex(x1+TABSLOPE, h()-1);
    addvertex(x2, h()-1);
    addvertex(x2+TABSLOPE, h()+H);
    strokepath();
    if (sel) {
      if (x1>0) drawline(0, h()+H, x1, h()+H);
      if (x2+TABSLOPE < w()-1) drawline(x2+TABSLOPE, h()+H, w()-1, h()+H);
    }
    setcolor(!sel && o==push_ ? GRAY33 : GRAY99);
    drawline(x1, h()+H, x1+TABSLOPE, h()-1);
  }
  if (W > TABSLOPE+EXTRASPACE/2) {
    int x = (what==LEFT ? x1 : x2-W)+(TABSLOPE+EXTRASPACE/2);
    int w = W-(TABSLOPE+EXTRASPACE/2);
    int y,h;
    if (H<0) {
      y = this->h()+H;
      h = -H-1;
    } else {
      y = 2;
      h = H-1;
    }
    o->draw_label(x, y, w, h, ALIGN_CENTER);
    if (sel && focused()) {
      if (H<0) y--;
      focusbox()->draw(x, y, w, h, BLACK, INVISIBLE);
    }
  }
}

static void revert(Style* s) {
  s->box = THIN_UP_BOX;
  s->color = GRAY75;
}
static NamedStyle style("Tabs", revert, &TabGroup::default_style);
NamedStyle* TabGroup::default_style = &::style;

TabGroup::TabGroup(int X,int Y,int W, int H, const char *l)
  : Group(X,Y,W,H,l)
{
  style(default_style);
  push_ = 0;
}

//
// End of "$Id: Fl_Tabs.cxx,v 1.59 2002/12/09 04:52:26 spitzak Exp $".
//
