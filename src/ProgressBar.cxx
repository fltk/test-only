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
  if (damage() & DAMAGE_ALL) draw_box();
  Rectangle r(w(),h());
  box()->inset(r);
  if (mPresent > mMax) mPresent = mMax;
  if (mPresent < mMin) mPresent = mMin;
  double pct = (mPresent - mMin) / mMax;
  r.w(int(r.w()*pct+.5));
  setcolor(selection_color());
  fillrect(r);
  if (mShowPct) {
    char buffer[30];
    sprintf(buffer, "%d%%", int(pct * 100+.5));
    setcolor(textcolor());
    setfont(this->labelfont(), this->labelsize());
    drawtext(buffer, Rectangle(w(),h()), ALIGN_CENTER);
  }
}

