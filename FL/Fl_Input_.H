//
// "$Id$"
//
// Input base class header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#ifndef Fl_Input__H
#define Fl_Input__H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif

#define FL_NORMAL_INPUT		0
#define FL_FLOAT_INPUT		1
#define FL_INT_INPUT		2
#define FL_HIDDEN_INPUT		3
#define FL_MULTILINE_INPUT	4
#define FL_SECRET_INPUT		5
#define FL_INPUT_TYPE		7
#define FL_INPUT_READONLY	8
#define FL_NORMAL_OUTPUT	(FL_NORMAL_INPUT | FL_INPUT_READONLY)
#define FL_MULTILINE_OUTPUT	(FL_MULTILINE_INPUT | FL_INPUT_READONLY)
#define FL_INPUT_WRAP		16
#define FL_MULTILINE_INPUT_WRAP	(FL_MULTILINE_INPUT | FL_INPUT_WRAP)
#define FL_MULTILINE_OUTPUT_WRAP (FL_MULTILINE_INPUT | FL_INPUT_READONLY | FL_INPUT_WRAP)

/** This is a virtual base class below Fl_Input. It has all the same 
 * interfaces, but lacks the handle() and draw() method. You may want to
 * subclass it if you are one of those people who likes to change how the
 * editing keys work. */
class FL_EXPORT Fl_Input_ : public Fl_Widget {

  const char* value_;
  char* buffer;

  int size_;
  int bufsize;
  int position_;
  int mark_;
  int xscroll_, yscroll_;
  int mu_p;
  int maximum_size_;

  uchar erase_cursor_only;
  uchar textfont_;
  uchar textsize_;
  unsigned textcolor_;
  unsigned cursor_color_;

  const char* expand(const char*, char*) const;
  double expandpos(const char*, const char*, const char*, int*) const;
  void minimal_update(int, int);
  void minimal_update(int p);
  void put_in_buffer(int newsize);

  void setfont() const;

protected:

    /** Returns true if position \a i is at the start of a word. */
  int word_start(int i) const;
    /** Returns true if position \a i is at the end of a word. */
  int word_end(int i) const;
    /** Returns true if position \a i is at the start of a line. */
  int line_start(int i) const;
    /** Returns true if position \a i is at the end of a line. */
  int line_end(int i) const;
    /** Draw the text in the passed bounding box. If damage() & 
     * \c FL_DAMAGE_ALL is true, this assumes the area has already been
     * erased to color(). Otherwise it does minimal update and erases the
     * area itself. */
  void drawtext(int x, int y, int width, int height);
    /** Do the correct thing for arrow keys. Sets the position (and mark if
     * keepmark is zero) to somewhere in the same line as \a i, such that 
     * pressing the arrows repeatedly will cause the point to move up and down. */
  int up_down_position(int i, int keepmark=0);
  void handle_mouse(int, int, int, int, int keepmark=0);
    /** Default handler for all event types. Your handle() method should
     * call this for all events that it does not handle completely. You
     * must pass it the same bounding box as passed to draw(). Handles
     * \c FL_PUSH, \c FL_DRAG, \c FL_RELEASE to select text, handles 
     * \c FL_FOCUS and \c FL_UNFOCUS to show and hide the cursor. */
  int handletext(int event, int x, int y, int width, int height);
    /** Does the callback if changed() is true or if when() & 
     * \c FL_WHEN_NOT_CHANGED is non-zero. You should call this at any point
     * you think you should generate a callback. */
  void maybe_do_callback();
  int xscroll() const {return xscroll_;}
  int yscroll() const {return yscroll_;}

public:

  void resize(int x, int y, int width, int height);

    /** Creates a new Fl_Input_ widget using the given position, size, and
     * label string. The default boxtype is \c FL_DOWN_BOX. */
  Fl_Input_(int x, int y, int width, int height, const char *label = 0);
    /** The destructor removes the widget and any value associated with it. */
  ~Fl_Input_();

    /** Changes the text and set the mark and the point to the end of it. 
     * The string is copied to the internal buffer. Passing \c NULL is the
     * same as "". This returns non-zero if the new value is different than
     * the current one. */
  int value(const char *sometext);
    /** Changes the text and set the mark and the point to the end of it. 
     * Use this version to directly set the length if you know it already
     * or want to put \c nul's in the text. */
  int value(const char *sometext, int length);
    /** Change the text and set the mark and the point to the end of it. 
     * The string is not copied. If the user edits the string it is copied
     * to the internal buffer then. This can save a great deal of time and
     * memory if your program is rapidly changing the values of text fields,
     * but this will only work if the passed string remains unchanged until 
     * either the Fl_Inputis destroyed or value() is called again. */
  int static_value(const char*);
  int static_value(const char*, int);
    /** Returns the current value, which is a pointer to the internal buffer
     * and is valid only until the next event is handled. */
  const char* value() const {return value_;}
    /** Same as <tt>value()[n]</tt>, but may be faster in plausible 
     * implementations. No bounds checking is done. */
  char index(int i) const {return value_[i];}
    /** Returns the number of characters in value(). This may be greater
     * than strlen(value()) if there are nul characters in it. */
  int size() const {return size_;}
  void size(int W, int H) { Fl_Widget::size(W, H); }
    /** Returns the maximum length of the input field. */
  int maximum_size() const {return maximum_size_;}
    /** Sets the maximum length of the input field. */
  void maximum_size(int m) {maximum_size_ = m;}

