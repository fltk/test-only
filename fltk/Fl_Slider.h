//
// "$Id: Fl_Slider.h,v 1.2 2002/01/20 07:37:15 spitzak Exp $"
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

public:

  enum { // values for type(), odd numbers are vertical
    VERTICAL		= 0,
    HORIZONTAL		= 1,
    VERTICAL_FILL	= 2,
    HORIZONTAL_FILL	= 3,
    VERTICAL_NICE	= 4,
    HORIZONTAL_NICE	= 5
  };

  void draw();
  int handle(int);

  Fl_Slider(int x,int y,int w,int h, const char *l = 0);
  Fl_Slider(uchar t,int x,int y,int w,int h, const char *l);
  static Fl_Named_Style* default_style;

  int slider_size() const {return slider_size_;}
  void slider_size(int n) {slider_size_ = n;}
#ifndef FLTK_2
  // back comptability:
  Fl_Boxtype slider() const {return box();}
  void slider(Fl_Boxtype b) {box(b);}
  void slider_size(double v) {slider_size(int(v*w()));}
#endif

protected:

  int slider_size(int W, int H); // return clamped slider_size()
  int slider_position(int W, int S);
  void draw(int, int, int, int, Fl_Flags); // draw plain slider in this box
  int handle(int, int, int, int, int); // handle slider in this box

private:

  int slider_size_;
  void draw_bg(int, int, int, int, Fl_Flags);
};

#ifndef FLTK_2
#define FL_VERT_SLIDER		Fl_Slider::VERTICAL
#define FL_HOR_SLIDER		Fl_Slider::HORIZONTAL
#define FL_VERT_FILL_SLIDER	Fl_Slider::VERTICAL_FILL
#define FL_HOR_FILL_SLIDER	Fl_Slider::HORIZONTAL_FILL
#define FL_VERT_NICE_SLIDER	Fl_Slider::VERTICAL_NICE
#define FL_HOR_NICE_SLIDER	Fl_Slider::HORIZONTAL_NICE
#endif

#endif

//
// End of "$Id: Fl_Slider.h,v 1.2 2002/01/20 07:37:15 spitzak Exp $".
//
