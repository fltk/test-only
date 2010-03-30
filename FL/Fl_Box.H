//
// "$Id$"
//
// Box compatibility header file for the Fast Light Tool Kit (FLTK).
//

#ifndef Fltk1_Box_H
#define Fltk1_Box_H

#include "Fl_Widget.H"

class FL_EXPORT Fl_Box : public Fl_Widget {
public:
  Fl_Box(int X, int Y, int W, int H, const char *l=0)
  : Fl_Widget(X,Y,W,H,l) {}
  
  Fl_Box(Fl_Boxtype b, int X, int Y, int W, int H, const char *l)
  : Fl_Widget(X,Y,W,H,l) {box(b);}
};

#endif

//
// End of "$Id$".
//
