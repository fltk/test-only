//
// "$Id: TextDisplay.h 8306 2011-01-24 17:04:22Z matt $"
//
// Header file for fltk3::TextDisplay class.
//
// Copyright 2001-2010 by Bill Spitzak and others.
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

/* \file
 fltk3::TextDisplay widget . */

#ifndef FLtk3_TEXT_DISPLAY_H
#define FLtk3_TEXT_DISPLAY_H

#include "draw.h"
#include "Group.h"
#include "Widget.h"
#include "Scrollbar.h"
#include "TextBuffer.h"


class Fl_Text_Display;


namespace fltk3 {
  
/**
 \brief Rich text display widget.
 
 This is the FLTK text display widget. It allows the user to view multiple lines
 of text and supports highlighting and scrolling. The buffer that is displayed 
 in the widget is managed by the fltk3::TextBuffer class. A single Text Buffer
 can be displayed by multiple Text Displays.
 */
class FLTK3_EXPORT TextDisplay: public fltk3::Group {

friend class ::Fl_Text_Display;

public:
  
  /** 
   text display cursor shapes enumeration 
   */
  enum {
    NORMAL_CURSOR,    /**< I-beam */
    CARET_CURSOR,     /**< caret under the text */
    DIM_CURSOR,       /**< dim I-beam */
    BLOCK_CURSOR,     /**< unfille box under the current character */
    HEAVY_CURSOR      /**< thick I-beam */
  };
  
  /**
   the character position is the left edge of a character, whereas 
   the cursor is thought to be between the centers of two consecutive
   characters.
   */
  enum {
    CURSOR_POS, 
    CHARACTER_POS
  };
  
  /** 
   drag types - they match fltk3::event_clicks() so that single clicking to
   start a collection selects by character, double clicking selects by
   word and triple clicking selects by line.
   */
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
  
  friend void text_drag_me(int pos, fltk3::TextDisplay* d);
  
  typedef void (*UnfinishedStyleCb)(int, void *);
  
  /** 
   This structure associates the color, font, andsize of a string to draw
   with an attribute mask matching attr
   */
  struct StyleTableEntry {
    fltk3::Color    color;
    fltk3::Font     font;
    fltk3::Fontsize size;
    unsigned    attr;
  };
  
  TextDisplay(int X, int Y, int W, int H, const char *l = 0);
  ~TextDisplay();
  
  virtual int readonly() { return 1; }
  
  void handle_menu_event();
  
  virtual int handle(int e);
  
  void buffer(fltk3::TextBuffer* buf);
  
  /**
   Sets the current text buffer associated with the text widget.
   Multiple text widgets can be associated with the same text buffer.
   \param buf new text buffer
   */
  void buffer(fltk3::TextBuffer& buf) { buffer(&buf); }
  
  /**
   Gets the current text buffer associated with the text widget.
   Multiple text widgets can be associated with the same text buffer.
   \return current text buffer
   */
  fltk3::TextBuffer* buffer() const { return mBuffer; }
  
  void redisplay_range(int start, int end);
  void scroll(int topLineNum, int horizOffset);
  void insert(const char* text);
  void overstrike(const char* text);
  void insert_position(int newPos);
  
  /** 
   Gets the position of the text insertion cursor for text display.
   \return insert position index into text buffer 
   */
  int insert_position() const { return mCursorPos; }
  int position_to_xy(int pos, int* x, int* y) const;

  int in_selection(int x, int y) const;
  void show_insert_position();
  
  int move_right();
  int move_left();
  int move_up();  
  int move_down();
  int count_lines(int start, int end, bool start_pos_is_line_start) const;
  int line_start(int pos) const;
  int line_end(int startPos, bool startPosIsLineStart) const;
  int skip_lines(int startPos, int nLines, bool startPosIsLineStart);
  int rewind_lines(int startPos, int nLines);
  void next_word(void);
  void previous_word(void);
  
  void show_cursor(int b = 1);
  
  /**
   Hides the text cursor.
   */
  void hide_cursor() { show_cursor(0); }
  
  void cursor_style(int style);
  
  /**
   Gets the text cursor color.  
   \return cursor color
   */
  fltk3::Color cursor_color() const {return mCursor_color;}
  
  /**    
   Sets the text cursor color.
   \param n new cursor color
   */
  void cursor_color(fltk3::Color n) {mCursor_color = n;}
  
  /**   
   Gets the width/height of the scrollbars.
   /return width of scrollbars
   */
  int scrollbar_width() const { return scrollbar_width_; }
  
  /**
   Sets the width/height of the scrollbars.
   \param W width of scrollbars
   */
  void scrollbar_width(int W) { scrollbar_width_ = W; }
  
  /**
   Gets the scrollbar alignment type.
   \return scrollbar alignment
   */
  fltk3::Align scrollbar_align() const { return scrollbar_align_; }
  
  /**
   Sets the scrollbar alignment type.
   \param a new scrollbar alignment
   */
  void scrollbar_align(fltk3::Align a) { scrollbar_align_ = a; }
  
