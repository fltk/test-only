// Default glyph function used by all the built-in styles.  This draws
// somewhat Windows-style things.

#include <FL/Fl_Style.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Bitmap.H>
#include "fastarrow.h"
static Fl_Bitmap fastarrow(fastarrow_bits, fastarrow_width, fastarrow_height);
#include "mediumarrow.h"
static Fl_Bitmap mediumarrow(mediumarrow_bits, mediumarrow_width, mediumarrow_height);
#include "slowarrow.h"
static Fl_Bitmap slowarrow(slowarrow_bits, slowarrow_width, slowarrow_height);

void fl_glyph(int t, int x,int y,int w,int h,
	      Fl_Color bc, Fl_Color fc, Fl_Flags f, Fl_Boxtype box)
{
  if (f&FL_INACTIVE) fc = fl_inactive(fc);

  switch (t) {
  case FL_GLYPH_CHECK:
    box->draw(x,y,w,h, bc, f);
    if (f & FL_VALUE) {
      fl_color(fc);
#if 0 // Draws an X
      x += box->dx();
      y += box->dy();
      int r = x+h-box->dh()-1;
      int b = y+h-box->dh()-1;
      fl_line(x+1,y+1,r-1,b-1);
      fl_line(x,y+1,r-2,b-1);
      fl_line(x,b-1,r-2,y+1);
      fl_line(x+1,b-1,r-1,y+1);
#else // draw Windows check:
      x += box->dx()+1;
      w = h - box->dh() - 2;
      int d1 = w/3;
      int d2 = w-d1;
      y = y+(h+d2)/2-d1-2;
      fl_line(x, y, x+d1, y+d1, x+w-1, y+d1-d2+1);
      y++;
      fl_line(x, y, x+d1, y+d1, x+w-1, y+d1-d2+1);
      y++;
      fl_line(x, y, x+d1, y+d1, x+w-1, y+d1-d2+1);
#endif
    }
    break;
  case FL_GLYPH_RADIO:
    //h = (h+1)&-2; // even only
    if (box != FL_NO_BOX) FL_ROUND_DOWN_BOX->draw(x,y,h,h, bc, f);
    if (f & FL_VALUE) {
      fl_color(fc);
      int d = h/4; // box != FL_NO_BOX ? h/4 : 0; //h/5;
      fl_pie(x+d,y+d,h-d-d-1,h-d-d-1,0,360);
    }
    break;
  case FL_GLYPH_LIGHT: {
    box->draw(x,y,w,h, fc, f);
    break;}
  case FL_GLYPH_UP: {
    box->draw(x,y,w,h, bc, f);
    x += 2; y += 2; w -= 4; h -= 4;
    int w1 = (w-1)|1; // use odd sizes only
    int X1 = x+w1/2;
    int W1 = w1/3;
    int Y1 = y+w1/2+W1/2;
    fl_color(fc);
    fl_polygon(X1, Y1-W1, X1+W1, Y1, X1-W1, Y1);
    break;}
  case FL_GLYPH_DOWN: {
    box->draw(x,y,w,h, bc, f);
    x += 2; y += 2; w -= 4; h -= 4;
    int w1 = (w-1)|1; // use odd sizes only
    int X1 = x+w1/2;
    int W1 = w1/3;
    int Y1 = y+h-w1/2-W1/2-1;
    fl_color(fc);
    fl_polygon(X1, Y1+W1, X1-W1, Y1, X1+W1, Y1);
    break;}
  case FL_GLYPH_LEFT: {
    box->draw(x,y,w,h, bc, f);
    x += 2; y += 2; w -= 4; h -= 4;
    int w1 = (h-1)|1; // use odd sizes only
    int Y1 = y+w1/2;
    int W1 = w1/3;
    int X1 = x+w1/2+W1/2;
    fl_color(fc);
    fl_polygon(X1-W1, Y1, X1, Y1-W1, X1, Y1+W1);
    break;}
  case FL_GLYPH_RIGHT: {
    box->draw(x,y,w,h, bc, f);
    x += 2; y += 2; w -= 4; h -= 4;
    int w1 = (h-1)|1; // use odd sizes only
    int Y1 = y+w1/2;
    int W1 = w1/3;
    int X1 = x+w-w1/2-W1/2-1;
    fl_color(fc);
    fl_polygon(X1+W1, Y1, X1, Y1+W1, X1, Y1-W1);
    break;}
  case FL_GLYPH_FASTARROW: {
    box->draw(x,y,w,h, bc, f);
    fl_color(fc);
    fastarrow.draw(x, y, w, h, FL_ALIGN_CENTER);
    break;}
  case FL_GLYPH_MEDIUMARROW: {
    box->draw(x,y,w,h, bc, f);
    fl_color(fc);
    mediumarrow.draw(x, y, w, h, FL_ALIGN_CENTER);
    break;}
  case FL_GLYPH_SLOWARROW: {
    box->draw(x,y,w,h, bc, f);
    fl_color(fc);
    slowarrow.draw(x, y, w, h, FL_ALIGN_CENTER);
    break;}
  case FL_GLYPH_VNSLIDER: {
    box->draw(x,y,w,h, bc, f);
    int d = (h-4)/2;
    FL_THIN_DOWN_BOX->draw(x+2, y+d, w-4, h-2*d, fc);
    break;}
  case FL_GLYPH_HNSLIDER: {
    box->draw(x,y,w,h, bc, f);
    int d = (w-4)/2;
    FL_THIN_DOWN_BOX->draw(x+d, y+2, w-2*d, h-4, fc);
    break;}

  default:
    box->draw(x,y,w,h, bc, f);
    break;
  }
}
