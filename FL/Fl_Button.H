//
// "$Id: Fl_Button.H,v 1.7 1999/03/15 18:18:59 carl Exp $"
//
// Button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Button_H
#define Fl_Button_H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif

#define BUTTON_STYLE ((Fl_Button::Style*)_style)

// values for type()
#define FL_TOGGLE_BUTTON	1
#define FL_RADIO_BUTTON		(FL_RESERVED_TYPE+2)
#define FL_HIDDEN_BUTTON	3 // for Forms compatability

extern int fl_old_shortcut(const char*);

class Fl_Button : public Fl_Widget {
public:
  enum Button_Attribute {
    FLY_COLOR = 0,
    FLY_BOX,
    DOWN_BOX,
    DOWN_LABELCOLOR,
    BA_END
  };

  struct Style : public Fl_Widget::Style {
    uchar button_[BA_END];
    uchar sbf;

    Style();
    uchar& button(Button_Attribute a) { return button_[a]; }
  };

  virtual void loadstyle();

  virtual int handle(int);
  Fl_Button(int,int,int,int,const char * = 0);
  int value(int);
  char value() const {return value_;}
  int set() {return value(1);}
  int clear() {return value(0);}
  void setonly(); // this should only be called on FL_RADIO_BUTTONs
  int shortcut() const {return shortcut_;}
  void shortcut(int s) {shortcut_ = s;}

  Fl_Boxtype down_box() const;
  void down_box(Fl_Boxtype b);

  Fl_Boxtype fly_box() const;
  void fly_box(Fl_Boxtype b);

  Fl_Color down_labelcolor() const;
  void down_labelcolor(uchar c);

  Fl_Color fly_color() const;
  void fly_color(uchar c);

  // back compatability:
  void shortcut(const char *s) {shortcut(fl_old_shortcut(s));}
  Fl_Color down_color() const;
  void down_color(uchar c);

protected:
  static Style _default_style;

  virtual Fl_Widget::Style* default_style() const { return &_default_style; }
  virtual void wstyle() { if (!_style) _style = new Style; }
  virtual void draw();

private:
  char value_;
  char oldval;
  int shortcut_;
};

#endif

//
// End of "$Id: Fl_Button.H,v 1.7 1999/03/15 18:18:59 carl Exp $".
//
