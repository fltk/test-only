//
// "$Id: Fl_Chart.H,v 1.6.2.3.2.4.2.4 2004/03/18 08:00:30 matthiaswm Exp $"
//
// Forms chart header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef Fl_Chart_H
#define Fl_Chart_H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif

// values for type()
#define FL_BAR_CHART		0
#define FL_HORBAR_CHART		1
#define FL_LINE_CHART		2
#define FL_FILL_CHART		3
#define FL_SPIKE_CHART		4
#define FL_PIE_CHART		5
#define FL_SPECIALPIE_CHART	6

#define FL_FILLED_CHART  FL_FILL_CHART	// compatibility

#define FL_CHART_MAX		128
#define FL_CHART_LABEL_MAX	18

struct FL_CHART_ENTRY {
   float val;
   unsigned col;
   char str[FL_CHART_LABEL_MAX+1];
};

/** This widget displays simple charts and is provided for Forms 
 * compatibility. 
 *   \image html charts.gif
 * The chart type() can be one of the following:
 * \arg \c FL_BAR_CHART - Each sample value is drawn as a vertical bar.
 * \arg \c FL_FILLED_CHART - The chart is filled from the bottom of the 
 *      graph to the sample values.
 * \arg \c FL_HORBAR_CHART - Each sample value is drawn as a horizontal bar.
 * \arg \c FL_LINE_CHART - The chart is drawn as a polyline with vertices 
 *      at each sample value.
 * \arg \c FL_PIE_CHART - A pie chart is drawn with each sample value being
 *      drawn as a proportionate slice in the circle.
 * \arg \c FL_SPECIALPIE_CHART - Like \c FL_PIE_CHART, but the first slice 
 *      is separated from the pie.
 * \arg \c FL_SPIKE_CHART - Each sample value is drawn as a vertical line.
 */
class FL_EXPORT Fl_Chart : public Fl_Widget {
    int numb;
    int maxnumb;
    int sizenumb;
    FL_CHART_ENTRY *entries;
    double min,max;
    uchar autosize_;
    uchar textfont_,textsize_;
    unsigned textcolor_;
protected:
    void draw();
public:
      /** Creates a new Fl_Chart widget using the given position, size, and
       * label string. The default boxtype is \c FL_NO_BOX. */
    Fl_Chart(int x,int y,int width,int height,const char *label = 0);
      /** Destroys the Fl_Chart widget and all of its data. */
    ~Fl_Chart();
      /** The clear method removes all values from the chart. */
    void clear();
      /** The add method adds the value and optionally label and color to the chart. */
    void add(double value, const char *label=0, unsigned color=0);
      /** The insert method inserts a data value at the given position pos. 
       * Position 1 is the first data value. */
    void insert(int position, double value, const char *label=0, unsigned color=0);
      /** The replace method replaces data value pos with value, label, and
       * color. Position 1 is the first data value. */
    void replace(int position, double value, const char *label=0, unsigned color=0);
    void bounds(double *a,double *b) const {*a = min; *b = max;}
      /** The bounds method sets the lower and upper bounds of the chart 
       * values to a andb respectively. */
    void bounds(double a,double b);
      /** The size method returns the number of data values in the chart. */
    int size() const {return numb;}
    void size(int W, int H) { Fl_Widget::size(W, H); }
    int maxsize() const {return maxnumb;}
      /** The maxsize method gets the maximum number of data values for a 
       * chart. If you do not call this method then the chart will be 
       * allowed to grow to any size depending on available memory. */
    void maxsize(int n);
    Fl_Font textfont() const {return (Fl_Font)textfont_;}
    void textfont(uchar s) {textfont_ = s;}
    uchar textsize() const {return textsize_;}
    void textsize(uchar s) {textsize_ = s;}
    Fl_Color textcolor() const {return (Fl_Color)textcolor_;}
    void textcolor(unsigned n) {textcolor_ = n;}
      /** The autosize method controls whether or not the chart will 
       * automatically adjust the bounds of the chart. It returns a boolean
       * value that is non-zero if auto-sizing is enabled and zero is 
       * auto-sizing is disabled. */
    uchar autosize() const {return autosize_;}
      /** Sets the auto-sizing property to onoff. */
    void autosize(uchar n) {autosize_ = n;}
};

#endif

//
// End of "$Id: Fl_Chart.H,v 1.6.2.3.2.4.2.4 2004/03/18 08:00:30 matthiaswm Exp $".
//
