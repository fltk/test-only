/
// "$Id$
/
// Browser header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
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

// Forms-compatable browser.  Probably useful for othe
// lists of textual data.  Notice that the line number
// start from 1, and 0 means "no line"

#ifndef Fl_Browser_
#define Fl_Browser_

#include "Fl_Browser_.H

struct FL_BLINE

/** The Fl_Browser widget displays a scrolling list of text lines, an
 * manages all the storage for the text. This is not a text editor o
 * spreadsheet! But it is useful for showing a vertical list of name
 * objects to the user
 
 * Each line in the browser is identified by number. The numbers start a
 * one (this is so that zero can be reserved for "no line" in the selectiv
 * browsers). <i>Unless otherwise noted, the methods do not check to see i
 * the passed line number is in range and legal. It must always be greate
 * than zero and <= size().</i
 
 * Each line contains a null-terminated string of text and a <i>void *</i> data
 * pointer. The text string is displayed, the <i>void *</i> pointer can be used b
 * the callbacks to reference the object the text describes
 
 * The base class does nothing when the user clicks on it. The subclasse
 * Fl_Select_Browser, Fl_Hold_Browser, and Fl_Multi_Browser react to use
 * clicks to select lines in the browser and do callbacks
 
 * The base class called Fl_Browser_ provides the scrolling and selectio
 * mechanisms of this and all the subclasses, but the dimensions an
 * appearance of each item are determined by the subclass. You can us
 * Fl_Browser_ to display information other than text, or text that i
 * dynamically produced from your own data structures. If you find tha
 * loading the browser is a lot of work or is inefficient, you may want t
 * make a subclass ofFl_Browser_. *
class FL_EXPORT Fl_Browser : public Fl_Browser_ 

  FL_BLINE *first;		// the array of line
  FL_BLINE *last
  FL_BLINE *cache
  int cacheline;		// line number of cach
  int lines;                	// Number of line
  int full_height_
  const int* column_widths_
  char format_char_;		// alternative to @-sig
  char column_char_;		// alternative to ta

protected

  // required routines for Fl_Browser_ subclass
  void* item_first() const 
  void* item_next(void*) const 
  void* item_prev(void*) const 
  int item_selected(void*) const 
  void item_select(void*, int)
  int item_height(void*) const 
  int item_width(void*) const 
  void item_draw(void*, int, int, int, int) const 
  int full_height() const 
  int incr_height() const 

  FL_BLINE* find_line(int) const 
  FL_BLINE* _remove(int) 
  void insert(int, FL_BLINE*)
  int lineno(void*) const 
  void swap(FL_BLINE *a, FL_BLINE *b)

public

    /** Remove line n and make the browser one line shorter. *
  void remove(int)
    /** Add a new line to the end of the browser. The text is copied usin
     * the \c strdup() function. It may also be \c NULL to make a blan
     * line. The void * argument is returned as the data() of the new item. *
  void add(const char*, void* = 0)
    /** Insert a new line before line n. If n > size() then the line i
     * added to the end. *
  void insert(int n, const char *text, void *data = 0)
    /** Line from is removed and reinserted at to; to is calculated after the line is removed. *
  void move(int to, int from)
    /** Clears the browser and reads the file, adding each line from th
     * file to the browser. If the filename is \c NULL or a zero-lengt
     * string then this just clears the browser. This returns zero if ther
     * was any error in opening or reading the file, in which case errno i
     * set to the system error. The data() of each line is set to \c NULL. *
  int  load(const char* filename)
    /** Swaps two lines in the browser. *
  void swap(int a, int b)
    /** Remove all the lines in the browser. *
  void clear()

    /** Returns how many lines are in the browser. The last line number is equal to this. *
  int size() const {return lines;
  void size(int W, int H) { Fl_Widget::size(W, H); 

    /** Returns the current top line in the browser. If there is no vertica
     * scrollbar then this will always return 1. *
  int topline() const 
  enum Fl_Line_Position { TOP, BOTTOM, MIDDLE }
    /** Scrolls the browser so the top line in the browser is n. *
  void lineposition(int n, Fl_Line_Position)
  void topline(int l) { lineposition(l, TOP); 
    /** Scrolls the browser so the bottom line in the browser is n. *
  void bottomline(int l) { lineposition(l, BOTTOM); 
    /** Scrolls the browser so the middle line in the browser is n. *
  void middleline(int l) { lineposition(l, MIDDLE); 

    /** Selects one or more lines. *
  int select(int line, int n=1)
    /** Gets the current selection state of a line. *
  int selected(int) const 
    /** Makes line n visible for selection. *
  void show(int n)
  void show() {Fl_Widget::show();
    /** Makes line n invisible, preventing selection by the user. The lin
     * can still be selected under program control. *
  void hide(int n)
  void hide() {Fl_Widget::hide();
    /** Returns a non-zero value if line n is visible. *
  int visible(int n) const 

    /** Gets the last toggled line. This returns zero if no line has bee
     * toggled, so be aware that this can happen in a callback. *
  int value() const 
    /** Selects a single line. *
  void value(int v) {select(v);
    /** Returns the text for line n. If n is out of range it returns \c NULL. *
  const char* text(int n) const 
    /** Sets the text for line n. *
  void text(int n, const char *text)
    /** Returns the data for line n. If n is out of range this returns \c NULL. *
  void* data(int n) const 
    /** Sets the data for line n. *
  void data(int n, void* v)

    /** The constructor makes an empty browser. *
  Fl_Browser(int, int, int, int, const char* = 0)
    /** The destructor deletes all list items and destroys the browser. *
  ~Fl_Browser() { clear(); 

    /** The first form gets the current format code prefix character, whic
     * by default is @. A string of formatting codes at the start of eac
     * column are stripped off and used to modify how the rest of the lin
     * is printed
     * \arg \c @@. Print rest of line, don't look for more '@' sign
     * \arg \c @@@@ Print rest of line starting with '@
     * \arg \c @@l Use a large (24 point) fon
     * \arg \c @@m Use a medium large (18 point) fon
     * \arg \c @@s Use a small (11 point) fon
     * \arg \c @@b Use a bold font (adds FL_BOLD to font
     * \arg \c @@i Use an italic font (adds FL_ITALIC to font
     * \arg \c @@f or @@t Use a fixed-pitch font (sets font to FL_COURIER
     * \arg \c @@c Center the line horizontall
     * \arg \c @@r Right-justify the tex
     * \arg <tt>@@B0, @@B1, ... @@B255</tt> Fill the backgound with fl_color(n
     * \arg <tt>@@C0, @@C1, ... @@C255</tt> Use fl_color(n) to draw the tex
     * \arg <tt>@@F0, @@F1, ...</tt> Use fl_font(n) to draw the tex
     * \arg <tt>@@S1, @@S2, ...</tt> Use point size n to draw the tex
     * \arg \c @@u or \c @@_ Underline the text
     * \arg \c @@- draw an engraved line through the middle
     
     * Notice that the <tt>@.</tt> command can be used to reliably terminat
     * the parsing. To print a random string in a random color, us
     * <tt>sprintf("@C%d@.%s", color, string)</tt> and it will work even i
     * the string starts with a digit or has the format character in it. *
  char format_char() const {return format_char_;
    /** Sets the current prefix to c. Set the prefix to 0 to disable formatting. *
  void format_char(char c) {format_char_ = c;
    /** Gets the current column separator character. By default this is '\\t' (tab). *
  char column_char() const {return column_char_;
    /** Sets the column separator to c. This will only have an effect i
     * you also set column_widths(). *
  void column_char(char c) {column_char_ = c;
    /** Gets the current column width array. *
  const int* column_widths() const {return column_widths_;
    /** Sets the current array to w. Make sure the last entry is zero
     
     * This array is zero-terminated and specifies the widths in pixels o
     * each column. The text is split at each column_char()and each part i
     * formatted into it's own column. After the last column any remainin
     * text is formatted into the space between the last column and th
     * right edge of the browser, even if the text contains instances o
     * column_char() . The default value is a one-element array of just 
     * zero, which makes there are no columns. *
  void column_widths(const int* l) {column_widths_ = l;

  int displayed(int n) const {return Fl_Browser_::displayed(find_line(n));
  void make_visible(int n) 
  	if (n < 1) Fl_Browser_::display(find_line(1))
	else if (n > lines) Fl_Browser_::display(find_line(lines))
	else Fl_Browser_::display(find_line(n))
  

  // for back compatability only
  void replace(int a, const char* b) {text(a, b);
  void display(int, int=1)
}

#endi

/
// End of "$Id$"
/
