//
// "$Id: TextDisplay.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $"
//
// Scrolling text display. I am not sure if this can be used by
// itself, or if this is just a private class used to create TextEditor.
//
// Copyright Mark Edel.  Permission to distribute under the LGPL for
// the FLTK library granted by Mark Edel.
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
//

#ifndef fltk_TextDisplay_h
#define fltk_TextDisplay_h

#include "Group.h"
#include "Scrollbar.h"
#include "TextBuffer.h"

namespace fltk {

class FL_API TextDisplay: public Group {
  public:
    enum {
      NORMAL_CURSOR, CARET_CURSOR, DIM_CURSOR,
      BLOCK_CURSOR, HEAVY_CURSOR
    };

    enum {
      CURSOR_POS, CHARACTER_POS
    };

    // drag types- they match Fl::event_clicks() so that single clicking to
    // start a collection selects by character, double clicking selects by
    // word and triple clicking selects by line.
    enum {
      DRAG_CHAR = 0, DRAG_WORD = 1, DRAG_LINE = 2
    };

    typedef void (*UnfinishedStyleCb)();

    struct FL_API StyleTableEntry {
      Color	color;
      Font*	font;
      int	size;
    };

    TextDisplay(int X, int Y, int W, int H, const char *l = 0);
    ~TextDisplay();
    static NamedStyle* default_style;

    virtual int handle(int e);
    void drag_me(int pos);
    void buffer(TextBuffer* buf);
    void buffer(TextBuffer& buf) { buffer(&buf); }
    TextBuffer* buffer() { return mBuffer; }
    void redisplay_range(int start, int end);
    void scroll(int topLineNum, int horizOffset);
    void insert(const char* text);
    void overstrike(const char* text);
    void insert_position(int newPos);
    int insert_position() { return mCursorPos; }
    int in_selection(int x, int y);
    void show_insert_position();
    int move_right();
    int move_left();
    int move_up();
    int move_down();
    void next_word(void);
    void previous_word(void);
    void show_cursor(int b = 1);
    void hide_cursor() { show_cursor(0); }
    void cursor_style(int style);
    int word_start(int pos) { return buffer()->word_start(pos); }
    int word_end(int pos) { return buffer()->word_end(pos); }

    void highlight_data(TextBuffer *styleBuffer,
                        StyleTableEntry *styleTable,
                        int nStyles, char unfinishedStyle,
                        UnfinishedStyleCb unfinishedHighlightCB,
                        void *cbArg);

    int position_style(int lineStartPos, int lineLen, int lineIndex,
                       int dispIndex);
    
    virtual void layout();
    virtual void draw();

  protected:
    // Most (all?) of this stuff should only be called from layout() or
    // draw().
    // Anything with "vline" indicates thats it deals with currently
    // visible lines.

    void drawtext(int X, int Y, int W, int H);
    void draw_range(int start, int end);
    void draw_cursor(int, int);

    void draw_string(int style, int x, int y, int toX, const char *string,
                     int nChars);

    void draw_vline(int visLineNum, int leftClip, int rightClip,
                    int leftCharIndex, int rightCharIndex);

    void clear_rect(int style, int x, int y, int width, int height);
    void display_insert();

    void offset_line_starts(int newTopLineNum);

    void calc_line_starts(int startLine, int endLine);

    void update_line_starts(int pos, int charsInserted, int charsDeleted,
                            int linesInserted, int linesDeleted, int *scrolled);

    void calc_last_char();

    int position_to_line( int pos, int* lineNum );
    int string_width(const char* string, int length, int style);

    static void buffer_modified_cb(int pos, int nInserted, int nDeleted,
                                   int nRestyled, const char* deletedText,
                                   void* cbArg);

    static void h_scrollbar_cb(Scrollbar* w, TextDisplay* d);
    static void v_scrollbar_cb( Scrollbar* w, TextDisplay* d);
    void update_v_scrollbar();
    void update_h_scrollbar();
    int measure_vline(int visLineNum);
    int longest_vline();
    int empty_vlines();
    int vline_length(int visLineNum);
    int xy_to_position(int x, int y, int PosType = CHARACTER_POS);

    void xy_to_rowcol(int x, int y, int* row, int* column,
                      int PosType = CHARACTER_POS);

    int position_to_xy(int pos, int* x, int* y);
    int position_to_linecol(int pos, int* lineNum, int* column);
    void scroll_(int topLineNum, int horizOffset);

    void extend_range_for_styles(int* start, int* end);


    int damage_range1_start, damage_range1_end;
    int damage_range2_start, damage_range2_end;
    int mCursorPos;
    int mCursorOn;
    int mCursorOldY;		/* Y pos. of cursor for blanking */
    int mCursorToHint;		/* Tells the buffer modified callback
                                   where to move the cursor, to reduce
                                   the number of redraw calls */
    int mCursorStyle;           /* One of enum cursorStyles above */
    int mCursorPreferredCol;    /* Column for vert. cursor movement */
    int mNVisibleLines;         /* # of visible (displayed) lines */
    int mNBufferLines;          /* # of newlines in the buffer */
    TextBuffer* mBuffer;    /* Contains text to be displayed */
    TextBuffer* mStyleBuffer; /* Optional parallel buffer containing
                                     color and font information */
    int mFirstChar, mLastChar;  /* Buffer positions of first and last
                                   displayed character (lastChar points
                                   either to a newline or one character
                                   beyond the end of the buffer) */
    int* mLineStarts;
    int mTopLineNum;            /* Line number of top displayed line
                                   of file (first line of file is 1) */
    int mHorizOffset;           /* Horizontal scroll pos. in pixels */
    int mTopLineNumHint;        /* Line number of top displayed line
                                   of file (first line of file is 1) */
    int mHorizOffsetHint;       /* Horizontal scroll pos. in pixels */
    int mVisibility;            /* Window visibility (see XVisibility
                                   event) */
    int mNStyles;               /* Number of entries in styleTable */
    StyleTableEntry *mStyleTable; /* Table of fonts and colors for
                                   coloring/syntax-highlighting */
    char mUnfinishedStyle;      /* Style buffer entry which triggers
                                   on-the-fly reparsing of region */
    UnfinishedStyleCb mUnfinishedHighlightCB; /* Callback to parse "unfinished" */
                                /* regions */
    void* mHighlightCBArg;      /* Arg to unfinishedHighlightCB */

    int mMaxsize;

    int mFixedFontWidth;        /* Font width if all current fonts are
                                   fixed and match in width, else -1 */

    Color mCursor_color;

    Scrollbar* mHScrollBar;
    Scrollbar* mVScrollBar;
    int dragPos, dragType, dragging;
    int display_insert_position_hint;
    struct { int x, y, w, h; } text_area;
};

}

#endif

//
// End of "$Id: TextDisplay.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $".
//
