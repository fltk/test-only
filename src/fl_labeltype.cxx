//
// "$Id: fl_labeltype.cxx,v 1.30 2002/09/09 01:39:58 spitzak Exp $"
//
// Label drawing routines for the Fast Light Tool Kit (FLTK).
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

// Drawing code for the (one) common label types.
// Other label types (symbols) are in their own source files
// to avoid linking if not used.

#include <fltk/Fl_Labeltype.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Widget.h>
#include <fltk/Fl_Image.h>
#include <string.h>
#include <config.h>

void Fl_No_Label::draw(const char*, int, int, int, int, Fl_Color, Fl_Flags)
const {}
Fl_No_Label fl_no_label("none");

void Fl_Labeltype_::draw(const char* label,
			 int X, int Y, int W, int H,
			 Fl_Color color, Fl_Flags flags) const
{
  if (flags & FL_INACTIVE) {
    fl_color(FL_LIGHT2);
    fl_draw(label, X+1, Y+1, W, H, flags);
    color = fl_inactive(color);
  }
  fl_color(color);
  fl_draw(label, X, Y, W, H, flags);
}
Fl_Labeltype_ fl_normal_label("normal");

////////////////////////////////////////////////////////////////

// The normal call for a draw() method. Draws the label inside the
// widget's box, if the align is set to draw an inside label.
void Fl_Widget::draw_inside_label() const {
  if (!(flags()&15) || (flags() & FL_ALIGN_INSIDE)) {
    int X=0; int Y=0; int W=w_; int H=h_; box()->inset(X,Y,W,H);
    if (W > 11 && flags()&(FL_ALIGN_LEFT|FL_ALIGN_RIGHT)) {X += 3; W -= 6;}
    draw_label(X, Y, W, H, flags());
  }
}

// Draws only inside labels, but allows the caller to specify the box.
// Also allows the caller to turn on some extra flags.
void Fl_Widget::draw_inside_label(int X, int Y, int W, int H, Fl_Flags f) const
{
  if (!(flags()&15) || (flags() & FL_ALIGN_INSIDE)) {
    if (W > 11 && flags()&(FL_ALIGN_LEFT|FL_ALIGN_RIGHT)) {X += 3; W -= 6;}
    draw_label(X, Y, W, H, f|flags()&FL_ALIGN_MASK);
  }
}

// Anybody can call this to force the label to draw anywhere, this is
// used by Fl_Group and Fl_Tabs to draw outside labels:
void Fl_Widget::draw_label(int X, int Y, int W, int H, Fl_Flags flags) const
{
  fl_font(label_font(), label_size());
  if (!active_r()) flags |= FL_INACTIVE;

  Fl_Color color;
  // Figure out if alignment puts the label inside the widget:
  if (!(this->flags()&15) || (this->flags() & FL_ALIGN_INSIDE)) {
    // yes, inside label is affected by selection or highlight:
    if (flags&FL_SELECTED)
      color = selection_text_color();
    else if (flags&FL_HIGHLIGHT && (color = highlight_label_color()))
      ;
    else
      color = label_color();
    if (focused()) flags |= FL_SELECTED;
  } else {
    color = label_color();
  }

  if (flags & FL_ALIGN_CLIP) fl_push_clip(X, Y, W, H);

  if (image_) {

    int w = W;
    int h = H;
    image_->measure(w, h);

    // If all the flags are off, draw the image and label centered "nicely"
    // by measuring their total size and centering that rectangle:
    if (!(flags & (FL_ALIGN_LEFT|FL_ALIGN_RIGHT|FL_ALIGN_TOP|FL_ALIGN_BOTTOM|
		   FL_ALIGN_INSIDE)) && label_) {
      int d = (H-int(h+fl_height()))>>1;
      if (d >= 0) {
	// put the image atop the text
	Y += d; H -= d; flags |= FL_ALIGN_TOP;
      } else {
	// put image to left
	int text_w = W; int text_h = H;
	fl_measure(label_, text_w, text_h, flags);
	int d = (W-(h+text_w))>>1;
	if (d > 0) {X += d; W -= d;}
	flags |= FL_ALIGN_LEFT;
      }
    }

    int cx,cy; // point in image to put at X,Y
    if (flags & FL_ALIGN_RIGHT) {
      cx = w-W;
      if (flags & FL_ALIGN_LEFT && cx < 0) cx = 0;
    }
    else if (flags & FL_ALIGN_LEFT) cx = 0;
    else cx = w/2-W/2;
    if (flags & FL_ALIGN_BOTTOM) {
      cy = h-H;
      if (flags & FL_ALIGN_TOP && cy < 0) cy = 0;
    }
    else if (flags & FL_ALIGN_TOP) cy = 0;
    else cy = h/2-H/2;

    fl_color(fl_inactive(color, flags));
    image_->draw(X-cx, Y-cy, W, H, flags);

    // figure out the rectangle that remains for text:
    if (flags & FL_ALIGN_LEFT) {X += w; W -= w;}
    else if (flags & FL_ALIGN_RIGHT) W -= w;
    else if (flags & FL_ALIGN_TOP) {Y += h; H -= h;}
    else if (flags & FL_ALIGN_BOTTOM) H -= h;
    else {Y += (h-cy); H -= (h-cy); /*flags |= FL_ALIGN_TOP;*/}
  }

  if (label_ && *label_) {
    label_type()->draw(label_, X, Y, W, H, color, flags);
  }

  if (flags & FL_ALIGN_CLIP) fl_pop_clip();
}

void Fl_Widget::measure_label(int& w, int& h) const {
  fl_font(label_font(), label_size());
  w = h = 300; // rather arbitrary choice for maximum wrap width
  fl_measure(label(), w, h, flags());
}

const Fl_Labeltype_* Fl_Labeltype_::find(const char* name) {
  for (const Fl_Labeltype_* p = Fl_Labeltype_::first; p; p = p->next)
    if (p->name && !strcasecmp(name, p->name)) return p;
  return 0;
}

const Fl_Labeltype_* Fl_Labeltype_::first = 0;

//
// End of "$Id: fl_labeltype.cxx,v 1.30 2002/09/09 01:39:58 spitzak Exp $".
//
