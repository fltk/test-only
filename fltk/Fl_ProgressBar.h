#include <fltk/Fl_Box.h>
#include <fltk/Fl.h>
#include <fltk/fl_draw.h>

#ifndef FL_API
#if (FL_MAJOR_VERSION <= 1)
#define FL_API   FL_EXPORT
#endif
#endif

class Fl_ProgressBar : public Fl_Box
{
protected:
	double mMin;
	double mMax;
	double mPresent;
	double mStep;
	bool mShowPct;
	Fl_Color mTextColor;
	FL_API void draw();
public:
	FL_API Fl_ProgressBar(int x, int y, int w, int h, const char *lbl);
	void range(double min, double max, double step = 1)  { mMin = min; mMax = max; mStep = step; };
	void step(double step)		{ mPresent += step; redraw(); };
	double minimum()	{ return mMin; }
	double maximum()	{ return mMax; }
	void minimum(double nm) { mMin = nm; };
	void maximum(double nm) { mMax = nm; };
	double position()	{ return mPresent; }
	double step()		{ return mStep; }
	void position(double pos) 	{ mPresent = pos; redraw(); }
	void showtext(bool st)	{ mShowPct = st; }
	bool showtext()		{ return mShowPct; }
	void textcolor(Fl_Color col)	{ mTextColor = col; }
	Fl_Color textcolor() 	{ return mTextColor; }
};


