//
// "$Id: Alternative.cxx,v 1.40 2002/02/10 22:57:50 spitzak Exp $"
//
// Theme plugin file for FLTK
//
// Copyright 1999 Bill Spitzak and others.
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

#include <fltk/Fl.h>
#include <fltk/Fl_Boxtype.h>
#include <fltk/Fl_Style.h>
#include <fltk/Fl_Widget.h>
#include <fltk/Fl_Button.h>
#include <fltk/fl_draw.h>

// a couple of of new boxtypes (look familiar?)
static const Fl_Frame_Box
alt_thick_down_box(0, "NNUUJJXXAAAA");

static const Fl_Frame_Box
alt_thick_up_box(0, "AAAAXXJJUUNN", &alt_thick_down_box);

enum {UPPER_LEFT, LOWER_RIGHT};

static void lozenge(int which, int x,int y,int w,int h, Fl_Color color)
{
  w--; h--;
  int d = w <= h ? w : h;
  if (d <= 1) return;
  fl_color(color);
  if (which == UPPER_LEFT) {
    fl_pie(x+w-d, y, d, d, 45, w<=h ? 180 : 90, FL_ARC);
    fl_pie(x, y+h-d, d, d, w<=h ? 180 : 90, 225, FL_ARC);
  } else { // LOWER_RIGHT
    fl_pie(x, y+h-d, d, d, 225, w<=h ? 360 : 270, FL_ARC);
    fl_pie(x+w-d, y, d, d, w<=h ? 360 : 270, 360+45, FL_ARC);
  }
  if (w < h) {
    if (which != UPPER_LEFT) fl_line(x+w, y+d/2, x+w, y+h-d/2);
    if (which != LOWER_RIGHT) fl_line(x, y+d/2, x, y+h-d/2);
  } else if (w > h) {
    if (which != UPPER_LEFT) fl_line(x+d/2, y+h, x+w-d/2, y+h);
    if (which != LOWER_RIGHT) fl_line(x+d/2, y, x+w-d/2, y);
  }
}

static Fl_Style* scrollbarstyle;

