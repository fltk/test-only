//
// "$Id: Fl_Input.h,v 1.10 2002/10/29 00:37:23 easysw Exp $"
//
// Input base class header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef Fl_Input_H
#define Fl_Input_H

#ifndef Fl_Widget_H
#include "Fl_Widget.h"
#endif
/**

   This is the FLTK text input widget. It displays a single line of text and 
   lets the user edit it. Normally it is drawn with an inset box and a white 
   background. The text may contain any characters (even 0). The unprintable 
   control characters are displayed with ^X notation. The appearance of other
   characters will depend on your operating system. 


<TABLE border=1 WIDTH=90%>

<TR><TD WIDTH=200><B>Mouse button 1</B></TD><TD>Moves the cursor to
 this point. Drag selects characters.  Double click selects words.
 Triple click selects all text.  Shift+click extends the selection.
 When you select text it is automatically copied to the clipboard.
</TD></TR>

<TR><TD><B>Mouse button 2</B></TD><TD>Insert the clipboard at
the point clicked.  You can also select a region and replace it with the
clipboard by selecting the region with mouse button 2.
</TD></TR>

<TR><TD><B>Mouse button 3</B></TD><TD>Currently acts like button 1.</TD></TR>

<TR><TD><B>Backspace</B></TD><TD>Deletes one character to the left, or 
deletes the selected region.</TD></TR>
<TR><TD><B>Enter</B></TD><TD>May cause the callback, see when().</TD></TR>
<TR><TD><B>^A or Home</B></TD><TD>Go to start of line.</TD></TR>
<TR><TD><B>^B or Left</B></TD><TD>Move left</TD></TR>
<TR><TD><B>^C</B></TD><TD>Copy the selection to the clipboard</TD></TR>
<TR><TD><B>^D or Delete</B></TD><TD>Deletes one character to the right 
or deletes the selected region.</TD></TR>
<TR><TD><B>^E or End</B></TD><TD>Go to the end of line.</TD></TR>
<TR><TD><B>^F or Right</B></TD><TD>Move right</TD></TR>
<TR><TD><B>^K</B></TD><TD>Delete to the end of line (next \n character) 
or deletes a single \n character.  These deletions are all concatenated 
into the clipboard.</TD></TR>
<TR><TD><B>^N or Down</B></TD><TD>Move down (for Fl_Multiline_Input 
only, otherwise it moves to the next input field).</TD></TR>
<TR><TD><B>^O</B></TD><TD>Insert a newline and put the cursor before it.</TD></TR>
<TR><TD><B>^P or Up</B></TD><TD>Move up (for Fl_Multiline_Input only, 
otherwise it moves to the previous input field).</TD></TR>
<TR><TD><B>^T</B></TD><TD>Swap the two characters around the cursor,
or the two characters before it if at the end of line.</TD></TR>
<TR><TD><B>^U</B></TD><TD>Delete everything.</TD></TR>
<TR><TD><B>^V or ^Y</B></TD><TD>Paste the clipboard</TD></TR>
<TR><TD><B>^X or ^W</B></TD><TD>Copy the region to the clipboard and 
delete it.</TD></TR>
<TR><TD><B>^Z or ^_</B></TD><TD>Undo.  This is a single-level undo 
mechanism, but all adjacent deletions and insertions are concatenated 
into a single &quot;undo&quot;.  Often this will undo a lot more than you 
expected.</TD></TR>
<TR><TD><B>Shift+move</B></TD><TD>Move the cursor but also extend the 
selection.</TD></TR>

<TR><TD><B>RightCtrl or
<BR>Compose</B></TD><TD><a name=compose>
Start a <A href="Fl.html#compose">compose-character</A>
sequence.  The next one or two keys typed define the character to insert:

<br><center><table border=1>
<tr>
	<th>Keys</th><th>Char</th>
	<th>Keys</th><th>Char</th>
	<th>Keys</th><th>Char</th>
	<th>Keys</th><th>Char</th>
	<th>Keys</th><th>Char</th>
	<th>Keys</th><th>Char</th>

</tr><tr>
	<td align=center><small>space</small></td><td align=center><small>nbsp</small></td>
	<td align=center><TT>*</tt></td><td align=center>°</td>
	<td align=center><TT>`A</tt></td><td align=center>À</td>
	<td align=center><TT>D-</tt></td><td align=center>Ð</td>
	<td align=center><TT>`a</tt></td><td align=center>à</td>
	<td align=center><TT>d-</tt></td><td align=center>ð
</tr><tr>
	<td align=center><TT>!</tt></td><td align=center>¡</td>
	<td align=center><TT>+-</tt></td><td align=center>±</td>
	<td align=center><TT>'A</tt></td><td align=center>Á</td>
	<td align=center><TT>~N</tt></td><td align=center>Ñ</td>
	<td align=center><TT>'a</tt></td><td align=center>á</td>
	<td align=center><TT>~n</tt></td><td align=center>ñ
</tr><tr>
	<td align=center><TT>c|</tt></td><td align=center>¢</td>
	<td align=center><TT>2</tt></td><td align=center>²</td>
	<td align=center><TT>A^</tt></td><td align=center>Â</td>
	<td align=center><TT>`O</tt></td><td align=center>Ò</td>
	<td align=center><TT>^a</tt></td><td align=center>â</td>
	<td align=center><TT>`o</tt></td><td align=center>ò
</tr><tr>
	<td align=center><TT>L-</tt></td><td align=center>£</td>
	<td align=center><TT>3</tt></td><td align=center>³</td>
	<td align=center><TT>~A</tt></td><td align=center>Ã</td>
	<td align=center><TT>'O</tt></td><td align=center>Ó</td>
	<td align=center><TT>~a</tt></td><td align=center>ã</td>
	<td align=center><TT>'o</tt></td><td align=center>ó
</tr><tr>
	<td align=center><TT>ox</tt></td><td align=center>¤</td>
	<td align=center><TT>'</tt></td><td align=center>´</td>
	<td align=center><TT>:A</tt></td><td align=center>Ä</td>
	<td align=center><TT>^O</tt></td><td align=center>Ô</td>
	<td align=center><TT>:a</tt></td><td align=center>ä</td>
	<td align=center><TT>^o</tt></td><td align=center>ô
</tr><tr>
	<td align=center><TT>y=</tt></td><td align=center>¥</td>
	<td align=center><TT>u</tt></td><td align=center>µ</td>
	<td align=center><TT>*A</tt></td><td align=center>Å</td>
	<td align=center><TT>~O</tt></td><td align=center>Õ</td>
	<td align=center><TT>*a</tt></td><td align=center>å</td>
	<td align=center><TT>~o</tt></td><td align=center>õ
</tr><tr>
	<td align=center><TT>|</tt></td><td align=center>¦</td>
	<td align=center><TT>p</tt></td><td align=center>¶</td>
	<td align=center><TT>AE</tt></td><td align=center>Æ</td>
	<td align=center><TT>:O</tt></td><td align=center>Ö</td>
	<td align=center><TT>ae</tt></td><td align=center>æ</td>
	<td align=center><TT>:o</tt></td><td align=center>ö
</tr><tr>
	<td align=center><TT>&amp;</tt></td><td align=center>§</td>
	<td align=center><TT>.</tt></td><td align=center>·</td>
	<td align=center><TT>,C</tt></td><td align=center>Ç</td>
	<td align=center><TT>x</tt></td><td align=center>×</td>
	<td align=center><TT>,c</tt></td><td align=center>ç</td>
	<td align=center><TT>-:</tt></td><td align=center>÷
</tr><tr>
	<td align=center><TT>:</tt></td><td align=center>¨</td>
	<td align=center><TT>,</tt></td><td align=center>¸</td>
	<td align=center><TT>E`</tt></td><td align=center>È</td>
	<td align=center><TT>O/</tt></td><td align=center>Ø</td>
	<td align=center><TT>`e</tt></td><td align=center>è</td>
	<td align=center><TT>o/</tt></td><td align=center>ø
</tr><tr>
	<td align=center><TT>c</tt></td><td align=center>©</td>
	<td align=center><TT>1</tt></td><td align=center>¹</td>
	<td align=center><TT>'E</tt></td><td align=center>É</td>
	<td align=center><TT>`U</tt></td><td align=center>Ù</td>
	<td align=center><TT>'e</tt></td><td align=center>é</td>
	<td align=center><TT>`u</tt></td><td align=center>ù
</tr><tr>
	<td align=center><TT>a</tt></td><td align=center>ª</td>
	<td align=center><TT>o</tt></td><td align=center>º</td>
	<td align=center><TT>^E</tt></td><td align=center>Ê</td>
	<td align=center><TT>'U</tt></td><td align=center>Ú</td>
	<td align=center><TT>^e</tt></td><td align=center>ê</td>
	<td align=center><TT>'u</tt></td><td align=center>ú
</tr><tr>
	<td align=center><TT>&lt;&lt;</tt></td><td align=center>«</td>
	<td align=center><TT>>></tt></td><td align=center>»</td>
	<td align=center><TT>:E</tt></td><td align=center>Ë</td>
	<td align=center><TT>^U</tt></td><td align=center>Û</td>
	<td align=center><TT>:e</tt></td><td align=center>ë</td>
	<td align=center><TT>^u</tt></td><td align=center>û
</tr><tr>
	<td align=center><TT>~</tt></td><td align=center>¬</td>
	<td align=center><TT>14</tt></td><td align=center>1/4</td>
	<td align=center><TT>`I</tt></td><td align=center>Ì</td>
	<td align=center><TT>:U</tt></td><td align=center>Ü</td>
	<td align=center><TT>`i</tt></td><td align=center>ì</td>
	<td align=center><TT>:u</tt></td><td align=center>ü
</tr><tr>
	<td align=center><TT>-</tt></td><td align=center>­</td>
	<td align=center><TT>12</tt></td><td align=center>1/2</td>
	<td align=center><TT>'I</tt></td><td align=center>Í</td>
	<td align=center><TT>'Y</tt></td><td align=center>Ý</td>
	<td align=center><TT>'i</tt></td><td align=center>í</td>
	<td align=center><TT>'y</tt></td><td align=center>ý
</tr><tr>
	<td align=center><TT>r</tt></td><td align=center>®</td>
	<td align=center><TT>34</tt></td><td align=center>3/4</td>
	<td align=center><TT>^I</tt></td><td align=center>Î</td>
	<td align=center><TT>TH</tt></td><td align=center>Þ</td>
	<td align=center><TT>^i</tt></td><td align=center>î</td>
	<td align=center><TT>th</tt></td><td align=center>þ
</tr><tr>
	<td align=center><TT>_</tt></td><td align=center>¯</td>
	<td align=center><TT>?</tt></td><td align=center>¿</td>
	<td align=center><TT>:I</tt></td><td align=center>Ï</td>
	<td align=center><TT>ss</tt></td><td align=center>ß</td>
	<td align=center><TT>:i</tt></td><td align=center>ï</td>
	<td align=center><TT>:y</tt></td><td align=center>ÿ
</table></center>

<p>For instance, to type "á" type [compose][a]['] or [compose]['][a].

<P>The character "nbsp" (non-breaking space) is typed by using
[compose][space].

<P>The single-character sequences may be followed by a space if
necessary to remove ambiguity.  For instance, if you really want to
type "ª~" rather than "ã" you must type [compose][a][space][~].

<p>The same key may be used to "quote" control characters into the
text.  If you need a <tt>^Q</tt> character you can get one by typing
[compose][Control+Q].

<p>X may have a key on the keyboard
defined as <tt>XK_Multi_key</tt>.  If so this key may be used as well
as the right-hand control key.  You can set this up with the program
<tt>xmodmap</tt>.

<p>If your keyboard is set to support a foreign language you should
also be able to type "dead key" prefix characters.  On X you will
actually be able to see what dead key you typed, and if you then move
the cursor without completing the sequence the accent will remain
inserted.
</TABLE>

when(Fl_When) controls when callbacks are done. 
The following values are useful, the default value is FL_WHEN_RELEASE: 
- 0: The callback is not done, but changed() is turned on. 
- FL_WHEN_CHANGED: 
   The callback is done each time the text is changed by the user. 
- FL_WHEN_RELEASE: 
   The callback will be done when this widget loses the focus, 
   including when the window is unmapped. This is a useful value 
   for text fields in a panel where doing the callback on every 
   change is wasteful. However the callback will also happen if 
   the mouse is moved out of the window, which means it should not
   do anything visible (like pop up an error message). You might 
   do better setting this to zero, and scanning all the items for 
   changed() when the OK button on a panel is pressed. 
- FL_WHEN_ENTER_KEY: 
   If the user types the Enter key, the entire text is selected, and the 
   callback is done if the text has changed. Normally the Enter key will 
   navigate to the next field (or insert a newline for a 
   Fl_Mulitline_Input), this changes the behavior. 
- FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED: 
   The Enter key will do the callback even if the text has not changed. 
   Useful for command fields. 

*/
class FL_API Fl_Input : public Fl_Widget {
public:
  enum { // values for type()
    NORMAL = 0,
    // this hole was used by float & int types
    SECRET = 3,
    MULTILINE = 4,
    WORDWRAP = 5
  };

