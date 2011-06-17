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

#include <fltk3/Browser_.h>
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

  // FIXME: 123 virtual methods
  virtual void *item_first() const { return 0; }
  virtual void *item_next(void *item) const { return 0; }
  virtual void *item_prev(void *item) const { return 0; }
  virtual void *item_last() const { return 0L; }
  virtual int item_height(void *item) const { return 0; }
  virtual int item_width(void *item) const { return 0; }
  virtual int item_quick_height(void *item) const { return 0; }
  virtual void item_draw(void *item,int X,int Y,int W,int H) const {}
  virtual const char *item_text(void *item) const { (void)item; return 0L; }
  virtual void item_swap(void *a,void *b) { (void)a; (void)b; }
  virtual void *item_at(int index) const { (void)index; return 0L; }
  virtual int full_width() const { return 0; }
  virtual int full_height() const { return 0; }
  virtual int incr_height() const { return 0; }
  virtual void item_select(void *item,int val=1) { }
  virtual int item_selected(void *item) const { return 0; }
  
  void *top() const {
    return ((fltk3::Browser_*)_p)->top();
  }
  
  void *selection() const { 
    return ((fltk3::Browser_*)_p)->selection();
  }
  
  void new_list() {
    ((fltk3::Browser_*)_p)->new_list();
  }
  
  void deleting(void *item) {
    ((fltk3::Browser_*)_p)->deleting(item);
  }
  
  void replacing(void *a,void *b) {
    ((fltk3::Browser_*)_p)->replacing(a, b);
  }
  
  void swapping(void *a,void *b) {
    ((fltk3::Browser_*)_p)->swapping(a, b);
  }
  
  void inserting(void *a,void *b) {
    ((fltk3::Browser_*)_p)->inserting(a, b);
  }
  
  int displayed(void *item) const {
    return ((fltk3::Browser_*)_p)->displayed(item);
  }
  
  void redraw_line(void *item) {
    ((fltk3::Browser_*)_p)->redraw_line(item);
  }
  
  void redraw_lines() { 
    ((fltk3::Browser_*)_p)->redraw_lines();
  }
  
  void bbox(int &X,int &Y,int &W,int &H) const {
    ((fltk3::Browser_*)_p)->bbox(X, Y, W, H);
  }
  
  int leftedge() const {
    return ((fltk3::Browser_*)_p)->leftedge();
  }
  
  void *find_item(int ypos) {
    return ((fltk3::Browser_*)_p)->find_item(ypos);
  }
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Browser_, draw(), draw(), Draw)
  
  Fl_Browser_() { /* empty */ }
 
public:

  // FIXME: 123 Fl_Scrollbar scrollbar;
  
  // FIXME: 123 Fl_Scrollbar hscrollbar;
  
  FLTK3_WRAPPER_VCALLS_OBJECT(Browser_, resize(int x, int y, int w, int h), resize(x, y, w, h), Resize)

  FLTK3_WRAPPER_VCALLS_OBJECT_INT(Browser_, handle(int event), handle(event), Handle)

  int select(void *item,int val=1,int docallbacks=0) {
    return ((fltk3::Browser_*)_p)->select(item, val, docallbacks);
  }
  
  int select_only(void *item,int docallbacks=0) {
    return ((fltk3::Browser_*)_p)->select_only(item, docallbacks);
  }
  
  int deselect(int docallbacks=0) {
    return ((fltk3::Browser_*)_p)->deselect(docallbacks);
  }
  
  int position() const {
    return ((fltk3::Browser_*)_p)->position();
  }
  
  void position(int pos) {
    ((fltk3::Browser_*)_p)->position(pos);
  }
  
  int hposition() const { 
    return ((fltk3::Browser_*)_p)->hposition();
  }
  
  void hposition(int p) {
    ((fltk3::Browser_*)_p)->hposition(p);
  }
  
  void display(void *item) {
    ((fltk3::Browser_*)_p)->display(item);
  }
  
  enum {
    HORIZONTAL = 1,
    VERTICAL = 2,
    BOTH = 3,
    ALWAYS_ON = 4,
    HORIZONTAL_ALWAYS = 5,
    VERTICAL_ALWAYS = 6,
    BOTH_ALWAYS = 7
  };
  
  uchar has_scrollbar() const { 
    return ((fltk3::Browser_*)_p)->has_scrollbar();
  }
  
  void has_scrollbar(uchar mode) {
    ((fltk3::Browser_*)_p)->has_scrollbar(mode);
  }
  
  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::Browser_*)_p)->textfont() ); 
  }
                             
  void textfont(Fl_Font font) { 
    ((fltk3::Browser_*)_p)->textfont( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::Browser_*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::Browser_*)_p)->textsize( fltk3::_1to3_fontsize(size) );
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::Browser_*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::Browser_*)_p)->textcolor( fltk3::_1to3_color(col) );
  }
  
  int scrollbar_size() const {
    return ((fltk3::Browser_*)_p)->scrollbar_size();
  }
  
  void scrollbar_size(int size) {
    ((fltk3::Browser_*)_p)->scrollbar_size(size);
  }
  
  int scrollbar_width() const {
    return ((fltk3::Browser_*)_p)->scrollbar_width();
  }
  
  void scrollbar_width(int width) {
    ((fltk3::Browser_*)_p)->scrollbar_width(width);
  }
  
  void scrollbar_right() { 
    ((fltk3::Browser_*)_p)->scrollbar_right();
  }
  
  void scrollbar_left() {
    ((fltk3::Browser_*)_p)->scrollbar_left();
  }
  
  void sort(int flags=0) {
    ((fltk3::Browser_*)_p)->sort(flags);
  }
};

#endif

//
// End of "$Id$".
//
