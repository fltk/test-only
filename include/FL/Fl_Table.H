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
  
  Fl_Table() { /* empty */ }
  
  /*virtual*/ void clear() { 
    ((fltk3::Table*)_p)->clear();
  }

  void table_box(Fl_Boxtype val) {
    ((fltk3::Table*)_p)->table_box(fltk3::_1to3_boxtype(val));
  }

  Fl_Boxtype table_box( void ) {
    return fltk3::_3to1_boxtype( ((fltk3::Table*)_p)->table_box() );
  }

  void rows(int val) {
    ((fltk3::Table*)_p)->rows(val);
  }

  int rows() {
    return ((fltk3::Table*)_p)->rows();
  }

  void cols(int val) {
    ((fltk3::Table*)_p)->cols(val);
  }

  int cols() {
    return ((fltk3::Table*)_p)->cols();
  }

  void visible_cells(int& r1, int& r2, int& c1, int& c2) {
    ((fltk3::Table*)_p)->visible_cells(r1, r2, c1, c2);
  }

  int is_interactive_resize() {
    return ((fltk3::Table*)_p)->is_interactive_resize();
  }

  int row_resize() {
    return ((fltk3::Table*)_p)->row_resize();
  }

  void row_resize(int flag) {
    ((fltk3::Table*)_p)->row_resize(flag);
  }

  int col_resize() {
    return ((fltk3::Table*)_p)->col_resize();
  }

  void col_resize(int flag) {
    ((fltk3::Table*)_p)->col_resize(flag);
  }

  int col_resize_min() {
    return ((fltk3::Table*)_p)->col_resize_min();
  }

  void col_resize_min(int val) {
    ((fltk3::Table*)_p)->col_resize_min(val);
  }

  int row_resize_min() {
    return ((fltk3::Table*)_p)->row_resize_min();
  }

  void row_resize_min(int val) {
    ((fltk3::Table*)_p)->row_resize_min(val);
  }

  int row_header() {
    return ((fltk3::Table*)_p)->row_header();
  }

  void row_header(int flag) {
    ((fltk3::Table*)_p)->row_header();
  }

  int col_header() {
    return ((fltk3::Table*)_p)->col_header();
  }

  void col_header(int flag) {
    ((fltk3::Table*)_p)->col_header(flag);
  }

  void col_header_height(int height) {
    ((fltk3::Table*)_p)->col_header_height(height);
  }

  int col_header_height() {
    return ((fltk3::Table*)_p)->col_header_height();
  }

  void row_header_width(int width) {
    ((fltk3::Table*)_p)->row_header_width(width);
  }

  int row_header_width() {
    return ((fltk3::Table*)_p)->row_header_width();
  }

  void row_header_color(Fl_Color val) {
    ((fltk3::Table*)_p)->row_header_color(fltk3::_1to3_color(val));
  }

  Fl_Color row_header_color() {
    return fltk3::_3to1_color( ((fltk3::Table*)_p)->row_header_color() );
  }

  void col_header_color(Fl_Color val) {
    ((fltk3::Table*)_p)->col_header_color(fltk3::_1to3_color(val));
  }

  inline Fl_Color col_header_color() {
    return fltk3::_3to1_color( ((fltk3::Table*)_p)->col_header_color() );
  }

  void row_height(int row, int height) {
    ((fltk3::Table*)_p)->row_height(row, height);
  }

  int row_height(int row) {
    return ((fltk3::Table*)_p)->row_height(row);
  }

  void col_width(int col, int width) {
    ((fltk3::Table*)_p)->col_width(col, width);
  }

  int col_width(int col) {
    return ((fltk3::Table*)_p)->col_width(col);
  }

  void row_height_all(int height) {
    ((fltk3::Table*)_p)->row_height_all(height);
  }

  void col_width_all(int width) {
    ((fltk3::Table*)_p)->col_width_all(width);
  }

  void row_position(int row) {
    ((fltk3::Table*)_p)->row_position(row);
  }

  void col_position(int col) {
    ((fltk3::Table*)_p)->col_position(col);
  }

  int row_position() {
    return ((fltk3::Table*)_p)->row_position();
  }

  int col_position() {
    return ((fltk3::Table*)_p)->col_position();
  }

#if 0
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
#endif

  int callback_row() {
    return ((fltk3::Table*)_p)->callback_row();
  }

  int callback_col() {
    return ((fltk3::Table*)_p)->callback_col();
  }

  TableContext callback_context() { 
    return (TableContext)((fltk3::Table*)_p)->callback_context();
  }

#if 0
  void do_callback(TableContext context, int row, int col) {
#endif
    
};

#endif /*_FL_TABLE_H*/

//
// End of "$Id$".
//
