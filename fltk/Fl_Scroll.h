//
// "$Id: Fl_Scroll.h,v 1.2 2002/10/29 00:37:23 easysw Exp $"
//
// Scroll header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Scroll_H
#define Fl_Scroll_H

#include "Fl_Group.h"
#include "Fl_Scrollbar.h"

/**

   This container widget lets you maneuver around a set of widgets much 
   larger than your window. If the child widgets are larger than the 
   size of this object then scrollbars will appear so that you can 
   scroll over to them: 

   \image Fl_Scroll.gif

   If all of the child widgets are packed together into a solid rectangle
   then you want to set box() to FL_NO_BOX or one of the _FRAME types. 
   This will result in the best output. However, if the child widgets are
   a sparse arrangment you must set box() to a real _BOX type. This can 
   result in some blinking during redrawing, but that can be solved by 
   using a Fl_Double_Window. 

   This widget can also be used to pan around a single child widget "canvas".
   This child widget should be of your own class, with a draw() method that 
   draws the contents. The scrolling is done by changing the x() and y() of
   the widget, so this child must use the x() and y() to position it's 
   drawing. To speed up drawing it should test fl_not_clipped() . 

   Another very useful child is a single Fl_Pack, which is itself a group
   that packs it's children together and changes size to surround them. 
   Filling the Fl_Pack with Fl_Tabs groups (and then putting normal widgets
   inside those) gives you a very powerful scrolling list of 
   individually-openable panels. 

   Fluid lets you create these, but you can only lay out objects that fit 
   inside the Fl_Scroll without scrolling. Be sure to leave space for the
   scrollbars, as Fluid won't show these either. 

   You cannot use Fl_Window as a child of this since the clipping is not 
   conveyed to it when drawn, and it will draw over the scrollbars and 
   neighboring objects.

*/
class FL_API Fl_Scroll : public Fl_Group {

  int xposition_, yposition_;
  int layoutdx, layoutdy;
  int scrolldx, scrolldy;
  static void hscrollbar_cb(Fl_Widget*, void*);
  static void scrollbar_cb(Fl_Widget*, void*);
  static void draw_clip(void*,int,int,int,int);
  Fl_End endgroup;

protected:

  void layout();
  void draw();

public:

  void bbox(int&,int&,int&,int&);
  Fl_Scrollbar scrollbar;
  Fl_Scrollbar hscrollbar;

  int handle(int);

  /**
   Creates a new Fl_Scroll widget using the given position, size, and 
   label string. The default boxtype is FL_NO_BOX. 
  */
  Fl_Scroll(int X,int Y,int W,int H,const char*l=0);

  /**
   By default you can scroll in both directions, and the scrollbars
   disappear if the data will fit in the area of the scroll. 
   type() can change this: 

   - 0 - No scrollbars 
   - Fl_Scroll::HORIZONTAL - Only a horizontal scrollbar. 
   - Fl_Scroll::VERTICAL - Only a vertical scrollbar. 
   - Fl_Scroll::BOTH - The default is both scrollbars. 
   - Fl_Scroll::HORIZONTAL_ALWAYS - Horizontal scrollbar always on, vertical always off. 
   - Fl_Scroll::VERTICAL_ALWAYS - Vertical scrollbar always on, horizontal always off. 
   - Fl_Scroll::BOTH_ALWAYS - Both always on. 
  */
  enum { // values for type()
    HORIZONTAL = 1,
    VERTICAL = 2,
    BOTH = 3,
    ALWAYS_ON = 4,
    HORIZONTAL_ALWAYS = 5,
    VERTICAL_ALWAYS = 6,
    BOTH_ALWAYS = 7
  };

  /** Gets the current horizontal scrolling position. */
  int xposition() const {return xposition_;}
  /** Gets the current vertical scrolling position. */
  int yposition() const {return yposition_;}
  /** Sets the upper-lefthand corner of the scrolling region. */
  void position(int x, int y);
};

#endif

//
// End of "$Id: Fl_Scroll.h,v 1.2 2002/10/29 00:37:23 easysw Exp $".
//
