//
// "$Id$"
//

#ifndef Fltk1_Tooltip_H
#define Fltk1_Tooltip_H

#include <fltk3/Tooltip.h>

#include "Fl.H"
#include "Fl_Widget.H"

class FL_EXPORT Fl_Tooltip {

public:
  static float delay() { return fltk3::Tooltip::delay(); }
  static void delay(float f) { fltk3::Tooltip::delay(f); }
  static float hoverdelay() { return fltk3::Tooltip::hoverdelay(); }
  static void hoverdelay(float f) { fltk3::Tooltip::hoverdelay(f); }
  static int enabled() { return fltk3::Tooltip::enabled(); }
  static void enable(int b = 1) { fltk3::Tooltip::enable(b); }
  static void disable() { fltk3::Tooltip::disable(); }
  // fltk123: static void (*enter)(fltk3::Widget* w);
  static void enter_area(Fl_Widget *w, int X, int Y, int W, int H, const char* tip)
  { fltk3::Tooltip::enter_area(w->fltk3Widget(), X, Y, W, H, tip); }
  // fltk123: static void (*exit)(fltk3::Widget *w);
  static Fl_Widget* current() { return (Fl_Widget*)fltk3::Tooltip::current()->wrapper(); }
  static void current(Fl_Widget *w) { fltk3::Tooltip::current(w->fltk3Widget()); }
  static Fl_Font font() { return fltk3::Tooltip::font(); }
  static void font(Fl_Font i) { fltk3::Tooltip::font(i); }
  static Fl_Fontsize size() { return fltk3::Tooltip::size(); }
  static void size(Fl_Fontsize s) { fltk3::Tooltip::size(s); }
  static Fl_Color color() { return fltk3::Tooltip::color(); }
  static void color(Fl_Color c) { fltk3::Tooltip::color(c); }
  static Fl_Color textcolor() { return fltk3::Tooltip::textcolor(); }
  static void textcolor(Fl_Color c) { fltk3::Tooltip::textcolor(c); }
};

#endif

//
// End of "$Id$".
//
