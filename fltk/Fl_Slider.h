//
// "$Id: Fl_Slider.h,v 1.3 2002/02/25 09:00:19 spitzak Exp $"
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

class FL_API Fl_Slider : public Fl_Valuator {

  unsigned short slider_size_;
  unsigned short old_position;

public:

  enum { // bit flags for type():
    VERTICAL		= 0,
    HORIZONTAL		= 1,
    TICK_ABOVE		= 2,
    TICK_LEFT		= TICK_ABOVE,
    TICK_BELOW		= 4,
    TICK_RIGHT		= TICK_BELOW,
    TICK_BOTH		= TICK_ABOVE|TICK_BELOW,
    LOG			= 8
  };
  bool horizontal() const {return type()&HORIZONTAL;}
  bool log() const {return type()&LOG;}

  void draw();
  int handle(int);

  Fl_Slider(int x,int y,int w,int h, const char *l = 0);
  static Fl_Named_Style* default_style;

  unsigned short slider_size() const {return slider_size_;}
  void slider_size(int n) {slider_size_ = (unsigned short)n;}
  
#ifndef FLTK_2
  // back comptability:
  Fl_Boxtype slider() const {return button_box();}
  void slider(Fl_Boxtype b) {button_box(b);}
  void slider_size(double v) {slider_size(int(v*w()));}
#endif

protected:

  int slider_position(double value, int w);
  double position_value(int x, int w);
  int handle(int event, int, int, int, int);
  void draw_ticks(int,int,int,int);
  bool draw(int, int, int, int, Fl_Flags flags, bool slot);
};

#ifndef FLTK_2
#define FL_VERT_SLIDER		Fl_Slider::VERTICAL
#define FL_HOR_SLIDER		Fl_Slider::HORIZONTAL
#define FL_VERT_FILL_SLIDER	Fl_Slider::VERTICAL
#define FL_HOR_FILL_SLIDER	Fl_Slider::HORIZONTAL
#define FL_VERT_NICE_SLIDER	Fl_Slider::VERTICAL
#define FL_HOR_NICE_SLIDER	Fl_Slider::HORIZONTAL
#define HORIZONTAL_FILL HORIZONTAL
#define VERTICAL_FILL VERTICAL
#define HORIZONTAL_NICE HORIZONTAL
#define VERTICAL_NICE VERTICAL
#endif

#endif

//
// End of "$Id: Fl_Slider.h,v 1.3 2002/02/25 09:00:19 spitzak Exp $".
//
