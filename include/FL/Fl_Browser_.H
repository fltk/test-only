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


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Browser_, Browser_)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Browser_, Browser_)
FLTK3_WRAPPER_INTERFACE_END()


namespace fltk3 { class Browser; }
inline fltk3::Browser *fl_new_browser(int, int, int, int, const char*);

class FL_EXPORT Fl_Browser_ : public Fl_Group {

protected:

  FLTK3_WRAPPER_VCALLS_OBJECT_RET(void *, Browser__I, item_first() const, item_first(), BrowserItemFirst)
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(void *, Browser__I, item_next(void *item) const, item_next(item), BrowserItemNext);
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(void *, Browser__I, item_prev(void *item) const, item_prev(item), BrowserItemPrev);
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(void*, Browser__I, item_last() const, item_last(), BrowserItemLast)  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Browser__I, item_height(void *item) const, item_height(item), BrowserItemHeight)  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Browser__I, item_width(void *item) const, item_width(item), BrowserItemWidth)  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Browser__I, item_quick_height(void *item) const, item_quick_height(item), BrowserItemQuickHeight)  
  FLTK3_WRAPPER_VCALLS_OBJECT(Browser__I, item_draw(void *item, int X, int Y, int W, int H) const, item_draw(item, X, Y, W, H), BrowserItemDraw)  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(const char *, Browser__I, item_text(void *item) const, item_text(item), BrowserItemText)  
  FLTK3_WRAPPER_VCALLS_OBJECT(Browser__I, item_swap(void *a, void *b), item_swap(a, b), BrowserItemSwap)  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(void *, Browser__I, item_at(int index) const, item_at(index), BrowserItemAt)  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Browser__I, full_width() const, full_width(), BrowserFullWidth)  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Browser__I, full_height() const, full_height(), BrowserFullHeight)  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Browser__I, incr_height() const, incr_height(), BrowserIncrHeight)  
  FLTK3_WRAPPER_VCALLS_OBJECT(Browser__I, item_select(void *item, int val=1), item_select(item, val), BrowserItemSelect)  
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Browser__I, item_selected(void *item) const, item_selected(item), BrowserItemSelected)  
  
  void *top() const {
    return ((fltk3::Browser__I*)_p)->top();
  }
  
  void *selection() const { 
    return ((fltk3::Browser__I*)_p)->selection();
  }
  
  void new_list() {
    ((fltk3::Browser__I*)_p)->new_list();
  }
  
  void deleting(void *item) {
    ((fltk3::Browser__I*)_p)->deleting(item);
  }
  
  void replacing(void *a,void *b) {
    ((fltk3::Browser__I*)_p)->replacing(a, b);
  }
  
  void swapping(void *a,void *b) {
    ((fltk3::Browser__I*)_p)->swapping(a, b);
  }
  
  void inserting(void *a,void *b) {
    ((fltk3::Browser__I*)_p)->inserting(a, b);
  }
  
  int displayed(void *item) const {
    return ((fltk3::Browser__I*)_p)->displayed(item);
  }
  
  void redraw_line(void *item) {
    ((fltk3::Browser__I*)_p)->redraw_line(item);
  }
  
  void redraw_lines() { 
    ((fltk3::Browser__I*)_p)->redraw_lines();
  }
  
  void bbox(int &X,int &Y,int &W,int &H) const {
    ((fltk3::Browser__I*)_p)->bbox(X, Y, W, H);
  }
  
  int leftedge() const {
    return ((fltk3::Browser__I*)_p)->leftedge();
  }
  
  void *find_item(int ypos) {
    return ((fltk3::Browser__I*)_p)->find_item(ypos);
  }
  
  Fl_Browser_() { /* empty */ }

  Fl_Browser_(int x, int y, int w, int h, const char *label=0) {
    _p = new fltk3::Browser__I(x, y, w, h, label);
    _p->wrapper(this);
  }
 
public:

  FLTK3_WIDGET_VCALLS(Fl_Browser_, Browser_)

  // FIXME: 123 Fl_Scrollbar scrollbar;
  
  // FIXME: 123 Fl_Scrollbar hscrollbar;
  
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
