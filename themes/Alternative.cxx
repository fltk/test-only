//
// "$Id: Alternative.cxx,v 1.6 1999/11/10 19:27:36 carl Exp $"
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

// a boxtype drawing function in fl_boxtype.cxx
extern void fl_frame(Fl_Boxtype b, int x, int y, int w, int h,
                     Fl_Color c, Fl_Flags f);

// a boxtype drawing function in fl_boxtype.cxx
extern void fl_flatx(Fl_Boxtype b, int x, int y, int w, int h,
                     Fl_Color c, Fl_Flags f);

// a couple of of new boxtypes (look familiar?)
static const Fl_Boxtype_ alt_down_box = {
  fl_frame, "NNUUJJXXAAAA", &alt_down_box, &alt_down_box, 3,3,6,6, 1
};

static const Fl_Boxtype_ alt_up_box = {
  fl_frame, "AAAAXXJJUUNN", &alt_down_box, &alt_up_box, 3,3,6,6, 1
};

static const Fl_Boxtype_ alt_menu_box = {
  fl_flatx, 0, FL_THIN_DOWN_BOX, FL_THIN_DOWN_BOX, 1,1,2,2, 1
};

static const Fl_Boxtype_ alt_menu_title_box = {
  fl_flatx, 0, FL_THIN_DOWN_BOX, FL_UP_BOX, 1,1,2,2, 1
};

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
static void alt_glyph(int t, int x, int y, int w, int h, Fl_Color bc, Fl_Color fc,
                Fl_Flags f, Fl_Boxtype box)
{
  switch (t) {
    case FL_GLYPH_LIGHT:
      FL_DOWN_FRAME->draw(x, y, w, h, fc, f);
      FL_THIN_UP_BOX->draw(x+2, y+2, w-4, h-4, fc, f);
      break;
    case FL_GLYPH_CHECK: {
      if (box == FL_NO_BOX) { fl_glyph(t, x, y, w, h, bc, fc, f, box); break; }
      w = (w-1)|1; h = (h-1)|1;
      int x1 = x+w/2;
      int y1 = y+h/2;
      Fl_Color light = 54, dark = 32;

      if (f&FL_INACTIVE)
        { fc = fl_inactive(fc); light = fl_inactive(light); dark = fl_inactive(dark); }
      fl_color((f&FL_VALUE) ? fc : bc); fl_polygon(x+3,y1, x1,y+3, x+w-4,y1, x1,y+h-4);

      fl_color(dark); fl_line(x, y1, x1, y, x+w-1, y1);
      fl_color(dark); fl_line(x+1, y1, x1, y+1, x+w-2, y1);
      fl_color(light); fl_line(x+2, y1, x1, y+2, x+w-3, y1);

      fl_color(light); fl_line(x, y1, x1, y+h-1, x+w-1, y1);
      fl_color(light); fl_line(x+1, y1, x1, y+h-2, x+w-2, y1);
      fl_color(dark); fl_line(x+2, y1, x1, y+h-3, x+w-3, y1);
      break;
    }
    case FL_GLYPH_RADIO: {
      if (box == FL_NO_BOX) { fl_glyph(t, x, y, w, h, bc, fc, f, box); break; }
      Fl_Color light = 54, dark = 32;

      if (f&FL_INACTIVE)
        { fc = fl_inactive(fc); light = fl_inactive(light); dark = fl_inactive(dark); }
      draw(FILL, x+2, y+2, w-4, h-4, 0, (f&FL_VALUE) ? fc : bc);

      draw(UPPER_LEFT, x+1, y, w-2, h, 0, dark);
      draw(UPPER_LEFT, x, y, w, h, 0, dark);
      draw(UPPER_LEFT, x+1, y+1, w-2, h-2, 0, dark);
      draw(UPPER_LEFT, x+2, y+2, w-4, h-4, 0, light);

      draw(LOWER_RIGHT, x+1, y, w-2, h, 0, light);
      draw(LOWER_RIGHT, x, y, w, h, 0, light);
      draw(LOWER_RIGHT, x+1, y+1, w-2, h-2, 0, light);
      draw(LOWER_RIGHT, x+2, y+2, w-4, h-4, 0, dark);
      break;
    }
    case FL_GLYPH_HSLIDER: {
      box->draw(x,y,w,h, bc, f);
      int dy = box->dy()+1;
      FL_THIN_UP_BOX->draw(x+w/2-1, y+dy, 2, h-dy*2, fc, f);
      if (w>18) {
        FL_THIN_UP_BOX->draw(x+w/2-1-4, y+dy, 2, h-dy*2, fc, f);
        FL_THIN_UP_BOX->draw(x+w/2-1+4, y+dy, 2, h-dy*2, fc, f);
      }
      break;
    }
    case FL_GLYPH_VSLIDER: {
      box->draw(x,y,w,h, bc, f);
      int dx = box->dx()+1;
      FL_THIN_UP_BOX->draw(x+dx, y+h/2-1, w-dx*2, 2, fc, f);
      if (h>18) {
        FL_THIN_UP_BOX->draw(x+dx, y+h/2-1-4, w-dx*2, 2, fc, f);
        FL_THIN_UP_BOX->draw(x+dx, y+h/2-1+4, w-dx*2, 2, fc, f);
      }
      break;
    }
    case FL_GLYPH_RIGHT:
    case FL_GLYPH_LEFT:
    case FL_GLYPH_UP:
    case FL_GLYPH_DOWN: {
        Fl_Color d1, d2, l1, l2;
        if (f&FL_VALUE) {
          d1 = FL_WHITE; d2 = FL_LIGHT2; l1 = FL_BLACK; l2 = FL_DARK2;
        } else{
          l1 = FL_WHITE; l2 = FL_LIGHT2; d1 = FL_BLACK; d2 = FL_DARK2;
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
    default:
      fl_glyph(t, x, y, w, h, bc, fc, f, box);
  }
}

extern "C" int fltk_theme(int, char**);

int fltk_theme(int, char** argv) {
  static Fl_Boxtype_Definer alt_down("alternative down", alt_down_box);
  static Fl_Boxtype_Definer alt_up("alternative up", alt_up_box);
  static Fl_Boxtype_Definer alt_menu("alternative menu", alt_menu_box);
  static Fl_Boxtype_Definer alt_menu_title("alternative menu title", alt_menu_title_box);

  Fl_Style* s;
  if ((s = Fl_Style::find("menu title"))) {
    s->set_glyph_box(FL_NO_BOX);
  }

  if ((s = Fl_Style::find("menu item"))) {
    s->set_glyph_box(FL_NO_BOX);
  }

  Fl_Widget::default_style.set_glyph(alt_glyph);

  return 0;
}

//
// End of "$Id: Alternative.cxx,v 1.6 1999/11/10 19:27:36 carl Exp $".
//
