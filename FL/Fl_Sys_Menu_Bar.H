/
// "$Id$
/
// MacOS system menu bar header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Sys_Menu_Bar_
#define Fl_Sys_Menu_Bar_

#include "Fl_Menu_Bar.H

class FL_EXPORT Fl_Sys_Menu_Bar : public Fl_Menu_Bar 
protected
  void draw()
public
//int handle(int)
  Fl_Sys_Menu_Bar(int x,int y,int w,int h,const char *l=0
      : Fl_Menu_Bar(x,y,w,h,l) {
  void menu(const Fl_Menu_Item *m)
//  Fl_Menu_Item* picked(const Fl_Menu_Item* v)
}

#endi

/
// End of "$Id$"
/
