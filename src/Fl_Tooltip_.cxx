// This file contains the parts of Fl_Tooltip.H that are always linked
// in.  Calling Fl_Widget::tooltip(const char*) will change the function
// pointers below and link in the tooltip popup code.

// It is also possible to change the function pointers to your own code
// if you want to display tooltips in your own way such as in a field
// in your main window.

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Tooltip.H>

static void nada(Fl_Widget*) {} // dummy place-holder function

void (*fl_tooltip_enter)(Fl_Widget *) = nada;
void (*fl_tooltip_exit)(Fl_Widget *) = nada;
