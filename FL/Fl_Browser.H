//
// "$Id: Fl_Browser.H,v 1.19 2000/08/21 03:56:23 spitzak Exp $"
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

  Fl_Widget* goto_top(); // set current item to top item in browser?
  Fl_Widget* goto_mark(int mark); // set current item to one of the marks?
  Fl_Widget* goto_position(int y); // set current item to one (near) y pixels from top?
  Fl_Widget* goto_number(int y); // go to toplevel item number
  Fl_Widget* goto_visible_focus();
  Fl_Widget* forward(); // set current item to next visible item?
  Fl_Widget* backward(); // set current item to previous visible item?

  // are these two marks the same item?
  int marks_equal(int mark1, int mark2);
  // is this mark the current item?
  int at_mark(int mark) { return marks_equal(mark, HERE); }
  // compare relative locations of these two marks
  int compare_mark_locations(int mark1, int mark2);
  // is current item before this mark?
  int before_mark(int mark) { return compare_mark_locations(HERE, mark) < 0; }
  // is current item after this mark?
  int after_mark(int mark) { return compare_mark_locations(HERE, mark) > 0; }
  // copy one mark to another
  void copy_mark(int dest_mark, int src_mark);
  // set this mark to current item
  void set_mark(int mark) { copy_mark(mark, HERE); }
  // unsets this mark (toplevel only)?
  void unset_mark(int mark);
  // is this toplevel mark set?
  int is_set(int mark);
  void damage_item(int mark = HERE);

  int set_focus();
  void set_top();
  int item_selected() const {return widget->flags()&FL_VALUE;}
  int item_select(int value = 1, int do_callback = 0);
  int item_select_only(int do_callback = 0);
  int item_visible() const {return widget->visible_r();}
  void item_show() {widget->show(); relayout();}
  void item_hide() {widget->hide(); relayout();}

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

  void set_item();
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
  Fl_Widget* widget;
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
// End of "$Id: Fl_Browser.H,v 1.19 2000/08/21 03:56:23 spitzak Exp $".
//
