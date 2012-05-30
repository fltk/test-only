//
// "$Id$"
//
// Scroll widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <fltk3/run.h>
#include <fltk3/TiledImage.h>
#include <fltk3/ScrollGroup.h>
#include <fltk3/draw.h>

/** Clear all but the scrollbars... */
void fltk3::ScrollGroup::clear() {
  // Note: the scrollbars are removed from the group before calling
  // fltk3::Group::clear() to take advantage of the optimized widget removal
  // and deletion. Finally they are added to fltk3::ScrollGroup's group again. This
  // is MUCH faster than removing the widgets one by one (STR #2409).

  remove(scrollbar);
  remove(hscrollbar);
  Group::clear();
  add(hscrollbar);
  add(scrollbar);
}

/** Insure the scrollbars are the last children */
void fltk3::ScrollGroup::fix_scrollbar_order() {
  fltk3::Widget** a = (fltk3::Widget**)array();
  if (a[children()-1] != &scrollbar) {
    int i,j; for (i = j = 0; j < children(); j++)
      if (a[j] != &hscrollbar && a[j] != &scrollbar) a[i++] = a[j];
    a[i++] = &hscrollbar;
    a[i++] = &scrollbar;
  }
}

// Draw widget's background and children within a specific clip region
//    So widget can just redraw damaged parts.
//
void fltk3::ScrollGroup::draw_clip(void* v,int X, int Y, int W, int H) {
  fltk3::push_clip(X,Y,W,H);
  fltk3::ScrollGroup* s = (fltk3::ScrollGroup*)v;
  // erase background as needed...
  fltk3::Box* sbx = s->box();
  if (   sbx==fltk3::NO_BOX
      || sbx==fltk3::UP_FRAME
      || sbx==fltk3::DOWN_FRAME
      || sbx==fltk3::THIN_UP_FRAME
      || sbx==fltk3::THIN_DOWN_FRAME
      || sbx==fltk3::ENGRAVED_FRAME
      || sbx==fltk3::EMBOSSED_FRAME
      || sbx==fltk3::BORDER_FRAME
      || sbx==fltk3::SHADOW_FRAME
      || sbx==fltk3::ROUNDED_FRAME
      || sbx==fltk3::OVAL_FRAME
      || sbx==fltk3::PLASTIC_UP_FRAME
      || sbx==fltk3::PLASTIC_DOWN_FRAME)
  {
    if (s->parent() == (fltk3::Group *)s->window() && fltk3::scheme_bg_) {
      fltk3::scheme_bg_->draw(X-(X%((fltk3::TiledImage *)fltk3::scheme_bg_)->image()->w()),
                              Y-(Y%((fltk3::TiledImage *)fltk3::scheme_bg_)->image()->h()),
                              W+((fltk3::TiledImage *)fltk3::scheme_bg_)->image()->w(),
                              H+((fltk3::TiledImage *)fltk3::scheme_bg_)->image()->h());
    }
  } else {
    fltk3::color(s->color());
    fltk3::rectf(X,Y,W,H);
  }
  fltk3::Widget*const* a = s->array();
  for (int i=s->children()-2; i--;) {
    fltk3::Widget& o = **a++;
    s->draw_child(o);
    s->draw_outside_label(o);
  }
  fltk3::pop_clip();
}

