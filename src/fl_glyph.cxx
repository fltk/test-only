// Default glyph function used by all the built-in styles.  This draws
// somewhat Windoze-style things.

#include <FL/Fl_Style.H>
#include <FL/fl_draw.H>

void fl_glyph(int t, int x,int y,int w,int h,
	      Fl_Color c, Fl_Flags f)
{
  Fl_Color l = (f&FL_INACTIVE) ? Fl_Color(FL_INACTIVE_COLOR)
    : fl_contrast(FL_NO_COLOR, c);
  switch (t) {
  case FL_GLYPH_CHECK:
    FL_DOWN_BOX->draw(x,y,w,h, c, f);
    if (f & FL_VALUE) {
      fl_color(l);
#if 0 // Draws an X
      x += FL_DOWN_BOX->dx();
      y += FL_DOWN_BOX->dy();
      int r = x+h-FL_DOWN_BOX->dh()-1;
      int b = y+h-FL_DOWN_BOX->dh()-1;
      fl_line(x+1,y+1,r-1,b-1);
      fl_line(x,y+1,r-2,b-1);
      fl_line(x,b-1,r-2,y+1);
      fl_line(x+1,b-1,r-1,y+1);
#else // draw Windoze check:
      x += FL_DOWN_BOX->dx()+1;
      w = h - FL_DOWN_BOX->dh() - 2;
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
    FL_ROUND_DOWN_BOX->draw(x,y,h,h, c, f);
    if (f & FL_VALUE) {
      fl_color(l);
      int d = h/5;
      fl_pie(x+d,y+d,h-d-d-1,h-d-d-1,0,360);
    }
    break;
  case FL_GLYPH_UP: {
    FL_NORMAL_BOX->draw(x,y,w,h, c, f);
    int w1 = (w-1)|1; // use odd sizes only
    int X1 = x+w1/2;
    int W1 = w1/3;
    int Y1 = y+w1/2+W1/2;
    fl_color(l);
    fl_polygon(X1, Y1-W1, X1+W1, Y1, X1-W1, Y1);
    break;}
  case FL_GLYPH_DOWN: {
    FL_NORMAL_BOX->draw(x,y,w,h, c, f);
    int w1 = (w-1)|1; // use odd sizes only
    int X1 = x+w1/2;
    int W1 = w1/3;
    int Y1 = y+h-w1/2-W1/2-1;
    fl_color(l);
    fl_polygon(X1, Y1+W1, X1-W1, Y1, X1+W1, Y1);
    break;}
  case FL_GLYPH_LEFT: {
    FL_NORMAL_BOX->draw(x,y,w,h, c, f);
    int w1 = (h-1)|1; // use odd sizes only
    int Y1 = y+w1/2;
    int W1 = w1/3;
    int X1 = x+w1/2+W1/2;
    fl_color(l);
    fl_polygon(X1-W1, Y1, X1, Y1-W1, X1, Y1+W1);
    break;}
  case FL_GLYPH_RIGHT: {
    FL_NORMAL_BOX->draw(x,y,w,h, c, f);
    int w1 = (h-1)|1; // use odd sizes only
    int Y1 = y+w1/2;
    int W1 = w1/3;
    int X1 = x+w-w1/2-W1/2-1;
    fl_color(l);
    fl_polygon(X1+W1, Y1, X1, Y1+W1, X1, Y1-W1);
    break;}
  default:
    FL_NORMAL_BOX->draw(x,y,w,h, c, f);
    break;
  }
}