    /** Gets the current cursor position. */
  int position() const {return position_;}
    /** Gets the current selection mark. */
  int mark() const {return mark_;}
    /** The input widget maintains two pointers into the string. The 
     * "position" is where the cursor is. The "mark" is the other end of the
     * selected text. If they are equal then there is no selection. Changing
     * this does not affect the clipboard (use copy() to do that).
     *
     * Changing these values causes a redraw(). The new values are bounds
     * checked. The return value is non-zero if the new position is
     * different than the old one. */
  int position(int p, int m);
    /** position(n) is the same as position(n,n). */
  int position(int p) {return position(p, p);}
    /** mark(n) is the same as position(position(),n). */
  int mark(int m) {return position(position(), m);}
    /** This call does all editing of the text. It deletes the region between
     * \a a and \a b (either one may be less or equal to the other), and then
     * inserts the string insert at that point and leaves the mark() and 
     * position() after the insertion. Does the callback if when() & 
     * \c FL_WHEN_CHANGED and there is a change.
     *
     * Set start and end equal to not delete anything. Set insert to \c NULL
     * to not insert anything.
     *
     * \a length must be zero or \c strlen(insert), this saves a tiny bit
     * of time if you happen to already know the length of the insertion, 
     * or can be used to insert a portion of a string or a string containing
     * \c nul's.
     *
     * \a a and \a b are clamped to the 0..size() range, so it is safe to
     * pass any values.
     *
     * cut() and insert() are just inline functions that call replace(). */
  int replace(int a, int b, const char *insert, int length=0);
    /** Fl_Input_::cut() deletes the current selection. */
  int cut() {return replace(position(), mark(), 0);}
    /** cut(n) deletes \a n characters after the position(). cut(-n) deletes
     * \a n characters before the position(). */
  int cut(int n) {return replace(position(), position()+n, 0);}
    /** cut(a,b) deletes the characters between offsets \a a and \a b. 
     * A, b, and n are all clamped to the size of the string. The mark and
     * point are left where the deleted text was.
     *
     * If you want the data to go into the clipboard, do Fl_Input_::copy() 
     * before calling Fl_Input_::cut(), or do Fl_Input_::copy_cuts() 
     * afterwards. */
  int cut(int a, int b) {return replace(a, b, 0);}
    /** Insert the string \a t at the current position, and leave the mark
     * and position after it. If \a l is not zero then it is assumed to be
     * \c strlen(t). */
  int insert(const char* t, int l=0){return replace(position_, mark_, t, l);}
    /** Put the current selection between mark() and position() into the 
     * clipboard. Does not replace the old clipboard contents if
     * position() and mark() are equal. */
  int copy(int clipboard);
    /** Does undo of several previous calls to replace(). Returns non-zero
     * if any change was made. */
  int undo();
    /** Copy all the previous contiguous cuts from the undo information to
     * the clipboard. This is used to make <tt>^K</tt> work. */
  int copy_cuts();

    /** Gets the font of the text in the input field. */
  Fl_Font textfont() const {return (Fl_Font)textfont_;}
    /** Sets the font of the text in the input field. */
  void textfont(uchar s) {textfont_ = s;}
    /** Gets the size of the text in the input field. */
  uchar textsize() const {return textsize_;}
    /** Sets the size of the text in the input field. */
  void textsize(uchar s) {textsize_ = s;}
    /** Gets the color of the text in the input field. */
  Fl_Color textcolor() const {return (Fl_Color)textcolor_;}
    /** Sets the color of the text in the input field. */
  void textcolor(unsigned n) {textcolor_ = n;}
    /** Get the color of the cursor. */
  Fl_Color cursor_color() const {return (Fl_Color)cursor_color_;}
    /** Set the color of the cursor. This is black by default. */
  void cursor_color(unsigned n) {cursor_color_ = n;}

    /** Gets the input field type. */
  int input_type() const {return type() & FL_INPUT_TYPE; }
    /** Sets the input field type. */
  void input_type(int t) { type((uchar)(t | readonly())); }
    /** Gets the read-only state of the input field. */
  int readonly() const { return type() & FL_INPUT_READONLY; }
    /** Sets the read-only state of the input field. */
  void readonly(int b) { if (b) type((uchar)(type() | FL_INPUT_READONLY));
                         else type((uchar)(type() & ~FL_INPUT_READONLY)); }
    /** Gets the word wrapping state of the input field. */
  int wrap() const { return type() & FL_INPUT_WRAP; }
    /** Sets the word wrapping state of the input field. Word wrap is only
     * functional with multi-line input fields. */
  void wrap(int b) { if (b) type((uchar)(type() | FL_INPUT_WRAP));
                         else type((uchar)(type() & ~FL_INPUT_WRAP)); }
};

#endif 

//
// End of "$Id$".
//
