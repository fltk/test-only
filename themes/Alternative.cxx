//
// "$Id: Alternative.cxx,v 1.19 2000/01/10 06:31:30 bill Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Boxtype.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>

// a couple of of new boxtypes (look familiar?)
static const Fl_Frame_Box
alt_thick_down_box("alternative thick down", "NNUUJJXXAAAA");

static const Fl_Frame_Box
alt_thick_up_box("alternative thick up", "AAAAXXJJUUNN", &alt_thick_down_box);

// some old stuff for boxtype drawing
enum {UPPER_LEFT, LOWER_RIGHT, CLOSED, FILL};

static void draw(int which, int x,int y,int w,int h, int inset, Fl_Color color)
{
  if (inset*2 >= w) inset = (w-1)/2;
  if (inset*2 >= h) inset = (h-1)/2;
  x += inset;
  y += inset;
  w -= 2*inset;
  h -= 2*inset;
  int d = w <= h ? w : h;
  if (d <= 1) return;
  fl_color(color);

  switch (which) {
    case UPPER_LEFT :
      fl_arc(x+w-d, y, d, d, 45, w<=h ? 180 : 90);
      fl_arc(x, y+h-d, d, d, w<=h ? 180 : 90, 225);
      break;
    case LOWER_RIGHT :
      fl_arc(x, y+h-d, d, d, 225, w<=h ? 360 : 270);
      fl_arc(x+w-d, y, d, d, w<=h ? 360 : 270, 360+45);
      break;
    case CLOSED :
      fl_arc(x+w-d, y, d, d, w<=h ? 0 : -90, w<=h ? 180 : 90);
      fl_arc(x, y+h-d, d, d, w<=h ? 180 : 90, w<=h ? 360 : 270);
      break;
    case FILL :
      fl_pie(x+w-d, y, d, d, w<=h ? 0 : -90, w<=h ? 180 : 90);
      fl_pie(x, y+h-d, d, d, w<=h ? 180 : 90, w<=h ? 360 : 270);
      break;
  }

  if (which == FILL) {
    if (w < h)
      fl_rectf(x, y+d/2, w, h-(d&-2));
    else if (w > h)
      fl_rectf(x+d/2, y, w-(d&-2), h);
  } else {
    if (w < h) {
      if (which != UPPER_LEFT) fl_yxline(x+w-1, y+d/2, y+h-d/2);
      if (which != LOWER_RIGHT) fl_yxline(x, y+d/2, y+h-d/2);
    } else if (w > h) {
      if (which != UPPER_LEFT) fl_xyline(x+d/2, y+h-1, x+w-d/2);
      if (which != LOWER_RIGHT) fl_xyline(x+d/2, y, x+w-d/2);
    }
  }
}

