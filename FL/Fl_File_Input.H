//
// "$Id$"
//
// File_Input header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
// Original version Copyright 1998 by Curtis Edwards.
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

#ifndef Fl_File_Input_H
#  define Fl_File_Input_H

#  include <FL/Fl_Input.H>


/** This widget displays a pathname in a text input field. A navigation bar
 * located above the input field allows the user to navigate upward in the
 * directory tree. 
 *   \image html Fl_File_Input.gif
 */
class FL_EXPORT Fl_File_Input : public Fl_Input
{
  Fl_Color	errorcolor_;
  char		ok_entry_;
  Fl_Boxtype		down_box_;
  short		buttons_[200];
  short		pressed_;

  void		draw_buttons();
  int		handle_button(int event);
  void		update_buttons();

public:

    /** Creates a new Fl_File_Input widget using the given position, size, 
     * and label string. The default boxtype is \c FL_DOWN_BOX. */
  Fl_File_Input(int x,int y,int width,int height,const char *label=0);

  virtual int handle(int event);
  virtual void draw();

    /** Gets the box type to use for the navigation bar. */
  Fl_Boxtype	down_box() const { return (Fl_Boxtype)down_box_; }
    /** Sets the box type to use for the navigation bar. */
  void		down_box(Fl_Boxtype b) { down_box_ = b; }
  Fl_Color	errorcolor() const { return errorcolor_; }
  void		errorcolor(Fl_Color c) { errorcolor_ = c; }
  int	value(const char*);
  int	value(const char*, int);
  const char	*value() { return Fl_Input_::value(); }
};

#endif // !Fl_File_Input_H


//
// End of "$Id$".
//
