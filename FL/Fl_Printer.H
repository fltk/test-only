//
// "$Id$"
//
// Printing support for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper started
//
// Copyright 2010-2011 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifndef Fl_Printer_H
#define Fl_Printer_H

#include <fltk3/Printer.h>
#include <FL/x.H>
#include <FL/Fl_Paged_Device.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Bitmap.H>
#include <stdio.h>
#if !(defined(__APPLE__) || defined(WIN32))
#include <FL/Fl_PostScript.H>
#elif defined(WIN32)
#include <commdlg.h>
#endif


class Fl_Printer : public Fl_Paged_Device {
  
public:
  
  const char *class_name() {
    return ((fltk3::Printer*)_p)->class_name();
  }

  Fl_Printer() {
    _p = new fltk3::Printer();
    _p->wrapper(this);
  }
  
  int start_job(int pagecount, int *frompage = NULL, int *topage = NULL) {
    return ((fltk3::Printer*)_p)->start_job(pagecount, frompage, topage);
  }
  
  int start_page(void) {
    return ((fltk3::Printer*)_p)->start_page();
  }
  
  int printable_rect(int *w, int *h) {
    return ((fltk3::Printer*)_p)->printable_rect(w, h);
  }
  
  void margins(int *left, int *top, int *right, int *bottom) {
    ((fltk3::Printer*)_p)->margins(left, top, right, bottom);
  }
  
  void origin(int *x, int *y) {
    ((fltk3::Printer*)_p)->origin(x, y);
  }
  
  void origin(int x, int y) {
    ((fltk3::Printer*)_p)->origin(x, y);
  }
  
  void scale(float scale_x, float scale_y = 0.) {
    ((fltk3::Printer*)_p)->scale(scale_x, scale_y);
  }
  
  void rotate(float angle) {
    ((fltk3::Printer*)_p)->rotate(angle);
  }
  
  void translate(int x, int y) {
    ((fltk3::Printer*)_p)->translate(x, y);
  }
  
  void untranslate(void) {
    ((fltk3::Printer*)_p)->untranslate();
  }
  
  int end_page (void) {
    return ((fltk3::Printer*)_p)->end_page();
  }
  
  void end_job (void) {
    end_job();
  }
  
  void print_widget(Fl_Widget* widget, int delta_x=0, int delta_y=0) {
    ((fltk3::Printer*)_p)->print_widget(fltk3::_1to3_widget(widget), delta_x, delta_y);
  }
  
  void print_window_part(Fl_Window *win, int x, int y, int w, int h, int delta_x=0, int delta_y=0) {
    ((fltk3::Printer*)_p)->print_window_part(fltk3::_1to3_window(win), x, y, w, h, delta_x, delta_y);
  }
  
  void set_current(void) {
    ((fltk3::Printer*)_p)->set_current();
  }
  
  Fl_Graphics_Driver* driver(void) {
    return (Fl_Graphics_Driver*)((fltk3::Printer*)_p)->driver();
  }

#if 0 // can't wrap!
  static const char *dialog_title; 
  static const char *dialog_printer; 
  static const char *dialog_range; 
  static const char *dialog_copies; 
  static const char *dialog_all; 
  static const char *dialog_pages;
  static const char *dialog_from;
  static const char *dialog_to;
  static const char *dialog_properties;
  static const char *dialog_copyNo;
  static const char *dialog_print_button;
  static const char *dialog_cancel_button;
  static const char *dialog_print_to_file;
  static const char *property_title;
  static const char *property_pagesize;
  static const char *property_mode;
  static const char *property_use;
  static const char *property_save;
  static const char *property_cancel;
#endif
};


#endif // Fl_Printer_H

//
// End of "$Id$"
//
