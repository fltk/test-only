//
// "$Id: Fl_Input_.H,v 1.12 1999/05/06 05:52:08 carl Exp $"
//
// Input base class header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Input__H
#define Fl_Input__H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif

#define INPUT_STYLE ((Fl_Input_::Style*)_style)

#define FL_NORMAL_INPUT		0
#define FL_FLOAT_INPUT		1
#define FL_INT_INPUT		2
#define FL_HIDDEN_INPUT		3
#define FL_MULTILINE_INPUT	4
#define FL_SECRET_INPUT		5

class Fl_Input_ : public Fl_Widget {
  friend class Fl_Value_Input;
  friend class Fl_Value_Output;
public:
  enum Attribute {
    TEXTFONT = 0,
    TEXTSIZE,
    TEXTCOLOR,
    SELECTED_TEXTCOLOR,
    CURSOR_COLOR,
    IA_END
  };

  struct Style : public Fl_Widget::Style {
    uchar input_[IA_END];
    uchar sbf;

    Style();
    uchar& input(Attribute a) { return input_[a]; }
  };

  int is_style_forced(Fl_Widget::Attribute a) const
    { return Fl_Widget::is_style_forced(a); }
  int is_style_forced(Attribute a) const
    { if (!_style) return 0; return ((Style*)_style)->sbf & bf(a); }

  void copy_style(Fl_Input_* o)
    { mstyle(&_style); *((Style*)_style) = *((Style*)o->style()); }

  virtual void loadstyle() const;

  void resize(int, int, int, int);

  Fl_Input_(int, int, int, int, const char* = 0);
  ~Fl_Input_();

  int value(const char*);
  int value(const char*, int);
  int static_value(const char*);
  int static_value(const char*, int);
  const char* value() const {return value_;}
  char index(int i) const {return value_[i];}
  int size() const {return size_;}
  int maximum_size() const {return maximum_size_;}
  void maximum_size(int m) {maximum_size_ = m;}

  int position() const {return position_;}
  int mark() const {return mark_;}
  int position(int p, int m);
  int position(int p) {return position(p, p);}
  int mark(int m) {return position(position(), m);}
  int replace(int, int, const char*, int=0);
  int cut() {return replace(position(), mark(), 0);}
  int cut(int n) {return replace(position(), position()+n, 0);}
  int cut(int a, int b) {return replace(a, b, 0);}
  int insert(const char* t, int l=0){return replace(position_, mark_, t, l);}
  int copy();
  int undo();
  int copy_cuts();

  Fl_Font textfont() const;
  void textfont(uchar s);

  uchar textsize() const;
  void textsize(uchar s);

  Fl_Color textcolor() const;
  void textcolor(uchar n);

  Fl_Color cursor_color() const;
  void cursor_color(uchar n);

  Fl_Color selected_textcolor() const;
  void selected_textcolor(uchar n);

protected:
  static Fl_Widget::Style* _default_style;

  virtual Fl_Widget::Style* default_style() const { mstyle(&_default_style); return _default_style; }
  virtual void mstyle(Fl_Widget::Style** s) const { if (!(*s)) (*s) = new Style; }

  int wordboundary(int i) const;
  int lineboundary(int i) const;
  void drawtext(int, int, int, int);
  int up_down_position(int, int keepmark=0);
  void handle_mouse(int, int, int, int, int keepmark=0);
  int handletext(int e, int, int, int, int);
  void maybe_do_callback();
  int xscroll() const {return xscroll_;}
  int yscroll() const {return yscroll_;}

private:
  uchar erase_cursor_only;

  int size_;
  int bufsize;
  int position_;
  int mark_;
  int xscroll_, yscroll_;
  int mu_p;
  int maximum_size_;

  const char* value_;
  char* buffer;

  const char* expand(const char*, char*) const;
  double expandpos(const char*, const char*, const char*, int*) const;
  void minimal_update(int, int);
  void minimal_update(int p);
  void put_in_buffer(int newsize);

  void setfont() const;

  uchar attr(Attribute) const;
  void attr(Attribute, uchar);
};

#endif

//
// End of "$Id: Fl_Input_.H,v 1.12 1999/05/06 05:52:08 carl Exp $".
//
