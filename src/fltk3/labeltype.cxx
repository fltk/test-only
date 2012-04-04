//
// "$Id$"
//
// Label drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

// Drawing code for the (one) common label types.
// Other label types (symbols) are in their own source files
// to avoid linking if not used.

#include <fltk3/run.h>
#include <fltk3/Widget.h>
#include <fltk3/Group.h>
#include <fltk3/draw.h>
#include <fltk3/Image.h>

void
fl_no_label(const fltk3::Label*,int,int,int,int,fltk3::Align) {}

void
fl_normal_label(const fltk3::Label* o, int X, int Y, int W, int H, fltk3::Align align)
{
  fltk3::font(o->labelfont(), o->labelsize());
  fltk3::color((fltk3::Color)o->labelcolor());
  fltk3::draw(o->label(), X, Y, W, H, align, (fltk3::Image*)o->image());
}

void
fl_normal_measure(const fltk3::Label* o, int& W, int& H) {
  fltk3::font(o->labelfont(), o->labelsize());
  fltk3::measure(o->label(), W, H);
  if (o->image()) {
    if (o->image()->w() > W) W = o->image()->w();
    H += o->image()->h();
  }
}

#define MAX_LABELTYPE 16

void fl_shadow_label(const fltk3::Label* o, int X, int Y, int W, int H, fltk3::Align align);
void fl_engraved_label(const fltk3::Label* o, int X, int Y, int W, int H, fltk3::Align align);
void fl_embossed_label(const fltk3::Label* o, int X, int Y, int W, int H, fltk3::Align align);
void fl_multi_labeltype(const fltk3::Label* o, int x, int y, int w, int h, fltk3::Align a);
void fl_multi_measure(const fltk3::Label* o, int& w, int& h);
void fl_icon_labeltype(const fltk3::Label* o, int x, int y, int w, int h, fltk3::Align a);

static fltk3::LabelDrawF* table[MAX_LABELTYPE] = {
  fl_normal_label,
  fl_no_label,
  fl_shadow_label,      // fltk3::SHADOW_LABEL,
  fl_engraved_label,    // fltk3::ENGRAVED_LABEL,
  fl_embossed_label,    // fltk3::EMBOSSED_LABEL,
  fl_multi_labeltype,   // fltk3::MULTI_LABEL,
  fl_icon_labeltype,    // fltk3::ICON_LABEL,
  fltk3::Image::labeltype, // fltk3::IMAGE_LABEL
  // fltk3::FREE_LABELTYPE+n:
  fl_no_label, fl_no_label,
  fl_no_label, fl_no_label, fl_no_label,
  fl_no_label, fl_no_label, fl_no_label
};

static fltk3::LabelMeasureF* measure[MAX_LABELTYPE] = {
  0L, // normal
  0L, // no
  0L, // shadow
  0L, // engraved
  0L, // embossed
  fl_multi_measure, // multi
  0L, //icon
  fltk3::Image::measure // image
};

/** Sets the functions to call to draw and measure a specific labeltype. */
void fltk3::set_labeltype(fltk3::Labeltype t,fltk3::LabelDrawF* f,fltk3::LabelMeasureF*m) 
{
  table[t] = f; ::measure[t] = m;
}

////////////////////////////////////////////////////////////////

/** Draws a label with arbitrary alignment in an arbitrary box. */
void fltk3::Label::draw(int X, int Y, int W, int H, fltk3::Align align) const {
  if (!labeltext_ && !image()) return;
  table[labeltype()](this, X, Y, W, H, align);
}

/** 
    Measures the size of the label.
    \param[in,out] W, H : this is the requested size for the label text plus image;
         on return, this will contain the size needed to fit the label
*/
void fltk3::Label::measure(int& W, int& H) const {
  if (!labeltext_ && !image()) {
    W = H = 0;
    return;
  }

  fltk3::LabelMeasureF* f = ::measure[labeltype()]; if (!f) f = fl_normal_measure;
  f(this, W, H);
}

/** Draws the widget's label at the defined label position.
    This is the normal call for a widget's draw() method.
 */
void fltk3::Widget::draw_label() const {
  int X = x_+fltk3::box_dx(box());
  int W = w_-fltk3::box_dw(box());
  if (W > 11 && align()&(fltk3::ALIGN_LEFT|fltk3::ALIGN_RIGHT)) {X += 3; W -= 6;}
  draw_label(X, y_+fltk3::box_dy(box()), W, h_-fltk3::box_dh(box()));
}

/** Draws the label in an arbitrary bounding box.
    draw() can use this instead of draw_label(void) to change the bounding box
 */
void fltk3::Widget::draw_label(int X, int Y, int W, int H) const {
  // quit if we are not drawing a label inside the widget:
  if ((align()&15) && !(align() & fltk3::ALIGN_INSIDE)) return;
  draw_label(X,Y,W,H,align());
}

/** Draws the label in an arbitrary bounding box with an arbitrary alignment.
    Anybody can call this to force the label to draw anywhere.
 */
void fltk3::Widget::draw_label(int X, int Y, int W, int H, fltk3::Align a) const {
  if (flags()&SHORTCUT_LABEL) fltk3::draw_shortcut = 1;
  if (!active_r()) {
    fltk3::Label l1(*this);
    fltk3::Color c = l1.labelcolor();
    l1.labelcolor( fltk3::inactive((fltk3::Color)l1.labelcolor()) );
    if (l1.deimage()) l1.image(l1.deimage());
    l1.draw(X,Y,W,H,a);
    l1.labelcolor(c); // necessary because l1 and this share the same style
  } else {
    Label::draw(X,Y,W,H,a);
  }
  fltk3::draw_shortcut = 0;
}

// include these vars here so they can be referenced without including
// fltk3::Input_ code:
#include <fltk3/Input_.h>

//
// End of "$Id$".
//
