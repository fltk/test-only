#include <fltk/ProgressBar.h>
#include <fltk/damage.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <stdio.h>
using namespace fltk;

ProgressBar::ProgressBar(int x, int y, int w, int h, const char *lbl)
  : Widget(x,y,w,h,lbl)
{
  mMin = mPresent = 0; 
  mMax = 100; 
  mShowPct = true; 
  box(DOWN_BOX);
  selection_color(BLUE);
  color(WHITE);
  textcolor(RED);
}

void ProgressBar::draw() {
  int bdx, bdy;
  double pct;
  if (damage() & DAMAGE_ALL) draw_box();
  int X = 0, Y = 0;
#if (MAJOR_VERSION > 1)
  bdx = box()->dx();
  bdy = box()->dy();
#else
  X = x();
  Y = y();
  bdx = box()->dx();
  bdy = box()->dy();
#endif
  setcolor(selection_color());
  if(mPresent > mMax)
    mPresent = mMax;
  if(mPresent < mMin)
    mPresent = mMin;
  pct = (mPresent - mMin) / mMax;
  fillrect(bdx, bdy, (int)(((double)w() - 2*bdx) * pct), h() - (2*bdy + 1));
  if(mShowPct) {
    char buffer[30];
    sprintf(buffer, "%d%%", (int) (pct * 100));
    setcolor(textcolor());
    setfont(this->labelfont(), this->labelsize());
    drawtext(buffer,
	     (w() - getwidth(buffer))/2,
	     labelsize() + (((h() - 2*bdy) - labelsize())/2));
  }
}

