/
// "$Id$
/
// Header file for Fl_Text_Display class
/
// Copyright 2001-2004 by Bill Spitzak and others
// Original code Copyright Mark Edel.  Permission to distribute unde
// the LGPL for the FLTK library granted by Mark Edel
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#ifndef FL_TEXT_DISPLAY_
#define FL_TEXT_DISPLAY_

#include "fl_draw.H
#include "Fl_Group.H
#include "Fl_Widget.H
#include "Fl_Scrollbar.H
#include "Fl_Text_Buffer.H

/** This is the FLTK text display widget. It allows the user to view multiple lines o
 * text and supports highlighting and scrolling. The buffer that is displayed in th
 * widget is managed by the Fl_Text_Buffer class. *
class FL_EXPORT Fl_Text_Display: public Fl_Group 
  public
    enum 
      NORMAL_CURSOR, CARET_CURSOR, DIM_CURSOR
      BLOCK_CURSOR, HEAVY_CURSO
    }

    enum 
      CURSOR_POS, CHARACTER_PO
    }

    // drag types- they match Fl::event_clicks() so that single clicking t
    // start a collection selects by character, double clicking selects b
    // word and triple clicking selects by line
    enum 
      DRAG_CHAR = 0, DRAG_WORD = 1, DRAG_LINE = 
    }
    friend void fl_text_drag_me(int pos, Fl_Text_Display* d)

    typedef void (*Unfinished_Style_Cb)(int, void *)

    // style attributes - currently not implemented
    enum 
      ATTR_NONE = 0
      ATTR_UNDERLINE = 1
      ATTR_HIDDEN = 
    }

    struct Style_Table_Entry 
      Fl_Color	color
      Fl_Font	font
      int	size
      unsigned	attr
    }

      /** Creates a new text display widget. *
    Fl_Text_Display(int X, int Y, int W, int H, const char *l = 0)
      /** Destroys a text display widget. *
    ~Fl_Text_Display()

    virtual int handle(int e)
      /** Sets the current text buffer associated with the text widget. Multiple text
       * widgets can be associated with the same text buffer. *
    void buffer(Fl_Text_Buffer* buf)
    void buffer(Fl_Text_Buffer& buf) { buffer(&buf); 
      /** Gets the current text buffer associated with the text widget. *
    Fl_Text_Buffer* buffer() { return mBuffer; 
      /** Marks text from start to end as needing a redraw. *
    void redisplay_range(int start, int end)
      /** Scrolls the current buffer to start at the specified line and column. *
    void scroll(int topLineNum, int horizOffset)
      /** Inserts text at the current insert position. *
    void insert(const char* text)
      /** Replaces text at the current insert position. *
    void overstrike(const char* text)
      /** Sets the current insert position. *
    void insert_position(int newPos)
      /** Gets the current insert position. *
    int insert_position() { return mCursorPos; 
      /** Returns non-zero if the specified mouse position is inside the current selection. *
    int in_selection(int x, int y)
      /** Scrolls the text buffer to show the current insert position. *
    void show_insert_position()
      /** Moves the current insert position right one character. *
    int move_right()
      /** Moves the current insert position left one character. *
    int move_left()
      /** Moves the current insert position up one line. *
    int move_up()
      /** Moves the current insert position down one line. *
    int move_down()
    int count_lines(int start, int end, bool start_pos_is_line_start)
    int line_start(int pos)
    int line_end(int pos, bool start_pos_is_line_start)
    int skip_lines(int startPos, int nLines, bool startPosIsLineStart)
    int rewind_lines(int startPos, int nLines)
      /** Moves the current insert position right one word. *
    void next_word(void)
      /** Moves the current insert position left one word. *
    void previous_word(void)
      /** Shows or hides the text cursor. *
    void show_cursor(int b = 1)
      /** Hides the text cursor. *
    void hide_cursor() { show_cursor(0); 
      /** Gets the text cursor color. *
    Fl_Color cursor_color() const {return mCursor_color;
      /** Sets the text cursor color. *
    void cursor_color(Fl_Color n) {mCursor_color = n;
      /** Sets the text cursor style to one of the following
       * \arg \c Fl_Text_Display::NORMAL_CURSOR - Shows an I beam
       * \arg \c Fl_Text_Display::CARET_CURSOR - Shows a caret under the text
       * \arg \c Fl_Text_Display::DIM_CURSOR - Shows a dimmed I beam
       * \arg \c Fl_Text_Display::BLOCK_CURSOR - Shows an unfilled box around the current character
       * \arg \c Fl_Text_Display::HEAVY_CURSOR - Shows a thick I beam. *
    void cursor_style(int style)
      /** Gets the width/height of the scrollbars. *
    int scrollbar_width() { return scrollbar_width_; 
      /** Gets where scrollbars are attached to the widget. *
    Fl_Align scrollbar_align() { return scrollbar_align_; 
      /** Sets the width/height of the scrollbars. *
    void scrollbar_width(int W) { scrollbar_width_ = W; 
      /** ets where scrollbars are attached to the widget - \c FL_ALIGN_LEFT and
       * \c FL_ALIGN_RIGHT for the vertical scrollbar and \c FL_ALIGN_TOP an
       * \c FL_ALIGN_BOTTOM for the horizontal scrollbar. *
    void scrollbar_align(Fl_Align a) { scrollbar_align_ = a; 
      /** Moves the insert position to the beginning of the current word. *
    int word_start(int pos) { return buffer()->word_start(pos); 
      /** Moves the insert position to the end of the current word. *
    int word_end(int pos) { return buffer()->word_end(pos); 
      /** Sets the text buffer, text styles, and callbacks to use when displaying tex
       * in the widget. Style buffers cannot be shared between widgets and are ofte
       * used to do syntax highlighting. The editor example from Chapter 4 shows ho
       * to use the highlight_data() method. *
    void highlight_data(Fl_Text_Buffer *styleBuffer
                        const Style_Table_Entry *styleTable
                        int nStyles, char unfinishedStyle
                        Unfinished_Style_Cb unfinishedHighlightCB
                        void *cbArg)
      /** Returns the style associated with the character at position
       * <tt>lineStartPos + lineIndex</tt>. *
    int position_style(int lineStartPos, int lineLen, int lineIndex
                       int dispIndex)

      /** Gets the default font used when drawing text in the widget. *
    Fl_Font textfont() const {return (Fl_Font)textfont_;
      /** Sets the default font used when drawing text in the widget. *
    void textfont(uchar s) {textfont_ = s;
      /** Gets the default size of text in the widget. *
    uchar textsize() const {return textsize_;
      /** Sets the default size of text in the widget. *
    void textsize(uchar s) {textsize_ = s;
      /** Gets the default color of text in the widget. *
    Fl_Color textcolor() const {return (Fl_Color)textcolor_;
      /** Sets the default color of text in the widget. *
    void textcolor(unsigned n) {textcolor_ = n;

    int wrapped_column(int row, int column)
    int wrapped_row(int row)
    void wrap_mode(int wrap, int wrap_margin)

    virtual void resize(int X, int Y, int W, int H)

  protected
    // Most (all?) of this stuff should only be called from resize() o
    // draw()
    // Anything with "vline" indicates thats it deals with currentl
    // visible lines

    virtual void draw()
    void draw_text(int X, int Y, int W, int H)
    void draw_range(int start, int end)
    void draw_cursor(int, int)

    void draw_string(int style, int x, int y, int toX, const char *string
                     int nChars)

    void draw_vline(int visLineNum, int leftClip, int rightClip
                    int leftCharIndex, int rightCharIndex)

    void draw_line_numbers(bool clearAll)

    void clear_rect(int style, int x, int y, int width, int height)
    void display_insert()

    void offset_line_starts(int newTopLineNum)

    void calc_line_starts(int startLine, int endLine)

    void update_line_starts(int pos, int charsInserted, int charsDeleted
                            int linesInserted, int linesDeleted, int *scrolled)

    void calc_last_char()

    int position_to_line( int pos, int* lineNum )
    int string_width(const char* string, int length, int style)

    static void buffer_predelete_cb(int pos, int nDeleted, void* cbArg)
    static void buffer_modified_cb(int pos, int nInserted, int nDeleted
                                   int nRestyled, const char* deletedText
                                   void* cbArg)

    static void h_scrollbar_cb(Fl_Scrollbar* w, Fl_Text_Display* d)
    static void v_scrollbar_cb( Fl_Scrollbar* w, Fl_Text_Display* d)
    void update_v_scrollbar()
    void update_h_scrollbar()
    int measure_vline(int visLineNum)
    int longest_vline()
    int empty_vlines()
    int vline_length(int visLineNum)
    int xy_to_position(int x, int y, int PosType = CHARACTER_POS)

    void xy_to_rowcol(int x, int y, int* row, int* column
                      int PosType = CHARACTER_POS)

    int position_to_xy(int pos, int* x, int* y)
    void maintain_absolute_top_line_number(int state)
    int get_absolute_top_line_number()
    void absolute_top_line_number(int oldFirstChar)
    int maintaining_absolute_top_line_number()
    void reset_absolute_top_line_number()
    int position_to_linecol(int pos, int* lineNum, int* column)
    void scroll_(int topLineNum, int horizOffset)

    void extend_range_for_styles(int* start, int* end)

    void find_wrap_range(const char *deletedText, int pos, int nInserted
                           int nDeleted, int *modRangeStart, int *modRangeEnd
                           int *linesInserted, int *linesDeleted)
    void measure_deleted_lines(int pos, int nDeleted)
    void wrapped_line_counter(Fl_Text_Buffer *buf, int startPos, int maxPos
                               int maxLines, bool startPosIsLineStart
                               int styleBufOffset, int *retPos, int *retLines
                               int *retLineStart, int *retLineEnd
                               bool countLastLineMissingNewLine = true)
    void find_line_end(int pos, bool start_pos_is_line_start, int *lineEnd
                         int *nextLineStart)
    int measure_proportional_character(char c, int colNum, int pos)
    int wrap_uses_character(int lineEndPos)
    int range_touches_selection(Fl_Text_Selection *sel, int rangeStart
                                 int rangeEnd)

    int damage_range1_start, damage_range1_end
    int damage_range2_start, damage_range2_end
    int mCursorPos
    int mCursorOn
    int mCursorOldY;		/* Y pos. of cursor for blanking *
    int mCursorToHint;		/* Tells the buffer modified callbac
                                   where to move the cursor, to reduc
                                   the number of redraw calls *
    int mCursorStyle;           /* One of enum cursorStyles above *
    int mCursorPreferredCol;    /* Column for vert. cursor movement *
    int mNVisibleLines;         /* # of visible (displayed) lines *
    int mNBufferLines;          /* # of newlines in the buffer *
    Fl_Text_Buffer* mBuffer;    /* Contains text to be displayed *
    Fl_Text_Buffer* mStyleBuffer; /* Optional parallel buffer containin
                                     color and font information *
    int mFirstChar, mLastChar;  /* Buffer positions of first and las
                                   displayed character (lastChar point
                                   either to a newline or one characte
                                   beyond the end of the buffer) *
    int mContinuousWrap;     	  /* Wrap long lines when displaying *
    int mWrapMargin; 	    	  /* Margin in # of char positions fo
    	    	    	    	    	   wrapping in continuousWrap mode *
    int* mLineStarts
    int mTopLineNum;            /* Line number of top displayed lin
                                   of file (first line of file is 1) *
    int mAbsTopLineNum;			/* In continuous wrap mode, the lin
    					   number of the top line if the tex
					   were not wrapped (note that this i
					   only maintained as needed). *
    int mNeedAbsTopLineNum;	/* Externally settable flag to continu
    					   maintaining absTopLineNum even i
					   it isn't needed for line # display *
    int mHorizOffset;           /* Horizontal scroll pos. in pixels *
    int mTopLineNumHint;        /* Line number of top displayed lin
                                   of file (first line of file is 1) *
    int mHorizOffsetHint;       /* Horizontal scroll pos. in pixels *
    int mNStyles;               /* Number of entries in styleTable *
    const Style_Table_Entry *mStyleTable; /* Table of fonts and colors fo
                                   coloring/syntax-highlighting *
    char mUnfinishedStyle;      /* Style buffer entry which trigger
                                   on-the-fly reparsing of region *
    Unfinished_Style_Cb mUnfinishedHighlightCB; /* Callback to parse "unfinished" *
                                /* regions *
    void* mHighlightCBArg;      /* Arg to unfinishedHighlightCB *

    int mMaxsize

    int mFixedFontWidth;        /* Font width if all current fonts ar
                                   fixed and match in width, else -1 *
    int mSuppressResync;		/* Suppress resynchronization of lin
                                           starts during buffer updates *
    int mNLinesDeleted;			/* Number of lines deleted durin
					   buffer modification (only use
				           when resynchronization is suppressed) *
    int mModifyingTabDistance;	/* Whether tab distance is bein
    					   modified *

    Fl_Color mCursor_color

    Fl_Scrollbar* mHScrollBar
    Fl_Scrollbar* mVScrollBar
    int scrollbar_width_
    Fl_Align scrollbar_align_
    int dragPos, dragType, dragging
    int display_insert_position_hint
    struct { int x, y, w, h; } text_area

    uchar textfont_
    uchar textsize_
    unsigned textcolor_

	 // The following are not presently used from the original NEdit code
	 // but are being put here so that future versions of Fl_Text_Displa
	 // can implement line numbers without breaking binary compatibility
    int mLineNumLeft, mLineNumWidth
				/* Line number margin and width *
}

#endi

/
// End of "$Id$"
/
