//
// "$Id: Fl_Slider.h,v 1.6 2002/10/29 00:37:23 easysw Exp $"
//
// Slider header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Slider_H
#define Fl_Slider_H

#ifndef Fl_Valuator_H
#include "Fl_Valuator.h"
#endif

/**

   The Fl_Slider widget contains a sliding "knob" that controls a single 
   floating-point value. Moving the box all the way to the left or bottom
   sets it to the minimum(), and to the top/right to the maximum() value.
   The minimum() may be set greater than the maximum() to reverse the 
   slider direction. See Fl_Valuator for how to set or get the value or
   handle callbacks when the value changes. 
  
   \image slider.gif
   \image logslider.gif

   The appearance of the slider may be changed by setting the type() to
   the "or" of a several bits. 

   By default the slider moves vertically. Setting the HORIZONTAL bit 
   in the type makes it move horizontally. 

   "Log" sliders have a non-uniform scale. The above-right diagram shows
   some examples. The scale is truly logarithmic if both the minimum()
   and the maximum() are non-zero and have the same sign. Otherwise the 
   slider position is the square root of the value (or -sqrt(-value) 
   for negative values). 

   You can turn on tick marks by turning on bits in the type(). The tick
   marks are placed the slider_size() or more apart (they are also no 
   closer than the step() value). The color of the tick marks is controlled
   by text_color(), and the font used to draw the numbers is text_font(). 

   Setting the box() as shown in the example on the bottom-left will 
   remove the "slot" and draw a box around the slider and the tick marks.
   Setting the color() fills in this box. Setting the button_box() 
   changes how the moving part is drawn (only rectangular boxes work),
   and button_color() colors it. 

*/
class FL_API Fl_Slider : public Fl_Valuator {

  unsigned short slider_size_;
  unsigned short tick_size_;
  unsigned short old_position;

public:

  /**

   Any of these values may be "or'd" together to produce the type(), 
   the default value is VERTICAL. 

   - Fl_Slider::VERTICAL - Slider moves vertically. 
   - Fl_Slider::HORIZONTAL - Slider moves horizontally. 
   - Fl_Slider::TICK_ABOVE - Put tick marks above the horizontal slider. 
   - Fl_Slider::TICK_LEFT - 
      Put tick marks to the left of a vertical slider, 
      same value as TICK_ABOVE 
   - Fl_Slider::TICK_BELOW - Put tick marks below the horizontal slider. 
   - Fl_Slider::TICK_RIGHT - 
      Put tick marks to the right of a vertical slider, 
      same value as TICK_BELOW 
   - Fl_Slider::TICK_BOTH - Put tick marks on both sides of the slider. 
   - Fl_Slider::LOG - Use a logarithimic or power scale for the slider. 

  */
  enum { // bit flags for type():
    VERTICAL		= 0,
    HORIZONTAL		= 1,
    TICK_ABOVE		= 2,
    TICK_LEFT		= TICK_ABOVE,
    TICK_BELOW		= 4,
    TICK_RIGHT		= TICK_BELOW,
    TICK_BOTH		= TICK_ABOVE|TICK_BELOW,
    LOG			= 8,
    FILL		= 16 // for back compatability only
  };
  bool horizontal() const {return (type()&HORIZONTAL)!=0;}
  bool log() const {return (type()&LOG)!=0;}

  void draw();
  int handle(int);

  /**
   Creates a new Fl_Slider widget using the given position, size,
   and label string. The default type() is Fl_Slider::VERTICAL (0).
   The default boxtype is FL_DOWN_BOX. 
  */
  Fl_Slider(int x,int y,int w,int h, const char *l = 0);
  static Fl_Named_Style* default_style;

  /*@{*/
  /**

   Get or set the dimensions of the moving piece of slider. 
   This is measured in pixels in a direction parallel to the 
   slider's movement. The default value is 12. 

   Setting slider_size() to zero will make the slider into a
   "fill" slider that draws a solid bar from the left/bottom 
   to the current value. This is useful for indicating 
   progress or as a output indicator. 

  */
  unsigned short slider_size() const {return slider_size_;}
  void slider_size(int n) {slider_size_ = (unsigned short)n;}
  /*@}*/
  /*@{*/
  /**
   The slider is shrunk this many pixels from the widget's width 
   so that the tick marks are visible next to it. The default value is 4.
  */
  unsigned short tick_size() const {return tick_size_;}
  void tick_size(int n) {tick_size_ = (unsigned short)n;}
  /*@}*/
  
#ifndef FLTK_2
  // back comptability:
  Fl_Boxtype slider() const {return button_box();}
  void slider(Fl_Boxtype b) {button_box(b);}
  void slider_size(double v) {slider_size(int(v*w()));}
#endif

protected:

  /**
   Return the location in pixels of the left/top edge of a box 
   of slider_size() should be drawn at if it is positioned at
   the value value in an area the slider can move in of width/height w. 
  */
  int slider_position(double value, int w);
  /**
   Return the value that would place the left/top edge of a box 
   of slider_size() at pixel x in an area the slider can move in of 
   width/height w. 
  */
  double position_value(int x, int w);
  /**
   Handle events for a slider where the moving part is in the passed box.
  */
  int handle(int event, int, int, int, int);
  /**
   Draw tick marks to fill the passed rectangle. These lines cross the 
   passed rectangle perpendicular to the slider direction. In the direction
   parallel to the slider direction the box should have the same size as 
   the area the slider moves in. 
  */
  void draw_ticks(int x,int y,int w,int h, int min_spacing);
  /**
   Do minimal-update redraw of the moving part of the slider that fits in the 
   passed rectangle. Also draw the black slot if slot is true. If this returns
   true then it has done an fl_push_clip() and you must fill in the remaining
   area with the background that goes behind the slider. The clipped area will
   either be the entire widget or the area the slider used to be in, with the
   area of the new slider and the slot removed from it. 
   Typical usage in a subclass:
   
   \code

   void MySlider::draw() {
     // figure out inset box to hold moving part of slider:
     int ix = 10;
     int iy = 10;
     int iw = w()-20;
     int ih = h()-20;
     // draw the moving part:
     if (draw(ix, iy, iw, ih, 0, false)) {
       // we must draw the background:
       draw_spiffy_background(0,0,w(),h());
       // draw the tick marks across whole widget:
       draw_ticks(ix, 0, iw, h());
       fl_pop_clip();
     }
   }

   \endcode

  */
  bool draw(int, int, int, int, Fl_Flags flags, bool slot);
};

#ifndef FLTK_2
#define FL_VERT_SLIDER		Fl_Slider::VERTICAL
#define FL_HOR_SLIDER		Fl_Slider::HORIZONTAL
#define FL_VERT_FILL_SLIDER	Fl_Slider::VERTICAL|Fl_Slider::FILL
#define FL_HOR_FILL_SLIDER	Fl_Slider::HORIZONTAL|Fl_Slider::FILL
#define FL_VERT_NICE_SLIDER	Fl_Slider::VERTICAL
#define FL_HOR_NICE_SLIDER	Fl_Slider::HORIZONTAL
#define HORIZONTAL_FILL HORIZONTAL|Fl_Slider::FILL
#define VERTICAL_FILL VERTICAL|Fl_Slider::FILL
#define HORIZONTAL_NICE HORIZONTAL
#define VERTICAL_NICE VERTICAL
#endif

#endif

//
// End of "$Id: Fl_Slider.h,v 1.6 2002/10/29 00:37:23 easysw Exp $".
//