/**
   Calculate visibility/size/position of scrollbars, find children's bounding box.
   The \p si paramater will be filled with data from the calculations.
   Derived classes can make use of this call to figure out the scrolling area
   eg. during resize() handling.
   \param[in] si -- ScrollInfo structure
   \returns Structure containing the calculated info.
*/
void fltk3::ScrollGroup::recalc_scrollbars(ScrollInfo &si) {

  // inner box of widget (excluding scrollbars)
  si.innerbox_x =     fltk3::box_dx(box());
  si.innerbox_y =     fltk3::box_dy(box());
  si.innerbox_w = w()-fltk3::box_dw(box());
  si.innerbox_h = h()-fltk3::box_dh(box());

  // accumulate a bounding box for all the children
  si.child_l = si.innerbox_x;
  si.child_r = si.innerbox_x;
  si.child_b = si.innerbox_y;
  si.child_t = si.innerbox_y;
  int first = 1;
  fltk3::Widget*const* a = array();
  for (int i=children()-2; i--;) {
    fltk3::Widget* o = *a++;
    if ( first ) {
        first = 0;
	si.child_l = o->x();
	si.child_r = o->x()+o->w();
	si.child_b = o->y()+o->h();
	si.child_t = o->y();
    } else {
	if (o->x() < si.child_l) si.child_l = o->x();
	if (o->y() < si.child_t) si.child_t = o->y();
	if (o->x()+o->w() > si.child_r) si.child_r = o->x()+o->w();
	if (o->y()+o->h() > si.child_b) si.child_b = o->y()+o->h();
    }
  }

  // Turn the scrollbars on and off as necessary.
  // See if children would fit if we had no scrollbars...
  {
    int X = si.innerbox_x;
    int Y = si.innerbox_y;
    int W = si.innerbox_w;
    int H = si.innerbox_h;

    si.scrollsize = scrollbar_size_ ? scrollbar_size_ : fltk3::scrollbar_size();
    si.vneeded = 0;
    si.hneeded = 0;
    if (type() & VERTICAL) {
      if ((type() & ALWAYS_ON) || si.child_t < Y || si.child_b > Y+H) {
	si.vneeded = 1;
	W -= si.scrollsize;
	if (scrollbar.align() & fltk3::ALIGN_LEFT) X += si.scrollsize;
      }
    }
    if (type() & HORIZONTAL) {
      if ((type() & ALWAYS_ON) || si.child_l < X || si.child_r > X+W) {
	si.hneeded = 1;
	H -= si.scrollsize;
	if (scrollbar.align() & fltk3::ALIGN_TOP) Y += si.scrollsize;
	// recheck vertical since we added a horizontal scrollbar
	if (!si.vneeded && (type() & VERTICAL)) {
	  if ((type() & ALWAYS_ON) || si.child_t < Y || si.child_b > Y+H) {
	    si.vneeded = 1;
	    W -= si.scrollsize;
	    if (scrollbar.align() & fltk3::ALIGN_LEFT) X += si.scrollsize;
	  }
	}
      }
    }
    si.innerchild_x = X;
    si.innerchild_y = Y;
    si.innerchild_w = W;
    si.innerchild_h = H;
  }

  // calculate hor scrollbar position
  si.hscroll_x = si.innerchild_x;
  si.hscroll_y = (scrollbar.align() & fltk3::ALIGN_TOP) 
		     ? si.innerbox_y
		     : si.innerbox_y + si.innerbox_h - si.scrollsize;
  si.hscroll_w = si.innerchild_w;
  si.hscroll_h = si.scrollsize;

  // calculate ver scrollbar position
  si.vscroll_x = (scrollbar.align() & fltk3::ALIGN_LEFT)
                     ? si.innerbox_x
		     : si.innerbox_x + si.innerbox_w - si.scrollsize;
  si.vscroll_y = si.innerchild_y;
  si.vscroll_w = si.scrollsize;
  si.vscroll_h = si.innerchild_h;

  // calculate h/v scrollbar values (pos/size/first/total)
  si.hpos = si.innerchild_x - si.child_l;
  si.hsize = si.innerchild_w;
  si.hfirst = 0;
  si.htotal = si.child_r - si.child_l;
  if ( si.hpos < 0 ) { si.htotal += (-si.hpos); si.hfirst = si.hpos; }

  si.vpos = si.innerchild_y - si.child_t;
  si.vsize = si.innerchild_h;
  si.vfirst = 0;
  si.vtotal = si.child_b - si.child_t;
  if ( si.vpos < 0 ) { si.vtotal += (-si.vpos); si.vfirst = si.vpos; }

//  printf("DEBUG --- ScrollInfo ---\n");
//  printf("DEBUG        scrollsize: %d\n", si.scrollsize);
//  printf("DEBUG  hneeded, vneeded: %d %d\n", si.hneeded, si.vneeded);
//  printf("DEBUG     innerbox xywh: %d %d %d %d\n", si.innerbox_x,   si.innerbox_y,   si.innerbox_w,   si.innerbox_h);
//  printf("DEBUG   innerchild xywh: %d %d %d %d\n", si.innerchild_x, si.innerchild_y, si.innerchild_w, si.innerchild_h);
//  printf("DEBUG        child lrbt: %d %d %d %d\n", si.child_l, si.child_r, si.child_b, si.child_t);
//  printf("DEBUG      hscroll xywh: %d %d %d %d\n", si.hscroll_x, si.hscroll_y, si.hscroll_w, si.hscroll_h);
//  printf("DEBUG      vscroll xywh: %d %d %d %d\n", si.vscroll_x, si.vscroll_y, si.vscroll_w, si.vscroll_h);
//  printf("DEBUG  horz scroll vals: %d %d %d %d\n", si.hpos, si.hsize, si.hfirst, si.htotal);
//  printf("DEBUG  vert scroll vals: %d %d %d %d\n", si.vpos, si.vsize, si.vfirst, si.vtotal);
//  printf("DEBUG \n");
}

