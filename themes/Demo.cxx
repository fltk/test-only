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
const Fl_Boxtype_ demo_down_box = {
  fl_frame, "NNTUJJUWAAAA", &demo_down_box, &demo_down_box, 3,3,6,6, true
};

const Fl_Boxtype_ demo_up_box = {
  fl_frame, "AAAAWUJJUTNN", &demo_down_box, &demo_up_box, 3,3,6,6, true
};

const Fl_Boxtype_ demo_menu_box = {
  fl_flatx, 0, FL_THIN_DOWN_BOX, FL_THIN_DOWN_BOX, 1,1,2,2, true
};

const Fl_Boxtype_ demo_menu_title_box = {
  fl_flatx, 0, FL_THIN_DOWN_BOX, FL_NORMAL_BOX, 1,1,2,2, true
};

// some old stuff for boxtype drawing
static Fl_Color active_ramp[24] = {
  FL_GRAY_RAMP+0, FL_GRAY_RAMP+1, FL_GRAY_RAMP+2, FL_GRAY_RAMP+3,
  FL_GRAY_RAMP+4, FL_GRAY_RAMP+5, FL_GRAY_RAMP+6, FL_GRAY_RAMP+7,
  FL_GRAY_RAMP+8, FL_GRAY_RAMP+9, FL_GRAY_RAMP+10,FL_GRAY_RAMP+11,
  FL_GRAY_RAMP+12,FL_GRAY_RAMP+13,FL_GRAY_RAMP+14,FL_GRAY_RAMP+15,
  FL_GRAY_RAMP+16,FL_GRAY_RAMP+17,FL_GRAY_RAMP+18,FL_GRAY_RAMP+19,
  FL_GRAY_RAMP+20,FL_GRAY_RAMP+21,FL_GRAY_RAMP+22,FL_GRAY_RAMP+23};
static Fl_Color inactive_ramp[24];
static Fl_Color *gray_ramp(int inactive) {
  if (!inactive) return active_ramp-'A';
  for (int i = 0; i < 24; i++) inactive_ramp[i] = fl_inactive(fl_get_color(active_ramp[i]));
  return inactive_ramp-'A';
}

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
  void (*f)(int,int,int,int,double,double);
  f = (which==FILL) ? fl_pie : fl_arc;
  if (which >= CLOSED) {
    f(x+w-d, y, d, d, w<=h ? 0 : -90, w<=h ? 180 : 90);
    f(x, y+h-d, d, d, w<=h ? 180 : 90, w<=h ? 360 : 270);
  } else if (which == UPPER_LEFT) {
    f(x+w-d, y, d, d, 45, w<=h ? 180 : 90);
    f(x, y+h-d, d, d, w<=h ? 180 : 90, 225);
  } else { // LOWER_RIGHT
    f(x, y+h-d, d, d, 225, w<=h ? 360 : 270);
    f(x+w-d, y, d, d, w<=h ? 360 : 270, 360+45);
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
void demo_glyph(int t, int x, int y, int w, int h, Fl_Color bc, Fl_Color fc,
                Fl_Flags f, Fl_Boxtype box)
{
  switch (t) {
    case FL_GLYPH_LIGHT:
      FL_DOWN_FRAME->draw(x, y, w, h, fc, f);
      FL_THIN_UP_BOX->draw(x+2, y+2, w-4, h-4, fc, f);
      break;
    case FL_GLYPH_CHECK: {
      w &= -2;
      h &= -2;
      int x1 = x+w/2;
      int y1 = y+h/2;
      Fl_Color *g = gray_ramp(f&FL_INACTIVE);

      if (f&FL_INACTIVE) fc = fl_inactive(fc);
      fl_color((f&FL_VALUE) ? fc : bc); fl_polygon(x+2, y1, x1,y+2, x+w-4,y1, x1,y+h-4);

      fl_color(g['A']); fl_line(x, y1, x1, y, x+w, y1);
      fl_color(g['A']); fl_line(x+1, y1, x1, y+1, x+w-1, y1);
      fl_color(g['W']); fl_line(x+3, y1, x1, y+3, x+w-3, y1);

      fl_color(g['W']); fl_line(x, y1, x1, y+h, x+w, y1);
      fl_color(g['W']); fl_line(x+1, y1, x1, y+h-1, x+w-1, y1);
      fl_color(g['A']); fl_line(x+3, y1, x1, y+h-3, x+w-3, y1);
      break;
    }
    case FL_GLYPH_RADIO: {
      Fl_Color *g = gray_ramp(f&FL_INACTIVE);

      if (f&FL_INACTIVE) fc = fl_inactive(fc);
      draw(FILL, x+2, y+2, w-4, h-4, 0, (f&FL_VALUE) ? fc : bc);

      draw(UPPER_LEFT, x+1, y, w-2, h, 0, g['A']);
      draw(UPPER_LEFT, x, y, w, h, 0, g['A']);
      draw(UPPER_LEFT, x+1, y+1, w-2, h-2, 0, g['A']);
      draw(UPPER_LEFT, x+2, y+2, w-4, h-4, 0, g['W']);

      draw(LOWER_RIGHT, x+1, y, w-2, h, 0, g['W']);
      draw(LOWER_RIGHT, x, y, w, h, 0, g['W']);
      draw(LOWER_RIGHT, x+1, y+1, w-2, h-2, 0, g['W']);
      draw(LOWER_RIGHT, x+2, y+2, w-4, h-4, 0, g['A']);
      break;
    }
    case FL_GLYPH_HSLIDER: {
      box->draw(x,y,w,h, bc, f);
      int dy = box->dy()+1;
      if (w>24) {
        FL_THIN_UP_BOX->draw(x+w/2-4, y+dy, 2, h-dy*2, fc, f);
        FL_THIN_UP_BOX->draw(x+w/2, y+dy, 2, h-dy*2, fc, f);
        FL_THIN_UP_BOX->draw(x+w/2+4, y+dy, 2, h-dy*2, fc, f);
      }
      break;
    }
    case FL_GLYPH_VSLIDER: {
      box->draw(x,y,w,h, bc, f);
      int dx = box->dx()+1;
      if (h>24) {
        FL_THIN_UP_BOX->draw(x+dx, y+h/2-4, w-dx*2, 2, fc, f);
        FL_THIN_UP_BOX->draw(x+dx, y+h/2, w-dx*2, 2, fc, f);
        FL_THIN_UP_BOX->draw(x+dx, y+h/2+4, w-dx*2, 2, fc, f);
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

extern "C" fltk_theme(int, char**);

int fltk_theme(int, char** argv) {
  static Fl_Boxtype_Definer demo_down("demo down", demo_down_box);
  static Fl_Boxtype_Definer demo_up("demo up", demo_up_box);
  static Fl_Boxtype_Definer demo_menu("demo menu", demo_menu_box);
  static Fl_Boxtype_Definer demo_menu_title("demo menu title", demo_menu_title_box);

  Fl_Widget::default_style.set_glyph(demo_glyph);

  return 0;
}
