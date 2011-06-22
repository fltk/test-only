//
// "$Id$"
//
// Fl_Table -- A table widget
// FLTK 123 wrapper complete
//
// Copyright 2002-2011 by Greg Ercolano.
// Copyright (c) 2004 O'ksi'D
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
// Please report all bugs and problems to "erco at seriss dot com".
//
// TODO:
//    o Auto scroll during dragged selection
//    o Keyboard navigation (up/down/left/right arrow)
//

#ifndef _FL_TABLE_H
#define _FL_TABLE_H

#include <fltk3/Table.h>
#include <sys/types.h>
#include <string.h>		// memcpy
#ifdef WIN32
#include <malloc.h>		// WINDOWS: malloc/realloc
#else /*WIN32*/
#include <stdlib.h>		// UNIX: malloc/realloc
#endif /*WIN32*/

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Scrollbar.H>


class FL_EXPORT Fl_Table : public Fl_Group {
  
public:

  enum TableContext {
    CONTEXT_NONE       = 0,
    CONTEXT_STARTPAGE  = 0x01,
    CONTEXT_ENDPAGE    = 0x02,
    CONTEXT_ROW_HEADER = 0x04,
    CONTEXT_COL_HEADER = 0x08,
    CONTEXT_CELL       = 0x10,
    CONTEXT_TABLE      = 0x20,
    CONTEXT_RC_RESIZE  = 0x40
  };
  
protected:
  
  enum ResizeFlag {
    RESIZE_NONE      = 0,
    RESIZE_COL_LEFT  = 1,
    RESIZE_COL_RIGHT = 2,
    RESIZE_ROW_ABOVE = 3,
    RESIZE_ROW_BELOW = 4
  };
  
#if 0 // TODO: 123 
  void recalc_dimensions();
  void table_resized();
  void table_scrolled();
  void get_bounds(TableContext context, int &X, int &Y, int &W, int &H);
  void change_cursor(Fl_Cursor newcursor);
  TableContext cursor2rowcol(int &R, int &C, ResizeFlag &resizeflag);
  int find_cell(TableContext context, int R, int C, int &X, int &Y, int &W, int &H);
  int row_col_clamp(TableContext context, int &R, int &C);
  virtual void draw_cell(TableContext context, int R=0, int C=0, int X=0, int Y=0, int W=0, int H=0) {}
  long row_scroll_position(int row);
  long col_scroll_position(int col);
  int is_fltk_container();
  static void scroll_cb(Fl_Widget*,void*);	// h/v scrollbar callback
  void damage_zone(int r1, int c1, int r2, int c2, int r3 = 0, int c3 = 0);
  void redraw_range(int toprow, int botrow, int leftcol, int rightcol);
#endif
  
public:
  
  Fl_Table(int X, int Y, int W, int H, const char *l=0) {
    _p = new fltk3::Table(X, Y, W, H, l);
    _p->wrapper(this);
  }
  
#if 0 // TODO: 123 
  virtual void clear() { rows(0); cols(0); }
  void table_box(Fl_Boxtype val);
  Fl_Boxtype table_box( void );
#endif

  void rows(int val) {
    ((Fl_Table*)_p)->rows(val);
  }

  int rows() {
    return ((Fl_Table*)_p)->rows();
  }

  void cols(int val) {
    ((Fl_Table*)_p)->cols(val);
  }

  int cols() {
    return ((Fl_Table*)_p)->cols();
  }

#if 0
  inline void visible_cells(int& r1, int& r2, int& c1, int& c2);
  int is_interactive_resize();
#endif

  int row_resize() {
    return ((Fl_Table*)_p)->row_resize();
  }

  void row_resize(int flag) {
    ((Fl_Table*)_p)->row_resize(flag);
  }

  int col_resize() {
    return ((Fl_Table*)_p)->col_resize();
  }

  void col_resize(int flag) {
    ((Fl_Table*)_p)->col_resize(flag);
  }

#if 0
  inline int col_resize_min();
  void col_resize_min(int val);
  inline int row_resize_min();
  void row_resize_min(int val);
#endif

  int row_header() {
    return ((Fl_Table*)_p)->row_header();
  }

  void row_header(int flag) {
    ((Fl_Table*)_p)->row_header();
  }

  int col_header() {
    return ((Fl_Table*)_p)->col_header();
  }

  void col_header(int flag) {
    ((Fl_Table*)_p)->col_header(flag);
  }

#if 0
  inline void col_header_height(int height);
  inline int col_header_height();
  inline void row_header_width(int width);
  inline int row_header_width();
#endif

  void row_header_color(Fl_Color val) {
    ((Fl_Table*)_p)->row_header_color(fltk3::_1to3_color(val));
  }

  Fl_Color row_header_color() {
    return fltk3::_3to1_color( ((Fl_Table*)_p)->row_header_color() );
  }

#if 0
  inline void col_header_color(Fl_Color val);
  inline Fl_Color col_header_color();
  void row_height(int row, int height);
  inline int row_height(int row);
  void col_width(int col, int width);
  inline int col_width(int col);
#endif

  void row_height_all(int height) {
    ((Fl_Table*)_p)->row_height_all(height);
  }

  void col_width_all(int width) {
    ((Fl_Table*)_p)->col_width_all(width);
  }

#if 0
  void row_position(int row);			// set/get table's current scroll position
  void col_position(int col);
  int row_position() {				// current row position
  int col_position() {				// current col position
  inline void top_row(int row) {		// set/get top row (deprecated)
  inline int top_row() {
  int is_selected(int r, int c);		// selected cell
  void get_selection(int &row_top, int &col_left, int &row_bot, int &col_right);
  void set_selection(int row_top, int col_left, int row_bot, int col_right);
  int move_cursor(int R, int C);
  void resize(int X, int Y, int W, int H);	// fltk resize() override
  void draw(void);				// fltk draw() override
  void init_sizes() {
  void add(Fl_Widget& w) {
  void add(Fl_Widget* w) {
  void insert(Fl_Widget& w, int n) {
  void insert(Fl_Widget& w, Fl_Widget* w2) {
  void remove(Fl_Widget& w) {
  void begin() {
  void end() {
  Fl_Widget * const *array() {
  Fl_Widget *child(int n) const {
  int children() const {
  int find(const Fl_Widget *w) const {
  int find(const Fl_Widget &w) const {
  int callback_row() {
  int callback_col() {
  TableContext callback_context() {
  void do_callback(TableContext context, int row, int col) {
#endif
    
};

#endif /*_FL_TABLE_H*/

//
// End of "$Id$".
//
