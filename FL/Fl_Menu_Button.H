//
// "$Id: Fl_Menu_Button.H,v 1.6 1999/03/14 06:46:16 carl Exp $"
//
// Menu button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu_Button_H
#define Fl_Menu_Button_H

#include "Fl_Menu_.H"

#define MENU_BUTTON_STYLE ((Fl_Menu_Button::Style*)style)

class Fl_Menu_Button : public Fl_Menu_ {
public:
  enum Menu_Button_Attribute {
    FLY_COLOR = 0,
    FLY_BOX,
    MBA_END
  };

  struct Style : public Fl_Menu_::Style {
    uchar menu_button_[MBA_END];
    uchar sbf;

    Style();
    uchar& menu_button(Menu_Button_Attribute a) { return menu_button_[a]; }
  };

  virtual void loadstyle();

  Fl_Boxtype fly_box() const;
  void fly_box(Fl_Boxtype b);

  Fl_Color fly_color() const;
  void fly_color(uchar c);

  // values for type:
  enum {POPUP1 = 1, POPUP2, POPUP12, POPUP3, POPUP13, POPUP23, POPUP123};
  int handle(int);
  const Fl_Menu_Item* popup();
  Fl_Menu_Button(int,int,int,int,const char * =0);

protected:
  static Style _default_style;

  virtual Fl_Widget::Style* default_style() const { return &_default_style; }
  virtual void wstyle() { if (!style) style = new Style; }

  void draw();
};

#endif

//
// End of "$Id: Fl_Menu_Button.H,v 1.6 1999/03/14 06:46:16 carl Exp $".
//
