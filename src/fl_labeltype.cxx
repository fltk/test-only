//
// "$Id$"
//
// Label drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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
#include <fltk3/Fl_Image.H>

extern void fl_shadow_label(const Fl_Label*,int,int,int,int,fltk3::Align);
extern void fl_engraved_label(const Fl_Label*,int,int,int,int,fltk3::Align);
extern void fl_embossed_label(const Fl_Label*,int,int,int,int,fltk3::Align);
extern void fl_icon_label(const Fl_Label*,int,int,int,int,fltk3::Align);

extern void fl_multi_measure(const Fl_Label* o, int& w, int& h);
extern void fl_multi_labeltype(const Fl_Label* o, int x, int y, int w, int h, fltk3::Align a);


void fl_no_label(const Fl_Label*,int,int,int,int,fltk3::Align) {}

void
fl_normal_label(const Fl_Label* o, int X, int Y, int W, int H, fltk3::Align align)
{
  fl_font(o->font, o->size);
  fl_color((Fl_Color)o->color);
  fl_draw(o->value, X, Y, W, H, align, o->image);
}

void
fl_normal_measure(const Fl_Label* o, int& W, int& H) {
  fl_font(o->font, o->size);
  fl_measure(o->value, W, H);
  if (o->image) {
    if (o->image->w() > W) W = o->image->w();
    H += o->image->h();
  }
}

#define MAX_LABELTYPE 16

static Fl_Label_Draw_F* table[MAX_LABELTYPE] = {
  fl_normal_label,
  fl_no_label,
  fl_shadow_label,
  fl_engraved_label,
  fl_embossed_label,
  fl_multi_labeltype,
  fl_icon_label,
  // fltk3:FREE_LABELTYPE+n:
  fl_no_label, fl_no_label, fl_no_label,
  fl_no_label, fl_no_label, fl_no_label,
  fl_no_label, fl_no_label, fl_no_label
};

static Fl_Label_Measure_F* measure[MAX_LABELTYPE] = {
  0,
  0, 
  0,
  0, 
  0,
  fl_multi_measure,
};

/** Sets the functions to call to draw and measure a specific labeltype. */
void fltk3::set_labeltype(fltk3::Labeltype t,Fl_Label_Draw_F* f,Fl_Label_Measure_F*m) 
{
  table[t] = f; measure[t] = m;
}

////////////////////////////////////////////////////////////////

/** Draws a label with arbitrary alignment in an arbitrary box. */
void Fl_Label::draw(int X, int Y, int W, int H, fltk3::Align align) const {
  if (!value && !image) return;
  table[type](this, X, Y, W, H, align);
}
/** 
    Measures the size of the label.
    \param[in,out] W, H : this is the requested size for the label text plus image;
         on return, this will contain the size needed to fit the label
*/
void Fl_Label::measure(int& W, int& H) const {
  if (!value && !image) {
    W = H = 0;
    return;
  }

  Fl_Label_Measure_F* f = ::measure[type]; if (!f) f = fl_normal_measure;
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
  if (flags()&SHORTCUT_LABEL) fl_draw_shortcut = 1;
  Fl_Label l1 = label_;
  if (!active_r()) {
    l1.color = fl_inactive((Fl_Color)l1.color);
    if (l1.deimage) l1.image = l1.deimage;
  }
  l1.draw(X,Y,W,H,a);
  fl_draw_shortcut = 0;
}

// include these vars here so they can be referenced without including
// fltk3::Input_ code:
#include <fltk3/Input_.h>

//
// End of "$Id$".
//
