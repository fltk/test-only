/
// "$Id$
/
// Output header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Output_
#define Fl_Output_

#include "Fl_Input.H

/** This widget displays a piece of text. When you set the value(),
 * Fl_Output does a \c strcpy() to it's own storage, which is useful for
 * program-generated values. The user may select portions of the text usin
 * the mouse and paste the contents into other fields or programs
 *   \image html text.gi
 * There is a single subclass, Fl_Multiline_Output, which allows you to
 * display multiple lines of text
 
 * The text may contain any characters except \\0, and will correctl
 * display anything, using ^X notation for unprintable control character
 * and \\nnn notation for unprintable characters with the high bit set. I
 * assumes the font can draw any characters in the ISO-Latin
 * character set.
 *
class Fl_Output : public Fl_Input 
public
    /** Creates a new Fl_Output widget using the given position, size, an
     * label string. The default boxtype is \c FL_DOWN_BOX. *
  Fl_Output(int X,int Y,int W,int H, const char *l = 0
    : Fl_Input(X, Y, W, H, l) {type(FL_NORMAL_OUTPUT);
}

#endif

/
// End of "$Id$"
/
