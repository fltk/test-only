//
// "$Id: Fl_Slider.H,v 1.6 1999/03/14 06:46:17 carl Exp $"
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
#include "Fl_Valuator.H"
#endif

#define SLIDER_STYLE ((Fl_Slider::Style*)style)

// values for type(), lowest bit indicate horizontal:
#define FL_VERT_SLIDER		0
#define FL_HOR_SLIDER		1
#define FL_VERT_FILL_SLIDER	2
#define FL_HOR_FILL_SLIDER	3
#define FL_VERT_NICE_SLIDER	4
#define FL_HOR_NICE_SLIDER	5

class Fl_Slider : public Fl_Valuator {
public:
  enum Slider_Attribute {
    FLY_COLOR = 0,
    SLIDER_BOX,
    SA_END
  };

  struct Style : public Fl_Widget::Style {
    uchar slider_[SA_END];
    uchar sbf;

    Style();
    uchar& slider(Slider_Attribute a) { return slider_[a]; }
  };

  virtual void loadstyle();

  void draw();
  int handle(int);
  Fl_Slider(int x,int y,int w,int h, const char *l = 0);
  Fl_Slider(uchar t,int x,int y,int w,int h, const char *l);

  int scrollvalue(int windowtop,int windowsize,int first,int totalsize);
  void bounds(double a, double b);
  float slider_size() const {return slider_size_;}
  void slider_size(double v);

  Fl_Boxtype slider() const;
  void slider(Fl_Boxtype b);

  Fl_Color fly_color() const;
  void fly_color(uchar c);

protected:
  static Style _default_style;

  virtual Fl_Widget::Style* default_style() const { return &_default_style; }
  virtual void wstyle() { if (!style) style = new Style; }

  // these allow subclasses to put the slider in a smaller area:
  void draw(int, int, int, int);
  int handle(int, int, int, int, int);

private:
  float slider_size_;
  void _Fl_Slider();
  void draw_bg(int, int, int, int);
};

#endif

//
// End of "$Id: Fl_Slider.H,v 1.6 1999/03/14 06:46:17 carl Exp $".
//
