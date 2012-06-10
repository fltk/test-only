//
// "$Id$"
//
// Header file for Fl_Text_Display class.
//
// Copyright 2001-2012 by Bill Spitzak and others.
// Original code Copyright Mark Edel.  Permission to distribute under
// the LGPL for the FLTK library granted by Mark Edel.
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

#ifndef FL_TEXT_DISPLAY_H
#define FL_TEXT_DISPLAY_H

#include <fltk3/TextDisplay.h>
#include "Fl_Text_Buffer.H"
#include "Fl_Group.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Text_Display, TextDisplay)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Text_Display, TextDisplay)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Text_Display: public Fl_Group {

public:
  
  enum {
    NORMAL_CURSOR,
    CARET_CURSOR,
    DIM_CURSOR,
    BLOCK_CURSOR,
    HEAVY_CURSOR
  };
  
  enum {
    CURSOR_POS, 
    CHARACTER_POS
  };
  
  enum {
    DRAG_NONE = -2,
    DRAG_START_DND = -1,
    DRAG_CHAR = 0, 
    DRAG_WORD = 1, 
    DRAG_LINE = 2
  };
  
  /**
   wrap types - used in wrap_mode()
   */
  enum {
    WRAP_NONE,      /**< don't wrap text at all */
    WRAP_AT_COLUMN, /**< wrap text at the given text column */
    WRAP_AT_PIXEL,  /**< wrap text at a pixel position */
    WRAP_AT_BOUNDS  /**< wrap text so that it fits into the widget width */
  };    
  
  typedef fltk3::TextDisplay::UnfinishedStyleCb Unfinished_Style_Cb;
  
  struct Style_Table_Entry {
    Fl_Color    color;
    Fl_Font     font;
    Fl_Fontsize size;
    unsigned    attr;
  };
  
  FLTK3_WIDGET_VCALLS(Fl_Text_Display, TextDisplay)

  Fl_Text_Display() {}
  
  Fl_Text_Display(int X, int Y, int W, int H, const char *l = 0) {
    _p = new fltk3::TextDisplay_I(_ctor_dx(X), _ctor_dy(Y), W, H, l);
    _p->wrapper(this);
  }
    
  void buffer(Fl_Text_Buffer* buf) {
    ((fltk3::TextDisplay*)_p)->buffer(buf);
  }
  
  void buffer(Fl_Text_Buffer& buf) { 
    ((fltk3::TextDisplay*)_p)->buffer(buf);
  }
  
  Fl_Text_Buffer* buffer() const { 
    return (Fl_Text_Buffer*) ((fltk3::TextDisplay*)_p)->buffer();
  }
  
  void styleBuffer(Fl_Text_Buffer* buf) {
    ((fltk3::TextDisplay_I*)_p)->mStyleBuffer = buf;
  }
  
  Fl_Text_Buffer* styleBuffer() const { 
    return (Fl_Text_Buffer*) ((fltk3::TextDisplay_I*)_p)->mStyleBuffer;
  }
  
  void redisplay_range(int start, int end) {
    ((fltk3::TextDisplay*)_p)->redisplay_range(start, end);
  }
  
  void scroll(int topLineNum, int horizOffset) {
    ((fltk3::TextDisplay*)_p)->scroll(topLineNum, horizOffset);
  }
  
  void insert(const char* text) {
    ((fltk3::TextDisplay*)_p)->insert(text);
  }
  
  void overstrike(const char* text) {
    ((fltk3::TextDisplay*)_p)->overstrike(text);
  }
  
  void insert_position(int newPos) {
    ((fltk3::TextDisplay*)_p)->insert_position(newPos);
  }
  
  int insert_position() const { 
    return ((fltk3::TextDisplay*)_p)->insert_position();
  }
  
  int position_to_xy(int pos, int* x, int* y) const {
    return ((fltk3::TextDisplay*)_p)->position_to_xy(pos, x, y);
  }

  int in_selection(int x, int y) const {
    return ((fltk3::TextDisplay*)_p)->in_selection(x, y);
  }
  
  void show_insert_position() {
    ((fltk3::TextDisplay*)_p)->show_insert_position();
  }
  
  int move_right() {
    return ((fltk3::TextDisplay*)_p)->move_right();
  }
  
  int move_left() {
    return ((fltk3::TextDisplay*)_p)->move_left();
  }
                                                  
  int move_up() {
    return ((fltk3::TextDisplay*)_p)->move_up();
  }
  
  int move_down() {
    return ((fltk3::TextDisplay*)_p)->move_down();
  }
  
  int count_lines(int start, int end, bool s) const {
    return ((fltk3::TextDisplay*)_p)->count_lines(start, end, s);
  }
  
  int line_start(int pos) const {
    return ((fltk3::TextDisplay*)_p)->line_start(pos);
  }
  
  int line_end(int startPos, bool s) const {
    return ((fltk3::TextDisplay*)_p)->line_end(startPos, s);
  }
  
  int skip_lines(int startPos, int nLines, bool s) {
    return ((fltk3::TextDisplay*)_p)->skip_lines(startPos, nLines, s);
  }
  
  int rewind_lines(int startPos, int nLines) {
    return ((fltk3::TextDisplay*)_p)->rewind_lines(startPos, nLines);
  }
  
  void next_word(void) {
    ((fltk3::TextDisplay*)_p)->next_word();
  }
  
  void previous_word(void) {
    ((fltk3::TextDisplay*)_p)->previous_word();
  }
  
  void show_cursor(int b = 1) {
    ((fltk3::TextDisplay*)_p)->show_cursor(b);
  }
  
  void hide_cursor() {
    ((fltk3::TextDisplay*)_p)->hide_cursor();
  }
  
  void cursor_style(int style) {
    ((fltk3::TextDisplay*)_p)->cursor_style(style);
  }
  
  Fl_Color cursor_color() const {
    return fltk3::_3to1_color( ((fltk3::TextDisplay*)_p)->cursor_color() );
  }
  
  void cursor_color(Fl_Color n) {
    ((fltk3::TextDisplay*)_p)->cursor_color(fltk3::_1to3_color(n));
  }
  
  int scrollbar_width() const { 
    return ((fltk3::TextDisplay*)_p)->scrollbar_width();
  }
  
  void scrollbar_width(int W) {
    ((fltk3::TextDisplay*)_p)->scrollbar_width(W);
  }
  
  Fl_Align scrollbar_align() const { 
    return fltk3::_3to1_align( ((fltk3::TextDisplay*)_p)->scrollbar_align() );
  }
  
  void scrollbar_align(Fl_Align a) { 
    ((fltk3::TextDisplay*)_p)->scrollbar_align( fltk3::_1to3_align(a) );
  }
  
  int word_start(int pos) const { 
    return ((fltk3::TextDisplay*)_p)->word_start(pos);
  }
  
  int word_end(int pos) const { 
    return ((fltk3::TextDisplay*)_p)->word_end(pos); 
  }
  
  void highlight_data(Fl_Text_Buffer *styleBuffer,
                      const Style_Table_Entry *styleTable,
                      int nStyles, char unfinishedStyle,
                      Unfinished_Style_Cb unfinishedHighlightCB,
                      void *cbArg) {
    ((fltk3::TextDisplay*)_p)->highlight_data((fltk3::TextBuffer*)styleBuffer,
                                              (fltk3::TextDisplay::StyleTableEntry*)styleTable,
                                              nStyles, unfinishedStyle,
                                              (fltk3::TextDisplay::UnfinishedStyleCb)unfinishedHighlightCB,
                                              cbArg);
  }
  
  int position_style(int lineStartPos, int lineLen, int lineIndex) const {
    return ((fltk3::TextDisplay*)_p)->position_style(lineStartPos, lineLen, lineIndex);
  }
  
  int shortcut() const {
    return ((fltk3::TextDisplay*)_p)->shortcut();
  }
  
  void shortcut(int s) {
    ((fltk3::TextDisplay*)_p)->shortcut(s);
  }
  
  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::TextDisplay*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::TextDisplay*)_p)->textfont( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::TextDisplay*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::TextDisplay*)_p)->textsize( fltk3::_1to3_fontsize(size) );
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::TextDisplay*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::TextDisplay*)_p)->textcolor( fltk3::_1to3_color(col) );
  }
  
  int wrapped_column(int row, int column) const {
    return ((fltk3::TextDisplay*)_p)->wrapped_column(row, column);
  }
  
  int wrapped_row(int row) const {
    return ((fltk3::TextDisplay*)_p)->wrapped_row(row);
  }
  
  void wrap_mode(int wrap, int wrap_margin) {
    ((fltk3::TextDisplay*)_p)->wrap_mode(wrap, wrap_margin);
  }
  
  double x_to_col(double x) const {
    return ((fltk3::TextDisplay*)_p)->x_to_col(x);
  }
  
  double col_to_x(double col) const {
    return ((fltk3::TextDisplay*)_p)->col_to_x(col);
  }
  
