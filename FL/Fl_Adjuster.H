//
// "$Id: Fl_Adjuster.H,v 1.10 1999/03/18 22:58:51 carl Exp $"
//
// Adjuster widget header file for the Fast Light Tool Kit (FLTK).
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

// 3-button "slider", made for Nuke

#ifndef Fl_Adjuster_H
#define Fl_Adjuster_H

#ifndef Fl_Valuator_H
#include "Fl_Valuator.H"
#endif

#define ADJUSTER_STYLE ((Fl_Adjuster::Style*)_style)

class Fl_Adjuster : public Fl_Valuator {
public:
  enum Attribute {
    FLY_COLOR = 0,
    FLY_BOX,
    AA_END
  };

  struct Style : public Fl_Widget::Style {
    uchar adjuster_[AA_END];
    uchar sbf;

    Style();
    uchar& adjuster(Attribute a) { return adjuster_[a]; }
  };

  Fl_Adjuster(int x,int y,int w,int h,const char *l=0);

  int is_style_forced(Fl_Widget::Attribute a) const
    { return Fl_Widget::is_style_forced(a); }
  int is_style_forced(Attribute a) const
    { if (!_style) return 0; return ((Style*)_style)->sbf & bf(a); }

  void copy_style(Fl_Adjuster* o)
    { mstyle(&_style); *((Style*)_style) = *((Style*)o->style()); }

  virtual void loadstyle() const;
  void soft(int x) {soft_ = x;}
  int soft() const {return soft_;}

  Fl_Color fly_color() const;
  void fly_color(uchar c);

  Fl_Boxtype fly_box() const;
  void fly_box(uchar b);

protected:
  static Fl_Widget::Style* _default_style;

  virtual Fl_Widget::Style* default_style() const { mstyle(&_default_style); return _default_style; }
  virtual void mstyle(Fl_Widget::Style** s) const {if ( !(*s)) (*s) = new Style; }

  void draw();
  int handle(int);
  void value_damage();

private:
  int drag;
  int ix;
  int soft_;
};

#endif

//
// End of "$Id: Fl_Adjuster.H,v 1.10 1999/03/18 22:58:51 carl Exp $".
//
