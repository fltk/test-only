//
// "$Id$"
//

#ifndef Fltk1_Double_Window_H
#define Fltk1_Double_Window_H

#include <fltk3/DoubleBufferWindow.h>

#include "Fl_Window.H"

class FL_EXPORT Fl_Double_Window : public Fl_Window {
protected:
  void flush(int eraseoverlay) { ((fltk3::DoubleBufferWindow*)_p)->flush(eraseoverlay); }
public:
  void show() { ((fltk3::DoubleBufferWindow*)_p)->show(); }
  void show(int a, char **b) { ((fltk3::DoubleBufferWindow*)_p)->show(a, b); }
  void flush() { ((fltk3::DoubleBufferWindow*)_p)->flush(); }
  void resize(int x, int y, int w, int h) { ((fltk3::DoubleBufferWindow*)_p)->resize(x, y, w, h); }
  void hide() { ((fltk3::DoubleBufferWindow*)_p)->hide(); }
  ~Fl_Double_Window() {
  }
  Fl_Double_Window(int W, int H, const char *l = 0) {
    _p = new fltk3::DoubleBufferWindow(W, H, l); 
    _p->wrapper(this);
    begin();
  }
  Fl_Double_Window(int X, int Y, int W, int H, const char *l = 0) {
    _p = new fltk3::DoubleBufferWindow(X, Y, W, H, l); 
    _p->wrapper(this);
    begin();
  }
};

#endif

//
// End of "$Id$".
//
