//
// "$Id: Fl_Browser.H,v 1.20 2000/09/05 17:36:20 spitzak Exp $"
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

// This file is temporary, it will completely replace the current
// Fl_Browser.H and Fl_Browser_.H

#ifndef Fl_Browser_H
#define Fl_Browser_H

#include "Fl_Menu_.H"
#include "Fl_Scrollbar.H"

class FL_API Fl_Browser : public Fl_Menu_ {
  Fl_End endgroup;

public:

  // marks keep track of various items in the browser?
  enum { // predefined marks - (there are no non-predefined marks, right? yes)
    HERE = 0, // current item, the one moved by all the calls
    FOCUS,
    FIRST_VISIBLE,
    REDRAW_0,
    REDRAW_1,
    USER_0,
    USER_1,
    NUMMARKS
  };

  Fl_Widget* goto_top(); // set HERE to top of browser
  Fl_Widget* goto_mark(int mark); // set HERE to mark
  Fl_Widget* goto_position(int y); // set HERE to item y pixels from top
  Fl_Widget* goto_number(int n); // set HERE to n'th visible item
  Fl_Widget* goto_visible_focus(); // set HERE to focus if visible
  Fl_Widget* forward(); // set HERE to next visible item
  Fl_Widget* backward(); // set HERE to previous visible item
  void set_mark(int dest, int mark); // set dest to mark
  int compare_marks(int mark1, int mark2); // returns >0 if mark1 after mark2
  int at_mark(int mark) { return !compare_marks(HERE,mark); }
  void unset_mark(int mark);  // makes mark have illegal value
  int is_set(int mark);  // false if unset_mark was called
  void damage_item(int mark); // make this item redraw
  int set_focus();
  void set_top();
  int item_select(int value = 1, int do_callback = 0);
  int item_select_only(int do_callback = 0);

  Fl_Browser(int X,int Y,int W,int H,const char*l=0);
  ~Fl_Browser();

  enum { // values for type()
    HORIZONTAL = 1,
    VERTICAL = 2,
    BOTH = 3,
    ALWAYS_ON = 4,
    HORIZONTAL_ALWAYS = 5,
    VERTICAL_ALWAYS = 6,
    BOTH_ALWAYS = 7,
    MULTI_BROWSER = 8
  };

  int xposition() const {return xposition_;}
  void xposition(int);
  int yposition() const {return yposition_;}
  void yposition(int);
  char indented() const {return indented_;}
  void indented(char v) {indented_ = v;}
  int multi() const {return type()&MULTI_BROWSER;}

  Fl_Scrollbar scrollbar;
  Fl_Scrollbar hscrollbar;

  int handle(int);
  void layout();
  void draw();

  // fltk 1.0 Fl_Browser emulation: (see also Fl_Menu_.H)
  void deselect(int do_callback = 0);
  int select(int line, int value = 1);
  int selected(int line);
  void topline(int line) {goto_number(line); set_top();}
  char format_char() const {return format_char_;}
  void format_char(char c) {format_char_ = c;}
  char column_char() const {return column_char_;}
  void column_char(char c) {column_char_ = c;}
  int displayed(int line);
  void display(int line, int value = 1);
  int topline() const {return item_number[FIRST_VISIBLE];}
  void value(int v) {if (goto_number(v)) set_focus();}
  int value() const {return Fl_Menu_::value();}

private:

  char indented_;
  char format_char_;
  char column_char_;
  int xposition_, yposition_;
  int scrolldx, scrolldy;
  static void hscrollbar_cb(Fl_Widget*, void*);
  static void scrollbar_cb(Fl_Widget*, void*);
  void draw_item();
  void draw_clip(int,int,int,int);
  static void draw_clip_cb(void*,int,int,int,int);
  int X,Y,W,H; // bounding box area

  // Mark and current item storage:
  int item_level[NUMMARKS]; // mark item's nesting depth in group hierarchy?
  int item_position[NUMMARKS]; // distance in pixels from top of browser?
  int item_number[NUMMARKS];
  int* item_index[NUMMARKS];
  int levels;
  void set_level(int);

};

#define FL_NORMAL_BROWSER Fl_Browser::BOTH
#define FL_MULTI_BROWSER (Fl_Browser::BOTH|Fl_Browser::MULTI_BROWSER)

#ifndef FLTK_2
#define FL_SELECT_BROWSER Fl_Browser::BOTH
#define FL_HOLD_BROWSER Fl_Browser::BOTH
#endif

#endif

//
// End of "$Id: Fl_Browser.H,v 1.20 2000/09/05 17:36:20 spitzak Exp $".
//
