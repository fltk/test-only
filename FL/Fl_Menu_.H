//
// "$Id: Fl_Menu_.H,v 1.15 1999/07/31 21:27:53 carl Exp $"
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

#define MENU_STYLE ((Fl_Menu_::Style*)_style)

class Fl_Menu_ : public Fl_Widget {
public:
  enum Attribute {
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
    uchar& menu(Attribute a) { return menu_[a]; }
  };

  int is_style_forced(Fl_Widget::Attribute a) const
    { return Fl_Widget::is_style_forced(a); }
  int is_style_forced(Attribute a) const
    { if (!_style) return 0; return ((Style*)_style)->sbf & bf(a); }

  void copy_style(Fl_Menu_* o)
    { mstyle(&_style); *((Style*)_style) = *((Style*)o->style()); }

  Fl_Menu_(int,int,int,int,const char * =0);
  ~Fl_Menu_();

  virtual void loadstyle() const;
  const Fl_Menu_Item* test_shortcut() {return picked(menu()->test_shortcut());}
  void global();

  Fl_Menu_Item* menu() const {return menu_;}
  void menu(const Fl_Menu_Item* m);
  Fl_Menu_Item* find(const char* label) const;
  int index(Fl_Menu_Item* m) const { return m - menu_; }
  int index(const char* label) const { return index(find(label)); }
  void copy(const Fl_Menu_Item *m, void* user_data = 0);
  int  add(const char*, int shortcut, Fl_Callback*, void* = 0, int = 0);
  int  add(const char* a, const char* b, Fl_Callback* c,
	  void* d = 0, int e = 0) {return add(a,fl_old_shortcut(b),c,d,e);}
  int  size() const ;
  void clear();
  int  add(const char*);
  void replace(int,const char*);
  void replace(Fl_Menu_Item* m, const char* s) { replace(index(m), s); }
  void replace(const char* l, const char* s) { replace(index(l), s); }
  void remove(int);
  void remove(Fl_Menu_Item* m) { remove(index(m)); }
  void remove(const char* l) { remove(index(l)); }
  void shortcut(int i, int s) {menu_[i].shortcut(s);}
  void shortcut(Fl_Menu_Item* m, int s) { shortcut(index(m), s); }
  void shortcut(const char* l, int s) { shortcut(index(l), s); }
  void mode(int i,int x) {menu_[i].flags = x;}
  void mode(Fl_Menu_Item* m, int x) { mode(index(m), x); }
  void mode(const char* l, int x) { mode(index(l), x); }
  int  mode(int i) const {return menu_[i].flags;}
  int  mode(Fl_Menu_Item* m) const { return mode(index(m)); }
  int  mode(const char* l) const { return mode(index(l)); }
  const Fl_Menu_Item *mvalue() const {return value_;}
  int value() const {return value_-menu_;}
  int value(const Fl_Menu_Item*);
  int value(int i) {return value(menu_+i);}
  const char *text() const {return value_ ? value_->text : 0;}
  const char *text(int i) const {return menu_[i].text;}
  const Fl_Menu_Item* picked(const Fl_Menu_Item*);

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
  static Fl_Widget::Style* _default_style;

  virtual Fl_Widget::Style* default_style() const { mstyle(&_default_style); return _default_style; }
  virtual void mstyle(Fl_Widget::Style** s) const { if (!(*s)) (*s) = new Style; }

private:
  Fl_Menu_Item *menu_;
  const Fl_Menu_Item *value_;

  uchar attr(Attribute) const;
  void attr(Attribute, uchar);
};

#endif

//
// End of "$Id: Fl_Menu_.H,v 1.15 1999/07/31 21:27:53 carl Exp $".
//
