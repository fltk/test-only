// "$Id: Browser.h 6077 2008-03-21 00:14:56Z fabien $"
//
// Copyright 2006 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".

#ifndef fltk_Browser_h
#define fltk_Browser_h

#include "Menu.h"
#include "Scrollbar.h"

namespace fltk {

class FL_API Browser : public Menu {
public:

  int handle(int);
  void layout();
  void draw();

  Browser(int X,int Y,int W,int H,const char*l=0);
  static NamedStyle* default_style;
  ~Browser();

  enum { //<! values for type()
    IS_MULTI = 1,
    NORMAL =  GROUP_TYPE,  //!< means single selection can be achieved by user 
    MULTI  =  GROUP_TYPE+1 //!< means multiple selection can be achieved by user
  };
  enum { // value for selected_column
    NO_COLUMN_SELECTED = -1 //!< means that no column has been selected by user
  };

  class FL_API Mark {
    friend class Browser;
    unsigned level; // depth in hierarchy of the item
    unsigned open_level; // depth of highest closed parent
    int position;  // distance in pixels from top of browser
    unsigned indexes_size; // allocated size
    int* indexes;  // array of indexes
    int index0;    // used as *indexes if indexes_size==1
  public:
    Mark() {
      level = 0;
      open_level = 0;
      position = 0;
      indexes_size = 1;
      indexes = &index0;
      index0 = 0;
    }
    Mark(const Mark&);
    ~Mark() {
      if (indexes != &index0) delete[] indexes;
    }
    const Mark& operator=(const Mark&);
    int compare(const Mark& mark2) const;
    void unset() {indexes[0] = -1;}
    bool is_set() const {return indexes[0] >= 0;}
  };

  enum linepos {  //!< Argument to make_item_visible()
    NOSCROLL,	    //!< move as little as possible so item is visible
    TOP,	    //!< position current item to top
    MIDDLE,	    //!< position current item to middle
    BOTTOM	    //!< position current item to bottom
  };

  int width() const {return width_;}
  int height() const {return height_;}
  int box_width() const {return interior.w();}
  int box_height() const {return interior.h();}
  int xposition() const {return xposition_;}
  void xposition(int);
  int yposition() const {return yposition_;}
  void yposition(int);
  bool indented() const {return indented_;}
  void indented(bool v) {indented_ = v;}

  Scrollbar scrollbar;
  Scrollbar hscrollbar;

  Widget* goto_top();
  Widget* goto_focus() {return goto_mark(FOCUS);}
  Widget* goto_position(int y);
  Widget* goto_index(const int* indexes, unsigned level);
  Widget* goto_index(int);
  Widget* goto_index(int,int,int=-1,int=-1,int=-1);
  Widget* goto_mark(const Mark&); // set HERE to mark
  Widget* next();
  Widget* next_visible();
  Widget* previous_visible();
  bool at_mark(const Mark& mark) const {return !HERE.compare(mark);}
  int compare_to_mark(const Mark& mark) const {return HERE.compare(mark);}
  bool item_is_visible() const;
  bool item_is_parent() const;
  bool item_is_open() const;
  int item_h() const;

  bool set_focus();
  void set_mark(Mark& dest) const {dest = HERE;}
  void set_mark_to_focus(Mark& dest) const {dest = FOCUS;}
  bool select(Widget*e, int val, int do_callback=0);
  bool set_item_selected(bool value = true, int do_callback = 0);
  bool select_only_this(int do_callback = 0);
  bool deselect(int do_callback = 0);
  bool make_item_visible(linepos = NOSCROLL);
  void damage_item() {damage_item(HERE);}
  void damage_item(const Mark&); // make this item redraw
  bool set_item_opened(bool);
  bool set_item_visible(bool);

  int current_level() const {return HERE.level;}
  const int* current_index() const {return HERE.indexes;}
  int current_position() const {return HERE.position;}

  int focus_level() const {return FOCUS.level;}
  const int* focus_index() const {return FOCUS.indexes;}
  int focus_position() const {return FOCUS.position;}

  // Maybe these should be moved to base Menu class:
  const int *column_widths() const { return column_widths_p; }
  void column_widths(const int *pWidths);
  const char **column_labels() const { return column_labels_; }
  void column_labels(const char **pLabels);
  int selected_column() { return selected_column_; }
  int set_column_start(int column, int x);

  Widget *header(int col) { if(col<0 || col>=nHeader) return 0; return header_[col]; }
  int nheader() const { return nHeader; }

  // Convienence functions for flat browsers:
  void value(int v) {goto_index(v); set_focus();}
  int value() const {return FOCUS.indexes[0];}
  bool select(int line, bool value = true);
  bool selected(int line);
  int topline() const {return FIRST_VISIBLE.indexes[0];}
  void topline(int line) {goto_index(line); make_item_visible(TOP);}
  void bottomline(int line) {goto_index(line); make_item_visible(BOTTOM);}
  void middleline(int line) {goto_index(line); make_item_visible();}
  bool displayed(int line);
  bool display(int line, bool value = true);

  bool display_lines() const;
  void display_lines(bool display);

  int load(const char *filename);
 
  int multi() const {return type()&IS_MULTI;}

  const Symbol* leaf_symbol() const {return leaf_symbol_;}
  void leaf_symbol(const Symbol* s) {leaf_symbol_ = s;}
  const Symbol* group_symbol() const {return group_symbol_;}
  void group_symbol(const Symbol* s) {group_symbol_ = s;}

protected:
  void handle_callback(int doit); // defines how cb are handled in the browser

private:
  bool displaylines_;
  bool indented_;
  const int *column_widths_; // original column widths
  int *column_widths_i;      // original column widths after user interaction
  int *column_widths_p;      // actual column widths
  const char **column_labels_;
  int xposition_, yposition_;
  int width_, height_;
  int scrolldx, scrolldy;
  static void hscrollbar_cb(Widget*, void*);
  static void scrollbar_cb(Widget*, void*);
  void draw_item(int);
  void draw_clip(const Rectangle&);
  static void draw_clip_cb(void*,const Rectangle&);
  Rectangle interior; // inside box edges and scrollbars
  void set_belowmouse();
  void clear_belowmouse();

  Widget **header_;
  int nHeader, nColumn, selected_column_;
  void set_level(unsigned);
  enum {NUM_REDRAW = 2};
  Mark HERE, FOCUS, FIRST_VISIBLE, REDRAW[NUM_REDRAW], OPEN, BELOWMOUSE;

  Widget* goto_visible_focus(); // set HERE to focus if visible

  int siblings; // # of children of parent of HERE item
  static void column_click_cb_(Widget*, void*);

  const Symbol* leaf_symbol_;
  const Symbol* group_symbol_;
};

}

#endif

//
// End of "$Id: Browser.h 6077 2008-03-21 00:14:56Z fabien $".
//