// a new glyph function
static void
alt_glyph(int t, int x, int y, int w, int h, Fl_Color bc, Fl_Color fc,
          Fl_Flags f, Fl_Boxtype box)
{
  switch (t) {
    case FL_GLYPH_CHECK: {
      if (box == FL_NO_BOX) {
        fl_glyph(t, x, y, w, h, bc, fc, f, box); break; }
      w = (w-1)|1; h = (h-1)|1;
      int x1 = x+w/2;
      int y1 = y+h/2;
      Fl_Color light = 54, dark = 32;

      if (f&FL_INACTIVE)
        { fc = fl_inactive(fc); light = fl_inactive(light); dark = fl_inactive(dark); }
      fl_color((f&FL_VALUE) ? fc : bc); fl_polygon(x+3,y1, x1,y+3, x+w-4,y1, x1,y+h-4);

      fl_color(dark);
      fl_line(x,   y1, x1, y,   x+w-1, y1);
      fl_line(x+1, y1, x1, y+1, x+w-2, y1);
      fl_color(light);
      fl_line(x+2, y1, x1, y+2, x+w-3, y1);
      fl_line(x+3, y1, x1, y+3, x+w-4, y1);

      fl_color(light);
      fl_line(x,   y1, x1, y+h-1, x+w-1, y1);
      fl_line(x+1, y1, x1, y+h-2, x+w-2, y1);
      fl_color(dark);
      fl_line(x+2, y1, x1, y+h-3, x+w-3, y1);
      fl_line(x+3, y1, x1, y+h-4, x+w-4, y1);
      break;
    }
    case FL_GLYPH_RADIO: {
      if (box == FL_NO_BOX)
        { fl_glyph(t, x, y, w, h, bc, fc, f, box); break; }
      Fl_Color light = 54, dark = 32;

      if (f&FL_INACTIVE)
        { fc = fl_inactive(fc); light = fl_inactive(light); dark = fl_inactive(dark); }
      draw(FILL, x+2, y+2, w-4, h-4, 0, (f&FL_VALUE) ? fc : bc);

//      draw(UPPER_LEFT, x+1, y, w-2, h, 0, dark);
      draw(UPPER_LEFT, x, y, w, h, 0, dark);
      draw(UPPER_LEFT, x+1, y+1, w-2, h-2, 0, dark);
      draw(UPPER_LEFT, x+2, y+2, w-4, h-4, 0, light);

//      draw(LOWER_RIGHT, x+1, y, w-2, h, 0, light);
      draw(LOWER_RIGHT, x, y, w, h, 0, light);
      draw(LOWER_RIGHT, x+1, y+1, w-2, h-2, 0, light);
      draw(LOWER_RIGHT, x+2, y+2, w-4, h-4, 0, dark);

      break;
    }
    case FL_GLYPH_HSLIDER: {
      box->draw(x,y,w,h, bc, f);
      box->inset(x,y,w,h);
      if (w>10) FL_THIN_UP_BOX->draw(x+w/2-1, y+1, 2, h-2, fc, f);
      if (w>18) {
        FL_THIN_UP_BOX->draw(x+w/2-1-4, y+1, 2, h-2, fc, f);
        FL_THIN_UP_BOX->draw(x+w/2-1+4, y+1, 2, h-2, fc, f);
      }
      break;
    }
    case FL_GLYPH_VSLIDER: {
      box->draw(x,y,w,h, bc, f);
      box->inset(x,y,w,h);
      if (h>10) FL_THIN_UP_BOX->draw(x+1, y+h/2-1, w-2, 2, fc, f);
      if (h>18) {
        FL_THIN_UP_BOX->draw(x+1, y+h/2-1-4, w-2, 2, fc, f);
        FL_THIN_UP_BOX->draw(x+1, y+h/2-1+4, w-2, 2, fc, f);
      }
      break;
    }
    case FL_GLYPH_RIGHT:
    case FL_GLYPH_LEFT:
    case FL_GLYPH_UP:
    case FL_GLYPH_DOWN: {
      if (box == FL_NO_BOX) { x += 2; y += 2; w -= 4; h -= 4; } // menu fudge factor
      Fl_Color d1, d2, l1, l2;
      if (f&FL_VALUE) {
        d1 = FL_LIGHT3; d2 = FL_LIGHT1; l1 = FL_BLACK; l2 = FL_DARK2;
      } else{
        l1 = FL_LIGHT3; l2 = FL_LIGHT1; d1 = FL_BLACK; d2 = FL_DARK2;
      }

      if (f&FL_INACTIVE) {
        l1 = fl_inactive(l1); l2 = fl_inactive(l2);
        d1 = fl_inactive(d1); d2 = fl_inactive(d2);
      }

      if (t == FL_GLYPH_RIGHT) {
        fl_color(bc); fl_polygon(x,y, x+w-1,y+h/2, x,y+h-1);
        fl_color(l2); fl_line(x+1,y+h-2, x+1,y+1, x+w-2,y+h/2);
        fl_color(d2); fl_line(x+1,y+h-2, x+w-2,y+h/2);
        fl_color(l1); fl_line(x,y+h-1, x,y, x+w-1,y+h/2);
        fl_color(d1); fl_line(x,y+h-1, x+w-1,y+h/2);
      } else if (t == FL_GLYPH_LEFT) {
        fl_color(bc); fl_polygon(x+w-1,y, x+w-1,y+h-1, x,y+h/2);
        fl_color(d2); fl_line(x+w-2,y+1, x+w-2,y+h-2, x+1,y+h/2);
        fl_color(l2); fl_line(x+w-2,y+1, x+1,y+h/2);
        fl_color(d1); fl_line(x+w-1,y, x+w-1,y+h-1, x,y+h/2);
        fl_color(l1); fl_line(x+w-1,y, x,y+h/2);
      } else if (t == FL_GLYPH_DOWN) {
        fl_color(bc); fl_polygon(x,y, x+w/2,y+h-1, x+w-1,y);
        fl_color(l2); fl_line(x+w-2,y+1, x+1,y+1, x+w/2,y+h-2);
        fl_color(d2); fl_line(x+w-2,y+1, x+w/2,y+h-2);
        fl_color(l1); fl_line(x+w-1,y, x,y, x+w/2,y+h-1);
        fl_color(d1); fl_line(x+w-1,y, x+w/2,y+h-1);
      } else { // UP
        fl_color(bc); fl_polygon(x,y+h-1, x+w-1,y+h-1, x+w/2,y);
        fl_color(d2); fl_line(x+1,y+h-2, x+w-2,y+h-2, x+w/2,y+1);
        fl_color(l2); fl_line(x+1,y+h-2, x+w/2,y+1);
        fl_color(d1); fl_line(x,y+h-1, x+w-1,y+h-1, x+w/2,y);
        fl_color(l1); fl_line(x,y+h-1, x+w/2,y);
      }
      break;
    }
    case FL_GLYPH_VNSLIDER: {
      box->draw(x,y,w,h, bc, f);
      int d = (h-4)/2;
      FL_THIN_UP_BOX->draw(x+2, y+d, w-4, h-2*d, fc);
      break;
    }
    case FL_GLYPH_HNSLIDER: {
      box->draw(x,y,w,h, bc, f);
      int d = (w-4)/2;
      FL_THIN_UP_BOX->draw(x+d, y+2, w-2*d, h-4, fc);
      break;
    }
    default:
      box->draw(x,y,w,h, bc, f);
  }
}

