//
// "$Id$"
//
// Header file for Fl_Text_Buffer class.
//
// Copyright 2001-2004 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef FL_TEXT_BUFFER_H
#define FL_TEXT_BUFFER_H

/* Maximum length in characters of a tab or control character expansion
   of a single buffer character */
#define FL_TEXT_MAX_EXP_CHAR_LEN 20

#include "Fl_Export.H"

class FL_EXPORT Fl_Text_Selection {
  friend class Fl_Text_Buffer;

  public:
    void set(int start, int end);
    void set_rectangular(int start, int end, int rectStart, int rectEnd);
    void update(int pos, int nDeleted, int nInserted);
    char rectangular() { return mRectangular; }
    int start() { return mStart; }
    int end() { return mEnd; }
    int rect_start() { return mRectStart; }
    int rect_end() { return mRectEnd; }
    char selected() { return mSelected; }
    void selected(char b) { mSelected = b; }
    int includes(int pos, int lineStartPos, int dispIndex);
    int position(int* start, int* end);
    int position(int* start, int* end, int* isRect, int* rectStart, int* rectEnd);


  protected:
    char mSelected;
    char mRectangular;
    int mStart;
    int mEnd;
    int mRectStart;
    int mRectEnd;
};

typedef void (*Fl_Text_Modify_Cb)(int pos, int nInserted, int nDeleted,
                                  int nRestyled, const char* deletedText,
                                  void* cbArg);
typedef void (*Fl_Text_Predelete_Cb)(int pos, int nDeleted, void* cbArg);

/** The Fl_Text_Buffer class is used by the Fl_Text_Display and
 * Fl_Text_Editor to manage complex text data and is based upon the
 * excellent NEdit text editor engine - see http://www.nedit.org/ . 
 */
class FL_EXPORT Fl_Text_Buffer {
  public:
      /** Creates a new text buffer of the specified initial size. */
    Fl_Text_Buffer(int requestedSize = 0);
      /** Destroys a text buffer. */
    ~Fl_Text_Buffer();

      /** Returns the number of characters in the buffer. */
    int length() { return mLength; }
      /** Gets the text in the buffer. */
    char* text();
      /** Sets the text in the buffer. */
    void text(const char* text);
      /** Returns the text from the range of characters. */
    char* text_range(int start, int end);
      /** Returns the character at the specified position in the buffer. */
    char character(int pos);
      /** Returns the text from the given rectangle. */
    char* text_in_rectangle(int start, int end, int rectStart, int rectEnd);
      /** Inserts text at the specified position. */
    void insert(int pos, const char* text);
      /** Appends the text string to the end of the buffer. */
    void append(const char* t) { insert(length(), t); }
      /** Deletes a range of characters in the buffer. */
    void remove(int start, int end);
      /** Replaces the text in the specified range of characters in the buffer. */
    void replace(int start, int end, const char *text);
      /** Copies text from one buffer to this one; \a fromBuf may be the same as \a this. */
    void copy(Fl_Text_Buffer* fromBuf, int fromStart, int fromEnd, int toPos);
    int undo(int *cp=0);
    void canUndo(char flag=1);
      /** Inserts a file at the specified position. */
    int insertfile(const char *file, int pos, int buflen = 128*1024);
      /** Appends the named file to the end of the buffer. */
    int appendfile(const char *file, int buflen = 128*1024)
      { return insertfile(file, length(), buflen); }
      /** Replaces the current buffer with the contents of a file. */
    int loadfile(const char *file, int buflen = 128*1024)
      { select(0, length()); remove_selection(); return appendfile(file, buflen); }
      /** Writes the specified portions of the file to a file. */
    int outputfile(const char *file, int start, int end, int buflen = 128*1024);
      /** Saves the entire buffer to a file. */
    int savefile(const char *file, int buflen = 128*1024)
      { return outputfile(file, 0, length(), buflen); }
      /** Inserts a column of text without calling the modify callbacks. */
    void insert_column(int column, int startPos, const char* text,
                       int* charsInserted, int* charsDeleted);
      /** Replaces the text in a rectangular area. */
    void replace_rectangular(int start, int end, int rectStart, int rectEnd,
                             const char* text);
      /** Replaces a rectangular region of text with the given text. */
    void overlay_rectangular(int startPos, int rectStart, int rectEnd,
                             const char* text, int* charsInserted,
                             int* charsDeleted);                             
      /** Deletes a rectangular area of text in the buffer. */
    void remove_rectangular(int start, int end, int rectStart, int rectEnd);
      /** Clears text in the specified area. */
    void clear_rectangular(int start, int end, int rectStart, int rectEnd);
      /** Gets the tab width. */
    int tab_distance() { return mTabDist; }
      /** Sets the tab width. */
    void tab_distance(int tabDist);
      /** Selects a range of characters in the buffer. */
    void select(int start, int end);
      /** Returns a non-zero number if any text has been selected, or 0 if no text is selected. */
    int selected() { return mPrimary.selected(); }
      /** Unselects text in the buffer. */
    void unselect();
      /** Selects a rectangle of characters in the buffer. */
    void select_rectangular(int start, int end, int rectStart, int rectEnd);
      /** Returns the current selection. */
    int selection_position(int* start, int* end);
      /** Returns the current selection. */
    int selection_position(int* start, int* end, int* isRect, int* rectStart,
                           int* rectEnd);
      /** Returns the currently selected text. */
    char* selection_text();
      /** Removes the text in the primary selection. */
    void remove_selection();
      /** Replaces the text in the primary selection. */
    void replace_selection(const char* text);
      /** Selects a range of characters in the secondary selection. */
    void secondary_select(int start, int end);
      /** Turns the secondary selection off. */
    void secondary_unselect();
      /** Selects a rectangle of characters in the secondary selection. */
    void secondary_select_rectangular(int start, int end, int rectStart,
                                      int rectEnd);

