//
// "$Id: fl_labeltype.cxx,v 1.44 2003/11/04 08:11:03 spitzak Exp $"
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
#include <fltk/string.h>
#include <config.h>
using namespace fltk;

LabelType* LabelType::find(const char* name) {
  for (LabelType* p = LabelType::first; p; p = p->next)
    if (p->name && !strcasecmp(name, p->name)) return p;
  return 0;
}

LabelType* LabelType::first = 0;

////////////////////////////////////////////////////////////////

extern bool fl_drawing_shadow;

void LabelType::draw(const char* label,
		      int X, int Y, int W, int H,
		      const Style* style, Flags flags) const
{
  if (flags&OUTPUT)
    setfont(style->textfont(), style->textsize());
  else
    setfont(style->labelfont(), style->labelsize());
  Color bg, fg; style->boxcolors(flags, bg, fg);
  if (flags & INACTIVE) {
    fl_drawing_shadow = true;
    setcolor(GRAY90);
    drawtext(label, X+1, Y+1, W, H, flags);
    fl_drawing_shadow = false;
  }
  setcolor(fg);
  drawtext(label, X, Y, W, H, flags);
}

static LabelType normalLabel("normal");
LabelType* const fltk::NORMAL_LABEL = &normalLabel;
LabelType* const fltk::SYMBOL_LABEL = &normalLabel;

////////////////////////////////////////////////////////////////

class FL_API NoLabel : public LabelType {
public:
  void draw(const char*, int,int,int,int, const Style*, Flags) const {}
  NoLabel(const char * n) : LabelType(n) {}
};
static NoLabel noLabel("none");
LabelType* const fltk::NO_LABEL = &noLabel;

////////////////////////////////////////////////////////////////
// Drawing methods (designed to be called from a draw() implementation):

/** Change the graphics state so we are ready to draw into the widget.
    The transformation is set so 0,0 is at the upper-left corner of
    the widget and 1 unit equals one pixel. The transformation stack
    is empied, and all other graphics state is left in unknown
    settings.  This will already be done before draw() is called on a
    widget.
*/
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

/** Draw the widget's box() such that it fills the entire area of the
    widget. If the box is not rectangluar, this also draws the area
    of the parent widget that is exposed. The box drawing routine is
    passed the style() and current_flags() of th ewidget.
*/
void Widget::draw_box() const {
  Box* box = this->box();
  if (damage()&DAMAGE_EXPOSE && !box->fills_rectangle() && parent())
    draw_background();
  box->draw(0, 0, w(), h(), style(), current_flags()|OUTPUT);
  // draw a big x to show where image will be drawn:
//    setcolor(BLACK);
//    drawline(0,0,w(),h());
//    drawline(0,h(),w(),0);
}

/** Same as draw_box() but draws only the boundary of the box() by
    calling it's draw routine with the INVISIBLE flag set. This only
    works for rectangular boxes.  This is useful for avoiding blinking
    during update for widgets that erase their contents as part of
    redrawing them anyway (ie anything displaying text).
*/
void Widget::draw_frame() const {
  box()->draw(0, 0, w(), h(), style(), current_flags()|INVISIBLE|OUTPUT);
}

/** Return the flags that should be passed to drawing functions.

    This differs from the value of flags() in these ways:

    VALUE,INACTIVE,HIGHLIGHT are turned off

    INACTIVE is turned on if active_r() is false.
*/
Flags Widget::current_flags() const {
  Flags f = flags() & ~(VALUE|INACTIVE|HIGHLIGHT);
  if (!active_r()) f |= INACTIVE;
  return f;
}

/** Return the flags that should be passed to drawing functions.

    Same as current_flags() but HIGHLIGHT is turned on if the
    widget is active and belowmouse() is true.
*/
Flags Widget::current_flags_highlight() const {
  Flags f = flags() & ~(VALUE|INACTIVE|HIGHLIGHT);
  if (!active_r()) f |= INACTIVE;
  else if (takesevents() && belowmouse()) f |= HIGHLIGHT;
  return f;
}

/** Calls draw_label() with the area inside the box() and with
    the style() and flags().
*/
void Widget::draw_label() const {
  Flags flags = current_flags();
  // Do a quick test to see if we don't want to draw anything:
  if (!image() && (!label() || !*label() ||
		   (flags&15) && !(flags & ALIGN_INSIDE))) return;
  // figure out the inside of the box():
  int X=0; int Y=0; int W=w_; int H=h_; box()->inset(X,Y,W,H);
  // and draw it:
  draw_label(X,Y,W,H,style(),flags);
}

