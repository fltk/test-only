#include <FL/Fl_ProgressBar.H>
#include <stdio.h>

FL_API Fl_ProgressBar::Fl_ProgressBar(int x, int y, int w, int h, const char *lbl)
	: Fl_Box(x,y,w,h,lbl)
{
	mMin = mPresent = 0; 
	mMax = 100; 
	mShowPct = true; 
	box(FL_DOWN_BOX);
	selection_color(FL_BLUE);
	color(FL_WHITE);
	textcolor(FL_RED);
}					   
					   
FL_API void Fl_ProgressBar::draw()
{
	int bdx, bdy;
	double pct;
	if(damage() & FL_DAMAGE_ALL)
		draw_box();
	int X = 0, Y = 0;
#if (FL_MAJOR_VERSION > 1)
	bdx = box()->dx();
	bdy = box()->dy();
#else
	X = x();
	Y = y();
	bdx = Fl::box_dx(box());
	bdy = Fl::box_dy(box());
#endif
	fl_color(selection_color());
	if(mPresent > mMax)
		mPresent = mMax;
	if(mPresent < mMin)
		mPresent = mMin;
	pct = (mPresent - mMin) / mMax;
	fl_rectf(X + bdx, Y + bdy, (int)(((double)w() - 2*bdx) * pct), h() - (2*bdy + 1));
	if(mShowPct)
	{
		char buffer[30];
		sprintf(buffer, "%d%%", (int) (pct * 100));
		fl_color(textcolor());
#if (FL_MAJOR_VERSION > 1)
		fl_font(this->label_font(), this->label_size());
#else
		fl_font(this->labelfont(), this->labelsize());
#endif
		fl_draw(buffer, X + (w() - fl_width(buffer))/2, Y + fl_height() + (((h() - 2*bdy) - fl_height())/2));
	}
}

