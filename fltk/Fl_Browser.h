//
// "$Id: Fl_Browser.h,v 1.2 2001/07/24 16:05:15 robertk Exp $"
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
// Fl_Browser.h and Fl_Browser_.h

#ifndef Fl_Browser_H
#define Fl_Browser_H

#include "Fl_Menu_.h"
#include "Fl_Scrollbar.h"

class FL_API Fl_Browser : public Fl_Menu_ {
  Fl_End endgroup;

public:

  Fl_Browser(int X,int Y,int W,int H,const char*l=0);
  static Fl_Named_Style* default_style;
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
  Fl_Widget* goto_visible_focus(); // set HERE to focus if visible
  Fl_Widget* goto_index(const int* indexes, int level);
  Fl_Widget* goto_number(int n) {return goto_index(&n, 0);}
  Fl_Widget* next(); // set HERE to next widget
  Fl_Widget* next_visible(); // set HERE to next visible item
  Fl_Widget* previous_visible(); // set HERE to previous visible item
  void set_mark(int dest, int mark); // set dest to mark
  int compare_marks(int mark1, int mark2); // returns >0 if mark1 after mark2
  bool at_mark(int mark) { return !compare_marks(HERE,mark); }
  void unset_mark(int mark);  // makes mark have illegal value
  bool is_set(int mark);  // false if unset_mark was called
  void damage_item(int mark); // make this item redraw
  bool set_focus();
  enum linepos { TOP, MIDDLE, BOTTOM };
  void set_position(linepos);
  void set_top() { set_position(TOP); }
  void set_bottom() { set_position(BOTTOM); }
  void set_middle() { set_position(MIDDLE); }
  bool item_select(bool value = true, bool do_callback = false);
  bool item_select_only(bool do_callback = false);
  const int* indexes() const {return item_index[FOCUS];}
  int level() const {return item_level[FOCUS];}

  // fltk 1.0 Fl_Browser emulation: (see also Fl_Menu_.h)
  bool deselect(bool do_callback = 0);
  bool select(int line, bool value = true);
  bool selected(int line);
  void topline(int line) {goto_number(line); set_top();}
  void bottomline(int line) {goto_number(line); set_bottom();}
  void middleline(int line) {goto_number(line); set_middle();}
  char format_char() const {return format_char_;}
  void format_char(char c) {format_char_ = c;}
  char column_char() const {return column_char_;}
  void column_char(char c) {column_char_ = c;}
  const int *column_widths() { return column_widths_; }
  void column_widths(const int *pWidths)  { column_widths_ = pWidths; }
  bool displayed(int line);
  void display(int line, bool value = true);
  int topline() const {return item_index[FIRST_VISIBLE][0];}
  void value(int v) {goto_number(v); set_focus();}
  int value() const {return item_index[FOCUS][0];}

private:
  friend class Fl_Input_Browser;
  char indented_;
  char format_char_;
  char column_char_;
  const int *column_widths_;
  int xposition_, yposition_;
  int scrolldx, scrolldy;
  int last_height, last_max_width, last_num_lines;
  static void hscrollbar_cb(Fl_Widget*, void*);
  static void scrollbar_cb(Fl_Widget*, void*);
  void draw_item();
  void draw_clip(int,int,int,int);
  static void draw_clip_cb(void*,int,int,int,int);
  int X,Y,W,H; // bounding box area

  // For each mark:
  int item_level[NUMMARKS]; // depth in hierarchy of the item
  int item_position[NUMMARKS]; // distance in pixels from top of browser
  int* item_index[NUMMARKS]; // indexes at each level of hierarchy
  int levels; // maximum depth of all items encountered so far
  void set_level(int); // increases levels by reallocating the arrays

};

#define FL_NORMAL_BROWSER Fl_Browser::BOTH
#define FL_MULTI_BROWSER (Fl_Browser::BOTH|Fl_Browser::MULTI_BROWSER)

#ifndef FLTK_2
#define FL_SELECT_BROWSER Fl_Browser::BOTH
#define FL_HOLD_BROWSER Fl_Browser::BOTH
#endif

#endif

//
// End of "$Id: Fl_Browser.h,v 1.2 2001/07/24 16:05:15 robertk Exp $".
//
