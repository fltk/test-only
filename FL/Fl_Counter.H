//
// "$Id: Fl_Counter.H,v 1.10 1999/03/18 22:58:53 carl Exp $"
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
  enum Attribute {
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
    uchar& counter(Attribute a) { return counter_[a]; }
  };

  int is_style_forced(Fl_Widget::Attribute a) const
    { return Fl_Widget::is_style_forced(a); }
  int is_style_forced(Attribute a) const
    { if (!_style) return 0; return ((Style*)_style)->sbf & bf(a); }

  void copy_style(Fl_Counter* o)
    { mstyle(&_style); *((Style*)_style) = *((Style*)o->style()); }

  virtual void loadstyle() const;

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
  static Fl_Widget::Style* _default_style;

  virtual Fl_Widget::Style* default_style() const { mstyle(&_default_style); return _default_style; }
  virtual void mstyle(Fl_Widget::Style** s) const { if (!(*s)) (*s) = new Style; }

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
// End of "$Id: Fl_Counter.H,v 1.10 1999/03/18 22:58:53 carl Exp $".
//
