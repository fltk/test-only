//
// "$Id$"
//
// Slider header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Slider_H
#define Fl_Slider_H

#ifndef Fl_Valuator_H
#include "Fl_Valuator.H"
#endif

// values for type(), lowest bit indicate horizontal:
#define FL_VERT_SLIDER		0
#define FL_HOR_SLIDER		1
#define FL_VERT_FILL_SLIDER	2
#define FL_HOR_FILL_SLIDER	3
#define FL_VERT_NICE_SLIDER	4
#define FL_HOR_NICE_SLIDER	5

/** The Fl_Slider widget contains a sliding knob inside a box. It if often
 * used as a scrollbar. Moving the box all the way to the top/left sets it
 * to the minimum(), and to the bottom/right to the maximum(). 
 * The minimum() may be greater than the maximum() to reverse the slider
 * direction. 
 *   \image html slider.gif
 * Fl_Slider supports the following types:
 * \arg \c FL_VERTICAL - Draws a vertical slider (this is the default).
 * \arg \c FL_HORIZONTAL - Draws a horizontal slider.
 * \arg \c FL_VERT_FILL_SLIDER - Draws a filled vertical slider, useful 
 *      as a progress or value meter.
 * \arg \c FL_HOR_FILL_SLIDER - Draws a filled horizontal slider, useful
 *      as a progress or value meter.
 * \arg \c FL_VERT_NICE_SLIDER - Draws a vertical slider with a nice looking
 *      control knob.
 * \arg \c FL_HOR_NICE_SLIDER - Draws a horizontal slider with a nice
 *      looking control knob.
 */
class FL_EXPORT Fl_Slider : public Fl_Valuator {

  float slider_size_;
  Fl_Boxtype slider_;
  // uchar slider_;
  void _Fl_Slider();
  void draw_bg(int, int, int, int);

protected:

  // these allow subclasses to put the slider in a smaller area:
  void draw(int, int, int, int);
  int handle(int, int, int, int, int);

public:

  void draw();
  int handle(int);
    /** Creates a new Fl_Slider widget using the given position, size, 
     * and label string. The default boxtype is \c FL_DOWN_BOX. */
  Fl_Slider(int x,int y,int w,int h, const char *l = 0);
  Fl_Slider(uchar boxtype,int x,int y,int w,int h, const char *l);

    /** Returns Fl_Scrollbar::value(). */
  int scrollvalue(int windowtop,int windowsize,int first,int totalsize);
  void bounds(double a, double b);
    /** Get the dimensions of the moving piece of slider. */
  float slider_size() const {return slider_size_;}
    /** Set the dimensions of the moving piece of slider. This is the
     * fraction of the size of the entire widget. If you set this to 1 then
     * the slider cannot move. The default value is .08.
     *
     * For the "fill" sliders this is the size of the area around the end
     * that causes a drag effect rather than causing the slider to jump to
     * the mouse. */
  void slider_size(double v);
  Fl_Boxtype slider() const {return (Fl_Boxtype)slider_;}
    /** Set the type of box to draw for the moving part of the slider. The
     * color of the moving part (or of the notch in it for the nice sliders)
     * is controlled by selection_color(). The default value of zero causes
     * the slider to figure out what to draw from box(). */
  void slider(Fl_Boxtype c) {slider_ = c;}
};

#endif

//
// End of "$Id$".
//