// Replace the glyph functions on several widget types:
static void
check_glyph(const Fl_Widget* widget, int t,
		int x, int y, int w, int h, Fl_Flags f)
{
  Fl_Color gbc = widget->color();
  Fl_Color gfc = fl_inactive(widget->text_color(), f);
  Fl_Color bc;
  if (f & FL_SELECTED) bc = widget->selection_color();
  else if (f & FL_HIGHLIGHT && (bc = widget->highlight_color())) ;
  else bc = widget->button_color();

  if (widget->type() == Fl_Button::RADIO) {

    Fl_Color light = 54, dark = 32;
    if (f&FL_INACTIVE) {
      light = fl_inactive(light); dark = fl_inactive(dark);
    }
    int d = w <= h ? w : h;
//  d = (d - 1)|1;
    d &= (~1);

    fl_color((f&FL_VALUE) ? gfc : gbc);
    fl_pie(x+2, y+2, d-4, d-4, 0, 360);

    lozenge(UPPER_LEFT,  x+2, y+1, d-4, d-2, light);
    lozenge(UPPER_LEFT,  x+1, y+2, d-2, d-4, light);
    lozenge(UPPER_LEFT,  x+1, y+1, d-2, d-2, light);
    lozenge(UPPER_LEFT,  x+2, y+2, d-4, d-4, light);

    lozenge(LOWER_RIGHT, x+2, y+1, d-4, d-2, dark);
    lozenge(LOWER_RIGHT, x+1, y+2, d-2, d-4, dark);
    lozenge(LOWER_RIGHT, x+1, y+1, d-2, d-2, dark);
    lozenge(LOWER_RIGHT, x+2, y+2, d-4, d-4, dark);

    lozenge(LOWER_RIGHT, x+1, y,   d-2, d,   light);
    lozenge(LOWER_RIGHT, x,   y+1, d,   d-2, light);
    lozenge(LOWER_RIGHT, x,   y,   d,   d,   light);
    lozenge(LOWER_RIGHT, x+1, y+1, d-2, d-2, light);

    lozenge(UPPER_LEFT,  x+1, y,   d-2, d,   dark);
    lozenge(UPPER_LEFT,  x,   y+1, d,   d-2, dark);
    lozenge(UPPER_LEFT,  x,   y,   d,   d,   dark);
    lozenge(UPPER_LEFT,  x+1, y+1, d-2, d-2, dark);

  } else { // check button:

    w = (w-1)|1; h = (h-1)|1;
    int x1 = x+w/2;
    int y1 = y+h/2;
    Fl_Color light = 54, dark = 32;

    if (f&FL_INACTIVE) {
      /*bc = fl_inactive(bc); fc = fl_inactive(fc);*/
      light = fl_inactive(light); dark = fl_inactive(dark);
    }
    fl_newpath();
    fl_vertex(x+3, y1); fl_vertex(x1, y+3);
    fl_vertex(x+w-4, y1); fl_vertex(x1, y+h-4);
    fl_color((f&FL_VALUE) ? gfc : gbc);
    fl_fill();

    fl_color(dark);
    fl_line(x,   y1, x1,  y);  fl_line(x1, y,   x+w-1, y1);
    fl_line(x+1, y1, x1, y+1); fl_line(x1, y+1, x+w-2, y1);
    fl_color(light);
    fl_line(x+2, y1, x1, y+2); fl_line(x1, y+2, x+w-3, y1);
    fl_line(x+3, y1, x1, y+3); fl_line(x1, y+3, x+w-4, y1);

    fl_color(light);
    fl_line(x,   y1, x1, y+h-1); fl_line(x1, y+h-1, x+w-1, y1);
    fl_line(x+1, y1, x1, y+h-2); fl_line(x1, y+h-2, x+w-2, y1);
    fl_color(dark);
    fl_line(x+2, y1, x1, y+h-3); fl_line(x1, y+h-3, x+w-3, y1);
    fl_line(x+3, y1, x1, y+h-4); fl_line(x1, y+h-4, x+w-4, y1);
  }
}

