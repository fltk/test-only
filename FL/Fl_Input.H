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
 *
 * \copydoc _input_key
 * \htmlinclude character_table.htm
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