/**
  Returns the bounding box for the interior of the scrolling area, inside
  the scrollbars.
  
  Currently this is only reliable after draw(), and before any resizing of
  the fltk3::ScrollGroup or any child widgets occur.
  
  \todo The visibility of the scrollbars ought to be checked/calculated
  outside of the draw() method (STR #1895).
*/
void fltk3::ScrollGroup::bbox(int& X, int& Y, int& W, int& H) {
  X =     fltk3::box_dx(box());
  Y =     fltk3::box_dy(box());
  W = w()-fltk3::box_dw(box());
  H = h()-fltk3::box_dh(box());
  if (scrollbar.visible()) {
    W -= scrollbar.w();
    if (scrollbar.align() & fltk3::ALIGN_LEFT) X += scrollbar.w();
  }
  if (hscrollbar.visible()) {
    H -= hscrollbar.h();
    if (scrollbar.align() & fltk3::ALIGN_TOP) Y += hscrollbar.h();
  }
}

void fltk3::ScrollGroup::draw() {
  fix_scrollbar_order();
  int X,Y,W,H; bbox(X,Y,W,H);

  uchar d = damage();

  if (d & fltk3::DAMAGE_ALL) { // full redraw
    draw_box(box(),0,0,w(),h(),color());
    draw_clip(this, X, Y, W, H);
  } else {
    if (d & fltk3::DAMAGE_SCROLL) {
      // scroll the contents:
      fltk3::scroll(X, Y, W, H, oldx-xposition_, oldy-yposition_, draw_clip, this);

      // Erase the background as needed...
      fltk3::Widget*const* a = array();
      int L, R, T, B;
      L = 999999;
      R = 0;
      T = 999999;
      B = 0;
      for (int i=children()-2; i--; a++) {
        if ((*a)->x() < L) L = (*a)->x();
	if (((*a)->x() + (*a)->w()) > R) R = (*a)->x() + (*a)->w();
        if ((*a)->y() < T) T = (*a)->y();
	if (((*a)->y() + (*a)->h()) > B) B = (*a)->y() + (*a)->h();
      }
      if (L > X) draw_clip(this, X, Y, L - X, H);
      if (R < (X + W)) draw_clip(this, R, Y, X + W - R, H);
      if (T > Y) draw_clip(this, X, Y, W, T - Y);
      if (B < (Y + H)) draw_clip(this, X, B, W, Y + H - B);
    }
    if (d & fltk3::DAMAGE_CHILD) { // draw damaged children
      fltk3::push_clip(X, Y, W, H);
      fltk3::Widget*const* a = array();
      for (int i=children()-2; i--;) update_child(**a++);
      fltk3::pop_clip();
    }
  }

  // Calculate where scrollbars should go, and draw them
  {
      ScrollInfo si;
      recalc_scrollbars(si);

      // Now that we know what's needed, make it so.
      if (si.vneeded && !scrollbar.visible()) {
	scrollbar.set_visible();
	d = fltk3::DAMAGE_ALL;
      }
      else if (!si.vneeded && scrollbar.visible()) {
	scrollbar.clear_visible();
	draw_clip(this, si.vscroll_x, si.vscroll_y, si.vscroll_w, si.vscroll_h);
	d = fltk3::DAMAGE_ALL;
      }
      if (si.hneeded && !hscrollbar.visible()) {
	hscrollbar.set_visible();
	d = fltk3::DAMAGE_ALL;
      }
      else if (!si.hneeded && hscrollbar.visible()) {
	hscrollbar.clear_visible();
	draw_clip(this, si.hscroll_x, si.hscroll_y, si.hscroll_w, si.hscroll_h);
	d = fltk3::DAMAGE_ALL;
      }
      else if ( hscrollbar.h() != si.scrollsize || scrollbar.w() != si.scrollsize ) {
         // scrollsize changed
         d = fltk3::DAMAGE_ALL;
      }

      scrollbar.resize(si.vscroll_x, si.vscroll_y, si.vscroll_w, si.vscroll_h);
      oldy = yposition_ = si.vpos;	// si.innerchild_y - si.child_t;
      scrollbar.value(si.vpos, si.vsize, si.vfirst, si.vtotal);

      hscrollbar.resize(si.hscroll_x, si.hscroll_y, si.hscroll_w, si.hscroll_h);
      oldx = xposition_ = si.hpos;	// si.innerchild_x - si.child_l;
      hscrollbar.value(si.hpos, si.hsize, si.hfirst, si.htotal);
  }

  // draw the scrollbars:
  if (d & fltk3::DAMAGE_ALL) {
    draw_child(scrollbar);
    draw_child(hscrollbar);
    if (scrollbar.visible() && hscrollbar.visible()) {
      // fill in the little box in the corner
      fltk3::color(color());
      fltk3::rectf(scrollbar.x(), hscrollbar.y(), scrollbar.w(), hscrollbar.h());
    }
  } else {
    update_child(scrollbar);
    update_child(hscrollbar);
  }
}

