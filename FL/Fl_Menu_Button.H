//
// "$Id: Fl_Menu_Button.H,v 1.20 2000/02/14 11:32:35 bill Exp $"
//
// Menu button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu_Button_H
#define Fl_Menu_Button_H

#include "Fl_Menu_.H"

class FL_API Fl_Menu_Button : public Fl_Menu_ {
public:
  // values for type:
  enum {NORMAL=FL_GROUP, POPUP1, POPUP2, POPUP12, POPUP3, POPUP13, POPUP23, POPUP123};
  int handle(int);
  int popup();
  Fl_Menu_Button(int,int,int,int,const char * =0);

protected:
  void draw();
  void draw_n_clip();

};

#endif

//
// End of "$Id: Fl_Menu_Button.H,v 1.20 2000/02/14 11:32:35 bill Exp $".
//
