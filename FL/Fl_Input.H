//
// "$Id: Fl_Input.H,v 1.19 2000/09/07 08:52:35 spitzak Exp $"
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

#ifndef Fl_Input_H
#define Fl_Input_H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif

#define FL_NORMAL_INPUT		0
#define FL_SECRET_INPUT		3
#define FL_MULTILINE_INPUT	4
#define FL_WORDWRAP_INPUT	5

class FL_API Fl_Input : public Fl_Widget {
public:
  Fl_Input(int, int, int, int, const char* = 0);
  ~Fl_Input();

  void draw();
  void draw(int, int, int, int);
  int handle(int);
  int handle(int event, int, int, int, int);

  int value(const char*);
  int value(const char*, int);
  int static_value(const char*);
  int static_value(const char*, int);
  const char* value() const {return value_;}
  char index(int i) const {return value_[i];}
  int size() const {return size_;}
  int maximum_size() const {return maximum_size_;}
  void maximum_size(int m) {maximum_size_ = m;}

  char show_cursor() const {return show_cursor_;}
  void show_cursor(char i);

  int position() const {return position_;}
  int mark() const {return mark_;}
  int position(int p, int m);
  int position(int p) {return position(p, p);}
  int mark(int m) {return position(position(), m);}
  virtual int replace(int, int, const char*, int);
  int cut() {return replace(position(), mark(), 0, 0);}
  int cut(int n) {return replace(position(), position()+n, 0, 0);}
  int cut(int a, int b) {return replace(a, b, 0, 0);}
  int insert(const char* t, int l=0){return replace(position_, mark_, t, l);}
  int replace(int a, int b, char c) {return replace(a,b,&c,1);}
  int copy();
  int undo();
  int word_start(int i) const;
  int word_end(int i) const;
  int line_start(int i) const;
  int line_end(int i) const;
  int mouse_position(int, int, int, int) const;
  int up_down_position(int position, int extend);
  void maybe_do_callback();
  int xscroll() const {return xscroll_;}
  int yscroll() const {return yscroll_;}

#if 0 // back compatability, but perhaps not a good idea as it breaks fluid:
  Fl_Boxtype box() const {return text_box();}
  void box(Fl_Boxtype b) {text_box(b);}
  Fl_Color color() const {return text_background();}
  void color(Fl_Color c) {text_background(c);}
#endif

private:
  char erase_cursor_only;
  char show_cursor_;

  int size_;
  int bufsize;
  int position_;
  int mark_;
  int xscroll_, yscroll_;
  int mu_p;
  int maximum_size_;

  const char* value_;
  char* buffer;

  const char* expand(const char*, char*, int) const;
  int expandpos(const char*, const char*, const char*, int*) const;
  void minimal_update(int, int);
  void minimal_update(int p);
  void put_in_buffer(int newsize);

  void setfont() const;

  int handle_key();
  int shift_position(int p);
  int shift_up_down_position(int p);

};

#endif

//
// End of "$Id: Fl_Input.H,v 1.19 2000/09/07 08:52:35 spitzak Exp $".
//