void fltk3::ScrollGroup::resize(int X, int Y, int W, int H) {
  int dx = X-x(), dy = Y-y();
  int dw = W-w(), dh = H-h();
  Widget::resize(X,Y,W,H); // resize _before_ moving children around
  fix_scrollbar_order();
  // move all the children:
  fltk3::Widget*const* a = array();
  for (int i=children()-2; i--;) {
    fltk3::Widget* o = *a++;
    o->position(o->x()+dx, o->y()+dy);
  }
  if (dw==0 && dh==0) {
    char pad = ( scrollbar.visible() && hscrollbar.visible() );
    char al = ( (scrollbar.align() & fltk3::ALIGN_LEFT) != 0 );
    char at = ( (scrollbar.align() & fltk3::ALIGN_TOP)  !=0 );
    scrollbar.position(al?X:X+W-scrollbar.w(), (at&&pad)?Y+hscrollbar.h():Y);
    hscrollbar.position((al&&pad)?X+scrollbar.w():X, at?Y:Y+H-hscrollbar.h());
  } else {
    // FIXME recalculation of scrollbars needs to be moved out fo "draw()" (STR #1895)
    redraw(); // need full recalculation of scrollbars
  }
}

/**  Moves the contents of the scroll group to a new position.*/
void fltk3::ScrollGroup::scroll_to(int X, int Y) {
  int dx = xposition_-X;
  int dy = yposition_-Y;
  if (!dx && !dy) return;
  xposition_ = X;
  yposition_ = Y;
  fltk3::Widget*const* a = array();
  for (int i=children(); i--;) {
    fltk3::Widget* o = *a++;
    if (o == &hscrollbar || o == &scrollbar) continue;
    o->position(o->x()+dx, o->y()+dy);
  }
  if (parent() == (fltk3::Group *)window() && fltk3::scheme_bg_) damage(fltk3::DAMAGE_ALL);
  else damage(fltk3::DAMAGE_SCROLL);
}

void fltk3::ScrollGroup::hscrollbar_cb(fltk3::Widget* o, void*) {
  fltk3::ScrollGroup* s = (fltk3::ScrollGroup*)(o->parent());
  s->scroll_to(int(((fltk3::Scrollbar*)o)->value()), s->yposition());
}

void fltk3::ScrollGroup::scrollbar_cb(fltk3::Widget* o, void*) {
  fltk3::ScrollGroup* s = (fltk3::ScrollGroup*)(o->parent());
  s->scroll_to(s->xposition(), int(((fltk3::Scrollbar*)o)->value()));
}
/**
  Creates a new fltk3::ScrollGroup widget using the given position,
  size, and label string. The default boxtype is fltk3::NO_BOX.
  <P>The destructor <I>also deletes all the children</I>. This allows a
  whole tree to be deleted at once, without having to keep a pointer to
  all the children in the user code. A kludge has been done so the 
  fltk3::ScrollGroup and all of its children can be automatic (local)
  variables, but you must declare the fltk3::ScrollGroup<I>first</I>, so
  that it is destroyed last.
*/
fltk3::ScrollGroup::ScrollGroup(int X,int Y,int W,int H,const char* L)
: fltk3::Group(X,Y,W,H,L), 
  scrollbar(X+W-fltk3::scrollbar_size(),Y,
            fltk3::scrollbar_size(),H-fltk3::scrollbar_size()),
  hscrollbar(X,Y+H-fltk3::scrollbar_size(),
             W-fltk3::scrollbar_size(),fltk3::scrollbar_size()) 
{
  type(BOTH);
  xposition_ = oldx = 0;
  yposition_ = oldy = 0;
  scrollbar_size_ = 0;
  hscrollbar.type(fltk3::HORIZONTAL);
  hscrollbar.callback(hscrollbar_cb);
  scrollbar.callback(scrollbar_cb);
}

int fltk3::ScrollGroup::handle(int event) {
  fix_scrollbar_order();
  return Group::handle(event);
}

//
// End of "$Id$".
//
