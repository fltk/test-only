//
// "$Id: Fl_Choice.H,v 1.10 1999/03/18 22:58:53 carl Exp $"
//
// Choice header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Choice_H
#define Fl_Choice_H

#include "Fl_Menu_.H"

#define CHOICE_STYLE ((Fl_Choice::Style*)_style)

class Fl_Choice : public Fl_Menu_ {
public:
  enum Attribute {
    FLY_COLOR = 0,
    FLY_BOX,
    CA_END
  };

  struct Style : public Fl_Menu_::Style {
    uchar choice_[CA_END];
    uchar sbf;

    Style();
    uchar& choice(Attribute a) { return choice_[a]; }
  };

  int is_style_forced(Fl_Widget::Attribute a) const
    { return Fl_Widget::is_style_forced(a); }
  int is_style_forced(Fl_Menu_::Attribute a) const
    { return Fl_Menu_::is_style_forced(a); }
  int is_style_forced(Attribute a) const
    { if (!_style) return 0; return ((Style*)_style)->sbf & bf(a); }

  void copy_style(Fl_Choice* o)
    { mstyle(&_style); *((Style*)_style) = *((Style*)o->style()); }

  virtual void loadstyle() const;

  int handle(int);
  Fl_Choice(int,int,int,int,const char * = 0);
  int value(int i);
  int value() const {return Fl_Menu_::value();}

  Fl_Boxtype fly_box() const;
  void fly_box(Fl_Boxtype b);

  Fl_Color fly_color() const;
  void fly_color(uchar c);

protected:
  static Fl_Widget::Style* _default_style;

  virtual Fl_Widget::Style* default_style() const { mstyle(&_default_style); return _default_style; }
  virtual void mstyle(Fl_Widget::Style** s) const { if (!(*s)) (*s) = new Style; }

  void draw();
};

#endif

//
// End of "$Id: Fl_Choice.H,v 1.10 1999/03/18 22:58:53 carl Exp $".
//