static void choice_glyph(int/*t*/, int x,int y,int w,int h, Fl_Color bc, Fl_Color,
		  Fl_Flags f, Fl_Boxtype box)
{
  int H = h/3;
  int Y = y + (h-H)/2;
  box->draw(x,Y,w,H, bc, f);
}

static void light_glyph(int/*t*/, int x,int y,int w,int h, Fl_Color, Fl_Color fc,
		  Fl_Flags f, Fl_Boxtype)
{
  FL_DOWN_FRAME->draw(x, y, w, h, fc, f & ~FL_VALUE);
  FL_THIN_UP_BOX->draw(x+2, y+2, w-4, h-4, fc, f & ~FL_VALUE);
}

static void return_glyph(int/*t*/, int x,int y,int w,int h, Fl_Color, Fl_Color,
		  Fl_Flags f, Fl_Boxtype)
{
  int size = w; if (h<size) size = h;
  int d = (size+2)/4; if (d<3) d = 3;
  int t = (size+9)/12; if (t<1) t = 1;
  int x0 = x+(w-2*d-2*t-1)/2;
  int x1 = x0+d;
  int y0 = y+h/2;
  fl_color(fl_inactive(FL_DARK3, f));
  fl_line(x0, y0, x1, y0+d);
  fl_yxline(x1, y0+d, y0+t, x1+d+2*t, y0-d);
  fl_yxline(x1, y0-t, y0-d);
  fl_color(fl_inactive(FL_LIGHT2, f));
  fl_line(x0, y0, x1, y0-d);
  fl_xyline(x1+1,y0-t,x1+d,y0-d,x1+d+2*t);
}

extern "C"
int fltk_theme(int, char**) {

  Fl_Style* s;
  if ((s = Fl_Style::find("menu title"))) {
    s->box = FL_HIGHLIGHT_BOX;
    s->glyph = alt_glyph;
    s->glyph_box = FL_NO_BOX;
  }

  if ((s = Fl_Style::find("menu item"))) {
    s->glyph = alt_glyph;
    s->glyph_box = FL_NO_BOX;
  }

  if ((s = Fl_Style::find("menu button"))) {
    s->glyph = alt_glyph;
  }

  if ((s = Fl_Style::find("choice"))) {
    s->glyph = choice_glyph;
    s->glyph_box = FL_UP_BOX;
  }

  if ((s = Fl_Style::find("check button"))) {
    s->glyph = alt_glyph;
  }

  if ((s = Fl_Style::find("return button"))) {
    s->glyph = return_glyph;
  }

  if ((s = Fl_Style::find("light button"))) {
    s->glyph = light_glyph;
  }

  if ((s = Fl_Style::find("scrollbar"))) {
    s->glyph = alt_glyph;
  }

  if ((s = Fl_Style::find("slider"))) {
    s->glyph = alt_glyph;
  }

  if ((s = Fl_Style::find("value slider"))) {
    s->glyph = alt_glyph;
  }
  return 0;
}

//
// End of "$Id: Alternative.cxx,v 1.19 2000/01/10 06:31:30 bill Exp $".
//