  /**
   Moves the insert position to the beginning of the current word.
   \param pos start calculation at this index
   \return beginning of the words
   */
  int word_start(int pos) const { return buffer()->word_start(pos); }
  
  /** 
   Moves the insert position to the end of the current word.
   \param pos start calculation at this index
   \return index of first character after the end of the word
   */
  int word_end(int pos) const { return buffer()->word_end(pos); }
  
  
  void highlight_data(fltk3::TextBuffer *styleBuffer,
                      const StyleTableEntry *styleTable,
                      int nStyles, char unfinishedStyle,
                      UnfinishedStyleCb unfinishedHighlightCB,
                      void *cbArg);
  
  int position_style(int lineStartPos, int lineLen, int lineIndex) const;
  
  /** 
   \todo FIXME : get set methods pointing on shortcut_ 
   have no effects as shortcut_ is unused in this class and derived! 
   \return the current shortcut key
   */
  unsigned int shortcut() const {return shortcut_;}
  
  /**
   \todo FIXME : get set methods pointing on shortcut_ 
   have no effects as shortcut_ is unused in this class and derived! 
   \param s the new shortcut key
   */
  void shortcut(unsigned int s) {shortcut_ = s;}
  
  /**
   Sets the default font used when drawing text in the widget.
   \param s default text font face
   \todo textfont is a method of Widget and should not have this side effect
   */
  void textfont(fltk3::Font s) {Widget::textfont(s); mColumnScale = 0;}
  
  /**
   Sets the default size of text in the widget.
   \param s new text size
   \todo textsize is a method of Widget and should not have this side effect
   */
  void textsize(fltk3::Fontsize s) {Widget::textsize(s); mColumnScale = 0;}
  fltk3::Font textfont() const {return Widget::textfont();}
  fltk3::Fontsize textsize() const {return Widget::textsize();}
  
  int wrapped_column(int row, int column) const;
  int wrapped_row(int row) const;
  void wrap_mode(int wrap, int wrap_margin);
  
  virtual void resize(int X, int Y, int W, int H);

  /**
   Convert an x pixel position into a column number.
   \param x number of pixels from the left margin
   \return an approximate column number based on the main font
   */
  double x_to_col(double x) const;
  
  /**
   Convert a column number into an x pixel position.
   \param col an approximate column number based on the main font
   \return number of pixels from the left margin to the left of an
	   average sized character
   */
  double col_to_x(double col) const;
  
protected:
  // Most (all?) of this stuff should only be called from resize() or
  // draw().
  // Anything with "vline" indicates thats it deals with currently
  // visible lines.
  
  virtual void draw();
  void draw_text(int X, int Y, int W, int H);
  void draw_range(int start, int end);
  void draw_cursor(int, int);
  
  void draw_string(int style, int x, int y, int toX, const char *string,
                   int nChars) const;
  
  void draw_vline(int visLineNum, int leftClip, int rightClip,
                  int leftCharIndex, int rightCharIndex);
  
  int find_x(const char *s, int len, int style, int x) const;
  
  enum { 
    DRAW_LINE, 
    FIND_INDEX,
    FIND_INDEX_FROM_ZERO,
    GET_WIDTH 
  };
  
  int handle_vline(int mode, 
                   int lineStart, int lineLen, int leftChar, int rightChar,
                   int topClip, int bottomClip,
                   int leftClip, int rightClip) const;
  
  void draw_line_numbers(bool clearAll);
  
  void clear_rect(int style, int x, int y, int width, int height) const;
  void display_insert();
  
  void offset_line_starts(int newTopLineNum);
  
  void calc_line_starts(int startLine, int endLine);
  
  void update_line_starts(int pos, int charsInserted, int charsDeleted,
                          int linesInserted, int linesDeleted, int *scrolled);
  
  void calc_last_char();
  
  int position_to_line( int pos, int* lineNum ) const;
  double string_width(const char* string, int length, int style) const;
  
  static void scroll_timer_cb(void*);
  
  static void buffer_predelete_cb(int pos, int nDeleted, void* cbArg);
  static void buffer_modified_cb(int pos, int nInserted, int nDeleted,
                                 int nRestyled, const char* deletedText,
                                 void* cbArg);
  
  static void h_scrollbar_cb(fltk3::Scrollbar* w, fltk3::TextDisplay* d);
  static void v_scrollbar_cb( fltk3::Scrollbar* w, fltk3::TextDisplay* d);
  void update_v_scrollbar();
  void update_h_scrollbar();
  int measure_vline(int visLineNum) const;
  int longest_vline() const;
  int empty_vlines() const;
  int vline_length(int visLineNum) const;
  int xy_to_position(int x, int y, int PosType = CHARACTER_POS) const;
  
