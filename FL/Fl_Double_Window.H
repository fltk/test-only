#ifndef Fl_Double_Window_H
#define Fl_Double_Window_H

#include "Fl_Window.H"
#include <fltk/DoubleBufferWindow.h>

class Fl_Double_Window : public fltk::DoubleBufferWindow {

public:
  Fl_Double_Window(int x, int y, int w, int h, const char*l = 0)
    : fltk::DoubleBufferWindow(x,y,w,h,l) {begin();}
    
  Fl_Double_Window(int x, int y, const char*l = 0)
    : fltk::DoubleBufferWindow(x,y,l) {begin();}

};

#endif

