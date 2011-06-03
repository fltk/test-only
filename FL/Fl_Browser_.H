//
// "$Id$"
//
// Common browser header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Browser__H
#define Fl_Browser__H

#include "Fl_Group.H"
#include "Fl_Scrollbar.H"
#include "Fl.H"

#define FL_NORMAL_BROWSER	0	/**< type() of Fl_Browser */
#define FL_SELECT_BROWSER	1	/**< type() of FL_Select_Browser */
#define FL_HOLD_BROWSER		2	/**< type() of Fl_Hold_Browser */
#define FL_MULTI_BROWSER	3	/**< type() of Fl_Multi_Browser */

#define FL_SORT_ASCENDING	0	/**< sort browser items in ascending alphabetic order. */
#define FL_SORT_DESCENDING	1	/**< sort in descending order */

class FL_EXPORT Fl_Browser_ : public Fl_Group {

protected:

  virtual void *item_first() const = 0;
  virtual void *item_next(void *item) const = 0;
  virtual void *item_prev(void *item) const = 0;
  virtual void *item_last() const { return 0L; }
  virtual int item_height(void *item) const = 0;
  virtual int item_width(void *item) const = 0;
  virtual int item_quick_height(void *item) const ;
  virtual void item_draw(void *item,int X,int Y,int W,int H) const = 0;
  virtual const char *item_text(void *item) const { (void)item; return 0L; }
  virtual void item_swap(void *a,void *b) { (void)a; (void)b; }
  virtual void *item_at(int index) const { (void)index; return 0L; }
  virtual int full_width() const ;	// current width of all items
  virtual int full_height() const ;	// current height of all items
  virtual int incr_height() const ;	// average height of an item
  virtual void item_select(void *item,int val=1);
  virtual int item_selected(void *item) const ;
  void *top() const { return top_; }
  void *selection() const { return selection_; }
  void new_list(); // completely clobber all data, as though list replaced
  void deleting(void *item); // get rid of any pointers to item
  void replacing(void *a,void *b); // change a pointers to b
  void swapping(void *a,void *b); // exchange pointers a and b
  void inserting(void *a,void *b); // insert b near a
  int displayed(void *item) const ; // true if this item is visible
  void redraw_line(void *item); // minimal update, no change in size
  void redraw_lines() { damage(FL_DAMAGE_SCROLL); } // redraw all of them
  void bbox(int &X,int &Y,int &W,int &H) const;
  int leftedge() const;	// x position after scrollbar & border
  void *find_item(int ypos); // item under mouse
  void draw();
  Fl_Browser_(int X,int Y,int W,int H,const char *L=0);

public:

  Fl_Scrollbar scrollbar;
  Fl_Scrollbar hscrollbar;

  int handle(int event);
  void resize(int X,int Y,int W,int H);
  int select(void *item,int val=1,int docallbacks=0);
  int select_only(void *item,int docallbacks=0);
  int deselect(int docallbacks=0);
  int position() const { return position_; }
  void position(int pos); // scroll to here
  int hposition() const { return hposition_; }
  void hposition(int); // pan to here
  void display(void *item); // scroll so this item is shown
  enum { // values for has_scrollbar()
    HORIZONTAL = 1,		///< Only show horizontal scrollbar.
    VERTICAL = 2,		///< Only show vertical scrollbar.
    BOTH = 3,			///< Show both scrollbars. (default)
    ALWAYS_ON = 4,		///< Specified scrollbar(s) should 'always' be shown (to be used with HORIZONTAL/VERTICAL)
    HORIZONTAL_ALWAYS = 5,	///< Horizontal scrollbar always on.
    VERTICAL_ALWAYS = 6,	///< Vertical scrollbar always on.
    BOTH_ALWAYS = 7		///< Both scrollbars always on.
  };
  uchar has_scrollbar() const { return has_scrollbar_; }
  void has_scrollbar(uchar mode) { has_scrollbar_ = mode; }
  Fl_Font textfont() const { return textfont_; }
  void textfont(Fl_Font font) { textfont_ = font; }
  Fl_Fontsize textsize() const { return textsize_; }
  void textsize(Fl_Fontsize size) { textsize_ = size; }
  Fl_Color textcolor() const { return textcolor_; }
  void textcolor(Fl_Color col) { textcolor_ = col; }
  int scrollbar_size() const;
  void scrollbar_size(int size);
  int scrollbar_width() const;
  void scrollbar_width(int width);
  void scrollbar_right() { scrollbar.align(FL_ALIGN_RIGHT); }
  void scrollbar_left() { scrollbar.align(FL_ALIGN_LEFT); }
  void sort(int flags=0);
};

#endif

//
// End of "$Id$".
//
