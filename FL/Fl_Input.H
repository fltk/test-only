/
// "$Id$
/
// Input header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Input_
#define Fl_Input_

#include "Fl_Input_.H

/** This is the FLTK text input widget. It displays a single line of text
 * and lets the user edit it. Normally it is drawn with an inset box and 
 * white background. The text may contain any characters (even 0), and wil
 * correctly display anything, using ^X notation for unprintable control
 * characters and \\nnn notation for unprintable characters with the high bi
 * set. It assumes the font can draw any characters in the ISO-8859-
 * character set.
<CENTER><TABLE border=1 WIDTH=90% summary="Fl_Input keyboard and mouse bindings."

<TR><TD WIDTH=200><B>Mouse button 1</B></TD><TD>Moves the cursor t
 this point. Drag selects characters.  Double click selects words
 Triple click selects all text.  Shift+click extends the selection
 When you select text it is automatically copied to the clipboard
</TD></TR

<TR><TD><B>Mouse button 2</B></TD><TD>Insert the clipboard a
the point clicked.  You can also select a region and replace it with th
clipboard by selecting the region with mouse button 2
</TD></TR

<TR><TD><B>Mouse button 3</B></TD><TD>Currently acts like button 1.</TD></TR

<TR><TD><B>Backspace</B></TD><TD>Deletes one character to the left, o
deletes the selected region.</TD></TR
<TR><TD><B>Enter</B></TD><TD>May cause the callback, see when().</TD></TR
<TR><TD><B>^A or Home</B></TD><TD>Go to start of line.</TD></TR
<TR><TD><B>^B or Left</B></TD><TD>Move left</TD></TR
<TR><TD><B>^C</B></TD><TD>Copy the selection to the clipboard</TD></TR
<TR><TD><B>^D or Delete</B></TD><TD>Deletes one character to the righ
or deletes the selected region.</TD></TR
<TR><TD><B>^E or End</B></TD><TD>Go to the end of line.</TD></TR
<TR><TD><B>^F or Right</B></TD><TD>Move right</TD></TR
<TR><TD><B>^K</B></TD><TD>Delete to the end of line (next \n character
or deletes a single \n character.  These deletions are all concatenate
into the clipboard.</TD></TR
<TR><TD><B>^N or Down</B></TD><TD>Move down (for Fl_Multiline_Inpu
only, otherwise it moves to the next input field).</TD></TR
<TR><TD><B>^P or Up</B></TD><TD>Move up (for Fl_Multiline_Input only
otherwise it moves to the previous input field).</TD></TR
<TR><TD><B>^U</B></TD><TD>Delete everything.</TD></TR
<TR><TD><B>^V or ^Y</B></TD><TD>Paste the clipboard</TD></TR
<TR><TD><B>^X or ^W</B></TD><TD>Copy the region to the clipboard an
delete it.</TD></TR
<TR><TD><B>^Z or ^_</B></TD><TD>Undo.  This is a single-level und
mechanism, but all adjacent deletions and insertions are concatenate
into a single &quot;undo&quot;.  Often this will undo a lot more than yo
expected.</TD></TR
<TR><TD><B>Shift+move</B></TD><TD>Move the cursor but also extend th
selection.</TD></TR

<TR><TD><B>RightCtrl o
<BR>Compose</B></TD><TD><a name=compose>Start</A
a <A href="events.html#compose">compose-character</A
sequence.  The next one or two keys typed define the character t
insert (see table that follows.

<p>For instance, to type "&aacute;" type [compose][a]['] or [compose]['][a]

<P>The character "nbsp" (non-breaking space) is typed by usin
[compose][space]

<P>The single-character sequences may be followed by a space i
necessary to remove ambiguity.  For instance, if you really want t
type &quot;&ordf;~&quot; rather than &quot;&atilde;&quot; you must type [compose][a][space][~]

<p>The same key may be used to "quote" control characters into th
text.  If you need a <tt>^Q</tt> character you can get one by typin
[compose][Control+Q]

<p>X may have a key on the keyboar
defined as <tt>XK_Multi_key</tt>.  If so this key may be used as wel
as the right-hand control key.  You can set this up with the progra
<tt>xmodmap</tt>

<p>If your keyboard is set to support a foreign language you shoul
also be able to type "dead key" prefix characters.  On X you wil
actually be able to see what dead key you typed, and if you then mov
the cursor without completing the sequence the accent will remai
inserted
</TABLE></CENTER

<center><table border=1 summary="Character Composition Table"
<caption align="top">Character Composition Table</caption
<tr
	<th>Keys</th><th>Char</th
	<th>Keys</th><th>Char</th
	<th>Keys</th><th>Char</th
	<th>Keys</th><th>Char</th
	<th>Keys</th><th>Char</th
	<th>Keys</th><th>Char</th

</tr><tr
	<td align=center><TT>sp</TT></td><td align=center><small>nbsp</small></td
	<td align=center><TT>*</tt></td><td align=center>&deg;</td
	<td align=center><TT>` A</tt></td><td align=center>&Agrave;</td
	<td align=center><TT>D -</tt></td><td align=center>&ETH;</td
	<td align=center><TT>` a</tt></td><td align=center>&agrave;</td
	<td align=center><TT>d -</tt></td><td align=center>&eth
</tr><tr
	<td align=center><TT>!</tt></td><td align=center>&iexcl;</td
	<td align=center><TT>+ -</tt></td><td align=center>&plusmn;</td
	<td align=center><TT>' A</tt></td><td align=center>&Aacute;</td
	<td align=center><TT>~ N</tt></td><td align=center>&Ntilde;</td
	<td align=center><TT>' a</tt></td><td align=center>&aacute;</td
	<td align=center><TT>~ n</tt></td><td align=center>&ntilde
</tr><tr
	<td align=center><TT>%</tt></td><td align=center>&cent;</td
	<td align=center><TT>2</tt></td><td align=center>&sup2;</td
	<td align=center><TT>A ^</tt></td><td align=center>&Acirc;</td
	<td align=center><TT>` O</tt></td><td align=center>&Ograve;</td
	<td align=center><TT>^ a</tt></td><td align=center>&acirc;</td
	<td align=center><TT>` o</tt></td><td align=center>&ograve
</tr><tr
	<td align=center><TT>#</tt></td><td align=center>&pound;</td
	<td align=center><TT>3</tt></td><td align=center>&sup3;</td
	<td align=center><TT>~ A</tt></td><td align=center>&Atilde;</td
	<td align=center><TT>' O</tt></td><td align=center>&Oacute;</td
	<td align=center><TT>~ a</tt></td><td align=center>&atilde;</td
	<td align=center><TT>' o</tt></td><td align=center>&oacute
</tr><tr
	<td align=center><TT>$</tt></td><td align=center>&curren;</td
	<td align=center><TT>'</tt></td><td align=center>'</td
	<td align=center><TT>: A</tt></td><td align=center>&Auml;</td
	<td align=center><TT>^ O</tt></td><td align=center>&Ucirc;</td
	<td align=center><TT>: a</tt></td><td align=center>&auml;</td
	<td align=center><TT>^ o</tt></td><td align=center>&ocirc
</tr><tr
	<td align=center><TT>y =</tt></td><td align=center>&yen;</td
	<td align=center><TT>u</tt></td><td align=center>&micro;</td
	<td align=center><TT>* A</tt></td><td align=center>&Aring;</td
	<td align=center><TT>~ O</tt></td><td align=center>&Otilde;</td
	<td align=center><TT>* a</tt></td><td align=center>&aring;</td
	<td align=center><TT>~ o</tt></td><td align=center>&otilde
</tr><tr
	<td align=center><TT>|</tt></td><td align=center>&brvbar;</td
	<td align=center><TT>p</tt></td><td align=center>&para;</td
	<td align=center><TT>A E</tt></td><td align=center>&AElig;</td
	<td align=center><TT>: O</tt></td><td align=center>&Ouml;</td
	<td align=center><TT>a e</tt></td><td align=center>&aelig;</td
	<td align=center><TT>: o</tt></td><td align=center>&ouml
</tr><tr
	<td align=center><TT>&amp;</tt></td><td align=center>&sect;</td
	<td align=center><TT>.</tt></td><td align=center>&middot;</td
	<td align=center><TT>, C</tt></td><td align=center>&Ccedil;</td
	<td align=center><TT>x</tt></td><td align=center>&times;</td
	<td align=center><TT>, c</tt></td><td align=center>&ccedil;</td
	<td align=center><TT>- :</tt></td><td align=center>&divide
</tr><tr
	<td align=center><TT>:</tt></td><td align=center>&uml;</td
	<td align=center><TT>,</tt></td><td align=center>&cedil;</td
	<td align=center><TT>E `</tt></td><td align=center>&Egrave;</td
	<td align=center><TT>O /</tt></td><td align=center>&Oslash;</td
	<td align=center><TT>` e</tt></td><td align=center>&egrave;</td
	<td align=center><TT>o /</tt></td><td align=center>&oslash
</tr><tr
	<td align=center><TT>c</tt></td><td align=center>&copy;</td
	<td align=center><TT>1</tt></td><td align=center>&sup1;</td
	<td align=center><TT>' E</tt></td><td align=center>&Eacute;</td
	<td align=center><TT>` U</tt></td><td align=center>&Ugrave;</td
	<td align=center><TT>' e</tt></td><td align=center>&eacute;</td
	<td align=center><TT>` u</tt></td><td align=center>&ugrave
</tr><tr
	<td align=center><TT>a</tt></td><td align=center>&ordf;</td
	<td align=center><TT>o</tt></td><td align=center>&ordm;</td
	<td align=center><TT>^ E</tt></td><td align=center>&Ecirc;</td
	<td align=center><TT>' U</tt></td><td align=center>&Uacute;</td
	<td align=center><TT>^ e</tt></td><td align=center>&ecirc;</td
	<td align=center><TT>' u</tt></td><td align=center>&uacute
</tr><tr
	<td align=center><TT>&lt; &lt;</tt></td><td align=center><small>&lt;&lt;</small></td
	<td align=center><TT>&gt; &gt;</tt></td><td align=center><small>&gt;&gt;</small></td
	<td align=center><TT>: E</tt></td><td align=center>&Euml;</td
	<td align=center><TT>^ U</tt></td><td align=center>&Ucirc;</td
	<td align=center><TT>: e</tt></td><td align=center>&euml;</td
	<td align=center><TT>^ u</tt></td><td align=center>&ucirc
</tr><tr
	<td align=center><TT>~</tt></td><td align=center>&not;</td
	<td align=center><TT>1 4</tt></td><td align=center>&frac14;</td
	<td align=center><TT>` I</tt></td><td align=center>&Igrave;</td
	<td align=center><TT>: U</tt></td><td align=center>&Uuml;</td
	<td align=center><TT>` i</tt></td><td align=center>&igrave;</td
	<td align=center><TT>: u</tt></td><td align=center>&uuml
</tr><tr
	<td align=center><TT>-</tt></td><td align=center>­&shy;</td
	<td align=center><TT>1 2</tt></td><td align=center>&frac12;</td
	<td align=center><TT>' I</tt></td><td align=center>&Iacute;</td
	<td align=center><TT>' Y</tt></td><td align=center>&Yacute;</td
	<td align=center><TT>' i</tt></td><td align=center>&iacute;</td
	<td align=center><TT>' y</tt></td><td align=center>&yacute
</tr><tr
	<td align=center><TT>r</tt></td><td align=center>&reg;</td
	<td align=center><TT>3 4</tt></td><td align=center>&frac34;</td
	<td align=center><TT>^ I</tt></td><td align=center>&Icirc;</td
	<td align=center><TT>T H</tt></td><td align=center>&THORN;</td
	<td align=center><TT>^ i</tt></td><td align=center>&icirc;</td
	<td align=center><TT>t h</tt></td><td align=center>&thorn
</tr><tr
	<td align=center><TT>_</tt></td><td align=center>&macr;</td
	<td align=center><TT>?</tt></td><td align=center>&iquest;</td
	<td align=center><TT>: I</tt></td><td align=center>&Iuml;</td
	<td align=center><TT>s s</tt></td><td align=center>&szlig;</td
	<td align=center><TT>: i</tt></td><td align=center>&iuml;</td
	<td align=center><TT>: y</tt></td><td align=center>&yuml
</table></center>
 *
class FL_EXPORT Fl_Input : public Fl_Input_ 
  int handle_key()
  int shift_position(int p)
  int shift_up_down_position(int p)
  void handle_mouse(int keepmark=0)
public
  void draw()
  int handle(int)
    /** Creates a new Fl_Input widget using the given position, size, an
     * label string. The default boxtype is \c FL_DOWN_BOX. *
  Fl_Input(int x,int y,int width,int height,const char *label = 0)
}

#endif

/
// End of "$Id$"
/
