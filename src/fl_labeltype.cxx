//
// "$Id: fl_labeltype.cxx,v 1.33 2002/12/10 02:01:01 easysw Exp $"
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
//

// Originally this contained the definition of LabelType. It now
// contains all the drawing code used by widgets.

#include <fltk/LabelType.h>
#include <fltk/Box.h>
#include <fltk/Image.h>
#include <fltk/Widget.h>
#include <fltk/Window.h>
#include <fltk/damage.h>
#include <fltk/draw.h>
#include <string.h>
#include <config.h>
using namespace fltk;

LabelType* LabelType::find(const char* name) {
  for (LabelType* p = LabelType::first; p; p = p->next)
    if (p->name && !strcasecmp(name, p->name)) return p;
  return 0;
}

LabelType* LabelType::first = 0;

////////////////////////////////////////////////////////////////

void LabelType::draw(const char* label,
		      int X, int Y, int W, int H,
		      Color color, Flags flags) const
{
  if (flags & INACTIVE) {
    draw(label, X+1, Y+1, W, H, GRAY90, flags&~INACTIVE);
    color = inactive(color);
  }
  if (!(flags&RAW_LABEL) && *label == '@') {
    int x1 = X;
    int y1 = Y;
    int w1 = W;
    int h1 = H;
    if (flags & 15) { // if alignement is not center
      if (w1 < h1) h1 = w1; else w1 = h1;
      if (flags & ALIGN_LEFT) ;
      else if (flags & ALIGN_RIGHT) x1 = X+W-w1;
      else x1 = X+((W-w1)>>1);
      if (flags & ALIGN_TOP) ;
      else if (flags & ALIGN_BOTTOM) y1 = Y+H-h1;
      else y1 = Y+((H-h1)>>1);
    }
    if (draw_symbol(label, x1, y1, w1, h1, color)) return;
  }
  setcolor(color);
  drawtext(label, X, Y, W, H, flags);
}

static LabelType normalLabel("normal");
LabelType* const fltk::NORMAL_LABEL = &normalLabel;
LabelType* const fltk::SYMBOL_LABEL = &normalLabel;

////////////////////////////////////////////////////////////////

class FL_API NoLabel : public LabelType {
public:
  void draw(const char*, int,int,int,int, Color, Flags) const {}
  NoLabel(const char * n) : LabelType(n) {}
};
static NoLabel noLabel("none");
LabelType* const fltk::NO_LABEL = &noLabel;

////////////////////////////////////////////////////////////////
// Drawing methods (designed to be called from a draw() implementation):

// Draw only the edge of the widget. Assummes boxtype is rectangular:
void Widget::draw_frame() const {
  Flags flags = this->flags();
  if (!active_r()) flags |= INACTIVE;
  if (focused()) flags |= SELECTED;
  box()->draw(0, 0, w(), h(), color(), flags|INVISIBLE);
}

// Fill the entire widget with it's box, handle non-rectangular boxes:
void Widget::draw_box() const {
  Box* box = this->box();
  if (damage()&DAMAGE_EXPOSE && !box->fills_rectangle() && parent())
    draw_background();
  Flags flags = this->flags();
  if (!active_r()) flags |= INACTIVE;
  if (focused()) flags |= SELECTED;
  box->draw(0, 0, w(), h(), color(), flags);
}

// Set up for incremental redraw:
void Widget::make_current() const {
  int x = 0;
  int y = 0;
  const Widget* widget = this;
  while (!widget->is_window()) {
    x += widget->x();
    y += widget->y();
    widget = widget->parent();
  }
  ((const Window*)widget)->make_current();
  translate(x,y);
}

// The normal call for a draw() method. Draws the label inside the
// widget's box, if the align is set to draw an inside label.
void Widget::draw_inside_label() const {
  if (!(flags()&15) || (flags() & ALIGN_INSIDE)) {
    int X=0; int Y=0; int W=w_; int H=h_; box()->inset(X,Y,W,H);
    if (W > 11 && flags()&(ALIGN_LEFT|ALIGN_RIGHT)) {X += 3; W -= 6;}
    draw_label(X, Y, W, H, flags());
  }
}