    int secondary_selection_position(int* start, int* end, int* isRect,
                                     int* rectStart, int* rectEnd);
      /** Returns the text in the secondary selection. */
    char* secondary_selection_text();
      /** Removes the text in the secondary selection. */
    void remove_secondary_selection();
      /** Replaces the text in the secondary selection. */
    void replace_secondary_selection(const char* text);
      /** Highlights the specified text within the buffer. */
    void highlight(int start, int end);
      /** Unhighlights text in the buffer. */
    void unhighlight();
      /** Highlights the specified rectangle of text within the buffer. */
    void highlight_rectangular(int start, int end, int rectStart, int rectEnd);

      /** Returns the current highlight positions. */
    int highlight_position(int* start, int* end, int* isRect, int* rectStart,
                           int* rectEnd);

      /** Returns the highlighted text. */
    char* highlight_text();
      /** Adds a callback function that is called whenever the text buffer
       * is modified. The callback function is declared as follows:
       * \code
typedef void (*Fl_Text_Modify_Cb)(int pos, int nInserted, int nDeleted,
                                  int nRestyled, const char* deletedText,
                                  void* cbArg);
       * \endcode */
    void add_modify_callback(Fl_Text_Modify_Cb bufModifiedCB, void* cbArg);
      /** Removes a modify callback. */
    void remove_modify_callback(Fl_Text_Modify_Cb bufModifiedCB, void* cbArg);

      /** Calls all modify callbacks that have been registered using the 
       * add_modify_callback() method. */
    void call_modify_callbacks() { call_modify_callbacks(0, 0, 0, 0, 0); }

    void add_predelete_callback(Fl_Text_Predelete_Cb bufPredelCB, void* cbArg);
    void remove_predelete_callback(Fl_Text_Predelete_Cb predelCB, void* cbArg);

    void call_predelete_callbacks() { call_predelete_callbacks(0, 0); }

      /** Returns the text for the line containing the specified character position. */
    char* line_text(int pos);
      /** Returns the start position of the line. */
    int line_start(int pos);
      /** Returns the end position of the line. */
    int line_end(int pos);
      /** Returns the position for the start of the word. */
    int word_start(int pos);
      /** Returns the position for the end of the word. */
    int word_end(int pos);
      /** Expands the given character to a displayable format. Tabs and other control
       * characters are given special treatment. */
    int expand_character(int pos, int indent, char *outStr);

    static int expand_character(char c, int indent, char* outStr, int tabDist,
                                char nullSubsChar);

