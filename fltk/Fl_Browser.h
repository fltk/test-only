//
// "$Id: Fl_Browser.h,v 1.5 2002/01/11 08:49:07 spitzak Exp $"
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

#ifndef Fl_Browser_H
#define Fl_Browser_H

#include "Fl_Menu_.h"
#include "Fl_Scrollbar.h"

class FL_API Fl_Browser : public Fl_Menu_ {
  Fl_End endgroup;

public:

  int handle(int);
  void layout();
  void draw();

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

  int width() const {return width_;}
  int height() const {return height_;}
  int box_width() const {return W;}
  int box_height() const {return H;}
  int xposition() const {return xposition_;}
  void xposition(int);
  int yposition() const {return yposition_;}
  void yposition(int);
  bool indented() const {return indented_;}
  void indented(bool v) {indented_ = v;}

  Fl_Scrollbar scrollbar;
  Fl_Scrollbar hscrollbar;

  Fl_Widget* goto_top();
  Fl_Widget* goto_focus() {return goto_mark(FOCUS);}
  Fl_Widget* goto_position(int y);
  Fl_Widget* goto_index(const int* indexes, int level);
  Fl_Widget* goto_index(int);
  Fl_Widget* goto_index(int,int,int=-1,int=-1,int=-1);
  Fl_Widget* next();
  Fl_Widget* next_visible();
  Fl_Widget* previous_visible();

  bool set_focus();
  void set_top() { set_position(TOP); }
  void set_bottom() { set_position(BOTTOM); }
  void set_middle() { set_position(MIDDLE); }
  bool set_selected(bool value = true, int do_callback = 0);
  bool select_only_this(int do_callback = 0);
  bool deselect(int do_callback = 0);

  int current_level() const {return item_level[HERE];}
  const int* current_index() const {return item_index[HERE];}
  int current_position() const {return item_position[HERE];}

  int focus_level() const {return item_level[FOCUS];}
  const int* focus_index() const {return item_index[FOCUS];}
  int focus_position() const {return item_position[FOCUS];}
  void value(int v) {goto_index(v); set_focus();}
  int value() const {return focus_index()[0];}

  // Maybe these should be moved to base Fl_Menu_ class:
  char format_char() const {return format_char_;}
  void format_char(char c) {format_char_ = c;}
  const int *column_widths() const { return column_widths_; }
  void column_widths(const int *pWidths)  { column_widths_ = pWidths; }

  // fltk 1.0 Fl_Browser emulation: (see also Fl_Menu_.h)
  bool select(int line, bool value = true);
  bool selected(int line);
  void topline(int line) {goto_index(line); set_top();}
  void bottomline(int line) {goto_index(line); set_bottom();}
  void middleline(int line) {goto_index(line); set_middle();}
  bool displayed(int line);
  void display(int line, bool value = true);
  int topline() const {return item_index[FIRST_VISIBLE][0];}

private:

  bool indented_;
  char format_char_;
  const int *column_widths_;
  int xposition_, yposition_;
  int width_, height_;
  int scrolldx, scrolldy;
  static void hscrollbar_cb(Fl_Widget*, void*);
  static void scrollbar_cb(Fl_Widget*, void*);
  void draw_item();
  void draw_clip(int,int,int,int);
  static void draw_clip_cb(void*,int,int,int,int);
  int X,Y,W,H; // bounding box area

  int multi() const {return type()&MULTI_BROWSER;}

  // Marks serve as "indexes" into the hierarchial browser. We probably
  // need to make some sort of public interface but I have not figured
  // it out completely.
  enum { // predefined marks
    HERE = 0, // current item, the one moved by all the calls
    FOCUS,
    FIRST_VISIBLE,
    REDRAW_0,
    REDRAW_1,
    TEMP,
    NUMMARKS
  };
  Fl_Widget* goto_mark(int mark); // set HERE to mark
  Fl_Widget* goto_visible_focus(); // set HERE to focus if visible
  void set_mark(int dest, int mark); // set dest to mark
  int compare_marks(int mark1, int mark2); // returns >0 if mark1 after mark2
  bool at_mark(int mark) { return !compare_marks(HERE,mark); }
  void unset_mark(int mark);  // makes mark have illegal value
  bool is_set(int mark);  // false if unset_mark was called
  void damage_item(int mark); // make this item redraw
  bool item_open(bool); // set open/close state of this item, if it is a parent
  enum linepos { TOP, MIDDLE, BOTTOM };
  void set_position(linepos);

  // For each mark:
  int item_level[NUMMARKS]; // depth in hierarchy of the item
  int item_position[NUMMARKS]; // distance in pixels from top of browser
  int* item_index[NUMMARKS]; // indexes at each level of hierarchy
  int levels; // maximum depth of all items encountered so far
  void set_level(int); // increases levels by reallocating the arrays

};

#ifndef FLTK_2
#define FL_NORMAL_BROWSER Fl_Browser::BOTH
#define FL_MULTI_BROWSER (Fl_Browser::BOTH|Fl_Browser::MULTI_BROWSER)
#define FL_SELECT_BROWSER Fl_Browser::BOTH
#define FL_HOLD_BROWSER Fl_Browser::BOTH
#endif

#endif

//
// End of "$Id: Fl_Browser.h,v 1.5 2002/01/11 08:49:07 spitzak Exp $".
//
