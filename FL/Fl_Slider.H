#ifndef Fl_Slider_H
#define Fl_Slider_H

#include "Fl_Widget.H"
#define FLTK_1_SLIDER 1
#include <fltk/Slider.h>

typedef fltk::Slider Fl_Slider;

enum {
  FL_VERT_SLIDER	= fltk::Slider::VERTICAL,
  FL_HOR_SLIDER		= fltk::Slider::HORIZONTAL,
  FL_VERT_FILL_SLIDER	= fltk::Slider::VERTICAL+fltk::Slider::FILL,
  FL_HOR_FILL_SLIDER	= fltk::Slider::HORIZONTAL+fltk::Slider::FILL,
  FL_VERT_NICE_SLIDER	= fltk::Slider::VERTICAL,
  FL_HOR_NICE_SLIDER	= fltk::Slider::HORIZONTAL
};

#endif