  void xy_to_rowcol(int x, int y, int* row, int* column,
                    int PosType = CHARACTER_POS) const;
  void maintain_absolute_top_line_number(int state);
  int get_absolute_top_line_number() const;
  void absolute_top_line_number(int oldFirstChar);
  int maintaining_absolute_top_line_number() const;
  void reset_absolute_top_line_number();
  int position_to_linecol(int pos, int* lineNum, int* column) const;
  int scroll_(int topLineNum, int horizOffset);
  
  void extend_range_for_styles(int* start, int* end);
  
  void find_wrap_range(const char *deletedText, int pos, int nInserted,
                       int nDeleted, int *modRangeStart, int *modRangeEnd,
                       int *linesInserted, int *linesDeleted);
  void measure_deleted_lines(int pos, int nDeleted);
  void wrapped_line_counter(fltk3::TextBuffer *buf, int startPos, int maxPos,
                            int maxLines, bool startPosIsLineStart,
                            int styleBufOffset, int *retPos, int *retLines,
                            int *retLineStart, int *retLineEnd,
                            bool countLastLineMissingNewLine = true) const;
  void find_line_end(int pos, bool start_pos_is_line_start, int *lineEnd,
                     int *nextLineStart) const;
  double measure_proportional_character(const char *s, int colNum, int pos) const;
  int wrap_uses_character(int lineEndPos) const;
  
  int damage_range1_start, damage_range1_end;
  int damage_range2_start, damage_range2_end;
  int mCursorPos;
  int mCursorOn;
  int mCursorOldY;              /* Y pos. of cursor for blanking */
  int mCursorToHint;            /* Tells the buffer modified callback
                                 where to move the cursor, to reduce
                                 the number of redraw calls */
  int mCursorStyle;             /* One of enum cursorStyles above */
  int mCursorPreferredXPos;     /* Pixel position for vert. cursor movement */
  int mNVisibleLines;           /* # of visible (displayed) lines */
  int mNBufferLines;            /* # of newlines in the buffer */
  fltk3::TextBuffer* mBuffer;      /* Contains text to be displayed */
  fltk3::TextBuffer* mStyleBuffer; /* Optional parallel buffer containing
                                 color and font information */
  int mFirstChar, mLastChar;    /* Buffer positions of first and last
                                 displayed character (lastChar points
                                 either to a newline or one character
                                 beyond the end of the buffer) */
  int mContinuousWrap;          /* Wrap long lines when displaying */
  int mWrapMarginPix; 	    	/* Margin in # of pixels for
                                 wrapping in continuousWrap mode */
  int* mLineStarts;
  int mTopLineNum;              /* Line number of top displayed line
                                 of file (first line of file is 1) */
  int mAbsTopLineNum;           /* In continuous wrap mode, the line
                                  number of the top line if the text
                                  were not wrapped (note that this is
                                  only maintained as needed). */
  int mNeedAbsTopLineNum;       /* Externally settable flag to continue
                                 maintaining absTopLineNum even if
                                 it isn't needed for line # display */
  int mHorizOffset;             /* Horizontal scroll pos. in pixels */
  int mTopLineNumHint;          /* Line number of top displayed line
                                 of file (first line of file is 1) */
  int mHorizOffsetHint;         /* Horizontal scroll pos. in pixels */
  int mNStyles;                 /* Number of entries in styleTable */
  const StyleTableEntry *mStyleTable; /* Table of fonts and colors for
                                         coloring/syntax-highlighting */
  char mUnfinishedStyle;        /* Style buffer entry which triggers
                                 on-the-fly reparsing of region */
  UnfinishedStyleCb mUnfinishedHighlightCB; /* Callback to parse "unfinished" */
  /* regions */
  void* mHighlightCBArg;        /* Arg to unfinishedHighlightCB */
  
  int mMaxsize;
  
  int mSuppressResync;          /* Suppress resynchronization of line
                                 starts during buffer updates */
  int mNLinesDeleted;           /* Number of lines deleted during
                                 buffer modification (only used
                                 when resynchronization is suppressed) */
  int mModifyingTabDistance;    /* Whether tab distance is being
                                 modified */
  
  mutable double mColumnScale; /* Width in pixels of an average character. This
                                 value is calculated as needed (lazy eval); it 
                                 needs to be mutable so that it can be calculated
                                 within a method marked as "const" */
  
  fltk3::Color mCursor_color;
  
  fltk3::Scrollbar* mHScrollBar;
  fltk3::Scrollbar* mVScrollBar;
  int scrollbar_width_;
  fltk3::Align scrollbar_align_;
  int dragPos, dragType, dragging;
  int display_insert_position_hint;
  struct { int x, y, w, h; } text_area;
  
  unsigned int shortcut_;
  
  // The following are not presently used from the original NEdit code,
  // but are being put here so that future versions of fltk3::TextDisplay
  // can implement line numbers without breaking binary compatibility.
  
  /* Line number margin and width */
  int mLineNumLeft, mLineNumWidth;
};

} // namespace

#endif

//
// End of "$Id: TextDisplay.h 8306 2011-01-24 17:04:22Z matt $".
//
