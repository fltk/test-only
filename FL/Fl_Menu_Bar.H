//
// "$Id: Fl_Menu_Bar.H,v 1.9 1999/04/13 20:18:33 carl Exp $"
//
// Menu bar header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu_Bar_H
#define Fl_Menu_Bar_H

#include "Fl_Menu_.H"

#define FL_MAX_MENU_TITLES 20

class Fl_Menu_Bar : public Fl_Menu_ {
public:
  struct Style : public Fl_Menu_::Style {
    Style();
  };

  Fl_Menu_Bar(int x,int y,int w,int h,const char *l=0);
  ~Fl_Menu_Bar() { for(int i=0; i<FL_MAX_MENU_TITLES; i++) delete titles[i]; }

  virtual void loadstyle() const;

protected:
  void* titles[FL_MAX_MENU_TITLES];
  static Fl_Widget::Style* _default_style;

  virtual Fl_Widget::Style* default_style() const { mstyle(&_default_style); return _default_style; }
  virtual void mstyle(Fl_Widget::Style** s) const { if (!(*s)) (*s) = new Style; }

  int handle(int);
  void draw();
private:
  int e;
  void *current_tb, *last_tb;
friend class TitleBox;
};

#endif

//
// End of "$Id: Fl_Menu_Bar.H,v 1.9 1999/04/13 20:18:33 carl Exp $".
//