  /**
   Creates a new Fl_Input widget using the given position, size, 
   and label string. The default boxtype is FL_DOWN_BOX. 
  */
  Fl_Input(int x, int y, int w, int h, const char* label= 0);
  /** The destructor removes the widget and any value associated with it. */
  ~Fl_Input();
  static Fl_Named_Style* default_style;

  void draw();
  void draw(int, int, int, int);
  int handle(int);
  int handle(int event, int, int, int, int);

  /*@{*/
  /**
   The first form returns the current value, which is a pointer to the 
   internal buffer and is valid only until the next event is handled. 

   The second two forms change the text and set the mark and the point to
   the end of it. The string is copied to the internal buffer. Passing NULL 
   is the same as "". This returns non-zero if the new value is different than
   the current one. You can use the second version to directly set the length 
   if you know it already or want to put nul's in the text. 
  */
  bool value(const char*);
  bool value(const char*, int);
  const char* value() const {return value_;}
  /*@}*/

  /*@{*/
  /**
   Change the text and set the mark and the point to the end of it.
   The string is not copied. If the user edits the string it is copied
   to the internal buffer then. This can save a great deal of time and
   memory if your program is rapidly changing the values of text fields,
   but this will only work if the passed string remains unchanged until
   either the Fl_Input is destroyed or value() is called again. 
  */
  bool static_value(const char*);
  bool static_value(const char*, int);
  /*@}*/

