//
// "$Id: Fl_Counter.H,v 1.7 1999/03/15 18:19:00 carl Exp $"
//
// Counter header file for the Fast Light Tool Kit (FLTK).
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

// A numerical value with up/down step buttons.  From Forms.

#ifndef Fl_Counter_H
#define Fl_Counter_H

#ifndef Fl_Valuator_H
#include "Fl_Valuator.H"
#endif

#define COUNTER_STYLE ((Fl_Counter::Style*)_style)

// values for type():
#define FL_NORMAL_COUNTER	0
#define FL_SIMPLE_COUNTER	1

class Fl_Counter : public Fl_Valuator {
public:
  enum Counter_Attribute {
    FLY_COLOR = 0,
    FLY_BOX,
    DOWN_BOX,
    TEXTFONT,
    TEXTSIZE,
    TEXTCOLOR,
    CA_END
  };

  struct Style : public Fl_Widget::Style {
    uchar counter_[CA_END];
    uchar sbf;

    Style();
    uchar& counter(Counter_Attribute a) { return counter_[a]; }
  };

  virtual void loadstyle();

  int handle(int);
  Fl_Counter(int,int,int,int,const char * = 0);
  ~Fl_Counter();
  void lstep(double a) {lstep_ = a;}
  void step(double a,double b) {Fl_Valuator::step(a); lstep_ = b;}
  void step(double a) {Fl_Valuator::step(a);}

  Fl_Font textfont() const;
  void textfont(uchar s);

  uchar textsize() const;
  void textsize(uchar s);

  Fl_Color textcolor() const;
  void textcolor(uchar n);

  Fl_Boxtype down_box() const;
  void down_box(Fl_Boxtype b);

  Fl_Boxtype fly_box() const;
  void fly_box(Fl_Boxtype b);

  Fl_Color fly_color() const;
  void fly_color(uchar c);

protected:
  static Style _default_style;

  virtual Fl_Widget::Style* default_style() const { return &_default_style; }
  virtual void wstyle() { if (!_style) _style = new Style; }

  void draw();

private:
  double lstep_;
  uchar mouseobj;
  static void repeat_callback(void *);
  int calc_mouseobj();
  void increment_cb();
};

#endif

//
// End of "$Id: Fl_Counter.H,v 1.7 1999/03/15 18:19:00 carl Exp $".
//
