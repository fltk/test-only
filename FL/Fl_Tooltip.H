//
// "$Id: Fl_Tooltip.H,v 1.16.2.8.2.5 2004/03/18 08:00:35 matthiaswm Exp $"
//
// Tooltip header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Tooltip_H
#define Fl_Tooltip_H

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>

/** The Fl_Tooltip class provides tooltip support for all FLTK widgets. */
class FL_EXPORT Fl_Tooltip {
public:
    /** Gets the tooltip delay. The default delay is 1.0 seconds. */
  static float delay() { return delay_; }
    /** Sets the tooltip delay. The default delay is 1.0 seconds. */
  static void delay(float f) { delay_ = f; }
  static float hoverdelay() { return hoverdelay_; }
  static void hoverdelay(float f) { hoverdelay_ = f; }
    /** Returns non-zero if tooltips are enabled. */
  static int enabled() { return enabled_; }
    /** Enables tooltips on all widgets (or disables if b is false). */
  static void enable(int b = 1) { enabled_ = b;}
    /** Same as enable(0), disables tooltips on all widgets. */
  static void disable() { enabled_ = 0; }
    /** This method is called when the mouse pointer enters a widget. */
  static void (*enter)(Fl_Widget* w);
    /** You may be able to use this to provide tooltips for internal pieces
     * of your widget. Call this after setting Fl::belowmouse() to your
     * widget (because that calls the above enter() method). Then figure
     * out what thing the mouse is pointing at, and call this with the
     * widget (this pointer is used to remove the tooltip if the widget is
     * deleted or hidden, and to locate the tooltip), the rectangle
     * surrounding the area, relative to the top-left corner of the widget
     * (used to calculate where to put the tooltip), and the text of the
     * tooltip (which must be a pointer to static data as it is not copied). */
  static void enter_area(Fl_Widget* w, int X, int Y, int W, int H, const char* tip);
    /** This method is called when the mouse pointer leaves a widget. */
  static void (*exit)(Fl_Widget *w);
  static Fl_Widget* current() {return widget_;}
  static void current(Fl_Widget*);

    /** Gets the typeface for the tooltip text. */
  static int font() { return font_; }
    /** Gets the size of the tooltip text. */
  static int size() { return size_; }
    /** Sets the typeface for the tooltip text. */
  static void font(int i) { font_ = i; }
    /** Sets the size of the tooltip text. */
  static void size(int s) { size_ = s; }
    /** Sets the background color for tooltips. The default background color is a pale yellow. */
  static void color(unsigned c) { color_ = c; }
    /** Gets the background color for tooltips. */
  static Fl_Color color() { return (Fl_Color)color_; }
    /** Sets the color of the text in the tooltip. The default is black. */
  static void textcolor(unsigned c) { textcolor_ = c; }
    /** Gets the color of the text in the tooltip. */
  static Fl_Color textcolor() { return (Fl_Color)textcolor_; }
private:
  static float delay_;
  static float hoverdelay_;
  static int enabled_;
  static unsigned color_;
  static unsigned textcolor_;
  static int font_;
  static int size_;
  static Fl_Widget* widget_;
};

#endif

//
// End of "$Id: Fl_Tooltip.H,v 1.16.2.8.2.5 2004/03/18 08:00:35 matthiaswm Exp $".
//