  /**
   Same as value()[n], but may be faster in plausible implementations. 
   No bounds checking is done. 
  */
  char index(int i) const {return value_[i];}
  /**
   Returns the number of characters in value(). This may be greater 
   than strlen(value()) if there are nul characters in it.
  */
  int size() const {return size_;}

  /*@{*/
  /**
   The input widget maintains two pointers into the string. The "position" 
   is where the cursor is. The "mark" is the other end of the selected text.
   If they are equal then there is no selection. Changing this does not affect
   the clipboard (use copy() to do that). 

   Changing these values causes a redraw(). The new values are bounds checked.
   The return value is non-zero if the new position is different than the old 
   one. position(n) is the same as position(n,n). mark(n) is the same as 
   position(position(),n). 
  */
  int position() const {return position_;}
  void position(int p, int m);
  void position(int p) {position(p, p);}
  /*@}*/
  /**
   Do the correct thing for arrow keys. i must be the location of 
   the start of a line. Sets the position (and mark if keepmark is
   zero) to somewhere after i, such that pressing the arrows
   repeatedly will cause the point to move up and down. 
  */
  void up_down_position(int position, bool extend);

  /*@{*/
  /**
   Gets or sets the current selection mark. 
   mark(n) is the same as position(position(),n).
   */
  int mark() const {return mark_;}
  void mark(int m) { position(position(), m);}
  /*@}*/