protected:
  
  void draw_text(int X, int Y, int W, int H) {
    ((fltk3::TextDisplay_I*)_p)->draw_text(X, Y, W, H);
  }
  
  void draw_range(int start, int end) {
    ((fltk3::TextDisplay_I*)_p)->draw_range(start, end);
  }
  
  void draw_cursor(int a, int b) {
    ((fltk3::TextDisplay_I*)_p)->draw_cursor(a, b);
  }
  
  void draw_string(int style, int x, int y, int toX, const char *string, int nChars) const {
    ((fltk3::TextDisplay_I*)_p)->draw_string(style, x, y, toX, string, nChars);
  }
  
  void draw_vline(int visLineNum, int leftClip, int rightClip,
                  int leftCharIndex, int rightCharIndex) {
    ((fltk3::TextDisplay_I*)_p)->draw_vline(visLineNum, leftClip, rightClip,
                                          leftCharIndex, rightCharIndex);
  }
  
  int find_x(const char *s, int len, int style, int x) const {
    return ((fltk3::TextDisplay_I*)_p)->find_x(s, len, style, x);
  }
  
  enum { 
    DRAW_LINE, 
    FIND_INDEX,
    FIND_INDEX_FROM_ZERO,
    GET_WIDTH 
  };
  
  int handle_vline(int mode, 
                   int lineStart, int lineLen, int leftChar, int rightChar,
                   int topClip, int bottomClip,
                   int leftClip, int rightClip) const {
    return ((fltk3::TextDisplay_I*)_p)->handle_vline(mode, 
                                            lineStart, lineLen, leftChar, rightChar,
                                            topClip, bottomClip,
                                            leftClip, rightChar);
  }
  
  void draw_line_numbers(bool clearAll) {
    ((fltk3::TextDisplay_I*)_p)->draw_line_numbers(clearAll);
  }
  
  void clear_rect(int style, int x, int y, int width, int height) const {
    ((fltk3::TextDisplay_I*)_p)->clear_rect(style, x, y, width, height);
  }
  
  void display_insert() {
    ((fltk3::TextDisplay_I*)_p)->display_insert();
  }
  
  void offset_line_starts(int newTopLineNum) {
    ((fltk3::TextDisplay_I*)_p)->offset_line_starts(newTopLineNum);
  }
  
  void calc_line_starts(int startLine, int endLine) {
    ((fltk3::TextDisplay_I*)_p)->calc_line_starts(startLine, endLine);
  }
  
  void update_line_starts(int pos, int charsInserted, int charsDeleted,
                          int linesInserted, int linesDeleted, int *scrolled) {
    ((fltk3::TextDisplay_I*)_p)->update_line_starts(pos, charsInserted, charsDeleted,
                                                  linesInserted, linesDeleted, scrolled);
  }
  
  void calc_last_char() {
    ((fltk3::TextDisplay_I*)_p)->calc_last_char();
  }
  
  int position_to_line( int pos, int* lineNum ) const {
    return ((fltk3::TextDisplay_I*)_p)->position_to_line(pos, lineNum);
  }
  
  double string_width(const char* string, int length, int style) const {
    return ((fltk3::TextDisplay_I*)_p)->string_width(string, length, style);
  }
  
  static void scroll_timer_cb(void *v) {
    fltk3::TextDisplay_I::scroll_timer_cb(v);
  }
  
  static void buffer_predelete_cb(int pos, int nDeleted, void* cbArg) {
    fltk3::TextDisplay_I::buffer_predelete_cb(pos, nDeleted, cbArg);
  }
  
  static void buffer_modified_cb(int pos, int nInserted, int nDeleted,
                                 int nRestyled, const char* deletedText,
                                 void* cbArg) {
    fltk3::TextDisplay_I::buffer_modified_cb(pos, nInserted, nDeleted,
                                          nRestyled, deletedText,
                                          cbArg);
  }
  
  
  // FIXME: 123-1 - static void h_scrollbar_cb(Fl_Scrollbar* w, Fl_Text_Display* d);
  // FIXME: 123-1 - static void v_scrollbar_cb( Fl_Scrollbar* w, Fl_Text_Display* d) {
    
  void update_v_scrollbar() {
    ((fltk3::TextDisplay_I*)_p)->update_v_scrollbar();
  }
  
  void update_h_scrollbar() {
    ((fltk3::TextDisplay_I*)_p)->update_h_scrollbar();
  }
  
  int measure_vline(int visLineNum) const {
    return ((fltk3::TextDisplay_I*)_p)->measure_vline(visLineNum);
  }
  
  int longest_vline() const { 
    return ((fltk3::TextDisplay_I*)_p)->longest_vline();
  }
  
  int empty_vlines() const {
    return ((fltk3::TextDisplay_I*)_p)->empty_vlines();
  }
  
  int vline_length(int visLineNum) const {
    return ((fltk3::TextDisplay_I*)_p)->vline_length(visLineNum);
  }
  
  int xy_to_position(int x, int y, int PosType = CHARACTER_POS) const {
    return ((fltk3::TextDisplay_I*)_p)->xy_to_position(x, y, PosType);
  }
  
  void xy_to_rowcol(int x, int y, int* row, int* column, int PosType = CHARACTER_POS) const {
    ((fltk3::TextDisplay_I*)_p)->xy_to_rowcol(x, y, row, column, PosType);
  }
  
  void maintain_absolute_top_line_number(int state) {
    ((fltk3::TextDisplay_I*)_p)->maintain_absolute_top_line_number(state);
  }
  
  int get_absolute_top_line_number() const {
    return ((fltk3::TextDisplay_I*)_p)->get_absolute_top_line_number();
  }
  
  void absolute_top_line_number(int oldFirstChar) {
    ((fltk3::TextDisplay_I*)_p)->absolute_top_line_number(oldFirstChar);
  }
  
  int maintaining_absolute_top_line_number() const {
    return ((fltk3::TextDisplay_I*)_p)->maintaining_absolute_top_line_number();
  }
  
  void reset_absolute_top_line_number() {
    ((fltk3::TextDisplay_I*)_p)->reset_absolute_top_line_number();
  }
    
  int position_to_linecol(int pos, int* lineNum, int* column) const {
    return ((fltk3::TextDisplay_I*)_p)->position_to_linecol(pos, lineNum, column);
  }
  
  int scroll_(int topLineNum, int horizOffset) {
    return ((fltk3::TextDisplay_I*)_p)->scroll_(topLineNum, horizOffset);
  }
  
  void extend_range_for_styles(int* start, int* end) {
    ((fltk3::TextDisplay_I*)_p)->extend_range_for_styles(start, end);
  }
  
  void find_wrap_range(const char *deletedText, int pos, int nInserted,
                       int nDeleted, int *modRangeStart, int *modRangeEnd,
                       int *linesInserted, int *linesDeleted);
  
  void measure_deleted_lines(int pos, int nDeleted) {
    ((fltk3::TextDisplay_I*)_p)->measure_deleted_lines(pos, nDeleted);
  }
  
  void wrapped_line_counter(Fl_Text_Buffer *buf, int startPos, int maxPos,
                            int maxLines, bool startPosIsLineStart,
                            int styleBufOffset, int *retPos, int *retLines,
                            int *retLineStart, int *retLineEnd,
                            bool countLastLineMissingNewLine = true) const {
    ((fltk3::TextDisplay_I*)_p)->wrapped_line_counter((fltk3::TextBuffer*)buf,
                                                    startPos, maxPos,
                                                    maxLines, startPosIsLineStart,
                                                    styleBufOffset, retPos, retLines,
                                                    retLineStart, retLineEnd,
                                                    countLastLineMissingNewLine);
  }
  
  void find_line_end(int pos, bool start_pos_is_line_start, int *lineEnd, int *nextLineStart) const {
    ((fltk3::TextDisplay_I*)_p)->find_line_end(pos, start_pos_is_line_start, lineEnd, nextLineStart);
  }
  
  double measure_proportional_character(const char *s, int colNum, int pos) const {
    return ((fltk3::TextDisplay_I*)_p)->measure_proportional_character(s, colNum, pos);
  }
  
  int wrap_uses_character(int lineEndPos) const {
    return ((fltk3::TextDisplay_I*)_p)->wrap_uses_character(lineEndPos);
  }

};  


#endif

//
// End of "$Id$".
//
