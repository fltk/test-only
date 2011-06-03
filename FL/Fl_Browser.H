//
// "$Id$"
//
// Browser header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper started
//
// Copyright 1998-2011 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifndef Fl_Browser_H
#define Fl_Browser_H

#include <fltk3/Browser.h>
#include "Fl_Browser_.H"
#include "Fl_Image.H"

struct FL_BLINE;

class FL_EXPORT Fl_Browser : public Fl_Browser_ {

protected:

  // FIXME: 123 - many of these are virtual!
  void* item_first() const ;
  void* item_next(void* item) const ;
  void* item_prev(void* item) const ;
  void* item_last()const ;
  int item_selected(void* item) const ;
  void item_select(void* item, int val);
  int item_height(void* item) const ;
  int item_width(void* item) const ;
  void item_draw(void* item, int X, int Y, int W, int H) const ;
  int full_height() const ;
  int incr_height() const ;
  const char *item_text(void *item) const;
  void item_swap(void *a, void *b) { swap((FL_BLINE*)a, (FL_BLINE*)b); }
  void *item_at(int line) const { return (void*)find_line(line); }

  FL_BLINE* find_line(int line) const ;
  FL_BLINE* _remove(int line) ;
  void insert(int line, FL_BLINE* item);
  int lineno(void *item) const ;
  void swap(FL_BLINE *a, FL_BLINE *b);

public:

  void remove(int line);
  void add(const char* newtext, void* d = 0);
  void insert(int line, const char* newtext, void* d = 0);
  void move(int to, int from);
  int  load(const char* filename);
  void swap(int a, int b);
  void clear();
  int size() const { return lines; }
  void size(int W, int H) { Fl_Widget::size(W, H); }
  int topline() const ;
  enum Fl_Line_Position { TOP, BOTTOM, MIDDLE };
  void lineposition(int line, Fl_Line_Position pos);
  void topline(int line) { lineposition(line, TOP); }
  void bottomline(int line) { lineposition(line, BOTTOM); }
  void middleline(int line) { lineposition(line, MIDDLE); }
  int select(int line, int val=1);
  int selected(int line) const ;
  void show(int line);
  void show() { Fl_Widget::show(); }
  void hide(int line);
  void hide() { Fl_Widget::hide(); }
  int visible(int line) const ;
  int value() const ;
  void value(int line) { select(line); }
  const char* text(int line) const ;
  void text(int line, const char* newtext);
  void* data(int line) const ;
  void data(int line, void* d);
  Fl_Browser(int X, int Y, int W, int H, const char *L = 0);
  ~Fl_Browser() { clear(); }
  char format_char() const { return format_char_; }
  void format_char(char c) { format_char_ = c; }
  char column_char() const { return column_char_; }
  void column_char(char c) { column_char_ = c; }
  const int* column_widths() const { return column_widths_; }
  void column_widths(const int* arr) { column_widths_ = arr; }
  int displayed(int line) const { return Fl_Browser_::displayed(find_line(line)); }
  void make_visible(int line) ;
  void icon(int line, Fl_Image* icon);
  Fl_Image* icon(int line) const;
  void remove_icon(int line);
};

#endif

//
// End of "$Id$".
//
