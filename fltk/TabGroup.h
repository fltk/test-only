//
// "$Id$"
//
// For making a "tabbed dialog box". Each child widget creates a tab.
// Only one is visible at a time. This works best if all the children
// have the same size and they don't cover a strip at the top for the
// tabs.
//
// Copyright 1998-2006 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef fltk_TabGroup_h
#define fltk_TabGroup_h

#include "Group.h"

namespace fltk {

class FL_API TabGroup : public Group {

public:
  int handle(int);

  TabGroup(int,int,int,int,const char * = 0);
  static NamedStyle* default_style;

  int value();
  bool value(int);
  int which(int event_x, int event_y);
  Widget *selected_child();
  bool selected_child(Widget *);
  void set_draw_outline( bool draw_outline );

protected:
  void draw();

  int tab_height();
  void draw_tab(int x1, int x2, int W, int H, Widget* o, int sel=0);
  void draw_tab_background();

private:
  int push(Widget *);
  int tab_positions(int*, int*);

  bool _drawOutline;
};

}

#endif

//
// End of "$Id$".
//
