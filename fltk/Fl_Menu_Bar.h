//
// "$Id: Fl_Menu_Bar.h,v 1.1 2001/07/23 09:50:04 spitzak Exp $"
//
// Menu bar header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu_Bar_H
#define Fl_Menu_Bar_H

#include "Fl_Menu_.h"

class FL_API Fl_Menu_Bar : public Fl_Menu_ {
public:
  Fl_Menu_Bar(int x,int y,int w,int h,const char *l=0);
  static Fl_Named_Style* default_style;
  int handle(int);
protected:
  void draw();
private:
  int highlight_, last_;
};

#endif

//
// End of "$Id: Fl_Menu_Bar.h,v 1.1 2001/07/23 09:50:04 spitzak Exp $".
//