  /**
   This call does all editing of the text. It deletes the region between 
   a and b (either one may be less or equal to the other), and then 
   inserts length (which may be zero) characters from the string insert
   at that point and leaves the mark() and position() after the insertion.
   Does the callback if when()&FL_WHEN_CHANGED and there is a change.
   
   Subclasses of Fl_Input can override this function to control what 
   characters can be inserted into the text. A typical implementation 
   will check the characters in the insertion for legality and then call
   Fl_Input::replace only if they are all ok. 

   Subclasses should return true if the keystroke that produced this call
   should be "eaten". If false is returned the keystroke is allowed to be
   tested as a shortcut for other widgets. In our experience it is best to 
   return true even if you don't make changes. The base class version 
   returns true always. 
  */
  virtual bool replace(int a, int b, const char* insert, int length);
  /*@{*/
  /**
   These are wrappers around replace(). Fl_Input::cut() deletes the area
   between mark() and position(). cut(n) deletes n characters after the 
   position(). cut(-n) deletes n characters before the position() . 
   cut(a,b) deletes the characters between offsets a and b. A, b, and n 
   are all clamped to the size of the string. The mark and point are left 
   where the deleted text was. 

   If you want the data to go into the clipboard, do Fl_Input::copy() 
   before calling Fl_Input::cut(). 
  */
  bool cut() {return replace(position(), mark(), 0, 0);}
  bool cut(int n) {return replace(position(), position()+n, 0, 0);}
  bool cut(int a, int b) {return replace(a, b, 0, 0);}
  /*@}*/
  /**
   Insert the string t at the current position, and leave the mark and 
   position after it. If l is not zero then it is assummed to be 
   strlen(t). 
  */
  bool insert(const char* t, int l=0){return replace(position_, mark_, t, l);}
  bool replace(int a, int b, char c) {return replace(a,b,&c,1);}
  /**
   Put the current selection between mark() and position() into the 
   clipboard. Does not replace the old clipboard contents if 
   position() and mark() are equal. This calls Fl::copy(). 

   If clipboard is true the text is put into the user-visible 
   cut & paste clipboard (this is probably what you want). If false
   it is put into the less-visible selection buffer that is used to do 
   middle-mouse paste and drag & drop. 

   To paste the clipboard, call Fl::paste(true) and fltk will send the widget
   a FL_PASTE event with the text, which will cause it to be inserted. 
  */
  bool copy(bool clipboard = true);
  /**
   Does undo of several previous calls to replace(). 
   Returns non-zero if any change was made.
  */
  bool undo();
  /**
   Does the callback if changed() is true or if when() 
   FL_WHEN_NOT_CHANGED is non-zero. You should call this 
   at any point you think you should generate a callback. 
  */
  void maybe_do_callback();