      /** Returns the column width of the specified character. The indent argument
       * specifies the current column position, and tabDist specifies the number of
       * columns to use for each tab.
       *
       * The nullSubsChar argument specifies the current nul character. */
    static int character_width(char c, int indent, int tabDist, char nullSubsChar);
      /** Determines the number of characters that will be displayed between 
       * \a lineStartPos and \a targetPos. */
    int count_displayed_characters(int lineStartPos, int targetPos);
      /** Skips forward the indicated number of characters in the buffer from the start position. */
    int skip_displayed_characters(int lineStartPos, int nChars);
      /** Determines the number of lines between \a startPos and \a endPos. */
    int count_lines(int startPos, int endPos);
      /** Returns the buffer position for the Nth line after the start position. */
    int skip_lines(int startPos, int nLines);
      /** Returns the buffer position for the Nth previous line. */
    int rewind_lines(int startPos, int nLines);
      /** Finds the next occurrence of the specified character. */
    int findchar_forward(int startPos, char searchChar, int* foundPos);
      /** Finds the previous occurrence of the specified character. */
    int findchar_backward(int startPos, char searchChar, int* foundPos);
      /** Finds the next occurrence of the specified characters. */
    int findchars_forward(int startPos, const char* searchChars, int* foundPos);
      /** Finds the previous occurrence of the specified characters. */
    int findchars_backward(int startPos, const char* searchChars, int* foundPos);
      /** Searches forwards for the specified string. */
    int search_forward(int startPos, const char* searchString, int* foundPos,
                       int matchCase = 0);
      /** Searches backwards for the specified string. */
    int search_backward(int startPos, const char* searchString, int* foundPos,
                        int matchCase = 0);
      /** Replaces nul characters in the given string with the nul substitution character. */
    int substitute_null_characters(char* string, int length);
      /** Replaces the nul substitution characters in the provided string with the nul character. */
    void unsubstitute_null_characters(char* string);
      /** Returns the current nul substitution character. */
    char null_substitution_character() { return mNullSubsChar; }
      /** Returns the primary selection. */
    Fl_Text_Selection* primary_selection() { return &mPrimary; }
      /** Returns the secondary selection. */
    Fl_Text_Selection* secondary_selection() { return &mSecondary; }
      /** Returns the current highlight selection. */
    Fl_Text_Selection* highlight_selection() { return &mHighlight; }

  protected:
    void call_modify_callbacks(int pos, int nDeleted, int nInserted,
                               int nRestyled, const char* deletedText);
    void call_predelete_callbacks(int pos, int nDeleted);

    int insert_(int pos, const char* text);
    void remove_(int start, int end);

    void remove_rectangular_(int start, int end, int rectStart, int rectEnd,
                             int* replaceLen, int* endPos);
    void insert_column_(int column, int startPos, const char* insText,
                        int* nDeleted, int* nInserted, int* endPos);

    void overlay_rectangular_(int startPos, int rectStart, int rectEnd,
                              const char* insText, int* nDeleted,
                              int* nInserted, int* endPos);

    void redisplay_selection(Fl_Text_Selection* oldSelection,
                             Fl_Text_Selection* newSelection);

    void move_gap(int pos);
    void reallocate_with_gap(int newGapStart, int newGapLen);
    char* selection_text_(Fl_Text_Selection* sel);
    void remove_selection_(Fl_Text_Selection* sel);
    void replace_selection_(Fl_Text_Selection* sel, const char* text);

    void rectangular_selection_boundaries(int lineStartPos, int rectStart,
                                          int rectEnd, int* selStart,
                                          int* selEnd);

    void update_selections(int pos, int nDeleted, int nInserted);

    Fl_Text_Selection mPrimary; /* highlighted areas */
    Fl_Text_Selection mSecondary;
    Fl_Text_Selection mHighlight;
    int mLength;                /* length of the text in the buffer (the length
                                   of the buffer itself must be calculated:
                                   gapEnd - gapStart + length) */
    char* mBuf;                 /* allocated memory where the text is stored */
    int mGapStart;              /* points to the first character of the gap */
    int mGapEnd;                /* points to the first char after the gap */
    // The hardware tab distance used by all displays for this buffer,
    // and used in computing offsets for rectangular selection operations.
    int mTabDist;               /* equiv. number of characters in a tab */
    int mUseTabs;               /* True if buffer routines are allowed to use
                                   tabs for padding in rectangular operations */
    int mNModifyProcs;          /* number of modify-redisplay procs attached */
    Fl_Text_Modify_Cb*          /* procedures to call when buffer is */
    mNodifyProcs;               /* modified to redisplay contents */
    void** mCbArgs;             /* caller arguments for modifyProcs above */
    int mNPredeleteProcs;	/* number of pre-delete procs attached */
    Fl_Text_Predelete_Cb*	/* procedure to call before text is deleted */
	 mPredeleteProcs;	/* from the buffer; at most one is supported. */
    void **mPredeleteCbArgs;	/* caller argument for pre-delete proc above */
    int mCursorPosHint;         /* hint for reasonable cursor position after
                                   a buffer modification operation */
    char mNullSubsChar;         /* NEdit is based on C null-terminated strings,
                                   so ascii-nul characters must be substituted
                                   with something else.  This is the else, but
                                   of course, things get quite messy when you
                                   use it */
    char mCanUndo;		/* if this buffer is used for attributes, it must
				   not do any undo calls */
};

#endif

//
// End of "$Id$".
//
