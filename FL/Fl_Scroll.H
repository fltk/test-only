/
// "$Id$
/
// Scroll header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Scroll_
#define Fl_Scroll_

#include "Fl_Group.H
#include "Fl_Scrollbar.H

/** This container widget lets you maneuver around a set of widgets muc
 * larger than your window. If the child widgets are larger than th
 * size of this object then scrollbars will appear so that you can scrol
 * over to them
 *   \image html Fl_Scroll.gi
 * If all of the child widgets are packed together into a solid rectangle
 * then you want to setbox() to \c FL_NO_BOX or one of the \c _FRAME types.
 * This will result in the best output. However, if the child widgets are 
 * sparse arrangment you must set box() to a real <tt>_BOX</tt>type. Thi
 * can result in some blinking during redrawing, but that can be solved b
 * using a Fl_Double_Window
 
 * This widget can also be used to pan around a single child widget
 * "canvas". This child widget should be of your own class, with a draw()
 * method that draws the contents. The scrolling is done by changing the
 * x() and y() of the widget, so this child must use the x() and y() to
 * position it's drawing. To speed up drawing it should test fl_push_clip()
 
 * Another very useful child is a single Fl_Pack, which is itself a grou
 * that packs it's children together and changes size to surround them
 * Filling the Fl_Pack with Fl_Tabs groups (and then putting normal widget
 * inside those) gives you a very powerful scrolling list o
 * individually-openable panels
 
 * Fluid lets you create these, but you can only lay out objects that fi
 * inside the Fl_Scroll without scrolling. Be sure to leave space for th
 * scrollbars, as Fluid won't show these either
 
 * <i>You cannot use Fl_Window as a child of this since the clipping is no
 * conveyed to it when drawn, and it will draw over the scrollbars and
 * neighboring objects.</i>
 
 * Use <tt>Fl_Scroll::scrollbar.align(int)</tt> and
 * <tt>Fl_Scroll::hscrollbar.align(int)</tt> to change what side th
 * scrollbars are drawn on. If the \c FL_ALIGN_LEFT bit is on, the vertica
 * scrollbar is on the left. If the \c FL_ALIGN_TOP bit is on, the horizonta
 * scrollbar is on the top
 *
class FL_EXPORT Fl_Scroll : public Fl_Group 

  int xposition_, yposition_
  int width_, height_
  int oldx, oldy
  static void hscrollbar_cb(Fl_Widget*, void*)
  static void scrollbar_cb(Fl_Widget*, void*)
  void fix_scrollbar_order()
  static void draw_clip(void*,int,int,int,int)
  void bbox(int&,int&,int&,int&)

protected

  void draw()

public

  Fl_Scrollbar scrollbar
  Fl_Scrollbar hscrollbar

  void resize(int x,int y,int width,int height)
  int handle(int event)

    /** Creates a new Fl_Scroll widget using the given position, size, an
     * label string. The default boxtype is \c FL_NO_BOX. *
  Fl_Scroll(int X,int Y,int W,int H,const char*l=0)

    /** Values for type(). By default you can scroll in both directions, an
     * the scrollbars disappear if the data will fit in the area of th
     * scroll. type() can change this. *
  enum 
    HORIZONTAL = 1,        ///< Only a horizontal scrollbar
    VERTICAL = 2,          ///< Only a vertical scrollba
    BOTH = 3,              ///< The default is both scrollbar
    ALWAYS_ON = 4,        
    HORIZONTAL_ALWAYS = 5, ///< Horizontal scrollbar always on, vertical always of
    VERTICAL_ALWAYS = 6,   ///< Vertical scrollbar always on, horizontal always of
    BOTH_ALWAYS = 7        ///< Both always o
  }

    /** Gets the current horizontal scrolling position. *
  int xposition() const {return xposition_;
    /** Gets the current vertical scrolling position. *
  int yposition() const {return yposition_;
    /** Sets the upper-lefthand corner of the scrolling region. *
  void position(int x, int y)
  void clear()
}

#endi

/
// End of "$Id$"
/
