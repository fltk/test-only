//
// "$Id: Fl_Scroll.H,v 1.5.2.3.2.2.2.2 2003/11/17 07:15:16 matthiaswm Exp $"
//
// Scroll header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#ifndef Fl_Scroll_H
#define Fl_Scroll_H

#include "Fl_Group.H"
#include "Fl_Scrollbar.H"

/** This container widget lets you maneuver around a set of widgets much
 * larger than your window. If the child widgets are larger than the
 * size of this object then scrollbars will appear so that you can scroll
 * over to them:
 *   \image html Fl_Scroll.gif
 * If all of the child widgets are packed together into a solid rectangle 
 * then you want to setbox() to \c FL_NO_BOX or one of the \c _FRAME types. 
 * This will result in the best output. However, if the child widgets are a
 * sparse arrangment you must set box() to a real <tt>_BOX</tt>type. This
 * can result in some blinking during redrawing, but that can be solved by
 * using a Fl_Double_Window.
 *
 * This widget can also be used to pan around a single child widget 
 * "canvas". This child widget should be of your own class, with a draw() 
 * method that draws the contents. The scrolling is done by changing the 
 * x() and y() of the widget, so this child must use the x() and y() to 
 * position it's drawing. To speed up drawing it should test fl_push_clip().
 *
 * Another very useful child is a single Fl_Pack, which is itself a group
 * that packs it's children together and changes size to surround them.
 * Filling the Fl_Pack with Fl_Tabs groups (and then putting normal widgets
 * inside those) gives you a very powerful scrolling list of
 * individually-openable panels.
 *
 * Fluid lets you create these, but you can only lay out objects that fit
 * inside the Fl_Scroll without scrolling. Be sure to leave space for the
 * scrollbars, as Fluid won't show these either.
 *
 * <i>You cannot use Fl_Window as a child of this since the clipping is not
 * conveyed to it when drawn, and it will draw over the scrollbars and 
 * neighboring objects.</i> 
 *
 * Use <tt>Fl_Scroll::scrollbar.align(int)</tt> and 
 * <tt>Fl_Scroll::hscrollbar.align(int)</tt> to change what side the
 * scrollbars are drawn on. If the \c FL_ALIGN_LEFT bit is on, the vertical
 * scrollbar is on the left. If the \c FL_ALIGN_TOP bit is on, the horizontal
 * scrollbar is on the top.
 */
class FL_EXPORT Fl_Scroll : public Fl_Group {

  int xposition_, yposition_;
  int width_, height_;
  int oldx, oldy;
  static void hscrollbar_cb(Fl_Widget*, void*);
  static void scrollbar_cb(Fl_Widget*, void*);
  void fix_scrollbar_order();
  static void draw_clip(void*,int,int,int,int);
  void bbox(int&,int&,int&,int&);

protected:

  void draw();

public:

  Fl_Scrollbar scrollbar;
  Fl_Scrollbar hscrollbar;

  void resize(int x,int y,int width,int height);
  int handle(int event);

    /** Creates a new Fl_Scroll widget using the given position, size, and
     * label string. The default boxtype is \c FL_NO_BOX. */
  Fl_Scroll(int X,int Y,int W,int H,const char*l=0);

    /** Values for type(). By default you can scroll in both directions, and
     * the scrollbars disappear if the data will fit in the area of the
     * scroll. type() can change this. */
  enum {
    HORIZONTAL = 1,        ///< Only a horizontal scrollbar.
    VERTICAL = 2,          ///< Only a vertical scrollbar
    BOTH = 3,              ///< The default is both scrollbars
    ALWAYS_ON = 4,         
    HORIZONTAL_ALWAYS = 5, ///< Horizontal scrollbar always on, vertical always off
    VERTICAL_ALWAYS = 6,   ///< Vertical scrollbar always on, horizontal always off
    BOTH_ALWAYS = 7        ///< Both always on
  };

    /** Gets the current horizontal scrolling position. */
  int xposition() const {return xposition_;}
    /** Gets the current vertical scrolling position. */
  int yposition() const {return yposition_;}
    /** Sets the upper-lefthand corner of the scrolling region. */
  void position(int x, int y);
  void clear();
};

#endif

//
// End of "$Id: Fl_Scroll.H,v 1.5.2.3.2.2.2.2 2003/11/17 07:15:16 matthiaswm Exp $".
//
