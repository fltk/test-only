//
// "$Id: Fl_Button.H,v 1.13 1999/09/19 04:58:52 vincent Exp $"
//
// Button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Button_H
#define Fl_Button_H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif

// values for type()
#define FL_TOGGLE_BUTTON        1
#define FL_RADIO_BUTTON         (FL_RESERVED_TYPE+2)
#define FL_HIDDEN_BUTTON        3 // for Forms compatability

FL_IMPORT extern int fl_old_shortcut(const char*);

class Fl_Button : public Fl_Widget {
public:
  virtual int handle(int);
  Fl_Button(int,int,int,int,const char * = 0);
  int shortcut() const {return shortcut_;}
  void shortcut(int s) {shortcut_ = s;}

  // back compatability:
  void shortcut(const char *s) {shortcut(fl_old_shortcut(s));}

protected:
  virtual void draw();

private:
  int shortcut_;
};

#endif

//
// End of "$Id: Fl_Button.H,v 1.13 1999/09/19 04:58:52 vincent Exp $".
//