// The main glyph function draws all the arrows:
static void
alt_glyph(const Fl_Widget* widget, int t,
          int x, int y, int w, int h, Fl_Flags f)
{
  Fl_Color bc;
  if (f & FL_SELECTED) bc = widget->selection_color();
  else if (f & FL_HIGHLIGHT && (bc = widget->highlight_color())) ;
  else bc = widget->button_color();

  switch (t) {
    case FL_GLYPH_RIGHT_BUTTON:
    case FL_GLYPH_LEFT_BUTTON:
    case FL_GLYPH_UP_BUTTON:
    case FL_GLYPH_DOWN_BUTTON:
      // erase area behind scrollbars arrows
      fl_color(widget->color());
      fl_rectf(x,y,w,h);
      t -= (FL_GLYPH_RIGHT_BUTTON-FL_GLYPH_RIGHT);
      // make it square:
      if (w > h) {x += (w-h)/2; w = h;} else {y += (h-w)/2; h = w;}
      goto JUMP1;
    case FL_GLYPH_RIGHT:
    case FL_GLYPH_LEFT:
    case FL_GLYPH_UP:
    case FL_GLYPH_DOWN:
    JUMP1:
      {Fl_Color d1, d2, l1, l2;
      if (f&(FL_VALUE|FL_SELECTED)) {
        d1 = FL_LIGHT3; d2 = FL_LIGHT1; l1 = FL_BLACK; l2 = FL_DARK2;
      } else{
        l1 = FL_LIGHT3; l2 = FL_LIGHT1; d1 = FL_BLACK; d2 = FL_DARK2;
      }

      if (f&FL_INACTIVE) {
        l1 = fl_inactive(l1); l2 = fl_inactive(l2);
        d1 = fl_inactive(d1); d2 = fl_inactive(d2);
      }

      if (t == FL_GLYPH_RIGHT) {
        fl_color(bc);
        fl_newpath();
        fl_vertex(x,y); fl_vertex(x+w-1, y+h/2); fl_vertex(x, y+h-1);
        fl_fill();
        fl_color(l2); fl_line(x+1,y+h-2, x+1,y+1); fl_line(x+1,y+1, x+w-2,y+h/2);
        fl_color(d2); fl_line(x+1,y+h-2, x+w-2,y+h/2);
        fl_color(l1); fl_line(x,y+h-1, x,y); fl_line(x,y, x+w-1,y+h/2);
        fl_color(d1); fl_line(x,y+h-1, x+w-1,y+h/2);
      } else if (t == FL_GLYPH_LEFT) {
        fl_color(bc);
        fl_newpath();
        fl_vertex(x+w-1,y); fl_vertex(x+w-1,y+h-1); fl_vertex(x,y+h/2);
        fl_fill();
        fl_color(d2); fl_line(x+w-2,y+1, x+w-2,y+h-2); fl_line(x+w-2,y+h-2, x+1,y+h/2);
        fl_color(l2); fl_line(x+w-2,y+1, x+1,y+h/2);
        fl_color(d1); fl_line(x+w-1,y, x+w-1,y+h-1); fl_line(x+w-1,y+h-1, x,y+h/2);
        fl_color(l1); fl_line(x+w-1,y, x,y+h/2);
      } else if (t == FL_GLYPH_DOWN) {
        fl_color(bc);
        fl_newpath();

        fl_vertex(x,y); fl_vertex(x+w/2,y+h-1); fl_vertex(x+w-1,y);
        fl_fill();
        fl_color(l2); fl_line(x+w-2,y+1, x+1,y+1); fl_line(x+1,y+1, x+w/2,y+h-2);
        fl_color(d2); fl_line(x+w-2,y+1, x+w/2,y+h-2);
        fl_color(l1); fl_line(x+w-1,y, x,y); fl_line(x,y, x+w/2,y+h-1);
        fl_color(d1); fl_line(x+w-1,y, x+w/2,y+h-1);
      } else { // UP
        fl_color(bc);
        fl_newpath();
        fl_vertex(x,y+h-1); fl_vertex(x+w-1,y+h-1); fl_vertex(x+w/2,y);
        fl_fill();
        fl_color(d2); fl_line(x+1,y+h-2, x+w-2,y+h-2); fl_line(x+w-2,y+h-2, x+w/2,y+1);
        fl_color(l2); fl_line(x+1,y+h-2, x+w/2,y+1);
        fl_color(d1); fl_line(x,y+h-1, x+w-1,y+h-1); fl_line(x+w-1,y+h-1, x+w/2,y);
        fl_color(l1); fl_line(x,y+h-1, x+w/2,y);
      }
      break;
    }
    default:
      widget->button_box()->draw(x,y,w,h, bc, f);
  }
}

// Sliders. You need to look at the type of widget to figure out if
// it is horizontal or vertical.
static void
slider_glyph(const Fl_Widget* widget, int t,
          int x, int y, int w, int h, Fl_Flags f)
{
  // draw the up/down arrows using the main glyph function:
  if (t) {
    alt_glyph(widget, t, x, y, w, h, f);
    return;
  }
  Fl_Color bc;
  if (f & FL_SELECTED) bc = widget->selection_color();
  else if (f & FL_HIGHLIGHT && (bc = widget->highlight_color())) ;
  else bc = widget->button_color();
  if (widget->type() & 1) { // horizontal
    widget->button_box()->draw(x,y,w,h, bc, f);
    widget->button_box()->inset(x,y,w,h);
    if (w>10) FL_THIN_UP_BOX->draw(x+w/2-1, y+1, 2, h-2, bc, f);
    if (w>18) {
      FL_THIN_UP_BOX->draw(x+w/2-1-4, y+1, 2, h-2, bc, f);
      FL_THIN_UP_BOX->draw(x+w/2-1+4, y+1, 2, h-2, bc, f);
    }
  } else {
    widget->button_box()->draw(x,y,w,h, bc, f);
    widget->button_box()->inset(x,y,w,h);
    if (h>10) FL_THIN_UP_BOX->draw(x+1, y+h/2-1, w-2, 2, bc, f);
    if (h>18) {
      FL_THIN_UP_BOX->draw(x+1, y+h/2-1-4, w-2, 2, bc, f);
      FL_THIN_UP_BOX->draw(x+1, y+h/2-1+4, w-2, 2, bc, f);
    }
  }
}

