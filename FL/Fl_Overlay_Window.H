#ifndef Fl_Overlay_Window_H
#define Fl_Overlay_Window_H

#include "Fl_Window.H"
#include <fltk/OverlayWindow.h>

class Fl_Overlay_Window : public fltk::OverlayWindow {

public:
  Fl_Overlay_Window(int x, int y, int w, int h, const char*l = 0)
    : fltk::OverlayWindow(x,y,w,h,l) {begin();}
    
  Fl_Overlay_Window(int x, int y, const char*l = 0)
    : fltk::OverlayWindow(x,y,l) {begin();}

};

#endif