/** Draws labels inside the widget. \a XYWH is the bounding box to
    fit the label into. \a style and \a flags are used to decide what
    color to draw the label.

    If the flags contain any ALIGN flags and don't have ALIGN_INSIDE
    then the label() is not drawn. Instead the image() is drawn to
    fill the box (most image() types will center the picture).

    Otherwise it tries to put both the label() and the image() into the
    box in a nice way. The image() is put against the side that any
    ALIGN flags say, and then the label() is put next to that.
*/
void Widget::draw_label(int X, int Y, int W, int H, const Style* style, Flags flags) const
{
  // If label is drawn outside, draw the image only:
  if ((flags&15) && !(flags & ALIGN_INSIDE)) {
    if (!image_) return;
    float fw = W;
    float fh = H;
    image_->measure(fw, fh);
    if (flags & ALIGN_CLIP) push_clip(X, Y, W, H);
    image_->draw(X+((W-int(fw))>>1), Y+((H-int(fh))>>1),W,H,style,flags);
    if (flags & ALIGN_CLIP) pop_clip();
    return;
  }

  if (flags & ALIGN_CLIP) push_clip(X, Y, W, H);

  if (image_) {

    float fw = W;
    float fh = H;
    image_->measure(fw, fh);
    int w = int(fw);
    int h = int(fh);

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
	if (flags&OUTPUT)
	  setfont(textfont(), textsize());
	else
	  setfont(labelfont(), labelsize());
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

    image_->draw(X-cx, Y-cy, w, h, style, flags);

    // figure out the rectangle that remains for text:
    if (flags & ALIGN_LEFT) {X += w; W -= w;}
    else if (flags & ALIGN_RIGHT) W -= w;
    else if (flags & ALIGN_TOP) {Y += h; H -= h;}
    else if (flags & ALIGN_BOTTOM) H -= h;
    else {Y += (h-cy); H -= (h-cy); /*flags |= ALIGN_TOP;*/}
  }

  if (label_ && *label_) {
    // add some interior border for the text:
    if (W > 6 && flags&ALIGN_LEFT) {
      if (W > 9) {X += 3; W -= 6;}
      else {X += W-6; W = 6;}
    }
    if (W > 6 && flags&ALIGN_RIGHT) {
      if (W > 9) W -= 6;
      else W = 6;
    }
    labeltype()->draw(label_, X, Y, W, H, style, flags);
  }

  if (flags & ALIGN_CLIP) pop_clip();
}

/** Groups normally call this to draw the label() outside a widget.
    This uses the flags() to determine where to put the label. If
    the ALIGN flags are all zero, or if ALIGN_INSIDE is turned
    on, then nothing is done. Otherwise the align is used to select
    a rectangle outside the widget and the widget's label() is
    formatted into that area.

    The font is set to labelfont()/labelsize(), and labelcolor()
    is used to color the text. The flags are passed to the draw()
    function, but with the alignment changed to put the text against
    the widget, and INACTIVE is added if active_r() is false.

    The image() of the widget is not drawn by this. It is always
    drawn inside the widget.
*/
void Group::draw_outside_label(Widget& w) const {
  if (!w.visible()) return;
  // skip any labels that are inside the widget:
  if (!(w.flags()&15) || (w.flags() & ALIGN_INSIDE)) return;
  if (!w.label() || !*w.label()) return;
  // invent a box that is outside the widget:
  Flags align = w.flags();
  int X = w.x();
  int Y = w.y();
  int W = w.w();
  int H = w.h();
  if (align & ALIGN_TOP) {
    align ^= (ALIGN_BOTTOM|ALIGN_TOP);
    Y = 0;
    H = w.y();
  } else if (align & ALIGN_BOTTOM) {
    align ^= (ALIGN_BOTTOM|ALIGN_TOP);
    Y = Y+H;
    H = h()-Y;
  } else if (align & ALIGN_LEFT) {
    align ^= (ALIGN_LEFT|ALIGN_RIGHT);
    X = 0;
    W = w.x()-3;
  } else if (align & ALIGN_RIGHT) {
    align ^= (ALIGN_LEFT|ALIGN_RIGHT);
    X = X+W+3;
    W = this->w()-X;
  }
  if (!w.active_r()) align |= INACTIVE;
  //push_clip(X, Y, W, H); // this will break some old fltk programs
  w.labeltype()->draw(w.label(), X, Y, W, H, w.style(), align);
  //pop_clip();
}

/** Replace \a w and \a h with the size of the area the label will
    take up. This is the size of the draw_outside_label() and thus
    does not include any image() and always uses the labelfont even
    if the OUTPUT flag is set.

    If the ALIGN_WRAP flag is set this chooses the rather arbitrary
    width of 300 to wrap the label at. Ideally this should have been
    passed in \a w but is not for back-compatability reasons.
*/
void Widget::measure_label(int& w, int& h) const {
  setfont(labelfont(), labelsize());
  w = h = 300; // rather arbitrary choice for maximum wrap width
  measure(label(), w, h, flags());
}

//
// End of "$Id: fl_labeltype.cxx,v 1.44 2003/11/04 08:11:03 spitzak Exp $".
//
