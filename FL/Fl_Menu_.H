//
// "$Id: Fl_Menu_.H,v 1.8 1999/03/14 06:46:15 carl Exp $"
//
// Menu base class header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu__H
#define Fl_Menu__H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif
#include "Fl_Menu_Item.H"

#define MENU_STYLE ((Fl_Menu_::Style*)style)

class Fl_Menu_ : public Fl_Widget {
public:
  enum Menu_Attribute {
    TEXTFONT = 0,
    TEXTSIZE,
    TEXTCOLOR,
    DOWN_BOX,
    MA_END
  };

  struct Style : public Fl_Widget::Style {
    uchar menu_[MA_END];
    uchar sbf;

    Style();
    uchar& menu(Menu_Attribute a) { return menu_[a]; }
  };

  Fl_Menu_(int,int,int,int,const char * =0);
  ~Fl_Menu_();

  virtual void loadstyle();
  const Fl_Menu_Item* test_shortcut() {return picked(menu()->test_shortcut());}
  void global();

  Fl_Menu_Item* menu() const {return menu_;}
  void menu(const Fl_Menu_Item* m);
  Fl_Menu_Item* find(const char* label);
  void copy(const Fl_Menu_Item *m, void* user_data = 0);
  int  add(const char*, int shortcut, Fl_Callback*, void* = 0, int = 0);
  int  add(const char* a, const char* b, Fl_Callback* c,
	  void* d = 0, int e = 0) {return add(a,fl_old_shortcut(b),c,d,e);}
  int  size() const ;
  void clear();
  int  add(const char *);
  void replace(int,const char *);
  void remove(int);
  void shortcut(int i, int s) {menu_[i].shortcut(s);}
  void mode(int i,int x) {menu_[i].flags = x;}
  int  mode(int i) const {return menu_[i].flags;}

  const Fl_Menu_Item *mvalue() const {return value_;}
  int value() const {return value_-menu_;}
  int value(const Fl_Menu_Item*);
  int value(int i) {return value(menu_+i);}
  const char *text() const {return value_ ? value_->text : 0;}
  const char *text(int i) const {return menu_[i].text;}

  Fl_Font textfont() const;
  void textfont(uchar s);

  uchar textsize() const;
  void textsize(uchar s);

  Fl_Color textcolor() const;
  void textcolor(uchar n);

  Fl_Boxtype down_box() const;
  void down_box(Fl_Boxtype b);

  // back compatability:
  Fl_Color down_color() const;
  void down_color(uchar c);

protected:
  uchar alloc;
  static Style _default_style;

  virtual Fl_Widget::Style* default_style() const { return &_default_style; }
  virtual void wstyle() { if (!style) style = new Style; }

  const Fl_Menu_Item* picked(const Fl_Menu_Item*);

private:
  Fl_Menu_Item *menu_;
  const Fl_Menu_Item *value_;
};

#endif

//
// End of "$Id: Fl_Menu_.H,v 1.8 1999/03/14 06:46:15 carl Exp $".
//
