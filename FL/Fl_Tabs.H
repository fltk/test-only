//
// "$Id: Fl_Tabs.H,v 1.15 2000/01/10 06:31:04 bill Exp $"
//
// Tab header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Tabs_H
#define Fl_Tabs_H

#include "Fl_Group.H"

#define FL_TABS (FL_GROUP+1)

class FL_API Fl_Tabs : public Fl_Group {
public:
  int handle(int);
  Fl_Widget *value();
  int value(Fl_Widget *);
  Fl_Widget *push() const {return push_;}
  int push(Fl_Widget *);
  Fl_Tabs(int,int,int,int,const char * = 0);
  Fl_Widget *which(int event_x, int event_y);
private:
  friend class Fl_Group;
  Fl_Widget *push_;
  void draw();
  void draw_n_clip();
  int tab_positions(int*, int*);
  int tab_height();
  void draw_tab(int x1, int x2, int W, int H, Fl_Widget* o, int sel=0);
};

#endif

//
// End of "$Id: Fl_Tabs.H,v 1.15 2000/01/10 06:31:04 bill Exp $".
//