static void choice_glyph(const Fl_Widget* widget, int,
                         int x,int y,int w, int h, Fl_Flags f)
{
  FL_FLAT_BOX->draw(x,y,w,h,widget->color(),f);
  int H = h/3;
  int Y = y + (h-H)/2;
  widget->button_box()->draw(x,Y,w,H,widget->button_color(),f);
}

static void light_glyph(const Fl_Widget* widget, int,
                        int x,int y,int w, int h, Fl_Flags f)
{
  Fl_Color fc = fl_inactive(widget->text_color(), f);
  Fl_Color bc = widget->color();
  FL_DOWN_BOX->draw(x+2, y, w-4, h, bc, f&(~FL_VALUE));
  FL_THIN_UP_BOX->draw(x+4, y+2, w-8, h-4, f&FL_VALUE ? fc : bc, f&(~FL_VALUE));
}

static void return_glyph(const Fl_Widget*, int,
                         int x,int y,int w,int h, Fl_Flags f)
{
  int size = w; if (h<size) size = h;
  int d = (size+2)/4; if (d<3) d = 3;
  int t = (size+9)/12; if (t<1) t = 1;
  int x0 = x+(w-2*d-2*t-1)/2;
  int x1 = x0+d;
  int y0 = y+h/2;
  fl_color(fl_inactive(FL_DARK3, f));
  fl_line(x0, y0, x1, y0+d);
  fl_line(x1, y0+d, x1, y0+t);
  fl_line(x1, y0+t, x1+d+2*t, y0+t);
  fl_line(x1+d+2*t, y0+t, x1+d+2*t, y0-d);
  fl_line(x1, y0-t, x1, y0-d);
  fl_color(fl_inactive(FL_LIGHT2, f));
  fl_line(x0, y0, x1, y0-d);
  fl_line(x1+1, y0-t, x1+d, y0-t);
  fl_line(x1+d, y0-t, x1+d, y0-d);
  fl_line(x1+d, y0-d, x1+d+2*t, y0-d);
}

extern "C" bool fltk_theme() {
  //Fl_Style::draw_sliders_pushed = 1;

  Fl_Widget::default_style->glyph = alt_glyph;

  Fl_Style* s;

  if ((s = Fl_Style::find("menu bar"))) {
    s->button_box = FL_HIGHLIGHT_BOX;
  }

  if ((s = Fl_Style::find("item"))) {
    s->glyph = check_glyph;
    s->button_box = FL_NO_BOX;
  }

  if ((s = Fl_Style::find("menu button"))) {
    s->glyph = alt_glyph;
  }

  if ((s = Fl_Style::find("choice"))) {
    s->box = FL_NORMAL_BOX;
    s->glyph = choice_glyph;
  }

  if ((s = Fl_Style::find("check button"))) {
    s->glyph = check_glyph;
  }

  if ((s = Fl_Style::find("return button"))) {
    s->glyph = return_glyph;
  }

  if ((s = Fl_Style::find("light button"))) {
    s->glyph = light_glyph;
  }

  if ((s = Fl_Style::find("scrollbar"))) {
    scrollbarstyle = s;
    s->glyph = slider_glyph;
  }

  if ((s = Fl_Style::find("slider"))) {
    s->glyph = slider_glyph;
  }

  if ((s = Fl_Style::find("value slider"))) {
    s->glyph = slider_glyph;
  }

  if ((s = Fl_Style::find("input browser"))) {
    s->glyph = alt_glyph;
  }

  return true;
}

//
// End of "$Id: Alternative.cxx,v 1.40 2002/02/10 22:57:50 spitzak Exp $".
//
