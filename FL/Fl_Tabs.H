/
// "$Id$
/
// Tab header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Tabs_
#define Fl_Tabs_

#include "Fl_Group.H

/** The Fl_Tabs widget is the "file card tabs" interface that allows you t
 * put lots and lots of buttons and switches in a panel, as popularized b
 * many toolkits
 *   \image html tabs.gi
 * Clicking the tab makes a child visible() by calling show() on it, and al
 * other children are made invisible by calling hide() on them. Usually th
 * children are Fl_Group widgets containing several widgets themselves
 
 * Each child makes a card, and it's label() is printed on the card tab
 * including the label font and style. The selection color of that child i
 * used to color the tab, while the color of the child determines th
 * background color of the pane
 
 * The size of the tabs is controlled by the bounding box of the childre
 * (there should be some space between the children and the edge of th
 * Fl_Tabs), and the tabs may be placed "inverted" on the bottom, this i
 * determined by which gap is larger. It is easiest to lay this out in fluid
 * using the fluid browser to select each child group and resize them unti
 * the tabs look the way you want them to.
 *
class FL_EXPORT Fl_Tabs : public Fl_Group 
  Fl_Widget *value_
  Fl_Widget *push_
  int tab_positions(int*, int*)
  int tab_height()
  void draw_tab(int x1, int x2, int W, int H, Fl_Widget* o, int sel=0)
protected
  void draw()

public
  int handle(int)
    /** Gets the currently visible widget/tab. *
  Fl_Widget *value()
    /** Sets the currently visible widget/tab. *
  int value(Fl_Widget *)
  Fl_Widget *push() const {return push_;
  int push(Fl_Widget *)
    /** Creates a new Fl_Tabs widget using the given position, size, an
     * label string. The default boxtype is \c FL_THIN_UP_BOX
     
     * Use add(Fl_Widget *) to add each child, which are usually Fl_Grou
     * widgets. The children should be sized to stay away from the top o
     * bottom edge of the Fl_Tabs widget, which is where the tabs will b
     * drawn. *
  Fl_Tabs(int x,int y,int width,int height,const char *label = 0)
  Fl_Widget *which(int event_x, int event_y)
}

#endi

/
// End of "$Id$"
/
