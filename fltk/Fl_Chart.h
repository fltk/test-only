//
// "$Id: Fl_Chart.h,v 1.3 2002/10/29 00:37:23 easysw Exp $"
//
// Forms chart header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#ifndef Fl_Chart_H
#define Fl_Chart_H

#include <fltk/Fl_Widget.h>

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

struct FL_FORMS_API FL_CHART_ENTRY {
   float val;
   uchar col;
   char str[FL_CHART_LABEL_MAX+1];
};

/**
   This widget displays simple charts and is provided for Forms compatibility.
   
   \image charts.gif


   The chart type (set using type(uchar)) can be one of the following: 
   - FL_BAR_CHART  
      Each sample value is drawn as a vertical bar. 
   - FL_FILLED_CHART 
      The chart is filled from the bottom of the graph to the sample values. 
   - FL_HORBAR_CHART 
      Each sample value is drawn as a horizontal bar. 
   - FL_LINE_CHART 
      The chart is drawn as a polyline with vertices at each sample value. 
   - FL_PIE_CHART 
      A pie chart is drawn with each sample value being drawn as a proportionate 
      slice in the circle. 
   - FL_SPECIALPIE_CHART 
      Like FL_PIE_CHART, but the first slice is separated from the pie. 
   - FL_SPIKE_CHART 
      Each sample value is drawn as a vertical line. 

*/
class FL_FORMS_API Fl_Chart : public Fl_Widget {
public:
  /**
   Creates a new Fl_Chart widget using the given position, size, 
   and label string. The default boxtype is FL_NO_BOX. 
  */
  Fl_Chart(int x,int y,int w,int h,const char * label= 0);
  /** Destroys the Fl_Chart widget and all of its data. */
  ~Fl_Chart();

  /**
   The clear method removes all values from the chart.
  */
  void clear();
  /**
   The add method adds the value and optionally label and colour to the chart. 
  */
  void add(double value, const char * label=0, uchar colour=0);
  /**
   The insert method inserts a data value at the given position pos. 
   Position 0 is the first data value. 
  */
  void insert(int pos, double value, const char * label=0, uchar colour=0);
  /**
   The replace method replaces data value pos with value, label, and color. 
   Position 0 is the first data value. 
  */
  void replace(int, double, const char * =0, uchar=0);
  /*@{*/
  /**
   The bounds method gets or sets the lower and upper bounds of 
   the chart values to a and b respectively.
  */
  void bounds(double *a,double *b) const {*a = min; *b = max;}
  void bounds(double a,double b);
  /*@}*/
  /** The size method returns the number of data values in the chart. */
  int size() const {return numb;}
  /*@{*/
  /**
   The maxsize method gets or sets the maximum number of data values for a 
   chart. If you do not call this method then the chart will be allowed to 
   grow to any size depending on available memory.
  */
  int maxsize() const {return maxnumb;}
  void maxsize(int);
  /*@}*/
  /*@{*/
  /**
   The autosize method controls whether or not the chart will automatically 
   adjust the bounds of the chart. The first form returns a boolean value 
   that is non-zero if auto-sizing is enabled and zero is auto-sizing is 
   disabled. 

   The second form of autosize sets the auto-sizing property to onoff.
  */
  uchar autosize() const {return autosize_;}
  void autosize(uchar n) {autosize_ = n;}
  /*@}*/
protected:
  void draw();

private:
  uchar autosize_;
  int numb;
  int maxnumb;
  int sizenumb;
  FL_CHART_ENTRY *entries;
  double min,max;
};

#endif

//
// End of "$Id: Fl_Chart.h,v 1.3 2002/10/29 00:37:23 easysw Exp $".
//