// Draws only inside labels, but allows the caller to specify the box.
// Also allows the caller to turn on some extra flags.
void Widget::draw_inside_label(int X, int Y, int W, int H, Flags f) const
{
  if (!(flags()&15) || (flags() & ALIGN_INSIDE)) {
    if (W > 11 && flags()&(ALIGN_LEFT|ALIGN_RIGHT)) {X += 3; W -= 6;}
    draw_label(X, Y, W, H, f|flags()&ALIGN_MASK);
  }
}

// Anybody can call this to force the label to draw anywhere, this is
// used by Group and TabGroup to draw outside labels:
void Widget::draw_label(int X, int Y, int W, int H, Flags flags) const
{
  setfont(labelfont(), labelsize());
  if (!active_r()) flags |= INACTIVE;

  Color color;
  // Figure out if alignment puts the label inside the widget:
  if (!(this->flags()&15) || (this->flags() & ALIGN_INSIDE)) {
    // yes, inside label is affected by selection or highlight:
    if (flags&SELECTED)
      color = selection_textcolor();
    else if (flags&HIGHLIGHT && (color = highlight_labelcolor()))
      ;
    else
      color = labelcolor();
    if (focused()) flags |= SELECTED;
  } else {
    color = labelcolor();
  }

  if (flags & ALIGN_CLIP) push_clip(X, Y, W, H);

  if (image_) {

    int w = W;
    int h = H;
    image_->measure(w, h);

    // If all the flags are off, draw the image and label centered "nicely"
    // by measuring their total size and centering that rectangle:
    if (!(flags & (ALIGN_LEFT|ALIGN_RIGHT|ALIGN_TOP|ALIGN_BOTTOM|
		   ALIGN_INSIDE)) && label_) {
      int d = (H-int(h+labelsize()+leading()+.5))>>1;
      if (d >= 0) {
	// put the image atop the text
	Y += d; H -= d; flags |= ALIGN_TOP;
      } else {
	// put image to left
	int text_w = W; int text_h = H;
	measure(label_, text_w, text_h, flags);
	int d = (W-(h+text_w))>>1;
	if (d > 0) {X += d; W -= d;}
	flags |= ALIGN_LEFT;
      }
    }

    int cx,cy; // point in image to put at X,Y
    if (flags & ALIGN_RIGHT) {
      cx = w-W;
      if (flags & ALIGN_LEFT && cx < 0) cx = 0;
    }
    else if (flags & ALIGN_LEFT) cx = 0;
    else cx = w/2-W/2;
    if (flags & ALIGN_BOTTOM) {
      cy = h-H;
      if (flags & ALIGN_TOP && cy < 0) cy = 0;
    }
    else if (flags & ALIGN_TOP) cy = 0;
    else cy = h/2-H/2;

    setcolor(inactive(color, flags));
    image_->draw(X-cx, Y-cy, W, H, flags);

    // figure out the rectangle that remains for text:
    if (flags & ALIGN_LEFT) {X += w; W -= w;}
    else if (flags & ALIGN_RIGHT) W -= w;
    else if (flags & ALIGN_TOP) {Y += h; H -= h;}
    else if (flags & ALIGN_BOTTOM) H -= h;
    else {Y += (h-cy); H -= (h-cy); /*flags |= ALIGN_TOP;*/}
  }

  if (label_ && *label_) {
    labeltype()->draw(label_, X, Y, W, H, color, flags);
  }

  if (flags & ALIGN_CLIP) pop_clip();
}

void Widget::measure_label(int& w, int& h) const {
  setfont(labelfont(), labelsize());
  w = h = 300; // rather arbitrary choice for maximum wrap width
  measure(label(), w, h, flags());
}

//
// End of "$Id: fl_labeltype.cxx,v 1.33 2002/12/10 02:01:01 easysw Exp $".
//