  /** Returns the location of the first word boundary at or before position. */
  int word_start(int position) const;
  /** Returns the location of the next word boundary at or after position. */
  int word_end(int position) const;
  /** Returns the location of the start of the line containing the position. */
  int line_start(int position) const;
  /** 
   Returns the location of the next newline or wordwrap space at 
   or after position.
   */
  int line_end(int i) const;
  int mouse_position(int, int, int, int) const;
  int xscroll() const {return xscroll_;}
  int yscroll() const {return yscroll_;}

private:

  const char* value_;
  char* buffer;

  int size_;
  int bufsize;
  int position_;
  int mark_;
  int xscroll_, yscroll_;
  int mu_p;
  int label_width;

  const char* expand(const char*, char*, int) const;
  float expandpos(const char*, const char*, const char*, int*) const;
  void minimal_update(int, int);
  void minimal_update(int p);
  void erase_cursor_at(int p);
  void put_in_buffer(int newsize);

  void setfont() const;

  bool handle_key();
  bool key_is_shortcut();
  void shift_position(int p);
  void shift_up_down_position(int p);

};

#ifndef FLTK_2
#define FL_NORMAL_INPUT		Fl_Input::NORMAL
#define FL_SECRET_INPUT		Fl_Input::SECRET
#define FL_MULTILINE_INPUT	Fl_Input::MULTILINE
#define FL_WORDWRAP_INPUT	Fl_Input::WORDWRAP
#endif

#endif

//
// End of "$Id: Fl_Input.h,v 1.10 2002/10/29 00:37:23 easysw Exp $".
//
