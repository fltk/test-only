//
// "$Id: Chart.h 7981 2010-12-08 23:53:04Z greg.ercolano $"
//
// Forms chart header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/* \file
 fltk3::Chart widget . */

#ifndef Fltk3_Chart_H
#define Fltk3_Chart_H

#include "Widget.h"

namespace fltk3 {
  
  // values for type()
  const uchar BAR_CHART		= 0;	/**< type() for Bar Chart variant */
  const uchar HORBAR_CHART	= 1;	/**< type() for Horizontal Bar Chart variant */
  const uchar LINE_CHART	= 2;	/**< type() for Line Chart variant */
  const uchar FILL_CHART	= 3;	/**< type() for Fill Line Chart variant */
  const uchar SPIKE_CHART	= 4;	/**< type() for Spike Chart variant */
  const uchar PIE_CHART		= 5;	/**< type() for Pie Chart variant */
  const uchar SPECIALPIE_CHART	= 6;	/**< type() for Special Pie Chart variant */
  
  const uchar FILLED_CHART      = FILL_CHART;	/**< for compatibility */
  
  const int CHART_MAX		= 128;	/**< max entries per chart */
  const int CHART_LABEL_MAX	= 18;	/**< max label length for entry */
  
  /** For internal use only */
  struct CHART_ENTRY {
    float val;				/**< For internal use only. */
    unsigned col;			/**< For internal use only. */
    char str[fltk3::CHART_LABEL_MAX+1];	/**< For internal use only. */
  };
  
  /**
   \class fltk3::Chart
   \brief fltk3::Chart displays simple charts.
   It is provided for Forms compatibility.
   
   \image html charts.png  
   \image latex charts.png  "fltk3::Chart" width=10cm
   \todo Refactor fltk3::Chart::type() information.
   
   The type of an fltk3::Chart object can be set using type(uchar t) to:
   \li \c fltk3::BAR_CHART: Each sample value is drawn as a vertical bar.
   \li \c fltk3::FILLED_CHART: The chart is filled from the bottom of the graph
   to the sample values.
   \li \c fltk3::HORBAR_CHART: Each sample value is drawn as a horizontal bar.
   \li \c fltk3::LINE_CHART: The chart is drawn as a polyline with vertices at
   each sample value.
   \li \c fltk3::PIE_CHART: A pie chart is drawn with each sample value being
   drawn as a proportionate slice in the circle.
   \li \c fltk3::SPECIALPIE_CHART: Like \c fltk3::PIE_CHART, but the first slice is
   separated from the pie.
   \li \c fltk3::SPIKE_CHART: Each sample value is drawn as a vertical line.
   */
  class FLTK3_EXPORT Chart : public fltk3::Widget {
    int numb;
    int maxnumb;
    int sizenumb;
    fltk3::CHART_ENTRY *entries;
    double min,max;
    uchar autosize_;
    fltk3::Font textfont_;
    fltk3::Fontsize textsize_;
    fltk3::Color textcolor_;
  protected:
    void draw();
  public:
    Chart(int X, int Y, int W, int H, const char *L = 0);
    
    ~Chart();
    
    void clear();
    
    void add(double val, const char *str = 0, unsigned col = 0);
    
    void insert(int ind, double val, const char *str = 0, unsigned col = 0);
    
    void replace(int ind, double val, const char *str = 0, unsigned col = 0);
    
    /**
     Gets the lower and upper bounds of the chart values.
     \param[out] a, b are set to lower, upper
     */
    void bounds(double *a,double *b) const {*a = min; *b = max;}
    
    void bounds(double a,double b);
    
    /**
     Returns the number of data values in the chart.
     */
    int size() const {return numb;}
    
    void size(int W, int H) { fltk3::Widget::size(W, H); }
    
    /**
     Gets the maximum number of data values for a chart.
     */
    int maxsize() const {return maxnumb;}
    
    void maxsize(int m);
    
    /** Gets the chart's text font */
    fltk3::Font textfont() const {return textfont_;}
    /** Sets the chart's text font to \p s. */
    void textfont(fltk3::Font s) {textfont_ = s;}
    
    /** Gets the chart's text size */
    fltk3::Fontsize textsize() const {return textsize_;}
    /** gets the chart's text size to \p s. */
    void textsize(fltk3::Fontsize s) {textsize_ = s;}
    
    /** Gets the chart's text color */
    fltk3::Color textcolor() const {return textcolor_;}
    /** gets the chart's text color to \p n. */
    void textcolor(fltk3::Color n) {textcolor_ = n;}
    
    /**
     Get whether the chart will automatically adjust the bounds of the chart.
     \returns non-zero if auto-sizing is enabled and zero if disabled.
     */
    uchar autosize() const {return autosize_;}
    
    /**
     Set whether the chart will automatically adjust the bounds of the chart.
     \param[in] n non-zero to enable automatic resizing, zero to disable.
     */
    void autosize(uchar n) {autosize_ = n;}
  };
  
}

#endif

//
// End of "$Id: Chart.h 7981 2010-12-08 23:53:04Z greg.ercolano $".
//
