//
// "$Id: Fl_Tabs.h,v 1.5 2002/10/29 00:37:23 easysw Exp $"
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

#include "Fl_Group.h"

/**

   The Fl_Tabs widget is the "file card tabs" interface that allows you to 
   put lots and lots of buttons and switches in a panel, as popularized by
   many toolkits. 

   \image tabs.gif

   Clicking the tab makes a child visible() (by calling show() on it) and 
   all other children are invisible (by calling hide() on them). Usually 
   the children are Fl_Group widgets containing several widgets themselves. 

   Each child makes a card, and it's label() is printed on the card tab 
   (including the label font and style). The color of that child is used 
   to color the card as well. Currently this only draws nicely if you set
   box() to the default FL_THIN_UP_BOX or to FL_FLAT_BOX, which gets rid 
   of the edges drawn on the sides and bottom. 

   The size of the tabs is controlled by the bounding box of the children
   (there should be some space between the children and the edge of the 
   Fl_Tabs), and the tabs may be placed "inverted" on the bottom, this 
   is determined by which gap is larger. It is easiest to lay this out 
   in fluid, using the fluid browser to select each child group and 
   resize them until the tabs look the way you want them to. 


*/
class FL_API Fl_Tabs : public Fl_Group {

public:
  int handle(int);

  /**
   Creates a new Fl_Tab widget using the given position, size, and
   label string. The default boxtype is FL_THIN_UP_BOX. 

   Use add(Fl_Widget *) to add each child (which is probably itself a Fl_Group). 
   The children should be sized to stay away from the top or bottom edge of the 
   Fl_Tabs, which is where the tabs are drawn.
  */
  Fl_Tabs(int,int,int,int,const char * = 0);
  static Fl_Named_Style* default_style;

  /*@{*/
  /** Gets or sets the currently visible widget/tab. */
  Fl_Widget *value();
  int value(Fl_Widget *);
  /*@}*/
  Fl_Widget *push() const {return push_;}
  int push(Fl_Widget *);
  Fl_Widget *which(int event_x, int event_y);

protected:
  void draw();

private:
  friend class Fl_Group;
  Fl_Widget *push_;
  int tab_positions(int*, int*);
  int tab_height();
  void draw_tab(int x1, int x2, int W, int H, Fl_Widget* o, int sel=0);
};

#endif

//
// End of "$Id: Fl_Tabs.h,v 1.5 2002/10/29 00:37:23 easysw Exp $".
//
